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

#define rdb$_arith_except       0x138800a
#define rdb$_bad_dbkey          0x1388012
#define rdb$_bad_db_format      0x138801a
#define rdb$_bad_db_handle      0x1388022
#define rdb$_bad_dpb_content    0x138802a
#define rdb$_bad_dpb_form       0x1388032
#define rdb$_bad_req_handle     0x138803a
#define rdb$_bad_segstr_handle  0x1388042
#define rdb$_bad_segstr_id      0x138804a
#define rdb$_bad_tpb_content    0x1388052
#define rdb$_bad_tpb_form       0x138805a
#define rdb$_bad_trans_handle   0x1388062
#define rdb$_bug_check          0x138806c
#define rdb$_convert_error      0x1388072
#define rdb$_db_corrupt         0x138807c
#define rdb$_deadlock           0x1388082
#define rdb$_from_no_match      0x1388092
#define rdb$_infinap            0x1388098
#define rdb$_infona             0x13880a0
#define rdb$_infunk             0x13880a8
#define rdb$_integ_fail         0x13880b2
#define rdb$_invalid_blr        0x13880ba
#define rdb$_io_error           0x13880c4
#define rdb$_lock_conflict      0x13880ca
#define rdb$_metadata_corrupt   0x13880d4
#define rdb$_not_valid          0x13880da
#define rdb$_no_cur_rec         0x13880e2
#define rdb$_no_dup             0x13880ea
#define rdb$_no_finish          0x13880f3
#define rdb$_no_meta_update     0x13880fa
#define rdb$_no_priv            0x1388102
#define rdb$_no_recon           0x138810a
#define rdb$_no_record          0x1388112
#define rdb$_no_segstr_close    0x138811a
#define rdb$_obsolete_metadata  0x1388122
#define rdb$_open_trans         0x138812a
#define rdb$_port_len           0x1388132
#define rdb$_read_only_field    0x138813a
#define rdb$_read_only_rel      0x1388142
#define rdb$_read_only_trans    0x138814a
#define rdb$_read_only_view     0x1388152
#define rdb$_req_no_trans       0x138815a
#define rdb$_req_sync           0x1388162
#define rdb$_req_wrong_db       0x138816a
#define rdb$_segment            0x1388171
#define rdb$_segstr_eof         0x138817a
#define rdb$_segstr_no_op       0x1388182
#define rdb$_segstr_no_read     0x138818a
#define rdb$_segstr_no_trans    0x1388192
#define rdb$_segstr_no_write    0x138819a
#define rdb$_segstr_wrong_db    0x13881a2
#define rdb$_sys_request        0x13881b4
#define rdb$_unavailable        0x13881ba
#define rdb$_unres_rel          0x13881c2
#define rdb$_uns_ext            0x13881cc
#define rdb$_wish_list          0x13881d4
#define rdb$_wrong_ods          0x13881dc
#define rdb$_wronumarg          0x13881e2
#define rdb$_imp_exc            0x13881ea
#define rdbb$_badblk             0x16a800a
#define rdbb$_invpoolcl          0x16a8012
#define rdbb$_nopoolids          0x16a801a
#define rdbb$_relbadblk          0x16a8022
#define rdbb$_blktoobig          0x16a802a
#define rdbb$_bufexh             0x16a8032
#define rdbb$_syntaxerr          0x16a803a
#define rdbb$_bufinuse           0x16a8042
#define rdbb$_bdbincon           0x16a804a
#define rdbb$_reqinuse           0x16a8052
#define rdbb$_badodsver          0x16a805a
#define rdbb$_relnotdef          0x16a8062
#define rdbb$_fldnotdef          0x16a806a
#define rdbb$_dirtypage          0x16a8073
#define rdbb$_waifortra          0x16a807b
#define rdbb$_doubleloc          0x16a8082
#define rdbb$_nodnotfnd          0x16a808b
#define rdbb$_dupnodfnd          0x16a8093
#define rdbb$_locnotmar          0x16a80a3
#define rdbb$_badpagtyp          0x16a80ac
#define rdbb$_corrupt            0x16a80b4
#define rdbb$_badpage            0x16a80bb
#define rdbb$_badindex           0x16a80cb
#define rdbb$_dbbnotzer          0x16a80d2
#define rdbb$_tranotzer          0x16a80da
#define rdbb$_trareqmis          0x16a80e2
#define rdbb$_badhndcnt          0x16a80ea
#define rdbb$_wrotpbver          0x16a80f2
#define rdbb$_wroblrver          0x16a80fa
#define rdbb$_wrodpbver          0x16a8102
#define rdbb$_blobnotsup         0x16a810a
#define rdbb$_badrelation        0x16a8112
#define rdbb$_nodetach           0x16a811b
#define rdbb$_notremote          0x16a81ba
#define rdbb$_trainlim           0x16a81c2
#define rdbb$_notinlim           0x16a81ca
#define rdbb$_traoutsta          0x16a81d2
#define rdbb$_connect_reject     0x16a81da
#define rdbb$_dbfile             0x16a81e2
#define rdbb$_orphan             0x16a81eb

#endif /* JRD_RDB_H */
