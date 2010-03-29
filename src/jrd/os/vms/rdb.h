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
#ifndef JRD_RDB_H
#define JRD_RDB_H

const int rdb$_arith_except       0x138800a
const int rdb$_bad_dbkey          0x1388012
const int rdb$_bad_db_format      0x138801a
const int rdb$_bad_db_handle      0x1388022
const int rdb$_bad_dpb_content    0x138802a
const int rdb$_bad_dpb_form       0x1388032
const int rdb$_bad_req_handle     0x138803a
const int rdb$_bad_segstr_handle  0x1388042
const int rdb$_bad_segstr_id      0x138804a
const int rdb$_bad_tpb_content    0x1388052
const int rdb$_bad_tpb_form       0x138805a
const int rdb$_bad_trans_handle   0x1388062
const int rdb$_bug_check          0x138806c
const int rdb$_convert_error      0x1388072
const int rdb$_db_corrupt         0x138807c
const int rdb$_deadlock           0x1388082
const int rdb$_from_no_match      0x1388092
const int rdb$_infinap            0x1388098
const int rdb$_infona             0x13880a0
const int rdb$_infunk             0x13880a8
const int rdb$_integ_fail         0x13880b2
const int rdb$_invalid_blr        0x13880ba
const int rdb$_io_error           0x13880c4
const int rdb$_lock_conflict      0x13880ca
const int rdb$_metadata_corrupt   0x13880d4
const int rdb$_not_valid          0x13880da
const int rdb$_no_cur_rec         0x13880e2
const int rdb$_no_dup             0x13880ea
const int rdb$_no_finish          0x13880f3
const int rdb$_no_meta_update     0x13880fa
const int rdb$_no_priv            0x1388102
const int rdb$_no_recon           0x138810a
const int rdb$_no_record          0x1388112
const int rdb$_no_segstr_close    0x138811a
const int rdb$_obsolete_metadata  0x1388122
const int rdb$_open_trans         0x138812a
const int rdb$_port_len           0x1388132
const int rdb$_read_only_field    0x138813a
const int rdb$_read_only_rel      0x1388142
const int rdb$_read_only_trans    0x138814a
const int rdb$_read_only_view     0x1388152
const int rdb$_req_no_trans       0x138815a
const int rdb$_req_sync           0x1388162
const int rdb$_req_wrong_db       0x138816a
const int rdb$_segment            0x1388171
const int rdb$_segstr_eof         0x138817a
const int rdb$_segstr_no_op       0x1388182
const int rdb$_segstr_no_read     0x138818a
const int rdb$_segstr_no_trans    0x1388192
const int rdb$_segstr_no_write    0x138819a
const int rdb$_segstr_wrong_db    0x13881a2
const int rdb$_sys_request        0x13881b4
const int rdb$_unavailable        0x13881ba
const int rdb$_unres_rel          0x13881c2
const int rdb$_uns_ext            0x13881cc
const int rdb$_wish_list          0x13881d4
const int rdb$_wrong_ods          0x13881dc
const int rdb$_wronumarg          0x13881e2
const int rdb$_imp_exc            0x13881ea
//#define rdbb$_badblk             0x16a800a
//#define rdbb$_invpoolcl          0x16a8012
//#define rdbb$_nopoolids          0x16a801a
//#define rdbb$_relbadblk          0x16a8022
//#define rdbb$_blktoobig          0x16a802a
//#define rdbb$_bufexh             0x16a8032
//#define rdbb$_syntaxerr          0x16a803a
//#define rdbb$_bufinuse           0x16a8042
//#define rdbb$_bdbincon           0x16a804a
//#define rdbb$_reqinuse           0x16a8052
//#define rdbb$_badodsver          0x16a805a
//#define rdbb$_relnotdef          0x16a8062
//#define rdbb$_fldnotdef          0x16a806a
//#define rdbb$_dirtypage          0x16a8073
//#define rdbb$_waifortra          0x16a807b
//#define rdbb$_doubleloc          0x16a8082
//#define rdbb$_nodnotfnd          0x16a808b
//#define rdbb$_dupnodfnd          0x16a8093
//#define rdbb$_locnotmar          0x16a80a3
//#define rdbb$_badpagtyp          0x16a80ac
//#define rdbb$_corrupt            0x16a80b4
//#define rdbb$_badpage            0x16a80bb
//#define rdbb$_badindex           0x16a80cb
//#define rdbb$_dbbnotzer          0x16a80d2
//#define rdbb$_tranotzer          0x16a80da
//#define rdbb$_trareqmis          0x16a80e2
//#define rdbb$_badhndcnt          0x16a80ea
//#define rdbb$_wrotpbver          0x16a80f2
//#define rdbb$_wroblrver          0x16a80fa
//#define rdbb$_wrodpbver          0x16a8102
//#define rdbb$_blobnotsup         0x16a810a
//#define rdbb$_badrelation        0x16a8112
//#define rdbb$_nodetach           0x16a811b
//#define rdbb$_notremote          0x16a81ba
//#define rdbb$_trainlim           0x16a81c2
//#define rdbb$_notinlim           0x16a81ca
//#define rdbb$_traoutsta          0x16a81d2
//#define rdbb$_connect_reject     0x16a81da
//#define rdbb$_dbfile             0x16a81e2
//#define rdbb$_orphan             0x16a81eb

#endif /* JRD_RDB_H */
