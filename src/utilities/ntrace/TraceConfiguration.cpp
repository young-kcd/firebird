/*
 *	PROGRAM:	Firebird Trace Services
 *	MODULE:		TraceConfiguration.h
 *	DESCRIPTION:	Trace Configuration Reader
 *
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
 *  The Original Code was created by Khorsun Vladyslav
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2009 Khorsun Vladyslav <hvlad@users.sourceforge.net>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 */

#include "TraceConfiguration.h"

using namespace Firebird;

void TraceCfgReader::readTraceConfiguration(const char* text, 
		const PathName& databaseName, 
		TracePluginConfig& config)
{
	TraceCfgReader cfgReader(text, databaseName, config);
	cfgReader.readConfig();
}


#define PATH_PARAMETER(NAME, VALUE) \
	if (!found && el->name == #NAME) { \
		string temp = el->getAttributeName(0); \
		expandPattern(temp); \
		m_config.NAME = temp.c_str(); \
		found = true; \
	}
#define STR_PARAMETER(NAME, VALUE) \
	if (!found && el->name == #NAME) { \
		m_config.NAME = el->getAttributeName(0); \
		found = true; \
	}
#define BOOL_PARAMETER(NAME, VALUE) \
	if (!found && el->name == #NAME) { \
		m_config.NAME = parseBoolean(el->getAttributeName(0)); \
		found = true; \
	}
#define UINT_PARAMETER(NAME, VALUE) \
	if (!found && el->name == #NAME) { \
		m_config.NAME = parseUInteger(el->getAttributeName(0)); \
		found = true; \
	}

void TraceCfgReader::readConfig()
{
	Firebird::AutoPtr<ConfigFile> cfgFile = new ConfigFile(Lex::LEX_none);
	cfgFile->addText(m_text);
	cfgFile->parse();

	m_subpatterns[0].rm_so = 0;
	m_subpatterns[0].rm_eo = m_databaseName.length();
	for (size_t i = 1; i < FB_NELEM(m_subpatterns); i++) 
	{
		m_subpatterns[i].rm_so = -1;
		m_subpatterns[i].rm_eo = -1;
	}

	bool defDB = false, defSvc = false, exactMatch = false;
	const Element *section = cfgFile->getObjects()->children;
	for (; section && !exactMatch; section = section->sibling)
	{
		const bool isDatabase = (section->name == "database");
		if (!isDatabase && section->name != "services")
			continue;

		string pattern = section->getAttributes() ? section->getAttributeName(0) : "";
		bool match = false;
		if (pattern.empty())
		{
			if (isDatabase) 
			{
				if (defDB) {
					fatal_exception::raiseFmt("line %d: second default database section is not allowed", 
						section->lineNumber + 1);
				}

				match = !m_databaseName.empty();
				defDB = true; 
			}
			else 
			{
				if (defSvc) {
					fatal_exception::raiseFmt("line %d: second default service section is not allowed", 
						section->lineNumber + 1);
				}
				match = m_databaseName.empty();
				defSvc = true; 
			}
		}
		else if (isDatabase && !m_databaseName.empty())
		{
			if (m_databaseName == pattern.c_str()) {
				match = exactMatch = true;
			}
			else
			{
				regex_t matcher;
				int flags = REG_EXTENDED;
				if (!CASE_SENSITIVITY)
					flags |= REG_ICASE;
				int errorCode = regcomp(&matcher, pattern.c_str(), flags);

				if (errorCode) 
				{
					char errBuf[256];
					regerror(errorCode, NULL, errBuf, sizeof(errBuf));
					fatal_exception::raiseFmt(
						"line %d: error \"%s\" while compiling regular expression \"%s\"", 
						section->lineNumber + 1, errBuf, pattern.c_str());
				}

				errorCode = regexec(&matcher, m_databaseName.c_str(), FB_NELEM(m_subpatterns), m_subpatterns, 0);

				if (errorCode && errorCode != REG_NOMATCH) 
				{
					char errBuf[256];
					regerror(errorCode, NULL, errBuf, sizeof(errBuf));
					fatal_exception::raiseFmt(
						"line %d: error \"%s\" while applying regular expression \"%s\" to database \"%s\"", 
						section->lineNumber + 1, errBuf, pattern.c_str(), m_databaseName.c_str());
				}

				if (errorCode == 0)
					match = exactMatch = true;
				}
		}

		if (!match)
			continue;

		const Element *el = section->children;
		for (; el; el = el->sibling)
		{
			if (!el->getAttributes()) {
				fatal_exception::raiseFmt("line %d: element \"%s\" have no attribute value set", 
					el->lineNumber + 1, el->name.c_str());
			}

			bool found = false;
			if (isDatabase)
			{
#define DATABASE_PARAMS
				#include "paramtable.h"
#undef DATABASE_PARAMS
			}
			else
			{
#define SERVICE_PARAMS
				#include "paramtable.h"
#undef SERVICE_PARAMS
			}

			if (!found) {
				fatal_exception::raiseFmt("line %d: element \"%s\" is unknown", 
					el->lineNumber + 1, el->name.c_str());
			}
		}
	}
}

#undef PATH_PARAMETER
#undef STR_PARAMETER
#undef BOOL_PARAMETER
#undef UINT_PARAMETER

bool TraceCfgReader::parseBoolean(const string& value) const 
{
	string tempValue = value;
	tempValue.upper();
	if (value == "1" || tempValue == "ON" || tempValue == "YES" || tempValue == "TRUE")
		return true;
	if (value == "0" || tempValue == "OFF" || tempValue == "NO" || tempValue == "FALSE")
		return false;

	fatal_exception::raiseFmt("\"%s\" is not a valid boolean value", value.c_str());
	return false; // Silence the compiler
}

ULONG TraceCfgReader::parseUInteger(const string& value) const 
{
	ULONG result = 0;
	if (!sscanf(value.c_str(), "%"ULONGFORMAT, &result)) {
		fatal_exception::raiseFmt("\"%s\" is not a valid integer value", value.c_str());
	}
	return result;
}

void TraceCfgReader::expandPattern(string& valueToExpand) 
{
	string::size_type pos = 0;
	while (pos < valueToExpand.length()) 
	{
		string::char_type c = valueToExpand[pos];
		if (c == '$') 
		{
			if (pos + 1 >= valueToExpand.length()) {
				fatal_exception::raiseFmt("pattern is invalid");
			}
			
			c = valueToExpand[pos+1];
			if (c == '$') 
			{
				// Kill one of the dollar signs and loop again
				valueToExpand.erase(pos, 1);
				continue;
			}
			
			if (c >= '0' && c <= '9') 
			{
				regmatch_t *subpattern = m_subpatterns + (c - '0');
				// Replace value with piece of database name
				valueToExpand.erase(pos, 2);
				if (subpattern->rm_eo != -1 && subpattern->rm_so != -1) 
				{
					off_t subpattern_len = subpattern->rm_eo - subpattern->rm_so;
					valueToExpand.insert(pos, 
						m_databaseName.substr(subpattern->rm_so, subpattern_len).c_str(),
						subpattern_len);
					pos += subpattern_len;
				}
				continue;
			}

			if (c == '&') 
			{
				// Replace value with whole database file name
				valueToExpand.erase(pos, 2);
				const Firebird::PathName& filename = m_databaseName;
				valueToExpand.insert(pos, filename.c_str(), filename.length());
				pos += filename.length();
				continue;
			}
			fatal_exception::raiseFmt("pattern is invalid");
		}
		pos++;
	}
}

