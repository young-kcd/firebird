#include "firebird.h"

//#include "fb_exception.h"

#include <string.h>
#include <typeinfo>
#include <errno.h>
#include <stdarg.h>
#include "gen/iberror.h"
#include "../common/classes/alloc.h"

namespace {

// This logic taken from JRD is bad, bad, bad!
// Replace it with attachment-level buffers whenever possible
const size_t ENGINE_FAILURE_SPACE = 4096;

typedef Firebird::CircularStringsBuffer<ENGINE_FAILURE_SPACE> CircularBuffer;

class InterlockedStringsBuffer : public CircularBuffer {
public:
	virtual char* alloc(const char* string, size_t length) {
		buffer_lock.enter();
		char* new_string = CircularBuffer::alloc(string, length);
		buffer_lock.leave();
		return new_string;
	}
private:
	Firebird::Mutex buffer_lock;
};

} // namespace

namespace Firebird {

/********************************* status_exception *******************************/

status_exception::status_exception() throw() : 
	m_strings_permanent(true), m_status_known(false) 
{
	memset(m_status_vector, 0, sizeof(m_status_vector));
}

status_exception::status_exception(const ISC_STATUS *status_vector) throw() : 
	m_strings_permanent(true), m_status_known(status_vector != NULL)
{
	if (m_status_known) {
		ISC_STATUS *ptr = m_status_vector;
		do {
			const ISC_STATUS type = *ptr++ = *status_vector++;
			if (type == isc_arg_end)
				break;
			if (type == isc_arg_cstring)
				*ptr++ = *status_vector++;
			*ptr++ = *status_vector++;
		} while (true);
	}
}
	
void status_exception::fill_status(ISC_STATUS status, va_list status_args)
{
	m_strings_permanent = false;
	m_status_known = true;
	// Move in status and clone transient strings
	ISC_STATUS *ptr = m_status_vector;
	*ptr++ = isc_arg_gds;
	*ptr++ = status;
	do {
		const ISC_STATUS type = *ptr++ = va_arg(status_args, ISC_STATUS);
		if (type == isc_arg_end) 
			break;

		switch (type) {
		case isc_arg_cstring: 
			{				
				const UCHAR len = *ptr++ = va_arg(status_args, ISC_STATUS);
				char *string = FB_NEW(*getDefaultMemoryPool()) char[len];
				const char *temp = reinterpret_cast<char*>(va_arg(status_args, ISC_STATUS));
				memcpy(string, temp, len);
				*ptr++ = (ISC_STATUS)(IPTR)(string);
				break;
			}
		case isc_arg_string:
		case isc_arg_interpreted:
			{
				const char *temp = reinterpret_cast<char*>(va_arg(status_args, ISC_STATUS));
				const size_t len = strlen(temp);
				char *string = FB_NEW(*getDefaultMemoryPool()) char[len + 1];
				memcpy(string, temp, len + 1);
				*ptr++ = (ISC_STATUS)(IPTR)(string);
				break;
			}
		default:
			*ptr++ = va_arg(status_args, ISC_STATUS);
			break;
		}
	} while (true);	
}

status_exception::status_exception(ISC_STATUS status, va_list status_args) 
{
	fill_status(status, status_args);
}

status_exception::status_exception(ISC_STATUS status, ...) 
{
	va_list args;
	va_start(args, status);
	fill_status(status, args);
	va_end(args);
}

status_exception::~status_exception() throw() {
	if (m_strings_permanent)
		return;
	
	// Free owned strings
	ISC_STATUS *ptr = m_status_vector;
	do {
		const ISC_STATUS type = *ptr++;
		if (type == isc_arg_end)
			break;

		switch (type) {
		case isc_arg_cstring:
			ptr++;
			delete[] reinterpret_cast<char*>(*ptr++);
			break;
		case isc_arg_string:
		case isc_arg_interpreted:
			delete[] reinterpret_cast<char*>(*ptr++);
			break;
		default:
			ptr++;
			break;
		}		
	} while (true);
}

void fatal_exception::raiseFmt(const char* format, ...) {
	va_list args;
	va_start(args, format);
	char buffer[1024];
	VSNPRINTF(buffer, sizeof(buffer), format, args);
	buffer[sizeof(buffer) - 1] = 0;
	va_end(args);
	throw fatal_exception(buffer);
}

void status_exception::raise() 
{
	throw status_exception();
}
	
void status_exception::raise(const ISC_STATUS *status_vector) 
{
	throw status_exception(status_vector);
}
	
void status_exception::raise(ISC_STATUS status, ...) 
{
	va_list args;
	va_start(args, status);
	status_exception ex(status, args);
	va_end(args);
	throw ex;
}

/********************************* system_call_failed ****************************/

system_call_failed::system_call_failed(const char* v_syscall, int v_error_code) : 
	status_exception(isc_sys_request, isc_arg_string, v_syscall, SYS_ARG, v_error_code, isc_arg_end)
{	
}

void system_call_failed::raise(const char* syscall, int error_code)
{
	throw system_call_failed(syscall, error_code);
}

void system_call_failed::raise(const char* syscall)
{
#ifdef WIN_NT
	int error_code = GetLastError();
#else
	int error_code = errno;
#endif
	throw system_call_failed(syscall, error_code);
}

/********************************* fatal_exception ********************************/

fatal_exception::fatal_exception(const char* message) :
	status_exception(isc_random, isc_arg_string, message, isc_arg_end)
{
}

// Moved to the header due to gpre. Gpre non-static can't receive it, but we
// want to avoid problems with picky compilers that can't find what().
// We can't link this file into normal gpre.
// Keep in sync with the constructor above, please; "message" becomes 4th element
// after status_exception's constructor invokes fill_status().
//const char* fatal_exception::what() const throw()
//{
//	return reinterpret_cast<const char*>(value()[3]);
//}

void fatal_exception::raise(const char* message)
{
	throw fatal_exception(message);
}

/************************** exception handling routines ***************************/

static InterlockedStringsBuffer engine_failures;

ISC_STATUS stuff_exception(ISC_STATUS *status_vector, const std::exception& ex, StringsBuffer *sb) throw() 
{
	// Note that this function will call unexpected() that will terminate process 
	// if exception appears during status vector serialization

	if (!sb)
		sb = &engine_failures;
	
	const std::type_info& ex_type = typeid(ex);
	
	if (ex_type == typeid(std::bad_alloc)) {
		*status_vector++ = isc_arg_gds;
		*status_vector++ = isc_virmemexh;
		*status_vector++ = isc_arg_end;
		return isc_virmemexh;
	}
	
	try {
		const status_exception& c_ex = dynamic_cast<const status_exception&>(ex);
		if (c_ex.status_known()) {
			const ISC_STATUS *ptr = c_ex.value();
			if (c_ex.strings_permanent()) 
			{
				// Copy status vector
				do {
					const ISC_STATUS type = *status_vector++ = *ptr++;
					if (type == isc_arg_end)
						break;
					if (type == isc_arg_cstring)
						*status_vector++ = *ptr++;
					*status_vector++ = *ptr++;
				} while (true);
			}
			else {
				// Move in status and clone transient strings
				do {
					const ISC_STATUS type = *status_vector++ = *ptr++;		
					if (type == isc_arg_end)
						break;

					switch (type) {
					case isc_arg_cstring: 
						{				
							const UCHAR len = *status_vector++ = *ptr++;
							char *temp = reinterpret_cast<char*>(*ptr++);
							*status_vector++ = (ISC_STATUS)(IPTR) (sb->alloc(temp, len));
							break;
						}
					case isc_arg_string:
					case isc_arg_interpreted:
						{
							char *temp = reinterpret_cast<char*>(*ptr++);
							*status_vector++ = (ISC_STATUS)(IPTR) (sb->alloc(temp, strlen(temp)));
							break;
						}
					default:
						*status_vector++ = *ptr++;
						break;
					}
				} while (true);
			}
		}
		return status_vector[1];
	} 
	catch (const std::bad_cast&) {
	}
	
	// Other random C++ exceptions
	char temp[256];
	SNPRINTF(temp, sizeof(temp) - 1,
		"Unexpected C++ exception (class=\"%s\", what()=\"%s\")",
		ex_type.name(), ex.what());
	temp[sizeof(temp) - 1] = 0;
	*status_vector++ = isc_arg_gds;
	*status_vector++ = isc_random;
	*status_vector++ = isc_arg_string;
	*status_vector++ = (ISC_STATUS)(IPTR) (sb->alloc(temp, strlen(temp)));
	*status_vector++ = isc_arg_end;	
	return isc_random;
}


const char* status_string(const char* string) 
{
	return status_nstring(string, strlen(string));
}

const char* status_nstring(const char* string, size_t length) 
{
	return engine_failures.alloc(string, length);
}

}	// namespace Firebird

