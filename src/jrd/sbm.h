/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		sbm.h
 *	DESCRIPTION:	Sparse bit map block definitions
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

#ifndef _JRD_SBM_H_
#define _JRD_SBM_H_

#if (defined PC_PLATFORM && !defined NETWARE_386)
#define BUNCH_BITS	4
#define BUNCH		SSHORT
#else
#define BUNCH_BITS	LOG2_BITS_PER_LONG
#define BUNCH		SLONG
#endif

#define SEGMENT_BITS	10
#define BITS_BUNCH	(1 << BUNCH_BITS)
#define BITS_SEGMENT	(1 << SEGMENT_BITS)
#define BUNCH_SEGMENT	BITS_SEGMENT / BITS_BUNCH

#define BUCKET_BITS	15
#define BUNCH_BUCKET	(1 << (BUCKET_BITS - SEGMENT_BITS))

/* Sparse bit map */

typedef struct sbm {
	struct blk sbm_header;
	struct sbm *sbm_next;
	struct plb *sbm_pool;
	UCHAR sbm_state;			/* State of bitmap */
	UCHAR sbm_type;				/* Root or bucket */
	USHORT sbm_count;			/* Slots allocated */
	USHORT sbm_used;			/* Slots used */
	USHORT sbm_high_water;		/* Maximum slot used */
	SLONG sbm_number;			/* Value when singular */
	struct bms *sbm_segments[1];
} *SBM;

/* States */

#define	SBM_EMPTY	0
#define SBM_SINGULAR	1
#define SBM_PLURAL	2

/* Types */

#define SBM_BUCKET	0
#define SBM_ROOT	1

/* Bit map segment */

typedef struct bms {
	struct blk bms_header;
	struct bms *bms_next;
	struct plb *bms_pool;
	SSHORT bms_min;				/* Minimum bit set in segment */
	SSHORT bms_max;				/* Maximum bit set in segment */
	BUNCH bms_bits[BUNCH_SEGMENT];
} *BMS;

#endif /* _JRD_SBM_H_ */
