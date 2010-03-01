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

#ifndef CONFIG_CONFIG_FILE_H
#define CONFIG_CONFIG_FILE_H

#include "../common/classes/alloc.h"
#include "../common/classes/fb_pair.h"
#include "../common/classes/objects_array.h"
#include "../common/classes/fb_string.h"
#include "../common/classes/auto.h"

/**
	Since the original (isc.cpp) code wasn't able to provide powerful and
	easy-to-use abilities to work with complex configurations, a decision
	has been made to create a completely new one.

	The below class implements generic file abstraction for new configuration
	manager. It allows "value-by-key" retrieval based on plain text files. Both
	keys and values are just strings that may be handled case-sensitively or
	case-insensitively, depending on host OS. The configuration file is loaded
	on demand, its current status can be checked with isLoaded() member function.
	All leading and trailing spaces are ignored. Comments (follow after a
	hash-mark) are ignored as well.

	Now this implementation is used by generic configuration manager
	(common/config/config.cpp) and server-side alias manager (jrd/db_alias.cpp).
**/

class ConfigFile : public Firebird::AutoStorage, public Firebird::RefCounted
{
public:
	// flags for config file
	static const USHORT EXCEPTION_ON_ERROR =	0x01;
	static const USHORT HAS_SUB_CONF =			0x02;
	static const USHORT NO_MACRO =				0x04;

	// enum to distinguish ctors
	enum UseText {USE_TEXT};

	// config_file works with OS case-sensitivity
	typedef Firebird::PathName String;

	class Stream
	{
	public:
		virtual ~Stream();
		virtual bool getLine(String&, unsigned int&) = 0;
	};

	struct Parameter : public AutoStorage
	{
		Parameter(MemoryPool& p, const Parameter& par)
			: AutoStorage(p), name(getPool(), par.name), value(getPool(), par.value),
			  sub(par.sub), line(par.line)
		{ }
		Parameter()
			: AutoStorage(), name(getPool()), value(getPool()), sub(0), line(0)
		{ }

		String name;
		String value;
		Firebird::RefPtr<ConfigFile> sub;
		unsigned int line;

		static const String* generate(const void* /*sender*/, const Parameter* item)
		{
			return &item->name;
		}
	};

    typedef Firebird::SortedObjectsArray<Parameter, Firebird::InlineStorage<Parameter*, 100>,
										 String, Parameter> Parameters;

	ConfigFile(const String& file, USHORT fl);
	ConfigFile(const char* file, USHORT fl);
	ConfigFile(UseText, const char* configText, USHORT fl);

private:
	ConfigFile(MemoryPool& p, ConfigFile::Stream* s, USHORT fl, const String& file);

public:
	// key and value management
	const Parameter* findParameter(const String& name) const;
	const Parameter* findParameter(const String& name, const String& value) const;

	// all parameters access
	const Parameters& getParameters() const
	{
		return parameters;
	}

private:
	enum LineType {LINE_BAD, LINE_REGULAR, LINE_START_SUB};

    String configFile;
    Parameters parameters;
	USHORT flags;
	USHORT badLinesCount;

	// utilities
	void parse(Stream* stream);
	LineType parseLine(const String& input, String& key, String& value);
	bool translate(const String& from, String& to);
	void badLine(const String& line);
};

#endif	// CONFIG_CONFIG_FILE_H
