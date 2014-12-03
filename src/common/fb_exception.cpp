#include "firebird.h"

//#include "fb_exception.h"

#include <string.h>
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

InterlockedStringsBuffer engine_failures;

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
	
// ********************************* Exception *******************************

Exception::~Exception() throw() { }

/********************************* status_exception *******************************/

status_exception::status_exception() throw() : 
	m_strings_permanent(true)
{
	memset(m_status_vector, 0, sizeof(m_status_vector));
}

status_exception::status_exception(const ISC_STATUS *status_vector, bool permanent) throw() :
	m_strings_permanent(true)
{
	m_status_vector[0] = isc_arg_end;

	if (status_vector)
	{
		set_status(status_vector, permanent);	
	}
}
	
void status_exception::set_status(const ISC_STATUS *new_vector, bool permanent) throw()
{
	fb_assert(new_vector != 0);

	release_vector();

	m_strings_permanent = permanent;

	ISC_STATUS *ptr = m_status_vector;
	while (true) 
	{
		const ISC_STATUS type = *ptr++ = *new_vector++;
		if (type == isc_arg_end)
			break;
		if (type == isc_arg_cstring)
			*ptr++ = *new_vector++;
		*ptr++ = *new_vector++;
	}
}

void status_exception::release_vector() throw()
{
	if (m_strings_permanent)
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

ISC_STATUS status_exception::stuff_exception(ISC_STATUS* const status_vector, StringsBuffer* sb) const throw()
{
	const ISC_STATUS *ptr = value();
	ISC_STATUS *sv = status_vector;
	if (!sb)
	{
		sb = &engine_failures;
	}

	if (strings_permanent()) 
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
	else {
		// Move in status and clone transient strings
		sb->makePermanentVector(sv, ptr);
	}

	return status_vector[1];
}

const char* status_exception::what() const throw()
{
	return "Firebird::status_exception";
}

/********************************* BadAlloc ****************************/

void BadAlloc::raise()
{
	throw BadAlloc();
}

ISC_STATUS BadAlloc::stuff_exception(ISC_STATUS* const status_vector, StringsBuffer* sb) const throw()
{
	ISC_STATUS *sv = status_vector;

	*sv++ = isc_arg_gds;
	*sv++ = isc_virmemexh;
	*sv++ = isc_arg_end;

	return status_vector[1];
}

const char* BadAlloc::what() const throw()
{
	return "Firebird::BadAlloc";
}

/********************************* LongJump ****************************/

void LongJump::raise()
{
	throw LongJump();
}

ISC_STATUS LongJump::stuff_exception(ISC_STATUS* const status_vector, StringsBuffer* sb) const throw()
{
   /*
	* Do nothing for a while - not all utilities are ready, 
	* status_vector is passed in them by other means.
	* Ideally status_exception should be always used for it,
	* and we should activate the following code:

	ISC_STATUS *sv = status_vector;
	if (!sb)
	{
		sb = &engine_failures;
	}
	
	const char *temp = "Unexpected Firebird::LongJump";

	*sv++ = isc_arg_gds;
	*sv++ = isc_random;
	*sv++ = isc_arg_string;
	*sv++ = (ISC_STATUS)(IPTR) (sb->alloc(temp, strlen(temp)));
	*sv++ = isc_arg_end;
	*/
	
	return status_vector[1];
}

const char* LongJump::what() const throw()
{
	return "Firebird::LongJump";
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

void fatal_exception::raise(const char* message)
{
	throw fatal_exception(message);
}

// after initialization of status vector in constructor.
const char* fatal_exception::what() const throw()
{
	return reinterpret_cast<const char*>(value()[3]);
}

/************************** exception handling routines ***************************/

const char* status_string(const char* string) 
{
	return status_nstring(string, strlen(string));
}

const char* status_nstring(const char* string, size_t length) 
{
	return engine_failures.alloc(string, length);
}

// Serialize exception into status_vector, put transient strings from exception into given StringsBuffer
ISC_STATUS stuff_exception(ISC_STATUS *status_vector, const Firebird::Exception& ex, StringsBuffer* sb) throw()
{
	return ex.stuff_exception(status_vector, sb);
}

}	// namespace Firebird

