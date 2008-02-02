/*
 *	PROGRAM:		Firebird utilities interface
 *	MODULE:			UtilSvc.h
 *	DESCRIPTION:	Interface making it possible to use same code 
 *					as both utility or service
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
 *  Copyright (c) 2007 Alex Peshkov <peshkoff at mail dot ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 */

#ifndef FB_UTILFACE
#define FB_UTILFACE

#include "../common/classes/alloc.h"
#include "../common/classes/array.h"

namespace MsgFormat {
	class SafeArg;
}

namespace Firebird {

class UtilSvc : public Firebird::GlobalStorage
{
public:
	typedef Firebird::HalfStaticArray<const char*, 20> ArgvType;

public:
	UtilSvc() : argv(getPool()) { }

	void output(const char* text)
	{
		printf("%s", text);
	}

    virtual void printf(const SCHAR* format, ...) = 0;
	virtual bool isService() = 0;
	virtual void started() = 0;
	virtual void finish() = 0;
    virtual void putLine(char, const char*) = 0;
    virtual void putSLong(char, SLONG) = 0;
	virtual void putChar(char, char) = 0;
	virtual void stuffStatus(const ISC_STATUS*) = 0;
	virtual void stuffStatus(const USHORT, const USHORT, const MsgFormat::SafeArg&) = 0;
	virtual void hidePasswd(ArgvType&, int) = 0;
	virtual ISC_STATUS* getStatus() = 0;
	virtual void checkService() = 0;


	virtual ~UtilSvc() { }

	static UtilSvc* createStandalone(int ac, char** argv);

public:
	ArgvType argv;
};


} // namespace Firebird


#endif // FB_UTILFACE
