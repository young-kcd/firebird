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

namespace Jrd {

class ArrayField;
class blb;
class jrd_req;
class jrd_tra;
class Symbol;


class Format : public pool_alloc<type_fmt>
{
public:
	Format(MemoryPool& p, int len)
	:	fmt_desc(len, p, type_fmt)
	{
	}
	static Format* newFormat(MemoryPool& p, int len = 0)
	{ 
		return FB_NEW(p) Format(p, len); 
	}

	USHORT fmt_length;
	USHORT fmt_count;
	USHORT fmt_version;
	Firebird::vector<dsc> fmt_desc;
	typedef Firebird::vector<dsc>::iterator fmt_desc_iterator;
	typedef Firebird::vector<dsc>::const_iterator fmt_desc_const_iterator;
};
#endif /* REQUESTER */

#define MAX_FORMAT_SIZE		65535

//typedef vary VARY;

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


class UserFunction : public pool_alloc_rpt<fun_repeat, type_fun>
{
    public:
	Firebird::string fun_exception_message;	/* message containing the exception error message */
	UserFunction*	fun_homonym;	/* Homonym functions */
	Symbol*		fun_symbol;			/* Symbol block */
	int (*fun_entrypoint) ();		/* Function entrypoint */
	USHORT		fun_count;			/* Number of arguments (including return) */
	USHORT		fun_args;			/* Number of input arguments */
	USHORT		fun_return_arg;		/* Return argument */
	USHORT		fun_type;			/* Type of function */
	ULONG		fun_temp_length;	/* Temporary space required */
    fun_repeat fun_rpt[1];
    public:
	UserFunction(MemoryPool& p) : fun_exception_message(p) { }
};

// Those two defines seems an intention to do something that wasn't completed.
#define FUN_value	0
#define FUN_boolean	1

/* Blob passing structure */
// CVC: Moved to fun.epp where it belongs.

/* Scalar array descriptor, "external side" seen by UDF's */

struct scalar_array_desc {
	DSC sad_desc;
	SLONG sad_dimensions;
	struct sad_repeat {
		SLONG sad_lower;
		SLONG sad_upper;
	} sad_rpt[1];
};

#endif /* REQUESTER */


/* Array description, "internal side" used by the engine. */
// Renamed internal_array_desc to end the confusion.

struct internal_array_desc {
	UCHAR iad_version;			/* Array descriptor version number */
	UCHAR iad_dimensions;		/* Dimensions of array */
	USHORT iad_struct_count;	/* Number of struct elements */
	USHORT iad_element_length;	/* Length of array element */
	USHORT iad_length;			/* Length of array descriptor */
	SLONG iad_count;			/* Total number of elements */
	SLONG iad_total_length;		/* Total length of array */
	struct iad_repeat {
		DSC iad_desc;			/* Element descriptor */
		SLONG iad_length;		/* Length of "vector" element */
		SLONG iad_lower;		/* Lower bound */
		SLONG iad_upper;		/* Upper bound */
	};
	iad_repeat iad_rpt[1];
};

#define IAD_VERSION_1	1

/*
inline int IAD_LEN(int count)
{
	if (!count)
		count = 1;
	return sizeof (internal_array_desc) + 
		(count - 1) * sizeof (internal_array_desc::iad_repeat);
}
*/
#define IAD_LEN(count)	(sizeof (internal_array_desc) + \
	(count ? count - 1: count) * sizeof (internal_array_desc::iad_repeat))

#ifndef REQUESTER

// Sorry for the clumsy name, but in blk.h this is referred as array description.
// Since we already have Scalar Array Descriptor and Array Description [Slice],
// it was too confusing. Therefore, it was renamed ArrayField, since ultimately,
// it represents an array field the user can manipulate.

class ArrayField : public pool_alloc_rpt<internal_array_desc::iad_repeat, type_arr>
{
    public:
	UCHAR*				arr_data;			/* Data block, if allocated */
	blb*				arr_blob;			/* Blob for data access */
	jrd_tra*			arr_transaction;	/* Parent transaction block */
	ArrayField*			arr_next;			/* Next array in transaction */
	jrd_req*			arr_request;		/* request */
	SLONG				arr_effective_length;	/* Length of array instance */
	USHORT				arr_desc_length;	/* Length of array descriptor */
	internal_array_desc	arr_desc;			/* Array descriptor */
	ULONG				arr_temp_id;		// Temporary ID for open array inside the transaction
};


#endif /* REQUESTER */

} //namespace Jrd

#endif /* JRD_VAL_H */

