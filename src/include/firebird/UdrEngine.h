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

//----------------------------------------------------------------
// This ifdef is a dirty hack to make tests (fbstuff) to compile
#ifndef INCLUDE_Firebird_H

namespace Firebird
{
typedef FirebirdApi<class TempPolicy> Api;
FB_USE_API(Api, I)

class TempPolicy
{
public:
	template <unsigned V, typename T>
	static inline bool checkVersion(T* versioned, IStatus* status)
	{ return true; }
	static void checkException(Api::Status*) { }
	static void catchException(Api::Status*) { }
	typedef Api::Status* Status;
};
} //namespace Firebird

DECLARE_GET_MASTER(Firebird::TempPolicy)
#define INCLUDE_Firebird_H

#endif
//- END OF HACK --------------------------------------------------


#ifndef FB_EXPORTED
#if defined(DARWIN)
#define FB_EXPORTED __attribute__((visibility("default")))
#else
#define FB_EXPORTED
#endif // OS choice (DARWIN)
#endif // FB_EXPORTED


namespace Firebird
{
	namespace Udr
	{
//------------------------------------------------------------------------------


// Factory classes. They should be singletons instances created by user's modules and
// registered. When UDR engine is going to load a routine, it calls newItem.

class FunctionFactory
{
public:
	virtual void setup(IStatus* status, IExternalContext* context, IRoutineMetadata* metadata,
		IMetadataBuilder* inBuilder, IMetadataBuilder* outBuilder) = 0;
	virtual IExternalFunction* newItem(IStatus* status, IExternalContext* context,
		IRoutineMetadata* metadata) = 0;
};

class ProcedureFactory
{
public:
	virtual void setup(IStatus* status, IExternalContext* context, IRoutineMetadata* metadata,
		IMetadataBuilder* inBuilder, IMetadataBuilder* outBuilder) = 0;
	virtual IExternalProcedure* newItem(IStatus* status, IExternalContext* context,
		IRoutineMetadata* metadata) = 0;
};

class TriggerFactory
{
public:
	virtual void setup(IStatus* status, IExternalContext* context, IRoutineMetadata* metadata,
		IMetadataBuilder* fieldsBuilder) = 0;
	virtual IExternalTrigger* newItem(IStatus* status, IExternalContext* context,
		IRoutineMetadata* metadata) = 0;
};

// Routine registration functions.
extern "C" void FB_EXPORTED fbUdrRegFunction(const char* name, FunctionFactory* factory);
extern "C" void FB_EXPORTED fbUdrRegProcedure(const char* name, ProcedureFactory* factory);
extern "C" void FB_EXPORTED fbUdrRegTrigger(const char* name, TriggerFactory* factory);


//------------------------------------------------------------------------------
	}	// namespace Udr
}	// namespace Firebird

#endif	// FIREBIRD_UDR_H
