#include "firebird.h"

//#include "fb_exception.h"

#include <string.h>

namespace Firebird {

void status_exception::raise(const ISC_STATUS s)
{
	throw status_exception(s);
}

void red_zone_error::raise()
{
	throw red_zone_error();
}

void memory_corrupt::raise()
{
	throw memory_corrupt();
}

void system_call_failed::raise()
{
	throw system_call_failed();
}

fatal_exception::fatal_exception(const char* message)
{
	strncpy(txt, message, 255);
	txt[255] = 0;
}

void fatal_exception::raise(const char* message)
{
	throw fatal_exception(message);
}

}	// namespace Firebird

