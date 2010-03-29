#include "firebird.h"
#include "FileName.h"

#ifdef WIN_NT
#define IS_SLASH(c)	(c == '/' || c == '\\')
#else
#define IS_SLASH(c)	(c == '/')
#endif

FileName::FileName(JString name)
{
	pathName = name;
	const char *start = pathName;
	const char *slash = NULL;
	const char *dot = NULL;
	const char *rootName = start;
	absolute = IS_SLASH (start [0]);

	for (const char	*p = start; *p; ++p)
		if (!dot && IS_SLASH (*p))
			slash = p;
		else if (*p == '.')
			dot = p;
	
	
	if (slash)
		{
		directory.setString (start, (int) (slash - rootName));
		rootName = slash + 1;
		}
	
	if (dot)
		{
		extension = dot + 1;
		root.setString (rootName, (int) (dot - rootName));
		}
	else
		root = rootName;	
}

FileName::~FileName(void)
{
}
