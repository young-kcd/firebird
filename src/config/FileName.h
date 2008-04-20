#ifndef _FILENAME_H_
#define _FILENAME_H_

#include "JString.h"

class FileName
{
public:
	FileName(JString name);
	~FileName();
	
	JString		pathName;
	JString		directory;
	JString		root;
	JString		extension;
	bool        isAbsolute() const { return absolute; }
private:
	bool		absolute;
};

#endif

