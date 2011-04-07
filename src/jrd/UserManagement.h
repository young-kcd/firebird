/*
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
 *  The Original Code was created by Alex Peshkov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2008 Alex Peshkov <alexpeshkoff@users.sf.net>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#ifndef JRD_USER_MANAGEMENT_H
#define JRD_USER_MANAGEMENT_H

#include "firebird.h"
#include "../common/classes/array.h"
#include "../common/classes/fb_string.h"
#include "../jrd/ibase.h"
#include "../jrd/DatabaseSnapshot.h"
#include "../jrd/recsrc/RecordSource.h"
#include "../auth/AuthInterface.h"
#include "../common/security.h"

namespace Jrd {

class thread_db;
class jrd_tra;
class RecordBuffer;

class UsersTableScan: public VirtualTableScan
{
public:
	UsersTableScan(CompilerScratch* csb, const Firebird::string& name, UCHAR stream)
		: VirtualTableScan(csb, name, stream)
	{}

protected:
	const Format* getFormat(thread_db* tdbb, jrd_rel* relation) const;
	bool retrieveRecord(thread_db* tdbb, jrd_rel* relation, FB_UINT64 position, Record* record) const;
};

// User management argument for deferred work
class UserManagement : public DataDump
{
public:
	class Display : public Firebird::StackIface<Auth::ListUsers>
	{
	public:
		explicit Display(UserManagement* um)
			: userManagement(um)
		{ }

		void FB_CARG list(Auth::User* user);

	private:
		UserManagement* userManagement;
	};

	explicit UserManagement(jrd_tra* tra);
	~UserManagement();

	// store userData for DFW-time processing
	USHORT put(Auth::UserData* userData);
	// execute command with ID
	void execute(USHORT id);
	// commit transaction in security database
	void commit();
	// return users list for SEC$USERS
	RecordBuffer* getList(thread_db* tdbb, jrd_rel* relation);
	// callback for users display
	void list(Auth::User* u);

private:
	RecordBuffer* buffer;
	thread_db* threadDbb;
	Firebird::HalfStaticArray<Auth::UserData*, 8> commands;
	Display display;
	Auth::Management* manager;

	static void checkSecurityResult(int errcode, Firebird::IStatus* status, const char* userName);
};

}	// namespace

#endif // JRD_USER_MANAGEMENT_H
