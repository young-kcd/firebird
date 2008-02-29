/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		execute_statement.h
 *	DESCRIPTION:	Dynamic SQL statements execution
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
 *  Copyright (c) 2003 Alexander Peshkoff <peshkoff@mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#ifndef JRD_EXECUTE_STATEMENT_H
#define JRD_EXECUTE_STATEMENT_H

#include "../jrd/jrd_blks.h"
#include "../include/fb_blk.h"
#include "../jrd/exe.h"
#include "../jrd/ibase.h"
#include "../dsql/dsql.h"

const int MAX_CALLBACKS	= 50;

namespace Jrd {

class ExecuteStatement {
private:
	dsql_req* statement;
	Firebird::UCharBuffer* blr;
	Firebird::UCharBuffer* message;
	Firebird::Array<dsc>* values;
	int varCount;
	bool singleMode;
	TEXT startOfSqlOperator[32];

	void generateBlr(const dsc* desc);

public:
	void Open(Jrd::thread_db* tdbb, Jrd::jrd_nod* sql, SSHORT nVars, bool SingleTon);
	bool Fetch(Jrd::thread_db* tdbb, Jrd::jrd_nod** FirstVar);
	void Close(Jrd::thread_db* tdbb);

	static void getString(Jrd::thread_db*, Firebird::string&, const dsc* d, const Jrd::jrd_req* r);
};

} // namespace

#endif // JRD_EXECUTE_STATEMENT_H

