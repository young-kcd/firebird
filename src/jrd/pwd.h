/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		pwd.h
 *	DESCRIPTION:	User information database name
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
/*
$Id: pwd.h,v 1.4 2002-07-06 05:31:56 skywalker Exp $
*/

#ifndef _JRD_PWD_H_
#define _JRD_PWD_H_

#define PASSWORD_SALT		"9z"
#define MAX_PASSWORD_ENC_LENGTH	12


#ifdef VMS
#define USER_INFO_NAME	"[sysmgr]isc4.gdb"
#endif

#if defined(WIN_NT)
#define USER_INFO_NAME	"isc4.gdb"
#endif

#ifdef NETWARE_386
#define USER_INFO_NAME	"SYS:/INTERBAS/ISC4.GDB"
#endif

#ifdef SINIXZ
#define USER_INFO_NAME	"isc4.gdb"
#endif

#ifdef LINUX
#define USER_INFO_NAME	"isc4.gdb"
#endif

#if defined FREEBSD || defined NETBSD
#define USER_INFO_NAME	"isc4.gdb"
#endif

#ifdef DARWIN
#define USER_INFO_NAME	"isc4.gdb"
#endif

#ifdef AIX_PPC
#define USER_INFO_NAME	"isc4.gdb"
#endif

#ifndef USER_INFO_NAME
#ifdef SUPERSERVER
#define USER_INFO_NAME   "isc4.gdb"
#else
#define USER_INFO_NAME   "/usr/interbase/isc4.gdb"
#endif /* SUPERSERVER */
#endif /* USER_INFO_NAME */

#endif /* _JRD_PWD_H_ */
