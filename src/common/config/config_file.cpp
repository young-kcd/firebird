/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		config_file.cpp
 *	DESCRIPTION:	Configuration manager (file handling)
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
 * The Original Code was created by Inprise Corporation
 * and its predecessors. Portions created by Inprise Corporation are
 * Copyright (C) Inprise Corporation.
 *
 * Created by: Mark O'Donohue <mark.odonohue@ludwig.edu.au>
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "../../jrd/ib_stdio.h"

#include "../../common/config/config_file.h"
#include "../../common/classes/auto.h"
#include "../jrd/os/fbsyslog.h"


#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

// Invalid or missing CONF_FILE may lead to severe errors
// in applications. That's why for regular SERVER builds
// it's better to exit with appropriate diags rather continue
// with missing / wrong configuration.
#if (! defined(BOOT_BUILD)) && (! defined(EMBEDDED)) && (! defined(SUPERCLIENT))
#define EXIT_ON_NO_CONF
#define INFORM_ON_NO_CONF
#else
#undef EXIT_ON_NO_CONF
#undef INFORM_ON_NO_CONF
#endif

typedef Firebird::string string;

/******************************************************************************
 *
 *	Allow case-insensitive comparison
 */

bool ConfigFile::key_compare::operator()(const string& x, const string& y) const
{
	return Firebird::PathName(x) < Firebird::PathName(y);
}

/******************************************************************************
 *
 *	Strip leading spaces
 */

void ConfigFile::stripLeadingWhiteSpace(string& s)
{
	if (!s.size())
	{
		return;
	}

	const string::size_type startPos = s.find_first_not_of(" \t\r");
	if (startPos == string::npos)
	{
		s.erase();	// nothing but air
	}
	else if (startPos)
	{
		s = s.substr(startPos);
	}
}

/******************************************************************************
 *
 *	Strip trailing spaces
 */

void ConfigFile::stripTrailingWhiteSpace(string& s)
{
	if (!s.size())
	{
		return;
	}

	string::size_type endPos = s.find_last_not_of(" \t\r");
	if (endPos != string::npos)
	{
		// Note that endPos is the index to the last non-ws char
		// why we have to inc. it
		++endPos;
		s = s.substr(0, endPos);
	}
}

/******************************************************************************
 *
 *	Strip any comments
 */

void ConfigFile::stripComments(string& s)
{
	// Note that this is only a hack. It won't work in case inputLine
	// contains hash-marks embedded in quotes! Not that I know if we
	// should care about that case.
	const string::size_type commentPos = s.find('#');
	if (commentPos != string::npos)
	{
		s = s.substr(0, commentPos);
	}
}

/******************************************************************************
 *
 *	Check whether the given key exists or not
 */

bool ConfigFile::doesKeyExist(const string& key)
{
    checkLoadConfig();

    string data = getString(key);

    return !data.empty();
}

/******************************************************************************
 *
 *	Return string value corresponding the given key
 */

string ConfigFile::getString(const string& key)
{
    checkLoadConfig();

    mymap_t::iterator lookup;

    lookup = parameters.find(key);

    if (lookup != parameters.end())
    {
    	return lookup->second;
    }

    return string();
}

/******************************************************************************
 *
 *	Parse key
 */

string ConfigFile::parseKeyFrom(const string& inputLine, string::size_type& endPos)
{
    endPos = inputLine.find_first_of("=\t");
    if (endPos == string::npos)
    {
        return inputLine;
    }

    return inputLine.substr(0, endPos);
}

/******************************************************************************
 *
 *	Parse value
 */

string ConfigFile::parseValueFrom(string inputLine, string::size_type initialPos)
{
    if (initialPos == string::npos)
    {
        return string();
    }

    // skip leading white spaces
    unsigned int startPos = inputLine.find_first_not_of("= \t", initialPos);
    if (startPos == string::npos)
    {
        return string();
    }

    stripTrailingWhiteSpace(inputLine);
    return inputLine.substr(startPos);
}

/******************************************************************************
 *
 *	Load file, if necessary
 */

void ConfigFile::checkLoadConfig()
{
	if (!isLoadedFlg)
	{
    	loadConfig();
	}
}

/******************************************************************************
 *
 *	Load file immediately
 */

namespace {
	class FileClose
	{
	public:
		static void clear(FILE *f)
		{
			if (f) {
				fclose(f);
			}
		}
	};
} // namespace

void ConfigFile::loadConfig()
{
	isLoadedFlg = true;

	parameters.clear();

	// Note we request file in "binary" form. We'll handle line breaks ourselves
	Firebird::AutoPtr<FILE, FileClose> ifile(fopen(configFile.c_str(), "rb"));
	
#ifdef EXIT_ON_NO_CONF
	int BadLinesCount = 0;
#endif
    if (!ifile)
    {
        // config file does not exist
#ifdef INFORM_ON_NO_CONF
		string Msg = "Missing configuration file: " + configFile;
#ifdef EXIT_ON_NO_CONF
		if (fExitOnError)
		{
			Msg += ", exiting";
		}
#endif
		if (fExitOnError) 
		{
			Firebird::Syslog::Record(Firebird::Syslog::Error, Msg);
		}
#ifdef EXIT_ON_NO_CONF
		if (fExitOnError)
			exit(1);
#endif
#endif //INFORM_ON_NO_CONF
		return;
    }
    string inputLine;

	// Variables are placed outside of the loop to work around GCC bug with 
	// debug info generation
	char buffer[100];
	int bytesRead;
	bool prevCR;

    while (true)
    {
		// Nickolay Samofatov, 14-Sep-2004.
		// std::getline doesn't work with vanilla GCC 3.3.2 and 3.3.3 (see GCC/14720)
		// Implement something similar inline (code logic stolen from 
		// Firebird::FileObject in my private Firebird 2.0 tree). Use stdio because 
		// ifstream::readsome is broken in GCC since version 3.1 (still broken in 
		// 3.3.2, not tested further)

		// This code is not very efficient, but is still much better then reading
		// characters one-by-one. Plus it should be able to handle line breaks in
		// Windows, Linux and Macintosh format nicely on all platforms
		inputLine.resize(0);
		prevCR = false;
		do {
			bytesRead = fread(buffer, 1, sizeof(buffer), ifile);
			if (ferror(ifile)) {
			   string Msg = "Error reading config file: " + configFile;
			   Firebird::Syslog::Record(Firebird::Syslog::Error, Msg);
			}
			for (int pos = 0; pos < bytesRead; pos++) {
				switch(buffer[pos]) {
					case '\n':
						// Unix or Windows line break
						inputLine.append(buffer, pos);
						// Adjust file pointer
						fseek(ifile, pos - bytesRead + 1, SEEK_CUR);
						// Kill trailing CR if present (Windows)
						if (prevCR)
							inputLine.resize(inputLine.length() - 1);
						goto line_finished;

					case '\r':
						//  Mac line break or portion of Windows line break
						prevCR = true;
						break;

					default:
						if (prevCR) {
							// Mac line break
							inputLine.append(buffer, pos);
							// Adjust file pointer
							fseek(ifile, pos - bytesRead, SEEK_CUR);
							goto line_finished; 
						}
				}
			}
			inputLine.append(buffer, bytesRead);
		} while (bytesRead == sizeof(buffer));

		// Kill trailing CR if present
		if (prevCR)
			inputLine.resize(inputLine.length() - 1);

		// Check if we reached end of file
		if (!inputLine.length() && !bytesRead)
			break;

line_finished:

		stripComments(inputLine);
		stripLeadingWhiteSpace(inputLine);
		
		if (!inputLine.size())
		{
			continue;	// comment-line or empty line
		}

        if (inputLine.find('=') == string::npos)
        {
            string Msg = configFile + ": illegal line \"" +
				inputLine + "\"";
			Firebird::Syslog::Record(fExitOnError ? 
					Firebird::Syslog::Error :
					Firebird::Syslog::Warning, Msg);
#ifdef EXIT_ON_NO_CONF
			BadLinesCount++;
#endif
            continue;
        }

        string::size_type endPos;

        string key   = parseKeyFrom(inputLine, endPos);
		stripTrailingWhiteSpace(key);
		// TODO: here we must check for correct parameter spelling !
        string value = parseValueFrom(inputLine, endPos);

		// parameters.insert(pair<string, string>(key, value));
		// Just to display yet another template function
        parameters.insert(std::make_pair(key, value));
    }
#ifdef EXIT_ON_NO_CONF
	if (BadLinesCount && fExitOnError) {
		exit(1);
	}
#endif
}
