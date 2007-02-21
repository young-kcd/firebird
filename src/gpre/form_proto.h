/*
 *	PROGRAM:	Preprocessor
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
#ifdef PYXIS

#ifndef _GPRE_FORM_PROTO_H_
#define _GPRE_FORM_PROTO_H_

extern GPRE_FLD FORM_lookup_field(FORM, int *, char *);
extern FORM FORM_lookup_form(DBB, UCHAR *);
extern FORM FORM_lookup_subform(FORM, GPRE_FLD);

#endif /* _GPRE_FORM_PROTO_H_ */

#endif
