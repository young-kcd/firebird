/*
 *	PROGRAM:	Language Preprocessor
 *	MODULE:		form.h
 *	DESCRIPTION:	Form definition modules
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

#ifndef _GPRE_FORM_H_
#define _GPRE_FORM_H_

/* Form instance */

typedef struct form {
	dbb* form_dbb;				/* Parent database */
	form* form_next;			/* Next form in database */
	sym* form_name;				/* Form name */
	gpre_fld* form_fields;		/* Form fields */
	gpre_req* form_parent;		/* Parent form request */
	gpre_fld* form_field;		/* Field in parent */
	int *form_object;			/* Form data structure */
	TEXT *form_handle;			/* Runtime form handle */
} *FORM;

/* Form Interaction */

typedef struct fint {
	gpre_req* fint_request;		/* Parent request */
	USHORT fint_flags;			/* Misc crud */
	lls* fint_display_fields;	/* List of fields for display */
	lls* fint_update_fields;	/* List of fields for update */
	lls* fint_wakeup_fields;	/* List of fields for immediate wakeup */
	lls* fint_position_fields;	/* Field for cursor position */
	lls* fint_override_fields;	/* Fields to be left alone */
} *FINT;

#define FINT_display_all	1
#define FINT_update_all		2
#define FINT_wakeup_all		4
#define FINT_override_all	8
#define FINT_no_wait		16

/* menu instance.  Note that the entree structure must appear at the beginning of the
   menu structure.  */


typedef struct entree {
	gpre_req* entree_request;	/* Parent request */
	USHORT entree_entree;		/* ident for entree */
	USHORT entree_value;		/* ident for entree value */
	USHORT entree_end;			/* ident for end flag for get */
} *ENTREE;

typedef struct menu {
	entree menu_entree;			/* current/terminating entree */
	USHORT menu_title;			/* ident for menu title */
	USHORT menu_terminator;		/* ident for menu terminator */
} *MENU;

#define		menu_request		menu_entree.entree_request
#define		menu_entree_value	menu_entree.entree_value
#define		menu_entree_entree	menu_entree.entree_entree

#endif /* _GPRE_FORM_H_ */

#endif
