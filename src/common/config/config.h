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

#ifndef COMMON_CONFIG_H
#define COMMON_CONFIG_H

#include "../common/classes/alloc.h"
#include "../common/classes/fb_string.h"
#include "../common/classes/RefCounted.h"
#include "../common/config/config_file.h"
#include "../common/classes/ImplementHelper.h"

/**
	Since the original (isc.cpp) code wasn't able to provide powerful and
	easy-to-use abilities to work with complex configurations, a decision
	has been made to create a completely new one.

	This class is a public interface for our generic configuration manager
	and allows to access all configuration values by its getXXX() member
	functions. Each of these functions corresponds to one and only one key
	and has one input argument - default value, which is used when the
	requested key is missing or the configuration file is not found. Supported
	value datatypes are "const char*", "int" and "bool". Usual default values for
	these datatypes are empty string, zero and false respectively. There are
	two types of member functions - scalar and vector. The former ones return
	single value of the given type. The latter ones return vector which
	contains an ordered array of values.

	There's one exception - getRootDirectory() member function, which returns
	root pathname of the current installation. This value isn't stored in the
	configuration file, but is managed by the code itself. But there's a way
	to override this value via the configuration file as well.

	To add new configuration item, you have to take the following steps:

		1. Add key description to ConfigImpl::entries[] array (config.cpp)
		2. Add logical key to Config::ConfigKey enumeration (config.h)
		   (note: both physical and logical keys MUST have the same ordinal
				  position within appropriate structures)
		3. Add member function to Config class (config.h) and implement it
		   in config.cpp module.
		4. For per-database configurable parameters, please use
				type getParameterName() const;
		   form, for world-wide parameters:
				static type getParameterName();
		   should be used.
**/

extern const char*	GCPolicyCooperative;
extern const char*	GCPolicyBackground;
extern const char*	GCPolicyCombined;

extern const char*	AmNative;
extern const char*	AmTrusted;
extern const char*	AmMixed;

enum AmCache {AM_UNKNOWN, AM_DISABLED, AM_ENABLED};

const char* const CONFIG_FILE = "firebird.conf";

class Config : public Firebird::RefCounted, public Firebird::GlobalStorage
{
public:
	typedef IPTR ConfigValue;

	enum ConfigKey
	{
		KEY_TEMP_BLOCK_SIZE,
		KEY_TEMP_CACHE_LIMIT,
		KEY_REMOTE_FILE_OPEN_ABILITY,
		KEY_GUARDIAN_OPTION,
		KEY_CPU_AFFINITY_MASK,
		KEY_TCP_REMOTE_BUFFER_SIZE,
		KEY_TCP_NO_NAGLE,
		KEY_DEFAULT_DB_CACHE_PAGES,
		KEY_CONNECTION_TIMEOUT,
		KEY_DUMMY_PACKET_INTERVAL,
		KEY_LOCK_MEM_SIZE,
		KEY_LOCK_GRANT_ORDER,
		KEY_LOCK_HASH_SLOTS,
		KEY_LOCK_ACQUIRE_SPINS,
		KEY_EVENT_MEM_SIZE,
		KEY_DEADLOCK_TIMEOUT,
		KEY_PRIORITY_SWITCH_DELAY,
		KEY_USE_PRIORITY_SCHEDULER,
		KEY_PRIORITY_BOOST,
		KEY_REMOTE_SERVICE_NAME,
		KEY_REMOTE_SERVICE_PORT,
		KEY_REMOTE_PIPE_NAME,
		KEY_IPC_NAME,
		KEY_MAX_UNFLUSHED_WRITES,
		KEY_MAX_UNFLUSHED_WRITE_TIME,
		KEY_PROCESS_PRIORITY_LEVEL,
		KEY_REMOTE_AUX_PORT,
		KEY_REMOTE_BIND_ADDRESS,
		KEY_EXTERNAL_FILE_ACCESS,
		KEY_DATABASE_ACCESS,
		KEY_UDF_ACCESS,
		KEY_TEMP_DIRECTORIES,
		KEY_BUGCHECK_ABORT,
		KEY_TRACE_DSQL,
		KEY_LEGACY_HASH,
		KEY_GC_POLICY,
		KEY_REDIRECTION,
		KEY_AUTH_METHOD,
		KEY_DATABASE_GROWTH_INCREMENT,
		KEY_FILESYSTEM_CACHE_THRESHOLD,
		KEY_RELAXED_ALIAS_CHECKING,
		KEY_OLD_SET_CLAUSE_SEMANTICS,
		KEY_TRACE_CONFIG,
		KEY_MAX_TRACELOG_SIZE,
		KEY_FILESYSTEM_CACHE_SIZE,
		KEY_PLUG_PROVIDERS,
		KEY_PLUG_AUTH_SERVER,
		KEY_PLUG_AUTH_CLIENT,
		KEY_PLUG_AUTH_MANAGE,
		KEY_PLUG_TRACE,
		KEY_SECURITY_DATABASE,
		KEY_SHARED_CACHE,
		KEY_SHARED_DATABASE,
		MAX_CONFIG_KEY		// keep it last
	};


private:
	enum ConfigType
	{
		TYPE_BOOLEAN,
		TYPE_INTEGER,
		TYPE_STRING
		//TYPE_STRING_VECTOR // CVC: Unused
	};

	typedef const char* ConfigName;

	struct ConfigEntry
	{
		ConfigType data_type;
		ConfigName key;
		ConfigValue default_value;
	};

	static ConfigFile::String getValue(const ConfigFile&, ConfigName);

	static int asInteger(const ConfigFile::String&);
	static bool asBoolean(const ConfigFile::String&);
	static const char* asString(const ConfigFile::String&);

	void loadValues(const ConfigFile& file);

	template <typename T> T get(Config::ConfigKey key) const
	{
		return (T) values[key];
	}

	static const ConfigEntry entries[MAX_CONFIG_KEY];

	ConfigValue values[MAX_CONFIG_KEY];

public:
	explicit Config(const ConfigFile& file);				// use to build default config
	Config(const ConfigFile& file, const Config& base);		// use to build db-specific config
	~Config();

	// Check for errors in .conf file

	static const char* getMessage();

	// Interface to support command line root specification.
	// This ugly solution was required to make it possible to specify root
	// in command line to load firebird.conf from that root, though in other
	// cases firebird.conf may be also used to specify root.

	static void setRootDirectoryFromCommandLine(const Firebird::PathName& newRoot);
	static const Firebird::PathName* getCommandLineRootDirectory();

	// Master config - needed to provide per-database config
	static const Firebird::RefPtr<Config> getDefaultConfig();

	// reports key to be used by the following functions
	static unsigned int getKeyByName(ConfigName name);
	// helpers to build interface for firebird.conf file
	int getInt(unsigned int key) const;
	const char* getString(unsigned int key) const;

	// Static functions apply to instance-wide values,
	// non-static may be specified per database.

	// Installation directory
	static const char* getInstallDirectory();

	// Root directory of current installation
	static const char* getRootDirectory();

	// Allocation chunk for the temporary spaces
	static int getTempBlockSize();

	// Caching limit for the temporary data
	static int getTempCacheLimit();

	// Whether remote (NFS) files can be opened
	static bool getRemoteFileOpenAbility();

	// Startup option for the guardian
	static int getGuardianOption();

	// CPU affinity mask
	static int getCpuAffinityMask();

	// XDR buffer size
	static int getTcpRemoteBufferSize();

	// Disable Nagle algorithm
	static bool getTcpNoNagle();

	// Default database cache size
	int getDefaultDbCachePages() const;

	// Connection timeout
	static int getConnectionTimeout();

	// Dummy packet interval
	static int getDummyPacketInterval();

	// Lock manager memory size
	int getLockMemSize() const;

	// Lock manager grant order
	bool getLockGrantOrder() const;

	// Lock manager hash slots
	int getLockHashSlots() const;

	// Lock manager acquire spins
	int getLockAcquireSpins() const;

	// Event manager memory size
	int getEventMemSize() const;

	// Deadlock timeout
	int getDeadlockTimeout() const;

	// Priority switch delay
	static int getPrioritySwitchDelay();

	// Use priority scheduler
	static bool getUsePriorityScheduler();

	// Priority boost
	static int getPriorityBoost();

	// Service name for remote protocols
	static const char* getRemoteServiceName();

	// Service port for INET
	static unsigned short getRemoteServicePort();

	// Pipe name for WNET
	static const char* getRemotePipeName();

	// Name for IPC-related objects
	static const char* getIpcName();

	// Unflushed writes number
	int getMaxUnflushedWrites() const;

	// Unflushed write time
	int getMaxUnflushedWriteTime() const;

	// Process priority level
	static int getProcessPriorityLevel();

	// Port for event processing
	static int getRemoteAuxPort();

	// Server binding NIC address
	static const char* getRemoteBindAddress();

	// Directory list for external tables
	const char* getExternalFileAccess() const;

	// Directory list for databases
	static const char* getDatabaseAccess();

	// Directory list for UDF libraries
	static const char* getUdfAccess();

	// Temporary directories list
	static const char* getTempDirectories();

	// DSQL trace bitmask
	static int getTraceDSQL();

	// Abort on BUGCHECK and structured exceptions
 	static bool getBugcheckAbort();

	// Let use of des hash to verify passwords
	static bool getLegacyHash();

	// GC policy
	const char* getGCPolicy() const;

	// Redirection
	static bool getRedirection();

	// Use native, trusted or mixed authentication
	static const char* getAuthMethod();

	int getDatabaseGrowthIncrement() const;

	int getFileSystemCacheThreshold() const;

	static int getFileSystemCacheSize();

	static bool getRelaxedAliasChecking();

	static bool getOldSetClauseSemantics();

	static const char* getAuditTraceConfigFile();

	static int getMaxUserTraceLogSize();

	static bool getSharedCache();

	static bool getSharedDatabase();

	static bool getMultiClientServer();

	static const char* getPlugins(unsigned int type);

	const char* getSecurityDatabase() const;
};

// Implementation of interface to access master configuration file
class FirebirdConf : public Firebird::StdIface<Firebird::IFirebirdConf, FB_I_FIREBIRD_CONF_VERSION>
{
public:
	FirebirdConf(Config* existingConfig)
		: config(existingConfig)
	{ }

	// IFirebirdConf implementation
	unsigned int FB_CARG getKey(const char* name);
	int FB_CARG asInteger(unsigned int key);
	const char* FB_CARG asString(unsigned int key);

	int FB_CARG release();

private:
	Firebird::RefPtr<Config> config;
};

#endif // COMMON_CONFIG_H
