/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		dir_list.cpp
 *	DESCRIPTION:	Directory listing config file operation
 *
 * The contents of this file are subject to the Interbase Public
 * License Version 1.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy
 * of the License at http://www.Inprise.com/IPL.html
 *
 * Software distributed under the License is distributed on an
 * "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
 * or implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * Created by: Alex Peshkov <AlexPeshkov@users.sourceforge.net>
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "../common/config/config.h"
#include "../common/config/dir_list.h"
#include "../jrd/os/path_utils.h"

#define New FB_NEW(*getDefaultMemoryPool())

ParsedPath::ParsedPath(void) {
	PathElem = 0;
	nElem = 0;
}

ParsedPath::ParsedPath(const Firebird::string & path) {
	PathElem = 0;
	Parse(path);
}

ParsedPath::~ParsedPath() {
	delete[] PathElem;
}

void ParsedPath::Parse(const Firebird::string &path) {
	delete PathElem;
	nElem = 0;
	Firebird::string oldpath = path;
	do {
		Firebird::string newpath, elem;
		PathUtils::splitLastComponent(newpath, elem, oldpath);
		oldpath = newpath;
		nElem++;
	} while (oldpath.length() > 0);

	PathElem = New Firebird::string[nElem];
	oldpath = path;
	for (int i=nElem; i--; ) {
		Firebird::string newpath;
		PathUtils::splitLastComponent(newpath, PathElem[i], oldpath);
		oldpath = newpath;
	}
}

bool ParsedPath::operator==(const char * path) {
	return (Firebird::string(*this) == Firebird::string(path));
}

Firebird::string ParsedPath::SubPath(int n) const {
	Firebird::string rc = "/" + PathElem[0];
	for (int i=1; i<n; i++) {
		Firebird::string newpath;
		PathUtils::concatPath(newpath, rc, PathElem[i]);
		rc = newpath;
	}
	return rc;
}

ParsedPath::operator Firebird::string() {
	if (! PathElem)
		return "";
	return SubPath(nElem);
}

bool ParsedPath::Contains(const ParsedPath & pPath) {
	if (pPath.nElem < nElem) {
		return false;
	}
	int i;
	for (i=0; i<nElem; i++) {
		if (! PathUtils::comparePaths(pPath.PathElem[i], PathElem[i])) {
			return false;
		}
	}
	for (i = nElem + 1; i<=pPath.nElem; i++) {
		Firebird::string x = pPath.SubPath(i);
		if (PathUtils::isSymLink(x)) {
			return false;
		}
	}
	return true;
}

DirectoryList::DirectoryList() {
	ConfigDirs = 0;
	nDirs = 0;
}

DirectoryList::~DirectoryList() {
	delete[] ConfigDirs;
}

void DirectoryList::Initialize(void) {
	const Firebird::string val = GetConfigString();
	nDirs = 1;
	int i;
	for (i=0; i<val.length(); i++) {
		if (val[i] == ';') {
			nDirs++;
		}
	}
	ConfigDirs = New ParsedPath[nDirs];
	int Last = 0;
	nDirs = 0;
	Firebird::string Root = Config::getRootDirectory();
	for (i=0; i<val.length(); i++) {
		if (val[i] == ';') {
			Firebird::string dir = "";
			if (i > Last) {
				dir = val.substr(Last, i-Last);
			}
			if (PathUtils::isRelative(dir)) {
				Firebird::string newdir;
				PathUtils::concatPath(newdir, Root, dir);
				dir = newdir;
			}
			ConfigDirs[nDirs++].Parse(dir);
			Last = i + 1;
		}
	}
	Firebird::string dir = "";
	if (i > Last) {
		dir = val.substr(Last, i-Last);
	}
	if (PathUtils::isRelative(dir)) {
		Firebird::string newdir;
		PathUtils::concatPath(newdir, Root, dir);
		dir = newdir;
	}
	ConfigDirs[nDirs++].Parse(dir);
}

bool DirectoryList::IsPathInList(const Firebird::string &path) {
	if (! ConfigDirs)
		Initialize();

	// Disable any up-dir(..) references - in case our path_utils
	// and OS handle paths in slightly different ways,
	// this is "wonderful" potential hole for hacks
	// Example of IIS attack attempt:
	// "GET /scripts/..%252f../winnt/system32/cmd.exe?/c+dir HTTP/1.0"
	//								(live from apache access log :)
	if (path.find(PathUtils::up_dir_link) != Firebird::string::npos)
		return false;

	if (nDirs == 1 && ConfigDirs[0] == "/") // Handle special case - all open
		return true;

	Firebird::string varpath = path;
	if (PathUtils::isRelative(path)) {
		PathUtils::concatPath(varpath, 
			Firebird::string(Config::getRootDirectory()), path);
	}

	ParsedPath pPath = path;
    bool rc = 0;
    for (int i=0; i<nDirs; i++) {
		if (ConfigDirs[i].Contains(pPath)) {
			rc = true;
			break;
		}
	}
	return rc;
}
