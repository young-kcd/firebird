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

#ifndef JRD_PIO_PROTO_H
#define JRD_PIO_PROTO_H

#ifdef __cplusplus
extern "C" {
#endif

int		PIO_add_file(struct dbb *, struct fil *, TEXT *, SLONG);
void	PIO_close(struct fil *);
struct fil*	PIO_create(struct dbb *, TEXT *, SSHORT, BOOLEAN);
int		PIO_connection(TEXT *, USHORT *);
int		PIO_expand(TEXT *, USHORT, TEXT *);
void	PIO_flush(struct fil *);
void	PIO_force_write(struct fil *, USHORT);
void	PIO_header(struct dbb *, SCHAR *, int);
SLONG	PIO_max_alloc(struct dbb *);
SLONG	PIO_act_alloc(struct dbb *);
struct fil*	PIO_open(struct dbb *, TEXT *, SSHORT, SSHORT,
							struct blk *, TEXT *, USHORT);
int		PIO_read(struct fil *, struct bdb *, struct pag *, ISC_STATUS *);

#ifdef SUPERSERVER_V2
int		PIO_read_ahead(struct dbb *, SLONG, SCHAR *, SLONG, struct piob *,
						  ISC_STATUS *);
int		PIO_status(struct piob *, ISC_STATUS *);
#endif

int		PIO_unlink(TEXT *);
int		PIO_write(struct fil *, struct bdb *, struct pag *, ISC_STATUS *);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // JRD_PIO_PROTO_H
