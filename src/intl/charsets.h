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
 */
#ifndef INTL_CHARSETS_H
#define INTL_CHARSETS_H

#ifdef __cplusplus
extern "C" {
#endif

#define   CS_NONE               0	/* No Character Set */
#define   CS_BINARY             1	/* BINARY BYTES     */
#define   CS_ASCII              2	/* ASCII            */
#define   CS_UNICODE_FSS        3	/* UNICODE in FSS format */
#define	  CS_reserved4          4	/* reserved for future internal use */

#define   CS_SJIS               5	/* SJIS             */
#define   CS_EUCJ               6	/* EUC-J            */

#define   CS_JIS_0208		7	/* JIS 0208; 1990   */
#define   CS_UNICODE_110        8	/* UNICODE v 1.10   */

#define   CS_DOS_437            10	/* DOS CP 437       */
#define   CS_DOS_850            11	/* DOS CP 850       */
#define   CS_DOS_865            12	/* DOS CP 865       */
#define   CS_DOS_860            13	/* DOS CP 860       */
#define   CS_DOS_863            14	/* DOS CP 863       */
#define   CS_NeXTSTEP           19	/* NeXTSTEP OS native charset */

#define   CS_LATIN1             21	/* ISO-8859.1       */
#define   CS_LATIN2             22	/* ISO-8859.2       */
#define   CS_LATIN3             23	/* ISO-8859.3       */
#define   CS_LATIN4             34	/* ISO-8859.4       */
#define   CS_LATIN5             35	/* ISO-8859.5       */
#define   CS_LATIN6             36	/* ISO-8859.6       */
#define   CS_LATIN7             37	/* ISO-8859.7       */
#define   CS_LATIN8             38	/* ISO-8859.8       */
#define   CS_LATIN9             39	/* ISO-8859.9       */

#define   CS_ANSI               40	/* ANSI             */
#define   CS_KSC5601            44	/* KOREAN STANDARD 5601 */

#define   CS_DOS_852		45	/* DOS CP 852   */
#define   CS_DOS_857		46	/* DOS CP 857   */
#define   CS_DOS_861		47	/* DOS CP 861   */

#define   CS_CYRL		50
#define   CS_WIN1250		51	/* Windows cp 1250  */
#define   CS_WIN1251		52	/* Windows cp 1251  */
#define   CS_WIN1252		53	/* Windows cp 1252  */
#define   CS_WIN1253		54	/* Windows cp 1253  */
#define   CS_WIN1254		55	/* Windows cp 1254  */

#define	  CS_BIG5		56		/* Big Five unicode cs */
#define	  CS_GB2312		57		/* GB 2312-80 cs */

#define   CS_dynamic           127	/* Pseudo number for runtime charset */

#define   CS_MAX_CS_NUMBER	58

/* Alias name */

#define   CS_UNICODE		CS_UNICODE_110
#define   CS_UNICODE101	        CS_UNICODE_110	/* temporary rename */
#define   CS_UNICODE110	        CS_UNICODE_110	/* temporary rename */
#define   CS_NEXT		CS_NeXTSTEP	/* NeXT charset   */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* INTL_CHARSETS_H */
