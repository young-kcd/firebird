/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		dmp_proto.h
 *	DESCRIPTION:	Prototype header file for dmp.c
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

#ifndef JRD_DMP_PROTO_H
#define JRD_DMP_PROTO_H

void DMP_active(void);
void DMP_btc(void);
void DMP_btc_errors(void);
void DMP_btc_ordered(void);
void DMP_dirty(void);
void DMP_page(SLONG, USHORT);
void DMP_fetched_page(struct pag *, ULONG, ULONG, USHORT);

#endif // JRD_DMP_PROTO_H

