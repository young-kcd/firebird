/*
 *	PROGRAM:	JRD Backup and Restore Program
 *	MODULE:		mvol_proto.h		
 *	DESCRIPTION:	Prototype Header file for mvol.c
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

#ifndef _BURP_MVOL_PROTO_H_
#define _BURP_MVOL_PROTO_H_

#ifdef __cplusplus
extern "C" {
#endif

extern void	MVOL_fini_read (int *);
extern void	MVOL_fini_write (int *, UCHAR * *, int *);
extern void	MVOL_init (ULONG);
extern void	MVOL_init_read (UCHAR *, UCHAR *, USHORT *, int *, UCHAR * *);
extern void	MVOL_init_write (UCHAR *, UCHAR *, int *, UCHAR * *);
extern BOOLEAN	MVOL_split_hdr_write (void);
extern BOOLEAN	MVOL_split_hdr_read (void);
extern int	MVOL_read (int *, UCHAR * *);
extern UCHAR *	MVOL_read_block (TGBL, UCHAR *, ULONG);
extern void	MVOL_skip_block (TGBL, ULONG);
extern UCHAR	MVOL_write (UCHAR, int *, UCHAR * *);
extern UCHAR *	MVOL_write_block (TGBL, UCHAR *, ULONG);
#if defined WIN_NT && defined _WINNT_
/* the _WINNT_ symbol is defined if winnt.h has been included */
/* it contains the definition of a HANDLE */
/* those files that don't include winnt.h don't need MVOL_open either */
extern HANDLE   MVOL_open (TEXT*,DWORD,DWORD);
#endif

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* _BURP_MVOL_PROTO_H_ */
