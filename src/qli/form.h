/*
 *	PROGRAM:	JRD Command Oriented Query Language
 *	MODULE:		form.h
 *	DESCRIPTION:	Form interface definitions
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

#ifndef _QLI_FORM_H_
#define _QLI_FORM_H_

#ifdef PYXIS

/* Form block */

typedef struct frm {
    struct blk	frm_header;
    struct frm	*frm_next;		/* Next form in database */
    struct dbb	*frm_database;		/* Parent database */
    struct sym	*frm_symbol;		/* Associated symbol */
    struct ffl	*frm_fields;		/* Form fields */
    int		*frm_handle;
    TEXT	frm_name[2];
} *FRM;

/* Form field block */

typedef struct ffl {
    struct blk	ffl_header;
    FRM		ffl_form;		/* Parent form */
    struct sym	*ffl_symbol;		/* Associated symbol */
    struct ffl	*ffl_next;		/* Next field in form */
    int		*ffl_handle;
    USHORT	ffl_dtype;
    SSHORT	ffl_scale;
    USHORT	ffl_length;
    TEXT	ffl_name[2];
} *FFL;

/* Menu block */

typedef struct men {
    struct blk	men_header;
    struct men	*men_next;		/* Next menu in system */
    int		*men_handle;		/* Internal menu handle */
    TEXT	men_menu[2];
} *MEN;

#endif

#endif /* _QLI_FORM_H_ */
