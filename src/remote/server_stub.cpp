#include "firebird.h"
#include "../jrd/common.h"

#ifndef MINGW

extern "C" {
int CLIB_ROUTINE server_main( int argc, char** argv);
}

// This routine invokes server loop implemented in the shared library
int CLIB_ROUTINE main( int argc, char** argv) {
	return server_main(argc, argv);
}

#endif
