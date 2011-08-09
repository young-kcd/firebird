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
#include "../common/classes/ImplementHelper.h"

namespace Auth {

bool legacy(const char* nm);

class WriterImplementation : public Firebird::AutoIface<IWriter, FB_AUTH_WRITER_VERSION>
{
public:
	WriterImplementation(bool svcFlag);

	void store(Firebird::ClumpletWriter& to);
	void setMethod(const char* m);

	// IWriter implementation
	void FB_CARG reset();
	void FB_CARG add(const char* name);
	void FB_CARG setAttribute(unsigned char tag, const char* value);

private:
	Firebird::ClumpletWriter current, result;
	Firebird::string method;
	unsigned int sequence;
	unsigned char tag;

	void putLevel();
};

class DpbImplementation : public Firebird::AutoIface<IDpbReader, FB_AUTH_DPB_READER_VERSION>
{
public:
	DpbImplementation(Firebird::ClumpletWriter& base);

	// IDpbReader implementation
	int FB_CARG find(UCHAR tag);
	void FB_CARG add(UCHAR tag, const void* bytes, unsigned int count);
	void FB_CARG drop();

private:
	Firebird::ClumpletWriter* body;
};

//#define AUTH_DEBUG

#ifdef AUTH_DEBUG

// The idea of debug plugin is to send some data from server to client,
// modify them on client and return result (which becomes login name) to the server

class DebugServer : Firebird::StdIface<public Firebird::IPluginFactory, FB_PLUGINS_FACTORY_VERSION>
{
public:
	// IPluginFactory implementation
	Firebird::IInterface* FB_CARG createPlugin(const char* name, const char* configFile);
    int FB_CARG release();
};

class DebugClient : public Firebird::StdIface<Firebird::IPluginFactory, FB_PLUGINS_FACTORY_VERSION>
{
public:
	// IPluginFactory implementation
	Firebird::IInterface* FB_CARG createPlugin(const char* name, const char* configFile);
    int FB_CARG release();
};

class DebugServerInstance : public Firebird::StdPlugin<ServerInstance, FB_AUTH_SERVER_VERSION>
{
public:
	DebugServerInstance();

	// ServerInstance implementation
    Result FB_CARG startAuthentication(bool isService, const char* dbName,
                                       const unsigned char* dpb, unsigned int dpbSize,
                                       IWriter* writerInterface);
    Result FB_CARG contAuthentication(IWriter* writerInterface,
                                      const unsigned char* data, unsigned int size);
    void FB_CARG getData(const unsigned char** data, unsigned short* dataSize);
    int FB_CARG release();

private:
	Firebird::string str;
};

class DebugClientInstance : public Firebird::StdPlugin<ClientInstance, FB_AUTH_CLIENT_VERSION>
{
public:
	DebugClientInstance();

	// ClientInstance implementation
	Result FB_CARG startAuthentication(bool isService, const char* dbName, IDpbReader* dpb);
	Result FB_CARG contAuthentication(const unsigned char* data, unsigned int size);
    void FB_CARG getData(const unsigned char** data, unsigned short* dataSize);
    int FB_CARG release();

private:
	Firebird::string str;
};

#endif // AUTH_DEBUG

} // namespace Auth


#endif // FB_AUTH_H
