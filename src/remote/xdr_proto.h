/*
 *	PROGRAM:	JRD Remote Server 
 *	MODULE:		xdr_proto.h
 *	DESCRIPTION:	Prototype Header file for xdr.c
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

#ifndef REMOTE_XDR_PROTO_H
#define REMOTE_XDR_PROTO_H

#include "../remote/xdr.h"

// 15 Jan 2003. Nickolay Samofatov
// Lower functions need to have C++ linkage to avoid name clash with
// standard XDR. Firebird XDR is NOT compatible with Sun XDR at interface level

extern bool_t	xdr_bool (XDR *, int *);
extern bool_t	xdr_bytes (XDR *, SCHAR * *, u_int *, u_int);
extern bool_t	xdr_double (XDR *, double *);
extern bool_t	xdr_enum (XDR *, enum xdr_op *);
extern bool_t	xdr_float (XDR *, float *);
extern bool_t	xdr_free (xdrproc_t proc, SCHAR *objp);
extern bool_t	xdr_int (XDR *, int *);
extern bool_t	xdr_long (XDR *, SLONG *);
extern bool_t	xdrmem_create (XDR *, SCHAR *, u_int, enum xdr_op);
extern bool_t	xdr_opaque (XDR *, SCHAR *, u_int);
extern bool_t	xdr_short (XDR *, SSHORT *);
extern bool_t	xdr_string (XDR *, SCHAR * *, u_int);
extern bool_t	xdr_u_int (XDR *, u_int *);
extern bool_t	xdr_u_long (XDR *, u_long *);
extern bool_t	xdr_u_short (XDR *, u_short *);
extern int		xdr_union (XDR *, enum xdr_op *, SCHAR *, struct xdr_discrim *, xdrproc_t);
extern bool_t	xdr_wrapstring (XDR *, SCHAR **);
extern bool_t	xdr_hyper(XDR *, SINT64 *);

#endif	/* REMOTE_XDR_PROTO_H */
