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

/*	Needed record - with PASSWD = random + SHA1 (random + 'SYSDBA' + crypt('masterke')) */
INSERT INTO RDB$USERS(RDB$USER_NAME, RDB$PASSWD, RDB$FIRST_NAME, RDB$MIDDLE_NAME, RDB$LAST_NAME)
	VALUES ('SYSDBA', 'NLtwcs9LrxLMOYhG0uGM9i6KS7mf3QAKvFVpmRg=', 'Sql', 'Server', 'Administrator');

COMMIT;
