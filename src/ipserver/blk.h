/*
 *	PROGRAM:	Interprocess server interface
 *	MODULE:		blk.h
 *	DESCRIPTION:	Block type definitions
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

BLKDEF(type_idb, idb, 0)
	BLKDEF(type_irq, irq, 0)
	BLKDEF(type_itr, itr, 0)
	BLKDEF(type_ibl, ibl, 1)
	BLKDEF(type_ivnt, ivnt, 0)
	BLKDEF(type_ipserver_isr, ipserver_isr, 0)
	BLKDEF(type_icc, icc, 0)
