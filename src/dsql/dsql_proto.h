/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		dsql_proto.h
 *	DESCRIPTION:	Prototype Header file for dsql.cpp
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

#ifndef DSQL_DSQL_PROTO_H
#define DSQL_DSQL_PROTO_H

#include "../common/classes/fb_string.h"

#ifdef __cplusplus
extern "C" {
#endif

namespace Jrd {
	class Attachment;
	class jrd_tra;
}

class dsql_req;

ISC_STATUS dsql8_allocate_statement(ISC_STATUS*, FB_API_HANDLE*, dsql_req**);
ISC_STATUS dsql8_execute(ISC_STATUS*, FB_API_HANDLE*, dsql_req**,
									   USHORT, const SCHAR*, USHORT, USHORT,
									   SCHAR*, USHORT, SCHAR*, USHORT,
									   USHORT, SCHAR*);
ISC_STATUS dsql8_execute_immediate(ISC_STATUS*, FB_API_HANDLE*, FB_API_HANDLE*,
												 USHORT, const TEXT*, USHORT,
												 USHORT, const SCHAR*, USHORT,
												 USHORT, SCHAR*, USHORT,
												 SCHAR*, USHORT, USHORT,
												 SCHAR*);
#ifdef SCROLLABLE_CURSORS
ISC_STATUS dsql8_fetch(ISC_STATUS*, dsql_req**, USHORT, const SCHAR*,
									 USHORT, USHORT, SCHAR*, USHORT, SLONG);
#else
ISC_STATUS dsql8_fetch(ISC_STATUS*, dsql_req**, USHORT, const SCHAR*,
									 USHORT, USHORT, SCHAR*);
#endif // SCROLLABLE_CURSORS 
ISC_STATUS dsql8_free_statement(ISC_STATUS*, dsql_req**,
											  USHORT);
ISC_STATUS dsql8_insert(ISC_STATUS*, dsql_req**, USHORT,
									  const SCHAR*, USHORT, USHORT, const SCHAR*);
ISC_STATUS dsql8_prepare(ISC_STATUS*, FB_API_HANDLE*, dsql_req**,
									   USHORT, const TEXT*, USHORT, USHORT,
									   const SCHAR*, USHORT, SCHAR*);
ISC_STATUS dsql8_set_cursor(ISC_STATUS*, dsql_req**, const TEXT*,
										  USHORT);
ISC_STATUS dsql8_sql_info(ISC_STATUS*, dsql_req**, USHORT,
										const SCHAR*, USHORT, SCHAR*);

ISC_STATUS callback_execute_immediate(ISC_STATUS* status,
									Jrd::Attachment* jrd_attachment_handle,
									Jrd::jrd_tra* jrd_transaction_handle,
									const Firebird::string& sql_operator);


#ifdef __cplusplus
} // extern "C" 
#endif


#endif //  DSQL_DSQL_PROTO_H

