/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		memory_routines.h
 *	DESCRIPTION:	Misc memory content routines
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
 * Created by: Dimitry Sibiryakov <aafemt@users.sourceforge.net>
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 */

#ifndef MEMORY_ROUTINES_H
#define MEMORY_ROUTINES_H

/* SD: I didn't find a way to make external functions inline
       except including their bodies into header. FIX ME, please */
/*
extern inline short get_short(const char*);
extern inline long get_long(const char*);
extern inline short gather_short(char* &);
extern inline long gather_long(char* &);
*/

inline SSHORT get_short(const SCHAR* p)
{
/**************************************
 *
 *      g e t _ s h o r t
 *
 **************************************
 *
 * Functional description
 *    gather one short int from two chars
 *
 * Based on BTR_get_quad
 *
 **************************************/

#if defined(i386) || defined(I386) || defined(_M_IX86) || defined(VMS)
	// For IA32 (little-endian) this optimization is a _very_ large speed-up!
	// According to CTO32L definition in common.h this trick work for VAX/VMS
	return *reinterpret_cast<const SSHORT*>(p);
#else
	// Non-IA32

union {
 SSHORT n;
 SCHAR c[2]; } value;

	value.c[0] = p[0];
	value.c[1] = p[1];

	return value.n;

#endif	// endianness
}

inline SLONG get_long(const SCHAR* p)
{
/**************************************
 *
 *      g e t _ l o n g
 *
 **************************************
 *
 * Functional description
 *    gather one long int from four chars
 *
 * Based on BTR_get_quad
 *
 **************************************/

#if defined(i386) || defined(I386) || defined(_M_IX86) || defined(VMS)
	// For IA32 (little-endian) this optimization is a _very_ large speed-up!
	return *reinterpret_cast<const SLONG*>(p);
#else
	// Non-IA32

union {
 SLONG n;
 SCHAR c[4]; } value;

	value.c[0] = p[0];
	value.c[1] = p[1];
	value.c[2] = p[2];
	value.c[3] = p[3];

	return value.n;

#endif	// endianness
}

inline SLONG get_long(const UCHAR* p)
{
/**************************************
 *
 *      g e t _ l o n g (overloaded)
 *
 **************************************
 *
 * Functional description
 *    gather one unsigned long int from
 *    four unsigned chars
 *
 * Based on get_long and CTO32L macro
 *
 **************************************/

#if defined(i386) || defined(I386) || defined(_M_IX86) || defined(VMS)
	// For IA32 (little-endian) this optimization is a _very_ large speed-up!
	return *reinterpret_cast<const SLONG*>(p);
#else
	// Non-IA32 a bit slower implementation but faster than using union
#ifndef WORDS_BIGENDIAN
	return (p[3] << 24) | (p[2] << 16) | (p[1] << 8) | p[0];
#else
	return (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
#endif // WORDS_BIGENDIAN
#endif
}

inline SSHORT gather_short(SCHAR* &p)
{
/**************************************
 *
 *      g a t h e r _ s h o r t
 *
 **************************************
 *
 * Functional description
 *    gather one short int from two chars
 *    and advance the pointer
 *
 * Based on BTR_get_quad
 *
 **************************************/

#if defined(i386) || defined(I386) || defined(_M_IX86) || defined(VMS)
	// For IA32 (little-endian) this optimization is a _very_ large speed-up!
	SSHORT value;
	value = *reinterpret_cast<const SSHORT*>(p);
	p += 2;
	return value;
#else
	// Non-IA32

union {
 SSHORT n;
 SCHAR c[2]; } value;

	value.c[0] = *p++;
	value.c[1] = *p++;

	return value.n;

#endif	// endianness
}

inline SLONG gather_long(SCHAR* &p)
{
/**************************************
 *
 *      g a t h e r _ l o n g
 *
 **************************************
 *
 * Functional description
 *    gather one long int from four chars
 *    and advance the pointer
 *
 * Based on BTR_get_quad
 *
 **************************************/

#if defined(i386) || defined(I386) || defined(_M_IX86) || defined(VMS)
	// For IA32 (little-endian) this optimization is a _very_ large speed-up!
	SLONG value;
	value = *reinterpret_cast<const SLONG*>(p);
	p += 4;
	return value;
#else
	// Non-IA32

union {
 SLONG n;
 SCHAR c[4]; } value;

	value.c[0] = *p++;
	value.c[1] = *p++;
	value.c[2] = *p++;
	value.c[3] = *p++;

	return value.n;

#endif	// endianness
}

#endif // MEMORY_ROUTINES_H
