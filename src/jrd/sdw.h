/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		sdw.h
 *	DESCRIPTION:	Shadowing definitions
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

#ifndef JRD_SDW_H
#define JRD_SDW_H

#include "../jrd/jrd_blks.h"
#include "../include/fb_blk.h"

/* Shadowing block */

class sdw : public pool_alloc<type_sdw>
{
    public:
	struct sdw *sdw_next;		/* next in linked list */
	struct fil *sdw_file;		/* Stack of shadow files */
	USHORT sdw_number;			/* number of shadow */
	USHORT sdw_flags;
};
typedef sdw *SDW;

#define SDW_dumped	1			/* bit set when file has been copied */
#define SDW_shutdown	2		/* stop shadowing on next cache flush */
#define SDW_manual	4			/* shadow is a manual shadow--don't delete */
#define	SDW_delete	8			/* delete the shadow at the next shutdown */
#define SDW_found	16			/* flag to mark shadow found in database */
#define SDW_rollover	32		/* this shadow was rolled over to when the main db file went away */
#define SDW_conditional	64		/* shadow to be used if another shadow becomes unavailable */

/* these macros are a convenient combination of switches: 
   the first specifies the shadow is invalid for writing to;
   the second specifies that the shadow no SLONGer exists and the
   shadow block simply hasn't been cleared out yet */

#define SDW_INVALID	(SDW_shutdown | SDW_delete | SDW_rollover | SDW_conditional)
#define SDW_IGNORE	(SDW_shutdown | SDW_delete)

#endif /* JRD_SDW_H */
