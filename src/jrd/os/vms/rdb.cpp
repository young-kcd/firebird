/*
 *	PROGRAM:	Rdb/GDS Access Method Interface
 *	MODULE:		rdb.cpp
 *	DESCRIPTION:	User visible entrypoints
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
#include "../jrd/y_ref.h"
#include "../jrd/ibase.h"
#include "../jrd/rdb.h"
#include "../jrd/common.h"
#include "../jrd/mblr.h"
#include "../jrd/gds_proto.h"
#include "../jrd/map_proto.h"
#include "../jrd/mblr_proto.h"

#define RDB_IMAGE	"RDBVMSSHR"
#define RDB_CALL(name)	if (!name) find_symbol (&name,"name"); stat = (*name)

#define rdb$bad_req_handle	20480058

#define DBTYPE_ISC	37

/* Transaction element block */

typedef struct teb {
	int *teb_database;
	int teb_tpb_length;
	SCHAR *teb_tpb;
} TEB;

typedef enum {
	impl_rdb = 1,
	impl_gds
} IMPL;

typedef struct handle {
	IMPL implementation;
	int *handle;
	int *messages;
	struct handle *parent;
	struct handle *next;
} *HANDLE, *REQ, *DBB, *TRA, *BLB;

static TRA *dyn_tra_handle;
static ISC_STATUS_ARRAY status_vector;
static UCHAR *temp_buffer;
static SLONG temp_buffer_length;

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
	(*RDB$TRANSACTION_INFO) (),
	(*RDB$RECONNECT_TRANSACTION) (), (*RDB$DDL) (), (*RDB$EXTENSION) ();

static HANDLE allocate_handle(IMPL, int *);
static UCHAR *allocate_temp(int);
static int condition_handler(int *, int *, int *);
static SSHORT dsc_length(struct dsc$descriptor *);
static int find_symbol(int *, UCHAR *);
static void make_desc(UCHAR *, int, struct dsc$descriptor *);
static void putmsg(struct dsc$descriptor *, SCHAR *);
static int set_status(int *, int);

static SLONG codes[] = {
#include "../jrd/rdbcodes.h"
};


int rdb$attach_database(
						int* user_status,
						struct dsc$descriptor* file,
						DBB* handle,
						SSHORT dpb_length, const SCHAR* dpb, SSHORT db_type)
{
/**************************************
 *
 *	r d b $ A T T A C H _ D A T A B A S E
 *
 **************************************
 *
 * Functional description
 *	Create a nice, squeeky clean database, uncorrupted by user data.
 *
 **************************************/
	int *status, stat;
	int len;

/* Try Rdb/VMS first. If it succeeds, everything is ducky */

	if (RDB$ATTACH_DATABASE ||
		find_symbol(&RDB$ATTACH_DATABASE, "RDB$ATTACH_DATABASE")) {
		RDB_CALL(RDB$ATTACH_DATABASE) (user_status, file, handle, dpb_length,
									   dpb);
		if (stat & 1) {
			*handle = allocate_handle(impl_rdb, *handle);
			return stat;
		}
	}

/* Try GDS next.                                               
   Rdb/VMS implemented a new dpb version number, not documented.
   Sense it and ignore the dpb. */

	if (dpb_length && (dpb[0] == 2))
		len = 0;
	else
		len = dpb_length;

	stat = gds__attach_database(status_vector, dsc_length(file),
								file->dsc$a_pointer, handle, len, dpb,
								db_type);

	if (!stat) {
		*handle = allocate_handle(impl_gds, *handle);
		return MAP_status_to_rdb(status_vector, user_status);
	}

/* Neither worked -- return Rdb/VMS status */

	return user_status[1];
}


int rdb$cancel_segmented_string(int *user_status, BLB * blob_handle)
{
/**************************************
 *
 *	r d b $ C A N C E L _ B L O B
 *
 **************************************
 *
 * Functional description
 *	Cancel a blob (surprise!)
 *
 **************************************/
	int stat;

	if (!*blob_handle) {
		*user_status++ = 1;
		*user_status++ = 1;
		return 1;
	}

	if ((*blob_handle)->implementation == impl_gds) {
		gds__cancel_blob(status_vector, &(*blob_handle)->handle);
		stat = MAP_status_to_rdb(status_vector, user_status);
	}
	else {
		RDB_CALL(RDB$CANCEL_SEGMENTED_STRING) (user_status,
											   &(*blob_handle)->handle);
	}

	if (stat & 1) {
		gds__free(*blob_handle);
		*blob_handle = NULL;
	}

	return stat;
}


int rdb$close_segmented_string(int *user_status, BLB * blob_handle)
{
/**************************************
 *
 *	r d b $ _ C L O S E _ B L O B
 *
 **************************************
 *
 * Functional description
 *	Close a blob (surprise!)
 *
 **************************************/
	int stat;

	if (!*blob_handle)
		return set_status(user_status, rdb$_bad_req_handle);

	if ((*blob_handle)->implementation == impl_gds) {
		gds__close_blob(status_vector, &(*blob_handle)->handle);
		stat = MAP_status_to_rdb(status_vector, user_status);
	}
	else {
		RDB_CALL(RDB$CLOSE_SEGMENTED_STRING) (user_status,
											  &(*blob_handle)->handle);
	}

	if (stat & 1) {
		gds__free(*blob_handle);
		*blob_handle = NULL;
	}

	return stat;
}


int rdb$commit_transaction(int *user_status, TRA * tra_handle)
{
/**************************************
 *
 *	r d b $ C O M M I T
 *
 **************************************
 *
 * Functional description
 *	Commit a transaction.
 *
 **************************************/
	int stat;

	if (!*tra_handle)
		return set_status(user_status, rdb$_bad_trans_handle);

	if ((*tra_handle)->implementation == impl_gds) {
		gds__commit_transaction(status_vector, &(*tra_handle)->handle);
		stat = MAP_status_to_rdb(status_vector, user_status);
	}
	else {
		RDB_CALL(RDB$COMMIT_TRANSACTION) (user_status,
										  &(*tra_handle)->handle);
	}

	if (stat & 1) {
		gds__free(*tra_handle);
		*tra_handle = NULL;
	}

	return stat;
}


int rdb$compile_request(
						int* user_status,
						DBB* db_handle,
						REQ* req_handle, SSHORT blr_length, const SCHAR* blr)
{
/**************************************
 *
 *	r d b $ C O M P I L E
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	ISC_STATUS stat;
	int *messages;
	REQ request;
	DBB database;
	USHORT temp_length;
	UCHAR *temp;
	SLONG max_length;

	if (!*db_handle)
		return set_status(user_status, rdb$_bad_db_handle);

	messages = NULL;
	database = *db_handle;
	temp = allocate_temp(blr_length + 200);

	if ((*db_handle)->implementation == impl_gds) {
		if (messages =
			MAP_parse_blr(blr, blr_length, temp, &temp_length, &max_length)) {
			blr = temp;
			blr_length = temp_length;
			allocate_temp(max_length);
		}
		if (gds__compile_request(status_vector, &database->handle,
								 req_handle, blr_length, blr))
			MAP_release(messages);
		stat = MAP_status_to_rdb(status_vector, user_status);
	}
	else {
		RDB_CALL(RDB$COMPILE_REQUEST) (user_status, &database->handle,
									   req_handle, blr_length, blr);
	}

	if (stat & 1) {
		*req_handle = request =
			allocate_handle(database->implementation, *req_handle);
		request->parent = database;
		request->next = database->next;
		database->next = request;
		request->messages = messages;
	}

	return stat;
}


int rdb$create_database(
						int* user_status,
						struct dsc$descriptor* file,
						DBB* handle,
						USHORT dpb_length, const UCHAR* dpb, USHORT db_type)
{
/**************************************
 *
 *	r d b $ C R E A T E _ D A T A B A S E
 *
 **************************************
 *
 * Functional description
 *	Create a nice, squeeky clean database, uncorrupted by user data.
 *
 **************************************/
	int stat;

	if (db_type = DBTYPE_ISC) {
		stat = gds__create_database(status_vector, dsc_length(file),
									file->dsc$a_pointer, handle, dpb_length,
									dpb, db_type);
		if (stat & 1)
			*handle = allocate_handle(impl_gds, *handle);
		return MAP_status_to_rdb(status_vector, user_status);
	}

	else {
		RDB_CALL(RDB$CREATE_DATABASE) (user_status, file, handle,
									   dpb_length, dpb, db_type);
		if (stat & 1)
			*handle = allocate_handle(impl_rdb, *handle);
		return stat;
	}

}


int rdb$create_segmented_string(int *user_status,
								HANDLE * db_handle,
								HANDLE * tra_handle,
								HANDLE * blob_handle, 
								ISC_QUAD* blob_id)
{
/**************************************
 *
 *	r d b $ _ C R E A T E _ B L O B
 *
 **************************************
 *
 * Functional description
 *	Get a segment from a blob (surprise!)
 *
 **************************************/
	int stat;

	if (!*db_handle)
		return set_status(user_status, rdb$_bad_db_handle);

	if ((*db_handle)->implementation == impl_gds) {
		gds__create_blob(status_vector, &(*db_handle)->handle,
						 &(*tra_handle)->handle, blob_handle, blob_id);
		stat = MAP_status_to_rdb(status_vector, user_status);
	}
	else {
		RDB_CALL(RDB$CREATE_SEGMENTED_STRING) (user_status,
											   &(*db_handle)->handle,
											   &(*tra_handle)->handle,
											   blob_handle, blob_id);
	}

	if (stat & 1)
		*blob_handle =
			allocate_handle((*db_handle)->implementation, *blob_handle);

	return stat;
}


int rdb$database_info(
					  int* user_status,
					  DBB* handle,
					  SSHORT item_length,
					  const SCHAR* items, SSHORT buffer_length, SCHAR* buffer)
{
/**************************************
 *
 *	r d b $ d a t a b a s e _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Get info on object.
 *
 **************************************/
	int stat;

	if (!*handle)
		return set_status(user_status, rdb$_bad_db_handle);

	if ((*handle)->implementation == impl_gds) {
		/* Rdb implemented more info items (but didnt' document them)
		   and theirs overlap ours. So, force error on the ones
		   we don't know about.  */

        SCHAR tmp_buff[32];
        SCHAR item;
        
		SCHAR* tmp_ptr = tmp_buff;
		const SCHAR* item_ptr = items;
		const SCHAR* const end = item_ptr + item_length;
		while (item_ptr < end) {
			*tmp_ptr = item = *item_ptr++;
			if (item > isc_info_limbo)
				*tmp_ptr += 200;
			tmp_ptr++;
		}

		gds__database_info(status_vector, &(*handle)->handle,
						   item_length, &tmp_buff, buffer_length, buffer);

		/* Now clean the clumplets all back up again... */

		item_ptr = buffer;
		end = item_ptr + buffer_length;
		while (item_ptr < end) {
			item = *item_ptr++;
			SSHORT len = *item_ptr++;
			len |= (*item_ptr++) << 8;
			if (item == isc_info_error && *item_ptr > 200)
				*item_ptr -= 200;
			else if (item == isc_info_end || item == isc_info_truncated)
				break;
			item_ptr += len;
		}

		stat = MAP_status_to_rdb(status_vector, user_status);
	}
	else {
		RDB_CALL(RDB$DATABASE_INFO) (user_status, &(*handle)->handle,
									 item_length, items, buffer_length,
									 buffer);
	}

	return stat;
}


int rdb$ddl(
			int *user_status,
			DBB * db_handle, UCHAR ddl_operation, int length, UCHAR * buffer)
{
/**************************************
 *
 *	r d b $ d d l
 *
 **************************************
 *
 * Functional description
 *    Support RDB metadata BLR (MBLR) through
 *    translation to InterBase DYN, or pass
 *    it through as before to RDB.
 *
 **************************************/
	int stat, dyn_length, gds_vector[32], *gds;
	UCHAR *dyn_buffer;

	if (!*db_handle)
		return set_status(user_status, rdb$_bad_db_handle);

	if (!dyn_tra_handle)
		return set_status(user_status, rdb$_bad_trans_handle);

	if ((*db_handle)->implementation == impl_gds) {

/* First see if we can in fact translate the MBLR string into a DYN string */

		dyn_buffer = allocate_temp(2 * length);
		if (!MBLR_translate_to_dyn((int) ddl_operation, length, buffer,
								   &dyn_length, dyn_buffer))
			return set_status(user_status, rdb$_wish_list);

/* Finally we can ship off the DYN string built to be processed */

		if (gds__ddl(status_vector, &(*db_handle)->handle,
					 &(*dyn_tra_handle)->handle, dyn_length, dyn_buffer));
		{
			stat = MAP_status_to_rdb(status_vector, user_status);
			return stat;
		}
	}
	else {
		RDB_CALL(RDB$DDL) (user_status, &(*db_handle)->handle,
						   ddl_operation, length, buffer);
	}

	return stat;
}


int rdb$detach_database(int *user_status, DBB * handle)
{
/**************************************
 *
 *	r d b $ D E T A C H
 *
 **************************************
 *
 * Functional description
 *	Close down a database.
 *
 **************************************/
	DBB database;
	REQ request;
	int stat;

	if (!(database = *handle))
		return set_status(user_status, rdb$_bad_db_handle);

	if (database->implementation == impl_gds) {
		gds__detach_database(status_vector, &database->handle);
		stat = MAP_status_to_rdb(status_vector, user_status);
	}
	else {
		RDB_CALL(RDB$DETACH_DATABASE) (user_status, &database->handle);
	}

	if (stat & 1) {
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


int rdb$extension(
				  int *user_status,
				  USHORT index,
				  HANDLE * handle,
				  UCHAR type,
				  USHORT input_length,
UCHAR * input, USHORT result_length, UCHAR * result)
{
/**************************************
 *
 *	r d b $ e x t e n s i o n
 *
 **************************************
 *
 * Functional description
 *	Unimplemented function.
 *
 **************************************/
	int stat;

	if (!*handle)
		return set_status(user_status, rdb$_bad_db_handle);

	if ((*handle)->implementation == impl_gds)
		return set_status(user_status, rdb$_wish_list);
	else {
		RDB_CALL(RDB$EXTENSION) (user_status,
								 index,
								 &(*handle)->handle,
								 type,
								 input_length, input, result_length, result);
	}

	return stat;
}


int rdb$get_segment(
					int *user_status,
					HANDLE * blob_handle,
					SSHORT * length, SSHORT buffer_length, UCHAR * buffer)
{
/**************************************
 *
 *	r d b $ _ G E T _ S E G M E N T
 *
 **************************************
 *
 * Functional description
 *	Get a segment from a blob (surprise!)
 *
 **************************************/
	int stat;

	if (!*blob_handle)
		return set_status(user_status, rdb$_bad_segstr_handle);

	if ((*blob_handle)->implementation == impl_gds) {
		gds__get_segment(status_vector, &(*blob_handle)->handle,
						 length, buffer_length, buffer);
		stat = MAP_status_to_rdb(status_vector, user_status);
	}
	else {
		RDB_CALL(RDB$GET_SEGMENT) (user_status, &(*blob_handle)->handle,
								   length, buffer_length, buffer);
	}

	return stat;
}


int rdb$open_segmented_string(int *user_status,
							  HANDLE * db_handle,
							  HANDLE * tra_handle,
							  HANDLE * blob_handle, 
							  ISC_QUAD* blob_id)
{
/**************************************
 *
 *	r d b $ _ O P E N _ B L O B
 *
 **************************************
 *
 * Functional description
 *	Get a segment from a blob (surprise!)
 *
 **************************************/
	int stat;

	if (!*db_handle)
		return set_status(user_status, rdb$_bad_db_handle);

	if ((*db_handle)->implementation == impl_gds) {
		gds__open_blob(status_vector, &(*db_handle)->handle,
					   &(*tra_handle)->handle, blob_handle, blob_id);
		stat = MAP_status_to_rdb(status_vector, user_status);
	}
	else {
		RDB_CALL(RDB$OPEN_SEGMENTED_STRING) (user_status,
											 &(*db_handle)->handle,
											 &(*tra_handle)->handle,
											 blob_handle, blob_id);
	}

	if (stat & 1)
		*blob_handle =
			allocate_handle((*db_handle)->implementation, *blob_handle);

	return stat;
}


int rdb$put_segment(
					int* user_status,
					BLB* blob_handle, SSHORT buffer_length, const UCHAR* buffer)
{
/**************************************
 *
 *	r d b $ _ P U T _ S E G M E N T
 *
 **************************************
 *
 * Functional description
 *	Put a segment into a blob (surprise!)
 *
 **************************************/
	int stat;

	if (!*blob_handle)
		return set_status(user_status, rdb$_bad_segstr_handle);

	if ((*blob_handle)->implementation == impl_gds) {
		gds__put_segment(status_vector, &(*blob_handle)->handle,
						 buffer_length, buffer);
		stat = MAP_status_to_rdb(status_vector, user_status);
	}
	else {
		RDB_CALL(RDB$PUT_SEGMENT) (user_status, &(*blob_handle)->handle,
								   buffer_length, buffer);
	}

	return stat;
}


int rdb$prepare_transaction(int *user_status, TRA * tra_handle)
{
/**************************************
 *
 *	r d b $ p r e p a r e
 *
 **************************************
 *
 * Functional description
 *	Prepare a transaction for commit.  First phase of a two
 *	phase commit.
 *
 **************************************/
	int stat;

	if (!*tra_handle)
		return set_status(user_status, rdb$_bad_trans_handle);

	if ((*tra_handle)->implementation == impl_gds) {
		gds__prepare_transaction(status_vector, &(*tra_handle)->handle);
		stat = MAP_status_to_rdb(status_vector, user_status);
	}
	else {
		RDB_CALL(RDB$PREPARE_TRANSACTION) (user_status,
										   &(*tra_handle)->handle);
	}

	return stat;
}


int rdb$receive(
				int *user_status,
				REQ * req_handle,
				USHORT msg_type, USHORT msg_length, SCHAR * msg, SSHORT level)
{
/**************************************
 *
 *	r d b $ R E C E I V E
 *
 **************************************
 *
 * Functional description
 *	Get a record from the host program.
 *
 **************************************/
	ISC_STATUS stat;
	REQ request;
	UCHAR *temp;
	int length;

	if (!(request = *req_handle))
		return set_status(user_status, rdb$_bad_req_handle);

	if (request->implementation == impl_gds) {
		if (length = MAP_rdb_length(msg_type, request->messages)) {
			temp = allocate_temp(0);
			gds__receive(status_vector, &request->handle,
						 msg_type, length, temp, level);
			MAP_gds_to_rdb(msg_type, request->messages, temp, msg);
		}
		else
			gds__receive(status_vector, &request->handle,
						 msg_type, msg_length, msg, level);
		stat = MAP_status_to_rdb(status_vector, user_status);
	}
	else {
		RDB_CALL(RDB$RECEIVE) (user_status, &request->handle,
							   msg_type, msg_length, msg, level);
	}

	return stat;
}


int rdb$reconnect_transaction(
							  int* user_status,
							  DBB* db_handle,
							  TRA* tra_handle, SSHORT length, const UCHAR* id)
{
/**************************************
 *
 *	r d b $ r e c o n n e c t _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Reconnect to a transaction in limbo.
 *
 **************************************/
	int stat;

	if (!*db_handle)
		return set_status(user_status, rdb$_bad_db_handle);

	if ((*db_handle)->handle == impl_gds) {
		gds__reconnect_transaction(status_vector,
								   (*db_handle)->handle,
								   (*tra_handle)->handle, length, id);
		return MAP_status_to_rdb(status_vector, user_status);
	}

	RDB_CALL(RDB$RECONNECT_TRANSACTION) (user_status,
										 (*db_handle)->handle,
										 (*tra_handle)->handle, length, id);

	return stat;
}


int rdb$request_info(
					 int* user_status,
					 REQ* handle,
					 SSHORT level,
					 SSHORT item_length,
					 const SCHAR* items, SSHORT buffer_length, SCHAR* buffer)
{
/**************************************
 *
 *	r d b $ r e q u e s t _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Get info on object.
 *
 **************************************/
	int stat;

	if (!*handle)
		return set_status(user_status, rdb$_bad_req_handle);

	if ((*handle)->implementation == impl_gds) {
		gds__request_info(status_vector, &(*handle)->handle,
						  item_length, items, buffer_length, buffer, level);
		stat = MAP_status_to_rdb(status_vector, user_status);
	}
	else {
		RDB_CALL(RDB$REQUEST_INFO) (user_status, &(*handle)->handle,
									item_length, items, buffer_length, buffer,
									level);
	}

	return stat;
}


int rdb$release_request(int *user_status, REQ * req_handle)
{
/**************************************
 *
 *	r d b $ R E L E A S E _ R E Q U E S T
 *
 **************************************
 *
 * Functional description
 *	Release a request.
 *
 **************************************/
	REQ *ptr, request;
	DBB database;
	int stat;

	if (!(request = *req_handle))
		return set_status(user_status, rdb$_bad_req_handle);

	if (request->implementation == impl_gds) {
		gds__release_request(status_vector, &request->handle);
		stat = MAP_status_to_rdb(status_vector, user_status);
	}
	else {
		RDB_CALL(RDB$RELEASE_REQUEST) (user_status, &request->handle);
	}

	if (stat & 1) {
		database = request->parent;
		for (ptr = &database->next; *ptr; ptr = &(*ptr)->next)
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


int rdb$rollback_transaction(int *user_status, TRA * tra_handle)
{
/**************************************
 *
 *	r d b $ R O L L B A C K
 *
 **************************************
 *
 * Functional description
 *	Abort a transaction.
 *
 **************************************/
	int stat;

	if (!*tra_handle)
		return set_status(user_status, rdb$_bad_trans_handle);

	if ((*tra_handle)->implementation == impl_gds) {
		gds__rollback_transaction(status_vector, &(*tra_handle)->handle);
		stat = MAP_status_to_rdb(status_vector, user_status);
	}
	else {
		RDB_CALL(RDB$ROLLBACK_TRANSACTION) (user_status,
											&(*tra_handle)->handle);
	}

	if (stat & 1) {
		gds__free(*tra_handle);
		*tra_handle = NULL;
	}

	return stat;
}


int rdb$segmented_string_info(
							  int* user_status,
							  BLB* handle,
							  SSHORT item_length,
							  const SCHAR* items,
							SSHORT buffer_length, SCHAR* buffer)
{
/**************************************
 *
 *	r d b $ b l o b _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Get info on object.
 *
 **************************************/
	int stat;

	if (!*handle)
		return set_status(user_status, rdb$_bad_segstr_handle);

	if ((*handle)->implementation == impl_gds) {
		gds__blob_info(status_vector, &(*handle)->handle,
					   item_length, items, buffer_length, buffer);
		stat = MAP_status_to_rdb(status_vector, user_status);
	}
	else {
		RDB_CALL(RDB$SEGMENTED_STRING_INFO) (user_status, &(*handle)->handle,
											 item_length, items,
											 buffer_length, buffer);
	}

	return stat;
}


int rdb$send(
			 int *user_status,
			 REQ * req_handle,
			 USHORT msg_type, USHORT msg_length, SCHAR * msg, SSHORT level)
{
/**************************************
 *
 *	r d b $ S E N D
 *
 **************************************
 *
 * Functional description
 *	Get a record from the host program.
 *
 **************************************/
	int length, stat;
	REQ request;
	UCHAR *temp;

	if (!(request = *req_handle))
		return set_status(user_status, rdb$_bad_req_handle);

	if (request->implementation == impl_gds) {
		temp = allocate_temp(0);
		if (length = MAP_rdb_to_gds(msg_type, request->messages, msg, temp)) {
			msg_length = length;
			msg = temp;
		}
		gds__send(status_vector, &request->handle,
				  msg_type, msg_length, msg, level);
		stat = MAP_status_to_rdb(status_vector, user_status);
	}
	else {
		RDB_CALL(RDB$SEND) (user_status, &request->handle,
							msg_type, msg_length, msg, level);
	}

	return stat;
}


int rdb$start_and_send(
					   int *user_status,
					   REQ * req_handle,
					   TRA * tra_handle,
					   USHORT msg_type,
USHORT msg_length, SCHAR * msg, SSHORT level)
{
/**************************************
 *
 *	r d b $ S T A R T _ A N D _ S E N D
 *
 **************************************
 *
 * Functional description
 *	Get a record from the host program.
 *
 **************************************/
	int length, stat;
	REQ request;
	UCHAR *temp;

	if (!(request = *req_handle))
		return set_status(user_status, rdb$_bad_req_handle);

	if ((*req_handle)->implementation == impl_gds) {
		temp = allocate_temp(0);
		if (length = MAP_rdb_to_gds(msg_type, request->messages, msg, temp)) {
			msg_length = length;
			msg = temp;
		}
		gds__start_and_send(status_vector, &request->handle,
							&(*tra_handle)->handle, msg_type, msg_length, msg,
							level);
		stat = MAP_status_to_rdb(status_vector, user_status);
	}
	else {
		RDB_CALL(RDB$START_AND_SEND) (user_status, &request->handle,
									  &(*tra_handle)->handle, msg_type,
									  msg_length, msg, level);
	}

	return stat;
}


int rdb$start_request(
					  int *user_status,
					  REQ * req_handle,
					  TRA * tra_handle, SSHORT level)
{
/**************************************
 *
 *	r d b $ s t a r t _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Get a record from the host program.
 *
 **************************************/
	int stat;

	if (!*req_handle)
		return set_status(user_status, rdb$_bad_req_handle);

	if ((*req_handle)->implementation == impl_gds) {
		gds__start_request(status_vector, &(*req_handle)->handle,
						   &(*tra_handle)->handle, level);
		stat = MAP_status_to_rdb(status_vector, user_status);
	}
	else {
		RDB_CALL(RDB$START_REQUEST) (user_status, &(*req_handle)->handle,
									 &(*tra_handle)->handle, level);
	}

	return stat;
}


int rdb$start_multiple(
					   int *user_status,
					   TRA * tra_handle, SSHORT count, TEB * teb)
{
/**************************************
 *
 *	r d b $ s t a r t _ m u l t i p l e
 *
 **************************************
 *
 * Functional description
 *	Start a transaction.
 *
 **************************************/
}


int rdb$start_transaction(
						  int *user_status,
						  TRA * tra_handle,
						  SSHORT count,
						  DBB * db_handle, SSHORT tpb_length, SCHAR * tpb)
{
/**************************************
 *
 *	r d b $ s t a r t _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Start a transaction.
 *
 **************************************/
	TEB *teb;
	DBB database;
	int stat, gds_vector[32], rdb_vector[32], *gds, *rdb, c;

	if (*tra_handle)
		return set_status(user_status, rdb$_bad_trans_handle);

	gds = gds_vector;
	*gds++ = 0;
	*gds++ = user_status;
	*gds++ = tra_handle;
	*gds++ = 0;

	rdb = rdb_vector;
	*rdb++ = 0;
	*rdb++ = status_vector;
	*rdb++ = tra_handle;
	*rdb++ = 0;

	for (teb = &db_handle, c = 0; c < count; c++, teb++) {
		database = *teb->teb_database;
		if (!database)
			return set_status(user_status, rdb$_bad_db_handle);
		if (database->implementation == impl_gds) {
			++gds_vector[3];
			*gds++ = &database->handle;
			*gds++ = teb->teb_tpb_length;
			*gds++ = teb->teb_tpb;
		}
		else {
			++rdb_vector[3];
			*rdb++ = &database->handle;
			*rdb++ = teb->teb_tpb_length;
			*rdb++ = teb->teb_tpb;
		}
	}

	if (gds_vector[3]) {
		gds_vector[0] = gds - gds_vector - 1;
		stat = lib$callg(gds_vector, gds__start_transaction);
		if (!stat) {
			*tra_handle = allocate_handle(impl_gds, *tra_handle);
			dyn_tra_handle = *tra_handle;
		}
		stat = MAP_status_to_rdb(status_vector, user_status);
	}

	if (rdb_vector[3]) {
		rdb_vector[0] = rdb - rdb_vector - 1;
		find_symbol(&RDB$START_TRANSACTION, "RDB$START_TRANSACTION");
		stat = lib$callg(rdb_vector, RDB$START_TRANSACTION);
		if (stat & 1)
			*tra_handle = allocate_handle(impl_rdb, *tra_handle);
	}

	return stat;
}


int rdb$transaction_info(
						 int* user_status,
						 TRA* handle,
						 SSHORT item_length,
						 const SCHAR* items, SSHORT buffer_length, SCHAR* buffer)
{
/**************************************
 *
 *	r d b $ t r a n s a c t i o n _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Get info on object.
 *
 **************************************/
	int stat;

	if (!*handle)
		return set_status(user_status, rdb$_bad_trans_handle);

	if ((*handle)->implementation == impl_gds) {
		gds__transaction_info(status_vector, &(*handle)->handle,
							  item_length, items, buffer_length, buffer);
		stat = MAP_status_to_rdb(status_vector, user_status);
	}
	else {
		RDB_CALL(RDB$TRANSACTION_INFO) (user_status, &(*handle)->handle,
										item_length, items, buffer_length,
										buffer);
	}

	return stat;
}


int rdb$unwind_request(int *user_status, REQ * req_handle, SSHORT level)
{
/**************************************
 *
 *	r d b $ U N W I N D _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Unwind a running request.
 *
 **************************************/
	int stat;

	if (!*req_handle)
		return set_status(user_status, rdb$_bad_req_handle);

	if ((*req_handle)->implementation == impl_gds) {
		gds__unwind_request(status_vector, &(*req_handle)->handle, level);
		stat = MAP_status_to_rdb(status_vector, user_status);
	}
	else {
		RDB_CALL(RDB$UNWIND_REQUEST) (user_status, &(*req_handle)->handle,
									  level);
	}

	return stat;
}


static HANDLE allocate_handle(IMPL implementation, int *real_handle)
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
	UCHAR *p, *end;
	HANDLE handle;

	handle = gds__alloc((SLONG) sizeof(struct handle));
/* FREE: unknown */
	if (!handle) {				/* NOMEM: crash here */
		DEV_REPORT("allocate_handle: no memory");
		/* NOMEM: not handled, crash here */
	}

	for (p = handle, end = p + sizeof(struct handle); p < end;)
		*p++ = 0;

	handle->implementation = implementation;
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
		/* FREE: unknown - reallocation of buffer handled above */
		/* NOMEM: caller must handle */
		if (temp_buffer)
			temp_buffer_length = length;
		else
			temp_buffer_length = 0;
	}

	return temp_buffer;
}


static int condition_handler(int *sig, int *mech, int *enbl)
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


static SSHORT dsc_length(struct dsc$descriptor *d)
{
/**************************************
 *
 *	d s c _ l e n g t h
 *
 **************************************
 *
 * Functional description
 *	Determine length of argument passed by descriptor
 *
 **************************************/
	struct dsc$descriptor_a *a;

	switch (d->dsc$b_class) {
	case DSC$K_CLASS_A:
		a = d;
		return a->dsc$l_arsize;

	default:
		return d->dsc$w_length;
	}
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



static void putmsg(struct dsc$descriptor_s *desc, SCHAR * arg)
{
/**************************************
 *
 *	p u t m s g
 *
 **************************************
 *
 * Functional description
 *	Get a line of message.
 *
 **************************************/

}


static int set_status(int *status, int code)
{
/**************************************
 *
 *	s e t _ s t a t u s
 *
 **************************************
 *
 * Functional description
 *	Generate an error for a bad handle.
 *
 **************************************/

	*status++ = 1;
	*status++ = code;
	*status = 0;

	return code;
}


#define DUMMY(name) name () {ib_printf ("name entered\n"); return 0;}
DUMMY(rdb$dummy1);

