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

#ifdef __cplusplus
extern "C" {
#endif


extern bool_t	xdr_bool (register XDR *, register int *);
extern bool_t	xdr_bytes (register XDR *, register SCHAR * *, u_int *, register u_int);
extern bool_t	xdr_double (register XDR *, register double *);
extern bool_t	xdr_enum (register XDR *, enum xdr_op *);
extern bool_t	xdr_float (register XDR *, register float *);
extern bool_t	xdr_free (xdrproc_t proc, SCHAR *objp);
extern bool_t	xdr_int (register XDR *, register int *);
extern bool_t	xdr_long (register XDR *, register SLONG *);
extern bool_t	xdrmem_create (register XDR *, SCHAR *, u_int, enum xdr_op);
extern bool_t	xdr_opaque (register XDR *, register SCHAR *, register u_int);
extern bool_t	xdr_short (register XDR *, register SSHORT *);
extern bool_t	xdr_string (register XDR *, register SCHAR * *, register u_int);
extern bool_t	xdr_u_int (register XDR *, register u_int *);
extern bool_t	xdr_u_long (register XDR *, register u_long *);
extern bool_t	xdr_u_short (register XDR *, register u_short *);
extern int		xdr_union (XDR *, enum xdr_op *, SCHAR *, struct xdr_discrim *, xdrproc_t);
extern bool_t	xdr_wrapstring (register XDR *, register SCHAR **);

/* I assume all of these should be from the rpc/xdr.h file 
 * rather than having the definitions here, but since I don't 
 * want to create more trouble than I need, Im adding this 
 * definition here, where it is supplied by the OS.  Otherwise 
 * a local function is defined to do the same task.
 * MOD 12-Nov-2002 */

#ifdef HAVE_XDR_HYPER
extern bool_t xdr_hyper(register XDR *, SINT64 *);
#endif



#ifdef __cplusplus
}	/* extern "C" */
#endif


#endif	/* REMOTE_XDR_PROTO_H */
