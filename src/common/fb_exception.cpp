#include "fb_exception.h"

void Firebird::status_exception::raise(STATUS s)
{
	throw status_exception(s);
}
