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

		1. Add key description to Config::entries[] array (config.cpp)
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

namespace Firebird
{

extern const char*	GCPolicyCooperative;
extern const char*	GCPolicyBackground;
extern const char*	GCPolicyCombined;

const int WIRE_CRYPT_DISABLED = 0;
const int WIRE_CRYPT_ENABLED = 1;
const int WIRE_CRYPT_REQUIRED = 2;

enum WireCryptMode {WC_CLIENT, WC_SERVER};		// Have different defaults

const int MODE_SUPER = 0;
const int MODE_SUPERCLASSIC = 1;
const int MODE_CLASSIC = 2;

const char* const CONFIG_FILE = "firebird.conf";

class Config : public RefCounted, public GlobalStorage
{
public:
	//typedef IPTR ConfigValue;
	struct ConfigValue
	{
		ConfigValue() : intVal(0) {};
		explicit ConfigValue(const char* val) : strVal(val) {};
		explicit ConfigValue(bool val) : boolVal(val) {};
		explicit ConfigValue(SINT64 val) : intVal(val) {};
		explicit ConfigValue(unsigned val) : intVal(val) {};
		explicit ConfigValue(int val) : intVal(val) {};

		union 
		{
			const char* strVal;
			bool boolVal;
			SINT64 intVal;
		};

		// simple bitwise comparison
		bool operator== (const ConfigValue& other) const
		{
			return this->intVal == other.intVal;
		}

		bool operator!= (const ConfigValue& other) const
		{
			return !(*this == other);
		}
	};

	enum ConfigKey
	{
		KEY_TEMP_BLOCK_SIZE,
		KEY_TEMP_CACHE_LIMIT,
		KEY_REMOTE_FILE_OPEN_ABILITY,
		KEY_GUARDIAN_OPTION,
		KEY_CPU_AFFINITY_MASK,
		KEY_TCP_REMOTE_BUFFER_SIZE,
		KEY_TCP_NO_NAGLE,
		KEY_TCP_LOOPBACK_FAST_PATH,
		KEY_DEFAULT_DB_CACHE_PAGES,
		KEY_CONNECTION_TIMEOUT,
		KEY_DUMMY_PACKET_INTERVAL,
		KEY_DEFAULT_TIME_ZONE,
		KEY_LOCK_MEM_SIZE,
		KEY_LOCK_HASH_SLOTS,
		KEY_LOCK_ACQUIRE_SPINS,
		KEY_EVENT_MEM_SIZE,
		KEY_DEADLOCK_TIMEOUT,
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
		KEY_DATABASE_GROWTH_INCREMENT,
		KEY_FILESYSTEM_CACHE_THRESHOLD,
		KEY_RELAXED_ALIAS_CHECKING,
		KEY_TRACE_CONFIG,
		KEY_MAX_TRACELOG_SIZE,
		KEY_FILESYSTEM_CACHE_SIZE,
		KEY_PLUG_PROVIDERS,
		KEY_PLUG_AUTH_SERVER,
		KEY_PLUG_AUTH_CLIENT,
		KEY_PLUG_AUTH_MANAGE,
		KEY_PLUG_TRACE,
		KEY_SECURITY_DATABASE,
		KEY_SERVER_MODE,
		KEY_WIRE_CRYPT,
		KEY_PLUG_WIRE_CRYPT,
		KEY_PLUG_KEY_HOLDER,
		KEY_REMOTE_ACCESS,
		KEY_IPV6_V6ONLY,
		KEY_WIRE_COMPRESSION,
		KEY_MAX_IDENTIFIER_BYTE_LENGTH,
		KEY_MAX_IDENTIFIER_CHAR_LENGTH,
		KEY_ENCRYPT_SECURITY_DATABASE,
		KEY_STMT_TIMEOUT,
		KEY_CONN_IDLE_TIMEOUT,
		KEY_CLIENT_BATCH_BUFFER,
		KEY_OUTPUT_REDIRECTION_FILE,
		KEY_EXT_CONN_POOL_SIZE,
		KEY_EXT_CONN_POOL_LIFETIME,
		KEY_SNAPSHOTS_MEM_SIZE,
		KEY_TIP_CACHE_BLOCK_SIZE,
		KEY_READ_CONSISTENCY,
		KEY_CLEAR_GTT_RETAINING,
		KEY_DATA_TYPE_COMPATIBILITY,
		KEY_USE_FILESYSTEM_CACHE,
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

	void loadValues(const ConfigFile& file);
	void setupDefaultConfig();
	void checkValues();

	// helper check-value functions
	void checkIntForLoBound(ConfigKey key, SINT64 loBound, bool setDefault);
	void checkIntForHiBound(ConfigKey key, SINT64 hiBound, bool setDefault);

	const char* getStr(ConfigKey key, bool* pPresent = nullptr) const
	{
		if (pPresent)
			*pPresent = testKey(key);

		return values[key].strVal;
	}

	bool getBool(ConfigKey key, bool* pPresent = nullptr) const
	{
		if (pPresent)
			*pPresent = testKey(key);

		return values[key].boolVal;
	}

	SINT64 getInt(ConfigKey key, bool* pPresent = nullptr) const
	{
		if (pPresent)
			*pPresent = testKey(key);

		return values[key].intVal;
	}

	static bool valueAsString(ConfigValue val, ConfigType type, string& str);

	static ConfigEntry entries[MAX_CONFIG_KEY];

	ConfigValue values[MAX_CONFIG_KEY];

	// One bit per key: bit is set if corresponding key value was set in config
	ULONG bits[MAX_CONFIG_KEY / BITS_PER_LONG + 1];

	// test if given key value was set in config
	bool testKey(unsigned int key) const
	{
		return bits[key / BITS_PER_LONG] & (1ul << key % BITS_PER_LONG);
	}

	// set bit of given key that was set in config
	void setKey(unsigned int key)
	{
		bits[key / BITS_PER_LONG] |= (1ul << key % BITS_PER_LONG);
	}

	mutable PathName notifyDatabase;

	// set in default config only
	int serverMode;

public:
	explicit Config(const ConfigFile& file);				// use to build default config
	Config(const ConfigFile& file, const Config& base);		// use to build db-specific config
	Config(const ConfigFile& file, const Config& base, const PathName& notify);	// use to build db-specific config with notification
	~Config();

	// Call it when database with given config is created

	void notify() const;

	// Check for missing firebird.conf

	static bool missFirebirdConf();

	// Interface to support command line root specification.
	// This ugly solution was required to make it possible to specify root
	// in command line to load firebird.conf from that root, though in other
	// cases firebird.conf may be also used to specify root.

	static void setRootDirectoryFromCommandLine(const PathName& newRoot);
	static const PathName* getCommandLineRootDirectory();

	// Master config - needed to provide per-database config
	static const RefPtr<const Config>& getDefaultConfig();

	// Merge config entries from DPB into existing config
	static void merge(RefPtr<const Config>& config, const string* dpbConfig);

	// reports key to be used by the following functions
	static unsigned int getKeyByName(ConfigName name);
	// helpers to build interface for firebird.conf file
	SINT64 getInt(unsigned int key) const;
	const char* getString(unsigned int key) const;
	bool getBoolean(unsigned int key) const;

	// Number of known keys
	static unsigned int getKeyCount() { return MAX_CONFIG_KEY; };
	static const char* getKeyName(unsigned int key) 
	{
		if (key >= MAX_CONFIG_KEY)
			return nullptr;

		return entries[key].key;
	}
	// false if value is null or key is not exists
	bool getValue(unsigned int key, string& str) const;
	static bool getDefaultValue(unsigned int key, string& str);
	// return true if value is set at some level
	bool getIsSet(unsigned int key) const { return testKey(key); }

	// Static functions apply to instance-wide values,
	// non-static may be specified per database.

	// Installation directory
	static const char* getInstallDirectory();

	// Root directory of current installation
	static const char* getRootDirectory();

	// Allocation chunk for the temporary spaces
	static int getTempBlockSize();

	// Caching limit for the temporary data
	FB_UINT64 getTempCacheLimit() const;

	// Whether remote (NFS) files can be opened
	static bool getRemoteFileOpenAbility();

	// Startup option for the guardian
	static int getGuardianOption();

	// CPU affinity mask
	static FB_UINT64 getCpuAffinityMask();

	// XDR buffer size
	static int getTcpRemoteBufferSize();

	// Disable Nagle algorithm
	bool getTcpNoNagle() const;

	// Enable or disable the TCP Loopback Fast Path option
	bool getTcpLoopbackFastPath() const;

	// Let IPv6 socket accept only IPv6 packets
	bool getIPv6V6Only() const;

	// Default database cache size
	int getDefaultDbCachePages() const;

	// Connection timeout
	int getConnectionTimeout() const;

	// Dummy packet interval
	int getDummyPacketInterval() const;

	static const char* getDefaultTimeZone();

	// Lock manager memory size
	int getLockMemSize() const;

	// Lock manager hash slots
	int getLockHashSlots() const;

	// Lock manager acquire spins
	int getLockAcquireSpins() const;

	// Event manager memory size
	int getEventMemSize() const;

	// Deadlock timeout
	int getDeadlockTimeout() const;

	// Service name for remote protocols
	const char* getRemoteServiceName() const;

	// Service port for INET
	unsigned short getRemoteServicePort() const;

	// Pipe name for WNET
	const char* getRemotePipeName() const;

	// Name for IPC-related objects
	const char* getIpcName() const;

	// Unflushed writes number
	int getMaxUnflushedWrites() const;

	// Unflushed write time
	int getMaxUnflushedWriteTime() const;

	// Process priority level
	static int getProcessPriorityLevel();

	// Port for event processing
	int getRemoteAuxPort() const;

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

	int getDatabaseGrowthIncrement() const;

	int getFileSystemCacheThreshold() const;

	static FB_UINT64 getFileSystemCacheSize();

	static bool getRelaxedAliasChecking();

	static const char* getAuditTraceConfigFile();

	static FB_UINT64 getMaxUserTraceLogSize();

	static int getServerMode();

	const char* getPlugins(unsigned int type) const;

	const char* getSecurityDatabase() const;

	int getWireCrypt(WireCryptMode wcMode) const;

	bool getRemoteAccess() const;

	bool getWireCompression() const;

	int getMaxIdentifierByteLength() const;

	int getMaxIdentifierCharLength() const;

	bool getCryptSecurityDatabase() const;

	// set in seconds
	unsigned int getStatementTimeout() const;
	// set in minutes
	unsigned int getConnIdleTimeout() const;

	unsigned int getClientBatchBuffer() const;

	static const char* getOutputRedirectionFile();

	static int getExtConnPoolSize();

	static int getExtConnPoolLifeTime();

	ULONG getSnapshotsMemSize() const;

	ULONG getTipCacheBlockSize() const;

	bool getReadConsistency() const;

	bool getClearGTTAtRetaining() const;

	const char* getDataTypeCompatibility() const;

	bool getUseFileSystemCache(bool* pPresent = nullptr) const;
};

// Implementation of interface to access master configuration file
class FirebirdConf FB_FINAL :
	public RefCntIface<IFirebirdConfImpl<FirebirdConf, CheckStatusWrapper> >
{
public:
	FirebirdConf(const Config* existingConfig)
		: config(existingConfig)
	{ }

	// IFirebirdConf implementation
	unsigned int getKey(const char* name);
	SINT64 asInteger(unsigned int key);
	const char* asString(unsigned int key);
	FB_BOOLEAN asBoolean(unsigned int key);
	unsigned int getVersion(CheckStatusWrapper* status);

	int release();

private:
	RefPtr<const Config> config;
};

// Create default instance of IFirebirdConf interface
IFirebirdConf* getFirebirdConfig();

} // namespace Firebird

#endif // COMMON_CONFIG_H
