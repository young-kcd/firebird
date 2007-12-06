/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		pcmet_proto.h
 *	DESCRIPTION:	Prototype header file for pcmet.c
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

#ifndef _JRD_PCMET_PROTO_H_
#define _JRD_PCMET_PROTO_H_

extern int PCMET_expression_index(TDBB, SSHORT, struct dfw *, struct jrd_tra *);
extern void PCMET_lookup_index(struct jrd_rel *, struct idx *);

#endif /* _JRD_PCMET_PROTO_H_ */
