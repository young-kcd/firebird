/* This file is a rebuild of head.cpp with the GDS_PIPE defined 
It has been done like this to make the makefile dependencies simple
MOD 30-July-2001 
*/

#if (defined __cplusplus) && (defined SOLX86)
/* Who else got mixed c and C++ linkage error - let join me. KLK
*/
extern "C" {
#endif

#define GDS_PIPE "bin/gds_pipe5"
#include "head.cpp"

#if (defined __cplusplus) && (defined SOLX86)
};
#endif
