/*
 *	PROGRAM:	Rdb/GDS Access Method Interface
 *	MODULE:		rdbint.cpp
 *	DESCRIPTION:	Rdb/VMS interface
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

#include "firebird.h"
#include descrip
#include ssdef

#include "../jrd/lnmdef.h"
#include "../jrd/ibase.h"
#include "../jrd/common.h"

const char* RDB_IMAGE	= "RDBVMSSHR";
#define RDB_CALL(name)	if (!name) find_symbol (&name, "name"); (*name)
#define CHECK_HANDLE(handle, code)	if (check_handle(handle))\
		 			return bad_handle (user_status, code)

//#define rdb$bad_req_handle	20480058

struct handle {
	int *handle;
	int *messages;
	struct handle *parent;
	struct handle *next;
};

typedef handle *HANDLE;
typedef handle *REQ;
typedef handle *DBB;
typedef handle *HND_TRA;
typedef handle *HND_BLB;

struct teb {
	DBB* teb_database;
	int teb_tpb_length;
	UCHAR *teb_tpb;
};

typedef teb TEB;

static USHORT debug_flags;
static UCHAR *temp_buffer;
static SLONG temp_buffer_length;
static struct handle empty_handle;

// debug_flags
const USHORT DEBUG_BLR			= 1;
const USHORT DEBUG_ERROR_BLR	= 2;

static int (*RDB$ATTACH_DATABASE) (),
	(*RDB$COMMIT_TRANSACTION) (),
	(*RDB$COMPILE_REQUEST) (),
	(*RDB$CREATE_DATABASE) (),
	(*RDB$DETACH_DATABASE) (),
	(*RDB$PREPARE_TRANSACTION) (),
	(*RDB$RECEIVE) (),
	(*RDB$RELEASE_REQUEST) (),
	(*RDB$ROLLBACK_TRANSACTION) (),
	(*RDB$SEND) (),
	(*RDB$START_AND_SEND) (),
	(*RDB$START_REQUEST) (),
	(*RDB$START_TRANSACTION) (),
	(*RDB$UNWIND_REQUEST) (),
	(*RDB$CREATE_SEGMENTED_STRING) (),
	(*RDB$OPEN_SEGMENTED_STRING) (),
	(*RDB$CLOSE_SEGMENTED_STRING) (),
	(*RDB$CANCEL_SEGMENTED_STRING) (),
	(*RDB$GET_SEGMENT) (),
	(*RDB$PUT_SEGMENT) (),
	(*RDB$SEGMENTED_STRING_INFO) (),
	(*RDB$REQUEST_INFO) (),
	(*RDB$DATABASE_INFO) (),
	(*RDB$TRANSACTION_INFO) (), (*RDB$RECONNECT_TRANSACTION) ();

static HANDLE allocate_handle(int *);
static UCHAR *allocate_temp(int);
static int bad_handle(int *, int);
static int check_handle(HANDLE *);
static int condition_handler(int *, int *, int *);
static int find_symbol(int *, UCHAR *);
static void init(void);
static void make_desc(UCHAR *, int, struct dsc$descriptor *);


int RDB_attach_database(
						int* user_status,
						SSHORT file_length,
						const SCHAR* file_name,
						DBB* handle,
						SSHORT dpb_length, const SCHAR* dpb, SSHORT db_type)
{
/**************************************
 *
 *	G D S _ A T T A C H _ D A T A B A S E
 *
 **************************************
 *
 * Functional description
 *	Attempt to attach an RDB database.  Since
 *	we and RDB share some dpb values and have
 *	the same values for different paramters in
 *	other cases.  The results of misunderstandings 
 *      are bad, so we strip off our extended parameters.
 *
 **************************************/
	ISC_STATUS stat;
	ISC_STATUS_ARRAY status_vector;
	struct dsc$descriptor_s name;
	SCHAR new_dpb[128];
	SSHORT new_length;

	init();

	if (!RDB$ATTACH_DATABASE &&
		!find_symbol(&RDB$ATTACH_DATABASE, "RDB$ATTACH_DATABASE")) 
	{
		user_status[0] = isc_arg_gds;
		user_status[1] = gds_unavailable;
		user_status[0] = 0;
		return user_status[1];
	}

	if (dpb_length > 1) {
		char* p = new_dpb;
		const char* q = dpb;
		*p++ = gds_dpb_version1;
		q++;
		for (int l = dpb_length; l; l -= (q - dpb))
			if (*q < gds_dpb_damaged) {
				*p++ = *q++;
				int c_len = *p++ = *q++;
				while (c_len--)
					*p++ = *q++;
			}
			else {
				*q++;
				int c_len = *q++;
				while (c_len--)
					*q++;
			}

		if ((new_length = p - new_dpb) == 1)
			new_length = 0;
	}
	else
		new_length = 0;

	make_desc(file_name, file_length, &name);
	RDB_CALL(RDB$ATTACH_DATABASE) (status_vector, &name, handle, new_length,
								   new_dpb);

	if (!(stat = MAP_status_to_gds(status_vector, user_status)))
		*handle = allocate_handle(*handle);

	return stat;
}


int RDB_blob_info(
				  int* user_status,
				  HND_BLB* handle,
				  SSHORT item_length,
				  SCHAR* items, SSHORT buffer_length, SCHAR* buffer)
{
/**************************************
 *
 *	g d s _ $ b l o b _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Get info on object.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;

	CHECK_HANDLE(handle, gds_bad_segstr_handle);

	RDB_CALL(RDB$SEGMENTED_STRING_INFO) (status_vector, &(*handle)->handle,
										 item_length, items, buffer_length,
										 buffer);

	return MAP_status_to_gds(status_vector, user_status);
}


int RDB_cancel_blob(int *user_status, HND_BLB* blob_handle)
{
/**************************************
 *
 *	R D B _ C A N C E L _ B L O B
 *
 **************************************
 *
 * Functional description
 *	Cancel a blob (surprise!)
 *
 **************************************/
	ISC_STATUS stat;
	ISC_STATUS_ARRAY status_vector;

	if (!*blob_handle) {
		if (user_status) {
			*user_status++ = isc_arg_gds;
			*user_status++ = 0;
		}
		return 0;
	}

	RDB_CALL(RDB$CANCEL_SEGMENTED_STRING) (status_vector,
										   &(*blob_handle)->handle);

	if (!(stat = MAP_status_to_gds(status_vector, user_status))) {
		gds__free(*blob_handle);
		*blob_handle = NULL;
	}

	return stat;
}


int RDB_close_blob(int *user_status, HND_BLB* blob_handle)
{
/**************************************
 *
 *	R D B _ C L O S E _ B L O B
 *
 **************************************
 *
 * Functional description
 *	Close a blob (surprise!)
 *
 **************************************/
	ISC_STATUS stat;
	ISC_STATUS_ARRAY status_vector;

	CHECK_HANDLE(blob_handle, gds_bad_segstr_handle);

	RDB_CALL(RDB$CLOSE_SEGMENTED_STRING) (status_vector,
										  &(*blob_handle)->handle);

	if (!(stat = MAP_status_to_gds(status_vector, user_status))) {
		gds__free(*blob_handle);
		*blob_handle = NULL;
	}

	return stat;
}


int RDB_commit_transaction(int *user_status, HND_TRA* tra_handle)
{
/**************************************
 *
 *	G D S _ C O M M I T
 *
 **************************************
 *
 * Functional description
 *	Commit a transaction.
 *
 **************************************/
	ISC_STATUS stat;
	ISC_STATUS_ARRAY status_vector;

	CHECK_HANDLE(tra_handle, gds_bad_trans_handle);
	RDB_CALL(RDB$COMMIT_TRANSACTION) (status_vector, &(*tra_handle)->handle);

	if (!(stat = MAP_status_to_gds(status_vector, user_status))) {
		gds__free(*tra_handle);
		*tra_handle = NULL;
	}

	return stat;
}


int RDB_compile_request(
						int* user_status,
						DBB* db_handle,
						REQ* req_handle, SSHORT blr_length, const SCHAR* blr)
{
/**************************************
 *
 *	G D S _ C O M P I L E
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	ISC_STATUS stat;
	ISC_STATUS_ARRAY status_vector;

	CHECK_HANDLE(db_handle, gds_bad_db_handle);
	DBB database = *db_handle;
	UCHAR* temp = allocate_temp(blr_length + 200);

	USHORT temp_length;
	SLONG max_length;
	int* messages =
		MAP_parse_blr(blr, blr_length, temp, &temp_length, &max_length);
	if (messages)
	{
		blr = temp;
		blr_length = temp_length;
		allocate_temp(max_length);
	}

	RDB_CALL(RDB$COMPILE_REQUEST) (status_vector, &database->handle,
								   req_handle, blr_length, blr);

	if (stat = MAP_status_to_gds(status_vector, user_status))
		MAP_release(messages);
	else {
		REQ request = allocate_handle(*req_handle);
		*req_handle = request;
		request->messages = messages;
		request->parent = database;
		request->next = database->next;
		database->next = request;
	}

	return stat;
}


int RDB_create_blob(int *user_status,
					HANDLE * db_handle,
					HANDLE * tra_handle,
					HANDLE * blob_handle, 
					ISC_QUAD* blob_id)
{
/**************************************
 *
 *	R D B _ C R E A T E _ B L O B
 *
 **************************************
 *
 * Functional description
 *	Get a segment from a blob (surprise!)
 *
 **************************************/
	ISC_STATUS stat;
	ISC_STATUS_ARRAY status_vector;

	CHECK_HANDLE(db_handle, gds_bad_db_handle);
	CHECK_HANDLE(tra_handle, gds_bad_trans_handle);

	RDB_CALL(RDB$CREATE_SEGMENTED_STRING) (status_vector,
										   &(*db_handle)->handle,
										   &(*tra_handle)->handle,
										   blob_handle, blob_id);

	if (!(stat = MAP_status_to_gds(status_vector, user_status)))
		*blob_handle = allocate_handle(*blob_handle);

	return stat;
}


int RDB_create_database(
						int* user_status,
						USHORT file_length,
						const UCHAR* file_name,
						DBB* handle,
						USHORT dpb_length, const UCHAR* dpb, USHORT db_type)
{
/**************************************
 *
 *	G D S _ C R E A T E _ D A T A B A S E
 *
 **************************************
 *
 * Functional description
 *	Create a nice, squeeky clean database, uncorrupted by user data.
 *
 **************************************/
	ISC_STATUS stat;
	ISC_STATUS_ARRAY status_vector;
	struct dsc$descriptor_s name;

/* Try GDS first; if ok, we're done */

	init();
	if (!RDB$CREATE_DATABASE &&
		!find_symbol(&RDB$CREATE_DATABASE, "RDB$ATTACH_DATABASE")) {
		user_status[0] = isc_arg_gds;
		user_status[1] = gds_unavailable;
		user_status[0] = 0;
		return user_status[1];
	}

	make_desc(file_name, file_length, &name);
	RDB_CALL(RDB$CREATE_DATABASE) (status_vector, &name, handle, dpb_length,
								   dpb, db_type);

	if (!(stat = MAP_status_to_gds(status_vector, user_status)))
		*handle = allocate_handle(*handle);

	return stat;
}


int RDB_database_info(
					  int* user_status,
					  DBB* handle,
					  SSHORT item_length,
					  const SCHAR* items, SSHORT buffer_length, SCHAR* buffer)
{
/**************************************
 *
 *	g d s _ $ d a t a b a s e _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Get info on object.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;
	SCHAR tmp_buff[32];

	CHECK_HANDLE(handle, gds_bad_db_handle);

/* Rdb implemented some new db info items which overlap ours!
   If we're here, we're known to be coming from gds, so force them
   to return error by upping our codes */

	SCHAR* tmp_ptr = tmp_buff;
	const SCHAR* item_ptr = items;
	const SCHAR* const end = item_ptr + item_length;
	while (item_ptr < end) {
		const char item = *item_ptr++;
		*tmp_ptr = item;
		if (item > gds_info_limbo)
			*tmp_ptr += 200;
		tmp_ptr++;
	}

	RDB_CALL(RDB$DATABASE_INFO) (status_vector, &(*handle)->handle,
								 item_length, &tmp_buff, buffer_length,
								 buffer);

/* Now clean the clumplets all back up again... */

	item_ptr = buffer;
	const char* const end2 = item_ptr + buffer_length;
	while (item_ptr < end2) {
		const char item = *item_ptr++;
		SSHORT len = *item_ptr++;
		len |= (*item_ptr++) << 8;
		if (item == gds_info_error && *item_ptr > 200)
			*item_ptr -= 200;
		else if (item == gds_info_end || item == gds_info_truncated)
			break;
		item_ptr += len;
	}

	return MAP_status_to_gds(status_vector, user_status);
}


int RDB_detach_database(int *user_status, DBB* handle)
{
/**************************************
 *
 *	G D S _ D E T A C H
 *
 **************************************
 *
 * Functional description
 *	Close down a database.
 *
 **************************************/
	ISC_STATUS stat;
	ISC_STATUS_ARRAY status_vector;

	CHECK_HANDLE(handle, gds_bad_db_handle);
	DBB database = *handle;

	RDB_CALL(RDB$DETACH_DATABASE) (status_vector, &database->handle);

	if (!(stat = MAP_status_to_gds(status_vector, user_status))) {
		REQ request;
		while (request = database->next) {
			database->next = request->next;
			MAP_release(request->messages);
			gds__free(request);
		}
		gds__free(database);
		*handle = NULL;
	}

	return stat;
}


int RDB_get_segment(
					int *user_status,
					HND_BLB* blob_handle,
					SSHORT * length, SSHORT buffer_length, UCHAR * buffer)
{
/**************************************
 *
 *	R D B _ G E T _ S E G M E N T
 *
 **************************************
 *
 * Functional description
 *	Get a segment from a blob (surprise!)
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;

	CHECK_HANDLE(blob_handle, gds_bad_segstr_handle);

	RDB_CALL(RDB$GET_SEGMENT) (status_vector, &(*blob_handle)->handle,
							   length, buffer_length, buffer);

	return MAP_status_to_gds(status_vector, user_status);
}


int RDB_open_blob(int *user_status,
				  HANDLE * db_handle,
				  HANDLE * tra_handle,
				  HANDLE * blob_handle, 
				  ISC_QUAD* blob_id)
{
/**************************************
 *
 *	R D B _ O P E N _ B L O B
 *
 **************************************
 *
 * Functional description
 *	Get a segment from a blob (surprise!)
 *
 **************************************/
	ISC_STATUS stat;
	ISC_STATUS_ARRAY status_vector;

	CHECK_HANDLE(db_handle, gds_bad_db_handle);
	CHECK_HANDLE(tra_handle, gds_bad_trans_handle);

	RDB_CALL(RDB$OPEN_SEGMENTED_STRING) (status_vector, &(*db_handle)->handle,
										 &(*tra_handle)->handle, blob_handle,
										 blob_id);

	if (!(stat = MAP_status_to_gds(status_vector, user_status)))
		*blob_handle = allocate_handle(*blob_handle);

	return stat;
}


int RDB_prepare_transaction(int *user_status, HND_TRA* tra_handle)
{
/**************************************
 *
 *	G D S _ P R E P A R E
 *
 **************************************
 *
 * Functional description
 *	Prepare a transaction for commit.  First phase of a two
 *	phase commit.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;

	CHECK_HANDLE(tra_handle, gds_bad_trans_handle);
	RDB_CALL(RDB$PREPARE_TRANSACTION) (status_vector, &(*tra_handle)->handle);

	return MAP_status_to_gds(status_vector, user_status);
}


int RDB_put_segment(
					int* user_status,
					HND_BLB* blob_handle, SSHORT buffer_length, const UCHAR* buffer)
{
/**************************************
 *
 *	R D B _ P U T _ S E G M E N T
 *
 **************************************
 *
 * Functional description
 *	Put a segment into a blob (surprise!)
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;

	CHECK_HANDLE(blob_handle, gds_bad_segstr_handle);

	RDB_CALL(RDB$PUT_SEGMENT) (status_vector, &(*blob_handle)->handle,
							   buffer_length, buffer);

	return MAP_status_to_gds(status_vector, user_status);
}


int RDB_receive(
				int *user_status,
				REQ * req_handle,
				USHORT msg_type, USHORT msg_length, SCHAR * msg, SSHORT level)
{
/**************************************
 *
 *	G D S _ R E C E I V E
 *
 **************************************
 *
 * Functional description
 *	Get a record from the host program.  Note
 *	that compile has already wandered through
 *	and made the temporary buffer large enough
 *	for the largest message.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;

	CHECK_HANDLE(req_handle, gds_bad_req_handle);
	UCHAR* temp = allocate_temp(0);

	const int length = MAP_rdb_length(msg_type, (*req_handle)->messages);
	if (length) {
		RDB_CALL(RDB$RECEIVE) (status_vector, &(*req_handle)->handle,
							   msg_type, length, temp, level);
		MAP_rdb_to_gds(msg_type, (*req_handle)->messages, temp, msg);
	}
	else {
		RDB_CALL(RDB$RECEIVE) (status_vector, &(*req_handle)->handle,
							   msg_type, msg_length, msg, level);
	}

	return MAP_status_to_gds(status_vector, user_status);
}


int RDB_reconnect_transaction(
							  int* user_status,
							  DBB* db_handle,
							  HND_TRA* tra_handle, SSHORT length,
							  const UCHAR* id)
{
/**************************************
 *
 *	G D S _ R E C O N N E C T
 *
 **************************************
 *
 * Functional description
 *	Reconnect to a transaction in limbo.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;

	RDB_CALL(RDB$RECONNECT_TRANSACTION) (status_vector,
										 (*db_handle)->handle,
										 (*tra_handle)->handle, length, id);

	return MAP_status_to_gds(status_vector, user_status);
}


int RDB_release_request(int *user_status, REQ * req_handle)
{
/**************************************
 *
 *	G D S _ R E L E A S E _ R E Q U E S T
 *
 **************************************
 *
 * Functional description
 *	Release a request.
 *
 **************************************/
	ISC_STATUS stat;
	ISC_STATUS_ARRAY status_vector;

	CHECK_HANDLE(req_handle, gds_bad_req_handle);

	REQ request = *req_handle;
	RDB_CALL(RDB$RELEASE_REQUEST) (status_vector, &request->handle);

	if (!(stat = MAP_status_to_gds(status_vector, user_status))) {
		DBB database = request->parent;
		for (REQ* ptr = &database->next; *ptr; ptr = &(*ptr)->next)
			if (*ptr == request) {
				*ptr = request->next;
				break;
			}
		MAP_release(request->messages);
		gds__free(request);
		*req_handle = NULL;
	}

	return stat;
}


int RDB_request_info(int* user_status,
					 REQ* handle,
					 SSHORT level,
					 SSHORT item_length,
					 const SCHAR* items, SSHORT buffer_length, SCHAR* buffer)
{
/**************************************
 *
 *	g d s _ $ r e q u e s t _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Get info on object.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;

	CHECK_HANDLE(handle, gds_bad_req_handle);

	RDB_CALL(RDB$REQUEST_INFO) (status_vector, &(*handle)->handle, level,
								item_length, items, buffer_length, buffer);

	return MAP_status_to_gds(status_vector, user_status);
}


int RDB_rollback_transaction(int *user_status, HND_TRA* tra_handle)
{
/**************************************
 *
 *	G D S _ R O L L B A C K
 *
 **************************************
 *
 * Functional description
 *	Abort a transaction.
 *
 **************************************/
	ISC_STATUS stat;
	ISC_STATUS_ARRAY status_vector;

	CHECK_HANDLE(tra_handle, gds_bad_trans_handle);
	RDB_CALL(RDB$ROLLBACK_TRANSACTION) (status_vector,
										&(*tra_handle)->handle);

	if (!(stat = MAP_status_to_gds(status_vector, user_status))) {
		gds__free(*tra_handle);
		*tra_handle = NULL;
	}

	return stat;
}


int RDB_send(
			 int *user_status,
			 REQ * req_handle,
			 USHORT msg_type, USHORT msg_length, SCHAR * msg, SSHORT level)
{
/**************************************
 *
 *	G D S _ S E N D
 *
 **************************************
 *
 * Functional description
 *	Get a record from the host program.  Note that
 *	the compile call has already checked and made the
 *	buffer big enough.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;

	UCHAR* temp = allocate_temp(0);

	const int length = MAP_gds_to_rdb(msg_type, (*req_handle)->messages, msg, temp);
	if (length) {
		msg_length = length;
		msg = temp;
	}

	RDB_CALL(RDB$SEND) (status_vector, &(*req_handle)->handle,
						msg_type, msg_length, msg, level);

	return MAP_status_to_gds(status_vector, user_status);
}


int RDB_start_and_send(
					   int* user_status,
					   REQ* req_handle,
					   HND_TRA* tra_handle,
					   USHORT msg_type,
	USHORT msg_length, SCHAR* msg, SSHORT level)
{
/**************************************
 *
 *	G D S _ S T A R T _ A N D _ S E N D
 *
 **************************************
 *
 * Functional description
 *	Get a record from the host program.  Note
 *	that compile has already made the temporary
 *	buffer large enough for the largest message.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;

	CHECK_HANDLE(tra_handle, gds_bad_trans_handle);
	CHECK_HANDLE(req_handle, gds_bad_req_handle);
	UCHAR* temp = allocate_temp(0);

	const int length = MAP_gds_to_rdb(msg_type, (*req_handle)->messages, msg, temp);
	if (length) {
		msg_length = length;
		msg = temp;
	}

	RDB_CALL(RDB$START_AND_SEND) (status_vector, &(*req_handle)->handle,
								  &(*tra_handle)->handle, msg_type,
								  msg_length, msg, level);

	return MAP_status_to_gds(status_vector, user_status);
}


int RDB_start_request(
					  int* user_status,
					  REQ* req_handle,
					  HND_TRA* tra_handle, SSHORT level)
{
/**************************************
 *
 *	G D S _ S T A R T _ R E Q U E S T
 *
 **************************************
 *
 * Functional description
 *	Get a record from the host program.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;

	CHECK_HANDLE(tra_handle, gds_bad_trans_handle);
	CHECK_HANDLE(req_handle, gds_bad_req_handle);

	RDB_CALL(RDB$START_REQUEST) (status_vector, &(*req_handle)->handle,
								 &(*tra_handle)->handle, level);

	return MAP_status_to_gds(status_vector, user_status);
}


int RDB_start_multiple(
					   int* user_status,
					   HND_TRA* tra_handle, SSHORT count, TEB * teb)
{
/**************************************
 *
 *	G D S _ S T A R T _ M U L T I P L E
 *
 **************************************
 *
 * Functional description
 *	Start a transaction.
 *
 **************************************/
	ISC_STATUS stat;
	ISC_STATUS_ARRAY status_vector;
	int rdb_vector[32];

	if (*tra_handle)
		return bad_handle(user_status, gds_bad_trans_handle);

	int* rdb = rdb_vector;
	*rdb++ = 0;
	*rdb++ = status_vector;
	*rdb++ = tra_handle;
	*rdb++ = 0;

	for (int c = 0; c < count; c++, teb++) {
		DBB database = *teb->teb_database;
		if (!database)
			return bad_handle(user_status, gds_bad_db_handle);
		++rdb_vector[3];
		*rdb++ = &database->handle;
		*rdb++ = teb->teb_tpb_length;
		*rdb++ = teb->teb_tpb;
	}

	rdb_vector[0] = rdb - rdb_vector - 1;
	find_symbol(&RDB$START_TRANSACTION, "RDB$START_TRANSACTION");
	lib$callg(rdb_vector, RDB$START_TRANSACTION);

	if (!(stat = MAP_status_to_gds(status_vector, user_status)))
		*tra_handle = allocate_handle(*tra_handle);

	return stat;
}


int RDB_start_transaction(
						  int* user_status,
						  HND_TRA* tra_handle,
						  SSHORT count,
						  DBB* db_handle, SSHORT tpb_length, SCHAR * tpb)
{
/**************************************
 *
 *	G D S _ S T A R T _ T R A N S A C T I O N
 *
 **************************************
 *
 * Functional description
 *	Start a transaction.
 *
 **************************************/
	ISC_STATUS stat;
	ISC_STATUS_ARRAY status_vector;
	int rdb_vector[32];

	if (*tra_handle)
		return bad_handle(user_status, gds_bad_trans_handle);

	int* rdb = rdb_vector;
	*rdb++ = 0;
	*rdb++ = status_vector;
	*rdb++ = tra_handle;
	*rdb++ = 0;

	int c = 0;
	for (TEB* teb = &db_handle; c < count; c++, teb++) {
		DBB database = *teb->teb_database;
		if (!database)
			return bad_handle(user_status, gds_bad_db_handle);
		++rdb_vector[3];
		*rdb++ = &database->handle;
		*rdb++ = teb->teb_tpb_length;
		*rdb++ = teb->teb_tpb;
	}

	rdb_vector[0] = rdb - rdb_vector - 1;
	find_symbol(&RDB$START_TRANSACTION, "RDB$START_TRANSACTION");
	lib$callg(rdb_vector, RDB$START_TRANSACTION);

	if (!(stat = MAP_status_to_gds(status_vector, user_status)))
		*tra_handle = allocate_handle(*tra_handle);

	return stat;
}


int RDB_transaction_info(
						 int* user_status,
						 HND_TRA* handle,
						 SSHORT item_length,
						 const SCHAR* items,
						 SSHORT buffer_length, SCHAR* buffer)
{
/**************************************
 *
 *	g d s _ $ t r a n s a c t i o n _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Get info on object.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;

	CHECK_HANDLE(handle, gds_bad_trans_handle);

	RDB_CALL(RDB$TRANSACTION_INFO) (status_vector, &(*handle)->handle,
									item_length, items, buffer_length,
									buffer);

	return MAP_status_to_gds(status_vector, user_status);
}


int RDB_unwind_request(int *user_status, REQ * req_handle, SSHORT level)
{
/**************************************
 *
 *	G D S _ U N W I N D _ R E Q U E S T
 *
 **************************************
 *
 * Functional description
 *	Unwind a running request.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;

	CHECK_HANDLE(req_handle, gds_bad_req_handle);

	RDB_CALL(RDB$UNWIND_REQUEST) (status_vector, &(*req_handle)->handle,
								  level);

	return MAP_status_to_gds(status_vector, user_status);
}


static HANDLE allocate_handle(int *real_handle)
{
/**************************************
 *
 *	a l l o c a t e _ h a n d l e
 *
 **************************************
 *
 * Functional description
 *	Allocate an indirect handle.
 *
 **************************************/
	HANDLE handle = gds__alloc((SLONG) sizeof(struct handle));
/* FREE: unknown - user process? */
	if (!handle)				/* NOMEM: not a great handler */
		return NULL;
	*handle = empty_handle;
	handle->handle = real_handle;

	return handle;
}


static UCHAR *allocate_temp(int length)
{
/**************************************
 *
 *	a l l o c a t e _ t e m p
 *
 **************************************
 *
 * Functional description
 *	Allocate a temp of at least a given size.
 *
 **************************************/

	if (length && temp_buffer_length < length) {
		if (temp_buffer)
			gds__free(temp_buffer);
		temp_buffer = gds__alloc((SLONG) length);
		/* FREE: unknown, reallocation handled above */
		/* NOMEM: Caller must handle */
		if (temp_buffer)
			temp_buffer_length = length;
		else
			temp_buffer_length = 0;
	}

	return temp_buffer;
}


static ISC_STATUS bad_handle(ISC_STATUS *user_status, ISC_STATUS code)
{
/**************************************
 *
 *	b a d _ h a n d l e
 *
 **************************************
 *
 * Functional description
 *	Generate an error for a bad handle.
 *
 **************************************/
	ISC_STATUS_ARRAY local_status;
	ISC_STATUS* vector = (user_status) ? user_status : local_status;
	*vector++ = isc_arg_gds;
	*vector++ = code;
	*vector = 0;

	return user_status[1];
}


static int check_handle(HANDLE * handle)
{
/**************************************
 *
 *	c h e c k _ h a n d l e
 *
 **************************************
 *
 * Functional description
 *	Validate a handle.  Return TRUE if it's lousy.
 *
 **************************************/
	HANDLE blk = *handle;
	if (blk)
		return FALSE;

	return TRUE;
}


static int condition_handler(int *sig.int *mech.int *enbl)
{
/**************************************
 *
 *	c o n d i t i o n _ h a n d l e r
 *
 **************************************
 *
 * Functional description
 *	Ignore signal from "lib$find_symbol".
 *
 **************************************/

	return SS$_CONTINUE;
}


static int find_symbol(int *address, UCHAR * name)
{
/**************************************
 *
 *	f i n d _ s y m b o l
 *
 **************************************
 *
 * Functional description
 *	Look up entrypoint into RDB image.
 *
 **************************************/
	struct dsc$descriptor file, symbol;

	VAXC$ESTABLISH(condition_handler);
	make_desc(RDB_IMAGE, 0, &file);
	make_desc(name, 0, &symbol);
	lib$find_image_symbol(&file, &symbol, address);

	return *address;
}


static void init(void)
{
/**************************************
 *
 *	i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize module.  Mostly just look up the
 *	Rdb/VMS debug flags.
 *
 **************************************/
	USHORT l;
	struct dsc$descriptor name, value;

	make_desc("RDMS$DEBUG_FLAGS", 0, &name);
	make_desc(buffer, sizeof(buffer), &value);
	const int status = lib$sys_trnlog(&name, &l, &value);

	if (status != SS$_NORMAL)
		return;

	UCHAR buffer[16];
	for (const UCHAR* p = buffer; l; --l)
		switch (*p++) {
		case 'B':
			debug_flags |= DEBUG_BLR;
			break;

		case 'E':
			debug_flags |= DEBUG_ERROR_BLR;
			break;
		}
}


static void make_desc(
					  UCHAR * string, int length, struct dsc$descriptor *desc)
{
/**************************************
 *
 *	m a k e _ d e s c
 *
 **************************************
 *
 * Functional description
 *	Fill in VMS descriptor.
 *
 **************************************/

	desc->dsc$b_dtype = DSC$K_DTYPE_T;
	desc->dsc$b_class = DSC$K_CLASS_S;
	desc->dsc$a_pointer = string;

	if (!(desc->dsc$w_length = length))
		desc->dsc$w_length = strlen(string);
}

