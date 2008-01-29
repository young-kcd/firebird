#ifndef JRD_BLKS_H
#define JRD_BLKS_H

#define BLKDEF(type, root, tail) type,
enum jrd_blk_t { 
        type_MIN_jrd = 0,
#include "../jrd/blk.h"
        type_MAX_jrd
};
#undef BLKDEF

#endif	// JRD_BLKS_H
