/*
 *	PROGRAM:		Firebird authentication
 *	MODULE:			Auth.h
 *	DESCRIPTION:	Implementation of interfaces, passed to plugins
 *					Plugins loader
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

#ifndef FB_AUTH_H
#define FB_AUTH_H

#include "../auth/AuthInterface.h"
#include "../common/classes/ClumpletWriter.h"
#include "../common/classes/init.h"
#include "../common/classes/array.h"
#include "../common/classes/fb_string.h"

namespace Auth {

bool legacy(Firebird::Plugin* plugin);

class WriterImplementation : public WriterInterface, public Firebird::PermanentStorage
{
public:
	WriterImplementation(Firebird::MemoryPool&, bool svcFlag);

	void store(Firebird::ClumpletWriter& to);

	void reset();
	void add(const char* name, const char* method, const char* details);

private:
	Firebird::AuthWriter body;
	unsigned int sequence;
	unsigned char tag;
};

class DpbImplementation : public DpbInterface
{
public:
	DpbImplementation(Firebird::ClumpletWriter& base);

	int find(UCHAR tag);
	void add(UCHAR tag, const void* bytes, unsigned int count);
	void drop();

private:
	Firebird::ClumpletWriter* body;
};

#define AUTH_DEBUG

#ifdef AUTH_DEBUG

// The idea of debug plugin is to send some data from server to client,
// modify them on client and return result (which becomes login name) to the server

class DebugServer : public ServerPlugin
{
public:
	ServerInstance* instance();
    void release();
};

class DebugClient : public ClientPlugin
{
public:
	ClientInstance* instance();
    void release();
};

class DebugServerInstance : public ServerInstance
{
public:
	DebugServerInstance();

    Result startAuthentication(bool isService, const char* dbName,
                               const unsigned char* dpb, unsigned int dpbSize,
                               WriterInterface* writerInterface);
    Result contAuthentication(WriterInterface* writerInterface,
                              const unsigned char* data, unsigned int size);
    void getData(const unsigned char** data, unsigned short* dataSize);
    void release();

private:
	Firebird::string str;
};

class DebugClientInstance : public ClientInstance
{
public:
	DebugClientInstance();

	Result startAuthentication(bool isService, const char* dbName, DpbInterface* dpb);
	Result contAuthentication(const unsigned char* data, unsigned int size);
    void getData(const unsigned char** data, unsigned short* dataSize);
    void release();

private:
	Firebird::string str;
};

#endif // AUTH_DEBUG

} // namespace Auth


#endif // FB_AUTH_H
