/*
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
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete ports:
 *                          - DELTA and IMP
 *
 * 2002.10.27 Sean Leyne - Code Cleanup, removed obsolete "Ultrix/MIPS" port
 * 2002.10.27 Sean Leyne - Completed removal of "NeXT" port
 *
 * 2002.10.28 Sean Leyne - Code Cleanup, removed obsolete "DG" port
 *
 */
#ifndef _INTL_LANGDRV_H_
#define _INTL_LANGDRV_H_

#define	IB_LANGDRV_VERSION	1

/* Unique ID's for character sets */
#define	IB_ASCII7	0
#define	IB_BINARY	0
#define	IB_LATIN1	1
#define	IB_LATIN2	2
#define	IB_LATIN3	3
#define	IB_LATIN4	4
#define	IB_LATIN5	5
#define	IB_LATIN6	6
#define	IB_LATIN7	7
#define	IB_LATIN8	8
#define	IB_LATIN9	9
#define	IB_SJIS		10
#define	IB_EUC		11

/* Unique ID's for Locales */
#define	IB_USA		0
#define	IB_GERMANY	1
#define	IB_FRANCE	2
#define	IB_JAPAN	3
#define	IB_UK		4
#define	IB_CANADA_FRENCH 5
#define	IB_CANADA_ENGLISH 6

#endif /* _INTL_LANGDRV_H_ */
