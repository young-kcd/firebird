#ifndef INCLUDE_FB_BLK
#define INCLUDE_FB_BLK

#define INCLUDE_OLD_FB_BLK

#include "../jrd/common.h"

typedef struct blk
{
	UCHAR	blk_type;
	UCHAR	blk_pool_id;
	USHORT	blk_length;
} *BLK;


#endif	/* INCLUDE_FB_BLK */

