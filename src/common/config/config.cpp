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
#include "../common/classes/init.h"
#include "../common/dllinst.h"
#include "../common/os/fbsyslog.h"
#include "../common/utils_proto.h"
#include "../jrd/constants.h"
#include "firebird/Interface.h"
#include "../common/db_alias.h"
#include "../jrd/build_no.h"

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

// NS 2014-07-23 FIXME: Rework error handling
// 1. We shall not silently truncate upper bits of integer values read from configuration files.
// 2. Invalid configuration file values that we ignored shall leave trace in firebird.log
//    to avoid user confusion.
// 3. Incorrect syntax for parameter values shall not be ignored silently
// 4. Integer overflow during parsing of parameter value shall not be ignored silently
//
// Currently user can only guess which parameter values have been applied by the engine
// and which were ignored. Or resort to reading source code and using debugger to find out.

namespace {

/******************************************************************************
 *
 *	firebird.conf implementation
 */

class ConfigImpl : public Firebird::PermanentStorage
{
public:
	explicit ConfigImpl(Firebird::MemoryPool& p)
		: Firebird::PermanentStorage(p), missConf(false)
	{
		try
		{
			ConfigFile file(fb_utils::getPrefix(Firebird::IConfigManager::DIR_CONF, Firebird::CONFIG_FILE),
				ConfigFile::ERROR_WHEN_MISS);
			defaultConfig = FB_NEW Firebird::Config(file);
		}
		catch (const Firebird::status_exception& ex)
		{
			if (ex.value()[1] != isc_miss_config)
			{
				throw;
			}

			missConf = true;

			ConfigFile file(ConfigFile::USE_TEXT, "");
			defaultConfig = FB_NEW Firebird::Config(file);
		}
	}

	/***
	It was a kind of getting ready for changing config remotely...

	void changeDefaultConfig(Firebird::Config* newConfig)
	{
		defaultConfig = newConfig;
	}
	***/

	Firebird::RefPtr<const Firebird::Config>& getDefaultConfig()
	{
		return defaultConfig;
	}

	bool missFirebirdConf() const
	{
		return missConf;
	}

	Firebird::IFirebirdConf* getFirebirdConf()
	{
		Firebird::IFirebirdConf* rc = FB_NEW Firebird::FirebirdConf(defaultConfig);
		rc->addRef();
		return rc;
	}

private:
	Firebird::RefPtr<const Firebird::Config> defaultConfig;

    ConfigImpl(const ConfigImpl&);
    void operator=(const ConfigImpl&);

	bool missConf;
};

/******************************************************************************
 *
 *	Static instance of the system configuration file
 */

Firebird::InitInstance<ConfigImpl> firebirdConf;

}	// anonymous namespace

namespace Firebird
{

IFirebirdConf* getFirebirdConfig()
{
	return firebirdConf().getFirebirdConf();
}

/******************************************************************************
 *
 *	Configuration entries
 */

const char*	GCPolicyCooperative	= "cooperative";
const char*	GCPolicyBackground	= "background";
const char*	GCPolicyCombined	= "combined";


Config::ConfigEntry Config::entries[MAX_CONFIG_KEY] =
{
	{TYPE_INTEGER,		"TempBlockSize",			ConfigValue(1048576)},	// bytes
	{TYPE_INTEGER,		"TempCacheLimit",			ConfigValue(-1)},		// bytes
	{TYPE_BOOLEAN,		"RemoteFileOpenAbility",	ConfigValue(false)},
	{TYPE_INTEGER,		"GuardianOption",			ConfigValue(1)},
	{TYPE_INTEGER,		"CpuAffinityMask",			ConfigValue(0)},
	{TYPE_INTEGER,		"TcpRemoteBufferSize",		ConfigValue(8192)},		// bytes
	{TYPE_BOOLEAN,		"TcpNoNagle",				ConfigValue(true)},
	{TYPE_BOOLEAN,		"TcpLoopbackFastPath",      ConfigValue(true)},
	{TYPE_INTEGER,		"DefaultDbCachePages",		ConfigValue(-1)},		// pages
	{TYPE_INTEGER,		"ConnectionTimeout",		ConfigValue(180)},		// seconds
	{TYPE_INTEGER,		"DummyPacketInterval",		ConfigValue(0)},		// seconds
	{TYPE_STRING,		"DefaultTimeZone",			ConfigValue(nullptr)},
	{TYPE_INTEGER,		"LockMemSize",				ConfigValue(1048576)},	// bytes
	{TYPE_INTEGER,		"LockHashSlots",			ConfigValue(8191)},		// slots
	{TYPE_INTEGER,		"LockAcquireSpins",			ConfigValue(0)},
	{TYPE_INTEGER,		"EventMemSize",				ConfigValue(65536)},	// bytes
	{TYPE_INTEGER,		"DeadlockTimeout",			ConfigValue(10)},		// seconds
	{TYPE_STRING,		"RemoteServiceName",		ConfigValue(FB_SERVICE_NAME)},
	{TYPE_INTEGER,		"RemoteServicePort",		ConfigValue(0)},
	{TYPE_STRING,		"RemotePipeName",			ConfigValue(FB_PIPE_NAME)},
	{TYPE_STRING,		"IpcName",					ConfigValue(FB_IPC_NAME)},
#ifdef WIN_NT
	{TYPE_INTEGER,		"MaxUnflushedWrites",		ConfigValue(100)},
	{TYPE_INTEGER,		"MaxUnflushedWriteTime",	ConfigValue(5)},
#else
	{TYPE_INTEGER,		"MaxUnflushedWrites",		ConfigValue(-1)},
	{TYPE_INTEGER,		"MaxUnflushedWriteTime",	ConfigValue(-1)},
#endif
	{TYPE_INTEGER,		"ProcessPriorityLevel",		ConfigValue(0)},
	{TYPE_INTEGER,		"RemoteAuxPort",			ConfigValue(0)},
	{TYPE_STRING,		"RemoteBindAddress",		ConfigValue(0)},
	{TYPE_STRING,		"ExternalFileAccess",		ConfigValue("None")},	// location(s) of external files for tables
	{TYPE_STRING,		"DatabaseAccess",			ConfigValue("Full")},	// location(s) of databases
	{TYPE_STRING,		"UdfAccess",				ConfigValue("None")},	// location(s) of UDFs
	{TYPE_STRING,		"TempDirectories",			ConfigValue(0)},
#ifdef DEV_BUILD
 	{TYPE_BOOLEAN,		"BugcheckAbort",			ConfigValue(true)},		// whether to abort() engine when internal error is found
#else
 	{TYPE_BOOLEAN,		"BugcheckAbort",			ConfigValue(false)},	// whether to abort() engine when internal error is found
#endif
	{TYPE_INTEGER,		"TraceDSQL",				ConfigValue(0)},		// bitmask
	{TYPE_BOOLEAN,		"LegacyHash",				ConfigValue(true)},		// let use old passwd hash verification
	{TYPE_STRING,		"GCPolicy",					ConfigValue(nullptr)},	// garbage collection policy
	{TYPE_BOOLEAN,		"Redirection",				ConfigValue(false)},
	{TYPE_INTEGER,		"DatabaseGrowthIncrement",	ConfigValue(128 * 1048576)},	// bytes
	{TYPE_INTEGER,		"FileSystemCacheThreshold",	ConfigValue(65536)},	// page buffers
	{TYPE_BOOLEAN,		"RelaxedAliasChecking",		ConfigValue(false)},	// if true relax strict alias checking rules in DSQL a bit
	{TYPE_STRING,		"AuditTraceConfigFile",		ConfigValue("")},		// location of audit trace configuration file
	{TYPE_INTEGER,		"MaxUserTraceLogSize",		ConfigValue(10)},		// maximum size of user session trace log
	{TYPE_INTEGER,		"FileSystemCacheSize",		ConfigValue(0)},		// percent
	{TYPE_STRING,		"Providers",				ConfigValue("Remote, " CURRENT_ENGINE ", Loopback")},
	{TYPE_STRING,		"AuthServer",				ConfigValue("Srp256")},
#ifdef WIN_NT
	{TYPE_STRING,		"AuthClient",				ConfigValue("Srp256, Srp, Win_Sspi, Legacy_Auth")},
#else
	{TYPE_STRING,		"AuthClient",				ConfigValue("Srp256, Srp, Legacy_Auth")},
#endif
	{TYPE_STRING,		"UserManager",				ConfigValue("Srp")},
	{TYPE_STRING,		"TracePlugin",				ConfigValue("fbtrace")},
	{TYPE_STRING,		"SecurityDatabase",			ConfigValue(nullptr)},	// sec/db alias - rely on ConfigManager::getDefaultSecurityDb()
	{TYPE_STRING,		"ServerMode",				ConfigValue(nullptr)},	// actual value differs in boot/regular cases and set at setupDefaultConfig()
	{TYPE_STRING,		"WireCrypt",				ConfigValue(nullptr)},
	{TYPE_STRING,		"WireCryptPlugin",			ConfigValue("ChaCha, Arc4")},
	{TYPE_STRING,		"KeyHolderPlugin",			ConfigValue("")},
	{TYPE_BOOLEAN,		"RemoteAccess",				ConfigValue(true)},
	{TYPE_BOOLEAN,		"IPv6V6Only",				ConfigValue(false)},
	{TYPE_BOOLEAN,		"WireCompression",			ConfigValue(false)},
	{TYPE_INTEGER,		"MaxIdentifierByteLength",	ConfigValue(MAX_SQL_IDENTIFIER_LEN)},
	{TYPE_INTEGER,		"MaxIdentifierCharLength",	ConfigValue(METADATA_IDENTIFIER_CHAR_LEN)},
	{TYPE_BOOLEAN,		"AllowEncryptedSecurityDatabase", ConfigValue(false)},
	{TYPE_INTEGER,		"StatementTimeout",			ConfigValue(0)},
	{TYPE_INTEGER,		"ConnectionIdleTimeout",	ConfigValue(0)},
	{TYPE_INTEGER,		"ClientBatchBuffer",		ConfigValue((128 * 1024))},
#ifdef DEV_BUILD
	{TYPE_STRING,		"OutputRedirectionFile", 	ConfigValue("-")},
#else
#ifdef WIN_NT
	{TYPE_STRING,		"OutputRedirectionFile", 	ConfigValue("nul")},
#else
	{TYPE_STRING,		"OutputRedirectionFile", 	ConfigValue("/dev/null")},
#endif
#endif
	{TYPE_INTEGER,		"ExtConnPoolSize",			ConfigValue(0)},
	{TYPE_INTEGER,		"ExtConnPoolLifeTime",		ConfigValue(7200)},
	{TYPE_INTEGER,		"SnapshotsMemSize",			ConfigValue(65536)},	// bytes
	{TYPE_INTEGER,		"TipCacheBlockSize",		ConfigValue(4194304)},	// bytes
	{TYPE_BOOLEAN,		"ReadConsistency",			ConfigValue(true)},
	{TYPE_BOOLEAN,		"ClearGTTAtRetaining",		ConfigValue(false)},
	{TYPE_STRING,		"DataTypeCompatibility",	ConfigValue(nullptr)},
	{TYPE_BOOLEAN,		"UseFileSystemCache",		ConfigValue(true)}
};

/******************************************************************************
 *
 *	Config routines
 */

Config::Config(const ConfigFile& file)
	: notifyDatabase(*getDefaultMemoryPool()), 
	serverMode(-1)
{
	memset(bits, 0, sizeof(bits));

	setupDefaultConfig();

	// Array to save string temporarily
	// Will be finally saved by loadValues() in the end of ctor
	ObjectsArray<ConfigFile::String> tempStrings(getPool());

	// Iterate through the known configuration entries
	for (unsigned int i = 0; i < MAX_CONFIG_KEY; i++)
	{
		values[i] = entries[i].default_value;
		if (entries[i].data_type == TYPE_STRING && values[i].strVal)
		{
			ConfigFile::String expand(values[i].strVal);
			if (file.macroParse(expand, NULL) && expand != values[i].strVal)
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
	: notifyDatabase(*getDefaultMemoryPool()), 
	serverMode(-1)
{
	memset(bits, 0, sizeof(bits));

	// Iterate through the known configuration entries

	for (unsigned int i = 0; i < MAX_CONFIG_KEY; i++)
	{
		values[i] = base.values[i];
		if (base.testKey(i))
			setKey(i);
	}

	loadValues(file);
}

Config::Config(const ConfigFile& file, const Config& base, const PathName& notify)
	: notifyDatabase(*getDefaultMemoryPool()), 
	serverMode(-1)
{
	memset(bits, 0, sizeof(bits));

	// Iterate through the known configuration entries

	for (unsigned int i = 0; i < MAX_CONFIG_KEY; i++)
	{
		values[i] = base.values[i];
		if (base.testKey(i))
			setKey(i);
	}

	loadValues(file);

	notifyDatabase = notify;
}

void Config::notify() const
{
	if (!notifyDatabase.hasData())
		return;
	if (notifyDatabaseName(notifyDatabase))
		notifyDatabase.erase();
}

void Config::merge(RefPtr<const Config>& config, const string* dpbConfig)
{
	if (dpbConfig && dpbConfig->hasData())
	{
		ConfigFile txtStream(ConfigFile::USE_TEXT, dpbConfig->c_str());
		config = FB_NEW Config(txtStream, *(config.hasData() ? config : getDefaultConfig()));
	}
}

void Config::loadValues(const ConfigFile& file)
{
	// Iterate through the known configuration entries

	for (int i = 0; i < MAX_CONFIG_KEY; i++)
	{
		const ConfigEntry& entry = entries[i];
		const ConfigFile::Parameter* par = file.findParameter(entry.key);

		if (par)
		{
			// Assign the actual value

			switch (entry.data_type)
			{
			case TYPE_BOOLEAN:
				values[i] = (ConfigValue) par->asBoolean();
				break;
			case TYPE_INTEGER:
				values[i] = (ConfigValue) par->asInteger();
				break;
			case TYPE_STRING:
				values[i] = (ConfigValue) par->value.c_str();
				break;
			//case TYPE_STRING_VECTOR:
			//	break;
			}

			setKey(i);
		}

		if (entry.data_type == TYPE_STRING && values[i] != entry.default_value)
		{
			const char* src = values[i].strVal;
			char* dst = FB_NEW_POOL(getPool()) char[strlen(src) + 1];
			strcpy(dst, src);
			values[i] = (ConfigValue) dst;
		}
	}

	checkValues();
}

static const char* txtServerModes[6] = 
{
	"Super", "ThreadedDedicated", 
	"SuperClassic", "ThreadedShared", 
	"Classic", "MultiProcess" 
};

void Config::setupDefaultConfig()
{
	const bool bootBuild = fb_utils::bootBuild();

	ConfigValue* pDefault = &entries[KEY_SERVER_MODE].default_value;
	serverMode = bootBuild ? MODE_CLASSIC : MODE_SUPER;
	pDefault->strVal = txtServerModes[2 * serverMode];

	pDefault = &entries[KEY_TEMP_CACHE_LIMIT].default_value;
	if (pDefault->intVal < 0)
		pDefault->intVal = (serverMode != MODE_SUPER) ? 8388608 : 67108864;	// bytes

	entries[KEY_REMOTE_FILE_OPEN_ABILITY].default_value.boolVal = bootBuild;

	pDefault = &entries[KEY_DEFAULT_DB_CACHE_PAGES].default_value;
	if (pDefault->intVal < 0)
		pDefault->intVal = (serverMode != MODE_SUPER) ? 256 : 2048;	// pages

	pDefault = &entries[KEY_GC_POLICY].default_value;
	if (!pDefault->strVal)
	{
		pDefault->strVal = (serverMode == MODE_SUPER) ? GCPolicyCombined : GCPolicyCooperative;
	}

	//pDefault = &entries[KEY_SECURITY_DATABASE].default_value;

	//pDefault = &entries[KEY_WIRE_CRYPT].default_value;
//	if (!*pDefault)
//		*pDefault == (ConfigValue) (xxx == WC_CLIENT) ? WIRE_CRYPT_ENABLED : WIRE_CRYPT_REQUIRED;

}

void Config::checkIntForLoBound(ConfigKey key, SINT64 loBound, bool setDefault)
{
	fb_assert(entries[key].data_type == TYPE_INTEGER);
	if (values[key].intVal < loBound)
		values[key].intVal = setDefault ? entries[key].default_value.intVal : loBound;
}

void Config::checkIntForHiBound(ConfigKey key, SINT64 hiBound, bool setDefault)
{
	fb_assert(entries[key].data_type == TYPE_INTEGER);
	if (values[key].intVal > hiBound)
		values[key].intVal = setDefault ? entries[key].default_value.intVal : hiBound;
}

void Config::checkValues()
{
	checkIntForLoBound(KEY_TEMP_CACHE_LIMIT, 0, true);

	checkIntForLoBound(KEY_TCP_REMOTE_BUFFER_SIZE, 1448, false);
	checkIntForHiBound(KEY_TCP_REMOTE_BUFFER_SIZE, MAX_SSHORT, false);

	checkIntForLoBound(KEY_DEFAULT_DB_CACHE_PAGES, 0, true);

	checkIntForLoBound(KEY_LOCK_MEM_SIZE, 64 * 1024, false);

	const char* strVal = values[KEY_GC_POLICY].strVal;
	if (strVal)
	{
		NoCaseString gcPolicy(strVal);
		if (gcPolicy != GCPolicyCooperative &&
			gcPolicy != GCPolicyBackground &&
			gcPolicy != GCPolicyCombined)
		{
			// user-provided value is invalid - fail to default
			values[KEY_GC_POLICY] = entries[KEY_GC_POLICY].default_value;
		}
	}

	strVal = values[KEY_WIRE_CRYPT].strVal;
	if (strVal)
	{
		NoCaseString wireCrypt(strVal);
		if (wireCrypt != "DISABLED" && wireCrypt != "ENABLED" && wireCrypt != "REQUIRED")
		{
			// user-provided value is invalid - fail to default
			values[KEY_WIRE_CRYPT] = entries[KEY_WIRE_CRYPT].default_value;
		}
	}

	strVal = values[KEY_SERVER_MODE].strVal;
	if (strVal && !fb_utils::bootBuild())
	{
		bool found = false;
		NoCaseString mode(strVal);
		for (int x = 0; x < 6; ++x)
		{
			if (mode == txtServerModes[x])
			{
				serverMode = x / 2;
				found = true;
				break;
			}
		}

		if (!found)
			values[KEY_SERVER_MODE] = entries[KEY_SERVER_MODE].default_value;
	}

	checkIntForLoBound(KEY_FILESYSTEM_CACHE_THRESHOLD, 0, true);

	checkIntForLoBound(KEY_MAX_IDENTIFIER_BYTE_LENGTH, 1, true);
	checkIntForHiBound(KEY_MAX_IDENTIFIER_BYTE_LENGTH, MAX_SQL_IDENTIFIER_LEN, true);

	checkIntForLoBound(KEY_MAX_IDENTIFIER_CHAR_LENGTH, 1, true);
	checkIntForHiBound(KEY_MAX_IDENTIFIER_CHAR_LENGTH, METADATA_IDENTIFIER_CHAR_LEN, true);

	checkIntForLoBound(KEY_SNAPSHOTS_MEM_SIZE, 1, true);
	checkIntForHiBound(KEY_SNAPSHOTS_MEM_SIZE, MAX_ULONG, true);

	checkIntForLoBound(KEY_TIP_CACHE_BLOCK_SIZE, 1, true);
	checkIntForHiBound(KEY_TIP_CACHE_BLOCK_SIZE, MAX_ULONG, true);
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
			delete[] values[i].strVal;
			break;
		//case TYPE_STRING_VECTOR:
		//	break;
		}
	}
}


/******************************************************************************
 *
 *	Public interface
 */

const RefPtr<const Config>& Config::getDefaultConfig()
{
	return firebirdConf().getDefaultConfig();
}

bool Config::missFirebirdConf()
{
	return firebirdConf().missFirebirdConf();
}

const char* Config::getInstallDirectory()
{
	return fb_get_master_interface()->getConfigManager()->getInstallDirectory();
}

static PathName* rootFromCommandLine = 0;

void Config::setRootDirectoryFromCommandLine(const PathName& newRoot)
{
	delete rootFromCommandLine;
	rootFromCommandLine = FB_NEW_POOL(*getDefaultMemoryPool())
		PathName(*getDefaultMemoryPool(), newRoot);
}

const PathName* Config::getCommandLineRootDirectory()
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

	return fb_get_master_interface()->getConfigManager()->getRootDirectory();
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

SINT64 Config::getInt(unsigned int key) const
{
	if (key >= MAX_CONFIG_KEY)
		return 0;
	return getInt(static_cast<ConfigKey>(key));
}

const char* Config::getString(unsigned int key) const
{
	if (key >= MAX_CONFIG_KEY)
		return NULL;

	// irregular case
	switch(key)
	{
	case KEY_SECURITY_DATABASE:
		return getSecurityDatabase();
	}

	return getStr(static_cast<ConfigKey>(key));
}

bool Config::getBoolean(unsigned int key) const
{
	if (key >= MAX_CONFIG_KEY)
		return false;
	return getBool(static_cast<ConfigKey>(key));
}

bool Config::valueAsString(ConfigValue val, ConfigType type, string& str)
{
	switch (type)
	{
	case Config::TYPE_INTEGER:
		str.printf("%" SQUADFORMAT, val.intVal);
		break;

	case Config::TYPE_STRING:
	{
		if (val.strVal == NULL)
			return false;

		str = val.strVal;
	}
	break;

	case Config::TYPE_BOOLEAN:
		str = val.boolVal ? "true" : "false";
		break;
	}

	return true;
}

bool Config::getValue(unsigned int key, string& str) const
{
	if (key >= MAX_CONFIG_KEY)
		return false;

	return valueAsString(values[key], entries[key].data_type, str);
}

bool Config::getDefaultValue(unsigned int key, string& str)
{
	if (key >= MAX_CONFIG_KEY)
		return false;

	if (key == KEY_WIRE_CRYPT && !entries[key].default_value.strVal)
	{
		str = "Required";	// see getWireCrypt(WC_SERVER)
		return true;
	}

	return valueAsString(entries[key].default_value, entries[key].data_type, str);
}


int Config::getTempBlockSize()
{
	return (int) getDefaultConfig()->values[KEY_TEMP_BLOCK_SIZE].intVal;
}

FB_UINT64 Config::getTempCacheLimit() const
{
	return  getInt(KEY_TEMP_CACHE_LIMIT);
}

bool Config::getRemoteFileOpenAbility()
{
	return getDefaultConfig()->getBool(KEY_REMOTE_FILE_OPEN_ABILITY);
}

int Config::getGuardianOption()
{
	return getDefaultConfig()->getInt(KEY_GUARDIAN_OPTION);
}

FB_UINT64 Config::getCpuAffinityMask()
{
	return getDefaultConfig()->getInt(KEY_CPU_AFFINITY_MASK);
}

int Config::getTcpRemoteBufferSize()
{
	return getDefaultConfig()->getInt(KEY_TCP_REMOTE_BUFFER_SIZE);
}

bool Config::getTcpNoNagle() const
{
	return getBool(KEY_TCP_NO_NAGLE);
}

bool Config::getTcpLoopbackFastPath() const
{
	return getBool(KEY_TCP_LOOPBACK_FAST_PATH);
}

bool Config::getIPv6V6Only() const
{
	return getBool(KEY_IPV6_V6ONLY);
}

int Config::getDefaultDbCachePages() const
{
	return getInt(KEY_DEFAULT_DB_CACHE_PAGES);
}

int Config::getConnectionTimeout() const
{
	return getInt(KEY_CONNECTION_TIMEOUT);
}

int Config::getDummyPacketInterval() const
{
	return getInt(KEY_DUMMY_PACKET_INTERVAL);
}

const char* Config::getDefaultTimeZone()
{
	return getDefaultConfig()->getStr(KEY_DEFAULT_TIME_ZONE);
}

int Config::getLockMemSize() const
{
	return getInt(KEY_LOCK_MEM_SIZE);
}

int Config::getLockHashSlots() const
{
	return getInt(KEY_LOCK_HASH_SLOTS);
}

int Config::getLockAcquireSpins() const
{
	return getInt(KEY_LOCK_ACQUIRE_SPINS);
}

int Config::getEventMemSize() const
{
	return getInt(KEY_EVENT_MEM_SIZE);
}

int Config::getDeadlockTimeout() const
{
	return getInt(KEY_DEADLOCK_TIMEOUT);
}

const char *Config::getRemoteServiceName() const
{
	return getStr(KEY_REMOTE_SERVICE_NAME);
}

unsigned short Config::getRemoteServicePort() const
{
	return getInt(KEY_REMOTE_SERVICE_PORT);
}

const char *Config::getRemotePipeName() const
{
	return getStr(KEY_REMOTE_PIPE_NAME);
}

const char *Config::getIpcName() const
{
	return getStr(KEY_IPC_NAME);
}

int Config::getMaxUnflushedWrites() const
{
	return getInt(KEY_MAX_UNFLUSHED_WRITES);
}

int Config::getMaxUnflushedWriteTime() const
{
	return getInt(KEY_MAX_UNFLUSHED_WRITE_TIME);
}

int Config::getProcessPriorityLevel()
{
	return getDefaultConfig()->getInt(KEY_PROCESS_PRIORITY_LEVEL);
}

int Config::getRemoteAuxPort() const
{
	return getInt(KEY_REMOTE_AUX_PORT);
}

const char *Config::getRemoteBindAddress()
{
	return getDefaultConfig()->getStr(KEY_REMOTE_BIND_ADDRESS);
}

const char *Config::getExternalFileAccess() const
{
	return getStr(KEY_EXTERNAL_FILE_ACCESS);
}

const char *Config::getDatabaseAccess()
{
	return getDefaultConfig()->getStr(KEY_DATABASE_ACCESS);
}

const char *Config::getUdfAccess()
{
	return getDefaultConfig()->getStr(KEY_UDF_ACCESS);
}

const char *Config::getTempDirectories()
{
	return getDefaultConfig()->getStr(KEY_TEMP_DIRECTORIES);
}

bool Config::getBugcheckAbort()
{
	return getDefaultConfig()->getBool(KEY_BUGCHECK_ABORT);
}

int Config::getTraceDSQL()
{
	return getDefaultConfig()->getInt(KEY_TRACE_DSQL);
}

bool Config::getLegacyHash()
{
	return getDefaultConfig()->getBool(KEY_LEGACY_HASH);
}

const char *Config::getGCPolicy() const
{
	return getStr(KEY_GC_POLICY);
}

bool Config::getRedirection()
{
	return getDefaultConfig()->getBool(KEY_REDIRECTION);
}

int Config::getDatabaseGrowthIncrement() const
{
	return getInt(KEY_DATABASE_GROWTH_INCREMENT);
}

int Config::getFileSystemCacheThreshold() const
{
	return getInt(KEY_FILESYSTEM_CACHE_THRESHOLD);
}

bool Config::getRelaxedAliasChecking()
{
	return getDefaultConfig()->getBool(KEY_RELAXED_ALIAS_CHECKING);
}

FB_UINT64 Config::getFileSystemCacheSize()
{
	return (FB_UINT64) getDefaultConfig()->getInt(KEY_FILESYSTEM_CACHE_SIZE);
}

const char *Config::getAuditTraceConfigFile()
{
	return getDefaultConfig()->getStr(KEY_TRACE_CONFIG);
}

FB_UINT64 Config::getMaxUserTraceLogSize()
{
	return (FB_UINT64) getDefaultConfig()->getInt(KEY_MAX_TRACELOG_SIZE);
}

int Config::getServerMode()
{
	return getDefaultConfig()->serverMode;
}

ULONG Config::getSnapshotsMemSize() const
{
	return getInt(KEY_SNAPSHOTS_MEM_SIZE);
}

ULONG Config::getTipCacheBlockSize() const
{
	return getInt(KEY_TIP_CACHE_BLOCK_SIZE);
}

const char* Config::getPlugins(unsigned int type) const
{
	ConfigKey key;
	switch (type)
	{
		case IPluginManager::TYPE_PROVIDER:
			key = KEY_PLUG_PROVIDERS;
			break;
		case IPluginManager::TYPE_AUTH_SERVER:
			key = KEY_PLUG_AUTH_SERVER;
			break;
		case IPluginManager::TYPE_AUTH_CLIENT:
			key = KEY_PLUG_AUTH_CLIENT;
			break;
		case IPluginManager::TYPE_AUTH_USER_MANAGEMENT:
			key = KEY_PLUG_AUTH_MANAGE;
			break;
		case IPluginManager::TYPE_TRACE:
			key = KEY_PLUG_TRACE;
			break;
		case IPluginManager::TYPE_WIRE_CRYPT:
			key = KEY_PLUG_WIRE_CRYPT;
			break;
		case IPluginManager::TYPE_KEY_HOLDER:
			key = KEY_PLUG_KEY_HOLDER;
			break;

		default:
			(Arg::Gds(isc_random) << "Internal error in Config::getPlugins(): unknown plugin type requested").raise();
	}

	return getStr(key);
}


// array format: major, minor, release, build
static unsigned short fileVerNumber[4] = {FILE_VER_NUMBER};

static inline unsigned int getPartialVersion()
{
			// major				   // minor
	return (fileVerNumber[0] << 24) | (fileVerNumber[1] << 16);
}

static inline unsigned int getFullVersion()
{
								 // build_no
	return getPartialVersion() | fileVerNumber[3];
}

static unsigned int PARTIAL_MASK = 0xFFFF0000;
static unsigned int KEY_MASK = 0xFFFF;

static inline void checkKey(unsigned int& key)
{
	if ((key & PARTIAL_MASK) != getPartialVersion())
		key = KEY_MASK;
	else
		key &= KEY_MASK;
}

unsigned int FirebirdConf::getVersion(CheckStatusWrapper* status)
{
	return getFullVersion();
}

unsigned int FirebirdConf::getKey(const char* name)
{
	return Config::getKeyByName(name) | getPartialVersion();
}

ISC_INT64 FirebirdConf::asInteger(unsigned int key)
{
	checkKey(key);
	return config->getInt(key);
}

const char* FirebirdConf::asString(unsigned int key)
{
	checkKey(key);
	return config->getString(key);
}

FB_BOOLEAN FirebirdConf::asBoolean(unsigned int key)
{
	checkKey(key);
	return config->getBoolean(key);
}

int FirebirdConf::release()
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
	const char* strVal = getStr(KEY_SECURITY_DATABASE);
	if (!strVal)
	{
		strVal = MasterInterfacePtr()->getConfigManager()->getDefaultSecurityDb();
		if (!strVal)
			strVal = "security.db";
	}

	return strVal;
}

int Config::getWireCrypt(WireCryptMode wcMode) const
{
	bool present;
	const char* wc = getStr(KEY_WIRE_CRYPT, &present);
	if (present && wc)
	{
		NoCaseString wireCrypt(wc);
		if (wireCrypt == "DISABLED")
			return WIRE_CRYPT_DISABLED;
		if (wireCrypt == "ENABLED")
			return WIRE_CRYPT_ENABLED;
		if (wireCrypt == "REQUIRED")
			return WIRE_CRYPT_REQUIRED;

		// wrong user value, fail to default
		// should not happens, see checkValues()
		fb_assert(false);
	}

	return wcMode == WC_CLIENT ? WIRE_CRYPT_ENABLED : WIRE_CRYPT_REQUIRED;
}

bool Config::getRemoteAccess() const
{
	return getBool(KEY_REMOTE_ACCESS);
}

bool Config::getWireCompression() const
{
	return getBool(KEY_WIRE_COMPRESSION);
}

int Config::getMaxIdentifierByteLength() const
{
	return getInt(KEY_MAX_IDENTIFIER_BYTE_LENGTH);
}

int Config::getMaxIdentifierCharLength() const
{
	return getInt(KEY_MAX_IDENTIFIER_CHAR_LENGTH);
}

bool Config::getCryptSecurityDatabase() const
{
	return getBool(KEY_ENCRYPT_SECURITY_DATABASE);
}

unsigned int Config::getStatementTimeout() const
{
	return getInt(KEY_STMT_TIMEOUT);
}

unsigned int Config::getConnIdleTimeout() const
{
	return getInt(KEY_CONN_IDLE_TIMEOUT);
}

unsigned int Config::getClientBatchBuffer() const
{
	return getInt(KEY_CLIENT_BATCH_BUFFER);
}

const char* Config::getOutputRedirectionFile()
{
	return getDefaultConfig()->getStr(KEY_OUTPUT_REDIRECTION_FILE);
}

int Config::getExtConnPoolSize()
{
	return getDefaultConfig()->getInt(KEY_EXT_CONN_POOL_SIZE);
}

int Config::getExtConnPoolLifeTime()
{
	return getDefaultConfig()->getInt(KEY_EXT_CONN_POOL_LIFETIME);
}

bool Config::getReadConsistency() const
{
	return getBool(KEY_READ_CONSISTENCY);
}

bool Config::getClearGTTAtRetaining() const
{
	return getBool(KEY_CLEAR_GTT_RETAINING);
}

const char* Config::getDataTypeCompatibility() const
{
	return getStr(KEY_DATA_TYPE_COMPATIBILITY);
}

bool Config::getUseFileSystemCache(bool* pPresent) const
{
	return getBool(KEY_USE_FILESYSTEM_CACHE, pPresent);
}

} // namespace Firebird
