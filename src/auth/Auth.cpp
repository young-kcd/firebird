/*
 *	PROGRAM:		Firebird authentication
 *	MODULE:			Auth.cpp
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

#include "firebird.h"
#include "../auth/Auth.h"
#include "../jrd/ibase.h"

#ifdef AUTH_DEBUG
namespace {
void debugName(unsigned char** data, unsigned short* dataSize)
{
	const char* name = "DEBUG_AUTH";
	*data = (unsigned char*)name;
	*dataSize = strlen(name);
}
}

namespace Auth {

ServerInstance* DebugServer::instance()
{
	return interfaceAlloc<DebugServerInstance>();
}

ClientInstance* DebugClient::instance()
{
	return interfaceAlloc<DebugClientInstance>();
}

void DebugServer::getName(unsigned char** data, unsigned short* dataSize)
{
	debugName(data, dataSize);
}

void DebugClient::getName(unsigned char** data, unsigned short* dataSize)
{
	debugName(data, dataSize);
}

void DebugServer::release()
{
	interfaceFree(this);
}

void DebugClient::release()
{
	interfaceFree(this);
}

Result DebugServerInstance::startAuthentication(bool isService, const char* dbName,
												const unsigned char* dpb, unsigned int dpbSize,
												WriterInterface* writerInterface)
{
	str[0] = 0;
	Firebird::ClumpletReader rdr(isService ? Firebird::ClumpletReader::spbList : 
								 Firebird::ClumpletReader::dpbList, dpb, dpbSize);
	if (rdr.find(isService ? isc_spb_trusted_auth : isc_dpb_trusted_auth))
	{
		memcpy(str, rdr.getBytes(), rdr.getClumpLength());
		str[rdr.getClumpLength()] = 0;
	}
	strcat((char*)str, "_");
	return AUTH_MORE_DATA;
}

Result DebugServerInstance::contAuthentication(WriterInterface* writerInterface,
											   const unsigned char* data, unsigned int size)
{
	//fprintf(stderr, "DebugServerInstance::contAuthentication: %.*s\n", size, data);
	writerInterface->add(Firebird::string((const char*)data, size).c_str(), "DEBUG", "");
	return AUTH_SUCCESS;
}

void DebugServerInstance::getData(unsigned char** data, unsigned short* dataSize)
{
	*data = str;
	*dataSize = strlen((const char*)str);
	//fprintf(stderr, "DebugServerInstance::getData: %.*s\n", *dataSize, *data);
}

void DebugServerInstance::release()
{
	interfaceFree(this);
}

Result DebugClientInstance::startAuthentication(bool isService, const char*, DpbInterface* dpb)
{
	strcpy((char*)str, "HAND");
	if (dpb)
	{
		dpb->add(isService ? isc_spb_trusted_auth : isc_dpb_trusted_auth, str, 
				 strlen((const char*)str));
		return AUTH_SUCCESS;
	}
	return AUTH_MORE_DATA;
}

Result DebugClientInstance::contAuthentication(const unsigned char* data, unsigned int size)
{
	if (size > sizeof(data) - 1)
	{
		size = sizeof(data) - 1;
	}
	//fprintf(stderr, "DebugClientInstance::contAuthentication: %.*s\n", size, data);
	memcpy(str, data, size);
	str[size] = 0;
	strcat((char*)str, "SHAKE");
	return AUTH_CONTINUE;
}

void DebugClientInstance::getData(unsigned char** data, unsigned short* dataSize)
{
	*data = str;
	*dataSize = strlen((const char*)str);
	//fprintf(stderr, "DebugClientInstance::getData: %.*s\n", *dataSize, *data);
}

void DebugClientInstance::release()
{
	interfaceFree(this);
}

} // namespace Auth
#endif // AUTH_DEBUG

namespace Auth {

WriterImplementation::WriterImplementation(Firebird::MemoryPool& pool, bool svcFlag)
	: Firebird::PermanentStorage(pool), body(getPool()), 
	  sequence(0), tag(svcFlag ? isc_spb_auth_block : isc_dpb_auth_block)
{ }

void WriterImplementation::store(Firebird::ClumpletWriter& to)
{
	to.deleteWithTag(tag);
	to.insertBytes(tag, body.getBuffer(), body.getBufferLength());
}

void WriterImplementation::reset()
{
	body.clear();
	sequence = 0;
}

void WriterImplementation::add(const char* name, const char* method, const char* details)
{
	body.putLevel(++sequence, name, method, details);
}


DpbImplementation::DpbImplementation(Firebird::ClumpletWriter& base)
	: body(&base)
{ }

int DpbImplementation::find(UCHAR tag)
{
	return body->find(tag) ? 1 : 0;
}

void DpbImplementation::add(UCHAR tag, void* bytes, unsigned int count)
{
	body->insertBytes(tag, bytes, count);
}

void DpbImplementation::drop()
{
	body->deleteClumplet();
}


bool legacy(Plugin* plugin)
{
	const char* legacyTrusted = "WIN_SSPI";
	const short legLength = strlen(legacyTrusted);
	UCHAR* nm;
	USHORT len;

	plugin->getName(&nm, &len);

	return len == legLength && memcmp(legacyTrusted, nm, legLength) == 0;
}

} // namespace Auth

