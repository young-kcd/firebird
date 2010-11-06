#include "firebird.h"

#include "ProviderInterface.h"

#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include "gen/iberror.h"
#include "../common/classes/alloc.h"
#include "../common/classes/init.h"
#include "../common/classes/array.h"
#include "../common/thd.h"
#include "../common/utils_proto.h"

#ifdef WIN_NT
#include <windows.h>
#endif

namespace {

class StringsBuffer
{
private:
	class ThreadBuffer : public Firebird::GlobalStorage
	{
	private:
		const static size_t BUFFER_SIZE = 4096;
		char buffer[BUFFER_SIZE];
		char* buffer_ptr;
		FB_THREAD_ID thread;

	public:
		explicit ThreadBuffer(FB_THREAD_ID thr) : buffer_ptr(buffer), thread(thr) { }

		const char* alloc(const char* string, size_t length)
		{
			// if string is already in our buffer - return it
			// it was already saved in our buffer once
			if (string >= buffer && string < &buffer[BUFFER_SIZE])
				return string;

			// if string too long, truncate it
			if (length > BUFFER_SIZE / 4)
				length = BUFFER_SIZE / 4;

			// If there isn't any more room in the buffer, start at the beginning again
			if (buffer_ptr + length + 1 > buffer + BUFFER_SIZE)
				buffer_ptr = buffer;

			char* new_string = buffer_ptr;
			memcpy(new_string, string, length);
			new_string[length] = 0;
			buffer_ptr += length + 1;

			return new_string;
		}

		bool thisThread(FB_THREAD_ID currTID)
		{
#ifdef WIN_NT
			if (thread != currTID)
			{
				HANDLE hThread = OpenThread(THREAD_QUERY_INFORMATION, false, thread);
				// commented exit code check - looks like OS does not return handle
				// for already exited thread
				//DWORD exitCode = STILL_ACTIVE;
				if (hThread)
				{
					//GetExitCodeThread(hThread, &exitCode);
					CloseHandle(hThread);
				}

				//if ((!hThread) || (exitCode != STILL_ACTIVE))
				if (!hThread)
				{
					// Thread does not exist any more
					thread = currTID;
				}
			}
#endif

			return thread == currTID;
		}
	};

	typedef Firebird::Array<ThreadBuffer*> ProcessBuffer;

	ProcessBuffer processBuffer;
	Firebird::Mutex mutex;

public:
	explicit StringsBuffer(Firebird::MemoryPool& p) : processBuffer(p) { }

	~StringsBuffer()
	{
		ThreadCleanup::remove(cleanupAllStrings, this);
	}

private:
	size_t position(FB_THREAD_ID thr)
	{
		// mutex should be locked when this function is called

		for (size_t i = 0; i < processBuffer.getCount(); ++i)
		{
			if (processBuffer[i]->thisThread(thr))
			{
				return i;
			}
		}

		return processBuffer.getCount();
	}

	ThreadBuffer* getThreadBuffer(FB_THREAD_ID thr)
	{
		Firebird::MutexLockGuard guard(mutex);

		size_t p = position(thr);
		if (p < processBuffer.getCount())
		{
			return processBuffer[p];
		}

		ThreadBuffer* b = new ThreadBuffer(thr);
		processBuffer.add(b);
		return b;
	}

	void cleanup()
	{
		Firebird::MutexLockGuard guard(mutex);

		size_t p = position(getThreadId());
		if (p >= processBuffer.getCount())
		{
			return;
		}

		delete processBuffer[p];
		processBuffer.remove(p);
	}

	static void cleanupAllStrings(void* toClean)
	{
		static_cast<StringsBuffer*>(toClean)->cleanup();
	}

public:
	const char* alloc(const char* s, size_t len, FB_THREAD_ID thr = getThreadId())
	{
		ThreadCleanup::add(cleanupAllStrings, this);
		return getThreadBuffer(thr)->alloc(s, len);
	}
};

Firebird::GlobalPtr<StringsBuffer> allStrings;

} // namespace

namespace Firebird {

// Before using thr parameter, make sure that thread is not going to work with
// this functions itself.
// CVC: Do not let "perm" be incremented before "trans", because it may lead to serious memory errors,
// since several places in our code blindly pass the same vector twice.
void makePermanentVector(ISC_STATUS* perm, const ISC_STATUS* trans, FB_THREAD_ID thr) throw()
{
	try
	{
		while (true)
		{
			const ISC_STATUS type = *perm++ = *trans++;

			switch (type)
			{
			case isc_arg_end:
				return;
			case isc_arg_cstring:
				{
					perm [-1] = isc_arg_string;
					const size_t len = *trans++;
					const char* temp = reinterpret_cast<char*>(*trans++);
					*perm++ = (ISC_STATUS)(IPTR) (allStrings->alloc(temp, len, thr));
				}
				break;
			case isc_arg_string:
			case isc_arg_interpreted:
			case isc_arg_sql_state:
				{
					const char* temp = reinterpret_cast<char*>(*trans++);
					const size_t len = strlen(temp);
					*perm++ = (ISC_STATUS)(IPTR) (allStrings->alloc(temp, len, thr));
				}
				break;
			default:
				*perm++ = *trans++;
				break;
			}
		}
	}
	catch (const system_call_failed& ex)
	{
		memcpy(perm, ex.value(), sizeof(ISC_STATUS_ARRAY));
	}
	catch (const BadAlloc&)
	{
		*perm++ = isc_arg_gds;
		*perm++ = isc_virmemexh;
		*perm++ = isc_arg_end;
	}
	catch (...)
	{
		*perm++ = isc_arg_gds;
		*perm++ = isc_random;
		*perm++ = isc_arg_string;
		*perm++ = (ISC_STATUS)(IPTR) "Unexpected exception in makePermanentVector()";
		*perm++ = isc_arg_end;
	}
}

void makePermanentVector(ISC_STATUS* v, FB_THREAD_ID thr) throw()
{
	makePermanentVector(v, v, thr);
}

// ********************************* Exception *******************************

Exception::~Exception() throw() { }

ISC_STATUS Exception::stuff_exception(ISC_STATUS* const status_vector) const throw()
{
	Status* status = FbApi::fb_get_status_instance();
	stuffException(status);
	const ISC_STATUS* s = status->get();
	fb_utils::copyStatus(status_vector, ISC_STATUS_LENGTH, s, fb_utils::statusLength(s));
	status->release();

	return status_vector[1];
}

// ********************************* status_exception *******************************

status_exception::status_exception() throw()
{
	memset(m_status_vector, 0, sizeof(m_status_vector));
}

status_exception::status_exception(const ISC_STATUS *status_vector) throw()
{
	memset(m_status_vector, 0, sizeof(m_status_vector));

	if (status_vector)
	{
		set_status(status_vector);
	}
}

void status_exception::set_status(const ISC_STATUS *new_vector) throw()
{
	fb_assert(new_vector != 0);

	makePermanentVector(m_status_vector, new_vector);
}

status_exception::~status_exception() throw()
{
}

const char* status_exception::what() const throw()
{
	return "Firebird::status_exception";
}

void status_exception::raise(const ISC_STATUS *status_vector)
{
	throw status_exception(status_vector);
}

void status_exception::raise(const Arg::StatusVector& statusVector)
{
	throw status_exception(statusVector.value());
}

ISC_STATUS status_exception::stuffException(Status* status) const throw()
{
	if (status)
	{
		status->set(value());
	}

	return value()[1];
}

// ********************************* BadAlloc ****************************

void BadAlloc::raise()
{
	throw BadAlloc();
}

ISC_STATUS BadAlloc::stuffException(Status* status) const throw()
{
	const ISC_STATUS sv[] = {isc_arg_gds, isc_virmemexh};

	if (status)
	{
		status->set(FB_NELEM(sv), sv);
	}

	return sv[1];
}

const char* BadAlloc::what() const throw()
{
	return "Firebird::BadAlloc";
}

// ********************************* LongJump ***************************

void LongJump::raise()
{
	throw LongJump();
}

ISC_STATUS LongJump::stuffException(Status* status) const throw()
{
	ISC_STATUS sv[] = {isc_arg_gds, isc_random, isc_arg_string, (ISC_STATUS)(IPTR)"Unexpected Firebird::LongJump"};

	if (status)
	{
		status->set(FB_NELEM(sv), sv);
	}

	return sv[1];
}

const char* LongJump::what() const throw()
{
	return "Firebird::LongJump";
}


// ********************************* system_error ***************************

system_error::system_error(const char* syscall, int error_code) :
	status_exception(), errorCode(error_code)
{
	Arg::Gds temp(isc_sys_request);
	temp << Arg::Str(syscall);
	temp << SYS_ERR(errorCode);
	set_status(temp.value());
}

void system_error::raise(const char* syscall, int error_code)
{
	throw system_error(syscall, error_code);
}

void system_error::raise(const char* syscall)
{
	throw system_error(syscall, getSystemError());
}

int system_error::getSystemError()
{
#ifdef WIN_NT
	return GetLastError();
#else
	return errno;
#endif
}

// ********************************* system_call_failed ***************************

system_call_failed::system_call_failed(const char* syscall, int error_code) :
	system_error(syscall, error_code)
{
	// NS: something unexpected has happened. Log the error to log file
	// In the future we may consider terminating the process even in PROD_BUILD
	gds__log("Operating system call %s failed. Error code %d", syscall, error_code);
#ifdef DEV_BUILD
	// raised failed system call exception in DEV_BUILD in 99.99% means
	// problems with the code - let's create memory dump now
	abort();
#endif
}

void system_call_failed::raise(const char* syscall, int error_code)
{
	throw system_call_failed(syscall, error_code);
}

void system_call_failed::raise(const char* syscall)
{
	throw system_call_failed(syscall, getSystemError());
}

// ********************************* fatal_exception *******************************

fatal_exception::fatal_exception(const char* message) :
	status_exception()
{
	const ISC_STATUS temp[] =
	{
		isc_arg_gds,
		isc_random,
		isc_arg_string,
		(ISC_STATUS)(IPTR) message,
		isc_arg_end
	};
	set_status(temp);
}

// Keep in sync with the constructor above, please; "message" becomes 4th element
// after initialization of status vector in constructor.
const char* fatal_exception::what() const throw()
{
	return reinterpret_cast<const char*>(value()[3]);
}

void fatal_exception::raise(const char* message)
{
	throw fatal_exception(message);
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

// ************************** exception handling routines **************************

// Serialize exception into status_vector
ISC_STATUS stuff_exception(ISC_STATUS *status_vector, const Firebird::Exception& ex) throw()
{
	return ex.stuff_exception(status_vector);
}

ISC_STATUS stuff_exception(Status* status, const Firebird::Exception& ex) throw()
{
	return ex.stuffException(status);
}

}	// namespace Firebird
