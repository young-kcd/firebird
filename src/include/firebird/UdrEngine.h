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
 *  The Original Code was created by Adriano dos Santos Fernandes
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2008 Adriano dos Santos Fernandes <adrianosf@uol.com.br>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#ifndef FIREBIRD_UDR_H
#define FIREBIRD_UDR_H

#include "./Interface.h"

#ifndef FB_EXPORTED
#if defined(DARWIN)
#define FB_EXPORTED __attribute__((visibility("default")))
#else
#define FB_EXPORTED
#endif // OS choice (DARWIN)
#endif // FB_EXPORTED


namespace Firebird
{
//------------------------------------------------------------------------------


// Routine registration functions.
extern "C" void FB_EXPORTED fbUdrRegFunction(const char* name, IUdrFunctionFactory* factory);
extern "C" void FB_EXPORTED fbUdrRegProcedure(const char* name, IUdrProcedureFactory* factory);
extern "C" void FB_EXPORTED fbUdrRegTrigger(const char* name, IUdrTriggerFactory* factory);


//------------------------------------------------------------------------------
}	// namespace Firebird

#endif	// FIREBIRD_UDR_H
