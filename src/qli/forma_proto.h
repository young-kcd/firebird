/*
 *	PROGRAM:	JRD Command Oriented Query Language
 *	MODULE:		forma_proto.h
 *	DESCRIPTION:	Prototype header file for format.c
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

#ifndef _QLI_FORMA_PROTO_H_
#define _QLI_FORMA_PROTO_H_

extern int		FMT_expression (struct qli_nod *);
extern TEXT		*FMT_format (struct lls *);
extern struct qli_nod	*FMT_list (struct qli_nod *);
extern void		FMT_print (struct qli_nod *, struct prt *);
extern void		FMT_put (TEXT *, struct prt *);
extern void		FMT_report (struct rpt *);

#endif /* _QLI_FORMA_PROTO_H_ */
