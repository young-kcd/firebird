#ifndef INCLUDE_FB_TYPES_H
#define INCLUDE_FB_TYPES_H

#ifndef JRD_COMMON_H
#include "../jrd/common.h"
#endif


#ifndef DEFINED_GDS_QUAD
#define DEFINED_GDS_QUAD

struct GDS_QUAD_t {
	SLONG gds_quad_high;
	ULONG gds_quad_low;
};

typedef struct GDS_QUAD_t GDS_QUAD;
typedef struct GDS_QUAD_t ISC_QUAD;

#endif /* DEFINED_GDS_QUAD */


#endif /* INCLUDE_FB_TYPES_H */
