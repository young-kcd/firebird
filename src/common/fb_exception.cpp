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
	virtual char* alloc(const char* string, size_t& length) 
	{
		buffer_lock.enter();
		char* new_string = CircularBuffer::alloc(string, length);
		buffer_lock.leave();
		return new_string;
	}
private:
	Firebird::Mutex buffer_lock;
};

ISC_STATUS dupStringTemp(const char* s)
{
	const size_t len = strlen(s);
	char *string = FB_NEW(*getDefaultMemoryPool()) char[len + 1];
	memcpy(string, s, len + 1);
	return (ISC_STATUS)(IPTR)(string);
}

void fill_status(ISC_STATUS *ptr, ISC_STATUS status, va_list status_args)
{
	// Move in status and clone transient strings
	*ptr++ = isc_arg_gds;
	*ptr++ = status;
	while (true) 
	{
		const ISC_STATUS type = *ptr++ = va_arg(status_args, ISC_STATUS);
		if (type == isc_arg_end) 
			break;

		switch (type) {
		case isc_arg_cstring: 
			{				
				const size_t len = *ptr++ = va_arg(status_args, ISC_STATUS);
				char *string = FB_NEW(*getDefaultMemoryPool()) char[len];
				const char *temp = reinterpret_cast<char*>(va_arg(status_args, ISC_STATUS));
				memcpy(string, temp, len);
				*ptr++ = (ISC_STATUS)(IPTR)(string);
				break;
			}
		case isc_arg_string:
		case isc_arg_interpreted:
			{
				*ptr++ = dupStringTemp(reinterpret_cast<char*>(va_arg(status_args, ISC_STATUS)));
				break;
			}
		default:
			*ptr++ = va_arg(status_args, ISC_STATUS);
			break;
		}
	}	
}

} // namespace

namespace Firebird {

/********************************* StringsBuffer *******************************/

void StringsBuffer::makePermanentVector(ISC_STATUS* perm, const ISC_STATUS* trans)
{
	while (true) 
	{
		const ISC_STATUS type = *perm++ = *trans++;

		switch (type) {
		case isc_arg_end:
			return;
		case isc_arg_cstring: 
			{				
				size_t len = *perm++ = *trans++;
				const char* temp = reinterpret_cast<char*>(*trans++);
				*perm++ = (ISC_STATUS)(IPTR) (alloc(temp, len));
				perm[-2] = len;
			}
			break;
		case isc_arg_string:
		case isc_arg_interpreted:
			{
				const char* temp = reinterpret_cast<char*>(*trans++);
				size_t len = strlen(temp);
				*perm++ = (ISC_STATUS)(IPTR) (alloc(temp, len));
			}
			break;
		default:
			*perm++ = *trans++;
			break;
		}
	}
}

/********************************* status_exception *******************************/

status_exception::status_exception() throw() : 
	m_strings_permanent(true), m_status_known(false) 
{
	memset(m_status_vector, 0, sizeof(m_status_vector));
}

status_exception::status_exception(const ISC_STATUS *status_vector, bool permanent) throw() : 
	m_strings_permanent(permanent), m_status_known(status_vector != NULL)
{
	if (m_status_known) {
		ISC_STATUS *ptr = m_status_vector;
		 while (true) 
		 {
			const ISC_STATUS type = *ptr++ = *status_vector++;
			if (type == isc_arg_end)
				break;
			if (type == isc_arg_cstring)
				*ptr++ = *status_vector++;
			*ptr++ = *status_vector++;
		}
	}
}
	
void status_exception::set_status(const ISC_STATUS *new_vector, bool permanent) throw()
{
	release_vector();
	m_status_known = (new_vector != NULL); 
	m_strings_permanent = permanent;
	if (m_status_known)
		memcpy(m_status_vector, new_vector, sizeof(m_status_vector));
}

void status_exception::release_vector() throw()
{
	if (m_strings_permanent || (!m_status_known))
		return;
	
	// Free owned strings
	ISC_STATUS *ptr = m_status_vector;
	 while (true) 
	 {
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
	}
}

status_exception::~status_exception() throw() 
{
	release_vector();
}

void fatal_exception::raiseFmt(const char* format, ...) 
{
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
	throw status_exception(status_vector, true);
}
	
void status_exception::raise(ISC_STATUS status, ...) 
{
	va_list args;
	va_start(args, status);
	ISC_STATUS_ARRAY temp;
	fill_status(temp, status, args);
	va_end(args);
	throw status_exception(temp, false);
}

/********************************* system_call_failed ****************************/

system_call_failed::system_call_failed(const char* v_syscall, int v_error_code) : 
	status_exception(0, false), errorCode(v_error_code)
{
	ISC_STATUS temp[] = {isc_arg_gds, 
						isc_sys_request, 
						isc_arg_string, dupStringTemp(v_syscall), 
						SYS_ARG, errorCode, 
						isc_arg_end};
	set_status(temp, false);
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
	status_exception(0, false)
{
	ISC_STATUS temp[] = {isc_arg_gds, 
						isc_random, 
						isc_arg_string, dupStringTemp(message), 
						isc_arg_end};
	set_status(temp, false);
}

// Moved to the header due to gpre. Gpre non-static can't receive it, but we
// want to avoid problems with picky compilers that can't find what().
// We can't link this file into normal gpre.
// Keep in sync with the constructor above, please; "message" becomes 4th element
// after initialization of status vector.
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
			ISC_STATUS *sv = status_vector;
			const ISC_STATUS *ptr = c_ex.value();
			if (c_ex.strings_permanent()) 
			{
				// Copy status vector
				while (true) 
				{
					const ISC_STATUS type = *sv++ = *ptr++;
					if (type == isc_arg_end)
						break;
					if (type == isc_arg_cstring)
						*sv++ = *ptr++;
					*sv++ = *ptr++;
				}
			}
			else 
			{
				// Move in status and clone transient strings
				sb->makePermanentVector(sv, ptr);
			}
		}
		return status_vector[1];
	} 
	catch (const std::bad_cast&) 
	{
	}
	
	// Other random C++ exceptions
	char temp[256];
	SNPRINTF(temp, sizeof(temp) - 1,
		"Unexpected C++ exception (class=\"%s\", what()=\"%s\")",
		ex_type.name(), ex.what());
	temp[sizeof(temp) - 1] = 0;
	ISC_STATUS *sv = status_vector;
	*status_vector++ = isc_arg_gds;
	*status_vector++ = isc_random;
	*status_vector++ = isc_arg_string;
	*status_vector++ = (ISC_STATUS)(IPTR)temp;
	*status_vector++ = isc_arg_end;	
	sb->makePermanentVector(sv, sv);
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

