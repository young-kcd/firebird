/*
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
CREATE DATABASE "security.fdb" ;

/* Domain definitions */
CREATE DOMAIN COMMENT AS BLOB SUB_TYPE TEXT SEGMENT SIZE 80 CHARACTER SET UNICODE_FSS;
CREATE DOMAIN NAME_PART AS VARCHAR(32) CHARACTER SET UNICODE_FSS DEFAULT _UNICODE_FSS '';
CREATE DOMAIN GID AS INTEGER;
CREATE DOMAIN HOST_KEY AS CHAR(20) CHARACTER SET BINARY;
CREATE DOMAIN HOST_NAME AS CHAR(20) CHARACTER SET ASCII;
CREATE DOMAIN PASSWD AS VARCHAR(32) CHARACTER SET BINARY;
CREATE DOMAIN UID AS INTEGER;
CREATE DOMAIN USER_NAME AS VARCHAR(128) CHARACTER SET ASCII;
CREATE DOMAIN PRIVILEGE AS INTEGER;

/*  Table: HOST_INFO, Owner: BUILDER */
CREATE TABLE HOST_INFO (HOST_NAME HOST_NAME,
	HOST_KEY HOST_KEY);


/*  Table: USERS, Owner: BUILDER */
CREATE TABLE USERS (USER_NAME USER_NAME,
	/* local system user name for setuid for file permissions */
	SYS_USER_NAME	USER_NAME,
	GROUP_NAME	USER_NAME,
	UID 		UID,
	GID 		GID,
	PASSWD 		PASSWD,

	/* Privilege level of user-mark a user as having DBA privilege */
	PRIVILEGE 	PRIVILEGE,

	COMMENT 	COMMENT,
	FIRST_NAME 	NAME_PART,
	MIDDLE_NAME	NAME_PART,
	LAST_NAME	NAME_PART,
	FULL_NAME COMPUTED BY  (first_name || _UNICODE_FSS ' ' || middle_name || _UNICODE_FSS ' ' || last_name ));

/*  Index definitions for all user tables */
CREATE UNIQUE INDEX USER_NAME_INDEX ON USERS(USER_NAME);

GRANT SELECT ON USERS to PUBLIC;
GRANT SELECT ON HOST_INFO to PUBLIC;
