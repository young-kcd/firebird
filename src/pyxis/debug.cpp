/*
 *	PROGRAM:	PYXIS Form Package
 *	MODULE:		debug.c
 *	DESCRIPTION:	Form debugging stuff
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
 * $Id: debug.cpp,v 1.1.1.1 2001-05-23 13:26:33 tamlin Exp $
 * Revision 1.2  2000/11/24 17:23:02  fsg
 * Another little fix in pyxis/debug.c that nobody
 * ever will notice :-)
 * Will print correct names for this forms stuff now.
 *
 */

#include "../pyxis/pyxis.h"
#include "../pyxis/scr.h"

#define INDENT	2

extern OBJ PYXIS_get_attribute_value();

/* changed "name" to #name to get what was intended
   FSG 24.Nov.2000
*/
#define ATT(name)	#name,
static SCHAR *attribute_names[] = {
	"att_any = 0",
#include "../pyxis/attributes.h"
	"****"
};
#undef ATT

static ATT_N special_attributes[] = {
	att_display_x,
	att_display_y,
	att_scroll_x,
	att_scroll_y,
	att_width,
	att_height,
	att_border,
	att_index,
	att_inactive,
	att_reverse_video,
	att_bold,
	att_underline,
	att_box,
	att_blank,
	att_insert_right,
	att_updatable_flag,
	att_wakeup_flag,
	att_reverse_for_update,
	att_any
};




PYXIS_print(object, level)
	 OBJ object;
	 USHORT level;
{
/**************************************
 *
 *	P Y X I S _ p r i n t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	SSHORT l;
	int value;
	ATT_N *ptr;
	ATT attribute;

	for (l = level; l; --l)
		putchar(' ');

	printf("%x object (%x)\n", object, object->obj_flags);
	level += INDENT;

	for (ptr = special_attributes; *ptr != att_any; ptr++)
		if (value = GET_VALUE(object, *ptr)) {
			for (l = level; l; --l)
				putchar(' ');
			printf("[ %s = %d ]\n", attribute_names[(int) *ptr], value);
		}

	for (attribute = object->obj_attributes; attribute;
		 attribute = attribute->att_next) {
		for (l = level; l; --l)
			putchar(' ');
		printf(attribute_names[(int) attribute->att_name]);
		switch (attribute->att_type) {
		case attype_numeric:
			printf(" = %d\n", attribute->att_value);
			break;

		case attype_string:
			printf(" = \"%s\"\n", attribute->att_value);
			break;

		case attype_other:
			printf(" = %x\n", attribute->att_value);
			break;

		case attype_object:
			printf("\n");
			PYXIS_print(attribute->att_value, level + INDENT);
			break;
		}
	}
}


PYXIS_print_screen(screen, text)
	 SCR screen;
	 UCHAR *text;
{
/**************************************
 *
 *	P Y X I S _ p r i n t _ s c r e e n
 *
 **************************************
 *
 * Functional description
 *	Print a logical screen for debugging purposes.
 *
 **************************************/
	UCHAR buffer[1024], *p, *last;
	USHORT x, y, *data, *end;

	printf("%s\n++++++++++\n", text);

	for (y = 0; y < screen->scr_height; y++) {
		data = screen->scr_screen + y * screen->scr_line_length + 1;
		end = data + screen->scr_width;
		p = last = buffer;
		while (data < end) {
			*p = *data++;
			if (*p++ != ' ')
				last = p;
		}
		*last = 0;
		printf("%s\n", buffer);
	}

	printf("----------\n");
}
