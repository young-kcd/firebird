/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		cvt2_proto.h
 *	DESCRIPTION:	Prototype header file for cvt2.c
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

#ifndef _JRD_CVT2_PROTO_H_
#define _JRD_CVT2_PROTO_H_

extern SSHORT CVT2_compare(struct dsc *, struct dsc *, FPTR_VOID);
extern SSHORT CVT2_blob_compare(struct dsc *, struct dsc *, FPTR_VOID);
extern void CVT2_get_name(struct dsc *, TEXT *, FPTR_VOID);
extern USHORT CVT2_make_string2(struct dsc *, USHORT, UCHAR **, struct vary *,
								USHORT, struct str **, FPTR_VOID);

#endif /* _JRD_CVT2_PROTO_H_ */
