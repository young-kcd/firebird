/*
 *	PROGRAM:	Object oriented API samples.
 *	MODULE:		01.create.cpp
 *	DESCRIPTION:	A sample of creating new database and new table in it.
 *					Run second time (when database already exists) to see
 *					how FbException is caught and handled by this code.
 *
 *					Example for the following interfaces:
 *					IMaster - main inteface to access all the rest
 *					IStatus - returns the status of executed command
 *					IProvider - main interface to access DB / service
 *					IAttachment - database attachment interface
 *					ITransaction - transaction interface
 *
 *					Run something like this to build: c++ 01.create.cpp -lfbclient
 *
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Alexander Peshkoff
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2013 Alexander Peshkoff <peshkoff@mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "ifaceExamples.h"

// Here we get access to master interface. This is main interface of firebird,
// and the only one for getting which there is special function in our API.
static IMaster* master = fb_get_master_interface();

int main()
{
	int rc = 0;

	// set default password if none specified in environment
	setenv("ISC_USER", "sysdba", 0);
	setenv("ISC_PASSWORD", "masterkey", 0);

	// Declare pointers to required interfaces
	// IStatus is used to return wide error description to user
	IStatus* st = NULL;

	// IProvider is needed to start to work with database (or service)
	IProvider* prov = NULL;

	// IAttachment and ITransaction contain methods to work with database attachment
	// and transactions
	IAttachment* att = NULL;
	ITransaction* tra = NULL;

	try
	{
		// status vector and main dispatcher are returned by calls to IMaster functions
		// no error return may happen - these functions always succeed
		st = master->getStatus();
		prov = master->getDispatcher();

		// status wrapper - will be used later in all calls where status interface is needed
		// With ThrowStatusWrapper passed as status interface FbException will be thrown on error
		ThrowStatusWrapper status(st);

		// create DPB (to be replaced with IPBWriter)
		unsigned char dpbBuf[32];
		unsigned char *dpb = dpbBuf;
		*dpb++ = isc_dpb_version1;
		*dpb++ = isc_dpb_page_size;
		*dpb++ = 2;
		*dpb++ = (8 * 1024) & 0xFF;
		*dpb++ = (8 * 1024) >> 8;

		// create empty database
		att = prov->createDatabase(&status, "fbtests.fdb", dpb - dpbBuf, dpbBuf);
		printf("Database fbtests.fdb created\n");

		// detach from database
		att->detach(&status);
		att = NULL;

		// attach it once again
		att = prov->attachDatabase(&status, "fbtests.fdb", 0, NULL);
		printf("Re-attached database fbtests.fdb\n");

		// start transaction
		tra = att->startTransaction(&status, 0, NULL);

		// create table
		att->execute(&status, tra, 0, "create table dates_table (d1 date)", 3,
			NULL, NULL, NULL, NULL);	// Input parameters and output data not used

		// commit transaction retaining
		tra->commitRetaining(&status);
		printf("Table dates_table created\n");

		// insert a record into dates_table
		att->execute(&status, tra, 0, "insert into dates_table values (CURRENT_DATE)", 3,
			NULL, NULL, NULL, NULL);	// Input parameters and output data not used

		// commit transaction (will close interface)
		tra->commit(&status);
		tra = NULL;

		printf("Record inserted into dates_table\n");

		// detach from database (will close interface)
		att->detach(&status);
		att = NULL;
	}
	catch (const FbException& error)
	{
		// handle error
		rc = 1;
		isc_print_status(error.getStatus()->getErrors());
	}

	// release interfaces after error caught
	if (tra)
		tra->release();
	if (att)
		att->release();
	if (prov)
		prov->release();
	if (st)
		st->dispose();

	return rc;
}
