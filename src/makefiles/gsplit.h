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

/*
 **************************************************************************
 *
 *	PROGRAM:		JRD file split utility program
 *	MODULE:			gsplit.h
 *	DESCRIPTION:	file split utility program main header file
 *
 *	copyright (c) 1996 by Borland International
 *
 **************************************************************************
 */

#include	"source/jrd/common.h"

#define	BLANK					' '
#define	BURP_ALLOC_ZERO(size)	MISC_alloc_burp((ULONG) size)
#define	FILE_IS_FULL			-9
#define	FILE_NM_ARR_LEN			20
#define	GBAK_STDIN_DESC			0	/* standart input  file descriptor */
#define	GBAK_STDOUT_DESC		1	/* standart output file descriptor */
#define	GSPLIT_HDR_REC_NM		"InterBase/Gsplit"

#define	IO_BUFFER_SIZE			(16 * K_BYTES)

#define	K_BYTES					1024
#define	M_BYTES					(K_BYTES * K_BYTES)
#define	G_BYTES					(K_BYTES * M_BYTES)
#define	MIN_FILE_SIZE			M_BYTES
#define	NEW_LINE				'\n'
#define	NULL_B_FIL				((B_FIL) NULL_PTR)
#define	TERMINAL				'\0'

typedef	int	FILE_DESC;
