/*
 *	PROGRAM:		String parser
 *	MODULE:			Tokens.cpp
 *	DESCRIPTION:	Enhanced variant of strtok()
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
 *  The Original Code was created by Alex Peshkov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2015 Alex Peshkov <peshkoff at mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "../common/Tokens.h"

#include "../common/StatusArg.h"

namespace Firebird {

Tokens::Tokens(FB_SIZE_T length, const char* toParse, const char* spaces, const char* quotes, const Comment* comments)
	: tokens(getPool()), str(getPool())
{
	if (!length)
		length = strlen(toParse);
	str.assign(toParse, length);

	char inStr = 0;
	Tok* inToken = NULL;
	FB_SIZE_T startp = 0;
	FB_SIZE_T origin = 0;

	FB_SIZE_T p = 0;
	while (p < str.length())
	{
		if (comments && !inStr)
		{
			for (const Comment* comm = comments; comm->start; ++comm)
			{
				if (strncmp(comm->start, &str[p], strlen(comm->start)) == 0)
				{
					FB_SIZE_T p2 = p + strlen(comm->start);
					p2 = str.find(comm->stop, p2);
					if (p2 == str.npos)
						error("Missing close comment for %s", comm->start);
					p2 += strlen(comm->stop);
					str.erase(p, p2 - p);
					origin += (p2 - p);
					continue;
				}
			}
		}

		char c = str[p];
		if (inStr)
		{
			if (c == inStr)
			{
				++p;
				++origin;
				if (p >= str.length() || str[p] != inStr)
				{
					inStr = 0;
					inToken->length = p - startp;
					inToken = NULL;
					continue;
				}
				// double quote - continue processing string
			}
			++p;
			++origin;
			continue;
		}

		bool space = spaces && strchr(spaces, c);
		if (space)
		{
			if (inToken)
			{
				inToken->length = p - startp;
				inToken = NULL;
			}
			++p;
			++origin;
			continue;
		}

		bool quote = quotes && strchr(quotes, c);
		if (quote)
		{
			if (inToken)
			{
				inToken->length = p - startp;
				inToken = NULL;
			}
			// start string
			inStr = c;
		}

		if (!inToken)
		{
			// start token
			startp = p;
			tokens.grow(tokens.getCount() + 1);
			inToken = &tokens[tokens.getCount() - 1];
			inToken->text = &str[p];
			inToken->origin = origin;
		}

		// done with char
		++p;
		++origin;
	}

	if (inStr)
		error("Missing close quote <%c>", inStr);

	if (inToken)
		inToken->length = p - startp;
}

void Tokens::error(const char* fmt, ...)
{
	string buffer;

	va_list params;
	va_start(params, fmt);
	buffer.vprintf(fmt, params);
	va_end(params);

	(Arg::Gds(isc_random) << "Parse to tokens error" << Arg::Gds(isc_random) << buffer).raise();
}

string Tokens::Tok::stripped() const
{
	string rc;
	char q = text[0];
	for (FB_SIZE_T i = 1; i < length - 1; ++i)
	{
		if (text[i] == q)
			++i;
		rc += text[i];
	}
	return rc;
}

Tokens::Comment sqlComments[3] = {
	{ "/*", "*/" },
	{ "--", "\n" },
	{ NULL, NULL }
};

const char* sqlSpaces = " \t\r\n";

} // namespace Firebird
