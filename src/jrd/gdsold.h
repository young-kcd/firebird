/*
 *	PROGRAM:	C preprocessor
 *	MODULE:		gds.h
 *	DESCRIPTION:	BLR constants
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

#ifndef _JRD_GDSOLD_H_
#define _JRD_GDSOLD_H_

#define gds_version3

#define GDS_TRUE	1
#define GDS_FALSE	0
#if !(defined __cplusplus)
#define GDS__TRUE	GDS_TRUE
#define GDS__FALSE	GDS_FALSE
#endif

#if (defined __osf__ && defined __alpha)
#define GDS_LONG	int
#define GDS_ULONG	unsigned int
#else
#define GDS_LONG	long
#define GDS_ULONG	unsigned long
#endif

#define GDS_STATUS	long

#define GDS_VAL(val)	val
#define GDS_REF(val)	&val
#define GDS_TYPE	GDS_STATUS

#define CANCEL_disable	1
#define CANCEL_enable	2
#define CANCEL_raise	3

/******************************************************************/
/* define type, export and other stuff based on c/c++ and Windows */
/******************************************************************/

#define  GDS_FAR	ISC_FAR
#define  GDS_EXPORT     ISC_EXPORT

typedef void GDS_FAR *gds_db_handle;
typedef void GDS_FAR *gds_req_handle;
typedef void GDS_FAR *gds_tr_handle;
typedef void GDS_FAR *gds_blob_handle;
typedef void GDS_FAR *gds_win_handle;
typedef void GDS_FAR *gds_form_handle;
typedef void GDS_FAR *gds_stmt_handle;
typedef void GDS_FAR *gds_svc_handle;
typedef void (GDS_FAR * gds_callback) ();


/*************************/
/* Old OSRI entrypoints  */
/*************************/

#ifndef NO_OSRI_ENTRYPOINTS

/* #ifndef __cplusplus */

#ifdef __cplusplus
extern "C" {
typedef GDS_QUAD GDS__QUAD;
#endif

#if defined(__STDC__) || defined(_Windows) || \
    (defined(_MSC_VER) && defined(WIN32)) || defined(_WINDOWS) || \
    (defined(__BORLANDC__) && (defined(__WIN32__) )) || \
    defined(AIX_PPC) || \
	defined(__cplusplus)


GDS_STATUS GDS_EXPORT gds__attach_database(GDS_STATUS GDS_FAR *,
										   short,
										   char GDS_FAR *,
										   void GDS_FAR * GDS_FAR *,
										   short,
										   char GDS_FAR *);

GDS_STATUS GDS_EXPORT gds__blob_info(GDS_STATUS GDS_FAR *,
									 void GDS_FAR * GDS_FAR *,
									 short,
									 char GDS_FAR *,
									 short,
									 char GDS_FAR *);

GDS_STATUS GDS_EXPORT gds__cancel_blob(GDS_STATUS GDS_FAR *,
									   void GDS_FAR * GDS_FAR *);

GDS_STATUS GDS_EXPORT gds__close_blob(GDS_STATUS GDS_FAR *,
									  void GDS_FAR * GDS_FAR *);

GDS_STATUS GDS_EXPORT gds__commit_transaction(GDS_STATUS GDS_FAR *,
											  void GDS_FAR * GDS_FAR *);

GDS_STATUS GDS_EXPORT gds__compile_request(GDS_STATUS GDS_FAR *,
										   void GDS_FAR * GDS_FAR *,
										   void GDS_FAR * GDS_FAR *,
										   short,
										   char GDS_FAR *);

GDS_STATUS GDS_EXPORT gds__compile_request2(GDS_STATUS GDS_FAR *,
											void GDS_FAR * GDS_FAR *,
											void GDS_FAR * GDS_FAR *,
											short,
											char GDS_FAR *);

GDS_STATUS GDS_EXPORT gds__create_blob(GDS_STATUS GDS_FAR *,
									   void GDS_FAR * GDS_FAR *,
									   void GDS_FAR * GDS_FAR *,
									   void GDS_FAR * GDS_FAR *,
									   GDS__QUAD GDS_FAR *);

GDS_STATUS GDS_EXPORT gds__create_blob2(GDS_STATUS GDS_FAR *,
										void GDS_FAR * GDS_FAR *,
										void GDS_FAR * GDS_FAR *,
										void GDS_FAR * GDS_FAR *,
										GDS__QUAD GDS_FAR *,
										short,
										char GDS_FAR *);

GDS_STATUS GDS_EXPORT gds__create_database(GDS_STATUS GDS_FAR *,
										   short,
										   char GDS_FAR *,
										   void GDS_FAR * GDS_FAR *,
										   short,
										   char GDS_FAR *,
										   short);

GDS_STATUS GDS_EXPORT gds__database_info(GDS_STATUS GDS_FAR *,
										 void GDS_FAR * GDS_FAR *,
										 short,
										 char GDS_FAR *,
										 short,
										 char GDS_FAR *);

void GDS_EXPORT gds__decode_date(GDS__QUAD GDS_FAR *,
								 void GDS_FAR *);

GDS_STATUS GDS_EXPORT gds__detach_database(GDS_STATUS GDS_FAR *,
										   void GDS_FAR * GDS_FAR *);

GDS_ULONG GDS_EXPORT gds__free(void GDS_FAR *);

GDS_STATUS GDS_EXPORT gds__get_segment(GDS_STATUS GDS_FAR *,
									   void GDS_FAR * GDS_FAR *,
									   unsigned short GDS_FAR *,
									   unsigned short,
									   char GDS_FAR *);

GDS_STATUS GDS_EXPORT gds__open_blob(GDS_STATUS GDS_FAR *,
									 void GDS_FAR * GDS_FAR *,
									 void GDS_FAR * GDS_FAR *,
									 void GDS_FAR * GDS_FAR *,
									 GDS__QUAD GDS_FAR *);

GDS_STATUS GDS_EXPORT gds__open_blob2(GDS_STATUS GDS_FAR *,
									  void GDS_FAR * GDS_FAR *,
									  void GDS_FAR * GDS_FAR *,
									  void GDS_FAR * GDS_FAR *,
									  GDS__QUAD GDS_FAR *,
									  short,
									  char GDS_FAR *);

GDS_STATUS GDS_EXPORT gds__prepare_transaction(GDS_STATUS GDS_FAR *,
											   void GDS_FAR * GDS_FAR *);

GDS_STATUS GDS_EXPORT gds__prepare_transaction2(GDS_STATUS GDS_FAR *,
												void GDS_FAR * GDS_FAR *,
												short,
												char GDS_FAR *);

GDS_STATUS GDS_EXPORT gds__put_segment(GDS_STATUS GDS_FAR *,
									   void GDS_FAR * GDS_FAR *,
									   unsigned short,
									   char GDS_FAR *);

GDS_STATUS GDS_EXPORT gds__receive(GDS_STATUS GDS_FAR *,
								   void GDS_FAR * GDS_FAR *,
								   short,
								   short,
								   void GDS_FAR *,
								   short);

GDS_STATUS GDS_EXPORT gds__reconnect_transaction(GDS_STATUS GDS_FAR *,
												 void GDS_FAR * GDS_FAR *,
												 void GDS_FAR * GDS_FAR *,
												 short,
												 char GDS_FAR *);

GDS_STATUS GDS_EXPORT gds__request_info(GDS_STATUS GDS_FAR *,
										void GDS_FAR * GDS_FAR *,
										short,
										short,
										char GDS_FAR *,
										short,
										char GDS_FAR *);

GDS_STATUS GDS_EXPORT gds__release_request(GDS_STATUS GDS_FAR *,
										   void GDS_FAR * GDS_FAR *);

GDS_STATUS GDS_EXPORT gds__rollback_transaction(GDS_STATUS GDS_FAR *,
												void GDS_FAR * GDS_FAR *);

GDS_STATUS GDS_EXPORT gds__seek_blob(GDS_STATUS GDS_FAR *,
									 void GDS_FAR * GDS_FAR *,
									 short,
									 GDS_LONG,
									 GDS_LONG GDS_FAR *);

GDS_STATUS GDS_EXPORT gds__send(GDS_STATUS GDS_FAR *,
								void GDS_FAR * GDS_FAR *,
								short,
								short,
								void GDS_FAR *,
								short);

void GDS_EXPORT gds__set_debug(int);

GDS_STATUS GDS_EXPORT gds__start_and_send(GDS_STATUS GDS_FAR *,
										  void GDS_FAR * GDS_FAR *,
										  void GDS_FAR * GDS_FAR *,
										  short,
										  short,
										  void GDS_FAR *,
										  short);

GDS_STATUS GDS_EXPORT gds__start_multiple(GDS_STATUS GDS_FAR *,
										  void GDS_FAR * GDS_FAR *,
										  short,
										  void GDS_FAR *);

GDS_STATUS GDS_EXPORT gds__start_request(GDS_STATUS GDS_FAR *,
										 void GDS_FAR * GDS_FAR *,
										 void GDS_FAR * GDS_FAR *,
										 short);

GDS_STATUS GDS_EXPORT gds__start_transaction(GDS_STATUS GDS_FAR *,
											 void GDS_FAR * GDS_FAR *,
											 short, ...);

GDS_STATUS GDS_EXPORT gds__transaction_info(GDS_STATUS GDS_FAR *,
											void GDS_FAR * GDS_FAR *,
											short,
											char GDS_FAR *,
											short,
											char GDS_FAR *);

GDS_STATUS GDS_EXPORT gds__unwind_request(GDS_STATUS GDS_FAR *,
										  void GDS_FAR * GDS_FAR *,
										  short);

GDS_LONG GDS_EXPORT gds__ftof(char GDS_FAR *,
							  unsigned short,
							  char GDS_FAR *,
							  unsigned short);

void GDS_EXPORT gds__vtov(CONST SCHAR GDS_FAR*,
						  SCHAR GDS_FAR *,
						  SSHORT);

int GDS_EXPORT gds__version(void GDS_FAR * GDS_FAR *,
							void (GDS_FAR *) (),
							void GDS_FAR *);

int GDS_EXPORT gds__disable_subsystem(char GDS_FAR *);

int GDS_EXPORT gds__enable_subsystem(char GDS_FAR *);

GDS_STATUS GDS_EXPORT gds__print_status(GDS_STATUS GDS_FAR *);

GDS_LONG GDS_EXPORT gds__sqlcode(GDS_STATUS GDS_FAR *);

GDS_STATUS GDS_EXPORT gds__ddl(GDS_STATUS GDS_FAR *,
							   void GDS_FAR * GDS_FAR *,
							   void GDS_FAR * GDS_FAR *,
							   short,
							   char GDS_FAR *);

GDS_STATUS GDS_EXPORT gds__commit_retaining(GDS_STATUS GDS_FAR *,
											void GDS_FAR * GDS_FAR *);

void GDS_EXPORT gds__encode_date(void GDS_FAR *,
								 GDS__QUAD GDS_FAR *);

GDS_STATUS GDS_EXPORT gds__que_events(GDS_STATUS GDS_FAR *,
									  void GDS_FAR * GDS_FAR *,
									  GDS_LONG GDS_FAR *,
									  short,
									  char GDS_FAR *,
									  void (GDS_FAR *) (),
									  void GDS_FAR *);

GDS_STATUS GDS_EXPORT gds__cancel_events(GDS_STATUS GDS_FAR *,
										 void GDS_FAR * GDS_FAR *,
										 GDS_LONG GDS_FAR *);

GDS_STATUS GDS_EXPORT gds__event_wait(GDS_STATUS GDS_FAR *,
									  void GDS_FAR * GDS_FAR *,
									  short,
									  char GDS_FAR *,
									  char GDS_FAR *);

void GDS_EXPORT gds__event_counts(unsigned GDS_LONG GDS_FAR *,
								  short,
								  char GDS_FAR *,
								  char GDS_FAR *);

GDS_LONG GDS_EXPORT gds__event_block(char GDS_FAR * GDS_FAR *,
									 char GDS_FAR * GDS_FAR *,
									 unsigned short, ...);

GDS_STATUS GDS_EXPORT gds__get_slice(GDS_STATUS GDS_FAR *,
									 void GDS_FAR * GDS_FAR *,
									 void GDS_FAR * GDS_FAR *,
									 GDS__QUAD GDS_FAR *,
									 short,
									 char GDS_FAR *,
									 short,
									 GDS_LONG GDS_FAR *,
									 GDS_LONG,
									 void GDS_FAR *,
									 GDS_LONG GDS_FAR *);

GDS_STATUS GDS_EXPORT gds__put_slice(GDS_STATUS GDS_FAR *,
									 void GDS_FAR * GDS_FAR *,
									 void GDS_FAR * GDS_FAR *,
									 GDS__QUAD GDS_FAR *,
									 short,
									 char GDS_FAR *,
									 short,
									 GDS_LONG GDS_FAR *,
									 GDS_LONG,
									 void GDS_FAR *);

void GDS_EXPORT gds__vtof(char GDS_FAR *,
						  char GDS_FAR *,
						  unsigned short);

#else /* __STDC__ && stuff */



#ifndef _Windows
#ifndef _WINDOWS
GDS_TYPE GDS_EXPORT gds__attach_database();
GDS_TYPE GDS_EXPORT gds__blob_info();
GDS_TYPE GDS_EXPORT gds__cancel_blob();
GDS_TYPE GDS_EXPORT gds__close_blob();
GDS_TYPE GDS_EXPORT gds__commit_transaction();
GDS_TYPE GDS_EXPORT gds__compile_request();
GDS_TYPE GDS_EXPORT gds__compile_request2();
GDS_TYPE GDS_EXPORT gds__create_blob();
GDS_TYPE GDS_EXPORT gds__create_blob2();
GDS_TYPE GDS_EXPORT gds__create_database();
GDS_TYPE GDS_EXPORT gds__database_info();
GDS_TYPE GDS_EXPORT gds__detach_database();
GDS_TYPE GDS_EXPORT gds__get_segment();
GDS_TYPE GDS_EXPORT gds__open_blob();
GDS_TYPE GDS_EXPORT gds__open_blob2();
GDS_TYPE GDS_EXPORT gds__prepare_transaction();
GDS_TYPE GDS_EXPORT gds__prepare_transaction2();
GDS_TYPE GDS_EXPORT gds__put_segment();
GDS_TYPE GDS_EXPORT gds__receive();
GDS_TYPE GDS_EXPORT gds__reconnect_transaction();
GDS_TYPE GDS_EXPORT gds__request_info();
GDS_TYPE GDS_EXPORT gds__release_request();
GDS_TYPE GDS_EXPORT gds__rollback_transaction();
GDS_TYPE GDS_EXPORT gds__seek_blob();
GDS_TYPE GDS_EXPORT gds__send();
GDS_TYPE GDS_EXPORT gds__start_and_send();
GDS_TYPE GDS_EXPORT gds__start_multiple();
GDS_TYPE GDS_EXPORT gds__start_request();
GDS_TYPE GDS_EXPORT gds__start_transaction();
GDS_TYPE GDS_EXPORT gds__transaction_info();
GDS_TYPE GDS_EXPORT gds__unwind_request();
GDS_TYPE GDS_EXPORT gds__ftof();
GDS_TYPE GDS_EXPORT gds__print_status();
GDS_TYPE GDS_EXPORT gds__sqlcode();
GDS_TYPE GDS_EXPORT gds__ddl();
GDS_TYPE GDS_EXPORT gds__commit_retaining();
GDS_TYPE GDS_EXPORT gds__que_events();
GDS_TYPE GDS_EXPORT gds__cancel_events();
GDS_TYPE GDS_EXPORT gds__event_wait();
GDS_TYPE GDS_EXPORT gds__event_block();
GDS_TYPE GDS_EXPORT gds__get_slice();
GDS_TYPE GDS_EXPORT gds__put_slice();
GDS_TYPE GDS_EXPORT gds__seek_blob();

void GDS_EXPORT gds__event_counts();
void GDS_EXPORT gds__set_debug();
void GDS_EXPORT gds__vtof();


#endif /* _WINDOWS */

#endif /* _Windows */

#endif /* __STDC__ && stuff */

#ifdef __cplusplus
} /* extern "C" */
#endif

/* #endif *//* __cplusplus */

#endif /* NO_OSRI_ENTRYPOINTS */




/**********************************/
/* Database parameter block stuff */
/**********************************/

#ifndef	__cplusplus				/* c definitions */

#define gds__dpb_version1                  1
#define gds__dpb_cdd_pathname              1
#define gds__dpb_allocation                2
#define gds__dpb_journal                   3
#define gds__dpb_page_size                 4
#define gds__dpb_num_buffers               5
#define gds__dpb_buffer_length             6
#define gds__dpb_debug                     7
#define gds__dpb_garbage_collect           8
#define gds__dpb_verify                    9
#define gds__dpb_sweep                     10
#define gds__dpb_enable_journal            11
#define gds__dpb_disable_journal           12
#define gds__dpb_dbkey_scope               13
#define gds__dpb_number_of_users           14
#define gds__dpb_trace                     15
#define gds__dpb_no_garbage_collect        16
#define gds__dpb_damaged                   17
#define gds__dpb_license                   18
#define gds__dpb_sys_user_name             19
#define gds__dpb_encrypt_key               20
#define gds__dpb_activate_shadow           21
#define gds__dpb_sweep_interval            22
#define gds__dpb_delete_shadow             23
#define gds__dpb_force_write               24
#define gds__dpb_begin_log                 25
#define gds__dpb_quit_log                  26
#define gds__dpb_no_reserve                27
#define gds__dpb_user_name                 28
#define gds__dpb_password                  29
#define gds__dpb_password_enc              30
#define gds__dpb_sys_user_name_enc         31
#define gds__dpb_interp                    32
#define gds__dpb_online_dump               33
#define gds__dpb_old_file_size             34
#define gds__dpb_old_num_files             35
#define gds__dpb_old_file                  36
#define gds__dpb_old_start_page            37
#define gds__dpb_old_start_seqno           38
#define gds__dpb_old_start_file            39
#define gds__dpb_drop_walfile              40
#define gds__dpb_old_dump_id               41
#define gds__dpb_wal_backup_dir            42
#define gds__dpb_wal_chkptlen              43
#define gds__dpb_wal_numbufs               44
#define gds__dpb_wal_bufsize               45
#define gds__dpb_wal_grp_cmt_wait          46
#define gds__dpb_lc_messages               47
#define gds__dpb_lc_ctype                  48
#define gds__dpb_cache_manager		   49
#define gds__dpb_shutdown		   50
#define gds__dpb_online			   51
#define gds__dpb_shutdown_delay		   52
#define gds__dpb_reserved		   53
#define gds__dpb_overwrite		   54
#define gds__dpb_sec_attach		   55
#define gds__dpb_disable_wal		   56
#define gds__dpb_connect_timeout	   57
#define gds__dpb_dummy_packet_interval     58

#else /* c++ definitions */

const char gds_dpb_version1 = 1;
const char gds_dpb_cdd_pathname = 1;
const char gds_dpb_allocation = 2;
const char gds_dpb_journal = 3;
const char gds_dpb_page_size = 4;
const char gds_dpb_num_buffers = 5;
const char gds_dpb_buffer_length = 6;
const char gds_dpb_debug = 7;
const char gds_dpb_garbage_collect = 8;
const char gds_dpb_verify = 9;
const char gds_dpb_sweep = 10;
const char gds_dpb_enable_journal = 11;
const char gds_dpb_disable_journal = 12;
const char gds_dpb_dbkey_scope = 13;
const char gds_dpb_number_of_users = 14;
const char gds_dpb_trace = 15;
const char gds_dpb_no_garbage_collect = 16;
const char gds_dpb_damaged = 17;
const char gds_dpb_license = 18;
const char gds_dpb_sys_user_name = 19;
const char gds_dpb_encrypt_key = 20;
const char gds_dpb_activate_shadow = 21;
const char gds_dpb_sweep_interval = 22;
const char gds_dpb_delete_shadow = 23;
const char gds_dpb_force_write = 24;
const char gds_dpb_begin_log = 25;
const char gds_dpb_quit_log = 26;
const char gds_dpb_no_reserve = 27;
const char gds_dpb_user_name = 28;
const char gds_dpb_password = 29;
const char gds_dpb_password_enc = 30;
const char gds_dpb_sys_user_name_enc = 31;
const char gds_dpb_interp = 32;
const char gds_dpb_online_dump = 33;
const char gds_dpb_old_file_size = 34;
const char gds_dpb_old_num_files = 35;
const char gds_dpb_old_file = 36;
const char gds_dpb_old_start_page = 37;
const char gds_dpb_old_start_seqno = 38;
const char gds_dpb_old_start_file = 39;
const char gds_dpb_drop_walfile = 40;
const char gds_dpb_old_dump_id = 41;
const char gds_dpb_wal_backup_dir = 42;
const char gds_dpb_wal_chkptlen = 43;
const char gds_dpb_wal_numbufs = 44;
const char gds_dpb_wal_bufsize = 45;
const char gds_dpb_wal_grp_cmt_wait = 46;
const char gds_dpb_lc_messages = 47;
const char gds_dpb_lc_ctype = 48;
const char gds_dpb_cache_manager = 49;
const char gds_dpb_shutdown = 50;
const char gds_dpb_online = 51;
const char gds_dpb_shutdown_delay = 52;
const char gds_dpb_reserved = 53;
const char gds_dpb_overwrite = 54;
const char gds_dpb_sec_attach = 55;
const char gds_dpb_disable_wal = 56;
const char gds_dpb_connect_timeout = 57;
const char gds_dpb_dummy_packet_interval = 58;

#endif



/**********************************/
/* gds__dpb_verify specific flags */
/**********************************/


#ifndef	__cplusplus				/* c definitions */

#define gds__dpb_pages                     1
#define gds__dpb_records                   2
#define gds__dpb_indices                   4
#define gds__dpb_transactions              8
#define gds__dpb_no_update                 16
#define gds__dpb_repair                    32
#define gds__dpb_ignore                    64

#else /* c++ definitions */

const char gds_dpb_pages = 1;
const char gds_dpb_records = 2;
const char gds_dpb_indices = 4;
const char gds_dpb_transactions = 8;
const char gds_dpb_no_update = 16;
const char gds_dpb_repair = 32;
const char gds_dpb_ignore = 64;

#endif

/************************************/
/* gds__dpb_shutdown specific flags */
/************************************/

#ifndef	__cplusplus				/* c definitions */

#define gds__dpb_shut_cache               1
#define gds__dpb_shut_attachment          2
#define gds__dpb_shut_transaction         4
#define gds__dpb_shut_force               8

#else /* c++ definitions */

const char gds_dpb_shut_cache = 1;
const char gds_dpb_shut_attachment = 2;
const char gds_dpb_shut_transaction = 4;
const char gds_dpb_shut_force = 8;

#endif


/*************************************/
/* Transaction parameter block stuff */
/*************************************/

#ifndef	__cplusplus				/* c definitions */

#define gds__tpb_version1                  1
#define gds__tpb_version3                  3
#define gds__tpb_consistency               1
#define gds__tpb_concurrency               2
#define gds__tpb_shared                    3
#define gds__tpb_protected                 4
#define gds__tpb_exclusive                 5
#define gds__tpb_wait                      6
#define gds__tpb_nowait                    7
#define gds__tpb_read                      8
#define gds__tpb_write                     9
#define gds__tpb_lock_read                 10
#define gds__tpb_lock_write                11
#define gds__tpb_verb_time                 12
#define gds__tpb_commit_time               13
#define gds__tpb_ignore_limbo              14
#define gds__tpb_read_committed		   15
#define gds__tpb_autocommit		   16
#define gds__tpb_rec_version		   17
#define gds__tpb_no_rec_version		   18
#define gds__tpb_restart_requests	   19
#define gds__tpb_no_auto_undo              20

#else /* c++ definitions */

const char gds_tpb_version1 = 1;
const char gds_tpb_version3 = 3;
const char gds_tpb_consistency = 1;
const char gds_tpb_concurrency = 2;
const char gds_tpb_shared = 3;
const char gds_tpb_protected = 4;
const char gds_tpb_exclusive = 5;
const char gds_tpb_wait = 6;
const char gds_tpb_nowait = 7;
const char gds_tpb_read = 8;
const char gds_tpb_write = 9;
const char gds_tpb_lock_read = 10;
const char gds_tpb_lock_write = 11;
const char gds_tpb_verb_time = 12;
const char gds_tpb_commit_time = 13;
const char gds_tpb_ignore_limbo = 14;
const char gds_tpb_read_committed = 15;
const char gds_tpb_autocommit = 16;
const char gds_tpb_rec_version = 17;
const char gds_tpb_no_rec_version = 18;
const char gds_tpb_restart_requests = 19;
const char gds_tpb_no_auto_undo = 20;

#endif




/************************/
/* Blob Parameter Block */
/************************/

#ifndef	__cplusplus				/* c definitions */

#define gds__bpb_version1                  1
#define gds__bpb_source_type               1
#define gds__bpb_target_type               2
#define gds__bpb_type                      3
#define gds__bpb_source_interp             4
#define gds__bpb_target_interp             5

#else /* c++ definitions */

const char gds_bpb_version1 = 1;
const char gds_bpb_source_type = 1;
const char gds_bpb_target_type = 2;
const char gds_bpb_type = 3;
const char gds_bpb_source_interp = 4;
const char gds_bpb_target_interp = 5;

#endif


#ifndef	__cplusplus				/* c definitions */

#define gds__bpb_type_segmented            0
#define gds__bpb_type_stream               1

#else /* c++ definitions */

const char gds_bpb_type_segmented = 0;
const char gds_bpb_type_stream = 1;

#endif


/*********************************/
/* Service parameter block stuff */
/*********************************/

#ifndef	__cplusplus				/* c definitions */

#define gds__spb_version1                  1
#define gds__spb_user_name                 2
#define gds__spb_sys_user_name             3
#define gds__spb_sys_user_name_enc         4
#define gds__spb_password                  5
#define gds__spb_password_enc              6
#define gds__spb_command_line              7
#define gds__spb_connect_timeout           8
#define gds__spb_dummy_packet_interval     9

#else /* c++ definitions */

const char gds_spb_version1 = 1;
const char gds_spb_user_name = 2;
const char gds_spb_sys_user_name = 3;
const char gds_spb_sys_user_name_enc = 4;
const char gds_spb_password = 5;
const char gds_spb_password_enc = 6;
const char gds_spb_command_line = 7;
const char gds_spb_connect_timeout = 8;
const char gds_spb_dummy_packet_interval = 9;

#endif




/*********************************/
/* Information call declarations */
/*********************************/

/****************************/
/* Common, structural codes */
/****************************/

#ifndef	__cplusplus				/* c definitions */

#define gds__info_end                      1
#define gds__info_truncated                2
#define gds__info_error                    3

#else /* c++ definitions */

const char gds_info_end = 1;
const char gds_info_truncated = 2;
const char gds_info_error = 3;

#endif




/******************************/
/* Database information items */
/******************************/

#ifndef	__cplusplus				/* c definitions */

#define gds__info_db_id                    4
#define gds__info_reads                    5
#define gds__info_writes                   6
#define gds__info_fetches                  7
#define gds__info_marks                    8
#define gds__info_implementation           11
#define gds__info_version                  12
#define gds__info_base_level               13
#define gds__info_page_size                14
#define gds__info_num_buffers              15
#define gds__info_limbo                    16
#define gds__info_current_memory           17
#define gds__info_max_memory               18
#define gds__info_window_turns             19
#define gds__info_license                  20
#define gds__info_allocation               21
#define gds__info_attachment_id            22
#define gds__info_read_seq_count           23
#define gds__info_read_idx_count           24
#define gds__info_insert_count             25
#define gds__info_update_count             26
#define gds__info_delete_count             27
#define gds__info_backout_count            28
#define gds__info_purge_count              29
#define gds__info_expunge_count            30
#define gds__info_sweep_interval           31
#define gds__info_ods_version              32
#define gds__info_ods_minor_version        33
#define gds__info_no_reserve               34
#define gds__info_logfile                  35
#define gds__info_cur_logfile_name         36
#define gds__info_cur_log_part_offset      37
#define gds__info_num_wal_buffers          38
#define gds__info_wal_buffer_size          39
#define gds__info_wal_ckpt_length          40
#define gds__info_wal_cur_ckpt_interval    41
#define gds__info_wal_prv_ckpt_fname       42
#define gds__info_wal_prv_ckpt_poffset     43
#define gds__info_wal_recv_ckpt_fname      44
#define gds__info_wal_recv_ckpt_poffset    45
#define gds__info_wal_grpc_wait_usecs      47
#define gds__info_wal_num_io               48
#define gds__info_wal_avg_io_size          49
#define gds__info_wal_num_commits          50
#define gds__info_wal_avg_grpc_size        51
#define gds__info_forced_writes		   52

#else /* c++ definitions */

const char gds_info_db_id = 4;
const char gds_info_reads = 5;
const char gds_info_writes = 6;
const char gds_info_fetches = 7;
const char gds_info_marks = 8;
const char gds_info_implementation = 11;
const char gds_info_version = 12;
const char gds_info_base_level = 13;
const char gds_info_page_size = 14;
const char gds_info_num_buffers = 15;
const char gds_info_limbo = 16;
const char gds_info_current_memory = 17;
const char gds_info_max_memory = 18;
const char gds_info_window_turns = 19;
const char gds_info_license = 20;
const char gds_info_allocation = 21;
const char gds_info_attachment_id = 22;
const char gds_info_read_seq_count = 23;
const char gds_info_read_idx_count = 24;
const char gds_info_insert_count = 25;
const char gds_info_update_count = 26;
const char gds_info_delete_count = 27;
const char gds_info_backout_count = 28;
const char gds_info_purge_count = 29;
const char gds_info_expunge_count = 30;
const char gds_info_sweep_interval = 31;
const char gds_info_ods_version = 32;
const char gds_info_ods_minor_version = 33;
const char gds_info_no_reserve = 34;
const char gds_info_logfile = 35;
const char gds_info_cur_logfile_name = 36;
const char gds_info_cur_log_part_offset = 37;
const char gds_info_num_wal_buffers = 38;
const char gds_info_wal_buffer_size = 39;
const char gds_info_wal_ckpt_length = 40;
const char gds_info_wal_cur_ckpt_interval = 41;
const char gds_info_wal_prv_ckpt_fname = 42;
const char gds_info_wal_prv_ckpt_poffset = 43;
const char gds_info_wal_recv_ckpt_fname = 44;
const char gds_info_wal_recv_ckpt_poffset = 45;
const char gds_info_wal_grpc_wait_usecs = 47;
const char gds_info_wal_num_io = 48;
const char gds_info_wal_avg_io_size = 49;
const char gds_info_wal_num_commits = 50;
const char gds_info_wal_avg_grpc_size = 51;
const char gds_info_forced_writes = 52;

#endif


/**************************************/
/* Database information return values */
/**************************************/

#ifndef	__cplusplus				/* c definitions */

#define gds__info_db_impl_rdb_vms          1
#define gds__info_db_impl_rdb_eln          2
#define gds__info_db_impl_rdb_eln_dev      3
#define gds__info_db_impl_rdb_vms_y        4
#define gds__info_db_impl_rdb_eln_y        5
#define gds__info_db_impl_jri              6
#define gds__info_db_impl_jsv              7
#define gds__info_db_impl_isc_a            25
#define gds__info_db_impl_isc_u            26
#define gds__info_db_impl_isc_v            27
#define gds__info_db_impl_isc_s            28
#define gds__info_db_impl_isc_apl_68K      25
#define gds__info_db_impl_isc_vax_ultr     26
#define gds__info_db_impl_isc_vms          27
#define gds__info_db_impl_isc_sun_68k      28
#define gds__info_db_impl_isc_os2          29
#define gds__info_db_impl_isc_sun4         30
#define gds__info_db_impl_isc_hp_ux        31
#define gds__info_db_impl_isc_sun_386i     32
#define gds__info_db_impl_isc_vms_orcl     33
#define gds__info_db_impl_isc_mac_aux      34
#define gds__info_db_impl_isc_rt_aix       35
#define gds__info_db_impl_isc_mips_ult     36
#define gds__info_db_impl_isc_xenix        37
#define gds__info_db_impl_isc_dg           38
#define gds__info_db_impl_isc_hp_mpexl     39
#define gds__info_db_impl_isc_hp_ux68K     40
#define gds__info_db_impl_isc_sgi          41
#define gds__info_db_impl_isc_sco_unix     42
#define gds__info_db_impl_isc_cray         43
#define gds__info_db_impl_isc_imp          44
#define gds__info_db_impl_isc_delta        45
#define gds__info_db_impl_isc_next         46
#define gds__info_db_impl_isc_dos          47
#define gds__info_db_impl_isc_winnt        48
#define gds__info_db_impl_isc_epson        49

#define gds__info_db_class_access          1
#define gds__info_db_class_y_valve         2
#define gds__info_db_class_rem_int         3
#define gds__info_db_class_rem_srvr        4
#define gds__info_db_class_pipe_int        7
#define gds__info_db_class_pipe_srvr       8
#define gds__info_db_class_sam_int         9
#define gds__info_db_class_sam_srvr        10
#define gds__info_db_class_gateway         11
#define gds__info_db_class_cache           12

#else /* c++ definitions */

const char gds_info_db_impl_rdb_vms = 1;
const char gds_info_db_impl_rdb_eln = 2;
const char gds_info_db_impl_rdb_eln_dev = 3;
const char gds_info_db_impl_rdb_vms_y = 4;
const char gds_info_db_impl_rdb_eln_y = 5;
const char gds_info_db_impl_jri = 6;
const char gds_info_db_impl_jsv = 7;
const char gds_info_db_impl_isc_a = 25;
const char gds_info_db_impl_isc_u = 26;
const char gds_info_db_impl_isc_v = 27;
const char gds_info_db_impl_isc_s = 28;
const char gds_info_db_impl_isc_apl_68K = 25;
const char gds_info_db_impl_isc_vax_ultr = 26;
const char gds_info_db_impl_isc_vms = 27;
const char gds_info_db_impl_isc_sun_68k = 28;
const char gds_info_db_impl_isc_os2 = 29;
const char gds_info_db_impl_isc_sun4 = 30;
const char gds_info_db_impl_isc_hp_ux = 31;
const char gds_info_db_impl_isc_sun_386i = 32;
const char gds_info_db_impl_isc_vms_orcl = 33;
const char gds_info_db_impl_isc_mac_aux = 34;
const char gds_info_db_impl_isc_rt_aix = 35;
const char gds_info_db_impl_isc_mips_ult = 36;
const char gds_info_db_impl_isc_xenix = 37;
const char gds_info_db_impl_isc_dg = 38;
const char gds_info_db_impl_isc_hp_mpexl = 39;
const char gds_info_db_impl_isc_hp_ux68K = 40;
const char gds_info_db_impl_isc_sgi = 41;
const char gds_info_db_impl_isc_sco_unix = 42;
const char gds_info_db_impl_isc_cray = 43;
const char gds_info_db_impl_isc_imp = 44;
const char gds_info_db_impl_isc_delta = 45;
const char gds_info_db_impl_isc_next = 46;
const char gds_info_db_impl_isc_dos = 47;
const char gds_info_db_impl_isc_winnt = 48;
const char gds_info_db_impl_isc_epson = 49;

const char gds_info_db_class_access = 1;
const char gds_info_db_class_y_valve = 2;
const char gds_info_db_class_rem_int = 3;
const char gds_info_db_class_rem_srvr = 4;
const char gds_info_db_class_pipe_int = 7;
const char gds_info_db_class_pipe_srvr = 8;
const char gds_info_db_class_sam_int = 9;
const char gds_info_db_class_sam_srvr = 10;
const char gds_info_db_class_gateway = 11;
const char gds_info_db_class_cache = 12;

#endif





/*****************************/
/* Request information items */
/*****************************/

#ifndef	__cplusplus				/* c definitions */

#define gds__info_number_messages          4
#define gds__info_max_message              5
#define gds__info_max_send                 6
#define gds__info_max_receive              7
#define gds__info_state                    8
#define gds__info_message_number           9
#define gds__info_message_size             10
#define gds__info_request_cost             11
#define gds__info_access_path              12
#define gds__info_req_select_count         13
#define gds__info_req_insert_count         14
#define gds__info_req_update_count         15
#define gds__info_req_delete_count         16

/*********************/
/* access path items */
/*********************/

#define gds__info_rsb_end		   0
#define gds__info_rsb_begin		   1
#define gds__info_rsb_type		   2
#define gds__info_rsb_relation		   3
#define gds__info_rsb_plan                 4

/*************/
/* rsb types */
/*************/

#define gds__info_rsb_unknown		   1
#define gds__info_rsb_indexed		   2
#define gds__info_rsb_navigate		   3
#define gds__info_rsb_sequential	   4
#define gds__info_rsb_cross		   5
#define gds__info_rsb_sort		   6
#define gds__info_rsb_first		   7
#define gds__info_rsb_boolean		   8
#define gds__info_rsb_union		   9
#define gds__info_rsb_aggregate		  10
#define gds__info_rsb_merge		  11
#define gds__info_rsb_ext_sequential	  12
#define gds__info_rsb_ext_indexed	  13
#define gds__info_rsb_ext_dbkey		  14
#define gds__info_rsb_left_cross	  15
#define gds__info_rsb_select		  16
#define gds__info_rsb_sql_join		  17
#define gds__info_rsb_simulate		  18
#define gds__info_rsb_sim_cross		  19
#define gds__info_rsb_once		  20
#define gds__info_rsb_procedure		  21

/**********************/
/* bitmap expressions */
/**********************/

#define gds__info_rsb_and		1
#define gds__info_rsb_or		2
#define gds__info_rsb_dbkey		3
#define gds__info_rsb_index		4

#define gds__info_req_active               2
#define gds__info_req_inactive             3
#define gds__info_req_send                 4
#define gds__info_req_receive              5
#define gds__info_req_select               6

#else /* c++ definitions */

const char gds_info_number_messages = 4;
const char gds_info_max_message = 5;
const char gds_info_max_send = 6;
const char gds_info_max_receive = 7;
const char gds_info_state = 8;
const char gds_info_message_number = 9;
const char gds_info_message_size = 10;
const char gds_info_request_cost = 11;
const char gds_info_access_path = 12;
const char gds_info_req_select_count = 13;
const char gds_info_req_insert_count = 14;
const char gds_info_req_update_count = 15;
const char gds_info_req_delete_count = 16;

/*********************/
/* access path items */
/*********************/

const char gds_info_rsb_end = 0;
const char gds_info_rsb_begin = 1;
const char gds_info_rsb_type = 2;
const char gds_info_rsb_relation = 3;



/*************/
/* rsb types */
/*************/

const char gds_info_rsb_unknown = 1;
const char gds_info_rsb_indexed = 2;
const char gds_info_rsb_navigate = 3;
const char gds_info_rsb_sequential = 4;
const char gds_info_rsb_cross = 5;
const char gds_info_rsb_sort = 6;
const char gds_info_rsb_first = 7;
const char gds_info_rsb_boolean = 8;
const char gds_info_rsb_union = 9;
const char gds_info_rsb_aggregate = 10;
const char gds_info_rsb_merge = 11;
const char gds_info_rsb_ext_sequential = 12;
const char gds_info_rsb_ext_indexed = 13;
const char gds_info_rsb_ext_dbkey = 14;
const char gds_info_rsb_left_cross = 15;
const char gds_info_rsb_select = 16;
const char gds_info_rsb_sql_join = 17;
const char gds_info_rsb_simulate = 18;
const char gds_info_rsb_sim_cross = 19;
const char gds_info_rsb_once = 20;
const char gds_info_rsb_procedure = 21;

/**********************/
/* bitmap expressions */
/**********************/

const char gds_info_rsb_and = 1;
const char gds_info_rsb_or = 2;
const char gds_info_rsb_dbkey = 3;
const char gds_info_rsb_index = 4;

const char gds_info_req_active = 2;
const char gds_info_req_inactive = 3;
const char gds_info_req_send = 4;
const char gds_info_req_receive = 5;
const char gds_info_req_select = 6;

#endif


/**************************/
/* Blob information items */
/**************************/

#ifndef	__cplusplus				/* c definitions */

#define gds__info_blob_num_segments        4
#define gds__info_blob_max_segment         5
#define gds__info_blob_total_length        6
#define gds__info_blob_type                7

#else /* c++ definitions */

const char gds_info_blob_num_segments = 4;
const char gds_info_blob_max_segment = 5;
const char gds_info_blob_total_length = 6;
const char gds_info_blob_type = 7;

#endif




/*********************************/
/* Transaction information items */
/*********************************/

#ifndef	__cplusplus				/* c definitions */

#define gds__info_tra_id                   4

#else /* c++ definitions */

const char gds_info_tra_id = 4;

#endif


/*****************************/
/* Service information items */
/*****************************/

#ifndef	__cplusplus				/* c definitions */

#define gds__info_svc_version              4
#define gds__info_svc_message              5
#define gds__info_svc_total_length         6
#define gds__info_svc_response             7
#define gds__info_svc_response_more        8
#define gds__info_svc_line                 9
#define gds__info_svc_to_eof               10
#define gds__info_svc_timeout              11

#else /* c++ definitions */

const char gds_info_svc_version = 4;
const char gds_info_svc_message = 5;
const char gds_info_svc_total_length = 6;
const char gds_info_svc_response = 7;
const char gds_info_svc_response_more = 8;
const char gds_info_svc_line = 9;
const char gds_info_svc_to_eof = 10;
const char gds_info_svc_timeout = 11;

#endif

/*************************/
/* SQL information items */
/*************************/

#ifndef	__cplusplus				/* c definitions */

#define gds__info_sql_select               4
#define gds__info_sql_bind                 5
#define gds__info_sql_num_variables        6
#define gds__info_sql_describe_vars        7
#define gds__info_sql_describe_end         8
#define gds__info_sql_sqlda_seq            9
#define gds__info_sql_message_seq          10
#define gds__info_sql_type                 11
#define gds__info_sql_sub_type             12
#define gds__info_sql_scale                13
#define gds__info_sql_length               14
#define gds__info_sql_null_ind             15
#define gds__info_sql_field                16
#define gds__info_sql_relation             17
#define gds__info_sql_owner                18
#define gds__info_sql_alias                19
#define gds__info_sql_sqlda_start          20
#define gds__info_sql_stmt_type            21
#define gds__info_sql_get_plan             22
#define gds__info_sql_records		   23

#else /* c++ definitions */

const char gds_info_sql_select = 4;
const char gds_info_sql_bind = 5;
const char gds_info_sql_num_variables = 6;
const char gds_info_sql_describe_vars = 7;
const char gds_info_sql_describe_end = 8;
const char gds_info_sql_sqlda_seq = 9;
const char gds_info_sql_message_seq = 10;
const char gds_info_sql_type = 11;
const char gds_info_sql_sub_type = 12;
const char gds_info_sql_scale = 13;
const char gds_info_sql_length = 14;
const char gds_info_sql_null_ind = 15;
const char gds_info_sql_field = 16;
const char gds_info_sql_relation = 17;
const char gds_info_sql_owner = 18;
const char gds_info_sql_alias = 19;
const char gds_info_sql_sqlda_start = 20;
const char gds_info_sql_stmt_type = 21;
const char gds_info_sql_get_plan = 22;
const char gds_info_sql_records = 23;

#endif




/*********************************/
/* SQL information return values */
/*********************************/

#ifndef	__cplusplus				/* c definitions */

#define gds__info_sql_stmt_select          1
#define gds__info_sql_stmt_insert          2
#define gds__info_sql_stmt_update          3
#define gds__info_sql_stmt_delete          4
#define gds__info_sql_stmt_ddl             5
#define gds__info_sql_stmt_get_segment     6
#define gds__info_sql_stmt_put_segment     7
#define gds__info_sql_stmt_exec_procedure  8
#define gds__info_sql_stmt_start_trans     9
#define gds__info_sql_stmt_commit          10
#define gds__info_sql_stmt_rollback        11
#define gds__info_sql_stmt_select_for_upd  12

#else /* c++ definitions */

const char gds_info_sql_stmt_select = 1;
const char gds_info_sql_stmt_insert = 2;
const char gds_info_sql_stmt_update = 3;
const char gds_info_sql_stmt_delete = 4;
const char gds_info_sql_stmt_ddl = 5;
const char gds_info_sql_stmt_get_segment = 6;
const char gds_info_sql_stmt_put_segment = 7;
const char gds_info_sql_stmt_exec_procedure = 8;
const char gds_info_sql_stmt_start_trans = 9;
const char gds_info_sql_stmt_commit = 10;
const char gds_info_sql_stmt_rollback = 11;
const char gds_info_sql_stmt_select_for_upd = 12;

#endif

/***************/
/* Error codes */
/***************/

#ifndef	__cplusplus				/* c definitions */

#define gds_facility                       20
#define gds_err_base                       335544320L
#define gds_err_factor                     1
#define gds_arg_end                        0
#define gds_arg_gds                        1
#define gds_arg_string                     2
#define gds_arg_cstring                    3
#define gds_arg_number                     4
#define gds_arg_interpreted                5
#define gds_arg_vms                        6
#define gds_arg_unix                       7
#define gds_arg_domain                     8
#define gds_arg_dos                        9
#define gds_arg_mpexl                      10
#define gds_arg_mpexl_ipc                  11
#define gds_arg_next_mach		   15
#define gds_arg_netware		           16
#define gds_arg_win32                      17

#else /* c++ definitions */

const GDS_LONG gds_facility = 20;
const GDS_LONG gds_err_base = 335544320L;
const GDS_LONG gds_err_factor = 1;
const GDS_LONG gds_arg_end = 0;
const GDS_LONG gds_arg_gds = 1;
const GDS_LONG gds_arg_string = 2;
const GDS_LONG gds_arg_cstring = 3;
const GDS_LONG gds_arg_number = 4;
const GDS_LONG gds_arg_interpreted = 5;
const GDS_LONG gds_arg_vms = 6;
const GDS_LONG gds_arg_unix = 7;
const GDS_LONG gds_arg_domain = 8;
const GDS_LONG gds_arg_dos = 9;
const GDS_LONG gds_arg_mpexl = 10;
const GDS_LONG gds_arg_mpexl_ipc = 11;
const GDS_LONG gds_arg_next_mach = 15;
const GDS_LONG gds_arg_netware = 16;
const GDS_LONG gds_arg_win32 = 17;

#endif

#ifndef	__cplusplus				/* c definitions */

#include "../jrd/codes.h"

#else /* c++ definitions */

const GDS_LONG gds_arith_except = 335544321L;
const GDS_LONG gds_bad_dbkey = 335544322L;
const GDS_LONG gds_bad_db_format = 335544323L;
const GDS_LONG gds_bad_db_handle = 335544324L;
const GDS_LONG gds_bad_dpb_content = 335544325L;
const GDS_LONG gds_bad_dpb_form = 335544326L;
const GDS_LONG gds_bad_req_handle = 335544327L;
const GDS_LONG gds_bad_segstr_handle = 335544328L;
const GDS_LONG gds_bad_segstr_id = 335544329L;
const GDS_LONG gds_bad_tpb_content = 335544330L;
const GDS_LONG gds_bad_tpb_form = 335544331L;
const GDS_LONG gds_bad_trans_handle = 335544332L;
const GDS_LONG gds_bug_check = 335544333L;
const GDS_LONG gds_convert_error = 335544334L;
const GDS_LONG gds_db_corrupt = 335544335L;
const GDS_LONG gds_deadlock = 335544336L;
const GDS_LONG gds_excess_trans = 335544337L;
const GDS_LONG gds_from_no_match = 335544338L;
const GDS_LONG gds_infinap = 335544339L;
const GDS_LONG gds_infona = 335544340L;
const GDS_LONG gds_infunk = 335544341L;
const GDS_LONG gds_integ_fail = 335544342L;
const GDS_LONG gds_invalid_blr = 335544343L;
const GDS_LONG gds_io_error = 335544344L;
const GDS_LONG gds_lock_conflict = 335544345L;
const GDS_LONG gds_metadata_corrupt = 335544346L;
const GDS_LONG gds_not_valid = 335544347L;
const GDS_LONG gds_no_cur_rec = 335544348L;
const GDS_LONG gds_no_dup = 335544349L;
const GDS_LONG gds_no_finish = 335544350L;
const GDS_LONG gds_no_meta_update = 335544351L;
const GDS_LONG gds_no_priv = 335544352L;
const GDS_LONG gds_no_recon = 335544353L;
const GDS_LONG gds_no_record = 335544354L;
const GDS_LONG gds_no_segstr_close = 335544355L;
const GDS_LONG gds_obsolete_metadata = 335544356L;
const GDS_LONG gds_open_trans = 335544357L;
const GDS_LONG gds_port_len = 335544358L;
const GDS_LONG gds_read_only_field = 335544359L;
const GDS_LONG gds_read_only_rel = 335544360L;
const GDS_LONG gds_read_only_trans = 335544361L;
const GDS_LONG gds_read_only_view = 335544362L;
const GDS_LONG gds_req_no_trans = 335544363L;
const GDS_LONG gds_req_sync = 335544364L;
const GDS_LONG gds_req_wrong_db = 335544365L;
const GDS_LONG gds_segment = 335544366L;
const GDS_LONG gds_segstr_eof = 335544367L;
const GDS_LONG gds_segstr_no_op = 335544368L;
const GDS_LONG gds_segstr_no_read = 335544369L;
const GDS_LONG gds_segstr_no_trans = 335544370L;
const GDS_LONG gds_segstr_no_write = 335544371L;
const GDS_LONG gds_segstr_wrong_db = 335544372L;
const GDS_LONG gds_sys_request = 335544373L;
const GDS_LONG gds_stream_eof = 335544374L;
const GDS_LONG gds_unavailable = 335544375L;
const GDS_LONG gds_unres_rel = 335544376L;
const GDS_LONG gds_uns_ext = 335544377L;
const GDS_LONG gds_wish_list = 335544378L;
const GDS_LONG gds_wrong_ods = 335544379L;
const GDS_LONG gds_wronumarg = 335544380L;
const GDS_LONG gds_imp_exc = 335544381L;
const GDS_LONG gds_random = 335544382L;
const GDS_LONG gds_fatal_conflict = 335544383L;
const GDS_LONG gds_badblk = 335544384L;
const GDS_LONG gds_invpoolcl = 335544385L;
const GDS_LONG gds_nopoolids = 335544386L;
const GDS_LONG gds_relbadblk = 335544387L;
const GDS_LONG gds_blktoobig = 335544388L;
const GDS_LONG gds_bufexh = 335544389L;
const GDS_LONG gds_syntaxerr = 335544390L;
const GDS_LONG gds_bufinuse = 335544391L;
const GDS_LONG gds_bdbincon = 335544392L;
const GDS_LONG gds_reqinuse = 335544393L;
const GDS_LONG gds_badodsver = 335544394L;
const GDS_LONG gds_relnotdef = 335544395L;
const GDS_LONG gds_fldnotdef = 335544396L;
const GDS_LONG gds_dirtypage = 335544397L;
const GDS_LONG gds_waifortra = 335544398L;
const GDS_LONG gds_doubleloc = 335544399L;
const GDS_LONG gds_nodnotfnd = 335544400L;
const GDS_LONG gds_dupnodfnd = 335544401L;
const GDS_LONG gds_locnotmar = 335544402L;
const GDS_LONG gds_badpagtyp = 335544403L;
const GDS_LONG gds_corrupt = 335544404L;
const GDS_LONG gds_badpage = 335544405L;
const GDS_LONG gds_badindex = 335544406L;
const GDS_LONG gds_dbbnotzer = 335544407L;
const GDS_LONG gds_tranotzer = 335544408L;
const GDS_LONG gds_trareqmis = 335544409L;
const GDS_LONG gds_badhndcnt = 335544410L;
const GDS_LONG gds_wrotpbver = 335544411L;
const GDS_LONG gds_wroblrver = 335544412L;
const GDS_LONG gds_wrodpbver = 335544413L;
const GDS_LONG gds_blobnotsup = 335544414L;
const GDS_LONG gds_badrelation = 335544415L;
const GDS_LONG gds_nodetach = 335544416L;
const GDS_LONG gds_notremote = 335544417L;
const GDS_LONG gds_trainlim = 335544418L;
const GDS_LONG gds_notinlim = 335544419L;
const GDS_LONG gds_traoutsta = 335544420L;
const GDS_LONG gds_connect_reject = 335544421L;
const GDS_LONG gds_dbfile = 335544422L;
const GDS_LONG gds_orphan = 335544423L;
const GDS_LONG gds_no_lock_mgr = 335544424L;
const GDS_LONG gds_ctxinuse = 335544425L;
const GDS_LONG gds_ctxnotdef = 335544426L;
const GDS_LONG gds_datnotsup = 335544427L;
const GDS_LONG gds_badmsgnum = 335544428L;
const GDS_LONG gds_badparnum = 335544429L;
const GDS_LONG gds_virmemexh = 335544430L;
const GDS_LONG gds_blocking_signal = 335544431L;
const GDS_LONG gds_lockmanerr = 335544432L;
const GDS_LONG gds_journerr = 335544433L;
const GDS_LONG gds_keytoobig = 335544434L;
const GDS_LONG gds_nullsegkey = 335544435L;
const GDS_LONG gds_sqlerr = 335544436L;
const GDS_LONG gds_wrodynver = 335544437L;
const GDS_LONG gds_funnotdef = 335544438L;
const GDS_LONG gds_funmismat = 335544439L;
const GDS_LONG gds_bad_msg_vec = 335544440L;
const GDS_LONG gds_bad_detach = 335544441L;
const GDS_LONG gds_noargacc_read = 335544442L;
const GDS_LONG gds_noargacc_write = 335544443L;
const GDS_LONG gds_read_only = 335544444L;
const GDS_LONG gds_ext_err = 335544445L;
const GDS_LONG gds_non_updatable = 335544446L;
const GDS_LONG gds_no_rollback = 335544447L;
const GDS_LONG gds_bad_sec_info = 335544448L;
const GDS_LONG gds_invalid_sec_info = 335544449L;
const GDS_LONG gds_misc_interpreted = 335544450L;
const GDS_LONG gds_update_conflict = 335544451L;
const GDS_LONG gds_unlicensed = 335544452L;
const GDS_LONG gds_obj_in_use = 335544453L;
const GDS_LONG gds_nofilter = 335544454L;
const GDS_LONG gds_shadow_accessed = 335544455L;
const GDS_LONG gds_invalid_sdl = 335544456L;
const GDS_LONG gds_out_of_bounds = 335544457L;
const GDS_LONG gds_invalid_dimension = 335544458L;
const GDS_LONG gds_rec_in_limbo = 335544459L;
const GDS_LONG gds_shadow_missing = 335544460L;
const GDS_LONG gds_cant_validate = 335544461L;
const GDS_LONG gds_cant_start_journal = 335544462L;
const GDS_LONG gds_gennotdef = 335544463L;
const GDS_LONG gds_cant_start_logging = 335544464L;
const GDS_LONG gds_bad_segstr_type = 335544465L;
const GDS_LONG gds_foreign_key = 335544466L;
const GDS_LONG gds_high_minor = 335544467L;
const GDS_LONG gds_tra_state = 335544468L;
const GDS_LONG gds_trans_invalid = 335544469L;
const GDS_LONG gds_buf_invalid = 335544470L;
const GDS_LONG gds_indexnotdefined = 335544471L;
const GDS_LONG gds_login = 335544472L;
const GDS_LONG gds_invalid_bookmark = 335544473L;
const GDS_LONG gds_bad_lock_level = 335544474L;
const GDS_LONG gds_relation_lock = 335544475L;
const GDS_LONG gds_record_lock = 335544476L;
const GDS_LONG gds_max_idx = 335544477L;
const GDS_LONG gds_jrn_enable = 335544478L;
const GDS_LONG gds_old_failure = 335544479L;
const GDS_LONG gds_old_in_progress = 335544480L;
const GDS_LONG gds_old_no_space = 335544481L;
const GDS_LONG gds_no_wal_no_jrn = 335544482L;
const GDS_LONG gds_num_old_files = 335544483L;
const GDS_LONG gds_wal_file_open = 335544484L;
const GDS_LONG gds_bad_stmt_handle = 335544485L;
const GDS_LONG gds_wal_failure = 335544486L;
const GDS_LONG gds_walw_err = 335544487L;
const GDS_LONG gds_logh_small = 335544488L;
const GDS_LONG gds_logh_inv_version = 335544489L;
const GDS_LONG gds_logh_open_flag = 335544490L;
const GDS_LONG gds_logh_open_flag2 = 335544491L;
const GDS_LONG gds_logh_diff_dbname = 335544492L;
const GDS_LONG gds_logf_unexpected_eof = 335544493L;
const GDS_LONG gds_logr_incomplete = 335544494L;
const GDS_LONG gds_logr_header_small = 335544495L;
const GDS_LONG gds_logb_small = 335544496L;
const GDS_LONG gds_wal_illegal_attach = 335544497L;
const GDS_LONG gds_wal_invalid_wpb = 335544498L;
const GDS_LONG gds_wal_err_rollover = 335544499L;
const GDS_LONG gds_no_wal = 335544500L;
const GDS_LONG gds_drop_wal = 335544501L;
const GDS_LONG gds_stream_not_defined = 335544502L;
const GDS_LONG gds_wal_subsys_error = 335544503L;
const GDS_LONG gds_wal_subsys_corrupt = 335544504L;
const GDS_LONG gds_no_archive = 335544505L;
const GDS_LONG gds_shutinprog = 335544506L;
const GDS_LONG gds_range_in_use = 335544507L;
const GDS_LONG gds_range_not_found = 335544508L;
const GDS_LONG gds_charset_not_found = 335544509L;
const GDS_LONG gds_lock_timeout = 335544510L;
const GDS_LONG gds_prcnotdef = 335544511L;
const GDS_LONG gds_prcmismat = 335544512L;
const GDS_LONG gds_wal_bugcheck = 335544513L;
const GDS_LONG gds_wal_cant_expand = 335544514L;
const GDS_LONG gds_codnotdef = 335544515L;
const GDS_LONG gds_xcpnotdef = 335544516L;
const GDS_LONG gds_except = 335544517L;
const GDS_LONG gds_cache_restart = 335544518L;
const GDS_LONG gds_bad_lock_handle = 335544519L;
const GDS_LONG gds_jrn_present = 335544520L;
const GDS_LONG gds_wal_err_rollover2 = 335544521L;
const GDS_LONG gds_wal_err_logwrite = 335544522L;
const GDS_LONG gds_wal_err_jrn_comm = 335544523L;
const GDS_LONG gds_wal_err_expansion = 335544524L;
const GDS_LONG gds_wal_err_setup = 335544525L;
const GDS_LONG gds_wal_err_ww_sync = 335544526L;
const GDS_LONG gds_wal_err_ww_start = 335544527L;
const GDS_LONG gds_shutdown = 335544528L;
const GDS_LONG gds_existing_priv_mod = 335544529L;
const GDS_LONG gds_primary_key_ref = 335544530L;
const GDS_LONG gds_primary_key_notnull = 335544531L;
const GDS_LONG gds_ref_cnstrnt_notfound = 335544532L;
const GDS_LONG gds_foreign_key_notfound = 335544533L;
const GDS_LONG gds_ref_cnstrnt_update = 335544534L;
const GDS_LONG gds_check_cnstrnt_update = 335544535L;
const GDS_LONG gds_check_cnstrnt_del = 335544536L;
const GDS_LONG gds_integ_index_seg_del = 335544537L;
const GDS_LONG gds_integ_index_seg_mod = 335544538L;
const GDS_LONG gds_integ_index_del = 335544539L;
const GDS_LONG gds_integ_index_mod = 335544540L;
const GDS_LONG gds_check_trig_del = 335544541L;
const GDS_LONG gds_check_trig_update = 335544542L;
const GDS_LONG gds_cnstrnt_fld_del = 335544543L;
const GDS_LONG gds_cnstrnt_fld_rename = 335544544L;
const GDS_LONG gds_rel_cnstrnt_update = 335544545L;
const GDS_LONG gds_constaint_on_view = 335544546L;
const GDS_LONG gds_invld_cnstrnt_type = 335544547L;
const GDS_LONG gds_primary_key_exists = 335544548L;
const GDS_LONG gds_systrig_update = 335544549L;
const GDS_LONG gds_not_rel_owner = 335544550L;
const GDS_LONG gds_grant_obj_notfound = 335544551L;
const GDS_LONG gds_grant_fld_notfound = 335544552L;
const GDS_LONG gds_grant_nopriv = 335544553L;
const GDS_LONG gds_nonsql_security_rel = 335544554L;
const GDS_LONG gds_nonsql_security_fld = 335544555L;
const GDS_LONG gds_wal_cache_err = 335544556L;
const GDS_LONG gds_shutfail = 335544557L;
const GDS_LONG gds_check_constraint = 335544558L;
const GDS_LONG gds_bad_svc_handle = 335544559L;
const GDS_LONG gds_shutwarn = 335544560L;
const GDS_LONG gds_wrospbver = 335544561L;
const GDS_LONG gds_bad_spb_form = 335544562L;
const GDS_LONG gds_svcnotdef = 335544563L;
const GDS_LONG gds_no_jrn = 335544564L;
const GDS_LONG gds_transliteration_failed = 335544565L;
const GDS_LONG gds_start_cm_for_wal = 335544566L;
const GDS_LONG gds_wal_ovflow_log_required = 335544567L;
const GDS_LONG gds_text_subtype = 335544568L;
const GDS_LONG gds_dsql_error = 335544569L;
const GDS_LONG gds_dsql_command_err = 335544570L;
const GDS_LONG gds_dsql_constant_err = 335544571L;
const GDS_LONG gds_dsql_cursor_err = 335544572L;
const GDS_LONG gds_dsql_datatype_err = 335544573L;
const GDS_LONG gds_dsql_decl_err = 335544574L;
const GDS_LONG gds_dsql_cursor_update_err = 335544575L;
const GDS_LONG gds_dsql_cursor_open_err = 335544576L;
const GDS_LONG gds_dsql_cursor_close_err = 335544577L;
const GDS_LONG gds_dsql_field_err = 335544578L;
const GDS_LONG gds_dsql_internal_err = 335544579L;
const GDS_LONG gds_dsql_relation_err = 335544580L;
const GDS_LONG gds_dsql_procedure_err = 335544581L;
const GDS_LONG gds_dsql_request_err = 335544582L;
const GDS_LONG gds_dsql_sqlda_err = 335544583L;
const GDS_LONG gds_dsql_var_count_err = 335544584L;
const GDS_LONG gds_dsql_stmt_handle = 335544585L;
const GDS_LONG gds_dsql_function_err = 335544586L;
const GDS_LONG gds_dsql_blob_err = 335544587L;
const GDS_LONG gds_collation_not_found = 335544588L;
const GDS_LONG gds_collation_not_for_charset = 335544589L;
const GDS_LONG gds_dsql_dup_option = 335544590L;
const GDS_LONG gds_dsql_tran_err = 335544591L;
const GDS_LONG gds_dsql_invalid_array = 335544592L;
const GDS_LONG gds_dsql_max_arr_dim_exceeded = 335544593L;
const GDS_LONG gds_dsql_arr_range_error = 335544594L;
const GDS_LONG gds_dsql_trigger_err = 335544595L;
const GDS_LONG gds_dsql_subselect_err = 335544596L;
const GDS_LONG gds_dsql_crdb_prepare_err = 335544597L;
const GDS_LONG gds_specify_field_err = 335544598L;
const GDS_LONG gds_num_field_err = 335544599L;
const GDS_LONG gds_col_name_err = 335544600L;
const GDS_LONG gds_where_err = 335544601L;
const GDS_LONG gds_table_view_err = 335544602L;
const GDS_LONG gds_distinct_err = 335544603L;
const GDS_LONG gds_key_field_count_err = 335544604L;
const GDS_LONG gds_subquery_err = 335544605L;
const GDS_LONG gds_expression_eval_err = 335544606L;
const GDS_LONG gds_node_err = 335544607L;
const GDS_LONG gds_command_end_err = 335544608L;
const GDS_LONG gds_index_name = 335544609L;
const GDS_LONG gds_exception_name = 335544610L;
const GDS_LONG gds_field_name = 335544611L;
const GDS_LONG gds_token_err = 335544612L;
const GDS_LONG gds_union_err = 335544613L;
const GDS_LONG gds_dsql_construct_err = 335544614L;
const GDS_LONG gds_field_aggregate_err = 335544615L;
const GDS_LONG gds_field_ref_err = 335544616L;
const GDS_LONG gds_order_by_err = 335544617L;
const GDS_LONG gds_return_mode_err = 335544618L;
const GDS_LONG gds_extern_func_err = 335544619L;
const GDS_LONG gds_alias_conflict_err = 335544620L;
const GDS_LONG gds_procedure_conflict_error = 335544621L;
const GDS_LONG gds_relation_conflict_err = 335544622L;
const GDS_LONG gds_dsql_domain_err = 335544623L;
const GDS_LONG gds_idx_seg_err = 335544624L;
const GDS_LONG gds_node_name_err = 335544625L;
const GDS_LONG gds_table_name = 335544626L;
const GDS_LONG gds_proc_name = 335544627L;
const GDS_LONG gds_idx_create_err = 335544628L;
const GDS_LONG gds_wal_shadow_err = 335544629L;
const GDS_LONG gds_dependency = 335544630L;
const GDS_LONG gds_idx_key_err = 335544631L;
const GDS_LONG gds_dsql_file_length_err = 335544632L;
const GDS_LONG gds_dsql_shadow_number_err = 335544633L;
const GDS_LONG gds_dsql_token_unk_err = 335544634L;
const GDS_LONG gds_dsql_no_relation_alias = 335544635L;
const GDS_LONG gds_indexname = 335544636L;
const GDS_LONG gds_no_stream_plan = 335544637L;
const GDS_LONG gds_stream_twice = 335544638L;
const GDS_LONG gds_stream_not_found = 335544639L;
const GDS_LONG gds_collation_requires_text = 335544640L;
const GDS_LONG gds_dsql_domain_not_found = 335544641L;
const GDS_LONG gds_index_unused = 335544642L;
const GDS_LONG gds_dsql_self_join = 335544643L;
const GDS_LONG gds_stream_bof = 335544644L;
const GDS_LONG gds_stream_crack = 335544645L;
const GDS_LONG gds_db_or_file_exists = 335544646L;
const GDS_LONG gds_invalid_operator = 335544647L;
const GDS_LONG gds_conn_lost = 335544648L;
const GDS_LONG gds_bad_checksum = 335544649L;
const GDS_LONG gds_page_type_err = 335544650L;
const GDS_LONG gds_ext_readonly_err = 335544651L;
const GDS_LONG gds_sing_select_err = 335544652L;
const GDS_LONG gds_psw_attach = 335544653L;
const GDS_LONG gds_psw_start_trans = 335544654L;
const GDS_LONG gds_invalid_direction = 335544655L;
const GDS_LONG gds_dsql_var_conflict = 335544656L;
const GDS_LONG gds_dsql_no_blob_array = 335544657L;
const GDS_LONG gds_dsql_base_table = 335544658L;
const GDS_LONG gds_duplicate_base_table = 335544659L;
const GDS_LONG gds_view_alias = 335544660L;
const GDS_LONG gds_index_root_page_full = 335544661L;
const GDS_LONG gds_dsql_blob_type_unknown = 335544662L;
const GDS_LONG gds_req_max_clones_exceeded = 335544663L;
const GDS_LONG gds_dsql_duplicate_spec = 335544664L;
const GDS_LONG gds_unique_key_violation = 335544665L;
const GDS_LONG gds_srvr_version_too_old = 335544666L;
const GDS_LONG gds_drdb_completed_with_errs = 335544667L;
const GDS_LONG gds_dsql_procedure_use_err = 335544668L;
const GDS_LONG gds_dsql_count_mismatch = 335544669L;
const GDS_LONG gds_blob_idx_err = 335544670L;
const GDS_LONG gds_array_idx_err = 335544671L;
const GDS_LONG gds_key_field_err = 335544672L;
const GDS_LONG gds_no_delete = 335544673L;
const GDS_LONG gds_del_last_field = 335544674L;
const GDS_LONG gds_sort_err = 335544675L;
const GDS_LONG gds_sort_mem_err = 335544676L;
const GDS_LONG gds_version_err = 335544677L;
const GDS_LONG gds_inval_key_posn = 335544678L;
const GDS_LONG gds_no_segments_err = 335544679L;
const GDS_LONG gds_crrp_data_err = 335544680L;
const GDS_LONG gds_rec_size_err = 335544681L;
const GDS_LONG gds_dsql_field_ref = 335544682L;
const GDS_LONG gds_req_depth_exceeded = 335544683L;
const GDS_LONG gds_no_field_access = 335544684L;
const GDS_LONG gds_no_dbkey = 335544685L;
const GDS_LONG gds_jrn_format_err = 335544686L;
const GDS_LONG gds_jrn_file_full = 335544687L;
const GDS_LONG gds_dsql_open_cursor_request = 335544688L;
const GDS_LONG gds_err_max = 368L;

#endif

#undef GDS_LONG



/**********************************************/
/* Dynamic Data Definition Language operators */
/**********************************************/

/******************/
/* Version number */
/******************/

#ifndef	__cplusplus				/* c definitions */

#define gds__dyn_version_1                 1
#define gds__dyn_eoc                       -1

#else /* c++ definitions */

const char gds_dyn_version_1 = 1;
const char gds_dyn_eoc = -1;

#endif

/******************************/
/* Operations (may be nested) */
/******************************/

#ifndef	__cplusplus				/* c definitions */

#define gds__dyn_begin                     2
#define gds__dyn_end                       3
#define gds__dyn_if                        4
#define gds__dyn_def_database              5
#define gds__dyn_def_global_fld            6
#define gds__dyn_def_local_fld             7
#define gds__dyn_def_idx                   8
#define gds__dyn_def_rel                   9
#define gds__dyn_def_sql_fld               10
#define gds__dyn_def_view                  12
#define gds__dyn_def_trigger               15
#define gds__dyn_def_security_class        120
#define gds__dyn_def_dimension             140
#define gds__dyn_def_generator             24
#define gds__dyn_def_function              25
#define gds__dyn_def_filter                26
#define gds__dyn_def_function_arg          27
#define gds__dyn_def_shadow                34
#define gds__dyn_def_trigger_msg           17
#define gds__dyn_def_file                  36
#define gds__dyn_mod_database              39
#define gds__dyn_mod_rel                   11
#define gds__dyn_mod_global_fld            13
#define gds__dyn_mod_idx                   102
#define gds__dyn_mod_local_fld             14
#define gds__dyn_mod_view                  16
#define gds__dyn_mod_security_class        122
#define gds__dyn_mod_trigger               113
#define gds__dyn_mod_trigger_msg           28
#define gds__dyn_delete_database           18
#define gds__dyn_delete_rel                19
#define gds__dyn_delete_global_fld         20
#define gds__dyn_delete_local_fld          21
#define gds__dyn_delete_idx                22
#define gds__dyn_delete_security_class     123
#define gds__dyn_delete_dimensions         143
#define gds__dyn_delete_trigger            23
#define gds__dyn_delete_trigger_msg        29
#define gds__dyn_delete_filter             32
#define gds__dyn_delete_function           33
#define gds__dyn_delete_shadow             35
#define gds__dyn_grant                     30
#define gds__dyn_revoke                    31
#define gds__dyn_def_primary_key           37
#define gds__dyn_def_foreign_key           38
#define gds__dyn_def_unique                40
#define gds__dyn_def_procedure             164
#define gds__dyn_delete_procedure          165
#define gds__dyn_def_parameter             135
#define gds__dyn_delete_parameter          136
#define gds__dyn_mod_procedure             175
#define gds__dyn_def_log_file              176
#define gds__dyn_def_cache_file            180
#define gds__dyn_def_exception             181
#define gds__dyn_mod_exception             182
#define gds__dyn_del_exception             183
#define gds__dyn_drop_log                  194
#define gds__dyn_drop_cache                195
#define gds__dyn_def_default_log           202

#else /* c++ definitions */

const unsigned char gds_dyn_begin = 2;
const unsigned char gds_dyn_end = 3;
const unsigned char gds_dyn_if = 4;
const unsigned char gds_dyn_def_database = 5;
const unsigned char gds_dyn_def_global_fld = 6;
const unsigned char gds_dyn_def_local_fld = 7;
const unsigned char gds_dyn_def_idx = 8;
const unsigned char gds_dyn_def_rel = 9;
const unsigned char gds_dyn_def_sql_fld = 10;
const unsigned char gds_dyn_def_view = 12;
const unsigned char gds_dyn_def_trigger = 15;
const unsigned char gds_dyn_def_security_class = 120;
const unsigned char gds_dyn_def_dimension = 140;
const unsigned char gds_dyn_def_generator = 24;
const unsigned char gds_dyn_def_function = 25;
const unsigned char gds_dyn_def_filter = 26;
const unsigned char gds_dyn_def_function_arg = 27;
const unsigned char gds_dyn_def_shadow = 34;
const unsigned char gds_dyn_def_trigger_msg = 17;
const unsigned char gds_dyn_def_file = 36;
const unsigned char gds_dyn_mod_database = 39;
const unsigned char gds_dyn_mod_rel = 11;
const unsigned char gds_dyn_mod_global_fld = 13;
const unsigned char gds_dyn_mod_idx = 102;
const unsigned char gds_dyn_mod_local_fld = 14;
const unsigned char gds_dyn_mod_view = 16;
const unsigned char gds_dyn_mod_security_class = 122;
const unsigned char gds_dyn_mod_trigger = 113;
const unsigned char gds_dyn_mod_trigger_msg = 28;
const unsigned char gds_dyn_delete_database = 18;
const unsigned char gds_dyn_delete_rel = 19;
const unsigned char gds_dyn_delete_global_fld = 20;
const unsigned char gds_dyn_delete_local_fld = 21;
const unsigned char gds_dyn_delete_idx = 22;
const unsigned char gds_dyn_delete_security_class = 123;
const unsigned char gds_dyn_delete_dimensions = 143;
const unsigned char gds_dyn_delete_trigger = 23;
const unsigned char gds_dyn_delete_trigger_msg = 29;
const unsigned char gds_dyn_delete_filter = 32;
const unsigned char gds_dyn_delete_function = 33;
const unsigned char gds_dyn_delete_shadow = 35;
const unsigned char gds_dyn_grant = 30;
const unsigned char gds_dyn_revoke = 31;
const unsigned char gds_dyn_def_primary_key = 37;
const unsigned char gds_dyn_def_foreign_key = 38;
const unsigned char gds_dyn_def_unique = 40;
const unsigned char gds_dyn_def_procedure = 164;
const unsigned char gds_dyn_delete_procedure = 165;
const unsigned char gds_dyn_def_parameter = 135;
const unsigned char gds_dyn_delete_parameter = 136;
const unsigned char gds_dyn_mod_procedure = 175;
const unsigned char gds_dyn_def_log_file = 176;
const unsigned char gds_dyn_def_cache_file = 180;
const unsigned char gds_dyn_def_exception = 181;
const unsigned char gds_dyn_mod_exception = 182;
const unsigned char gds_dyn_del_exception = 183;
const unsigned char gds_dyn_drop_log = 194;
const unsigned char gds_dyn_drop_cache = 195;
const unsigned char gds_dyn_def_default_log = 202;

#endif




/***********************/
/* View specific stuff */
/***********************/

#ifndef	__cplusplus				/* c definitions */

#define gds__dyn_view_blr                  43
#define gds__dyn_view_source               44
#define gds__dyn_view_relation             45
#define gds__dyn_view_context              46
#define gds__dyn_view_context_name         47

#else /* c++ definitions */

const char gds_dyn_view_blr = 43;
const char gds_dyn_view_source = 44;
const char gds_dyn_view_relation = 45;
const char gds_dyn_view_context = 46;
const char gds_dyn_view_context_name = 47;

#endif

/**********************/
/* Generic attributes */
/**********************/

#ifndef	__cplusplus				/* c definitions */

#define gds__dyn_rel_name                  50
#define gds__dyn_fld_name                  51
#define gds__dyn_idx_name                  52
#define gds__dyn_description               53
#define gds__dyn_security_class            54
#define gds__dyn_system_flag               55
#define gds__dyn_update_flag               56
#define gds__dyn_prc_name                  166
#define gds__dyn_prm_name                  137
#define gds__dyn_sql_object                196
#define gds__dyn_fld_character_set_name    174

#else /* c++ definitions */

const unsigned char gds_dyn_rel_name = 50;
const unsigned char gds_dyn_fld_name = 51;
const unsigned char gds_dyn_idx_name = 52;
const unsigned char gds_dyn_description = 53;
const unsigned char gds_dyn_security_class = 54;
const unsigned char gds_dyn_system_flag = 55;
const unsigned char gds_dyn_update_flag = 56;
const unsigned char gds_dyn_prc_name = 166;
const unsigned char gds_dyn_prm_name = 137;
const unsigned char gds_dyn_sql_object = 196;
const unsigned char gds_dyn_fld_character_set_name = 174;

#endif

/********************************/
/* Relation specific attributes */
/********************************/

#ifndef	__cplusplus				/* c definitions */

#define gds__dyn_rel_dbkey_length          61
#define gds__dyn_rel_store_trig            62
#define gds__dyn_rel_modify_trig           63
#define gds__dyn_rel_erase_trig            64
#define gds__dyn_rel_store_trig_source     65
#define gds__dyn_rel_modify_trig_source    66
#define gds__dyn_rel_erase_trig_source     67
#define gds__dyn_rel_ext_file              68
#define gds__dyn_rel_sql_protection        69
#define gds__dyn_rel_constraint            162
#define gds__dyn_delete_rel_constraint     163

#else /* c++ definitions */

const unsigned char gds_dyn_rel_dbkey_length = 61;
const unsigned char gds_dyn_rel_store_trig = 62;
const unsigned char gds_dyn_rel_modify_trig = 63;
const unsigned char gds_dyn_rel_erase_trig = 64;
const unsigned char gds_dyn_rel_store_trig_source = 65;
const unsigned char gds_dyn_rel_modify_trig_source = 66;
const unsigned char gds_dyn_rel_erase_trig_source = 67;
const unsigned char gds_dyn_rel_ext_file = 68;
const unsigned char gds_dyn_rel_sql_protection = 69;
const unsigned char gds_dyn_rel_constraint = 162;
const unsigned char gds_dyn_delete_rel_constraint = 163;

#endif


/************************************/
/* Global field specific attributes */
/************************************/

#ifndef	__cplusplus				/* c definitions */

#define gds__dyn_fld_type                  70
#define gds__dyn_fld_length                71
#define gds__dyn_fld_scale                 72
#define gds__dyn_fld_sub_type              73
#define gds__dyn_fld_segment_length        74
#define gds__dyn_fld_query_header          75
#define gds__dyn_fld_edit_string           76
#define gds__dyn_fld_validation_blr        77
#define gds__dyn_fld_validation_source     78
#define gds__dyn_fld_computed_blr          79
#define gds__dyn_fld_computed_source       80
#define gds__dyn_fld_missing_value         81
#define gds__dyn_fld_default_value         82
#define gds__dyn_fld_query_name            83
#define gds__dyn_fld_dimensions            84
#define gds__dyn_fld_not_null              85
#define gds__dyn_fld_char_length           172
#define gds__dyn_fld_collation             173
#define gds__dyn_fld_default_source        193
#define gds__dyn_del_default               197
#define gds__dyn_del_validation            198
#define gds__dyn_single_validation         199
#define gds__dyn_fld_character_set         203

#else /* c++ definitions */

const unsigned char gds_dyn_fld_type = 70;
const unsigned char gds_dyn_fld_length = 71;
const unsigned char gds_dyn_fld_scale = 72;
const unsigned char gds_dyn_fld_sub_type = 73;
const unsigned char gds_dyn_fld_segment_length = 74;
const unsigned char gds_dyn_fld_query_header = 75;
const unsigned char gds_dyn_fld_edit_string = 76;
const unsigned char gds_dyn_fld_validation_blr = 77;
const unsigned char gds_dyn_fld_validation_source = 78;
const unsigned char gds_dyn_fld_computed_blr = 79;
const unsigned char gds_dyn_fld_computed_source = 80;
const unsigned char gds_dyn_fld_missing_value = 81;
const unsigned char gds_dyn_fld_default_value = 82;
const unsigned char gds_dyn_fld_query_name = 83;
const unsigned char gds_dyn_fld_dimensions = 84;
const unsigned char gds_dyn_fld_not_null = 85;
const unsigned char gds_dyn_fld_char_length = 172;
const unsigned char gds_dyn_fld_collation = 173;
const unsigned char gds_dyn_fld_default_source = 193;
const unsigned char gds_dyn_del_default = 197;
const unsigned char gds_dyn_del_validation = 198;
const unsigned char gds_dyn_single_validation = 199;
const unsigned char gds_dyn_fld_character_set = 203;

#endif


/***********************************/
/* Local field specific attributes */
/***********************************/

#ifndef	__cplusplus				/* c definitions */

#define gds__dyn_fld_source                90
#define gds__dyn_fld_base_fld              91
#define gds__dyn_fld_position              92
#define gds__dyn_fld_update_flag           93

#else /* c++ definitions */

const unsigned char gds_dyn_fld_source = 90;
const unsigned char gds_dyn_fld_base_fld = 91;
const unsigned char gds_dyn_fld_position = 92;
const unsigned char gds_dyn_fld_update_flag = 93;

#endif


/*****************************/
/* Index specific attributes */
/*****************************/

#ifndef	__cplusplus				/* c definitions */

#define gds__dyn_idx_unique                100
#define gds__dyn_idx_inactive              101
#define gds__dyn_idx_type                  103
#define gds__dyn_idx_foreign_key           104
#define gds__dyn_idx_ref_column            105
#define gds__dyn_idx_statistic		   204

#else /* c++ definitions */

const unsigned char gds_dyn_idx_unique = 100;
const unsigned char gds_dyn_idx_inactive = 101;
const unsigned char gds_dyn_idx_type = 103;
const unsigned char gds_dyn_idx_foreign_key = 104;
const unsigned char gds_dyn_idx_ref_column = 105;
const unsigned char gds_dyn_idx_statistic = 204;

#endif


/*******************************/
/* Trigger specific attributes */
/*******************************/

#ifndef	__cplusplus				/* c definitions */

#define gds__dyn_trg_type                  110
#define gds__dyn_trg_blr                   111
#define gds__dyn_trg_source                112
#define gds__dyn_trg_name                  114
#define gds__dyn_trg_sequence              115
#define gds__dyn_trg_inactive              116
#define gds__dyn_trg_msg_number            117
#define gds__dyn_trg_msg                   118

#else /* c++ definitions */

const unsigned char gds_dyn_trg_type = 110;
const unsigned char gds_dyn_trg_blr = 111;
const unsigned char gds_dyn_trg_source = 112;
const unsigned char gds_dyn_trg_name = 114;
const unsigned char gds_dyn_trg_sequence = 115;
const unsigned char gds_dyn_trg_inactive = 116;
const unsigned char gds_dyn_trg_msg_number = 117;
const unsigned char gds_dyn_trg_msg = 118;

#endif


/**************************************/
/* Security Class specific attributes */
/**************************************/

#ifndef	__cplusplus				/* c definitions */

#define gds__dyn_scl_acl                   121
#define gds__dyn_grant_user                130
#define gds__dyn_grant_proc                186
#define gds__dyn_grant_trig                187
#define gds__dyn_grant_view                188
#define gds__dyn_grant_options             132

#else /* c++ definitions */

const unsigned char gds_dyn_scl_acl = 121;
const unsigned char gds_dyn_grant_user = 130;
const unsigned char gds_dyn_grant_proc = 186;
const unsigned char gds_dyn_grant_trig = 187;
const unsigned char gds_dyn_grant_view = 188;
const unsigned char gds_dyn_grant_options = 132;

#endif


/**********************************/
/* Dimension specific information */
/**********************************/

#ifndef	__cplusplus				/* c definitions */

#define gds__dyn_dim_lower                 141
#define gds__dyn_dim_upper                 142

#else /* c++ definitions */

const unsigned char gds_dyn_dim_lower = 141;
const unsigned char gds_dyn_dim_upper = 142;

#endif


/****************************/
/* File specific attributes */
/****************************/

#ifndef	__cplusplus				/* c definitions */

#define gds__dyn_file_name                 125
#define gds__dyn_file_start                126
#define gds__dyn_file_length               127
#define gds__dyn_shadow_number             128
#define gds__dyn_shadow_man_auto           129
#define gds__dyn_shadow_conditional        130

#else /* c++ definitions */

const unsigned char gds_dyn_file_name = 125;
const unsigned char gds_dyn_file_start = 126;
const unsigned char gds_dyn_file_length = 127;
const unsigned char gds_dyn_shadow_number = 128;
const unsigned char gds_dyn_shadow_man_auto = 129;
const unsigned char gds_dyn_shadow_conditional = 130;

#endif

/********************************/
/* Log file specific attributes */
/********************************/

#ifndef	__cplusplus				/* c definitions */

#define gds__dyn_log_file_sequence         177
#define gds__dyn_log_file_partitions       178
#define gds__dyn_log_file_serial           179
#define gds__dyn_log_file_overflow         200
#define gds__dyn_log_file_raw		   201

#else /* c++ definitions */

const unsigned char gds_dyn_log_file_sequence = 177;
const unsigned char gds_dyn_log_file_partitions = 178;
const unsigned char gds_dyn_log_file_serial = 179;
const unsigned char gds_dyn_log_file_overflow = 200;
const unsigned char gds_dyn_log_file_raw = 201;

#endif


/***************************/
/* Log specific attributes */
/***************************/

#ifndef	__cplusplus				/* c definitions */

#define gds__dyn_log_group_commit_wait     189
#define gds__dyn_log_buffer_size           190
#define gds__dyn_log_check_point_length    191
#define gds__dyn_log_num_of_buffers        192

#else /* c++ definitions */

const unsigned char gds_dyn_log_group_commit_wait = 189;
const unsigned char gds_dyn_log_buffer_size = 190;
const unsigned char gds_dyn_log_check_point_length = 191;
const unsigned char gds_dyn_log_num_of_buffers = 192;

#endif

/********************************/
/* Function specific attributes */
/********************************/

#ifndef	__cplusplus				/* c definitions */

#define gds__dyn_function_name             145
#define gds__dyn_function_type             146
#define gds__dyn_func_module_name          147
#define gds__dyn_func_entry_point          148
#define gds__dyn_func_return_argument      149
#define gds__dyn_func_arg_position         150
#define gds__dyn_func_mechanism            151
#define gds__dyn_filter_in_subtype         152
#define gds__dyn_filter_out_subtype        153

#else /* c++ definitions */

const unsigned char gds_dyn_function_name = 145;
const unsigned char gds_dyn_function_type = 146;
const unsigned char gds_dyn_func_module_name = 147;
const unsigned char gds_dyn_func_entry_point = 148;
const unsigned char gds_dyn_func_return_argument = 149;
const unsigned char gds_dyn_func_arg_position = 150;
const unsigned char gds_dyn_func_mechanism = 151;
const unsigned char gds_dyn_filter_in_subtype = 152;
const unsigned char gds_dyn_filter_out_subtype = 153;

#endif


#ifndef	__cplusplus				/* c definitions */

#define gds__dyn_description2		   154
#define gds__dyn_fld_computed_source2	   155
#define gds__dyn_fld_edit_string2	   156
#define gds__dyn_fld_query_header2	   157
#define gds__dyn_fld_validation_source2    158
#define gds__dyn_trg_msg2	   	   159
#define gds__dyn_trg_source2	   	   160
#define gds__dyn_view_source2		   161
#define gds__dyn_xcp_msg2                  184

#else /* c++ definitions */

const unsigned char gds_dyn_description2 = 154;
const unsigned char gds_dyn_fld_computed_source2 = 155;
const unsigned char gds_dyn_fld_edit_string2 = 156;
const unsigned char gds_dyn_fld_query_header2 = 157;
const unsigned char gds_dyn_fld_validation_source2 = 158;
const unsigned char gds_dyn_trg_msg2 = 159;
const unsigned char gds_dyn_trg_source2 = 160;
const unsigned char gds_dyn_view_source2 = 161;
const unsigned char gds_dyn_xcp_msg2 = 184;

#endif



/*********************************/
/* Generator specific attributes */
/*********************************/

#ifndef	__cplusplus				/* c definitions */

#define gds__dyn_generator_name            95
#define gds__dyn_generator_id              96

#else /* c++ definitions */

const unsigned char gds_dyn_generator_name = 95;
const unsigned char gds_dyn_generator_id = 96;

#endif


/*********************************/
/* Procedure specific attributes */
/*********************************/

#ifndef	__cplusplus				/* c definitions */

#define gds__dyn_prc_inputs                167
#define gds__dyn_prc_outputs               168
#define gds__dyn_prc_source                169
#define gds__dyn_prc_blr                   170
#define gds__dyn_prc_source2               171

#else /* c++ definitions */

const unsigned char gds_dyn_prc_inputs = 167;
const unsigned char gds_dyn_prc_outputs = 168;
const unsigned char gds_dyn_prc_source = 169;
const unsigned char gds_dyn_prc_blr = 170;
const unsigned char gds_dyn_prc_source2 = 171;

#endif


/*********************************/
/* Parameter specific attributes */
/*********************************/

#ifndef	__cplusplus				/* c definitions */

#define gds__dyn_prm_number                138
#define gds__dyn_prm_type                  139

#else /* c++ definitions */

const unsigned char gds_dyn_prm_number = 138;
const unsigned char gds_dyn_prm_type = 139;

#endif

/********************************/
/* Relation specific attributes */
/********************************/

#ifndef       __cplusplus		/* c definitions */

#define gds__dyn_xcp_msg                   185

#else /* c++ definitions */

const unsigned char gds_dyn_xcp_msg = 185;

#endif


/**********************************************/
/* Cascading referential integrity values     */
/**********************************************/
#ifndef __cplusplus				/* c definitions */

#define gds__dyn_foreign_key_update        205
#define gds__dyn_foreign_key_delete        206
#define gds__dyn_foreign_key_cascade       207
#define gds__dyn_foreign_key_default       208
#define gds__dyn_foreign_key_null          209
#define gds__dyn_foreign_key_none          210

#else /* c++ definitions */

const unsigned char gds_dyn_foreign_key_update = 205;
const unsigned char gds_dyn_foreign_key_delete = 206;
const unsigned char gds_dyn_foreign_key_cascade = 207;
const unsigned char gds_dyn_foreign_key_default = 208;
const unsigned char gds_dyn_foreign_key_null = 209;
const unsigned char gds_dyn_foreign_key_none = 210;

#endif


/****************************/
/* Last $dyn value assigned */
/****************************/

#ifndef	__cplusplus				/* c definitions */

#define gds__dyn_last_dyn_value            210

#else /* c++ definitions */

const unsigned char gds_dyn_last_dyn_value = 210;

#endif





/******************************************/
/* Array slice description language (SDL) */
/******************************************/

#ifndef	__cplusplus				/* c definitions */

#define gds__sdl_version1                  1
#define gds__sdl_eoc                       -1
#define gds__sdl_relation                  2
#define gds__sdl_rid                       3
#define gds__sdl_field                     4
#define gds__sdl_fid                       5
#define gds__sdl_struct                    6
#define gds__sdl_variable                  7
#define gds__sdl_scalar                    8
#define gds__sdl_tiny_integer              9
#define gds__sdl_short_integer             10
#define gds__sdl_long_integer              11
#define gds__sdl_literal                   12
#define gds__sdl_add                       13
#define gds__sdl_subtract                  14
#define gds__sdl_multiply                  15
#define gds__sdl_divide                    16
#define gds__sdl_negate                    17
#define gds__sdl_eql                       18
#define gds__sdl_neq                       19
#define gds__sdl_gtr                       20
#define gds__sdl_geq                       21
#define gds__sdl_lss                       22
#define gds__sdl_leq                       23
#define gds__sdl_and                       24
#define gds__sdl_or                        25
#define gds__sdl_not                       26
#define gds__sdl_while                     27
#define gds__sdl_assignment                28
#define gds__sdl_label                     29
#define gds__sdl_leave                     30
#define gds__sdl_begin                     31
#define gds__sdl_end                       32
#define gds__sdl_do3                       33
#define gds__sdl_do2                       34
#define gds__sdl_do1                       35
#define gds__sdl_element                   36

#else /* c++ definitions */

const unsigned char gds_sdl_version1 = 1;
const unsigned char gds_sdl_eoc = -1;
const unsigned char gds_sdl_relation = 2;
const unsigned char gds_sdl_rid = 3;
const unsigned char gds_sdl_field = 4;
const unsigned char gds_sdl_fid = 5;
const unsigned char gds_sdl_struct = 6;
const unsigned char gds_sdl_variable = 7;
const unsigned char gds_sdl_scalar = 8;
const unsigned char gds_sdl_tiny_integer = 9;
const unsigned char gds_sdl_short_integer = 10;
const unsigned char gds_sdl_long_integer = 11;
const unsigned char gds_sdl_literal = 12;
const unsigned char gds_sdl_add = 13;
const unsigned char gds_sdl_subtract = 14;
const unsigned char gds_sdl_multiply = 15;
const unsigned char gds_sdl_divide = 16;
const unsigned char gds_sdl_negate = 17;
const unsigned char gds_sdl_eql = 18;
const unsigned char gds_sdl_neq = 19;
const unsigned char gds_sdl_gtr = 20;
const unsigned char gds_sdl_geq = 21;
const unsigned char gds_sdl_lss = 22;
const unsigned char gds_sdl_leq = 23;
const unsigned char gds_sdl_and = 24;
const unsigned char gds_sdl_or = 25;
const unsigned char gds_sdl_not = 26;
const unsigned char gds_sdl_while = 27;
const unsigned char gds_sdl_assignment = 28;
const unsigned char gds_sdl_label = 29;
const unsigned char gds_sdl_leave = 30;
const unsigned char gds_sdl_begin = 31;
const unsigned char gds_sdl_end = 32;
const unsigned char gds_sdl_do3 = 33;
const unsigned char gds_sdl_do2 = 34;
const unsigned char gds_sdl_do1 = 35;
const unsigned char gds_sdl_element = 36;

#endif


/********************************************/
/* International text interpretation values */
/********************************************/

#ifndef	__cplusplus				/* c definitions */

#define gds__interp_eng_ascii              0
#define gds__interp_jpn_sjis               5
#define gds__interp_jpn_euc                6

#else /* c++ definitions */

const unsigned char gds_interp_eng_ascii = 0;
const unsigned char gds_interp_jpn_sjis = 5;
const unsigned char gds_interp_jpn_euc = 6;

#endif


/*****************************/
/* Forms Package definitions */
/*****************************/

/************************************/
/* Map definition block definitions */
/************************************/

#ifndef	__cplusplus				/* c definitions */

#define PYXIS__MAP_VERSION1                1
#define PYXIS__MAP_FIELD2                  2
#define PYXIS__MAP_FIELD1                  3
#define PYXIS__MAP_MESSAGE                 4
#define PYXIS__MAP_TERMINATOR              5
#define PYXIS__MAP_TERMINATING_FIELD       6
#define PYXIS__MAP_OPAQUE                  7
#define PYXIS__MAP_TRANSPARENT             8
#define PYXIS__MAP_TAG                     9
#define PYXIS__MAP_SUB_FORM                10
#define PYXIS__MAP_ITEM_INDEX              11
#define PYXIS__MAP_SUB_FIELD               12
#define PYXIS__MAP_END                     -1

#else /* c++ definitions */

const char PYXIS_MAP_VERSION1 = 1;
const char PYXIS_MAP_FIELD2 = 2;
const char PYXIS_MAP_FIELD1 = 3;
const char PYXIS_MAP_MESSAGE = 4;
const char PYXIS_MAP_TERMINATOR = 5;
const char PYXIS_MAP_TERMINATING_FIELD = 6;
const char PYXIS_MAP_OPAQUE = 7;
const char PYXIS_MAP_TRANSPARENT = 8;
const char PYXIS_MAP_TAG = 9;
const char PYXIS_MAP_SUB_FORM = 10;
const char PYXIS_MAP_ITEM_INDEX = 11;
const char PYXIS_MAP_SUB_FIELD = 12;
const char PYXIS_MAP_END = -1;

#endif


/******************************************/
/* Field option flags for display options */
/******************************************/

#ifndef	__cplusplus				/* c definitions */

#define PYXIS__OPT_DISPLAY                 1
#define PYXIS__OPT_UPDATE                  2
#define PYXIS__OPT_WAKEUP                  4
#define PYXIS__OPT_POSITION                8

#else /* c++ definitions */

const unsigned char PYXIS_OPT_DISPLAY = 1;
const unsigned char PYXIS_OPT_UPDATE = 2;
const unsigned char PYXIS_OPT_WAKEUP = 4;
const unsigned char PYXIS_OPT_POSITION = 8;

#endif


/*****************************************/
/* Field option values following display */
/*****************************************/

#ifndef	__cplusplus				/* c definitions */

#define PYXIS__OPT_NULL                    1
#define PYXIS__OPT_DEFAULT                 2
#define PYXIS__OPT_INITIAL                 3
#define PYXIS__OPT_USER_DATA               4

#else /* c++ definitions */

const unsigned char PYXIS_OPT_NULL = 1;
const unsigned char PYXIS_OPT_DEFAULT = 2;
const unsigned char PYXIS_OPT_INITIAL = 3;
const unsigned char PYXIS_OPT_USER_DATA = 4;

#endif


/**************************/
/* Pseudo key definitions */
/**************************/

#ifndef	__cplusplus				/* c definitions */

#define PYXIS__KEY_DELETE                  127
#define PYXIS__KEY_UP                      128
#define PYXIS__KEY_DOWN                    129
#define PYXIS__KEY_RIGHT                   130
#define PYXIS__KEY_LEFT                    131
#define PYXIS__KEY_PF1                     132
#define PYXIS__KEY_PF2                     133
#define PYXIS__KEY_PF3                     134
#define PYXIS__KEY_PF4                     135
#define PYXIS__KEY_PF5                     136
#define PYXIS__KEY_PF6                     137
#define PYXIS__KEY_PF7                     138
#define PYXIS__KEY_PF8                     139
#define PYXIS__KEY_PF9                     140
#define PYXIS__KEY_ENTER                   141
#define PYXIS__KEY_SCROLL_TOP              146
#define PYXIS__KEY_SCROLL_BOTTOM           147

#else /* c++ definitions */

const unsigned char PYXIS_KEY_DELETE = 127;
const unsigned char PYXIS_KEY_UP = 128;
const unsigned char PYXIS_KEY_DOWN = 129;
const unsigned char PYXIS_KEY_RIGHT = 130;
const unsigned char PYXIS_KEY_LEFT = 131;
const unsigned char PYXIS_KEY_PF1 = 132;
const unsigned char PYXIS_KEY_PF2 = 133;
const unsigned char PYXIS_KEY_PF3 = 134;
const unsigned char PYXIS_KEY_PF4 = 135;
const unsigned char PYXIS_KEY_PF5 = 136;
const unsigned char PYXIS_KEY_PF6 = 137;
const unsigned char PYXIS_KEY_PF7 = 138;
const unsigned char PYXIS_KEY_PF8 = 139;
const unsigned char PYXIS_KEY_PF9 = 140;
const unsigned char PYXIS_KEY_ENTER = 141;
const unsigned char PYXIS_KEY_SCROLL_TOP = 146;
const unsigned char PYXIS_KEY_SCROLL_BOTTOM = 147;

#endif


/*************************/
/* Menu definition stuff */
/*************************/

#ifndef	__cplusplus				/* c definitions */

#define PYXIS__MENU_VERSION1               1
#define PYXIS__MENU_LABEL                  2
#define PYXIS__MENU_ENTREE                 3
#define PYXIS__MENU_OPAQUE                 4
#define PYXIS__MENU_TRANSPARENT            5
#define PYXIS__MENU_HORIZONTAL             6
#define PYXIS__MENU_VERTICAL               7
#define PYXIS__MENU_END                    -1

#else /* c++ definitions */

const unsigned char PYXIS_MENU_VERSION1 = 1;
const unsigned char PYXIS_MENU_LABEL = 2;
const unsigned char PYXIS_MENU_ENTREE = 3;
const unsigned char PYXIS_MENU_OPAQUE = 4;
const unsigned char PYXIS_MENU_TRANSPARENT = 5;
const unsigned char PYXIS_MENU_HORIZONTAL = 6;
const unsigned char PYXIS_MENU_VERTICAL = 7;
const unsigned char PYXIS_MENU_END = -1;

#endif

#endif /* _JRD_GDSOLD_H_ */
