/*
 *	PROGRAM:	InterBase International support
 *	MODULE:		lc_big5.c
 *	DESCRIPTION:	Language Drivers in the BIG5 family.  
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

/* These macros have a duplicate in cv_big5.c */
static USHORT big5_to_upper(TEXTTYPE obj, UCS2_CHAR ch);
static SSHORT big5_str_to_upper(TEXTTYPE obj, USHORT iLen, BYTE *pStr, USHORT iOutLen, BYTE *pOutStr);
static USHORT big5_to_lower(TEXTTYPE obj, UCS2_CHAR ch);
