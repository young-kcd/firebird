/*
 *	PROGRAM:	JRD Access method
 *	MODULE:		tpc_proto.h
 *	DESCRIPTION:	Prototype Header file for tpc.c
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

#ifndef _JRD_TPC_PROTO_H_
#define _JRD_TPC_PROTO_H_

extern int TPC_cache_state(TDBB, SLONG);
extern void TPC_initialize_tpc(TDBB, SLONG);
extern void TPC_set_state(TDBB, SLONG, SSHORT);
extern int TPC_snapshot_state(TDBB, SLONG);
extern void TPC_update_cache(TDBB, struct tip *, SLONG);

#endif /* _JRD_TPC_PROTO_H_ */
