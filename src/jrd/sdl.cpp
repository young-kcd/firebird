/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		sdl.c
 *	DESCRIPTION:	Array slice manipulator
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
 */

#include "firebird.h"
#include <string.h>
#include "../jrd/common.h"
#include <stdarg.h>
#ifndef REQUESTER
#include "../jrd/jrd.h"
#endif
#include "../jrd/gds.h"
#include "../jrd/val.h"
#include "../jrd/sdl.h"
#include "../jrd/intl.h"
#include "../jrd/gds_proto.h"
#include "../jrd/sdl_proto.h"

#ifndef DEV_BUILD
#define _assert(ex)
#define assert(ex)

#else	// DEV_BUILD

#include "../jrd/ib_stdio.h"
#ifdef _assert
#undef _assert
#endif
#ifndef __LINE__IS_INT
#define _assert(ex)	{if (!(ex)){(void) ib_fprintf (ib_stderr, "assert failure: %s %ld\n", __FILE__, __LINE__);}}
#else
#define _assert(ex)	{if (!(ex)){(void) ib_fprintf (ib_stderr, "assert failure: %s %d\n", __FILE__, __LINE__);}}
#endif
#ifdef assert
#undef assert
#endif
#define assert(ex)	_assert(ex)

#endif	// DEV_BUILD


#define COMPILE_SIZE	256

typedef struct sdl_arg {
	USHORT sdl_arg_mode;
	ADS sdl_arg_desc;
	UCHAR *sdl_arg_sdl;
	UCHAR *sdl_arg_array;
	SLONG *sdl_arg_variables;
	void (*sdl_arg_callback) ();
	SLICE sdl_arg_argument;
	STATUS *sdl_arg_status_vector;
	IPTR sdl_arg_compiled[COMPILE_SIZE];
	IPTR *sdl_arg_next;
	IPTR *sdl_arg_end;
} *SDL_ARG;

/* Structure to computes ranges */

typedef struct rng {
	SLONG rng_minima[64];
	SLONG rng_maxima[64];
	SDL_INFO rng_info;
} *RNG;

static UCHAR *compile(UCHAR *, SDL_ARG);
static STATUS error(STATUS *, ...);
static BOOLEAN execute(SDL_ARG);
static UCHAR *get_range(UCHAR *, RNG, SLONG *, SLONG *);
static SSHORT get_word(UCHAR **);
static UCHAR *sdl_desc(UCHAR *, DSC *);
static IPTR *stuff(IPTR, SDL_ARG);


#define op_literal	1
#define op_variable	2
#define op_add		3
#define op_subtract	4
#define op_multiply	5
#define op_divide	6
#define op_iterate	7
#define op_goto		8
#define op_element	9
#define op_loop		10
#define op_exit		11
#define op_scalar	12

/*
   The structure for a loop is:

	<upper_bound> <increment> <initial_value> 
	<loop> <iterate> <variable> <exit_address> 
	[body] 
	<goto> <iterate_address>
	[exit]
*/


SLONG DLL_EXPORT SDL_compute_subscript(
									   STATUS * status_vector,
									   ADS desc,
									   USHORT dimensions, SLONG * subscripts)
{
/**************************************
 *
 *	S D L _ c o m p u t e _ s u b s c r i p t
 *
 **************************************
 *
 * Functional description
 *	Collapse a multi-dimension array reference into a vector
 *	reference. 
 *
 **************************************/
	SLONG subscript, n;
	ads::ads_repeat * range;
	ads::ads_repeat * end;

	if (dimensions != desc->ads_dimensions) {
		error(status_vector, gds_invalid_dimension,
			  gds_arg_number, (SLONG) desc->ads_dimensions,
			  gds_arg_number, (SLONG) dimensions, 0);
		return -1;
	}

	subscript = 0;

	for (range = desc->ads_rpt, end = range + desc->ads_dimensions;
		 range < end; ++range) {
		n = *subscripts++;
		if (n < range->ads_lower || n > range->ads_upper) {
			error(status_vector, gds_out_of_bounds, 0);
			return -1;
		}
		subscript += (n - range->ads_lower) * range->ads_length;
	}

	return subscript;
}


STATUS API_ROUTINE SDL_info(STATUS * status_vector,
							UCHAR * sdl, SDL_INFO info, SLONG * vector)
{
/**************************************
 *
 *	S D L _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Parse enough of SDL to pick relation and field identification and
 *	element descriptor.
 *
 **************************************/
	UCHAR *p;
	TEXT *q;
	USHORT n, offset;
	SLONG min, max;
	struct rng range;

	p = sdl;
	info->sdl_info_fid = info->sdl_info_rid = 0;
	info->sdl_info_relation[0] = info->sdl_info_field[0] = 0;

	if (*p++ != gds_sdl_version1)
		return error(status_vector, gds_invalid_sdl,
					 gds_arg_number, (SLONG) 0, 0);

	for (;;)
		switch (*p++) {
		case gds_sdl_struct:
			n = *p++;
			if (n != 1)
				return error(status_vector, gds_invalid_sdl,
							 gds_arg_number, (SLONG) (p - sdl - 1), 0);
			offset = p - sdl;
			if (!(p = sdl_desc(p, &info->sdl_info_element)))
				return error(status_vector, gds_invalid_sdl,
							 gds_arg_number, (SLONG) offset, 0);
			info->sdl_info_element.dsc_address = 0;
			break;

		case gds_sdl_fid:
			info->sdl_info_fid = get_word(&p);
			break;

		case gds_sdl_rid:
			info->sdl_info_rid = get_word(&p);
			break;

		case gds_sdl_field:
			for (n = *p++, q = info->sdl_info_field; n; --n)
				*q++ = (TEXT) * p++;
			*q = 0;
			break;

		case gds_sdl_relation:
			for (n = *p++, q = info->sdl_info_relation; n; --n)
				*q++ = (TEXT) * p++;
			*q = 0;
			break;

		default:
			info->sdl_info_dimensions = 0;
			if (vector) {
				memcpy(range.rng_minima, vector, sizeof(range.rng_minima));
				memcpy(range.rng_maxima, vector, sizeof(range.rng_maxima));
				range.rng_info = info;
				min = max = -1;
				if (!(p = get_range(p - 1, &range, &min, &max))
					|| (((UCHAR) * p) != gds_sdl_eoc))
					info->sdl_info_dimensions = 0;
			}
			return FB_SUCCESS;
		}
}


UCHAR *DLL_EXPORT SDL_prepare_slice(UCHAR * sdl, USHORT sdl_length)
{
/**************************************
 *
 *	S D L _ p r e p a r e _ s l i c e
 *
 **************************************
 *
 * Functional description
 *	Parse a slice and convert each occurrence of
 *	blr_d_float to blr_double.
 *
 **************************************/
	UCHAR*	new_sdl;
	UCHAR*	old_sdl;
	DSC		junk;
	USHORT	n;

	new_sdl = old_sdl = sdl;

	if (*sdl++ != gds_sdl_version1)
		return old_sdl;

	while ((UCHAR) * sdl != gds_sdl_eoc)
	{
		switch (*sdl++)
		{
		case gds_sdl_struct:
			for (n = *sdl++; n; --n)
			{
				if (*sdl == blr_d_float)
				{
					if (new_sdl == old_sdl)
					{
						new_sdl = (UCHAR*)gds__alloc((SLONG) sdl_length);
						/* FREE: apparently never freed */
						if (!new_sdl)
						{	/* NOMEM: ignore operation */
							assert(FALSE);	/* no real error handling */
							return old_sdl;
						}
						memcpy(new_sdl, old_sdl, sdl_length);
						sdl = new_sdl + (sdl - old_sdl);
					}
					*sdl = blr_double;
				}

				if (!(sdl = sdl_desc(sdl, &junk)))
					return new_sdl;
			}
			break;

		case gds_sdl_fid:
		case gds_sdl_rid:
			sdl += 2;
			break;

		case gds_sdl_field:
		case gds_sdl_relation:
			n = *sdl++;
			sdl += n;
			break;

		default:
			return new_sdl;
		}
	}

	return new_sdl;
}


int DLL_EXPORT SDL_walk(
						STATUS * status_vector,
						UCHAR * sdl,
						USHORT mode,
						UCHAR * array,
ADS array_desc, SLONG * variables, void (*callback) (), SLICE argument)
{
/**************************************
 *
 *	S D L _ w a l k
 *
 **************************************
 *
 * Functional description
 *	Walk a slice.  
 *
 **************************************/
	UCHAR *p;
	DSC junk;
	USHORT n, offset;
	struct sdl_arg arg;

	arg.sdl_arg_mode = mode;
	arg.sdl_arg_array = array;
	arg.sdl_arg_sdl = sdl;
	arg.sdl_arg_desc = array_desc;
	arg.sdl_arg_variables = variables;
	arg.sdl_arg_callback = callback;
	arg.sdl_arg_argument = argument;
	arg.sdl_arg_status_vector = status_vector;
	p = sdl + 1;

	while ((UCHAR) * p != gds_sdl_eoc) {
		switch (*p++) {
		case gds_sdl_struct:
			for (n = *p++; n; --n) {
				offset = p - sdl - 1;
				if (!(p = sdl_desc(p, &junk))) 
					return error(status_vector, gds_invalid_sdl,
								 gds_arg_number, (SLONG) offset, 0);
			}
			break;

		case gds_sdl_fid:
		case gds_sdl_rid:
			p += 2;
			break;

		case gds_sdl_field:
		case gds_sdl_relation:
			n = *p++;
			p += n;
			break;

		default:
			/* Check that element is in range of valid SDL */
			assert(*(p - 1) >= gds_sdl_version1
				   && *(p - 1) <= gds_sdl_element);

			arg.sdl_arg_next = arg.sdl_arg_compiled;
			arg.sdl_arg_end =
				(IPTR *) ((SCHAR *) arg.sdl_arg_compiled +
						  sizeof(arg.sdl_arg_compiled));
			if (!(p = compile(p - 1, &arg)))
				return FB_FAILURE;
			if (!stuff((IPTR) op_exit, &arg))
				return FB_FAILURE;
			if (!execute(&arg))
				return FB_FAILURE;
			break;
		}
    }

	return FB_SUCCESS;
}


static UCHAR *compile(UCHAR * sdl, SDL_ARG arg)
{
/**************************************
 *
 *	c o m p i l e
 *
 **************************************
 *
 * Functional description
 *	Compile an SDL element.  If the address of the argument block
 *	is null, parse, but do not generate anything.
 *
 **************************************/
	SLONG n, count, variable, value, op, operator_;
	IPTR *label;
	UCHAR *p, *ptr1, *expressions[16], **expr;

#define STUFF(word, arg)	if (!stuff ((IPTR) word, arg)) return NULL
#define COMPILE(p, arg)		if (!(p = compile (p, arg))) return NULL

	p = sdl;

	switch (op = *p++) {
	case gds_sdl_do1:
	case gds_sdl_do2:
	case gds_sdl_do3:
		variable = *p++;
		if (op == gds_sdl_do1)
			ptr1 = NULL;
		else {
			ptr1 = p;
			COMPILE(p, 0);		/* skip over lower bound */
		}
		COMPILE(p, arg);		/* upper bound */
		if (op == gds_sdl_do3) {
			COMPILE(p, arg);	/* increment */
		}
		else {
			STUFF(op_literal, arg);
			STUFF(1, arg);
		}
		if (ptr1) {
			COMPILE(ptr1, arg);	/* initial value */
		}
		else {
			STUFF(op_literal, arg);	/* default initial value */
			STUFF(1, arg);
		}
		STUFF(op_loop, arg);
		if (!(label = stuff(op_iterate, arg)))
			return NULL;
		STUFF(variable, arg);
		STUFF(0, arg);			/* future branch out address */
		if (!(p = compile(p, arg)))
			return NULL;
		STUFF(op_goto, arg);
		STUFF(label, arg);
		if (arg)
			label[2] = (IPTR) arg->sdl_arg_next;
		return p;

	case gds_sdl_variable:
		STUFF(op_variable, arg);
		STUFF(*p++, arg);
		return p;

	case gds_sdl_tiny_integer:
		value = (SCHAR) * p++;
		STUFF(op_literal, arg);
		STUFF(value, arg);
		return p;

	case gds_sdl_short_integer:
		value = (SSHORT) (p[0] | (p[1] << 8));
		STUFF(op_literal, arg);
		STUFF(value, arg);
		return p + 2;

	case gds_sdl_long_integer:
		value =
			(SLONG) (p[0] | (p[1] << 8) | ((SLONG) p[2] << 16) |
					 ((SLONG) p[3] << 24));
		STUFF(op_literal, arg);
		STUFF(value, arg);
		return p + 4;

	case gds_sdl_add:
		operator_ = op_add;
	case gds_sdl_subtract:
		if (!operator_)
			operator_ = op_subtract;
	case gds_sdl_multiply:
		if (!operator_)
			operator_ = op_multiply;
	case gds_sdl_divide:
		if (!operator_)
			operator_ = op_divide;
		COMPILE(p, arg);
		COMPILE(p, arg);
		STUFF(operator_, arg);
		return p;

	case gds_sdl_scalar:
		op = *p++;
		count = *p++;
		if (arg && count != arg->sdl_arg_desc->ads_dimensions) {
			error(arg->sdl_arg_status_vector, gds_invalid_dimension,
				  gds_arg_number, (SLONG) arg->sdl_arg_desc->ads_dimensions,
				  gds_arg_number, (SLONG) count, 0);
			return NULL;
		}
		expr = expressions;
		for (n = count; n; --n) {
			*expr++ = p;
			COMPILE(p, 0);
		}
		while (expr > expressions)
			if (!compile(*--expr, arg))
				return NULL;
		STUFF(op_scalar, arg);
		STUFF(op, arg);
		STUFF(count, arg);
		return p;

	case gds_sdl_element:
		count = *p++;
		if (arg && count != 1) {
			error(arg->sdl_arg_status_vector, gds_datnotsup, gds_arg_end);
			/* Msg107: "data operation not supported" (arrays of structures) */
			return NULL;
		}
		expr = expressions;
		for (n = count; n; --n) {
			*expr++ = p;
			COMPILE(p, 0);
		}
		while (expr > expressions)
			if (!compile(*--expr, arg))
				return NULL;
		STUFF(op_element, arg);
		STUFF(count, arg);
		return p;

	default:
		error(arg->sdl_arg_status_vector, gds_invalid_sdl,
			  gds_arg_number, (SLONG) (p - arg->sdl_arg_sdl - 1), 0);
		return NULL;
	}
}


static STATUS error(STATUS * status_vector, ...)
{
/**************************************
 *
 *	e r r o r
 *
 **************************************
 *
 * Functional description
 *	Post an error sequence to the status vector.  Since an error
 *	sequence can, in theory, be arbitrarily lock, pull a cheap
 *	trick to get the address of the argument vector.
 *
 **************************************/
	va_list args;
	STATUS *p;
	int type;

/* Get the addresses of the argument vector and the status vector, and do
   word-wise copy. */

	VA_START(args, status_vector);
	p = status_vector;

/* Copy first argument */

	*p++ = gds_arg_gds;
	*p++ = va_arg(args, STATUS);

/* Pick up remaining args */

	while (*p++ = type = va_arg(args, int))
		switch (type) {
		case gds_arg_gds:
			*p++ = (STATUS) va_arg(args, STATUS);
			break;

		case gds_arg_string:
		case gds_arg_interpreted:
			*p++ = (STATUS) va_arg(args, TEXT *);
			break;

/****
	case gds_arg_cstring:
	    *p++ = (STATUS) va_arg (args, int);
	    *p++ = (STATUS) va_arg (args, TEXT*);
	    break;
****/

		case gds_arg_number:
			*p++ = va_arg(args, SLONG);
			break;

		default:
			assert(FALSE);
		case gds_arg_vms:
		case gds_arg_unix:
			*p++ = va_arg(args, int);
			break;
		}

	return status_vector[1];
}


static BOOLEAN execute(SDL_ARG arg)
{
/**************************************
 *
 *	e x e c u t e
 *
 **************************************
 *
 * Functional description
 *	Execute compiled slice description language.
 *
 **************************************/
	SLONG *variables, *variable, stack[64], *stack_ptr,
		value, count, n, subscript, x;
	IPTR *next;
	ADS array_desc;
	DSC element_desc;
	ads::ads_repeat * range;
	ads::ads_repeat * range_end;
	DSC *slice_desc;

	array_desc = arg->sdl_arg_desc;
	range_end = array_desc->ads_rpt + array_desc->ads_dimensions;
	variables = arg->sdl_arg_variables;
	next = arg->sdl_arg_compiled;
	stack_ptr = stack + 64;

	for (;;) {
		x = *next++;
		switch (x) {
		case op_literal:
			*--stack_ptr = *next++;
			break;

		case op_variable:
			*--stack_ptr = variables[*next++];
			break;

		case op_add:
			value = *stack_ptr++;
			*stack_ptr += value;
			break;

		case op_subtract:
			value = *stack_ptr++;
			*stack_ptr -= value;
			break;

		case op_multiply:
			value = *stack_ptr++;
			*stack_ptr *= value;
			break;

		case op_divide:
			value = *stack_ptr++;
			*stack_ptr /= value;
			break;

		case op_goto:
			next = (IPTR *) * next;
			break;

		case op_loop:
			variable = variables + next[1];
			*variable = *stack_ptr++;
			if (*variable > stack_ptr[1]) {
				next = (IPTR *) next[2];
				stack_ptr += 2;
			}
			else
				next += 3;
			break;

		case op_iterate:
			variable = variables + next[0];
			*variable += *stack_ptr;
			if (*variable > stack_ptr[1]) {
				next = (IPTR *) next[1];
				stack_ptr += 2;
			}
			else
				next += 2;
			break;

		case op_scalar:
			value = *next++;
			next++;				/* Skip count, unsupported. */
			for (range = array_desc->ads_rpt, subscript = 0;
				 range < range_end; ++range) {
				n = *stack_ptr++;
				if (n < range->ads_lower || n > range->ads_upper) {
					error(arg->sdl_arg_status_vector, gds_out_of_bounds, 0);
					return FALSE;
				}
				subscript += (n - range->ads_lower) * range->ads_length;
			}
			element_desc = array_desc->ads_rpt[value].ads_desc;
			element_desc.dsc_address = (BLOB_PTR *) arg->sdl_arg_array +
				(SLONG) element_desc.dsc_address +
				(array_desc->ads_element_length * subscript);

			/* Is this element within the array bounds? */
			assert((BLOB_PTR *) element_desc.dsc_address >=
				   (BLOB_PTR *) arg->sdl_arg_array);
			assert((BLOB_PTR *) element_desc.dsc_address +
				   element_desc.dsc_length <=
				   (BLOB_PTR *) arg->sdl_arg_array +
				   array_desc->ads_total_length);
			break;

		case op_element:
			count = *next++;
			if (arg->sdl_arg_argument->slice_direction) {
				/* Storing INTO array */

#pragma FB_COMPILER_MESSAGE("Fix. Bad function pointer cast.")

				((void (*)(...)) (*arg->sdl_arg_callback)) (arg->sdl_arg_argument,
															count,
															&element_desc);
			}
			else {
				/* Fetching FROM array */

				if ((BLOB_PTR *) element_desc.dsc_address <
					(BLOB_PTR *) arg->sdl_arg_argument->slice_high_water) {

#pragma FB_COMPILER_MESSAGE("Fix. Bad function pointer cast.")

					((void (*)(...)) (*arg->sdl_arg_callback)) (arg->sdl_arg_argument,
																count,
																&element_desc);
				}
				else {
					slice_desc = &arg->sdl_arg_argument->slice_desc;
					slice_desc->dsc_address +=
						arg->sdl_arg_argument->slice_element_length;
				}
			}
			break;

		case op_exit:
			return TRUE;

		default:
			assert(FALSE);
			return FALSE;
		}
	}
}


static UCHAR *get_range(UCHAR * sdl, RNG arg, SLONG * min, SLONG * max)
{
/**************************************
 *
 *	g e t _ r a n g e
 *
 **************************************
 *
 * Functional description
 *	Analyse a piece of slice description language to get bounds
 *	of array references.
 *
 **************************************/
	SLONG n, variable, value, min1, max1, min2, max2, junk1, junk2;
	UCHAR *p, op;
	SDL_INFO info;

	p = sdl;

	switch (op = *p++) {
	case gds_sdl_do1:
	case gds_sdl_do2:
	case gds_sdl_do3:
		variable = *p++;
		if (op == gds_sdl_do1)
			arg->rng_minima[variable] = 1;
		else {
			if (!(p = get_range(p, arg, &arg->rng_minima[variable], &junk1)))
				return NULL;
		}
		if (!(p = get_range(p, arg, &junk1, &arg->rng_maxima[variable])))
			return NULL;
		if (op == gds_sdl_do3) {
			if (!(p = get_range(p, arg, &junk1, &junk2)))
				return NULL;
		}
		return get_range(p, arg, min, max);

	case gds_sdl_variable:
		variable = *p++;
		*min = arg->rng_minima[variable];
		*max = arg->rng_maxima[variable];
		return p;

	case gds_sdl_tiny_integer:
		value = (SCHAR) * p++;
		*min = *max = value;
		return p;

	case gds_sdl_short_integer:
		value = (SSHORT) (p[0] | (p[1] << 8));
		*min = *max = value;
		return p + 2;

	case gds_sdl_long_integer:
		value =
			(SLONG) (p[0] | (p[1] << 8) | ((SLONG) p[2] << 16) |
					 ((SLONG) p[3] << 24));
		*min = *max = value;
		return p + 4;

	case gds_sdl_add:
	case gds_sdl_subtract:
	case gds_sdl_multiply:
	case gds_sdl_divide:
		if (!(p = get_range(p, arg, &min1, &max1)))
			return NULL;
		if (!(p = get_range(p, arg, &min2, &max2)))
			return NULL;
		switch (op) {
		case gds_sdl_add:
			*min = min1 + min2;
			*max = max1 + max2;
			break;

		case gds_sdl_subtract:
			*min = min1 - max2;
			*max = max1 - min2;
			break;

		case gds_sdl_multiply:
			*min = min1 * min2;
			*max = max1 * max2;
			break;

		case gds_sdl_divide:
			return NULL;
		}
		return p;

	case gds_sdl_scalar:
		p++;
		info = arg->rng_info;
		info->sdl_info_dimensions = *p++;
		for (n = 0; n < info->sdl_info_dimensions; n++)
			if (!
				(p =
				 get_range(p, arg, &info->sdl_info_lower[n],
						   &info->sdl_info_upper[n]))) return NULL;
		return p;

	case gds_sdl_element:
		for (n = *p++; n; --n)
			if (!(p = get_range(p, arg, min, max)))
				return NULL;
		return p;

	default:
		assert(FALSE);
		return NULL;
	}
}


static SSHORT get_word(UCHAR ** ptr)
{
/**************************************
 *
 *	g e t _ w o r d
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	UCHAR *p;
	SSHORT n;

	p = *ptr;
	n = p[0] | (p[1] << 8);
	*ptr += 2;

	return n;
}


static UCHAR *sdl_desc(UCHAR * ptr, DSC * desc)
{
/**************************************
 *
 *	s d l _ d e s c
 *
 **************************************
 *
 * Functional description
 *	Parse BLR descriptor into internal descriptor.
 *	Return updated pointer is successful, otherwise NULL.
 *
 **************************************/
	UCHAR *sdl;

	sdl = ptr;
	desc->dsc_scale = 0;
	desc->dsc_length = 0;
	desc->dsc_sub_type = 0;
	desc->dsc_flags = 0;

	switch (*sdl++) {
	case blr_text2:
		desc->dsc_dtype = dtype_text;
		INTL_ASSIGN_TTYPE(desc, get_word(&sdl));
		break;

	case blr_text:
		desc->dsc_dtype = dtype_text;
		INTL_ASSIGN_TTYPE(desc, ttype_dynamic);
		desc->dsc_flags |= DSC_no_subtype;
		break;

	case blr_cstring2:
		desc->dsc_dtype = dtype_cstring;
		INTL_ASSIGN_TTYPE(desc, get_word(&sdl));
		break;

	case blr_cstring:
		desc->dsc_dtype = dtype_cstring;
		INTL_ASSIGN_TTYPE(desc, ttype_dynamic);
		desc->dsc_flags |= DSC_no_subtype;
		break;

	case blr_varying2:
		desc->dsc_dtype = dtype_cstring;
		INTL_ASSIGN_TTYPE(desc, get_word(&sdl));
		desc->dsc_length = sizeof(USHORT);
		break;

	case blr_varying:
		desc->dsc_dtype = dtype_cstring;
		INTL_ASSIGN_TTYPE(desc, ttype_dynamic);
		desc->dsc_length = sizeof(USHORT);
		desc->dsc_flags |= DSC_no_subtype;
		break;

	case blr_short:
		desc->dsc_dtype = dtype_short;
		desc->dsc_length = sizeof(SSHORT);
		break;

	case blr_long:
		desc->dsc_dtype = dtype_long;
		desc->dsc_length = sizeof(SLONG);
		break;

	case blr_int64:
		desc->dsc_dtype = dtype_int64;
		desc->dsc_length = sizeof(SINT64);
		break;

	case blr_quad:
		desc->dsc_dtype = dtype_quad;
		desc->dsc_length = sizeof(GDS__QUAD);
		break;

	case blr_float:
		desc->dsc_dtype = dtype_real;
		desc->dsc_length = sizeof(float);
		break;

	case blr_double:
#ifndef VMS
	case blr_d_float:
#endif
		desc->dsc_dtype = dtype_double;
		desc->dsc_length = sizeof(double);
		break;

#ifdef VMS
	case blr_d_float:
		desc->dsc_dtype = dtype_d_float;
		desc->dsc_length = sizeof(double);
		break;
#endif

	case blr_timestamp:
		desc->dsc_dtype = dtype_timestamp;
		desc->dsc_length = sizeof(GDS__QUAD);
		break;

	case blr_sql_date:
		desc->dsc_dtype = dtype_sql_date;
		desc->dsc_length = sizeof(SLONG);
		break;

	case blr_sql_time:
		desc->dsc_dtype = dtype_sql_time;
		desc->dsc_length = sizeof(ULONG);
		break;

	default:
		assert(FALSE);
		return NULL;
	}

	switch (desc->dsc_dtype) {
	case dtype_short:
	case dtype_long:
	case dtype_quad:
	case dtype_int64:
		desc->dsc_scale = *((SCHAR *) sdl);
		++sdl;
		break;

	case dtype_text:
	case dtype_cstring:
	case dtype_varying:
		desc->dsc_length += get_word(&sdl);
		break;

	default:
		break;
	}

	return sdl;
}


static IPTR *stuff(IPTR value, SDL_ARG arg)
{
/**************************************
 *
 *	s t u f f
 *
 **************************************
 *
 * Functional description
 *	Stuff a longword into the compiled code space.
 *
 **************************************/

	if (!arg)
		return (IPTR *) TRUE;

	if (arg->sdl_arg_next >= arg->sdl_arg_end)
		error(arg->sdl_arg_status_vector, gds_virmemexh, gds_arg_end);
	/* unable to allocate memory from operating system */

	*(arg->sdl_arg_next)++ = value;

	return arg->sdl_arg_next - 1;
}
