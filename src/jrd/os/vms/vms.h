/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		vms.h
 *	DESCRIPTION:	VMS specific physical IO definitions
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

#ifndef JRD_VMS_H
#define JRD_VMS_H

#include rms
#include iodef
#include descrip
#include ssdef

#define DVI$_DEVLOCKNAM		240



/* ACP File Information block */

typedef struct fib$ {
	SLONG fib$l_acctl;
	SSHORT fib$w_fid[3];
	SSHORT fib$w_did[3];
	SLONG fib$l_wcc;
	SSHORT fib$w_nmctl;
	SSHORT fib$w_exctl;
	SLONG fib$l_exsz;
	SLONG fib$l_exvbn;
} FIB$;

#define FIB$M_EXTEND	128
#define FIB$M_ALDEF	8
#define FIB$M_ALCONB	2
#define FIB$M_WRITETHRU	524288
#define FIB$M_FINDFID	2048

typedef struct atr$ {
	SSHORT atr$w_size;
	SSHORT atr$w_type;
	SCHAR *atr$l_addr;
	SLONG atr$l_zero;
} ATR$;

#define ATR$S_RECATTR	32
#define ATR$C_RECATTR	4

typedef struct fab$ {
	SCHAR fat$b_rtype;
	SCHAR fat$b_rattrib;
	SSHORT fat$w_rsize;
	USHORT fat$w_hiblk[2];
	USHORT fat$w_efblk[2];
	SSHORT fat$w_ffbyte;
	SCHAR fat$b_bktsize;
	SCHAR fat$b_vfcsize;
	SSHORT fat$w_maxrec;
	SSHORT fat$w_defext;
	SSHORT fat$w_gbc;
	SSHORT fat$w_reserved[3];
	SSHORT fat$w_not_used;
	SSHORT fat$w_versions;
} FAT$;

#endif /* JRD_VMS_H */
