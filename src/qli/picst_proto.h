/*
 *	PROGRAM:	JRD Command Oriented Query Language
 *	MODULE:		picst_proto.h
 *	DESCRIPTION:	Prototype header file for picstr.c
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

#ifndef QLI_PICST_PROTO_H
#define QLI_PICST_PROTO_H

extern struct pics* PIC_analyze (TEXT*, const struct dsc*);
extern void		PIC_edit (struct dsc *, struct pics *, TEXT **, USHORT);
extern void		PIC_missing (struct con *, struct pics *);

#endif /* QLI_PICST_PROTO_H */

