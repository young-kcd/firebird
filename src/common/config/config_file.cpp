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

#include "../../common/classes/alloc.h"
#include "../../common/classes/auto.h"
#include "../../common/config/config_file.h"
#include "../jrd/os/fbsyslog.h"
#include <stdio.h>

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

//#include <fstream>
//#include <iostream>

// Invalid or missing CONF_FILE may lead to severe errors
// in applications. That's why for regular SERVER builds
// it's better to exit with appropriate diags rather continue
// with missing / wrong configuration.
#if (! defined(BOOT_BUILD)) && (! defined(EMBEDDED)) && (! defined(SUPERCLIENT))
#define EXCEPTION_ON_NO_CONF
#else
#undef EXCEPTION_ON_NO_CONF
#endif

// config_file works with OS case-sensitivity
typedef Firebird::PathName string;

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

    size_t pos;
    return parameters.find(key, pos) ? parameters[pos].second : string();
}

/******************************************************************************
 *
 *	Parse key
 */

string ConfigFile::parseKeyFrom(const string& inputLine, string::size_type& endPos)
{
    endPos = inputLine.find_first_of("=");
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

    inputLine.rtrim(" \t\r");
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

	Firebird::AutoPtr<FILE, FileClose> ifile(fopen(configFile.c_str(), "rt"));
	
#ifdef EXCEPTION_ON_NO_CONF
	int BadLinesCount = 0;
#endif
    if (!ifile)
    {
        // config file does not exist
#ifdef EXCEPTION_ON_NO_CONF
		if (fExceptionOnError)
		{
			Firebird::string Msg = "Missing configuration file: " + 
				configFile.ToString() + ", exiting";
			Firebird::Syslog::Record(Firebird::Syslog::Error, Msg);
			Firebird::fatal_exception::raise(Msg.c_str());
		}
#endif //EXCEPTION_ON_NO_CONF
		return;
    }
    string inputLine;

    while (!feof(ifile))
    {
		inputLine.LoadFromFile(ifile);

		stripComments(inputLine);
		inputLine.ltrim(" \t\r");
		
		if (!inputLine.size())
		{
			continue;	// comment-line or empty line
		}

        if (inputLine.find('=') == string::npos)
        {
			Firebird::string Msg = (configFile + ": illegal line \"" +
				inputLine + "\"").ToString();
			Firebird::Syslog::Record(fExceptionOnError ? 
					Firebird::Syslog::Error :
					Firebird::Syslog::Warning, Msg);
#ifdef EXCEPTION_ON_NO_CONF
			BadLinesCount++;
#endif
            continue;
        }

        string::size_type endPos;

        string key   = parseKeyFrom(inputLine, endPos);
		key.rtrim(" \t\r");
		// TODO: here we must check for correct parameter spelling !
        string value = parseValueFrom(inputLine, endPos);

		parameters.add(Parameter(getPool(), key, value));
    }
#ifdef EXCEPTION_ON_NO_CONF
	if (BadLinesCount && fExceptionOnError) 
	{
		Firebird::fatal_exception::raise("Bad lines in firebird.conf");
	}
#endif
}
