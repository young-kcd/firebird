/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		auto.h
 *	DESCRIPTION:	STL::auto_ptr replacement
 *
 *		*** warning ***
 *  Unlike STL's auto_ptr ALWAYS deletes ptr in destructor -
 *  no ownership flag!
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

#ifndef CLASSES_AUTO_PTR_H
#define CLASSES_AUTO_PTR_H

template <typename Where>
class AutoPtr {
private:
	Where *ptr;
public:
	AutoPtr<Where>(Where *v) {ptr = v;}
	operator Where* () {return ptr;}
	Where* operator-> () {return ptr;}
	~AutoPtr<Where>() {delete ptr;}
};


// CVC: It turns out that AutoPtr was designed to deallocate single objects,
// not arrays. Worse even, we need in many places to allocate dynamically an
// array of char*/UCHAR* that's later converted into a pointer to a single
// object and passed to AutoPtr. In that case, AutoPtr will invoke the wrong
// deallocation logic and therefore we have undefined behavior, typically a leak.
// See execute_statement.cpp for an example. This is the reason this
// AutoPtrFromString beast was created.

template <typename Where>
class AutoPtrFromString {
private:
	Where* ptr;
public:
	AutoPtrFromString<Where>(Where* v) {ptr = v;}
	operator Where* () {return ptr;}
	Where* operator-> () {return ptr;}
	~AutoPtrFromString<Where>()
	{
		char* p = reinterpret_cast<char*>(ptr);
		delete[] p;
	}
};


#endif // CLASSES_AUTO_PTR_H

