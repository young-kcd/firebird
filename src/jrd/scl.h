/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		scl.h
 *	DESCRIPTION:	Security class definitions
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

#ifndef JRD_SCL_H
#define JRD_SCL_H

namespace Jrd {

/* Security class definition */

class SecurityClass : public pool_alloc_rpt<SCHAR, type_scl>
{
    public:
    typedef USHORT flags_t;
	SecurityClass* scl_next;	/* Next security class in system */
	flags_t scl_flags;			/* Access permissions */
	TEXT scl_name[2];
};

const SecurityClass::flags_t SCL_read			= 1;		/* Read access */
const SecurityClass::flags_t SCL_write			= 2;		/* Write access */
const SecurityClass::flags_t SCL_delete			= 4;		/* Delete access */
const SecurityClass::flags_t SCL_control		= 8;		/* Control access */
const SecurityClass::flags_t SCL_grant			= 16;		/* Grant privileges */
const SecurityClass::flags_t SCL_exists			= 32;		/* At least ACL exists */
const SecurityClass::flags_t SCL_scanned		= 64;		/* But we did look */
const SecurityClass::flags_t SCL_protect		= 128;		/* Change protection */
const SecurityClass::flags_t SCL_corrupt		= 256;		/* ACL does look too good */
const SecurityClass::flags_t SCL_sql_insert		= 512;
const SecurityClass::flags_t SCL_sql_delete		= 1024;
const SecurityClass::flags_t SCL_sql_update		= 2048;
const SecurityClass::flags_t SCL_sql_references	= 4096;
const SecurityClass::flags_t SCL_execute		= 8192;



/* information about the user */

class UserId : public pool_alloc_rpt<SCHAR, type_usr>
{
    public:
	const TEXT *usr_user_name;	/* User name */
	TEXT *usr_sql_role_name;	/* Role name */
	TEXT *usr_project_name;		/* Project name */
	TEXT *usr_org_name;			/* Organizaion name */
	TEXT *usr_node_name;		/* Network node name */
	USHORT usr_user_id;			/* User id */
	USHORT usr_group_id;		/* Group id */
	USHORT usr_node_id;			/* Node id */
	USHORT usr_flags;			/* Misc. crud */
	TEXT usr_data[2];
};

const USHORT USR_locksmith	= 1;		/* User has great karma */
const USHORT USR_dba		= 2;		/* User has DBA privileges */
const USHORT USR_owner		= 4;		/* User owns database */

/*
 * User name assigned to any user granted USR_locksmith rights.
 * If this name is changed, modify also the trigger in 
 * jrd/grant.gdl (which turns into jrd/trig.h.
 */
static const char* SYSDBA_USER_NAME	= "SYSDBA";

static const char* object_table		= "TABLE";
static const char* object_procedure	= "PROCEDURE";
static const char* object_column	= "COLUMN";

} //namespace Jrd

#endif // JRD_SCL_H

