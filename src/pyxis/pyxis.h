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

#define ALLOCD(type)		PYXIS_alloc (PYXIS_default_pool, type, 1)
#define ALLOCDV(type,repeat)	PYXIS_alloc (PYXIS_default_pool, type, repeat)
#define ALLOCP(type)		PYXIS_alloc (PYXIS_permanent_pool, type, 1)
#define ALLOCPV(type,repeat)	PYXIS_alloc (PYXIS_permanent_pool, type, repeat)
#define IBERROR(string)		PYXIS_error (string)
#define BUGCHECK(string)	PYXIS_bugcheck (string)

#define GET_VALUE(obj, att)	(SLONG) PYXIS_get_attribute_value (obj, att)
#define GET_STRING(obj, att)	(TEXT*) PYXIS_get_attribute_value (obj, att)
#define GET_OBJECT(obj, att)	PYXIS_get_attribute_value (obj, att)
#define GET_ATTRIBUTE(obj, att)	PYXIS_get_attribute (obj, att)
#define REPLACE_ATTRIBUTE(a,b,c,d) PYXIS_replace_attribute((OBJ)(a),b,c,(OBJ)(d))
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

/* Grab all our function prototypes */
#include "../pyxis/pyxis_proto.h"

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

/* Random string block -- jack of all kludges */

typedef struct str {
    struct blk	str_header;
    USHORT	str_length;
    SCHAR	str_data[2];
} *STR;


#endif /* _PYXIS_PYXIS_H_ */
