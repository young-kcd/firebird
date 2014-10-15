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
struct DtcStart;

#include "IdlFbInterfaces.h"

#ifdef INCLUDE_Firebird_H		// Building internal module

class FirebirdPolicy;
extern void raiseVersionError();
extern void upgradeInterface(FirebirdApi<FirebirdPolicy>::Versioned* toUpgrade, int desiredVersion, void* function);
extern void logOldPlugin();
extern ISC_STATUS* getUpgradeError();

namespace
{
	static void defaultUpgradeFunction(void*, FirebirdApi<FirebirdPolicy>::Status* status)
	{
		status->setErrors2(2, getUpgradeError());
	}

	// This may be used when old plugin, missing some newer events is used.
	// Reasonable action here is to log once and ignore next times.
	static void ignoreMissing(void*)
	{
		static bool flagFirst = true;

		if (flagFirst)
		{
			flagFirst = false;
			logOldPlugin();
		}
	};

	template <typename T>
	static inline void upgradeVersionedInterface(T* versioned)
	{
		if (versioned && versioned->cloopVTable->version < T::VERSION)
		{
			upgradeInterface(versioned, T::VERSION, (void*)defaultUpgradeFunction);
		}
	}

	template <>
	void upgradeVersionedInterface<FirebirdApi<FirebirdPolicy>::TracePlugin>
		(FirebirdApi<FirebirdPolicy>::TracePlugin* versioned)
	{
		if (versioned && versioned->cloopVTable->version < FirebirdApi<FirebirdPolicy>::TracePlugin::VERSION)
		{
			upgradeInterface(versioned, FirebirdApi<FirebirdPolicy>::TracePlugin::VERSION, (void*)ignoreMissing);
		}
	}
}

class FirebirdPolicy
{
public:
	template <unsigned V, typename T>
	static inline void checkVersion(T* versioned)
	{
		if (versioned && versioned->cloopVTable->version < V)
		{
			raiseVersionError();
		}
	}

	template <typename T>
	static inline T* upgrade(T* versioned)
	{
		upgradeVersionedInterface(versioned);
		return versioned;
	}

	static void checkException(FirebirdApi<FirebirdPolicy>::Status* status) { }
	static void catchException(FirebirdApi<FirebirdPolicy>::Status* status) { }
	typedef FirebirdApi<FirebirdPolicy>::Status* Status;
};

#else // INCLUDE_Firebird_H		building external module

// use empty default policy

class FirebirdPolicy
{
public:
	template <unsigned V, typename T>
	static inline void checkVersion(T* versioned) { }

	template <typename T>
	static inline T* upgrade(T* versioned)
	{
		return versioned;
	}

	template <typename S>
	static void checkException(S status) { }

	template <typename S>
	static void catchException(S status) { }

	typedef FirebirdApi<FirebirdPolicy>::Status* Status;
};

#endif // INCLUDE_Firebird_H

	typedef FirebirdApi<FirebirdPolicy> Api;

//awk <FirebirdInterface.idl '($1 == "interface") {printf "\ttypedef Api::%s I%s;\n", $2, $2;}'

	typedef Api::Versioned IVersioned;
	typedef Api::ReferenceCounted IReferenceCounted;
	typedef Api::Disposable IDisposable;
	typedef Api::Status IStatus;
	typedef Api::Master IMaster;
	typedef Api::PluginBase IPluginBase;
	typedef Api::PluginSet IPluginSet;
	typedef Api::ConfigEntry IConfigEntry;
	typedef Api::Config IConfig;
	typedef Api::FirebirdConf IFirebirdConf;
	typedef Api::PluginConfig IPluginConfig;
	typedef Api::PluginFactory IPluginFactory;
	typedef Api::PluginModule IPluginModule;
	typedef Api::PluginManager IPluginManager;
	typedef Api::ConfigManager IConfigManager;
	typedef Api::EventCallback IEventCallback;
	typedef Api::Blob IBlob;
	typedef Api::Transaction ITransaction;
	typedef Api::MessageMetadata IMessageMetadata;
	typedef Api::MetadataBuilder IMetadataBuilder;
	typedef Api::ResultSet IResultSet;
	typedef Api::Statement IStatement;
	typedef Api::Request IRequest;
	typedef Api::Events IEvents;
	typedef Api::Attachment IAttachment;
	typedef Api::Service IService;
	typedef Api::Provider IProvider;
	typedef Api::Dtc IDtc;
	typedef Api::Auth IAuth;
	typedef Api::Writer IWriter;
	typedef Api::ServerBlock IServerBlock;
	typedef Api::ClientBlock IClientBlock;
	typedef Api::Server IServer;
	typedef Api::Client IClient;
	typedef Api::UserField IUserField;
	typedef Api::CharUserField ICharUserField;
	typedef Api::IntUserField IIntUserField;
	typedef Api::User IUser;
	typedef Api::ListUsers IListUsers;
	typedef Api::LogonInfo ILogonInfo;
	typedef Api::Management IManagement;
	typedef Api::WireCryptPlugin IWireCryptPlugin;
	typedef Api::CryptKeyCallback ICryptKeyCallback;
	typedef Api::KeyHolderPlugin IKeyHolderPlugin;
	typedef Api::DbCryptPlugin IDbCryptPlugin;
	typedef Api::ExternalContext IExternalContext;
	typedef Api::ExternalResultSet IExternalResultSet;
	typedef Api::ExternalFunction IExternalFunction;
	typedef Api::ExternalProcedure IExternalProcedure;
	typedef Api::ExternalTrigger IExternalTrigger;
	typedef Api::RoutineMetadata IRoutineMetadata;
	typedef Api::ExternalEngine IExternalEngine;
	typedef Api::Timer ITimer;
	typedef Api::TimerControl ITimerControl;
	typedef Api::VersionCallback IVersionCallback;
	typedef Api::Utl IUtl;
	typedef Api::TraceConnection ITraceConnection;
	typedef Api::TraceDatabaseConnection ITraceDatabaseConnection;
	typedef Api::TraceTransaction ITraceTransaction;
	typedef Api::TraceParams ITraceParams;
	typedef Api::TraceStatement ITraceStatement;
	typedef Api::TraceSQLStatement ITraceSQLStatement;
	typedef Api::TraceBLRStatement ITraceBLRStatement;
	typedef Api::TraceDYNRequest ITraceDYNRequest;
	typedef Api::TraceContextVariable ITraceContextVariable;
	typedef Api::TraceProcedure ITraceProcedure;
	typedef Api::TraceFunction ITraceFunction;
	typedef Api::TraceTrigger ITraceTrigger;
	typedef Api::TraceServiceConnection ITraceServiceConnection;
	typedef Api::TraceStatusVector ITraceStatusVector;
	typedef Api::TraceSweepInfo ITraceSweepInfo;
	typedef Api::TraceLogWriter ITraceLogWriter;
	typedef Api::TraceInitInfo ITraceInitInfo;
	typedef Api::TracePlugin ITracePlugin;
	typedef Api::TraceFactory ITraceFactory;

struct FbCryptKey
{
	const char* type;					// If NULL type is auth plugin name
	const void* encryptKey;
	const void* decryptKey;				// May be NULL for symmetric keys
	unsigned encryptLength;
	unsigned decryptLength;			// Ignored when decryptKey is NULL
};

struct DtcStart
{
	IAttachment* attachment;
	const unsigned char* tpb;
	unsigned tpbLength;
};

typedef void PluginEntrypoint(IMaster* masterInterface);
typedef char Utf8;	// Utf8* used as nul-terminated string

#ifdef INCLUDE_Firebird_H		// Building internal module

// This item is for ISC API emulation only
// It may be gone in future versions
// Please do not use it!
static IMessageMetadata* const DELAYED_OUT_FORMAT = (IMessageMetadata*)(1);

namespace
{
	template <>
	void upgradeVersionedInterface<IMessageMetadata>(IMessageMetadata* versioned)
	{
		if (versioned && versioned != DELAYED_OUT_FORMAT &&
			versioned->cloopVTable->version < FirebirdApi<FirebirdPolicy>::TracePlugin::VERSION)
		{
			upgradeInterface(versioned, FirebirdApi<FirebirdPolicy>::TracePlugin::VERSION, (void*)ignoreMissing);
		}
	}
}

#endif //INCLUDE_Firebird_H

} // namespace Firebird

#define FB_PLUGIN_ENTRY_POINT firebird_plugin

extern "C"
{
	// Additional API function.
	// Should be used only in non-plugin modules.
	// All plugins including providers should use passed at init time interface instead.
	Firebird::IMaster* ISC_EXPORT fb_get_master_interface();
}

#endif // FB_INCLUDE_INTERFACE
