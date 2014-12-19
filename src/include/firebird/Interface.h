/*
 *	PROGRAM:		Firebird interface.
 *	MODULE:			firebird/Interface.h
 *	DESCRIPTION:	Base class for all FB interfaces / plugins.
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
 *  Copyright (c) 2010 Alex Peshkov <peshkoff at mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 */

#ifndef FB_INCLUDE_INTERFACE
#define FB_INCLUDE_INTERFACE

#include "ibase.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define CLOOP_CARG __cdecl
#endif

struct dsc;
struct PerformanceInfo;

namespace Firebird
{

struct FbCryptKey;

#include "IdlFbInterfaces.h"

#define FB_USE_API(name)	\
	typedef name::IVersioned IVersioned;	\
	typedef name::IReferenceCounted IReferenceCounted;	\
	typedef name::IDisposable IDisposable;	\
	typedef name::IStatus IStatus;	\
	typedef name::IMaster IMaster;	\
	typedef name::IPluginBase IPluginBase;	\
	typedef name::IPluginSet IPluginSet;	\
	typedef name::IConfigEntry IConfigEntry;	\
	typedef name::IConfig IConfig;	\
	typedef name::IFirebirdConf IFirebirdConf;	\
	typedef name::IPluginConfig IPluginConfig;	\
	typedef name::IPluginFactory IPluginFactory;	\
	typedef name::IPluginModule IPluginModule;	\
	typedef name::IPluginManager IPluginManager;	\
	typedef name::IConfigManager IConfigManager;	\
	typedef name::IEventCallback IEventCallback;	\
	typedef name::IBlob IBlob;	\
	typedef name::ITransaction ITransaction;	\
	typedef name::IMessageMetadata IMessageMetadata;	\
	typedef name::IMetadataBuilder IMetadataBuilder;	\
	typedef name::IResultSet IResultSet;	\
	typedef name::IStatement IStatement;	\
	typedef name::IRequest IRequest;	\
	typedef name::IEvents IEvents;	\
	typedef name::IAttachment IAttachment;	\
	typedef name::IService IService;	\
	typedef name::IProvider IProvider;	\
	typedef name::IDtcStart IDtcStart;	\
	typedef name::IDtc IDtc;	\
	typedef name::IAuth IAuth;	\
	typedef name::IWriter IWriter;	\
	typedef name::IServerBlock IServerBlock;	\
	typedef name::IClientBlock IClientBlock;	\
	typedef name::IServer IServer;	\
	typedef name::IClient IClient;	\
	typedef name::IUserField IUserField;	\
	typedef name::ICharUserField ICharUserField;	\
	typedef name::IIntUserField IIntUserField;	\
	typedef name::IUser IUser;	\
	typedef name::IListUsers IListUsers;	\
	typedef name::ILogonInfo ILogonInfo;	\
	typedef name::IManagement IManagement;	\
	typedef name::IWireCryptPlugin IWireCryptPlugin;	\
	typedef name::ICryptKeyCallback ICryptKeyCallback;	\
	typedef name::IKeyHolderPlugin IKeyHolderPlugin;	\
	typedef name::IDbCryptPlugin IDbCryptPlugin;	\
	typedef name::IExternalContext IExternalContext;	\
	typedef name::IExternalResultSet IExternalResultSet;	\
	typedef name::IExternalFunction IExternalFunction;	\
	typedef name::IExternalProcedure IExternalProcedure;	\
	typedef name::IExternalTrigger IExternalTrigger;	\
	typedef name::IRoutineMetadata IRoutineMetadata;	\
	typedef name::IExternalEngine IExternalEngine;	\
	typedef name::ITimer ITimer;	\
	typedef name::ITimerControl ITimerControl;	\
	typedef name::IVersionCallback IVersionCallback;	\
	typedef name::IUtl IUtl;	\
	typedef name::ITraceConnection ITraceConnection;	\
	typedef name::ITraceDatabaseConnection ITraceDatabaseConnection;	\
	typedef name::ITraceTransaction ITraceTransaction;	\
	typedef name::ITraceParams ITraceParams;	\
	typedef name::ITraceStatement ITraceStatement;	\
	typedef name::ITraceSQLStatement ITraceSQLStatement;	\
	typedef name::ITraceBLRStatement ITraceBLRStatement;	\
	typedef name::ITraceDYNRequest ITraceDYNRequest;	\
	typedef name::ITraceContextVariable ITraceContextVariable;	\
	typedef name::ITraceProcedure ITraceProcedure;	\
	typedef name::ITraceFunction ITraceFunction;	\
	typedef name::ITraceTrigger ITraceTrigger;	\
	typedef name::ITraceServiceConnection ITraceServiceConnection;	\
	typedef name::ITraceStatusVector ITraceStatusVector;	\
	typedef name::ITraceSweepInfo ITraceSweepInfo;	\
	typedef name::ITraceLogWriter ITraceLogWriter;	\
	typedef name::ITraceInitInfo ITraceInitInfo;	\
	typedef name::ITracePlugin ITracePlugin;	\
	typedef name::ITraceFactory ITraceFactory;

#ifdef INCLUDE_Firebird_H		// Building internal module

typedef FirebirdApi<class FirebirdPolicy> Api;
FB_USE_API(Api)

class FirebirdPolicy
{
public:
	template <unsigned V, typename T>
	static inline bool checkVersion(T* versioned, IStatus* status)
	{
		if (versioned->cloopVTable->version < V)
			return true;

		if (status)
		{
			intptr_t codes[] = {
				isc_arg_gds,
				isc_interface_version_too_old,
				isc_arg_number,
				(intptr_t) V,
				isc_arg_number,
				(intptr_t) versioned->cloopVTable->version,
				isc_arg_end
			};

			status->setErrors(codes);
		}

		return false;
	}

	static void checkException(Api::IStatus*) { }
	static void catchException(Api::IStatus*) { }
	typedef Api::IStatus* IStatus;
};

#else // INCLUDE_Firebird_H		building external module

#endif // INCLUDE_Firebird_H

struct FbCryptKey
{
	const char* type;					// If NULL type is auth plugin name
	const void* encryptKey;
	const void* decryptKey;				// May be NULL for symmetric keys
	unsigned encryptLength;
	unsigned decryptLength;			// Ignored when decryptKey is NULL
};

#ifdef INCLUDE_Firebird_H		// Building internal module

// This item is for ISC API emulation only
// It may be gone in future versions
// Please do not use it!
static IMessageMetadata* const DELAYED_OUT_FORMAT = reinterpret_cast<IMessageMetadata*>(1);

#endif //INCLUDE_Firebird_H

} // namespace Firebird

#define FB_PLUGIN_ENTRY_POINT firebird_plugin

// Additional API function.
// Should be used only in non-plugin modules.
// All plugins including providers should use passed at init time interface instead.
#define DECLARE_GET_MASTER(P) extern "C" Firebird::FirebirdApi<P>::IMaster* ISC_EXPORT fb_get_master_interface();

#ifdef INCLUDE_Firebird_H		// Building internal module
DECLARE_GET_MASTER(Firebird::FirebirdPolicy)
#endif //INCLUDE_Firebird_H

#endif // FB_INCLUDE_INTERFACE
