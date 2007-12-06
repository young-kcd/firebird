/*
 *	PROGRAM:	JRD Command Oriented Query Language
 *	MODULE:		form_proto.h
 *	DESCRIPTION:	Prototype header file for form.c
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

#ifndef _QLI_FORM_PROTO_H_
#define _QLI_FORM_PROTO_H_

#ifdef PYXIS
extern struct frm	*FORM_default_form (struct dbb *, TEXT *);
extern void		FORM_display (struct qli_nod *);
extern void		FORM_edit (struct dbb *, TEXT *);
extern void		FORM_fini (void);
extern void		FORM_finish (struct dbb *);
extern int		FORM_get_blob (struct qli_nod *, struct par *);
extern struct dsc	*FORM_get_field (struct qli_nod *);
extern struct frm	*FORM_lookup_form (struct dbb *, TEXT *);
extern struct ffl	*FORM_lookup_field (struct frm *, TEXT *);
extern int		FORM_put_field (struct qli_nod *, struct qli_nod *);
extern void		FORM_reset (void);
extern void		FORM_update (struct qli_nod *);
#endif

#endif /* _QLI_FORM_PROTO_H_ */
