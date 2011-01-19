/*
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Dmitry Yemanov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2002 Dmitry Yemanov <dimitr@users.sf.net>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"

#include "../common/classes/alloc.h"
#include "../common/classes/auto.h"
#include "../common/config/config_file.h"
#include "../common/config/config.h"
#include "../common/os/path_utils.h"
#include <stdio.h>

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

using namespace Firebird;

namespace {

class MainStream : public ConfigFile::Stream
{
public:
	MainStream(const char* fname, PathName& errString)
		: file(fopen(fname, "rt")), l(0)
	{
		if (!file)
		{
			// config file does not exist
			errString.printf("Missing configuration file: %s", fname);
		}
	}

	bool getLine(ConfigFile::String& input, unsigned int& line)
	{
		input = "";
		if (!file)
		{
			return false;
		}

		// this loop efficiently skips almost all comment lines
		do
		{
			if (feof(file))
			{
				return false;
			}
			input.LoadFromFile(file);
			++l;
			input.alltrim(" \t\r");
		} while (input.isEmpty() || input[0] == '#');

		line = l;
		return true;
	}

private:
	AutoPtr<FILE, FileClose> file;
	unsigned int l;
};

class TextStream : public ConfigFile::Stream
{
public:
	explicit TextStream(const char* configText)
		: s(configText), l(0)
	{
		if (s && !*s)
		{
			s = NULL;
		}
	}

	bool getLine(ConfigFile::String& input, unsigned int& line)
	{
		do
		{
			if (!s)
			{
				input = "";
				return false;
			}

			const char* ptr = strchr(s, '\n');
			if (!ptr)
			{
				input.assign(s);
				s = NULL;
			}
			else
			{
				input.assign(s, ptr - s);
				s = ptr + 1;
				if (!*s)
				{
					s = NULL;
				}
			}

			++l;
			input.alltrim(" \t\r");
		} while (input.isEmpty() || input[0] == '#');

		line = l;
		return true;
	}

private:
	const char* s;
	unsigned int l;
};

class SubStream : public ConfigFile::Stream
{
public:
	SubStream()
		: cnt(0)
	{ }

	bool getLine(ConfigFile::String& input, unsigned int& line)
	{
		if (cnt >= data.getCount())
		{
			input = "";
			return false;
		}

		input = data[cnt].first;
		line = data[cnt].second;
		++cnt;

		return true;
	}

	void putLine(const ConfigFile::String& input, unsigned int line)
	{
		data.push(Line(input, line));
	}

private:
	typedef Pair<Left<ConfigFile::String, unsigned int> > Line;
	ObjectsArray<Line> data;
	size_t cnt;
};

} // anonymous namespace


ConfigFile::ConfigFile(const Firebird::PathName& file, USHORT fl)
	: AutoStorage(),
	  configFile(getPool(), file),
	  parameters(getPool()),
	  flags(fl),
	  lastMessage(getPool())
{
	MainStream s(configFile.c_str(), lastMessage);
	parse(&s);
}

ConfigFile::ConfigFile(const char* file, USHORT fl)
	: AutoStorage(),
	  configFile(getPool(), String(file)),
	  parameters(getPool()),
	  flags(fl),
	  lastMessage(getPool())
{
	MainStream s(configFile.c_str(), lastMessage);
	parse(&s);
}

ConfigFile::ConfigFile(UseText, const char* configText, USHORT fl)
	: AutoStorage(),
	  configFile(getPool()),
	  parameters(getPool()),
	  flags(fl),
	  lastMessage(getPool())
{
	TextStream s(configText);
	parse(&s);
}

ConfigFile::ConfigFile(MemoryPool& p, const Firebird::PathName& file, USHORT fl)
	: AutoStorage(p),
	  configFile(getPool(), file),
	  parameters(getPool()),
	  flags(fl),
	  lastMessage(getPool())
{
	MainStream s(configFile.c_str(), lastMessage);
	parse(&s);
}

ConfigFile::ConfigFile(MemoryPool& p, ConfigFile::Stream* s, USHORT fl, const Firebird::PathName& file)
	: AutoStorage(p),
	  configFile(getPool(), file),
	  parameters(getPool()),
	  flags(fl),
	  lastMessage(getPool())
{
	parse(s);
}

ConfigFile::Stream::~Stream()
{
}

/******************************************************************************
 *
 *	Parse line, taking quotes into account
 */

ConfigFile::LineType ConfigFile::parseLine(const String& input, KeyType& key, String& value)
{
	int inString = 0;
	String::size_type valStart = 0;
	String::size_type eol = String::npos;
	bool hasSub = false;

	for (String::size_type n = 0; n < input.length(); ++n)
	{
		switch (input[n])
		{
		case '"':
			if (key.isEmpty())		// quoted string to the left of = doesn't make sense
				return LINE_BAD;
			if (inString >= 2)		// one more quote after quoted string doesn't make sense
				return LINE_BAD;
			inString++;
			break;

		case '=':
			key = input.substr(0, n).ToNoCaseString();
			key.rtrim(" \t\r");
			if (key.isEmpty())		// not good - no key
				return LINE_BAD;
			valStart = n + 1;
			break;

		case '#':
			if (inString != 1)
			{
				eol = n;
				n = input.length();	// skip the rest of symbols
			}
			break;

		case ' ':
		case '\t':
		case '\r':
			break;

		case '{':
		case '}':
			if (flags & HAS_SUB_CONF)
			{
				if (inString != 1)
				{
					if (input[n] == '}')	// Subconf close mark not expected
					{
						return LINE_BAD;
					}

					hasSub = true;
					inString = 2;
					eol = n;
				}
				break;
			}
			// fall through ....

		default:
			if (inString >= 2)		// Something after the end of line
				return LINE_BAD;
			break;
		}
	}

	if (inString == 1)				// If we are still inside a string, it's error
		return LINE_BAD;

	if (key.isEmpty())
	{
		key = input.substr(0, eol).ToNoCaseString();
		key.rtrim(" \t\r");
		value.erase();
	}
	else
	{
		value = input.substr(valStart, eol - valStart);
		value.alltrim(" \t\r");
		value.alltrim("\"");
	}

	// Now expand macros in value
	String::size_type subFrom;
	while ((subFrom = value.find("$(")) != String::npos)
	{
		String::size_type subTo = value.find(")", subFrom);
		if (subTo != String::npos)
		{
			String macro;
			String m = value.substr(subFrom + 2, subTo - (subFrom + 2));
			if (! translate(m, macro))
			{
				return LINE_BAD;
			}
			value.replace(subFrom, subTo + 1 - subFrom, macro);
		}
		else
		{
			return LINE_BAD;
		}
	}

	return hasSub ? LINE_START_SUB : LINE_REGULAR;
}

/******************************************************************************
 *
 *	Find macro value
 */

bool ConfigFile::translate(const String& from, String& to)
{
	if (from == "root")
	{
		to = Config::getRootDirectory();
	}
	else if (from == "install")
	{
		to = Config::getInstallDirectory();
	}
	else if (from == "this")
	{
		if (configFile.isEmpty())
		{
			return false;
		}

		PathName path, file;
		PathUtils::splitLastComponent(path, file, configFile.ToPathName());
		to = path.ToString();
	}
	/* ToDo - implement this feature
	else if (!substituteOneOfStandardFirebirdDirs(from, to))
	{
		return false;
	}
	*/
	else
	{
		return false;
	}

	return true;
}

/******************************************************************************
 *
 *	Return parameter corresponding the given key
 */

const ConfigFile::Parameter* ConfigFile::findParameter(const KeyType& name) const
{
	size_t pos;
	return parameters.find(name, pos) ? &parameters[pos] : NULL;
}

/******************************************************************************
 *
 *	Return parameter corresponding the given key and value
 */

const ConfigFile::Parameter* ConfigFile::findParameter(const KeyType& name, const String& value) const
{
	size_t pos;
	if (!parameters.find(name, pos))
	{
		return NULL;
	}

	while (pos < parameters.getCount() && parameters[pos].name == name)
	{
		if (parameters[pos].value == value)
		{
			return &parameters[pos];
		}
		++pos;
	}

	return NULL;
}

/******************************************************************************
 *
 *	Take into an account fault line
 */

void ConfigFile::badLine(const String& line)
{
	lastMessage.printf("%s: illegal line <%s>",
					   (configFile.hasData() ? configFile.c_str() : "Passed text"),
					   line.c_str());
}

/******************************************************************************
 *
 *	Load file immediately
 */

void ConfigFile::parse(Stream* stream)
{
	String inputLine;
	Parameter* previous = NULL;
	unsigned int line;

	while (stream->getLine(inputLine, line))
	{
		Parameter current;
		current.line = line;

		switch (parseLine(inputLine, current.name, current.value))
		{
		case LINE_BAD:
			badLine(inputLine);
			return;

		case LINE_REGULAR:
			if (current.name.isEmpty())
			{
				badLine(inputLine);
				return;
			}

			previous = &parameters[parameters.add(current)];
			break;

		case LINE_START_SUB:
			if (current.name.hasData())
			{
				size_t n = parameters.add(current);
				previous = &parameters[n];
			}

			{ // subconf scope
				SubStream subStream;
				while (stream->getLine(inputLine, line))
				{
					if (inputLine[0] == '}')
					{
						String s = inputLine.substr(1);
						s.ltrim(" \t\r");
						if (s.hasData() && s[0] != '#')
						{
							badLine(s);
							return;
						}
						break;
					}
					subStream.putLine(inputLine, line);
				}

				previous->sub = FB_NEW(getPool()) ConfigFile(getPool(), &subStream,
															 flags & ~HAS_SUB_CONF, configFile);
			}
			break;
		}
	}
}

/******************************************************************************
 *
 *	Check for parse/load error
 */

const char* ConfigFile::getMessage() const
{
	return lastMessage.nullStr();
}
