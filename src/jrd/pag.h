/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		pag.h
 *	DESCRIPTION:	Page interface definitions
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

/*
 * Modified by: Patrick J. P. Griffin
 * Date: 11/29/2000
 * Problem:   Bug 116733 Too many generators corrupt database.
 *            DPM_gen_id was not calculating page and offset correctly.
 * Change:    Add pgc_gpg, number of generators per page,
 *            for use in DPM_gen_id.
 */


#ifndef _JRD_PAG_H_
#define _JRD_PAG_H_

#include "../jrd/jrd_blks.h"
#include "../include/fb_blk.h"

/* Page control block -- used by PAG to keep track of critical
   constants */

class pgc : public pool_alloc<type_pgc>
{
    public:
	SLONG pgc_high_water;		/* Lowest PIP with space */
	SLONG pgc_ppp;				/* Pages per pip */
	SLONG pgc_pip;				/* First pointer page */
	ULONG pgc_bytes;			/* Number of bytes of bit in PIP */
	ULONG pgc_tpt;				/* Transactions per TIP */
	ULONG pgc_gpg;				/* Generators per generator page */
};
typedef pgc *PGC;

#endif /* _JRD_PAG_H_ */
