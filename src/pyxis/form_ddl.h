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
    gds_dyn_version_1, 
       gds_dyn_begin, 
          gds_dyn_def_global_fld, 10,0, 'P','Y','X','I','S','$','F','O','R','M',
             gds_dyn_system_flag, 2,0, 2,0,
             gds_dyn_fld_type, 2,0, 5,1,
             gds_dyn_fld_length, 2,0, 8,0,
             gds_dyn_fld_scale, 2,0, 0,0,
             gds_dyn_fld_sub_type, 2,0, 0,0,
             gds_dyn_fld_segment_length, 2,0, 'P',0,
             gds_dyn_end, 
          gds_dyn_def_global_fld, 15,0, 'P','Y','X','I','S','$','F','O','R','M','_','N','A','M','E',
             gds_dyn_system_flag, 2,0, 2,0,
             gds_dyn_fld_type, 2,0, 14,0,
             gds_dyn_fld_length, 2,0, 31,0,
             gds_dyn_fld_scale, 2,0, 0,0,
             gds_dyn_fld_sub_type, 2,0, 0,0,
             gds_dyn_end, 
          gds_dyn_def_global_fld, 15,0, 'P','Y','X','I','S','$','F','O','R','M','_','T','Y','P','E',
             gds_dyn_system_flag, 2,0, 2,0,
             gds_dyn_fld_type, 2,0, 14,0,
             gds_dyn_fld_length, 2,0, 16,0,
             gds_dyn_fld_scale, 2,0, 0,0,
             gds_dyn_fld_sub_type, 2,0, 0,0,
             gds_dyn_end, 
          gds_dyn_def_rel, 11,0, 'P','Y','X','I','S','$','F','O','R','M','S',
             gds_dyn_system_flag, 2,0, 2,0,
             gds_dyn_end, 
          gds_dyn_def_local_fld, 15,0, 'P','Y','X','I','S','$','F','O','R','M','_','N','A','M','E',
             gds_dyn_rel_name, 11,0, 'P','Y','X','I','S','$','F','O','R','M','S',
             gds_dyn_system_flag, 2,0, 2,0,
             gds_dyn_fld_position, 2,0, 1,0,
             gds_dyn_end, 
          gds_dyn_def_local_fld, 15,0, 'P','Y','X','I','S','$','F','O','R','M','_','T','Y','P','E',
             gds_dyn_rel_name, 11,0, 'P','Y','X','I','S','$','F','O','R','M','S',
             gds_dyn_system_flag, 2,0, 2,0,
             gds_dyn_fld_position, 2,0, 2,0,
             gds_dyn_end, 
          gds_dyn_def_local_fld, 10,0, 'P','Y','X','I','S','$','F','O','R','M',
             gds_dyn_rel_name, 11,0, 'P','Y','X','I','S','$','F','O','R','M','S',
             gds_dyn_system_flag, 2,0, 2,0,
             gds_dyn_fld_position, 2,0, 3,0,
             gds_dyn_end, 
          gds_dyn_def_idx, 11,0, 'P','Y','X','I','S','$','I','N','D','E','X',
             gds_dyn_rel_name, 11,0, 'P','Y','X','I','S','$','F','O','R','M','S',
             gds_dyn_idx_unique, 2,0, 1,0,
             gds_dyn_idx_inactive, 2,0, 0,0,
             gds_dyn_fld_name, 15,0, 'P','Y','X','I','S','$','F','O','R','M','_','N','A','M','E',
             gds_dyn_end, 
          gds_dyn_end, 
    gds_dyn_eoc
