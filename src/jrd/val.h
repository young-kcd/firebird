/*
 *	PROGRAM:	JRD access method
 *	MODULE:		val.h
 *	DESCRIPTION:	Definitions associated with value handling
 *
 * The contents of this file are subject to the Interbase Public
 * License Version 1.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy
 * of the License at http://www.Inprise.com/IPL.html
 *
 * Software distributed under the License is distributed on an
 * "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
 * or implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code was created by Inprise Corporation
 * and its predecessors. Portions created by Inprise Corporation are
 * Copyright (C) Inprise Corporation.
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 *
 */

#ifndef JRD_VAL_H
#define JRD_VAL_H

#include "../jrd/jrd_blks.h"
#include "../include/fb_blk.h"
#include "../include/fb_vector.h"

#include "../jrd/dsc.h"


#define FLAG_BYTES(n)	(((n + BITS_PER_LONG) & ~((ULONG)BITS_PER_LONG - 1)) >> 3)

#ifndef VMS
#define DEFAULT_DOUBLE	dtype_double
#else

#define DEFAULT_DOUBLE	dtype_double
#define SPECIAL_DOUBLE	dtype_d_float
#define CNVT_TO_DFLT(x)	MTH$CVT_D_G (x)
#define CNVT_FROM_DFLT(x)	MTH$CVT_G_D (x)

#endif

#ifndef REQUESTER
class fmt : public pool_alloc<type_fmt>
{
public:
	fmt(MemoryPool& p, int len)
	:	fmt_desc(len, p, type_fmt)
	{
	}
	static fmt* newFmt(MemoryPool& p, int len = 0)
		{ return FB_NEW(p) fmt(p, len); }

	USHORT fmt_length;
	USHORT fmt_count;
	USHORT fmt_version;
	Firebird::vector<dsc> fmt_desc;
	typedef Firebird::vector<dsc>::iterator fmt_desc_iterator;
};
typedef fmt* FMT;
#endif /* REQUESTER */

#define MAX_FORMAT_SIZE		65535

typedef vary VARY;

/* A macro to define a local vary stack variable of a given length
   Usage:  VARY_STR(5)	my_var;        */

#define VARY_STR(x)	struct { USHORT vary_length; UCHAR vary_string [x]; }

#ifndef REQUESTER
/* Function definition block */

typedef enum {
		FUN_value,
		FUN_reference,
		FUN_descriptor,
		FUN_blob_struct,
		FUN_scalar_array
} FUN_T;

struct fun_repeat {
	DSC fun_desc;			/* Datatype info */
	FUN_T fun_mechanism;	/* Passing mechanism */
};

class fun : public pool_alloc_rpt<fun_repeat, type_fun>
{
    public:
	STR fun_exception_message;	/* message containing the exception error message */
	struct fun *fun_homonym;	/* Homonym functions */
	struct sym *fun_symbol;		/* Symbol block */
	int (*fun_entrypoint) ();	/* Function entrypoint */
	USHORT fun_count;			/* Number of arguments (including return) */
	USHORT fun_args;			/* Number of input arguments */
	USHORT fun_return_arg;		/* Return argument */
	USHORT fun_type;			/* Type of function */
	ULONG fun_temp_length;		/* Temporary space required */
    fun_repeat fun_rpt[1];
};
typedef fun* FUN;

// Those two defines seems an intention to do something that was completed.
#define FUN_value	0
#define FUN_boolean	1

/* Blob passing structure */
// CVC: Moved to fun.epp where it belongs.

/* Scalar array descriptor */

typedef struct sad {
	DSC sad_desc;
	SLONG sad_dimensions;
	struct sad_repeat {
		SLONG sad_lower;
		SLONG sad_upper;
	} sad_rpt[1];
} *SAD;
#endif /* REQUESTER */

/* Array description */

typedef struct ads {
	UCHAR ads_version;			/* Array descriptor version number */
	UCHAR ads_dimensions;		/* Dimensions of array */
	USHORT ads_struct_count;	/* Number of struct elements */
	USHORT ads_element_length;	/* Length of array element */
	USHORT ads_length;			/* Length of array descriptor */
	SLONG ads_count;			/* Total number of elements */
	SLONG ads_total_length;		/* Total length of array */
	struct ads_repeat {
		DSC ads_desc;			/* Element descriptor */
		SLONG ads_length;		/* Length of "vector" element */
		SLONG ads_lower;		/* Lower bound */
		SLONG ads_upper;		/* Upper bound */
	};
	struct ads_repeat ads_rpt[1];
} *ADS;

#define ADS_VERSION_1	1
#ifdef __cplusplus
#define ADS_LEN(count)	(sizeof (struct ads) + (count - 1) * sizeof (struct ads::ads_repeat))
#else
#define ADS_LEN(count)	(sizeof (struct ads) + (count - 1) * sizeof (struct ads_repeat))
#endif

#ifndef REQUESTER
class arr : public pool_alloc_rpt<ads::ads_repeat, type_arr>
{
    public:
	UCHAR*		arr_data;			/* Data block, if allocated */
	class blb*	arr_blob;		/* Blob for data access */
	struct jrd_tra* arr_transaction;	/* Parent transaction block */
	struct arr* arr_next;		/* Next array in transaction */
	struct jrd_req* arr_request;	/* request */
	SLONG		arr_effective_length;	/* Length of array instance */
	USHORT		arr_desc_length;		/* Length of array descriptor */
	struct ads	arr_desc;		/* Array descriptor */
};
typedef arr* ARR;

#endif /* REQUESTER */


#endif /* JRD_VAL_H */

