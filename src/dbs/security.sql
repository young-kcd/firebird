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
 * 
 * 2004.09.14 Alex Peshkoff - security changes, preventing ordinary users
 *		from access to other users crypted passwords and enabling modification
 *		of there own password. Originally suggested by Ivan Prenosil
 *		(see http://www.volny.cz/iprenosil/interbase/ for details).
 */

/* Domain definitions */
CREATE DOMAIN COMMENT AS BLOB SUB_TYPE TEXT SEGMENT SIZE 80 CHARACTER SET UNICODE_FSS;
CREATE DOMAIN NAME_PART AS VARCHAR(32) CHARACTER SET UNICODE_FSS DEFAULT _UNICODE_FSS '';
CREATE DOMAIN GID AS INTEGER;
CREATE DOMAIN PASSWD AS VARCHAR(64) CHARACTER SET BINARY;
CREATE DOMAIN UID AS INTEGER;
CREATE DOMAIN USER_NAME AS VARCHAR(128) CHARACTER SET ASCII;
CREATE DOMAIN PRIVILEGE AS INTEGER;


/*  Table: USERS_BG, BG means BackGround */
CREATE TABLE USERS_BG (USER_NAME USER_NAME,
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

COMMIT;

/*  Index definition users_bg table */
CREATE UNIQUE INDEX USER_NAME_INDEX ON USERS_BG(USER_NAME);

/*  View: USERS. Let's existing tools access security.fdb. */
CREATE VIEW USERS (USER_NAME, SYS_USER_NAME, GROUP_NAME, UID, GID, PASSWD,
		PRIVILEGE, COMMENT, FIRST_NAME, MIDDLE_NAME, LAST_NAME, FULL_NAME) AS 
	SELECT USER_NAME, SYS_USER_NAME, GROUP_NAME, UID, GID, PASSWD, 
		PRIVILEGE, COMMENT, FIRST_NAME, MIDDLE_NAME, LAST_NAME, 
		first_name || _UNICODE_FSS ' ' || middle_name || _UNICODE_FSS ' ' || last_name
	FROM USERS_BG
	WHERE CURRENT_USER = 'authenticator'
	   OR CURRENT_USER = 'SYSDBA'
	   OR CURRENT_USER = USERS_BG.USER_NAME;

/*	Access rights */
GRANT ALL ON USERS_BG to VIEW USERS;
GRANT SELECT ON USERS to PUBLIC;
GRANT UPDATE(PASSWD, GROUP_NAME, UID, GID, FIRST_NAME, MIDDLE_NAME, LAST_NAME)
	ON USERS TO PUBLIC;

COMMIT;

/*	Needed record - with PASSWD = random + SHA1 (random + 'SYSDBA' + crypt('masterke')) */
INSERT INTO USERS_BG(USER_NAME, PASSWD, FIRST_NAME, MIDDLE_NAME, LAST_NAME)
	VALUES ('SYSDBA', 'NLtwcs9LrxLMOYhG0uGM9i6KS7mf3QAKvFVpmRg=', 'Sql', 'Server', 'Administrator');

COMMIT;
