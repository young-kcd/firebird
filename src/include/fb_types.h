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
 *  $Id: fb_types.h,v 1.2 2001-07-29 17:19:15 skywalker Exp $
 *
 */


#ifndef INCLUDE_FB_TYPES_H
#define INCLUDE_FB_TYPES_H


#ifndef CONST
#    define CONST           const
#endif

#ifndef VOLATILE
#    define VOLATILE        volatile
#endif


#if (defined(_MSC_VER) && defined(_WIN32)) || \
    (defined(__BORLANDC__) && (defined(__WIN32__) || defined(__OS2__)))
#  define  GDS_EXPORT	__stdcall
#else
#  define  GDS_EXPORT
#endif

#ifndef GDS_FAR
#define GDS_FAR
#endif


#if (defined(__osf__) && defined(__alpha))  
/* It appears the alpha has a 32 bit int and a 64 bit long. */
#   define LONG_DEFINED
    typedef int SLONG;
    typedef unsigned int ULONG;

#   define SQUAD_DEFINED
    typedef long SQUAD;
    typedef unsigned long UQUAD;
#   define NATIVE_QUAD

#endif /* DEC osf */



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


#define GDS_TRUE	1
#define GDS_FALSE	0

#if !(defined __cplusplus)
#define GDS__TRUE	GDS_TRUE
#define GDS__FALSE	GDS_FALSE
#endif

/* We can remove these three #defines if we change gdsold.h and gen/codes.h */
#define GDS_LONG SLONG
#define GDS_ULONG ULONG

#define GDS_STATUS	long


#endif /* INCLUDE_FB_TYPES_H */
