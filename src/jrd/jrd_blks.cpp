#include "../jrd/all.h"

plb::alloc_info jrd_alloc_info =
{
    jrd_blk_t.type_MIN,
    jrd_blk_t.type_MAX,
    {
        {0, 0},
#define BLKDEF(type, root, tail) { sizeof (struct root), tail },
#ifndef GATEWAY
#include "../jrd/blk.h"
#else
#include ".._gway/gway/blk.h"
#endif
#undef BLKDEF
        {0, 0}
    },
    {
        "type_MIN",
#define BLKDEF(type, root, tail)  #type,
#ifndef GATEWAY
#include "../jrd/blk.h"
#else
#include ".._gway/gway/blk.h"
#endif
#undef BLKDEF
        "type_MAX"
    }

#ifdef SUPERSERVER
    ,
#define BLKDEF(type, root, tail)  0,
    {
#include "../jrd/blk.h"
        0
#undef BLKDEF
    },
    sizeof(jrd_alloc_info.all_block_type_count)
#endif
};
