/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		ExecuteStatement.h
 *	DESCRIPTION:	Dynamic SQL statements execution
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
 * The Original Code was created by Alex Peshkoff, 2003
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 *
 */

#ifndef JRD_DynExec_H
#define JRD_DynExec_H

#include "../jrd/jrd_blks.h"
#include "../include/fb_blk.h"
#include "../include/fb_vector.h"
#include "../jrd/exe.h"
#include "../jrd/y_ref.h"
#include "../jrd/ibase.h"

#define MAX_CALLBACKS	50

class ExecuteStatement {
private:
	FRBRD	* Attachment;
	FRBRD	* Transaction;
	FRBRD	* Statement;
	XSQLDA	* Sqlda;
	SCHAR	* Buffer;
	bool	SingleMode;
	TEXT	* StartOfSqlOperator;
	XSQLDA	* MakeSqlda(TDBB tdbb, SSHORT n);
	ISC_STATUS	ReMakeSqlda(ISC_STATUS *vector, TDBB tdbb);
	ULONG	ParseSqlda(void);
public:
	void Open(TDBB tdbb, jrd_nod* sql, SSHORT nVars, bool SingleTon);
	bool Fetch(TDBB tdbb, jrd_nod** FirstVar);
	void Close(TDBB tdbb);
};


// To be moved to some more appropriate place
template <typename Where>
class AutoPtr {
private:
	Where *ptr;
public:
	inline AutoPtr<Where>(Where *v) {ptr = v;}
	inline operator Where* () {return ptr;}
	inline Where* operator-> () {return ptr;}
	inline ~AutoPtr<Where>() {delete ptr;}
};

#endif // JRD_DynExec_H

