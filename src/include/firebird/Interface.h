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

#define FB_USE_API(name, prefix)	\
	typedef name::Versioned prefix##Versioned;	\
	typedef name::ReferenceCounted prefix##ReferenceCounted;	\
	typedef name::Disposable prefix##Disposable;	\
	typedef name::Status prefix##Status;	\
	typedef name::Master prefix##Master;	\
	typedef name::PluginBase prefix##PluginBase;	\
	typedef name::PluginSet prefix##PluginSet;	\
	typedef name::ConfigEntry prefix##ConfigEntry;	\
	typedef name::Config prefix##Config;	\
	typedef name::FirebirdConf prefix##FirebirdConf;	\
	typedef name::PluginConfig prefix##PluginConfig;	\
	typedef name::PluginFactory prefix##PluginFactory;	\
	typedef name::PluginModule prefix##PluginModule;	\
	typedef name::PluginManager prefix##PluginManager;	\
	typedef name::ConfigManager prefix##ConfigManager;	\
	typedef name::EventCallback prefix##EventCallback;	\
	typedef name::Blob prefix##Blob;	\
	typedef name::Transaction prefix##Transaction;	\
	typedef name::MessageMetadata prefix##MessageMetadata;	\
	typedef name::MetadataBuilder prefix##MetadataBuilder;	\
	typedef name::ResultSet prefix##ResultSet;	\
	typedef name::Statement prefix##Statement;	\
	typedef name::Request prefix##Request;	\
	typedef name::Events prefix##Events;	\
	typedef name::Attachment prefix##Attachment;	\
	typedef name::Service prefix##Service;	\
	typedef name::Provider prefix##Provider;	\
	typedef name::DtcStart prefix##DtcStart;	\
	typedef name::Dtc prefix##Dtc;	\
	typedef name::Auth prefix##Auth;	\
	typedef name::Writer prefix##Writer;	\
	typedef name::ServerBlock prefix##ServerBlock;	\
	typedef name::ClientBlock prefix##ClientBlock;	\
	typedef name::Server prefix##Server;	\
	typedef name::Client prefix##Client;	\
	typedef name::UserField prefix##UserField;	\
	typedef name::CharUserField prefix##CharUserField;	\
	typedef name::IntUserField prefix##IntUserField;	\
	typedef name::User prefix##User;	\
	typedef name::ListUsers prefix##ListUsers;	\
	typedef name::LogonInfo prefix##LogonInfo;	\
	typedef name::Management prefix##Management;	\
	typedef name::WireCryptPlugin prefix##WireCryptPlugin;	\
	typedef name::CryptKeyCallback prefix##CryptKeyCallback;	\
	typedef name::KeyHolderPlugin prefix##KeyHolderPlugin;	\
	typedef name::DbCryptPlugin prefix##DbCryptPlugin;	\
	typedef name::ExternalContext prefix##ExternalContext;	\
	typedef name::ExternalResultSet prefix##ExternalResultSet;	\
	typedef name::ExternalFunction prefix##ExternalFunction;	\
	typedef name::ExternalProcedure prefix##ExternalProcedure;	\
	typedef name::ExternalTrigger prefix##ExternalTrigger;	\
	typedef name::RoutineMetadata prefix##RoutineMetadata;	\
	typedef name::ExternalEngine prefix##ExternalEngine;	\
	typedef name::Timer prefix##Timer;	\
	typedef name::TimerControl prefix##TimerControl;	\
	typedef name::VersionCallback prefix##VersionCallback;	\
	typedef name::Utl prefix##Utl;	\
	typedef name::TraceConnection prefix##TraceConnection;	\
	typedef name::TraceDatabaseConnection prefix##TraceDatabaseConnection;	\
	typedef name::TraceTransaction prefix##TraceTransaction;	\
	typedef name::TraceParams prefix##TraceParams;	\
	typedef name::TraceStatement prefix##TraceStatement;	\
	typedef name::TraceSQLStatement prefix##TraceSQLStatement;	\
	typedef name::TraceBLRStatement prefix##TraceBLRStatement;	\
	typedef name::TraceDYNRequest prefix##TraceDYNRequest;	\
	typedef name::TraceContextVariable prefix##TraceContextVariable;	\
	typedef name::TraceProcedure prefix##TraceProcedure;	\
	typedef name::TraceFunction prefix##TraceFunction;	\
	typedef name::TraceTrigger prefix##TraceTrigger;	\
	typedef name::TraceServiceConnection prefix##TraceServiceConnection;	\
	typedef name::TraceStatusVector prefix##TraceStatusVector;	\
	typedef name::TraceSweepInfo prefix##TraceSweepInfo;	\
	typedef name::TraceLogWriter prefix##TraceLogWriter;	\
	typedef name::TraceInitInfo prefix##TraceInitInfo;	\
	typedef name::TracePlugin prefix##TracePlugin;	\
	typedef name::TraceFactory prefix##TraceFactory;

#ifdef INCLUDE_Firebird_H		// Building internal module

typedef FirebirdApi<class FirebirdPolicy> Api;
FB_USE_API(Api, I)

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

	static void checkException(Api::Status*) { }
	static void catchException(Api::Status*) { }
	typedef Api::Status* Status;
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
#define DECLARE_GET_MASTER(P) extern "C" Firebird::FirebirdApi<P>::Master* ISC_EXPORT fb_get_master_interface();

#ifdef INCLUDE_Firebird_H		// Building internal module
DECLARE_GET_MASTER(Firebird::FirebirdPolicy)
#endif //INCLUDE_Firebird_H

#endif // FB_INCLUDE_INTERFACE
