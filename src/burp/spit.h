/*
 **************************************************************************
 *
 *	PROGRAM:		JRD file split utility program
 *	MODULE:			spit.h
 *	DESCRIPTION:	file split utility program main header file
 *
 *
 **************************************************************************
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

#include "../jrd/common.h"

#define	BLANK					' '
#define	BURP_ALLOC_ZERO(size)	MISC_alloc_burp((ULONG) size)
#define	FILE_IS_FULL			-9
#define	FILE_NM_ARR_LEN			20
#define	GSPLIT_HDR_REC_NM		"InterBase/Gsplit"

#define	IO_BUFFER_SIZE			(16 * K_BYTES)

#define	K_BYTES					1024
#define	M_BYTES					(K_BYTES * K_BYTES)
#define	G_BYTES					(K_BYTES * M_BYTES)
#define	MAX_FILE_NM_LEN			27		/* size of header_rec.fl_name */
#define	MAX_NUM_OF_FILES		9999
#define	MIN_FILE_SIZE			M_BYTES
#define	NEW_LINE				'\n'
#define	TERMINAL				'\0'

typedef	int	FILE_DESC;

