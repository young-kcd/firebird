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

#ifndef _PYXIS_PYXIS_H_
#define _PYXIS_PYXIS_H_

#include "../jrd/common.h"
#include "../pyxis/keys.h"
#include "../pyxis/phase1.h"

#define ALLOCD(type)		PYXIS_alloc (PYXIS_default_pool, type)
#define ALLOCDV(type,repeat)	PYXIS_alloc (PYXIS_default_pool, type, repeat)
#define ALLOCP(type)		PYXIS_alloc (PYXIS_permanent_pool, type)
#define ALLOCPV(type,repeat)	PYXIS_alloc (PYXIS_permanent_pool, type, repeat)
#define IBERROR(string)		PYXIS_error (string)
#define BUGCHECK(string)	PYXIS_bugcheck (string)

#define GET_VALUE(obj, att)	(SLONG) PYXIS_get_attribute_value (obj, att)
#define GET_STRING(obj, att)	(TEXT*) PYXIS_get_attribute_value (obj, att)
#define GET_OBJECT(obj, att)	PYXIS_get_attribute_value (obj, att)
#define GET_ATTRIBUTE(obj, att)	PYXIS_get_attribute (obj, att)
#define REPLACE_ATTRIBUTE	PYXIS_replace_attribute
#define PUT_ATTRIBUTE		PYXIS_put_attribute
#define SET_DISPLAY		PYXIS_set_display

/**
**#define DEBUG			1
**/

#define BLKDEF(type, root, tail) type,
enum blk_t
    {
    type_MIN = 0,
#include "../pyxis/blk.h"
    type_MAX
    };
#undef BLKDEF

/* Block types */

#ifndef INCLUDE_FB_BLK
#include "../include/fb_blk.h"
#endif

typedef struct vec {
    struct blk	vec_header;
    ULONG	vec_count;
    struct blk	*vec_object[1];
} *VEC;

#define LLS_PUSH(object, stack)		PYXIS_push (object, stack)
#define LLS_POP(stack)			PYXIS_pop (stack)

typedef struct lls {
    struct blk	lls_header;
    struct blk	*lls_object;
    struct lls	*lls_next;
} *LLS;

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
    int		(*win_text)();
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

typedef struct map {
    struct blk	map_header;
    WIN		map_window;		/* Active window */
    OBJ		map_form;		/* Associated form */
    OBJ		map_parent;		/* Repeating group parent object */
    OBJ		map_prototype;		/* Prototype for sub-form */
    USHORT	map_count;		/* Number of items */
    USHORT	map_flags;		/* Misc flags */
    USHORT	map_terminator;		/* Field to receive terminator */
    USHORT	map_terminating_field;	/* Field to receive field name */
    USHORT	map_item_index;		/* Field to receive item index */
    LLS		map_context;		/* Context to restart operation */
    USHORT	map_wakeup_char;	/* Wakeup character */
    struct map_repeat
    {
    DSC		map_desc;		/* Descriptor */
    OBJ		map_field;		/* Corresponding form field */
    TEXT	*map_name;		/* Field name */
    USHORT	map_option;		/* Slot for options (if any) */
    USHORT	map_field_flags;	/* Misc crud */
    }		map_rpt [1];
} *MAP;

#define MAP_options		1
#define MAP_display		2
#define MAP_update		4
#define MAP_transparent		8
#define MAP_tag			16

/* Menu block */

typedef struct menu {
    struct blk	menu_header;
    OBJ		menu_object;
    USHORT	menu_flags;
    ATT		menu_current_entree;	/* current entree in dynamic menu */
} *MENU;

/* #define MENU_horizontal		1  moved to object block */
#define MENU_transparent	2

/* Random string block -- jack of all kludges */

typedef struct str {
    struct blk	str_header;
    USHORT	str_length;
    SCHAR	str_data[2];
} *STR;

#endif /* _PYXIS_PYXIS_H_ */
