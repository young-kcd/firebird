/*
 *	PROGRAM:		Firebird authentication.
 *	MODULE:			SrpClient.cpp
 *	DESCRIPTION:	SPR authentication plugin.
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
 *  Copyright (c) 2012 Alex Peshkov <peshkoff at mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"

#include "./Arc4.h"
#include "../common/classes/ImplementHelper.h"

using namespace Firebird;

namespace
{

class Cypher : public RefCntIface<ICrypt, FB_CRYPT_VERSION>
{
public:
	Cypher()
		: s1(0), s2(0)
	{
		for (unsigned int n = 0; n < sizeof(state); ++n)
		{
			state[n] = n;
		}
	}

	void setKey(unsigned int l, const unsigned char* key)
	{
		for (unsigned int k1 = 0, k2 = 0; k1 < sizeof(state); ++k1)
		{
			k2 = (k2 + key[k1 % l] + state[k1]) & 0xff;
			swap(state[k1], state[k2]);
		}
	}

	// ICrypt implementation
	void FB_CARG transform(IStatus* status, unsigned int length, void* to, const void* from)
	{
		status->init();

		unsigned char* t = static_cast<unsigned char*>(to);
		const unsigned char* f = static_cast<const unsigned char*>(from);

		while (length--)
		{
			s2 += state[++s1];
			swap(state[s1], state[s2]);
			unsigned char k = state[s1] + state[s2];
			k = state[k];
			*t++ = k ^ *f++;
		}
	}

	int FB_CARG release()
	{
		if (--refCounter == 0)
		{
			delete this;
			return 0;
		}
		return 1;
	}

private:
	unsigned char state[256];
	unsigned char s1;
	unsigned char s2;

	void swap(unsigned char& c1, unsigned char& c2)
	{
		unsigned char temp = c1;
		c1 = c2;
		c2 = temp;
	}
};

} // anonymous namespace


namespace Crypt {

class Arc4 : public StdPlugin<ICryptPlugin, FB_CRYPT_PLUGIN_VERSION>
{
public:
	explicit Arc4(IPluginConfig*)
	{ }

	// ICryptPlugin implementation
	const char* FB_CARG getKnownTypes(IStatus* status);
	ICrypt* FB_CARG getEncrypt(IStatus* status, FbCryptKey* key);
	ICrypt* FB_CARG getDecrypt(IStatus* status, FbCryptKey* key);
	int FB_CARG release();

private:
	ICrypt* createCypher(IStatus* status, unsigned int l, const void* key);
};

int Arc4::release()
{
	if (--refCounter == 0)
	{
		delete this;
		return 0;
	}
	return 1;
}

ICrypt* Arc4::getEncrypt(IStatus* status, FbCryptKey* key)
{
	return createCypher(status, key->encryptLength, key->encryptKey);
}

ICrypt* Arc4::getDecrypt(IStatus* status, FbCryptKey* key)
{
	const void* k = key->decryptKey;
    unsigned int l = key->decryptLength;
    if (!k)
    {
    	k = key->encryptKey;
    	l = key->encryptLength;
    }
	return createCypher(status, l, k);
}

ICrypt* Arc4::createCypher(IStatus* status, unsigned int l, const void* key)
{
	status->init();
	try
	{
		Cypher* rc = new Cypher;
		rc->setKey(l, static_cast<const unsigned char*>(key));
		return rc;
	}
	catch (const Exception& ex)
	{
		ex.stuffException(status);
	}
	return NULL;
}

const char* Arc4::getKnownTypes(IStatus* status)
{
	status->init();
	return "Symmetric";
}


namespace
{
	SimpleFactory<Arc4> factory;
}

void registerArc4(IPluginManager* iPlugin)
{
	iPlugin->registerPluginFactory(PluginType::Crypt, "Arc4", &factory);
}

} // namespace Crypt
