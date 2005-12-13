/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		pio_proto.h
 *	DESCRIPTION:	Prototype header file for pio.c
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

#ifndef _JRD_PIO_PROTO_H_
#define _JRD_PIO_PROTO_H_

#ifdef __cplusplus
extern "C" {
#endif

extern int PIO_add_file(struct dbb *, struct fil *, TEXT *, SLONG);
extern void PIO_close(struct fil *);
extern struct fil *PIO_create(struct dbb *, TEXT *, SSHORT, BOOLEAN);
extern int PIO_connection(TEXT *, USHORT *);
extern int PIO_expand(TEXT *, USHORT, TEXT *);
extern void PIO_flush(struct fil *);
extern void PIO_force_write(struct fil *, USHORT);
extern void PIO_header(struct dbb *, SCHAR *, int);
extern SLONG PIO_max_alloc(struct dbb *);
extern SLONG PIO_act_alloc(struct dbb *);
extern struct fil *PIO_open(struct dbb *, TEXT *, SSHORT, SSHORT,
							struct blk *, TEXT *, USHORT);
extern int PIO_read(struct fil *, struct bdb *, struct pag *, ISC_STATUS *);

#ifdef SUPERSERVER_V2
extern int PIO_read_ahead(struct dbb *, SLONG, SCHAR *, SLONG, struct piob *,
						  ISC_STATUS *);
extern int PIO_status(struct piob *, ISC_STATUS *);
#endif

extern int PIO_unlink(TEXT *);
extern int PIO_write(struct fil *, struct bdb *, struct pag *, ISC_STATUS *);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _JRD_PIO_PROTO_H_ */
