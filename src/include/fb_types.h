/*
 *
 *  The contents of this file are subject to the Mozilla Public
 *  License Version 1.1 (the "License"); you may not use this file
 *  except in compliance with the License. You may obtain a copy of
 *  the License at http://www.mozilla.org/MPL/
 *  Alternatively, the contents of this file may be used under the
 *  terms of the GNU General Public License Version 2 or later (the
 *  "GPL"), in which case the provisions of the GPL are applicable
 *  instead of those above. You may obtain a copy of the Licence at
 *  http://www.gnu.org/copyleft/gpl.html
 *
 *  This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    Relevant for more details.
 *
 *    This file was created by members of the firebird development team.
 *    All individual contributions remain the Copyright (C) of those
 *    individuals.  Contributors to this file are either listed here or
 *    can be obtained from a CVS history command.
 *
 *   All rights reserved.
 *
 *   Contributor(s):
 *       Mike Nordel <tamlin@algonet.se>
 *       Mark O'Donohue <mark.odonohue@ludwig.edu.au>
 *
 *
 *  $Id: fb_types.h,v 1.10 2002-12-02 08:25:16 eku Exp $
 *
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "OS/2" port
 *
 */


#ifndef INCLUDE_FB_TYPES_H
#define INCLUDE_FB_TYPES_H


#ifndef CONST
#    define CONST           const
#endif


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#  define  GDS_EXPORT	__stdcall
#else
#  define  GDS_EXPORT
#endif

#ifndef GDS_FAR
#define GDS_FAR
#endif


#if SIZEOF_LONG == 8
	/* EKU: Firebird requires (S)LONG to be 32 bit */
#   define LONG_DEFINED
    typedef int SLONG;
    typedef unsigned int ULONG;

#   define SQUAD_DEFINED
    typedef long SQUAD;
    typedef unsigned long UQUAD;
#   define NATIVE_QUAD
#endif /* SIZEOF_LONG == 8 */



/* Basic data types */


#if 1
/* TMN: TODO It seems SCHAR is used just about *everywhere* where a plain
 * "char" is really intended. This currently forces us to this bad definition.
 */
typedef char SCHAR;
#else
typedef signed char SCHAR;
#endif


typedef unsigned char UCHAR;
typedef short SSHORT;
typedef unsigned short USHORT;


#ifndef LONG_DEFINED			/* 32 bit */
typedef long SLONG;
typedef unsigned long ULONG;
#else
#undef LONG_DEFINED
#endif


#ifndef SQUAD_DEFINED			/* 64 bit */
typedef struct {
	SLONG high;
	ULONG low;
} SQUAD;
#endif


#ifndef DEFINED_GDS_QUAD
#define DEFINED_GDS_QUAD
struct GDS_QUAD_t {
	SLONG gds_quad_high;
	ULONG gds_quad_low;
};

typedef struct GDS_QUAD_t GDS_QUAD;

#endif /* DEFINED_GDS_QUAD */

//
// TMN: some misc data types from all over the place
//
struct vary
{
	SSHORT vary_length;
	char   vary_string[1];
};
// TMN: Currently we can't do this, since remote uses a different
// definition of VARY than the rest of the code! :-<
//typedef vary* VARY;

struct lstring
{
	ULONG	lstr_length;
	ULONG	lstr_allocated;
	UCHAR*	lstr_address;
};
typedef struct lstring LSTRING;


typedef unsigned char BOOLEAN;
typedef char TEXT;				/* To be expunged over time */
typedef unsigned char STEXT;	/* Signed text - very rare */
typedef unsigned char UTEXT;	/* Unsigned text - common */
typedef unsigned char BYTE;		/* Unsigned byte - common */
typedef char SBYTE;				/* Signed byte - rare usage */
typedef long STATUS;
typedef long IPTR;
typedef unsigned long U_IPTR;
typedef void (*FPTR_VOID) ();
typedef void (*FPTR_VOID_PTR) (void *);
typedef int (*FPTR_INT) ();
typedef int (*FPTR_INT_VOID_PTR) (void *);
typedef ULONG RCRD_OFFSET;
typedef USHORT FLD_LENGTH;


/* Number of elements in an arry */
#define FB_NELEM(x)	((int)(sizeof(x) / sizeof(x[0])))

#endif /* INCLUDE_FB_TYPES_H */
