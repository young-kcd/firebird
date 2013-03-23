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

#include "./ExternalEngine.h"


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
	virtual void setup(IStatus* status, ExternalContext* context, const IRoutineMetadata* metadata,
		IMetadataBuilder* inBuilder, IMetadataBuilder* outBuilder) = 0;
	virtual ExternalFunction* FB_CALL newItem(IStatus* status, ExternalContext* context,
		const IRoutineMetadata* metadata) = 0;
};

class ProcedureFactory
{
public:
	virtual void setup(IStatus* status, ExternalContext* context, const IRoutineMetadata* metadata,
		IMetadataBuilder* inBuilder, IMetadataBuilder* outBuilder) = 0;
	virtual ExternalProcedure* FB_CALL newItem(IStatus* status, ExternalContext* context,
		const IRoutineMetadata* metadata) = 0;
};

class TriggerFactory
{
public:
	virtual void setup(IStatus* status, ExternalContext* context, const IRoutineMetadata* metadata,
		IMetadataBuilder* fieldsBuilder) = 0;
	virtual ExternalTrigger* FB_CALL newItem(IStatus* status, ExternalContext* context,
		const IRoutineMetadata* metadata) = 0;
};


// Routine registration functions.
extern "C" void fbUdrRegFunction(const char* name, FunctionFactory* factory);
extern "C" void fbUdrRegProcedure(const char* name, ProcedureFactory* factory);
extern "C" void fbUdrRegTrigger(const char* name, TriggerFactory* factory);


//------------------------------------------------------------------------------
	}	// namespace Udr
}	// namespace Firebird

#endif	// FIREBIRD_UDR_H
