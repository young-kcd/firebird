/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		config.h
 *	DESCRIPTION:	Configuration manager (public interface)
 *
 * The contents of this file are subject to the Interbase Public
 * License Version 1.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy
 * of the License at http://www.Inprise.com/IPL.html
 *
 * Software distributed under the License is distributed on an
 * "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
 * or implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code was created by Inprise Corporation
 * and its predecessors. Portions created by Inprise Corporation are
 * Copyright (C) Inprise Corporation.
 *
 * Created by: Dmitry Yemanov <yemanov@yandex.ru>
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "fb_string.h"
#include "fb_vector.h"

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
**/

class Config
{
	enum ConfigKey
	{
		KEY_ROOT_DIRECTORY,							// 0
		KEY_SORT_MEM_BLOCK_SIZE,					// 1
		KEY_SORT_MEM_UPPER_LIMIT,					// 2
		KEY_REMOTE_FILE_OPEN_ABILITY,				// 3
		KEY_GUARDIAN_OPTION,						// 4
		KEY_CPU_AFFINITY_MASK,						// 5
		KEY_OLD_PARAMETER_ORDERING,					// 6
		KEY_TCP_REMOTE_BUFFER_SIZE,					// 7
		KEY_TCP_NO_NAGLE							// 8
	};

public:

	/*
		Root directory of current installation
	*/
	static const char* getRootDirectory();

	/*
		Block size for the sorting manager
	*/
	static int getSortMemBlockSize();

	/*
		Memory usage limit for the sorting manager
	*/
	static int getSortMemUpperLimit();

	/*
		Whether remote (NFS) files can be opened
	*/
	static bool getRemoteFileOpenAbility();

	/*
		Startup option for the guardian
	*/
	static int getGuardianOption();

	/*
		CPU affinity mask
	*/
	static int getCpuAffinityMask();

	/*
		Old parameter ordering for backward compatibility with FB1/IB6.X
	*/
	static bool getOldParameterOrdering();

	/*
		XDR buffer size
	*/
	static int getTcpRemoteBufferSize();

	/*
		Disable Nagle algorithm
	*/
	static bool getTcpNoNagle();
};

#endif // CONFIG_H
