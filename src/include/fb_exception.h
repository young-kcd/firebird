//
//	Copyright (c) 2001 M. Nordell
//
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 */
//
// Special permission is given to the Firebird project to include,
// use and modify this file in the Firebird database engine.
//
#ifndef FB_EXCEPTION_H
#define FB_EXCEPTION_H

#include <exception>
#include "fb_types.h"


namespace Firebird {

class status_exception : public std::exception
{
public:
	explicit status_exception(ISC_STATUS s)
	:	m_s(s)
	{}
	virtual ~status_exception() throw() {}
	virtual const char* what() const throw()
		{ return "Firebird::status_exception"; }
	ISC_STATUS value() const { return m_s; }

	static void raise(ISC_STATUS s);

private:
	ISC_STATUS m_s;
};

class red_zone_error : public std::exception
{
public:
	virtual const char* what() const throw()
		{ return "Firebird::red_zone_error"; }

	static void raise();
};

class memory_corrupt : public std::exception
{
public:
	virtual const char* what() const throw()
		{ return "Firebird::memory_corrupt"; }

	static void raise();
};

class system_call_failed : public std::exception
{
public:
	virtual const char* what() const throw()
		{ return "Firebird::system_call_failed"; }

	static void raise();
};

class fatal_exception : public std::exception
{
public:
	explicit fatal_exception(char *message);
	virtual ~fatal_exception() throw() {}
	virtual const char* what() const throw()
		{ return txt; }

	static void raise(char *message);

private:
	char txt[256];
};

}	// namespace Firebird

#endif	// FB_EXCEPTION_H
