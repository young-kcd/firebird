/*
 *	PROGRAM:	JRD Command Oriented Query Language
 *	MODULE:		mov_proto.h
 *	DESCRIPTION:	Prototype header file for mov.c
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

#ifndef _QLI_MOV_PROTO_H_
#define _QLI_MOV_PROTO_H_

extern int			MOVQ_compare (struct dsc *, struct dsc *);
extern double		MOVQ_date_to_double (struct dsc *);
extern int			MOVQ_decompose (TEXT *, USHORT, SLONG *);
extern void			MOVQ_double_to_date (double, SLONG [2]);
extern void			MOVQ_fast (SCHAR *, SCHAR *, ULONG);
extern double		MOVQ_get_double (struct dsc *);
extern SLONG		MOVQ_get_long (struct dsc *, SSHORT);
extern int			MOVQ_get_string (struct dsc *, TEXT **, struct vary *, USHORT);
extern void			MOVQ_move (struct dsc *, struct dsc *);
extern void			MOVQ_terminate (SCHAR *, SCHAR *, SSHORT, SSHORT);

#endif /* _QLI_MOV_PROTO_H_ */
