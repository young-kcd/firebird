/*
 *	PROGRAM:	JRD Command Oriented Query Language
 *	MODULE:		eval_proto.h
 *	DESCRIPTION:	Prototype header file for eval.c
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

#ifndef _QLI_EVAL_PROTO_H_
#define _QLI_EVAL_PROTO_H_

extern int		EVAL_boolean (struct nod *);
extern void		EVAL_break_compute (struct nod *);
extern void		EVAL_break_increment (struct nod *);
extern void		EVAL_break_init (struct nod *);
extern struct dsc	*EVAL_parameter (struct par *);
extern struct dsc	*EVAL_value (struct nod *);

#endif /* _QLI_EVAL_PROTO_H_ */
