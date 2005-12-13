/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		acl.h
 *	DESCRIPTION:	Access control list definitions
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

#ifndef _JRD_ACL_H_
#define _JRD_ACL_H_

#define ACL_version	1

#define ACL_end		0
#define ACL_id_list	1
#define ACL_priv_list	2

/* Privileges to be granted */

#define priv_end		0
#define priv_control		1	/* Control over ACL */
#define priv_grant		2		/* Grant subset of rights to others */
#define priv_delete		3		/* Delete object */
#define priv_read		4		/* Read object */
#define priv_write		5		/* Write object */
#define priv_protect		6	/* Change protection */
#define priv_sql_insert		7	/* SQL insertion */
#define priv_sql_delete		8	/* SQL deletion */
#define priv_sql_update		9	/* SQL update */
#define priv_sql_references	10	/* SQL references for foreign key */
#define priv_execute		11	/* Execute (procedure) */
#define priv_max		12

/* Identification criterias */

#define id_end          0
#define id_group        1		/* UNIX group id */
#define id_user         2		/* UNIX user */
#define id_person       3		/* User name */
#define id_project      4		/* Project name */
#define id_organization 5		/* Organization name */
#define id_node         6		/* Node id */
#define id_view         7		/* View name */
#define id_views        8		/* All views */
#define id_trigger      9		/* Trigger name */
#define id_procedure    10		/* Procedure name */
#define id_sql_role     11		/* SQL role */
#define id_max          12

/* Format of access control list: 

	acl		:=	<ACL_version> [ <acl_element> ]... <0>
	acl_element	:=	<ACL_id_list> <id_list> <ACL_priv_list> <priv_list>
	id_list		:=	[ <id_item> ]... <id_end>
	id_item		:=	<id_criteria> <length> [<name>]
	priv_list	:=	[ <privilege> ]... <priv_end>

*/

/* Transaction Description Record */

#define TDR_VERSION		1
#define TDR_HOST_SITE		1
#define TDR_DATABASE_PATH	2
#define TDR_TRANSACTION_ID	3
#define TDR_REMOTE_SITE		4

#endif /* _JRD_ACL_H_ */
