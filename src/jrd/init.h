/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		init.h
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

#ifndef CLASSES_INIT_INSTANCE_H
#define CLASSES_INIT_INSTANCE_H

#include "../jrd/thd.h"

namespace Firebird {

// Mutex implementation based on THD_* functions

class Mutex {
#ifdef BOOT_BUILD
public:
	void enter() {}
	void leave() {}
#else //BOOT_BUILD
private:
#ifdef MULTI_THREAD
	mutx_t mutex;
#endif //MULTI_THREAD
public:
	Mutex();
	~Mutex();
	void enter();
	void leave();
#endif //BOOT_BUILD
};


// InitMutex - executes static void C::init() once and only once

template <typename C>
class InitMutex : private Mutex {
private:
	volatile bool flag;
public:
	InitMutex() : flag(false) { }
	void init() {
		if (!flag) {
			try {
				enter();
				if (!flag) {
					C::init();
					flag = true;
				}
			} catch(...) {
				leave();
				throw;
			}
			leave();
		}
	} 
	void cleanup() {
		if (flag) {
			try {
				enter();
				if (flag) {
					C::cleanup();
					flag = false;
				}
			} catch(...) {
				leave();
				throw;
			}
			leave();
		}
	} 
};

// InitInstance - initialize pointer to class once and only once,
// DefaultInit uses default memory pool for it.

template <typename T>
class DefaultInit {
public:
	static T* init() {
		return FB_NEW(*getDefaultMemoryPool()) T(*getDefaultMemoryPool());
	}
};

template <typename T, 
	typename C = DefaultInit<T> >
class InitInstance : private Mutex {
private:
	T* instance;
	volatile bool flag;
public:
	InitInstance<T, C>() : flag(false) { }
	T& operator()() {
		if (!flag) {
			try {
				enter();
				if (!flag) {
					instance = C::init();
					flag = true;
				}
			} catch(...) {
				leave();
				throw;
			}
			leave();
		}
		return *instance;
	}
};

} //namespace Firebird

#endif // CLASSES_INIT_INSTANCE_H
