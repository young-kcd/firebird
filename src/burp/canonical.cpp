/*
 *	PROGRAM:	JRD Backup and Restore Program
 *	MODULE:		canonical.c
 *	DESCRIPTION:	
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
 * 2002.10.27 Sean Leyne - Code Cleanup, removed obsolete "Ultrix/MIPS" port
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 *
 */
/*
$Id: canonical.cpp,v 1.26 2003-09-18 21:56:26 brodsom Exp $
*/

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <string.h>
#include "../burp/burp.h"
#include "../jrd/align.h"
#include "../jrd/sdl.h"
#include "../burp/canon_proto.h"
#include "../jrd/sdl_proto.h"
#include "../remote/xdr_proto.h"
#include "../jrd/gdsassert.h"
#include "../include/fb_types.h"

// TMN: Currently we can't include remote/remote.h because we'd get
// conflicting blk_t definitions (we are gonna fix this, in due time).
// Because this file needs the remote definition of VARY, which
// coincidentally is different from _all_ other modules in the code,
// we have to make the type definition here.

typedef vary* VARY;


static XDR_INT burp_destroy(XDR*);
static bool_t burp_getbytes(XDR*, SCHAR *, u_int);
static bool_t burp_getlong(XDR*, SLONG *);
static u_int burp_getpostn(XDR*);
static caddr_t burp_inline(XDR*, u_int);
static bool_t burp_putbytes(XDR*, SCHAR *, u_int);
static bool_t burp_putlong(XDR*, SLONG *);
static bool_t burp_setpostn(XDR*, u_int);
static bool_t expand_buffer(XDR*);
static bool_t xdr_datum(XDR*, DSC*, UCHAR*);
static bool_t xdr_quad(XDR*, SLONG*);
static int xdr_init(XDR*, LSTRING*, enum xdr_op);
static bool_t xdr_slice(XDR*, LSTRING*, USHORT, UCHAR*);

static xdr_t::xdr_ops burp_ops =
{
	burp_getlong,
	burp_putlong,
	burp_getbytes,
	burp_putbytes,
	burp_getpostn,
	burp_setpostn,
	burp_inline,
	burp_destroy
};

const int INCREMENT = 1024;


ULONG CAN_encode_decode(BURP_REL relation,
						LSTRING * buffer,
						UCHAR * data,
						bool_t direction)
{
/**************************************
 *
 *	C A N _ e n c o d e _ d e c o d e
 *
 **************************************
 *
 * Functional description
 *	encode and decode canonical backup.
 *
 **************************************/
	XDR xdr, *xdrs;
	BURP_FLD field;
	FLD_LENGTH length;
	SSHORT dtype, n;
	UCHAR *p;
	RCRD_OFFSET offset;
	USHORT array_fld;

	xdrs = &xdr;

	xdr_init(xdrs, buffer, (direction) ? XDR_ENCODE : XDR_DECODE);

	offset = 0;
	for (field = relation->rel_fields; field; field = field->fld_next)
	{
		if (field->fld_flags & FLD_computed)
			continue;
		p = data + field->fld_offset;
		if (array_fld = field->fld_flags & FLD_array)
			length = 8;
		else
			length = field->fld_length;
		if (field->fld_offset >= offset)
			offset = field->fld_offset + length;
		if (field->fld_type == blr_varying && !array_fld)
			offset += sizeof(SSHORT);
		if (field->fld_type == blr_blob || array_fld)
			dtype = dtype_blob;
		else
			dtype = (SSHORT) gds_cvt_blr_dtype[field->fld_type];
		switch (dtype)
		{
		case dtype_text:
			if (!xdr_opaque(xdrs, reinterpret_cast<char*>(p), length))
			{
				return FALSE;
			}
			break;

		case dtype_varying:
			{
				VARY pVary = reinterpret_cast<VARY>(p);
				if (!xdr_short(	xdrs,
								reinterpret_cast<SSHORT*>(&pVary->vary_length)))
				{
					return FALSE;
				}
				if (!xdr_opaque(xdrs,
								reinterpret_cast<SCHAR*>(pVary->vary_string),
								pVary->vary_length))
				{
				  return FALSE;
				}
			}
			break;

		case dtype_cstring:
			if (xdrs->x_op == XDR_ENCODE)
				n = (strlen(reinterpret_cast<const char*>(p)));
			if (!xdr_short(xdrs, &n))
				return FALSE;
			if (!xdr_opaque(xdrs, reinterpret_cast<SCHAR*>(p), n))
				  return FALSE;
			if (xdrs->x_op == XDR_DECODE)
				p[n] = 0;
			break;

		case dtype_short:
			if (!xdr_short(xdrs, (SSHORT *) p))
				return FALSE;
			break;

		case dtype_long:
		case dtype_sql_time:
		case dtype_sql_date:
			if (!xdr_long(xdrs, (SLONG *) p))
				return FALSE;
			break;

		case dtype_real:
			if (!xdr_float(xdrs, (float *) p))
				return FALSE;
			break;

		case dtype_double:
			if (!xdr_double(xdrs, (double *) p))
				return FALSE;
			break;

		case dtype_timestamp:
			if (!xdr_long(xdrs, &((SLONG *) p)[0]))
				return FALSE;
			if (!xdr_long(xdrs, &((SLONG *) p)[1]))
				return FALSE;
			break;

		case dtype_quad:
		case dtype_blob:
			if (!xdr_quad(xdrs, (SLONG *) p))
				return FALSE;
			break;

		case dtype_int64:
			if (!xdr_hyper(xdrs, (SINT64 *) p))
				return FALSE;
			break;

		default:
			assert(FALSE);
			return FALSE;
		}
	}

/* Next, get null flags */

	for (field = relation->rel_fields; field; field = field->fld_next)
	{
		if (field->fld_flags & FLD_computed)
			continue;
		offset = FB_ALIGN(offset, sizeof(SSHORT));
		p = data + offset;
		if (!xdr_short(xdrs, (SSHORT *) p))
			return FALSE;
		offset += sizeof(SSHORT);
	}
	return (xdrs->x_private - xdrs->x_base);
}


ULONG CAN_slice(LSTRING * buffer,
				LSTRING * slice,
				bool_t direction,
				USHORT sdl_length,
				UCHAR * sdl)
{
/**************************************
 *
 *	C A N _ s l i c e
 *
 **************************************
 *
 * Functional description
 *	encode and decode canonical backup.
 *
 **************************************/
	XDR xdr, *xdrs;

	xdrs = &xdr;

	xdr_init(xdrs, buffer, (direction) ? XDR_ENCODE : XDR_DECODE);

	xdr_slice(xdrs, slice, sdl_length, sdl);
	return (xdrs->x_private - xdrs->x_base);
}


static XDR_INT burp_destroy( XDR * xdrs)
{
/**************************************
 *
 *	b u r p _ d e s t r o y
 *
 **************************************
 *
 * Functional description
 *	Destroy a stream.  A no-op.
 *
 **************************************/
	return 0;
}


static bool_t burp_getbytes(XDR* xdrs, SCHAR* buff, u_int bytecount)
{
/**************************************
 *
 *	b u r p _ g e t b y t e s
 *
 **************************************
 *
 * Functional description
 *	Fetch a bunch of bytes into a memory stream if it fits.
 *
 **************************************/

	if (bytecount && xdrs->x_handy >= (int) bytecount)
	{
		xdrs->x_handy -= bytecount;
		do {
			*buff++ = *xdrs->x_private++;
		} while (--bytecount);
		return TRUE;
	}

	while (bytecount)
	{
		if (!xdrs->x_handy && !expand_buffer(xdrs))
			return FALSE;
		*buff++ = *xdrs->x_private++;
		--xdrs->x_handy;
		--bytecount;
	}

	return TRUE;
}


static bool_t burp_getlong( XDR * xdrs, SLONG * lp)
{
/**************************************
 *
 *	b u r p _ g e t l o n g
 *
 **************************************
 *
 * Functional description
 *	Fetch a longword into a memory stream if it fits.
 *
 **************************************/

	SLONG l;

	if (!(*xdrs->x_ops->x_getbytes) (xdrs, reinterpret_cast<char*>(&l), 4))
		return FALSE;

	*lp = ntohl(l);

	return TRUE;
}


static u_int burp_getpostn( XDR * xdrs)
{
/**************************************
 *
 *	b u r p _ g e t p o s t n
 *
 **************************************
 *
 * Functional description
 *	Get the current position (which is also current length) from stream.
 *
 **************************************/

	return xdrs->x_private - xdrs->x_base;
}


static caddr_t burp_inline( XDR * xdrs, u_int bytecount)
{
/**************************************
 *
 *	b u r p _  i n l i n e
 *
 **************************************
 *
 * Functional description
 *	Return a pointer to somewhere in the buffer.
 *
 **************************************/

	if (bytecount > (u_int) xdrs->x_handy)
		return FALSE;

	return xdrs->x_base + bytecount;
}


static bool_t burp_putbytes(XDR* xdrs, SCHAR* buff, u_int bytecount)
{
/**************************************
 *
 *	b u r p _ p u t b y t e s
 *
 **************************************
 *
 * Functional description
 *	Fetch a bunch of bytes into a memory stream if it fits.
 *
 **************************************/

	if (bytecount && xdrs->x_handy >= (int) bytecount)
	{
		xdrs->x_handy -= bytecount;
		do {
			*xdrs->x_private++ = *buff++;
		} while (--bytecount);
		return TRUE;
	}

	while (bytecount)
	{
		if (xdrs->x_handy <= 0 && !expand_buffer(xdrs))
		{
			return FALSE;
		}
		--xdrs->x_handy;
		*xdrs->x_private++ = *buff++;
		--bytecount;
	}

	return TRUE;
}


static bool_t burp_putlong( XDR * xdrs, SLONG * lp)
{
/**************************************
 *
 *	b u r p _ p u t l o n g
 *
 **************************************
 *
 * Functional description
 *	Fetch a longword into a memory stream if it fits.
 *
 **************************************/
	SLONG l;

	l = htonl(*lp);
	return (*xdrs->x_ops->x_putbytes) (xdrs,
									   reinterpret_cast<char*>(AOF32L(l)),
									   4);
}


static bool_t burp_setpostn( XDR * xdrs, u_int bytecount)
{
/**************************************
 *
 *	b u r p _ s e t p o s t n
 *
 **************************************
 *
 * Functional description
 *	Set the current position (which is also current length) from stream.
 *
 **************************************/

	if (bytecount > (u_int) xdrs->x_handy)
		return FALSE;

	xdrs->x_private = xdrs->x_base + bytecount;

	return TRUE;
}


static bool_t expand_buffer( XDR * xdrs)
{
/**************************************
 *
 *	e x p a n d _ b u f f e r
 *
 **************************************
 *
 * Functional description
 *	Allocate a new, larger buffer, copy
 *	everything we've got, and release the 
 *	old one.
 *
 **************************************/

	caddr_t p, q;
	SSHORT length;
	LSTRING *buffer;

	buffer = (LSTRING *) xdrs->x_public;
	length = (xdrs->x_private - xdrs->x_base) + xdrs->x_handy + INCREMENT;
	buffer->lstr_allocated = buffer->lstr_length = length;

	caddr_t new_ = (caddr_t) BURP_alloc(length);

	for (p = new_, q = xdrs->x_base; q < xdrs->x_private; *p++ = *q++)
		;

	BURP_free(xdrs->x_base);

	xdrs->x_base = new_;
	xdrs->x_private = p;
	xdrs->x_handy += INCREMENT;

	buffer->lstr_address = (UCHAR *) new_;

	return TRUE;
}



static bool_t xdr_datum( XDR * xdrs, DSC * desc, UCHAR * buffer)
{
/**************************************
 *
 *	x d r _ d a t u m
 *
 **************************************
 *
 * Functional description
 *	Handle a data item by relative descriptor and buffer.
 *
 **************************************/
	UCHAR *p;
	SSHORT n;

	p = buffer + (int) desc->dsc_address;

	switch (desc->dsc_dtype)
	{
	case dtype_text:
		if (!xdr_opaque(xdrs, reinterpret_cast<SCHAR*>(p), desc->dsc_length))
		{
			return FALSE;
		}
		break;

	case dtype_varying:
		{
			VARY pVary = reinterpret_cast<VARY>(p);
			if (!xdr_short(xdrs,
							reinterpret_cast<short*>(&pVary->vary_length)))
			{
				return FALSE;
			}
			if (!xdr_opaque(xdrs,
							reinterpret_cast<SCHAR*>(pVary->vary_string),
							MIN(desc->dsc_length - 2,
							pVary->vary_length)))
			{
				return FALSE;
			}
		}
		break;

	case dtype_cstring:
		if (xdrs->x_op == XDR_ENCODE)
			n = MIN(strlen(reinterpret_cast<const char*>(p)),
					(size_t) (desc->dsc_length - 1));
		if (!xdr_short(xdrs, &n))
			return FALSE;
		if (!xdr_opaque(xdrs, reinterpret_cast<char*>(p), n))
			  return FALSE;
		if (xdrs->x_op == XDR_DECODE)
			p[n] = 0;
		break;

	case dtype_short:
		if (!xdr_short(xdrs, (SSHORT *) p))
			return FALSE;
		break;

	case dtype_sql_date:
	case dtype_sql_time:
	case dtype_long:
		if (!xdr_long(xdrs, (SLONG *) p))
			return FALSE;
		break;

	case dtype_real:
		if (!xdr_float(xdrs, (float *) p))
			return FALSE;
		break;

	case dtype_double:
		if (!xdr_double(xdrs, (double *) p))
			return FALSE;
		break;

	case dtype_timestamp:
		if (!xdr_long(xdrs, &((SLONG *) p)[0]))
			return FALSE;
		if (!xdr_long(xdrs, &((SLONG *) p)[1]))
			return FALSE;
		break;

	case dtype_quad:
	case dtype_blob:
		if (!xdr_quad(xdrs, (SLONG *) p))
			return FALSE;
		break;

	case dtype_int64:
		if (!xdr_hyper(xdrs, (SINT64 *) p))
			return FALSE;
		break;

	default:
		assert(FALSE);
		return FALSE;
	}

	return TRUE;
}

static bool_t xdr_quad(XDR* xdrs, SLONG* ip)
{
/**************************************
 *
 *	x d r _ q u a d
 *
 **************************************
 *
 * Functional description
 *	Map from external to internal representation (or vice versa).
 *
 **************************************/

	switch (xdrs->x_op)
	{
	case XDR_ENCODE:
		if ((*xdrs->x_ops->x_putlong) (xdrs, &ip[0]) &&
			(*xdrs->x_ops->x_putlong) (xdrs, &ip[1]))
			return TRUE;
		return FALSE;

	case XDR_DECODE:
		if (!(*xdrs->x_ops->x_getlong) (xdrs, &ip[0]))
			return FALSE;
		return (*xdrs->x_ops->x_getlong) (xdrs, &ip[1]);

	case XDR_FREE:
		return TRUE;

	default:
		assert(FALSE);
		return FALSE;
	}
}



static int xdr_init( XDR * xdrs, LSTRING * buffer, enum xdr_op x_op)
{
/**************************************
 *
 *	x d r _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize an XDR stream for Apollo mailboxes.
 *
 **************************************/

	xdrs->x_public = (caddr_t) buffer;
	xdrs->x_base = xdrs->x_private = (caddr_t) buffer->lstr_address;
	xdrs->x_handy = buffer->lstr_length;
	xdrs->x_ops = &burp_ops;
	xdrs->x_op = x_op;

	return TRUE;
}



static bool_t xdr_slice(XDR* xdrs,
						LSTRING* slice,
						USHORT sdl_length,
						UCHAR* sdl)
{
/**************************************
 *
 *	x d r _ s l i c e
 *
 **************************************
 *
 * Functional description
 *	Move a slice of an array under
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;
	ULONG n;
	UCHAR *p, *end;
	DSC *desc;
	sdl_info info;

	if (!xdr_long(xdrs, reinterpret_cast<long*>(&slice->lstr_length)))
		  return FALSE;

/* Handle operation specific stuff, particularly memory allocation/deallocation */

	switch (xdrs->x_op)
	{
	case XDR_ENCODE:
		break;

	case XDR_DECODE:
		if (!slice->lstr_length)
			return TRUE;
		if (slice->lstr_length > slice->lstr_allocated &&
			slice->lstr_allocated)
		{
			BURP_free(slice->lstr_address);
			slice->lstr_address = NULL;
		}
		if (!slice->lstr_address)
		{
			slice->lstr_address = BURP_alloc((SLONG) slice->lstr_length);
			if (!slice->lstr_address) {
				return FALSE;
			}
			slice->lstr_allocated = slice->lstr_length;
		}
		break;

	case XDR_FREE:
		if (slice->lstr_allocated)
			BURP_free(slice->lstr_address);
		slice->lstr_address = NULL;
		slice->lstr_allocated = 0;
		return TRUE;

	default:
		assert(FALSE);
		return FALSE;
	}

/* Get descriptor of array element */

	if (SDL_info(status_vector, sdl, &info, 0))
		return FALSE;

	desc = &info.sdl_info_element;
	n = slice->lstr_length / desc->dsc_length;
	p = slice->lstr_address;

	for (end = p + n * desc->dsc_length; p < end; p += desc->dsc_length) {
		if (!xdr_datum(xdrs, desc, p)) {
			return FALSE;
		}
	}

	return TRUE;
}

