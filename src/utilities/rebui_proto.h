/*
 *	PROGRAM:	JRD Rebuild scrambled database
 *	MODULE:		rebui_proto.h
 *	DESCRIPTION:	Prototype header file for rebuild.c
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

#ifndef _UTILITIES_REBUI_PROTO_H_
#define _UTILITIES_REBUI_PROTO_H_

extern SCHAR		*RBDB_alloc (SLONG);
extern void		RBDB_close (struct rbdb *);
extern void		RBDB_open (struct rbdb *);
extern struct pag	*RBDB_read (struct rbdb *, SLONG);
extern void		RBDB_write (struct rbdb *, struct pag *, SLONG);

#endif /* _UTILITIES_REBUI_PROTO_H_ */
