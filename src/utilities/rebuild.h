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
#ifndef _UTILITIES_REBUILD_H_
#define _UTILITIES_REBUILD_H_

#include "../jrd/common.h"
#include "../jrd/ods.h"

#define BIG_NUMBER	(ULONG) -1

typedef struct fil {
    int		fil_file;
    SSHORT	fil_length;
    SCHAR	fil_name[1];
} *FIL;

typedef struct rbdb {
    ULONG	rbdb_map_length; 
    int		rbdb_map_base;
    int		rbdb_map_count;
    SCHAR	*rbdb_map_region;
    SSHORT	rbdb_page_size;
    SCHAR	rbdb_valid;
    SLONG	rbdb_window_offset;
    SLONG	rbdb_last_page;
    PAG		rbdb_buffer1;
    PAG		rbdb_buffer2;
    struct rbdb	*rbdb_next;
    struct fil	rbdb_file;
} *RBDB;

typedef struct swc { 
    SCHAR    swc_switch;
    SCHAR    swc_comma;
    TEXT    *swc_string;
} *SWC;

#endif /* _UTILITIES_REBUILD_H_ */
