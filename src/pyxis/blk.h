/*
 *	PROGRAM:	PYXIS Form Package
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

BLKDEF (type_lls, lls, 0)
BLKDEF (type_frb, frb, 0)
BLKDEF (type_hnk, hnk, 0)
BLKDEF (type_plb, plb, 0)
BLKDEF (type_vec, vec, sizeof (((VEC) 0)->vec_object[0]))

BLKDEF (type_att, att, 1)
BLKDEF (type_obj, p_obj, 0)
BLKDEF (type_scr, scr, sizeof (((SCR) 0)->scr_screen [0]))
BLKDEF (type_win, win, 0)
BLKDEF (type_str, str, 1)
BLKDEF (type_pic, pic, 0)
BLKDEF (type_map, map, sizeof (((MAP) 0)->map_rpt [0]))
BLKDEF (type_menu, menu, 0)
