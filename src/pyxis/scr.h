/*
 *	PROGRAM:	PYXIS Form Package
 *	MODULE:		scr.h
 *	DESCRIPTION:	Logical screen handler definitions
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

#ifndef _PYXIS_SCR_H_
#define _PYXIS_SCR_H_

/* Screen block.  The "screen" image consists of alternating bytes
   of video mask and character data. */

typedef struct scr {
    USHORT	scr_x;			/* Current x location */
    USHORT	scr_y;			/* Current y location */
    USHORT	scr_width;		/* Screen width */
    USHORT	scr_height;		/* Screen height */
    USHORT	scr_length;		/* Bytes in screen image */
    USHORT	scr_line_length;	/* Bytes in line image */
    USHORT	scr_flags;		/* Misc stuff */
    UCHAR	scr_mask;		/* Video options mask */
    USHORT	scr_screen[1];		/* Screen image */
} *SCR;

/* Flags */

#define SCR_clear	1

/* Display attributes */

#define SCR_reverse	1		/* Reverse video */
#define SCR_bold	2		/* Bold characters */
#define SCR_underline	4		/* Underlined text */

extern int SCR_create_window(WIN );
extern int SCR_fini(WIN );
extern int SCR_disable(WIN );
extern int SCR_getchar(WIN , OBJ , int , int );
extern int SCR_refresh_window(WIN , OBJ , int , int );
extern int SCR_clear_window(WIN );
extern int SCR_update_window(WIN , OBJ , int , int );
extern int SCR_clear_object(WIN , OBJ );

#endif /* _PYXIS_SCR_H_ */
