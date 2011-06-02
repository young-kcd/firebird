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
 *  The Original Code was created by Dmitry Yemanov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2002 Dmitry Yemanov <dimitr@users.sf.net>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"

#include "../common/config/config.h"
#include "../common/config/config_file.h"
#include "../common/config/os/config_root.h"
#include "../common/classes/init.h"
#include "../common/dllinst.h"
#include "../common/os/fbsyslog.h"
#include "firebird/Plugin.h"

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

namespace {

/******************************************************************************
 *
 *	firebird.conf implementation
 */

class ConfigImpl : public Firebird::PermanentStorage
{
public:
	explicit ConfigImpl(Firebird::MemoryPool& p)
		: Firebird::PermanentStorage(p), confMessage(getPool())
	{
		ConfigFile file(fb_utils::getPrefix(fb_utils::FB_DIR_CONF, CONFIG_FILE));
		defaultConfig = new Config(file);

		if (file.getMessage())
		{
			confMessage = file.getMessage();
		}
	}

/*	void changeDefaultConfig(Config* newConfig)
	{
		defaultConfig = newConfig;
	}
 */
	Firebird::RefPtr<Config> getDefaultConfig() const
	{
		return defaultConfig;
	}

	const char* getMessage()
	{
		return confMessage.nullStr();
	}

private:
	Firebird::RefPtr<Config> defaultConfig;

    ConfigImpl(const ConfigImpl&);
    void operator=(const ConfigImpl&);

	Firebird::string confMessage;
};

/******************************************************************************
 *
 *	Static instance of the system configuration file
 */

Firebird::InitInstance<ConfigImpl> firebirdConf;

/******************************************************************************
 *
 *	Static instance of the root and install directories detector
 */

Firebird::InitInstance<ConfigRoot> rootDetector;

}	// anonymous namespace

/******************************************************************************
 *
 *	Configuration entries
 */

const char*	GCPolicyCooperative	= "cooperative";
const char*	GCPolicyBackground	= "background";
const char*	GCPolicyCombined	= "combined";

const char*	AmNative	= "native";
const char*	AmTrusted	= "trusted";
const char*	AmMixed		= "mixed";

const Config::ConfigEntry Config::entries[MAX_CONFIG_KEY] =
{
	{TYPE_INTEGER,		"TempBlockSize",			(ConfigValue) 1048576},		// bytes
	{TYPE_INTEGER,		"TempCacheLimit",			(ConfigValue) -1},			// bytes
	{TYPE_BOOLEAN,		"RemoteFileOpenAbility",	(ConfigValue) false},
	{TYPE_INTEGER,		"GuardianOption",			(ConfigValue) 1},
	{TYPE_INTEGER,		"CpuAffinityMask",			(ConfigValue) 0},
	{TYPE_INTEGER,		"TcpRemoteBufferSize",		(ConfigValue) 8192},		// bytes
	{TYPE_BOOLEAN,		"TcpNoNagle",				(ConfigValue) true},
	{TYPE_INTEGER,		"DefaultDbCachePages",		(ConfigValue) -1},			// pages
	{TYPE_INTEGER,		"ConnectionTimeout",		(ConfigValue) 180},			// seconds
	{TYPE_INTEGER,		"DummyPacketInterval",		(ConfigValue) 0},			// seconds
	{TYPE_INTEGER,		"LockMemSize",				(ConfigValue) 1048576},		// bytes
	{TYPE_BOOLEAN,		"LockGrantOrder",			(ConfigValue) true},
	{TYPE_INTEGER,		"LockHashSlots",			(ConfigValue) 1009},		// slots
	{TYPE_INTEGER,		"LockAcquireSpins",			(ConfigValue) 0},
	{TYPE_INTEGER,		"EventMemSize",				(ConfigValue) 65536},		// bytes
	{TYPE_INTEGER,		"DeadlockTimeout",			(ConfigValue) 10},			// seconds
	{TYPE_INTEGER,		"PrioritySwitchDelay",		(ConfigValue) 100},			// milliseconds
	{TYPE_BOOLEAN,		"UsePriorityScheduler",		(ConfigValue) true},
	{TYPE_INTEGER,		"PriorityBoost",			(ConfigValue) 5},			// ratio oh high- to low-priority thread ticks in jrd.cpp
	{TYPE_STRING,		"RemoteServiceName",		(ConfigValue) FB_SERVICE_NAME},
	{TYPE_INTEGER,		"RemoteServicePort",		(ConfigValue) 0},
	{TYPE_STRING,		"RemotePipeName",			(ConfigValue) FB_PIPE_NAME},
	{TYPE_STRING,		"IpcName",					(ConfigValue) FB_IPC_NAME},
#ifdef WIN_NT
	{TYPE_INTEGER,		"MaxUnflushedWrites",		(ConfigValue) 100},
	{TYPE_INTEGER,		"MaxUnflushedWriteTime",	(ConfigValue) 5},
#else
	{TYPE_INTEGER,		"MaxUnflushedWrites",		(ConfigValue) -1},
	{TYPE_INTEGER,		"MaxUnflushedWriteTime",	(ConfigValue) -1},
#endif
	{TYPE_INTEGER,		"ProcessPriorityLevel",		(ConfigValue) 0},
	{TYPE_INTEGER,		"RemoteAuxPort",			(ConfigValue) 0},
	{TYPE_STRING,		"RemoteBindAddress",		(ConfigValue) 0},
	{TYPE_STRING,		"ExternalFileAccess",		(ConfigValue) "None"},	// location(s) of external files for tables
	{TYPE_STRING,		"DatabaseAccess",			(ConfigValue) "Full"},	// location(s) of databases
#define UDF_DEFAULT_CONFIG_VALUE "Restrict UDF"
	{TYPE_STRING,		"UdfAccess",				(ConfigValue) UDF_DEFAULT_CONFIG_VALUE},	// location(s) of UDFs
	{TYPE_STRING,		"TempDirectories",			(ConfigValue) 0},
#ifdef DEV_BUILD
 	{TYPE_BOOLEAN,		"BugcheckAbort",			(ConfigValue) true},	// whether to abort() engine when internal error is found
#else
 	{TYPE_BOOLEAN,		"BugcheckAbort",			(ConfigValue) false},	// whether to abort() engine when internal error is found
#endif
	{TYPE_INTEGER,		"TraceDSQL",				(ConfigValue) 0},		// bitmask
	{TYPE_BOOLEAN,		"LegacyHash",				(ConfigValue) true},	// let use old passwd hash verification
	{TYPE_STRING,		"GCPolicy",					(ConfigValue) NULL},	// garbage collection policy
	{TYPE_BOOLEAN,		"Redirection",				(ConfigValue) false},
	{TYPE_STRING,		"Authentication",			(ConfigValue) AmNative},	// use native, trusted or mixed
	{TYPE_INTEGER,		"DatabaseGrowthIncrement",	(ConfigValue) 128 * 1048576},	// bytes
	{TYPE_INTEGER,		"FileSystemCacheThreshold",	(ConfigValue) 65536},	// page buffers
	{TYPE_BOOLEAN,		"RelaxedAliasChecking",		(ConfigValue) false},	// if true relax strict alias checking rules in DSQL a bit
	{TYPE_BOOLEAN,		"OldSetClauseSemantics",	(ConfigValue) false},	// if true disallow SET A = B, B = A to exchange column values
	{TYPE_STRING,		"AuditTraceConfigFile",		(ConfigValue) ""},		// location of audit trace configuration file
	{TYPE_INTEGER,		"MaxUserTraceLogSize",		(ConfigValue) 10},		// maximum size of user session trace log
	{TYPE_INTEGER,		"FileSystemCacheSize",		(ConfigValue) 30},		// percent
	{TYPE_STRING,		"Providers",				(ConfigValue) "Remote, Engine12, Loopback"},
	{TYPE_STRING,		"AuthServer",				(ConfigValue) "Legacy_Auth, Win_Sspi"},
	{TYPE_STRING,		"AuthClient",				(ConfigValue) "Legacy_Auth, Win_Sspi"},
	{TYPE_STRING,		"UserManager",				(ConfigValue) "Legacy_Auth"},
	{TYPE_STRING,		"TracePlugin",				(ConfigValue) "fbtrace"},
	{TYPE_STRING,		"SecurityDatabase",			(ConfigValue) "$(root)/security3.fdb"},	// security database name
	{TYPE_BOOLEAN,		"SharedCache",				(ConfigValue) true},
	{TYPE_BOOLEAN,		"SharedDatabase",			(ConfigValue) false}
};

/******************************************************************************
 *
 *	Config routines
 */

Config::Config(const ConfigFile& file)
{
	// Array to save string temporarily
	// Will be finally save by loadValues() in the end of ctor
	Firebird::ObjectsArray<ConfigFile::String> tempStrings(getPool());

	// Iterate through the known configuration entries
	for (unsigned int i = 0; i < MAX_CONFIG_KEY; i++)
	{
		values[i] = entries[i].default_value;
		if (entries[i].data_type == TYPE_STRING && values[i])
		{
			ConfigFile::String expand((const char*)values[i]);
			if (file.macroParse(expand) && expand != (const char*) values[i])
			{
				ConfigFile::String& saved(tempStrings.add());
				saved = expand;
				values[i] = (ConfigValue) saved.c_str();
			}
		}
	}

	loadValues(file);
}

Config::Config(const ConfigFile& file, const Config& base)
{
	// Iterate through the known configuration entries

	for (unsigned int i = 0; i < MAX_CONFIG_KEY; i++)
	{
		values[i] = base.values[i];
	}

	loadValues(file);
}

void Config::loadValues(const ConfigFile& file)
{
	// Iterate through the known configuration entries

	for (int i = 0; i < MAX_CONFIG_KEY; i++)
	{
		const ConfigEntry& entry = entries[i];
		const ConfigFile::String value = getValue(file, entry.key);

		if (value.length())
		{
			// Assign the actual value

			switch (entry.data_type)
			{
			case TYPE_BOOLEAN:
				values[i] = (ConfigValue) asBoolean(value);
				break;
			case TYPE_INTEGER:
				values[i] = (ConfigValue) asInteger(value);
				break;
			case TYPE_STRING:
				values[i] = (ConfigValue) asString(value);
				break;
			//case TYPE_STRING_VECTOR:
			//	break;
			}
		}

		if (entry.data_type == TYPE_STRING && values[i] != entry.default_value)
		{
			const char* src = (const char*) values[i];
			char* dst = FB_NEW(getPool()) char[strlen(src) + 1];
			strcpy(dst, src);
			values[i] = (ConfigValue) dst;
		}
	}
}

Config::~Config()
{
	// Free allocated memory

	for (int i = 0; i < MAX_CONFIG_KEY; i++)
	{
		if (values[i] == entries[i].default_value)
			continue;

		switch (entries[i].data_type)
		{
		case TYPE_STRING:
			delete[] (char*) values[i];
			break;
		//case TYPE_STRING_VECTOR:
		//	break;
		}
	}
}

ConfigFile::String Config::getValue(const ConfigFile& file, ConfigName key)
{
	const ConfigFile::Parameter* p = file.findParameter(key);
	return p ? p->value : "";
}

int Config::asInteger(const ConfigFile::String &value)
{
	return atoi(value.data());
}

bool Config::asBoolean(const ConfigFile::String &value)
{
	return (atoi(value.data()) != 0) ||
		value.equalsNoCase("true") ||
		value.equalsNoCase("yes") ||
		value.equalsNoCase("y");
}

const char* Config::asString(const ConfigFile::String &value)
{
	return value.c_str();
}

/******************************************************************************
 *
 *	Public interface
 */

const Firebird::RefPtr<Config> Config::getDefaultConfig()
{
	return firebirdConf().getDefaultConfig();
}

const char* Config::getMessage()
{
	return firebirdConf().getMessage();
}

const char* Config::getInstallDirectory()
{
	return rootDetector().getInstallDirectory();
}

static Firebird::PathName* rootFromCommandLine = 0;

void Config::setRootDirectoryFromCommandLine(const Firebird::PathName& newRoot)
{
	delete rootFromCommandLine;
	rootFromCommandLine = FB_NEW(*getDefaultMemoryPool())
		Firebird::PathName(*getDefaultMemoryPool(), newRoot);
}

const Firebird::PathName* Config::getCommandLineRootDirectory()
{
	return rootFromCommandLine;
}

const char* Config::getRootDirectory()
{
	// must check it here - command line must override any other root settings
	if (rootFromCommandLine)
	{
		return rootFromCommandLine->c_str();
	}

	return rootDetector().getRootDirectory();;
}

unsigned int Config::getKeyByName(ConfigName nm)
{
	ConfigFile::KeyType name(nm);
	for (unsigned int i = 0; i < MAX_CONFIG_KEY; i++)
	{
		if (name == entries[i].key)
		{
			return i;
		}
	}

	return ~0;
}

int Config::getInt(unsigned int key) const
{
	if (key >= MAX_CONFIG_KEY)
		return 0;
	return get<int>(static_cast<ConfigKey>(key));
}

const char* Config::getString(unsigned int key) const
{
	if (key >= MAX_CONFIG_KEY)
		return NULL;
	return get<const char*>(static_cast<ConfigKey>(key));
}

int Config::getTempBlockSize()
{
	return (int) getDefaultConfig()->values[KEY_TEMP_BLOCK_SIZE];
}

int Config::getTempCacheLimit()
{
	int v = (int) getDefaultConfig()->values[KEY_TEMP_CACHE_LIMIT];
	if (v < 0)
	{
		v = getSharedDatabase() ? 8388608 : 67108864;	// bytes
	}
	return v;
}

bool Config::getRemoteFileOpenAbility()
{
	return fb_utils::bootBuild() ? true : ((bool) getDefaultConfig()->values[KEY_REMOTE_FILE_OPEN_ABILITY]);
}

int Config::getGuardianOption()
{
	return (int) getDefaultConfig()->values[KEY_GUARDIAN_OPTION];
}

int Config::getCpuAffinityMask()
{
	return (int) getDefaultConfig()->values[KEY_CPU_AFFINITY_MASK];
}

int Config::getTcpRemoteBufferSize()
{
	int rc = (int) getDefaultConfig()->values[KEY_TCP_REMOTE_BUFFER_SIZE];
	if (rc < 1448)
		rc = 1448;
	if (rc > MAX_SSHORT)
		rc = MAX_SSHORT;
	return rc;
}

bool Config::getTcpNoNagle()
{
	return (bool) getDefaultConfig()->values[KEY_TCP_NO_NAGLE];
}

int Config::getDefaultDbCachePages() const
{
	int rc = get<int>(KEY_DEFAULT_DB_CACHE_PAGES);
	if (rc < 0)
	{
		rc = getSharedDatabase() ? 256 : 2048;	// pages
	}
	return rc;
}

int Config::getConnectionTimeout()
{
	return (int) getDefaultConfig()->values[KEY_CONNECTION_TIMEOUT];
}

int Config::getDummyPacketInterval()
{
	return (int) getDefaultConfig()->values[KEY_DUMMY_PACKET_INTERVAL];
}

int Config::getLockMemSize() const
{
	return get<int>(KEY_LOCK_MEM_SIZE);
}

bool Config::getLockGrantOrder() const
{
	return get<bool>(KEY_LOCK_GRANT_ORDER);
}

int Config::getLockHashSlots() const
{
	return get<int>(KEY_LOCK_HASH_SLOTS);
}

int Config::getLockAcquireSpins() const
{
	return get<int>(KEY_LOCK_ACQUIRE_SPINS);
}

int Config::getEventMemSize() const
{
	return get<int>(KEY_EVENT_MEM_SIZE);
}

int Config::getDeadlockTimeout() const
{
	return get<int>(KEY_DEADLOCK_TIMEOUT);
}

int Config::getPrioritySwitchDelay()
{
	int rc = (int) getDefaultConfig()->values[KEY_PRIORITY_SWITCH_DELAY];
	if (rc < 1)
		rc = 1;
	return rc;
}

int Config::getPriorityBoost()
{
	int rc = (int) getDefaultConfig()->values[KEY_PRIORITY_BOOST];
	if (rc < 1)
		rc = 1;
	if (rc > 1000)
		rc = 1000;
	return rc;
}

bool Config::getUsePriorityScheduler()
{
	return (bool) getDefaultConfig()->values[KEY_USE_PRIORITY_SCHEDULER];
}

const char *Config::getRemoteServiceName()
{
	return (const char*) getDefaultConfig()->values[KEY_REMOTE_SERVICE_NAME];
}

unsigned short Config::getRemoteServicePort()
{
	return (unsigned short) getDefaultConfig()->values[KEY_REMOTE_SERVICE_PORT];
}

const char *Config::getRemotePipeName()
{
	return (const char*) getDefaultConfig()->values[KEY_REMOTE_PIPE_NAME];
}

const char *Config::getIpcName()
{
	return (const char*) getDefaultConfig()->values[KEY_IPC_NAME];
}

int Config::getMaxUnflushedWrites() const
{
	return get<int>(KEY_MAX_UNFLUSHED_WRITES);
}

int Config::getMaxUnflushedWriteTime() const
{
	return get<int>(KEY_MAX_UNFLUSHED_WRITE_TIME);
}

int Config::getProcessPriorityLevel()
{
	return (int) getDefaultConfig()->values[KEY_PROCESS_PRIORITY_LEVEL];
}

int Config::getRemoteAuxPort()
{
	return (int) getDefaultConfig()->values[KEY_REMOTE_AUX_PORT];
}

const char *Config::getRemoteBindAddress()
{
	return (const char*) getDefaultConfig()->values[KEY_REMOTE_BIND_ADDRESS];
}

const char *Config::getExternalFileAccess() const
{
	return get<const char*>(KEY_EXTERNAL_FILE_ACCESS);
}

const char *Config::getDatabaseAccess()
{
	return (const char*) getDefaultConfig()->values[KEY_DATABASE_ACCESS];
}

const char *Config::getUdfAccess()
{
	static Firebird::GlobalPtr<Firebird::Mutex> udfMutex;
	static Firebird::GlobalPtr<Firebird::string> udfValue;
	static const char* volatile value = 0;

	if (value)
	{
		return value;
	}

	Firebird::MutexLockGuard guard(udfMutex);

	if (value)
	{
		return value;
	}

	const char* v = (const char*) getDefaultConfig()->values[KEY_UDF_ACCESS];
	if (CASE_SENSITIVITY ? (! strcmp(v, UDF_DEFAULT_CONFIG_VALUE) && FB_UDFDIR[0]) :
						   (! fb_utils::stricmp(v, UDF_DEFAULT_CONFIG_VALUE) && FB_UDFDIR[0]))
	{
		udfValue->printf("Restrict %s", FB_UDFDIR);
		value = udfValue->c_str();
	}
	else
	{
		value = v;
	}
	return value;
}

const char *Config::getTempDirectories()
{
	return (const char*) getDefaultConfig()->values[KEY_TEMP_DIRECTORIES];
}

bool Config::getBugcheckAbort()
{
	return (bool) getDefaultConfig()->values[KEY_BUGCHECK_ABORT];
}

int Config::getTraceDSQL()
{
	return (int) getDefaultConfig()->values[KEY_TRACE_DSQL];
}

bool Config::getLegacyHash()
{
	return (bool) getDefaultConfig()->values[KEY_LEGACY_HASH];
}

const char *Config::getGCPolicy() const
{
	const char* rc = get<const char*>(KEY_GC_POLICY);

	if (rc)
	{
		if (strcmp(rc, GCPolicyCooperative) != 0 &&
			strcmp(rc, GCPolicyBackground) != 0 &&
			strcmp(rc, GCPolicyCombined) != 0)
		{
			// user-provided value is invalid - fail to default
			rc = NULL;
		}
	}

	if (! rc)
	{
		rc = getSharedCache() ? GCPolicyCombined : GCPolicyCooperative;
	}

	return rc;
}

bool Config::getRedirection()
{
	return (bool) getDefaultConfig()->values[KEY_REDIRECTION];
}

const char *Config::getAuthMethod()
{
	return (const char*) getDefaultConfig()->values[KEY_AUTH_METHOD];
}

int Config::getDatabaseGrowthIncrement() const
{
	return get<int>(KEY_DATABASE_GROWTH_INCREMENT);
}

int Config::getFileSystemCacheThreshold() const
{
	int rc = get<int>(KEY_FILESYSTEM_CACHE_THRESHOLD);
	return rc < 0 ? 0 : rc;
}

bool Config::getRelaxedAliasChecking()
{
	return (bool) getDefaultConfig()->values[KEY_RELAXED_ALIAS_CHECKING];
}

bool Config::getOldSetClauseSemantics()
{
	return (bool) getDefaultConfig()->values[KEY_OLD_SET_CLAUSE_SEMANTICS];
}

int Config::getFileSystemCacheSize()
{
	return (int) getDefaultConfig()->values[KEY_FILESYSTEM_CACHE_SIZE];
}

const char *Config::getAuditTraceConfigFile()
{
	return (const char*) getDefaultConfig()->values[KEY_TRACE_CONFIG];
}

int Config::getMaxUserTraceLogSize()
{
	return (int) getDefaultConfig()->values[KEY_MAX_TRACELOG_SIZE];
}

bool Config::getSharedCache()
{
	return (bool) getDefaultConfig()->values[KEY_SHARED_CACHE];
}

bool Config::getSharedDatabase()
{
	return (bool) getDefaultConfig()->values[KEY_SHARED_DATABASE];
}

bool Config::getMultiClientServer()
{
	// AP - absolutely wrong for superclassic assumption
	// should be set by server in case of 'super' mode in it
#ifdef SUPERSERVER
	return true;
#else
	return false;
#endif
}

const char* Config::getPlugins(unsigned int type)
{
	switch (type)
	{
		case Firebird::PluginType::Provider:
			return (const char*) getDefaultConfig()->values[KEY_PLUG_PROVIDERS];
		case Firebird::PluginType::AuthServer:
			return (const char*) getDefaultConfig()->values[KEY_PLUG_AUTH_SERVER];
		case Firebird::PluginType::AuthClient:
			return (const char*) getDefaultConfig()->values[KEY_PLUG_AUTH_CLIENT];
		case Firebird::PluginType::AuthUserManagement:
			return (const char*) getDefaultConfig()->values[KEY_PLUG_AUTH_MANAGE];
		case Firebird::PluginType::Trace:
			return (const char*) getDefaultConfig()->values[KEY_PLUG_TRACE];
	}

	(Firebird::Arg::Gds(isc_random) << "Internal error in getPlugins()").raise();
	return NULL;		// compiler warning silencer
}

unsigned int FB_CARG FirebirdConf::getKey(const char* name)
{
	return Config::getKeyByName(name);
}

int FB_CARG FirebirdConf::asInteger(unsigned int key)
{
	return config->getInt(key);
}

const char* FB_CARG FirebirdConf::asString(unsigned int key)
{
	return config->getString(key);
}

int FB_CARG FirebirdConf::release()
{
	if (--refCounter == 0)
	{
		delete this;
		return 0;
	}

	return 1;
}

const char* Config::getSecurityDatabase() const
{
	return get<const char*>(KEY_SECURITY_DATABASE);
}
