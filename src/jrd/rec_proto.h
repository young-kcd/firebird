/*
 *	PROGRAM:	JRD Access method
 *	MODULE:		rec_proto.h		
 *	DESCRIPTION:	Prototype Header file for rec.c 
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

#ifndef JRD_REC_PROTO_H
#define JRD_REC_PROTO_H

void	REC_recover(TEXT *, TEXT *, struct ctrl_pt *, struct sbm **, USHORT);
void	REC_recover_page(TEXT *, TEXT *, struct ctrl_pt *, SLONG, struct pag *);
void	REC_update_next_transid(void);

#endif // JRD_REC_PROTO_H
