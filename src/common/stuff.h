/*
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
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Relevant for more details.
 *
 *  This file was created by members of the firebird development team.
 *  All individual contributions remain the Copyright (C) of those
 *  individuals.  Contributors to this file are either listed here or
 *  can be obtained from a CVS history command.
 *
 *  All rights reserved.
 *  Blas Rodriguez Somoza created this file on 24-May-2004.
 *
 *  Contributor(s):
 *
 */

#ifndef COMMON_STUFF_H
#define COMMON_STUFF_H

inline void add_byte(UCHAR* &blr, const int byte)
{
	*blr++ = (SCHAR) (byte);
}

inline void add_word(UCHAR* &blr, const int word)
{
	add_byte(blr, word);
	add_byte(blr, (word) >> 8);
}

inline void add_long(UCHAR* &blr, const long lg)
{
	add_word(blr, lg);
	add_word(blr, (lg) >> 16);
}

inline void add_int64(UCHAR* &blr, const SINT64 i64)
{
	add_long(blr, i64);
	add_long(blr, (i64) >> 32);
}

inline void add_string(UCHAR* &blr, const TEXT* string)
{
	add_byte(blr, strlen (string));
	while (*string)
		add_byte(blr, *string++);
}

#endif // COMMON_STUFF_H
