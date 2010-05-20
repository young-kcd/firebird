/*
 *	PROGRAM:		Firebird authentication
 *	MODULE:			AuthInterface.h
 *	DESCRIPTION:	Interfaces, used by authentication plugins
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

#ifndef FB_AUTH_INTERFACE
#define FB_AUTH_INTERFACE

#include "../common/classes/Interface.h"

namespace Auth {

enum Result {AUTH_SUCCESS, AUTH_CONTINUE, AUTH_FAILED, AUTH_MORE_DATA};

class InterfaceBase : public Firebird::Interface
{
};

class WriterInterface
{
public:
	virtual void reset() = 0;
	virtual void add(const char* user, const char* method, const char* details) = 0;
};

class DpbInterface
{
public:
	virtual int find(UCHAR tag) = 0;
	virtual void add(UCHAR tag, const void* bytes, unsigned int count) = 0;
	virtual void drop() = 0;
};

class Plugin : public InterfaceBase
{
public:
	virtual void getName(const char** data, unsigned short* dataSize) = 0;
};

class ServerInstance : public InterfaceBase
{
public:
	virtual Result startAuthentication(bool isService, const char* dbName,
									   const unsigned char* dpb, unsigned int dpbSize,
									   WriterInterface* writerInterface) = 0;
	virtual Result contAuthentication(WriterInterface* writerInterface,
									  const unsigned char* data, unsigned int size) = 0;
	virtual void getData(const unsigned char** data, unsigned short* dataSize) = 0;
};

class ServerPlugin : public Plugin
{
public:
	virtual ServerInstance* instance() = 0;
};

class ClientInstance : public InterfaceBase
{
public:
	virtual Result startAuthentication(bool isService, const char* dbName, DpbInterface* dpb) = 0;
	virtual Result contAuthentication(const unsigned char* data, unsigned int size) = 0;
	virtual void getData(const unsigned char** data, unsigned short* dataSize) = 0;
};

class ClientPlugin : public Plugin
{
public:
	virtual ClientInstance* instance() = 0;
};

} // namespace Auth


#endif // FB_AUTH_INTERFACE
