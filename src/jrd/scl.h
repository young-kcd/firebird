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

/* Security class definition */

class scl : public pool_alloc_rpt<SCHAR, type_scl>
{
    public:
	scl *scl_next;		/* Next security class in system */
	USHORT scl_flags;			/* Access permissions */
	TEXT scl_name[2];
};
typedef scl *SCL;

#define SCL_read		1		/* Read access */
#define SCL_write		2		/* Write access */
#define SCL_delete		4		/* Delete access */
#define SCL_control		8		/* Control access */
#define SCL_grant		16		/* Grant privileges */
#define SCL_exists		32		/* At least ACL exists */
#define SCL_scanned		64		/* But we did look */
#define SCL_protect		128		/* Change protection */
#define SCL_corrupt		256		/* ACL does look too good */
#define SCL_sql_insert		512
#define SCL_sql_delete		1024
#define SCL_sql_update		2048
#define SCL_sql_references	4096
#define SCL_execute		8192



/* information about the user */

class usr : public pool_alloc_rpt<SCHAR, type_usr>
{
    public:
	TEXT *usr_user_name;		/* User name */
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
typedef usr *USR;

#define USR_locksmith	1		/* User has great karma */
#define USR_dba		2			/* User has DBA privileges */
#define USR_owner	4			/* User owns database */

/*
 * User name assigned to any user granted USR_locksmith rights.
 * If this name is changed, modify also the trigger in 
 * jrd/grant.gdl (which turns into jrd/trig.h.
 */
#define SYSDBA_USER_NAME	"SYSDBA"

#define object_table "TABLE"
#define object_procedure "PROCEDURE"
#define object_column "COLUMN"

#endif /* JRD_SCL_H */
