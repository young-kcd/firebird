/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		llio.h
 *	DESCRIPTION:	Low-level I/O routines
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

#ifndef JRD_LLIO_H
#define JRD_LLIO_H

/* I/O macros */

#define LLIO_OPEN_RW		1
#define LLIO_OPEN_WITH_SYNC_RW	2
#define LLIO_OPEN_EXISTING_RW	3
#define LLIO_OPEN_WITH_SYNC_W	4
#define LLIO_OPEN_WITH_TRUNC_RW	5
#define LLIO_OPEN_R		6
#define LLIO_OPEN_NEW_RW        7

#define LLIO_SEEK_NONE		0
#define LLIO_SEEK_BEGIN		1
#define LLIO_SEEK_CURRENT	2
#define LLIO_SEEK_END		3


#endif /* JRD_LLIO_H */
