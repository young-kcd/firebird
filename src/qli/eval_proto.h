/*
 *	PROGRAM:	JRD Command Oriented Query Language
 *	MODULE:		eval_proto.h
 *	DESCRIPTION:	Prototype header file for eval.cpp
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

#ifndef QLI_EVAL_PROTO_H
#define QLI_EVAL_PROTO_H

extern int		EVAL_boolean (struct qli_nod *);
extern void		EVAL_break_compute (struct qli_nod *);
extern void		EVAL_break_increment (struct qli_nod *);
extern void		EVAL_break_init (struct qli_nod *);
extern struct dsc	*EVAL_parameter (struct par *);
extern struct dsc	*EVAL_value (struct qli_nod *);

#endif /* QLI_EVAL_PROTO_H */

