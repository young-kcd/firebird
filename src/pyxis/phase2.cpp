/*
 *	PROGRAM:	PYXIS Form Package
 *	MODULE:		phase2.c
 *	DESCRIPTION:	High level form oriented primitives
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

#include <setjmp.h>
#include "../pyxis/pyxis.h"
#include "../pyxis/phase2.h"

static OBJ walk_form(), walk_repeating();
static ATT find_starting();

extern PIC PICSTR_analyze();
extern OBJ PYXIS_find_field(), PYXIS_get_attribute_value(),
PYXIS_find_index(), PYXIS_update(), PYXIS_menu(),
PYXIS_clone(), PYXIS_create_object();
extern ATT PYXIS_replicate_prototype(), PYXIS_find_object(),
PYXIS_put_attribute(), PYXIS_replace_attribute(),
PYXIS_get_attribute(), PYXIS_navigate();



OBJ PYXIS_find_field(form, field_name)
	 OBJ form;
	 TEXT *field_name;
{
/**************************************
 *
 *	P Y X I S _ f i n d _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Lookup a field in a form definition.  If the field doesn't
 *	exist, return NULL.  If the field name is NULL, assume that
 *	a field handle was supplied in lieu of form handle/field name.
 *
 **************************************/
	ATT attribute;
	OBJ field;
	TEXT *name;

	if (!field_name)
		return form;

	for (attribute = NULL;
		 attribute = PYXIS_find_object(form, attribute, att_field, TRUE);) {
		field = attribute->att_value;
		if (name = GET_STRING(field, att_field_name))
			if (!strcmp(name, field_name))
				return field;
	}

	return NULL;
}


OBJ PYXIS_find_index(form, index)
	 OBJ form;
	 USHORT index;
{
/**************************************
 *
 *	P Y X I S _ f i n d _ i n d e x
 *
 **************************************
 *
 * Functional description
 *	Find a field by index in a repeating form.
 *
 **************************************/
	OBJ field;
	ATT attribute;

	for (attribute = NULL;
		 attribute = PYXIS_find_object(form, attribute, att_field, TRUE);) {
		field = attribute->att_value;
		if (GET_VALUE(field, att_index) == index)
			return field;
	}

	return NULL;
}


TEXT *PYXIS_format_field(field, desc, buffer)
	 OBJ field;
	 DSC *desc;
	 TEXT *buffer;
{
/**************************************
 *
 *	P Y X I S _ f o r m a t _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Given a field and a value, try to edit the value.  If there is
 *	an edit string specified, edit value and return the running 
 *	output address.  If there isn't an edit string, return NULL.
 *
 **************************************/
	ATT attribute;
	DSC desc2;
	PIC picture;
	TEXT *edit_string;

/* Find processed "picture" attribute.  If it exists, look for
   a raw picture string; it this exists, pre-process it.  If, in
   the end, a picture string is found, edit the value using it */

	if (!(picture = (PIC) GET_STRING(field, att_picture))) {
		if (!(edit_string = GET_STRING(field, att_edit_string)))
			return NULL;
		picture = PICSTR_analyze(edit_string, &desc2);
		PUT_ATTRIBUTE(field, att_picture, attype_other, picture);
	}

	PICSTR_edit(desc, picture, &buffer, 32000);
	*buffer = 0;

	return buffer;
}


TEXT *PYXIS_get_string(form, field_name, type)
	 OBJ form;
	 TEXT *field_name;
	 USHORT type;
{
/**************************************
 *
 *	P Y X I S _ g e t _ s t r i n g
 *
 **************************************
 *
 * Functional description
 *	Get a string, any string.  
 *      The latest, of course, the better.             
 *	NOTE: This routine obsolete; 
 *	left in for compatibility during dev't.
 *
 **************************************/
	TEXT *string;
	OBJ field;

	if (!(field = PYXIS_find_field(form, field_name)))	/* 0)))  */
		return NULL;

	switch (type) {
	case PYXIS_user_data:
		return GET_STRING(field, att_update_data);
	case PYXIS_any_data:
		if (string = GET_STRING(field, att_update_data))
			return string;
	case PYXIS_prog_data:
		return GET_STRING(field, att_data);
	default:
		return NULL;
	}

/*if (string = (GET_STRING (field, att_update_data))
    return string;                 

return GET_STRING (field, att_data);*/
}


PYXIS_get_updated(form, field_name, p)
	 OBJ form;
	 TEXT *field_name;
	 TEXT **p;
{
/**************************************
 *
 *	P Y X I S _ g e t _ u p d a t e d
 *
 **************************************
 *
 * Functional description
 *	(This routine is a replacement for PYXIS_gget_string (sic).
 *	PYXIS_gget_string is left in for compatibilitiy during dev't.)
 *	Determine what data is out there and return it.
 *
 **************************************/
	OBJ field;

	*p = NULL;

	if (!(field = PYXIS_find_field(form, field_name)))
		return PYXIS_no_field;

	if (*p = GET_STRING(field, att_update_data))
		return PYXIS_user_data;

	if (GET_VALUE(field, att_data))
		return PYXIS_prog_data;

	return PYXIS_missing_data;
}


PYXIS_purge_segments(field, index)
	 OBJ field;
	 int index;
{
/**************************************
 *
 *	P Y X I S _ p u r g e _ s e g m e n t s
 *
 **************************************
 *
 * Functional description
 *	Purge trailing segments from a blob.
 *
 **************************************/
	ATT attribute, attr;
	OBJ segment;

/* Look for field object already created */

	for (attribute = field->obj_attributes; attr = attribute;) {
		attribute = attribute->att_next;
		if (attr->att_name != att_field)
			continue;
		segment = attr->att_value;
		if (segment->obj_index >= index)
			PYXIS_delete_attribute(field, attr);
	}

	if (index <= 1 && (segment = GET_OBJECT(field, att_prototype)))
		REPLACE_ATTRIBUTE(segment, att_inactive, attype_numeric, FALSE);
}


PYXIS_put_desc(field, desc)
	 OBJ field;
	 DSC *desc;
{
/**************************************
 *
 *	P Y X I S _ p u t _ d e s c
 *
 **************************************
 *
 * Functional description
 *	Establish a value for a field.
 *
 **************************************/
	TEXT *edit_string, *p, edit_buffer[256];
	USHORT l;
	ATT attribute;

	if (!PYXIS_format_field(field, desc, edit_buffer)) {
		p = edit_buffer;
		if (l =
			MOVP_get_string(desc, &edit_string, edit_buffer,
							sizeof(edit_buffer)))
			do
				*p++ = *edit_string++;
			while (--l);
	}

/* Replace the old data attribute */

	attribute =
		REPLACE_ATTRIBUTE(field, att_data, attype_string, edit_buffer);
	PYXIS_set_display_attribute(field, attribute);

	return TRUE;
}


PYXIS_put_segment(field, string, index)
	 OBJ field;
	 TEXT *string;
{
/**************************************
 *
 *	P Y X I S _ p u t _ s e g m e n t
 *
 **************************************
 *
 * Functional description
 *	Add a segment to a blob.
 *
 **************************************/
	ATT attribute;
	OBJ segment;

	if ((segment = PYXIS_find_index(field, index)) &&
		segment->obj_rel_y == index) {
		REPLACE_ATTRIBUTE(segment, att_width, attype_numeric, strlen(string));
		return TRUE;
	}

	if (segment = GET_OBJECT(field, att_prototype)) {
		segment = PYXIS_clone(segment);
		attribute =
			REPLACE_ATTRIBUTE(segment, att_data, attype_string, string);
		PYXIS_set_display_attribute(segment, attribute);
	}
	else
		segment = PYXIS_create_object(string, att_data);

	REPLACE_ATTRIBUTE(segment, att_index, attype_numeric, index);
	REPLACE_ATTRIBUTE(segment, att_display_y, attype_numeric, index - 1);
	PUT_ATTRIBUTE(field, att_field, attype_object, segment);

	return TRUE;
}


ATT PYXIS_replicate_prototype(form, prototype, index)
	 OBJ form;
	 OBJ prototype;
	 USHORT index;
{
/**************************************
 *
 *	P Y X I S _ r e p l i c a t e _ p r o t o t y p e
 *
 **************************************
 *
 * Functional description
 *	Replicate prototype sub-object, 
 *	clearing away extraneous stuff..
 *
 **************************************/
	OBJ clone;
	USHORT height;

	if (!(height = GET_VALUE(prototype, att_height)))
		height = 1;

	PYXIS_scroll_reset(form);
	clone = PYXIS_clone(prototype);

	REPLACE_ATTRIBUTE(clone, att_index, attype_numeric, index);
	REPLACE_ATTRIBUTE(clone, att_display_y, attype_numeric,
					  height * (index - 1));

	REPLACE_ATTRIBUTE(clone, att_updatable_flag, attype_numeric, TRUE);
	REPLACE_ATTRIBUTE(clone, att_inactive, attype_numeric, FALSE);

	return PUT_ATTRIBUTE(form, att_field, attype_object, clone);
}


PYXIS_set_field_options(form, field_name, options, mask)
	 OBJ form;
	 TEXT *field_name;
	 SLONG options, mask;
{
/**************************************
 *
 *	P Y X I S _ s e t _ f i e l d _ o p t i o n s
 *
 **************************************
 *
 * Functional description
 *	Lookup field on form and update field options.  If the field
 *	name is given as "all", update all fields on form.
 *
 **************************************/
	ATT attribute;
	OBJ field;

/* Handle the simple version first */

	if (field_name && !strcmp(field_name, "all")) {
		for (attribute = NULL;
			 attribute = PYXIS_find_object(form, attribute, att_field, TRUE);)
			set_fld_options(attribute->att_value, options, mask);
		return TRUE;
	}

	if (!(field = PYXIS_find_field(form, field_name)))
		return FALSE;

	set_fld_options(field, options, mask);

	return TRUE;
}


OBJ PYXIS_update(window, form, context_stack, terminator)
	 WIN window;
	 OBJ form;
	 LLS *context_stack;
	 USHORT *terminator;
{
/**************************************
 *
 *	P Y X I S _ u p d a t e
 *
 **************************************
 *
 * Functional description
 *	Update stuff.
 *
 **************************************/
	LLS *context, temp;
	OBJ field;

/* If no stack given, make a temp one.  */

	if (!(context = context_stack)) {
		temp = NULL;
		context = &temp;
	}

/* If no context, try to stack the first updatable field.
   If we don't find one, walk_form will find a first.  */

	if (!*context)
		find_starting(form, TRUE, FALSE, context);

	for (;;) {
		*terminator = 0;
		if ((field = walk_form(window, form, context, terminator)) ||
			!*terminator || (*terminator >= C_PF1 && *terminator <= C_ENTER))
			break;
	}

	if ((LLS) context == temp)
		while (temp)
			LLS_POP(&temp);

	return field;
}


static ATT find_starting(form, direction, any, out_stack)
	 OBJ form;
	 USHORT direction;
	 USHORT any;
	 LLS *out_stack;
{
/**************************************
 *
 *	f i n d _ s t a r t i n g
 *
 **************************************
 *
 * Functional description
 *	Find a starting field for form navigation
 *      and stack the fields leading to it.
 *	ASSUME AT LEAST ONE OBJECT (LABEL, FLD) IN A SUBFORM!!
 *
 **************************************/
	ATT attr, start, real_start;
	OBJ field;

/* For forms with say, all text that expect a TERMINATOR, position on first label? */

	start = NULL;
	attr = NULL;

/* During a search for updatables, if a proto has an updatable field but 
   no instantiations, return field but don't stack it.  */

	if (!any) {
		while (attr = start =
			   PYXIS_find_object(form, attr, att_field, direction)) {
			field = attr->att_value;
			if (GET_VALUE(field, att_updatable_flag))
				break;
			if ((field = GET_OBJECT(field, att_prototype)) &&
				(start = find_starting(field, direction, FALSE, out_stack))) {
				/* Hmmm... for recursion, may have to send in a loc stack , 
				   then pop from one to the other */
				LLS_POP(out_stack);
				if ((field = GET_OBJECT(field, att_field)) &&
					(real_start =
					 find_starting(field, direction, FALSE,
								   out_stack))) start = real_start;
				break;
			}
		}
		if (attr)
			LLS_PUSH(attr, out_stack);
		return start;
	}

	while (attr = start = PYXIS_find_object(form, attr, att_field, direction)) {
		field = attr->att_value;
		if (!(field = GET_OBJECT(field, att_prototype)))
			break;
		if (!PYXIS_find_object(field, 0, att_any, direction))
			break;
		if (start = find_starting(field, direction, TRUE, out_stack))
			break;
	}
	if (attr)
		LLS_PUSH(attr, out_stack);
	return start;
}


static set_fld_options(field, options, mask)
	 OBJ field;
	 SLONG options, mask;
{
/**************************************
 *
 *	s e t _ f l d _ o p t i o n s
 *
 **************************************
 *
 * Functional description
 *	Set various field masks.  
 *
 **************************************/
	ATT_N display_attribute;
	ATT attribute;
	USHORT flag;

/* Handle field wake up & updatability flags */

	if (options & PYXIS_wakeup)
		REPLACE_ATTRIBUTE(field, att_wakeup_flag, attype_numeric,
						  (mask & PYXIS_wakeup) ? TRUE : FALSE);

	if (options & PYXIS_updatable) {
		flag = (mask & PYXIS_updatable) ? TRUE : FALSE;
		REPLACE_ATTRIBUTE(field, att_updatable_flag, attype_numeric, flag);
		if (GET_VALUE(field, att_reverse_for_update))
			REPLACE_ATTRIBUTE(field, att_reverse_video, attype_numeric, flag);
	}
/*
	/* Handle automatic upcase flag 
	if (options & PYXIS_upcase)
	    REPLACE_ATTRIBUTE (field, att_upcase, attype_numeric, 
		(mask & PYXIS_upcase) ? TRUE : FALSE);
	/* Handle wakeup string option 
	if (options & PYXIS_wakeup_string & ~mask)
	    PYXIS_delete_named_attribute (field, att_wakeup_string);
*/
/* Handle update data option & program data option */

	if (options & PYXIS_update_present & ~mask)
		PYXIS_delete_named_attribute(field, att_update_data);

	if (options & PYXIS_data_present & ~mask)
		PYXIS_delete_named_attribute(field, att_data);

/* Handle repeating sub-items */

	if (options & PYXIS_repeating_items & ~mask)
		PYXIS_purge_segments(field, 1);

/* Handle various display masks */

	display_attribute = att_any;

	if (options & PYXIS_update_displayed) {
		attribute = NULL;
		if (mask & PYXIS_update_displayed)
			attribute = GET_ATTRIBUTE(field, att_update_data);
		if (!attribute)
			display_attribute = att_data;
		PYXIS_set_display_attribute(field, attribute);
	}

	if (options & PYXIS_data_displayed) {
		attribute = NULL;
		if (mask & PYXIS_data_displayed)
			attribute = GET_ATTRIBUTE(field, att_data);
		if (!attribute)
			display_attribute = att_fill_string;
		PYXIS_set_display_attribute(field, attribute);
	}

	if (options & PYXIS_fill_displayed) {
		attribute = (mask & PYXIS_fill_displayed) ?
			GET_ATTRIBUTE(field, att_fill_string) : NULL;
		if (!attribute)
			display_attribute = att_any;
		PYXIS_set_display_attribute(field, attribute);
	}

	if (display_attribute != att_any &&
		(attribute = GET_ATTRIBUTE(field, display_attribute)))
			PYXIS_set_display_attribute(field, attribute);

/* Recurse where appropriate */

	for (attribute = NULL;
		 attribute = PYXIS_find_object(field, attribute, att_field, TRUE);)
		set_fld_options(attribute->att_value, options, mask);

	for (attribute = NULL;
		 attribute =
		 PYXIS_find_object(field, attribute, att_prototype,
						   TRUE);) set_fld_options(attribute->att_value,
												   options, mask);
}


static OBJ walk_form(window, form, context, terminator)
	 WIN window;
	 OBJ form;
	 LLS *context;
	 USHORT *terminator;
{
/**************************************
 *
 *	w a l k _ f o r m
 *
 **************************************
 *
 * Functional description
 *	Walk around form, updating things updatable.  
 *      If the user tries to move "off" the form, return.
 *
 **************************************/
	ATT field, new_field;
	OBJ field_object, woken_field;
	USHORT lc, direction;

	new_field = NULL;
	direction = TRUE;

/* If there is no first field on the context stack, find one */

	if (*context || find_starting(form, direction, TRUE, context))
		field = (ATT) LLS_POP(context);
	else
		BUGCHECK("Forms sans fields not implemented");;

	for (;;) {
		switch (lc = *terminator) {
		case C_SCROLL_TOP:
			new_field = PYXIS_find_object(form, 0, att_field, TRUE);
			if (new_field == field)
				new_field = NULL;
			break;
		case C_SCROLL_BOTTOM:
			new_field = PYXIS_find_object(form, 0, att_field, FALSE);
			if (new_field == field)
				new_field = NULL;
			break;
		case C_UP:
		case C_DOWN:
		case C_LEFT:
		case C_RIGHT:
		case '\n':
		case '\t':
		case C_DELETE:
			new_field = PYXIS_navigate(form, field, att_field, lc);	/*context, */
			break;
		case C_EOF:			/* should this be same as case n&t ? */
		case C_ENTER:
		case C_PF1:
		case C_PF2:
		case C_PF3:
		case C_PF4:
		case C_PF5:
		case C_PF6:
		case C_PF7:
		case C_PF8:
		case C_PF9:
			new_field = NULL;
			break;
		default:
			new_field = field;
		}

		if (!new_field) {
			LLS_PUSH(field, context);
			return NULL;
		}
		field = new_field;
		field_object = field->att_value;

		if (GET_ATTRIBUTE(field_object, att_prototype)) {
			if (woken_field =
				walk_repeating(window, field_object, context,
							   terminator)) return woken_field;
		}
		else if (GET_VALUE(field_object, att_updatable_flag)) {
			if (PYXIS_get_data
				(window, field_object, att_update_data, form, terminator)
				&& GET_VALUE(field_object, att_wakeup_flag))
				return field_object;
		}
		else
			*terminator = PYXIS_get_char(window, field_object, 0, 0);
	}							/* end for */
}



static OBJ walk_repeating(window, form, context, terminator)
	 WIN window;
	 OBJ form;
	 LLS *context;
	 USHORT *terminator;
{
/**************************************
 *
 *	w a l k _ r e p e a t i n g
 *
 **************************************
 *
 * Functional description
 *	Walk around a repeating group.  Replicate new
 *	elements as required.  If we try to step out of
 *	the group, return.  This is very hairy since if 
 * 	we're walking a subform we mostly just need to 
 * 	choose a repeating group & pass the info cleanly
 * 	to WALK_FORM, but if we're dealing with a blob,
 *	we have to process characters here & so on...
 *
 **************************************/
	ATT attr;
	OBJ object, proto, woken_field;
	USHORT index, max_index, n, replicable, c;

	proto = GET_OBJECT(form, att_prototype);
	replicable = FALSE;
	max_index = 0;
	c = 0;

/* Compute max index & determine replicability */

	for (attr = NULL; attr = PYXIS_find_object(form, attr, att_field, FALSE);) {
		object = attr->att_value;
		n = GET_VALUE(object, att_index);
		max_index = MAX(n, max_index);
	}

	if (GET_VALUE(form, att_updatable_flag))
		replicable = TRUE;
	else
		for (attr = NULL;
			 attr = PYXIS_find_object(proto, attr, att_field, TRUE);) {
			object = attr->att_value;
			if (GET_VALUE(object, att_updatable_flag)) {
				replicable = TRUE;
				break;
			}
		}

/* Find starting index */

	if (!(index = GET_VALUE(form, att_current_index)) && max_index)
		index = 1;

	for (;;) {
		switch (c) {
		case C_SCROLL_TOP:
			if (index > 1) {
				index = 1;
				break;
			}
			REPLACE_ATTRIBUTE(form, att_current_index, attype_numeric, 0);
			*terminator = c;
			return NULL;
		case C_SCROLL_BOTTOM:
			if (index < max_index) {
				index = max_index;
				break;
			}
			REPLACE_ATTRIBUTE(form, att_current_index, attype_numeric,
							  max_index);
			*terminator = c;
			return NULL;
		case C_UP:
		case C_DELETE:
			if (--index > 0)
				break;
			REPLACE_ATTRIBUTE(form, att_current_index, attype_numeric, 0);
			*terminator = c;
			return NULL;
		case C_DOWN:
			if (++index <= max_index)
				break;
			REPLACE_ATTRIBUTE(form, att_current_index, attype_numeric,
							  max_index);
			*terminator = c;
			return NULL;
		case '\n':
		case '\t':
			++index;
			break;
		case C_EOF:
			PYXIS_purge_segments(form, index);
			REPLACE_ATTRIBUTE(form, att_current_index, attype_numeric, index);
			*terminator = 0;
			return NULL;
		case C_LEFT:
		case C_RIGHT:
		case C_ENTER:
		case C_PF1:
		case C_PF2:
		case C_PF3:
		case C_PF4:
		case C_PF5:
		case C_PF6:
		case C_PF7:
		case C_PF8:
		case C_PF9:
			REPLACE_ATTRIBUTE(form, att_current_index, attype_numeric, index);
			*terminator = c;
			return NULL;
		default:;
		}

		if (!index) {
			if (!replicable)
				object = proto;
			else {
				REPLACE_ATTRIBUTE(proto, att_inactive, attype_numeric, TRUE);
				attr =
					PYXIS_replicate_prototype(form, proto,
											  (max_index = ++index));
				object = attr->att_value;
			}
		}
		else if (index > max_index) {
			if (!replicable) {
				REPLACE_ATTRIBUTE(form, att_current_index, attype_numeric,
								  max_index);
				*terminator = c;
				return NULL;
			}
			attr =
				PYXIS_replicate_prototype(form, proto, (max_index = index));
			object = attr->att_value;
		}
		else
			object = PYXIS_find_index(form, index);

		/* Special case for a subform, then a blob */

		if (PYXIS_find_object(object, 0, att_field, TRUE)) {
			if (woken_field = walk_form(window, object, context, &c)) {
				REPLACE_ATTRIBUTE(form, att_current_index, attype_numeric,
								  index);
				*terminator = c;
				return woken_field;
			}
		}
		else if (replicable)
			PYXIS_get_data(window, object, att_update_data, form, &c);
		/* what does it mean to say "waking on" on a blob field? */
		else
			c = PYXIS_get_char(window, object, 0, 0);
	}
}
