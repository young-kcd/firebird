/*
 *	PROGRAM:	InterBase International support
 *	MODULE:		lc_narrow.c
 *	DESCRIPTION:	Common base for Narrow language drivers
 *			(full International collation)
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

USHORT LC_NARROW_key_length(TEXTTYPE obj, USHORT inLen);
USHORT LC_NARROW_string_to_key(TEXTTYPE obj, USHORT iInLen, const BYTE *pInChar, USHORT iOutLen, BYTE *pOutChar,
							   USHORT partial);
SSHORT LC_NARROW_compare(TEXTTYPE obj, USHORT l1, BYTE *s1, USHORT l2, BYTE *s2);

