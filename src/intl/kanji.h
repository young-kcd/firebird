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
#ifndef _JRD_KANJI_H_
#define _JRD_KANJI_H_

#define SJIS1(uc) \
	((0x80 <= (UCHAR) ((uc) & 0xff) && (UCHAR) ((uc) & 0xff) <= 0x9f) || \
	 (0xe0 <= (UCHAR) ((uc) & 0xff) && (UCHAR) ((uc) & 0xff) <= 0xef))
#define SJIS2(uc)     	((0x40 <= (UCHAR) ((uc) & 0xff) && (UCHAR) ((uc) & 0xff) <= 0xfc))
#define SJIS_SINGLE(uc)	( 0xa0 <= (UCHAR) ((uc) & 0xff) && (UCHAR) ((uc) & 0xff) <= 0xdf )

#define EUC1(c)		(c & 0x80)
#define EUC2(c)		(c & 0x80)

#define KANJISHORT(s)	(s & 0xff80)
#define KANJI_MASK	0x80

#ifdef JPN_EUC
#define KANJI1(c)	EUC1(c)
#define KANJI2(c)	EUC2(c)
#else
#ifdef JPN_SJIS
#define KANJI1(c)	SJIS1(c)
#define KANJI2(c)	SJIS2(c)
#endif /* JPN_SJIS */
#endif /* JPN_EUC */

#ifdef JPN_EUC
#define JPN1_CHAR(c)	EUC1(c)
#else
#ifdef JPN_SJIS
#define JPN1_CHAR(c)	(SJIS1(c) || SJIS_SINGLE(c))
#endif /* JPN_SJIS */
#endif /* JPN_EUC  */

#ifdef JPN_EUC
#define	KANJI_check		KANJI_check_euc
#else
#ifdef JPN_SJIS
#define KANJI_check		KANJI_check_sjis
#endif /* JPN_SJIS */
#endif /*JPN_EUC */

#ifdef JPN_EUC
#define KANJI_byte2short	KANJI_euc_byte2short
#else
#ifdef JPN_SJIS
#define KANJI_byte2short	KANJI_sjis_byte2short
#endif /* JPN_SJIS */
#endif /*JPN_EUC */

extern USHORT KANJI_check_euc(UCHAR *, USHORT);
extern USHORT KANJI_check_sjis(UCHAR *, USHORT);
extern USHORT KANJI_euc2sjis(UCHAR *, USHORT, UCHAR *, USHORT, USHORT *);
extern USHORT KANJI_euc_byte2short(UCHAR *, USHORT *, USHORT);
extern USHORT KANJI_euc_len(UCHAR *, USHORT, USHORT *);
extern USHORT KANJI_sjis2euc(UCHAR *, USHORT, UCHAR *, USHORT, USHORT *);
extern USHORT KANJI_sjis_byte2short(UCHAR *, USHORT *, USHORT);
extern USHORT KANJI_sjis2euc5(UCHAR *, USHORT, UCHAR *, USHORT, USHORT *,
							  USHORT *, USHORT *);
extern USHORT KANJI_sjis_len(UCHAR *, USHORT, USHORT *);

/*extern USHORT KANJI_check_euc();
extern USHORT KANJI_check_sjis();
extern USHORT KANJI_euc2sjis();
extern USHORT KANJI_euc_byte2short();
extern USHORT KANJI_euc_len();
extern USHORT KANJI_sjis2euc();
extern USHORT KANJI_sjis2euc5();
extern USHORT KANJI_sjis_byte2short();
extern USHORT KANJI_sjis_len();*/

#endif /* _JRD_KANJI_H_ */
