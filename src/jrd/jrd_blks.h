#ifndef JRD_BLKS_H
#define JRD_BLKS_H

/* #include "../jrd/all.h" */

//extern plb::alloc_info jrd_alloc_info;

#define BLKDEF(type, root, tail) type,
enum jrd_blk_t { 
        type_MIN_jrd = 0,
#include "../jrd/blk.h"
        type_MAX_jrd
};
#undef BLKDEF

#endif	// JRD_BLKS_H
