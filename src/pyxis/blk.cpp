/*
 *	PROGRAM:	PYXIS Form Package
 *	MODULE:		blk.c
 *	DESCRIPTION:	Block size tables
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

#include "../jrd/ib_stdio.h"
#include "../pyxis/everything.h"

#define BLKDEF(type, root, tail) sizeof (struct root), tail,
struct {
	SSHORT typ_root_length;
	SSHORT typ_tail_length;
} PYXIS_block_sizes[] = {
	0, 0,
#include "../pyxis/blk.h"
0};

#undef BLKDEF
