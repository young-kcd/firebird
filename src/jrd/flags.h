/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		flags.h
 *	DESCRIPTION:	Various flag definitions
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

#ifndef _JRD_FLAGS_H_
#define _JRD_FLAGS_H_

/* flags for RDB$FILE_FLAGS */

#define FILE_shadow	1
#define FILE_inactive	2
#define FILE_manual	4
#define FILE_cache	8
#define FILE_conditional 16

/* flags for RDB$LOG_FILES */

#define LOG_serial      1
#define LOG_default     2
#define LOG_raw         4
#define LOG_overflow    8

/* flags for RDB$RELATIONS */

#define REL_sql		1

/* flags for RDB$TRIGGERS */

#define TRG_sql		0x1
#define TRG_ignore_perm	0x2		/* trigger ignores permissions checks */

#endif /* _JRD_FLAGS_H_ */
