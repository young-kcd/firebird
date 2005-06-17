/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		init.cpp
 *	DESCRIPTION:	InitMutex, InitInstance - templates to help with initialization
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
 *  The Original Code was created by Alexander Peshkoff
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2004 Alexander Peshkoff <peshkoff@mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

/*
 * This BACKPORTED from 2.0 version of initialization templates 
 * should be used only in modules, linked with thd.cpp 
 * (i.e. with JRD_ClientFiles, where thd.cpp is located).
 * Engine and client library are OK from this POV.
 */

#include "firebird.h"
#include "../jrd/init.h"
#include "../jrd/thd_proto.h"

#ifndef BOOT_BUILD
namespace Firebird {

// Mutex implementation based on THD_* functions

Mutex::Mutex() {
#ifdef MULTI_THREAD
	THD_mutex_init(&mutex);				
#endif //MULTI_THREAD
}

Mutex::~Mutex() {
#ifdef MULTI_THREAD
	THD_mutex_destroy(&mutex);				
#endif //MULTI_THREAD
}

void Mutex::enter() {
#ifdef MULTI_THREAD
	THD_mutex_lock(&mutex);				
#endif //MULTI_THREAD
}

void Mutex::leave() {
#ifdef MULTI_THREAD
	THD_mutex_unlock(&mutex);				
#endif //MULTI_THREAD
}

} // namespace Firebird
#endif //BOOT_BUILD
