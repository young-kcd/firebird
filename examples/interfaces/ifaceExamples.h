/*
 *	PROGRAM:	Object oriented API samples.
 *	MODULE:		ifaceExamples.h
 *	DESCRIPTION:	A number of common defines for all samples.
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
 *  The Original Code was created by Alexander Peshkoff
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2014 Alexander Peshkoff <peshkoff@mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <ibase.h>
#include <firebird/Interface.h>

using namespace Firebird;

typedef FirebirdApi<class ExamplesPolicy> Api;
FB_USE_API(Api)

// Dummy policy - to be enhanced
class ExamplesPolicy
{
public:
	template <unsigned V, typename T>
	static inline bool checkVersion(T* versioned, IStatus* status)
	{ return true; }
	static void checkException(Api::IStatus*) { }
	static void catchException(Api::IStatus*) { }
	typedef Api::IStatus* IStatus;
};


// Declare function to get access to master interface.
DECLARE_GET_MASTER(ExamplesPolicy);

// Probably not best way of error processing, but it's OK for a sample
static void check(IStatus* s, const char* text)
{
	if (s->getStatus() & IStatus::FB_HAS_ERRORS)
		throw text;
}
