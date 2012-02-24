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
 *  The Original Code was created by Alexander Peshkov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2012 Alex Peshkov <peshkoff@mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#ifndef FIREBIRD_CRYPT_PLUGIN_H
#define FIREBIRD_CRYPT_PLUGIN_H

#include "./Plugin.h"

namespace Firebird {

class ICrypt : public IRefCounted
{
public:
	virtual void FB_CARG transform(IStatus* status, unsigned int length, void* to, const void* from) = 0;
};

#define FB_CRYPT_VERSION (FB_REFCOUNTED_VERSION + 1)

class ICryptPlugin : public IPluginBase
{
public:
	// getKnownTypes() function must return list of acceptable keys' types
	// special type 'builtin' means that crypt plugin knows itself where to get the key from
	virtual const char* FB_CARG getKnownTypes(IStatus* status) = 0;
	virtual ICrypt* FB_CARG getEncrypt(IStatus* status, FbCryptKey* key) = 0;
	virtual ICrypt* FB_CARG getDecrypt(IStatus* status, FbCryptKey* key) = 0;
};

#define FB_CRYPT_PLUGIN_VERSION (FB_PLUGIN_VERSION + 3)

}	// namespace Firebird


#endif	// FIREBIRD_CRYPT_PLUGIN_H
