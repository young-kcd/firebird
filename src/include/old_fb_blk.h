#ifndef INCLUDE_FB_BLK
#define INCLUDE_FB_BLK

#define INCLUDE_OLD_FB_BLK

#ifndef JRD_COMMON_H
#include "../jrd/common.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct blk
{
	UCHAR	blk_type;
	UCHAR	blk_pool_id;
	USHORT	blk_length;
} *BLK;


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* INCLUDE_FB_BLK */
