/*
 *	PROGRAM:	PYXIS Form Package
 *	MODULE:		pyxis.h
 *	DESCRIPTION:	General header module
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

#ifndef _PYXIS_PYXIS_PROTO_H_
#define _PYXIS_PYXIS_PROTO_H_

/* Attribute block names, types, and block */

typedef struct att {
    struct blk	att_header;
    struct att	*att_next;		/* next attribute in object */
    struct att	*att_prior;		/* prior attribute in object */
    ATT_T	att_type;		/* attribute type */
    ATT_N	att_name;		/* attribute name */
    struct p_obj	*att_value;
    USHORT	att_length;		/* Allocated length for string */
    UCHAR	att_data[2];		/* room for data */
} *ATT;

/* Menus */

typedef struct menx {
    UCHAR	*menx_string; 
    USHORT 	menx_entree;
} *MENX;

typedef struct p_obj {
    struct blk	obj_header;
    struct p_obj	*obj_parent;
    USHORT	obj_reference_count;
    ATT		obj_attributes;		/* list of attributes */
    ATT		obj_end_attributes;	/* end of attribute list */
    ATT		obj_display_attribute;	/* attribute for display (if any) */
    USHORT	obj_flags;		/* misc stuff */
    SSHORT	obj_x;			/* absolute position */
    SSHORT	obj_y;			/* absoute  position */
    SSHORT	obj_scroll_x;		/* scrolled delta */
    SSHORT	obj_scroll_y;		/* scrolled delta */
    SSHORT	obj_rel_x;		/* relative position */
    SSHORT	obj_rel_y;		/* relative position */
    USHORT	obj_width;		/* displayed width */
    USHORT	obj_height;		/* displayed height */
    USHORT	obj_occluded;
    USHORT	*obj_display_address;
    USHORT	obj_display_length;
    USHORT	obj_display_height;
    USHORT	obj_border;		/* Size of border, if any */
    USHORT	obj_index;		/* Index in repeating group */
} *OBJ;

#define OBJ_changed		1		/* object has been updated */
#define OBJ_displayed		2		/* object is currently displayed */
#define OBJ_inactive		4		/* don't display object */
#define OBJ_reverse_video	8		/* reverse video */
#define OBJ_insert_right	16		/* (10) insert from right (numeric) */
#define OBJ_bold		32		/* (20) field is bold */
#define OBJ_underline		64		/* (40) field is underlined */
#define OBJ_box			128		/* (80) draw box around object */
#define OBJ_blank		256		/* (100) blank fill before drawing */
#define OBJ_updatable_flag	512		/* (200) object may be updated*/
#define OBJ_wakeup_flag		1024		/* (400) object will trigger wakeup*/
#define OBJ_reverse_for_update	2048		/* (800) when updatable, set reverse video*/
#define OBJ_menu_horizontal	4096		/* (1000) menu is horizontal*/


/* Window definition */

/*  Key names for special keys.  */

typedef struct knm {
    UCHAR	knm_keycode;
    SCHAR	*knm_keyname;
} *KNM;

typedef struct win {
    struct blk	win_header;		/* Block header */
    struct win	*win_parent;		/* Parent window */
    struct win	*win_sibling;		/* Next sibling window */
    struct scr	*win_logical;		/* Logical screen */
    struct scr	*win_physical;		/* Physical screen */
    USHORT	win_flags;
    USHORT	win_x;
    USHORT	win_y;
    USHORT	win_width;
    USHORT	win_height;
    USHORT	win_pending_char;	/* "returned" character */
    OBJ		win_form;
    int		*win_input;
    int		*win_output;
    int 	*win_trace_in;
    int 	*win_trace_out;
    int		win_current_x;
    int		win_current_y;
    int		win_current_mode;
    int		(*win_clear)();
    int		(*win_disable)();
    int		(*win_fini)();
    int		(*win_getchar)();
    int		(*win_text)(struct win*,UCHAR*, SSHORT, SSHORT, SSHORT, USHORT);
    int		(*win_update)();
    KNM		win_keyname_table;
} *WIN;

#define WIN_disabled	1		/* Forms are diabled */
#define WIN_reset_80	2		/* Reset 80 column mode at end */
#define WIN_synthesized	4		/* Input char was synthesized from mouse movement */
#define WIN_sensitive	8		/* Mouse movement is sensitive */

/* Program wide globals */

#ifdef PYXIS_SOURCE
struct plb	*PYXIS_default_pool = 0;
struct plb	*PYXIS_permanent_pool = 0;
#else
#ifndef FRED_SOURCE
extern struct plb	*PYXIS_default_pool;
extern struct plb	*PYXIS_permanent_pool;
#endif
#endif

/* Descriptor format */

#include "../jrd/dsc.h"

/* Picture string handling block */

#include "../qli/format.h"

/* Phase 3 record <-> form map */

/* Menu block */

typedef struct menu {
    struct blk	menu_header;
    OBJ		menu_object;
    USHORT	menu_flags;
    ATT		menu_current_entree;	/* current entree in dynamic menu */
} *MENU;

/* #define MENU_horizontal		1  moved to object block */
#define MENU_transparent	2

#ifdef __cplusplus
extern "C" {
#endif

extern int PYXIS_bugcheck(TEXT *);
extern int PYXIS_error(TEXT *);
extern OBJ PYXIS_get_attribute_value(OBJ , ATT_N );
extern ATT PYXIS_replace_attribute(OBJ , ATT_N , ATT_T , OBJ );
extern ATT PYXIS_put_attribute(OBJ , ATT_N , ATT_T , OBJ );
extern ATT PYXIS_find_object(OBJ , ATT , ATT_N , USHORT );
extern ATT PYXIS_navigate(OBJ , ATT , ATT_N , USHORT);
extern OBJ PYXIS_create_object(TEXT *, ATT_N );
extern int PYXIS_compute_size(OBJ , USHORT *, USHORT *);
extern int PYXIS_scroll_reset(OBJ );
extern int PYXIS_get_char(WIN, OBJ, int, int);
extern int PYXIS_delete_attribute(OBJ , ATT );
extern int PYXIS_set_display_attribute(OBJ , ATT );
extern OBJ PYXIS_clone(OBJ );
extern int PYXIS_delete_named_attribute(OBJ , ATT_N );
extern ATT PYXIS_get_attribute(OBJ , ATT_N );
extern int PYXIS_get_data(WIN , OBJ , ATT_N , OBJ , USHORT *);
extern WIN PYXIS_create_window(USHORT , USHORT );
extern int PYXIS_delete_object(OBJ );
extern int PYXIS_delete_window(WIN );
extern int PYXIS_push_tag(WIN , OBJ );
extern int PYXIS_push_form(WIN , OBJ , USHORT );
extern int PYXIS_update_window(WIN , OBJ , int , int );
extern int PYXIS_position(OBJ , USHORT , USHORT );
extern int PYXIS_pop_form(WIN );
extern int PYXIS_init();
extern int PYXIS_disable_window(WIN );
extern int PYXIS_push_handler( int (*)() );
extern int PYXIS_pop_handler( int (*)() );
extern int PYXIS_return_char(WIN , USHORT );
extern int PYXIS_top_form(WIN );
extern int PYXIS_reference(OBJ );
extern ATT PYXIS_find_enumeration(OBJ , ATT , USHORT );
extern SCHAR *PYXIS_get_keyname(WIN , UCHAR );
extern int PYXIS_move(OBJ , OBJ );
extern int PYXIS_pop_handler( int (*handler)() );
extern int PYXIS_push_handler( int (*handler)() );
extern OBJ PYXIS_get_value(ATT );

#ifdef __cplusplus
};
#endif


#endif /* _PYXIS_PYXIS_H_ */
