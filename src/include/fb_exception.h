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
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include "fb_types.h"


namespace Firebird {

class StringsBuffer {
public:
	virtual char* alloc(const char* string, size_t length) = 0;
	virtual ~StringsBuffer() {}
};

template <size_t BUFFER_SIZE>
class CircularStringsBuffer : public StringsBuffer {
public:
	CircularStringsBuffer() throw() {
		// This is to ensure we have zero at the end of buffer in case of buffer overflow
		memset(buffer, 0, BUFFER_SIZE); 
		buffer_ptr = buffer;
	}
	virtual char* alloc(const char* string, size_t length) {
		// fb_assert(length+1 < BUFFER_SIZE);
		// If there isn't any more room in the buffer, start at the beginning again
		if (buffer_ptr + length + 1 > buffer + BUFFER_SIZE)
			buffer_ptr = buffer;
		char* new_string = buffer_ptr;
		memcpy(new_string, string, length);
		new_string[length] = 0;
		buffer_ptr += length + 1;	
		return new_string;
	}
private:
	char buffer[BUFFER_SIZE];
	char *buffer_ptr;
};

class status_exception : public std::exception
{
public:
	// This version of constructor receives status vector pointing to permanent strings
	// which may be returned to user in status vector directly without transfer of string ownership
	explicit status_exception(const ISC_STATUS *status_vector) throw();
	
	// These versions of constructor clone passed transient strings
	status_exception(ISC_STATUS status, va_list status_args);
	status_exception(ISC_STATUS status, ...);
	
	// Create exception with undefined status vector, this constructor allows to use this
	// class as jmpbuf replacement for transitional period
	status_exception() throw();

	virtual ~status_exception() throw();
	virtual const char* what() const throw()
		{ return "Firebird::status_exception"; }
	const ISC_STATUS* value() const { return m_status_vector; }

	// Returns true if strings contained in status vector are located in magical 
	// permanent circular buffer. False means that exception object owns strings 
	// and is about to deallocate them in its destructor
	bool strings_permanent() const { return m_strings_permanent; }

	// Returns true if exception class holds status vector for the error.
	// Returns false when status vector is passed "by magic", probably 
	// somewhere in tdbb_status_vector
	bool status_known() const { return m_status_known; }

	// Takes permanent strings
	static void raise(const ISC_STATUS *status_vector);
	static void raise();
	
	// Take transient strings
	static void raise(ISC_STATUS status, ...);
private:
	ISC_STATUS_ARRAY m_status_vector;
	bool m_strings_permanent;
	bool m_status_known;
	void fill_status(ISC_STATUS status, va_list status_args);
};

class system_call_failed : public status_exception
{
public:
	system_call_failed(const char* v_syscall, int v_error_code);

	static void raise(const char* syscall, int error_code);
	static void raise(const char* syscall);
};

// Moved what() here due to gpre. Didn't want to use macros for gpre_static.
class fatal_exception : public status_exception
{
public:
	explicit fatal_exception(const char* message);
	// Keep in sync with the constructor above, please; "message" becomes 4th element
	// after status_exception's constructor invokes fill_status().
	const char* what() const throw()
	{
		return reinterpret_cast<const char*>(value()[3]);
	}
	static void raise(const char* message);
};


// Serialize exception into status_vector, put transient strings from exception into given StringsBuffer
ISC_STATUS stuff_exception(ISC_STATUS *status_vector, const std::exception& ex, StringsBuffer* sb = NULL) throw();

// These routines put strings into process-level circular buffer
// They are obsolete, use transient version of status_exception::raise in combination with
// stuff_exception instead
const char* status_string(const char* string);
const char* status_nstring(const char* string, size_t length);

}	// namespace Firebird


#endif	// FB_EXCEPTION_H

