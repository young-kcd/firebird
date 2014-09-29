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

//awk <FirebirdInterface.idl '($1 == "interface") {printf "\ttypedef FirebirdApi<FirebirdPolicy>::%s I%s;\n", $2, $2;}'

	typedef FirebirdApi<FirebirdPolicy>::Versioned IVersioned;
	typedef FirebirdApi<FirebirdPolicy>::ReferenceCounted IReferenceCounted;
	typedef FirebirdApi<FirebirdPolicy>::Disposable IDisposable;
	typedef FirebirdApi<FirebirdPolicy>::Status IStatus;
	typedef FirebirdApi<FirebirdPolicy>::Master IMaster;
	typedef FirebirdApi<FirebirdPolicy>::PluginBase IPluginBase;
	typedef FirebirdApi<FirebirdPolicy>::PluginSet IPluginSet;
	typedef FirebirdApi<FirebirdPolicy>::ConfigEntry IConfigEntry;
	typedef FirebirdApi<FirebirdPolicy>::Config IConfig;
	typedef FirebirdApi<FirebirdPolicy>::FirebirdConf IFirebirdConf;
	typedef FirebirdApi<FirebirdPolicy>::PluginConfig IPluginConfig;
	typedef FirebirdApi<FirebirdPolicy>::PluginFactory IPluginFactory;
	typedef FirebirdApi<FirebirdPolicy>::PluginModule IPluginModule;
	typedef FirebirdApi<FirebirdPolicy>::PluginManager IPluginManager;
	typedef FirebirdApi<FirebirdPolicy>::ConfigManager IConfigManager;
	typedef FirebirdApi<FirebirdPolicy>::EventCallback IEventCallback;
	typedef FirebirdApi<FirebirdPolicy>::Blob IBlob;
	typedef FirebirdApi<FirebirdPolicy>::Transaction ITransaction;
	typedef FirebirdApi<FirebirdPolicy>::MessageMetadata IMessageMetadata;
	typedef FirebirdApi<FirebirdPolicy>::MetadataBuilder IMetadataBuilder;
	typedef FirebirdApi<FirebirdPolicy>::ResultSet IResultSet;
	typedef FirebirdApi<FirebirdPolicy>::Statement IStatement;
	typedef FirebirdApi<FirebirdPolicy>::Request IRequest;
	typedef FirebirdApi<FirebirdPolicy>::Events IEvents;
	typedef FirebirdApi<FirebirdPolicy>::Attachment IAttachment;
	typedef FirebirdApi<FirebirdPolicy>::Service IService;
	typedef FirebirdApi<FirebirdPolicy>::Provider IProvider;
	typedef FirebirdApi<FirebirdPolicy>::Dtc IDtc;
	typedef FirebirdApi<FirebirdPolicy>::Auth IAuth;
	typedef FirebirdApi<FirebirdPolicy>::Writer IWriter;
	typedef FirebirdApi<FirebirdPolicy>::ServerBlock IServerBlock;
	typedef FirebirdApi<FirebirdPolicy>::ClientBlock IClientBlock;
	typedef FirebirdApi<FirebirdPolicy>::Server IServer;
	typedef FirebirdApi<FirebirdPolicy>::Client IClient;
	typedef FirebirdApi<FirebirdPolicy>::UserField IUserField;
	typedef FirebirdApi<FirebirdPolicy>::CharUserField ICharUserField;
	typedef FirebirdApi<FirebirdPolicy>::IntUserField IIntUserField;
	typedef FirebirdApi<FirebirdPolicy>::User IUser;
	typedef FirebirdApi<FirebirdPolicy>::ListUsers IListUsers;
	typedef FirebirdApi<FirebirdPolicy>::LogonInfo ILogonInfo;
	typedef FirebirdApi<FirebirdPolicy>::Management IManagement;
	typedef FirebirdApi<FirebirdPolicy>::WireCryptPlugin IWireCryptPlugin;
	typedef FirebirdApi<FirebirdPolicy>::CryptKeyCallback ICryptKeyCallback;
	typedef FirebirdApi<FirebirdPolicy>::KeyHolderPlugin IKeyHolderPlugin;
	typedef FirebirdApi<FirebirdPolicy>::DbCryptPlugin IDbCryptPlugin;
	typedef FirebirdApi<FirebirdPolicy>::ExternalContext IExternalContext;
	typedef FirebirdApi<FirebirdPolicy>::ExternalResultSet IExternalResultSet;
	typedef FirebirdApi<FirebirdPolicy>::ExternalFunction IExternalFunction;
	typedef FirebirdApi<FirebirdPolicy>::ExternalProcedure IExternalProcedure;
	typedef FirebirdApi<FirebirdPolicy>::ExternalTrigger IExternalTrigger;
	typedef FirebirdApi<FirebirdPolicy>::RoutineMetadata IRoutineMetadata;
	typedef FirebirdApi<FirebirdPolicy>::ExternalEngine IExternalEngine;
	typedef FirebirdApi<FirebirdPolicy>::Timer ITimer;
	typedef FirebirdApi<FirebirdPolicy>::TimerControl ITimerControl;
	typedef FirebirdApi<FirebirdPolicy>::VersionCallback IVersionCallback;
	typedef FirebirdApi<FirebirdPolicy>::Utl IUtl;
	typedef FirebirdApi<FirebirdPolicy>::TraceConnection ITraceConnection;
	typedef FirebirdApi<FirebirdPolicy>::TraceDatabaseConnection ITraceDatabaseConnection;
	typedef FirebirdApi<FirebirdPolicy>::TraceTransaction ITraceTransaction;
	typedef FirebirdApi<FirebirdPolicy>::TraceParams ITraceParams;
	typedef FirebirdApi<FirebirdPolicy>::TraceStatement ITraceStatement;
	typedef FirebirdApi<FirebirdPolicy>::TraceSQLStatement ITraceSQLStatement;
	typedef FirebirdApi<FirebirdPolicy>::TraceBLRStatement ITraceBLRStatement;
	typedef FirebirdApi<FirebirdPolicy>::TraceDYNRequest ITraceDYNRequest;
	typedef FirebirdApi<FirebirdPolicy>::TraceContextVariable ITraceContextVariable;
	typedef FirebirdApi<FirebirdPolicy>::TraceProcedure ITraceProcedure;
	typedef FirebirdApi<FirebirdPolicy>::TraceFunction ITraceFunction;
	typedef FirebirdApi<FirebirdPolicy>::TraceTrigger ITraceTrigger;
	typedef FirebirdApi<FirebirdPolicy>::TraceServiceConnection ITraceServiceConnection;
	typedef FirebirdApi<FirebirdPolicy>::TraceStatusVector ITraceStatusVector;
	typedef FirebirdApi<FirebirdPolicy>::TraceSweepInfo ITraceSweepInfo;
	typedef FirebirdApi<FirebirdPolicy>::TraceLogWriter ITraceLogWriter;
	typedef FirebirdApi<FirebirdPolicy>::TraceInitInfo ITraceInitInfo;
	typedef FirebirdApi<FirebirdPolicy>::TracePlugin ITracePlugin;
	typedef FirebirdApi<FirebirdPolicy>::TraceFactory ITraceFactory;

	typedef FirebirdApi<FirebirdPolicy> Api;

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
