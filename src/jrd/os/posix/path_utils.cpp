#include "../jrd/os/path_utils.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

/// The POSIX implementation of the path_utils abstraction.

const char PathUtils::dir_sep = '/';
const Firebird::string PathUtils::up_dir_link = "..";

class PosixDirItr : public PathUtils::dir_iterator
{
public:
	PosixDirItr(const Firebird::string&);
	~PosixDirItr();
	const PosixDirItr& operator++();
	const Firebird::string& operator*() { return file; }
	operator bool() { return !done; }
	
private:
	DIR *dir;
	Firebird::string file;
	int done;
};

PosixDirItr::PosixDirItr(const Firebird::string& path)
	: dir_iterator(path), dir(0), done(0)
{
	dir = opendir(dirPrefix.c_str());
	if (!dir)
		done = 1;
	else
		++(*this);
}

PosixDirItr::~PosixDirItr()
{
	if (dir)
		closedir(dir);
	dir = 0;
	done = 1;
}

const PosixDirItr& PosixDirItr::operator++()
{
	if (done)
		return *this;
	struct dirent *ent = readdir(dir);
	if (ent == NULL)
	{
		done = 1;
	}
	else
	{
		PathUtils::concatPath(file, dirPrefix, ent->d_name);
	}
	return *this;
}

PathUtils::dir_iterator *PathUtils::newDirItr(MemoryPool& p, const Firebird::string& path)
{
	return FB_NEW(p) PosixDirItr(path);
}

void PathUtils::splitLastComponent(Firebird::string& path, Firebird::string& file,
		const Firebird::string& orgPath)
{
	Firebird::string::size_type pos;
	
	pos = orgPath.rfind(dir_sep);
	if (pos == Firebird::string::npos)
	{
		path = "";
		file = orgPath;
		return;
	}
	
	path.erase();
	path.append(orgPath, 0, pos);	// skip the directory separator
	file.erase();
	file.append(orgPath, pos+1, orgPath.length() - pos - 1);
}

void PathUtils::concatPath(Firebird::string& result,
		const Firebird::string& first,
		const Firebird::string& second)
{
	if (second.length() == 0)
	{
		result = first;
		return;
	}
	if (first.length() == 0)
	{
		result = second;
		return;
	}
	
	if (first[first.length()-1] != dir_sep &&
		second[0] != dir_sep)
	{
		result = first + dir_sep + second;
		return;
	}
	if (first[first.length()-1] == dir_sep &&
		second[0] == dir_sep)
	{
		result = first;
		result.append(second, 1, second.length() - 1);
		return;
	}
	
	result = first + second;
}

bool PathUtils::isRelative(const Firebird::string& path)
{
	if (path.length() > 0)
		return path[0] != dir_sep;
	return false;
}

bool PathUtils::isSymLink(const Firebird::string& path)
{
	struct stat st, lst;
	if (stat(path.c_str(), &st) != 0)
		return false;
	if (lstat(path.c_str(), &lst) != 0)
		return false;
	return st.st_ino != lst.st_ino;
}

bool PathUtils::comparePaths(const Firebird::string& path1, 
 							 const Firebird::string& path2) {
 	return path1 == path2;
}
