//____________________________________________________________
//  
//		PROGRAM:	General preprocessor
//		MODULE:		cob.cpp
//		DESCRIPTION:	COBOL text generator
//  
//  The contents of this file are subject to the Interbase Public
//  License Version 1.0 (the "License"); you may not use this file
//  except in compliance with the License. You may obtain a copy
//  of the License at http://www.Inprise.com/IPL.html
//  
//  Software distributed under the License is distributed on an
//  "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
//  or implied. See the License for the specific language governing
//  rights and limitations under the License.
//  
//  The Original Code was created by Inprise Corporation
//  and its predecessors. Portions created by Inprise Corporation are
//  Copyright (C) Inprise Corporation.
//  
//  All Rights Reserved.
//  Contributor(s): ______________________________________.
//  Solaris x86 changes - Konstantin Kuznetsov, Neil McCalden
//  8-Mar-2002 FSG (Frank Schlottmann-Gödde) tiny cobol support
//             fixed Bug No. 526204*
//  
//
//____________________________________________________________
//
//	$Id: cob.cpp,v 1.21 2003-03-27 17:15:46 brodsom Exp $
//
// 2002.10.27 Sean Leyne - Completed removal of obsolete "DG_X86" port
// 2002.10.27 Sean Leyne - Code Cleanup, removed obsolete "UNIXWARE" port
//
// 2002.10.28 Sean Leyne - Completed removal of obsolete "DGUX" port
// 2002.10.28 Sean Leyne - Completed removal of obsolete "HP700" port
//
//

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include "../jrd/common.h"
#include <stdarg.h>
#include "../jrd/gds.h"
#include "../gpre/gpre.h"
#include "../gpre/form.h"
#include "../gpre/pat.h"
#include "../gpre/cmp_proto.h"
#include "../gpre/lang_proto.h"
#include "../gpre/pat_proto.h"
#include "../gpre/prett_proto.h"
#include "../gpre/gpre_proto.h"

#ifdef HAVE_STRING_H
#include <string.h>
#endif

//  vms requires all caps in function call names; hp requires lc 

#ifdef VMS
#define GIVING_SUPPORTED
#define OMITTED 		"OMITTED"
#define RAW_BLR_TEMPLATE	"03  %s%d%s%d PIC S9(9) USAGE COMP VALUE IS %d."
#define RAW_TPB_TEMPLATE	"03  %s%d%s%d PIC S9(9) USAGE COMP VALUE IS %d."
#define BY_VALUE		"BY VALUE "
#define END_VALUE		""
#define BY_REF			"BY REFERENCE "
#define BY_DESC			"BY DESCRIPTOR "
#define ISC_BLOB		"ISC_%s_BLOB"
#define CLOSE			"CLOSE"
#define CANCEL			"CANCEL"
#define COMP_VALUE		"COMP"
#define ISC_CANCEL_BLOB		"ISC_CANCEL_BLOB"
#define ISC_COMPILE_REQUEST 	"ISC_COMPILE_REQUEST"
#define ISC_CREATE_DATABASE 	"ISC_CREATE_DATABASE"
#define ISC_DDL 		"ISC_DDL"
#define ISC_COMMIT_TRANSACTION	"ISC_COMMIT_TRANSACTION"
#define ISC_ROLLBACK_TRANSACTION "ISC_ROLLBACK_TRANSACTION"
#define ISC_DROP_DATABASE 	"ISC_DROP_DATABASE"
#define ISC_CLOSE 		"ISC_EMBED_DSQL_CLOSE"
#define ISC_DECLARE 		"ISC_EMBED_DSQL_DECLARE"
#define ISC_DESCRIBE 		"ISC_EMBED_DSQL_DESCRIBE"
#define ISC_DESCRIBE_BIND	"ISC_EMBED_DSQL_DESCRIBE_BIND"
#define ISC_EXECUTE 		"ISC_EMBED_DSQL_EXECUTE"
#define ISC_EXECUTE2 		"ISC_EMBED_DSQL_EXECUTE2"
#define ISC_EXECUTE_IMMEDIATE 	"ISC_EMBED_DSQL_EXECUTE_IMMED_D"
#define ISC_EXECUTE_IMMEDIATE2 	"ISC_EMBED_DSQL_EXECUTE_IMMED2_D"
#define ISC_FETCH 		"ISC_EMBED_DSQL_FETCH"
#define ISC_INSERT 		"ISC_EMBED_DSQL_INSERT"
#define ISC_OPEN		"ISC_EMBED_DSQL_OPEN"
#define ISC_OPEN2		"ISC_EMBED_DSQL_OPEN2"
#define ISC_PREPARE	 	"ISC_EMBED_DSQL_PREPARE_D"
#define ISC_DSQL_ALLOCATE	"ISC_DSQL_ALLOC_STATEMENT2"
#define ISC_DSQL_EXECUTE	"ISC_DSQL_EXECUTE_M"
#define ISC_DSQL_FREE		"ISC_DSQL_FREE_STATEMENT"
#define ISC_DSQL_SET_CURSOR	"ISC_DSQL_SET_CURSOR_NAME"
#define ISC_SQLCODE_CALL	"ISC_SQLCODE"
#define ISC_DETACH_DATABASE 	"ISC_DETACH_DATABASE"
#ifdef PYXIS
#define PYXIS_DRIVE_FORM 	"PYXIS__DRIVE_FORM"
#define PYXIS_POP_WINDOW 	"PYXIS__POP_WINDOW"
#define PYXIS_LOAD_FORM 	"PYXIS__LOAD_FORM"
#define PYXIS_COMPILE_MAP 	"PYXIS__COMPILE_MAP"
#define PYXIS_RESET_FORM 	"PYXIS__RESET_FORM"
#endif
#define ISC_GET_SLICE 		"ISC_GET_SLICE"
#define ISC_PUT_SLICE 		"ISC_PUT_SLICE"
#define ISC_GET_SEGMENT 	"ISC_GET_SEGMENT"
#ifdef PYXIS
#define PYXIS_FETCH 		"PYXIS__FETCH"
#define PYXIS_INSERT 		"PYXIS__INSERT"
#define PYXIS_COMPILE_SUB_MAP 	"PYXIS__COMPILE_SUB_MAP"
#define PYXIS_MENU 		"PYXIS__MENU"
#define PYXIS_DRIVE_MENU 	"PYXIS__DRIVE_MENU"
#define PYXIS_INITIALIZE_MENU 	"PYXIS__INITIALIZE_MENU"
#define PYXIS_GET_ENTREE 	"PYXIS__GET_ENTREE"
#define PYXIS_PUT_ENTREE 	"PYXIS__PUT_ENTREE"
#define PYXIS_GET_ENTREE 	"PYXIS__GET_ENTREE"
#endif
#define ISC_PUT_SEGMENT 	"ISC_PUT_SEGMENT"
#define ISC_RECEIVE 		"ISC_RECEIVE"
#define ISC_RELEASE_REQUEST	"ISC_RELEASE_REQUEST"
#define ISC_UNWIND_REQUEST 	"ISC_UNWIND_REQUEST"
#define ISC_SEND 		"ISC_SEND"
#define ISC_GET_SLICE 		"ISC_GET_SLICE"
#define ISC_PUT_SLICE 		"ISC_PUT_SLICE"
#define ISC_START_TRANSACTION 	"ISC_START_TRANSACTION"
#define ISC_START_AND_SEND 	"ISC_START_AND_SEND"
#define ISC_START_REQUEST 	"ISC_START_REQUEST"
#define ISC_TRANSACT_REQUEST 	"ISC_TRANSACT_REQUEST"
#define ISC_COMMIT_RETAINING 	"ISC_COMMIT_RETAINING"
#ifdef PYXIS
#define PYXIS_CREATE_WINDOW 	"PYXIS__CREATE_WINDOW"
#define PYXIS_DELETE_WINDOW 	"PYXIS__DELETE_WINDOW"
#define PYXIS_SUSPEND_WINDOW 	"PYXIS__SUSPEND_WINDOW"
#endif
#define ISC_ATTACH_DATABASE_D 	"ISC_ATTACH_DATABASE_D"
#define ISC_ATTACH_DATABASE 	"ISC_ATTACH_DATABASE"
#define ISC_MODIFY_DPB 		"ISC_MODIFY_DPB"
#define ISC_FREE		"ISC_FREE"
#define ISC_PREPARE_TRANSACTION	"ISC_PREPARE_TRANSACTION"
#define ISC_EVENT_BLOCK		"ISC_EVENT_BLOCK_A"
#define ISC_EVENT_COUNTS	"ISC_EVENT_COUNTS"
#define ISC_EVENT_WAIT		"ISC_EVENT_WAIT"
#define ISC_BADDRESS		"ISC_BADDRESS"

#else

#if (defined AIX || defined AIX_PPC)
#define MICROFOCUS
#endif
#if (defined sparc || defined SOLX86 )
#define MICROFOCUS
#endif
/* RITTER - added HP11 to the line below */
#if (defined HP10 || defined HP11)
#define MICROFOCUS
#endif

#ifdef SINIXZ
#define MICROFOCUS
#endif

#ifdef LINUX
#define MICROFOCUS
#endif

#ifdef DARWIN
#define MICROFOCUS
#endif

#if defined FREEBSD || defined NETBSD
#define MICROFOCUS
#endif

#ifdef WIN_NT
#define MICROFOCUS
#endif

#ifdef MICROFOCUS
#define OMITTED 		"BY VALUE 0"
#define BY_VALUE		"BY VALUE "
#define END_VALUE		""
#define BY_REF			"BY REFERENCE "
#define BY_DESC 		"BY REFERENCE "
#define RAW_BLR_TEMPLATE	"03  %s%d%s%d PIC XXXX USAGE COMP-X VALUE IS %u."
#define RAW_TPB_TEMPLATE	"03  %s%d%s%d PIC XXXX USAGE COMP-X VALUE IS %u."
#define COMP_VALUE		"COMP-5"
#endif

#ifndef COMP_VALUE
#define COMP_VALUE		"COMP"
#endif

#ifndef STRING_LENGTH
#define STRING_LENGTH		"\"isc_embed_dsql_length\""
#endif

#define ISC_BLOB		"isc_%s_blob"
#define CLOSE			"close"
#define CANCEL			"cancel"
#define ISC_CANCEL_BLOB		"isc_cancel_blob"
#define ISC_COMPILE_REQUEST 	"isc_compile_request"
#define ISC_CREATE_DATABASE 	"isc_create_database"
#define ISC_DDL 		"isc_ddl"
#define ISC_COMMIT_TRANSACTION	"isc_commit_transaction"
#define ISC_ROLLBACK_TRANSACTION "isc_rollback_transaction"
#define ISC_DROP_DATABASE 	"isc_drop_database"
#define ISC_CLOSE 		"isc_embed_dsql_close"
#define ISC_DECLARE 		"isc_embed_dsql_declare"
#define ISC_DESCRIBE 		"isc_embed_dsql_describe"
#define ISC_DESCRIBE_BIND	"isc_embed_dsql_describe_bind"
#define ISC_EXECUTE 		"isc_embed_dsql_execute"
#define ISC_EXECUTE2 		"isc_embed_dsql_execute2"
#define ISC_EXECUTE_IMMEDIATE 	"isc_embed_dsql_execute_immed"
#define ISC_EXECUTE_IMMEDIATE2 	"isc_embed_dsql_execute_immed2"
#define ISC_INSERT 		"isc_embed_dsql_insert"
#define ISC_OPEN		"isc_embed_dsql_open"
#define ISC_OPEN2		"isc_embed_dsql_open2"
#define ISC_PREPARE	 	"isc_embed_dsql_prepare"
#define ISC_DSQL_ALLOCATE	"isc_dsql_alloc_statement2"
#define ISC_DSQL_EXECUTE	"isc_dsql_execute_m"
#define ISC_DSQL_FREE		"isc_dsql_free_statement"
#define ISC_DSQL_SET_CURSOR	"isc_dsql_set_cursor_name"
#define ISC_COMMIT_ROLLBACK_TRANSACTION 	"isc_%s_transaction"
#define ISC_DETACH_DATABASE 	"isc_detach_database"
#ifdef PYXIS
#define PYXIS_DRIVE_FORM 	"isc_drive_form"
#define PYXIS_POP_WINDOW 	"isc_pop_window"
#define PYXIS_LOAD_FORM 	"isc_load_form"
#define PYXIS_COMPILE_MAP 	"isc_compile_map"
#define PYXIS_RESET_FORM 	"isc_reset_form"
#endif
#define ISC_GET_SLICE 		"isc_get_slice"
#define ISC_PUT_SLICE 		"isc_put_slice"
#define ISC_GET_SEGMENT 	"isc_get_segment"
#ifdef PYXIS
#define PYXIS_FETCH 		"isc_form_fetch"
#define PYXIS_INSERT 		"isc_form_insert"
#define PYXIS_COMPILE_SUB_MAP 	"isc_compile_sub_map"
#define PYXIS_MENU 		"isc_menu"
#define PYXIS_DRIVE_MENU 	"isc_form_drive_menu"
#define PYXIS_INITIALIZE_MENU 	"isc_initialize_menu"
#define PYXIS_GET_ENTREE 	"isc_get_entree"
#define PYXIS_PUT_ENTREE 	"isc_put_entree"
#define PYXIS_GET_ENTREE 	"isc_get_entree"
#endif
#define ISC_PUT_SEGMENT 	"isc_put_segment"
#define ISC_RECEIVE 		"isc_receive"
#define ISC_RELEASE_REQUEST	"isc_release_request"
#define ISC_UNWIND_REQUEST 	"isc_unwind_request"
#define ISC_SEND 		"isc_send"
#define ISC_GET_SLICE 		"isc_get_slice"
#define ISC_PUT_SLICE 		"isc_put_slice"
#define ISC_START_TRANSACTION 	"isc_start_transaction"
#define ISC_START_AND_SEND 	"isc_start_and_send"
#define ISC_START_REQUEST 	"isc_start_request"
#define ISC_TRANSACT_REQUEST 	"isc_transact_request"
#define ISC_COMMIT_RETAINING 	"isc_commit_retaining"
#ifdef PYXIS
#define PYXIS_CREATE_WINDOW 	"isc_create_window"
#define PYXIS_DELETE_WINDOW 	"isc_delete_window"
#define PYXIS_SUSPEND_WINDOW 	"isc_suspend_window"
#endif
#define ISC_ATTACH_DATABASE_D 	"isc_attach_database"
#define ISC_ATTACH_DATABASE 	"isc_attach_database"
#define ISC_MODIFY_DPB		"isc_modify_dpb"
#define ISC_FREE		"isc_free"
#ifdef GIVING_SUPPORTED
#define ISC_SQLCODE_CALL	"isc_sqlcode"
#define ISC_FETCH 		"isc_embed_dsql_fetch"
#define ISC_EVENT_BLOCK		"isc_event_block_a"
#else
#define ISC_SQLCODE_CALL 	"isc_sqlcode_s"
#define ISC_FETCH 		"isc_embed_dsql_fetch_a"
#define ISC_EVENT_BLOCK		"isc_event_block_s"
#endif
#define ISC_PREPARE_TRANSACTION	"isc_prepare_transaction"
#define ISC_EVENT_WAIT		"isc_wait_for_event"
#define ISC_EVENT_COUNTS	"isc_event_counts"
#ifdef GIVING_SUPPORTED
#define ISC_BADDRESS		"isc_baddress"
#else
#define ISC_BADDRESS		"isc_baddress_s"
#endif
#endif

#if (defined AIX || defined AIX_PPC)
#define USAGE_COMP		" USAGE IS COMP"
#endif

#if (defined sparc || defined SOLX86 )
#define USAGE_COMP		" USAGE IS COMP"
#endif

/* RITTER - added HP11 to the line below */
#if (defined HP10 || defined HP11)
#define USAGE_COMP		" USAGE IS COMP"
#endif

#ifndef FLOATS_COMPS_DECLARED
#define FLOATS_COMPS_DECLARED
#define DCL_FLOAT		"USAGE IS COMP-1"
#define DCL_DOUBLE		"USAGE IS COMP-2"
#endif

#ifndef USAGE_COMP
#define USAGE_COMP		" USAGE IS COMP"
#endif

extern DBB isc_databases;
extern GPRE_REQ requests;
extern IB_FILE *out_file;


static void	align(int column);
static void	asgn_from (ACT, REF);
static void	asgn_to (ACT, REF);
static void	asgn_to_proc (REF);
static void	gen_any (ACT);
static void	gen_at_end (ACT);
static void	gen_based (ACT);
static void	gen_blob_close (ACT);
static void	gen_blob_end (ACT);
static void	gen_blob_for (ACT);
static void	gen_blob_open (ACT);
static int	gen_blr (int *, int, TEXT *);
static void	gen_clear_handles (ACT);
static void	gen_compile (ACT);
static void	gen_create_database (ACT);
static void	gen_cursor_close (ACT, GPRE_REQ);
static void	gen_cursor_init (ACT);
static void	gen_cursor_open (ACT, GPRE_REQ);
static void	gen_database (ACT);
static void	gen_ddl (ACT);
static void	gen_dyn_close (ACT);
static void	gen_dyn_declare (ACT);
static void	gen_dyn_describe (ACT, BOOLEAN);
static void	gen_dyn_execute (ACT);
static void	gen_dyn_fetch (ACT);
static void	gen_dyn_immediate (ACT);
static void	gen_dyn_insert (ACT);
static void	gen_dyn_open (ACT);
static void	gen_dyn_prepare (ACT);
static void	gen_emodify (ACT);
static void	gen_estore (ACT);
static void	gen_end_fetch (ACT);
static void	gen_endfor (ACT);
static void	gen_erase (ACT);
static SSHORT	gen_event_block (ACT);
static void	gen_event_init (ACT);
static void	gen_event_wait (ACT);
static void	gen_fetch (ACT);
static void	gen_finish (ACT);
static void	gen_for (ACT);
#ifdef PYXIS
static void	gen_form_display (ACT);
static void	gen_form_end (ACT);
static void	gen_form_for (ACT);
#endif
static void	gen_function (ACT);
static void	gen_get_or_put_slice (ACT, REF, BOOLEAN);
static void	gen_get_segment (ACT);
#ifdef PYXIS
static void	gen_item_end (ACT);
static void	gen_item_for (ACT);
#endif
static void	gen_loop (ACT);
#ifdef PYXIS
static void	gen_menu (ACT);
static void	gen_menu_display (ACT);
static void	gen_menu_end (ACT);
static void	gen_menu_entree (ACT);
static void	gen_menu_entree_att (ACT);
static void	gen_menu_for (ACT);
static void	gen_menu_item_end (ACT);
static void	gen_menu_item_for (ACT);
static void	gen_menu_request (GPRE_REQ);
#endif
static TEXT	*gen_name (TEXT *, REF, BOOLEAN);
static void	gen_on_error (ACT);
static void	gen_procedure (ACT);
static void	gen_put_segment (ACT);
static void	gen_raw (UCHAR *, enum req_t, int, int);
static void	gen_ready (ACT);
static void	gen_receive (ACT, POR);
static void	gen_release (ACT);
static void	gen_request (GPRE_REQ);
static void	gen_s_end (ACT);
static void	gen_s_fetch (ACT);
static void	gen_s_start (ACT);
static void	gen_segment (ACT);
static void	gen_select (ACT);
static void	gen_send (ACT, POR);
static void	gen_slice (ACT);
static void	gen_start (ACT, POR);
static void	gen_store (ACT);
static void	gen_t_start (ACT);
static void	gen_tpb (TPB);
static void	gen_trans (ACT);
static void	gen_type (ACT);
static void	gen_update (ACT);
static void	gen_variable (ACT);
static void	gen_whenever (SWE);
#ifdef PYXIS
static void	gen_window_create (ACT);
static void	gen_window_delete (ACT);
static void	gen_window_suspend (ACT);
#endif
static void	make_array_declaration (REF);
static TEXT	*make_name (TEXT *, SYM);
static TEXT	*make_name_formatted (TEXT *, TEXT *, SYM);
static void	make_port (POR);
static void	make_ready (DBB, TEXT *, TEXT *, GPRE_REQ, USHORT);
static void	printa (TEXT *, BOOLEAN, TEXT *, ...) ATTRIBUTE_FORMAT(3,4);
static void	printb (TEXT *, ... ) ATTRIBUTE_FORMAT(1,2);
static TEXT	*request_trans (ACT, GPRE_REQ);
static void	set_sqlcode (ACT);
static TEXT	*status_vector (ACT);
static void	t_start_auto (GPRE_REQ, TEXT *, ACT, SSHORT);

static TEXT output_buffer[512];
static int first_flag;
static TEXT **names;

static TEXT *vnames[] = {
	"_",
	"ISC_",
	"isc_",
	"isc_blob_null",
	"ISC_TPB_",
	"ISC_TRANS",
	"ISC_STATUS_VECTOR",
	"ISC_STATUS",
	"ISC_STATUS_VECTOR2",
	"ISC_STATUS2",
	"ISC_WINDOW",
	"ISC_WIDTH",
	"ISC_HEIGHT",
	"RDB$K_DB_TYPE_GDS",
	"ISC_ARRAY_LENGTH",
	"     ",					/* column */
	"*    ",					/* comment */
	"-    ",					/* continue */
	"-      \"",				/* continue quote */
	"-      \'",				/* continue single quote */
	"",							/* column0 */
	"        ",					/* column indent */
	"ISC_SQLCODE",
	"ISC_EVENTS_VECTOR",
	"ISC_EVENTS",
	"ISC_EVENT_NAMES_VECTOR",
	"ISC_EVENT_NAMES",
	"ISC_EVENT_NAMES_VECTOR2",
	"ISC_EVENT_NAMES2",
	NULL
};

static TEXT *anames[] = {
	"-",
	"ISC-",
	"isc-",
	"isc-blob-null",
	"ISC-TPB-",
	"ISC-TRANS",
	"ISC-STATUS-VECTOR",
	"ISC-STATUS",
	"ISC-STATUS-VECTOR2",
	"ISC-STATUS2",
	"ISC-WINDOW",
	"ISC-WIDTH",
	"ISC-HEIGHT",
	"RDB-K-DB-TYPE-GDS",
	"ISC-ARRAY-LENGTH",
	"           ",				/* column */
	"      *    ",				/* comment */
	"            ",				/* continue */
	"      -      \"",			/* continue quote */
	"      -      \'",			/* continue single quote */
	"           ",				/* column0 */
	"                ",			/* column indent */
	"ISC-SQL-CODE",
	"ISC-EVENTS-VECTOR",
	"ISC-EVENTS",
	"ISC-EVENT-NAMES-VECTOR",
	"ISC-EVENT-NAMES",
	"ISC-EVENT-NAMES-VECTOR2",
	"ISC-EVENT-NAMES2",
	NULL
};

#define	UNDER			0
#define	ISC_			1
#define	isc_			2
#define isc_blob_null		3
#define	ISC_TPB_		4
#define	ISC_TRANS 		5
#define	ISC_STATUS_VECTOR	6
/* replace definition of ISC_STATUS from gds.h */
#undef  ISC_STATUS
#define	ISC_STATUS		7
#define	ISC_STATUS_VECTOR2	8
#define	ISC_STATUS2		9
#define	ISC_WINDOW		10
#define	ISC_WIDTH		11
#define	ISC_HEIGHT		12
#define	RDB$K_DB_TYPE_GDS	13
#define	ISC_ARRAY_LENGTH	14
#define COLUMN 			15
#define COMMENT			16
#define CONTINUE		17
#define CONTINUE_QUOTE		18
#define CONTINUE_SINGLE_QUOTE	19
#define COLUMN_0		20
#define COLUMN_INDENT		21
#define ISC_SQLCODE		22
#define	ISC_EVENTS_VECTOR	23
#define	ISC_EVENTS		24
#define	ISC_EVENT_NAMES_VECTOR	25
#define	ISC_EVENT_NAMES		26
#define	ISC_EVENT_NAMES_VECTOR2	27
#define	ISC_EVENT_NAMES2	28

#define INDENT		"   "


//____________________________________________________________
//  
//  

void COB_action( ACT action, int column)
{

	if (action->act_flags & ACT_break)
		first_flag = FALSE;

	switch (action->act_type) {
	case ACT_alter_database:
	case ACT_alter_domain:
	case ACT_alter_table:
	case ACT_alter_index:
	case ACT_create_domain:
	case ACT_create_generator:
	case ACT_create_index:
	case ACT_create_shadow:
	case ACT_create_table:
	case ACT_create_view:
	case ACT_declare_filter:
	case ACT_declare_udf:
	case ACT_statistics:
	case ACT_drop_domain:
	case ACT_drop_filter:
	case ACT_drop_index:
	case ACT_drop_shadow:
	case ACT_drop_table:
	case ACT_drop_udf:
	case ACT_drop_view:
		gen_ddl(action);
		break;
	case ACT_any:
		gen_any(action);
		return;
	case ACT_at_end:
		gen_at_end(action);
		return;
	case ACT_commit:
		gen_trans(action);
		break;
	case ACT_commit_retain_context:
		gen_trans(action);
		break;
	case ACT_basedon:
		gen_based(action);
		return;
	case ACT_b_declare:
		gen_database(action);
		return;
	case ACT_blob_cancel:
		gen_blob_close(action);
		return;
	case ACT_blob_close:
		gen_blob_close(action);
		break;
	case ACT_blob_create:
		gen_blob_open(action);
		break;
	case ACT_blob_for:
		gen_blob_for(action);
		return;
	case ACT_blob_handle:
		gen_segment(action);
		return;
	case ACT_blob_open:
		gen_blob_open(action);
		break;
	case ACT_clear_handles:
		gen_clear_handles(action);
		break;
	case ACT_close:
		gen_s_end(action);
		break;
	case ACT_create_database:
		gen_create_database(action);
		break;
	case ACT_cursor:
		gen_cursor_init(action);
		return;
	case ACT_database:
		gen_database(action);
		return;
	case ACT_disconnect:
		gen_finish(action);
		break;
	case ACT_dyn_close:
		gen_dyn_close(action);
		break;
	case ACT_dyn_cursor:
		gen_dyn_declare(action);
		break;
	case ACT_dyn_describe:
		gen_dyn_describe(action, FALSE);
		break;
	case ACT_dyn_describe_input:
		gen_dyn_describe(action, TRUE);
		break;
	case ACT_dyn_execute:
		gen_dyn_execute(action);
		break;
	case ACT_dyn_fetch:
		gen_dyn_fetch(action);
		break;
	case ACT_dyn_grant:
		gen_ddl(action);
		break;
	case ACT_dyn_immediate:
		gen_dyn_immediate(action);
		break;
	case ACT_dyn_insert:
		gen_dyn_insert(action);
		break;
	case ACT_dyn_open:
		gen_dyn_open(action);
		break;
	case ACT_dyn_prepare:
		gen_dyn_prepare(action);
		break;
	case ACT_dyn_revoke:
		gen_ddl(action);
		break;
	case ACT_endblob:
		gen_blob_end(action);
		return;
	case ACT_enderror:
		sprintf(output_buffer, "%sEND-IF", names[COLUMN]);
		COB_print_buffer(output_buffer, FALSE);
		return;
	case ACT_endfor:
		gen_endfor(action);
		break;
	case ACT_endmodify:
		gen_emodify(action);
		break;
	case ACT_endstore:
		gen_estore(action);
		break;
	case ACT_erase:
		gen_erase(action);
		return;
	case ACT_event_init:
		gen_event_init(action);
		return;
	case ACT_event_wait:
		gen_event_wait(action);
		return;
	case ACT_fetch:
		gen_fetch(action);
		break;
	case ACT_finish:
		gen_finish(action);
		break;
	case ACT_for:
		gen_for(action);
		return;
#ifdef PYXIS
	case ACT_form_display:
		gen_form_display(action);
		break;
	case ACT_form_end:
		gen_form_end(action);
		break;
	case ACT_form_for:
		gen_form_for(action);
		return;
#endif
	case ACT_function:
		gen_function(action);
		return;
	case ACT_get_segment:
		gen_get_segment(action);
		break;
	case ACT_get_slice:
		gen_slice(action);
		return;
	case ACT_hctef:
		gen_end_fetch(action);
		return;
	case ACT_insert:
		gen_s_start(action);
		break;
#ifdef PYXIS
	case ACT_item_for:
	case ACT_item_put:
		gen_item_for(action);
		return;
	case ACT_item_end:
		gen_item_end(action);
		break;
#endif
	case ACT_loop:
		gen_loop(action);
		break;
#ifdef PYXIS
	case ACT_menu:
		gen_menu(action);
		return;
	case ACT_menu_end:
		gen_menu_end(action);
		break;
	case ACT_menu_entree:
		gen_menu_entree(action);
		return;
	case ACT_menu_display:
		gen_menu_display(action);
		return;
	case ACT_menu_for:
		gen_menu_for(action);
		return;

	case ACT_title_text:
	case ACT_title_length:
	case ACT_terminator:
	case ACT_entree_text:
	case ACT_entree_length:
	case ACT_entree_value:
		gen_menu_entree_att(action);
		return;
#endif
	case ACT_open:
		gen_s_start(action);
		break;
	case ACT_on_error:
		gen_on_error(action);
		return;
	case ACT_prepare:
		gen_trans(action);
		break;
	case ACT_procedure:
		gen_procedure(action);
		break;
	case ACT_put_segment:
		gen_put_segment(action);
		break;
	case ACT_put_slice:
		gen_slice(action);
		return;
	case ACT_ready:
		gen_ready(action);
		return;
	case ACT_release:
		gen_release(action);
		break;
	case ACT_rfinish:
		gen_finish(action);
		return;
	case ACT_rollback:
		gen_trans(action);
		break;
	case ACT_routine:
		return;
	case ACT_s_end:
		gen_s_end(action);
		return;
	case ACT_s_fetch:
		gen_s_fetch(action);
		break;
	case ACT_s_start:
		gen_s_start(action);
		break;
	case ACT_select:
		gen_select(action);
		break;
	case ACT_segment_length:
		gen_segment(action);
		return;
	case ACT_segment:
		gen_segment(action);
		return;
	case ACT_sql_dialect:
		sw_sql_dialect = ((SDT) action->act_object)->sdt_dialect;
		return;
	case ACT_start:
		gen_t_start(action);
		break;
	case ACT_store:
		gen_store(action);
		break;
	case ACT_type:
		gen_type(action);
		return;
	case ACT_update:
		gen_update(action);
		break;
	case ACT_variable:
		gen_variable(action);
		return;
#ifdef PYXIS
	case ACT_window_create:
		gen_window_create(action);
		return;
	case ACT_window_delete:
		gen_window_delete(action);
		return;
	case ACT_window_suspend:
		gen_window_suspend(action);
		return;
#endif
	default:
		return;
	}

	if ((action->act_flags & ACT_sql) && action->act_whenever)
		gen_whenever(action->act_whenever);
	else
		ib_fprintf(out_file, names[COLUMN]);
}


//____________________________________________________________
//  
//		point 'names' at the appropriate list
//		ANSI has no underscores.
//  

void COB_name_init( BOOLEAN ansi)
{

	names = ansi ? anames : vnames;
}


//____________________________________________________________
//  
//		Print a statment, breaking it into
//		reasonable 80 character hunks.  This
//		function now works for COBOL statements
//		which are function calls and non-function
//		calls.
//  

void COB_print_buffer( TEXT * output_buffer, BOOLEAN function_call)
{
	TEXT s[80], *p, *q, *tempq;
	BOOLEAN open_quote, single_quote, save_open_quote, save_single_quote;
	USHORT max_line;

	if (sw_ansi)
		max_line = 72;
	else
		max_line = 79;

	open_quote = FALSE;
	single_quote = FALSE;
	p = s;

	for (q = output_buffer; *q; q++) {
		*p++ = *q;

		/*  If we have a single or double quote, toggle the
		   quote switch and indicate single or double quote  */
		if (*q == '\"') {
			open_quote = (open_quote) ? FALSE : TRUE;
			single_quote = FALSE;
		}
		else if (*q == '\'') {
			open_quote = (open_quote) ? FALSE : TRUE;
			single_quote = TRUE;
		}

		if ((p - s) > max_line) {
			tempq = q;
			save_open_quote = open_quote;
			save_single_quote = single_quote;
			if (function_call) {
				/*  Back up until we reach a comma  */
				for (p--; (p > s); p--, q--) {
					if (*(p + 1) == '\"' || *(p + 1) == '\'') {
						/*  If we have a single or double quote, toggle the
						   quote switch and indicate single or double quote  */
						open_quote = (open_quote) ? FALSE : TRUE;
						if (open_quote)
							single_quote = (*(p + 1) == '\'') ? TRUE : FALSE;
						else
							single_quote = FALSE;
					}
					if (!open_quote && (*p == ','))
						break;
				}
				/* if p == s, this is a call with no commas. back up to a blank */
				if (p == s) {
					q = tempq;
					p = s + max_line;
					open_quote = save_open_quote;
					single_quote = save_single_quote;
					for (p--; p > s; p--, q--) {
						if (*(p + 1) == '\"' || *(p + 1) == '\'') {
							/*  If we have a single or double quote, toggle the
							   quote switch and indicate single or double quote  */
							open_quote = (open_quote) ? FALSE : TRUE;
							if (open_quote)
								single_quote =
									(*(p + 1) == '\'') ? TRUE : FALSE;
							else
								single_quote = FALSE;
						}
						if (!open_quote && (*p == ' '))
							break;
					}
					q--;
					if (p == s) {
						q = tempq - 1;
						p = s + max_line;
						open_quote = save_open_quote;
						single_quote = save_single_quote;
					}
					*p = 0;
				}
				else
					*++p = 0;
			}
			else {
				/* back up to a blank */
				for (p--; p > s; p--, q--) {
					if (*(p + 1) == '\"' || *(p + 1) == '\'') {
						/* If we have a single or double quote, toggle the
						   quote switch and indicate single or double quote  */
						open_quote = (open_quote) ? FALSE : TRUE;
						if (open_quote)
							single_quote = (*(p + 1) == '\'') ? TRUE : FALSE;
						else
							single_quote = FALSE;
					}
					if (!open_quote && (*p == ' '))
						break;
				}
				q--;
				if (p == s) {
					q = tempq - 1;
					p = s + max_line;
					open_quote = save_open_quote;
					single_quote = save_single_quote;
				}
				*p = 0;
			}

			ib_fprintf(out_file, "%s\n", s);
			if (open_quote) {
				if (single_quote)
					strcpy(s, names[CONTINUE_SINGLE_QUOTE]);
				else
					strcpy(s, names[CONTINUE_QUOTE]);
			}
			else
				strcpy(s, names[CONTINUE]);
			for (p = s; *p; p++);
		}
	}
	*p = 0;
	ib_fprintf(out_file, "%s", s);
	output_buffer[0] = 0;
}


//____________________________________________________________
//  
//       Align output to a specific column for output.  If the
//       column is negative, don't do anything.
//  

static void align( int column)
{
	int i;

	if (column < 0)
		return;

	ib_putc('\n', out_file);

	for (i = column / 8; i; --i)
		ib_putc('\t', out_file);

	for (i = column % 8; i; --i)
		ib_putc(' ', out_file);
}



//____________________________________________________________
//  
//		Build an assignment from a host language variable to
//		a port variable.
//  

static void asgn_from( ACT action, REF reference)
{
	GPRE_FLD field;
	TEXT *value, name[64], variable[20], temp[20];

	for (; reference; reference = reference->ref_next) {
		field = reference->ref_field;
		if (field->fld_array_info)
			if (!(reference->ref_flags & REF_array_elem)) {
				printa(names[COLUMN], TRUE, "CALL \"isc_qtoq\" USING %s, %s",
					   names[isc_blob_null], gen_name(name, reference, TRUE));
				gen_get_or_put_slice(action, reference, FALSE);
				continue;
			}

		if (!reference->ref_source && !reference->ref_value)
			continue;
		gen_name(variable, reference, TRUE);
		if (reference->ref_source)
			value = gen_name(temp, reference->ref_source, TRUE);
		else
			value = reference->ref_value;
		if (!reference->ref_master || (reference->ref_flags & REF_literal))
			sprintf(output_buffer, "%sMOVE %s TO %s\n",
					names[COLUMN], value, variable);
		else {
			sprintf(output_buffer, "%sIF %s < 0 THEN\n",
					names[COLUMN], value);
			COB_print_buffer(output_buffer, FALSE);
			sprintf(output_buffer, "%sMOVE -1 TO %s\n", names[COLUMN_INDENT],
					variable);
			COB_print_buffer(output_buffer, FALSE);
			sprintf(output_buffer, "%sELSE\n", names[COLUMN]);
			COB_print_buffer(output_buffer, FALSE);
			sprintf(output_buffer, "%sMOVE 0 TO %s\n", names[COLUMN_INDENT],
					variable);
			COB_print_buffer(output_buffer, FALSE);
			sprintf(output_buffer, "%sEND-IF\n", names[COLUMN]);
		}
		COB_print_buffer(output_buffer, FALSE);
	}
}

//____________________________________________________________
//  
//		Build an assignment to a host language variable from
//		a port variable.
//  

static void asgn_to( ACT action, REF reference)
{
	GPRE_FLD field;
	REF source;
	TEXT s[64];

	source = reference->ref_friend;
	field = source->ref_field;
	gen_name(s, source, TRUE);

	if (field->fld_array_info) {
		source->ref_value = reference->ref_value;
		gen_get_or_put_slice(action, source, TRUE);
		return;
	}

	field = reference->ref_field;
	sprintf(output_buffer, "%sMOVE %s TO %s\n",
			names[COLUMN], s, reference->ref_value);
	COB_print_buffer(output_buffer, FALSE);

//  Pick up NULL value if one is there 

	if (reference = reference->ref_null) {
		sprintf(output_buffer, "%sMOVE %s TO %s\n",
				names[COLUMN], gen_name(s, reference, TRUE),
				reference->ref_value);
		COB_print_buffer(output_buffer, FALSE);
	}
}


//____________________________________________________________
//  
//		Build an assignment to a host language variable from
//		a port variable.
//  

static void asgn_to_proc( REF reference)
{
	GPRE_FLD field;
	TEXT s[64];

	for (; reference; reference = reference->ref_next) {
		if (!reference->ref_value)
			continue;
		field = reference->ref_field;
		gen_name(s, reference, TRUE);
		sprintf(output_buffer, "%sMOVE %s TO %s\n",
				names[COLUMN], s, reference->ref_value);
		COB_print_buffer(output_buffer, FALSE);
	}
}


//____________________________________________________________
//  
//		Generate a function call for free standing ANY.  Somebody else
//		will need to generate the actual function.
//  

static void gen_any( ACT action)
{
	GPRE_REQ request;
	POR port;
	REF reference;

	request = action->act_request;

	ib_fprintf(out_file, "%s_r (&%s, &%s",
			   request->req_handle, request->req_handle, request->req_trans);

	if (port = request->req_vport)
		for (reference = port->por_references; reference;
			 reference = reference->ref_next)
				ib_fprintf(out_file, ", %s", reference->ref_value);

	ib_fprintf(out_file, ")");
}


//____________________________________________________________
//  
//		Generate code for AT END clause of FETCH.
//  

static void gen_at_end( ACT action)
{
	GPRE_REQ request;
	TEXT s[20];

	request = action->act_request;
	printa(names[COLUMN], FALSE,
		   "IF %s = 0 THEN", gen_name(s, request->req_eof, TRUE));
	ib_fprintf(out_file, names[COLUMN]);
}


//____________________________________________________________
//  
//		Substitute for a BASED ON <field name> clause.
//  

static void gen_based( ACT action)
{
	BAS based_on;
	GPRE_FLD field;
	TEXT s[64];
	USHORT datatype;
	SLONG length;
	SSHORT digits;

	based_on = (BAS) action->act_object;
	field = based_on->bas_field;

	if (based_on->bas_flags & BAS_segment) {
		datatype = dtype_text;
		if (!(length = field->fld_seg_length))
			length = 256;
	}
	else if (field->fld_array_info) {
		IBERROR("Based on currently not implemented for arrays.");
//  
//     TBD - messy
//   datatype = field->fld_array_info->ary_dtype;
//   for (dimension = field->fld_array_info->ary_dimension; dimension;
//  dimension = dimension->dim_next)
// { 
// ib_fprintf (out_file, "
//  
	}
	else
		datatype = field->fld_dtype;

	switch (datatype) {
	case dtype_short:
	case dtype_long:
		digits = (datatype == dtype_short) ? 4 : 9;
		ib_fprintf(out_file, "%sPIC S", names[COLUMN]);
		if (field->fld_scale >= -digits && field->fld_scale <= 0) {
			if (field->fld_scale > -digits)
				ib_fprintf(out_file, "9(%d)", digits + field->fld_scale);
			if (field->fld_scale)
				ib_fprintf(out_file, "V9(%d)",
						   digits - (digits + field->fld_scale));
			ib_fprintf(out_file, USAGE_COMP);
		}
		else if (field->fld_scale > 0)
			ib_fprintf(out_file, "9(%d)P(%d)", digits, field->fld_scale);
		else
			ib_fprintf(out_file, "VP(%d)9(%d)", -(field->fld_scale + digits),
					   digits);
		break;

	case dtype_date:
	case dtype_blob:
		ib_fprintf(out_file, "%sPIC S9(18)%s", names[COLUMN], USAGE_COMP);
		break;

	case dtype_quad:
		ib_fprintf(out_file, "%sPIC S9(", names[COLUMN]);
		ib_fprintf(out_file, "%d)", 18 + field->fld_scale);
		if (field->fld_scale < 0)
			ib_fprintf(out_file, "V9(%d)", -field->fld_scale);
		else if (field->fld_scale > 0)
			ib_fprintf(out_file, "P(%d)", field->fld_scale);
		ib_fprintf(out_file, USAGE_COMP);
		break;

	case dtype_text:
		ib_fprintf(out_file, "%sPIC X(%d)", names[COLUMN], field->fld_length);
		break;

	case dtype_float:
		ib_fprintf(out_file, "%s%s", names[COLUMN], DCL_FLOAT);
		break;

	case dtype_double:
		ib_fprintf(out_file, "%s%s", names[COLUMN], DCL_DOUBLE);
		break;

	default:
		sprintf(s, "datatype %d unknown\n", field->fld_dtype);
		CPR_error(s);
		return;
	}

	if (*based_on->bas_terminator == '.')
		ib_fprintf(out_file, "%s\n", based_on->bas_terminator);
}


//____________________________________________________________
//  
//		Make a blob FOR loop.
//  

static void gen_blob_close( ACT action)
{
	TEXT *command, buffer[80];
	BLB blob;

	if (action->act_flags & ACT_sql) {
		gen_cursor_close(action, action->act_request);
		blob = (BLB) action->act_request->req_blobs;
	}
	else
		blob = (BLB) action->act_object;

	command = (action->act_type == ACT_blob_cancel) ? (TEXT*) CANCEL : (TEXT*) CLOSE;
	sprintf(buffer, ISC_BLOB, command);

	printa(names[COLUMN], TRUE, "CALL \"%s\" USING %s, %s%s%d",
		   buffer,
		   status_vector(action), BY_REF, names[ISC_], blob->blb_ident);

	set_sqlcode(action);
}


//____________________________________________________________
//  
//		End a blob FOR loop.
//  

static void gen_blob_end( ACT action)
{
	BLB blob;

	blob = (BLB) action->act_object;
	gen_get_segment(action);
	printa(names[COLUMN], FALSE, "END-PERFORM");
	if (action->act_error)
		printa(names[COLUMN], TRUE,
			   "%sCALL \"%s\" USING %s%s, %s%s%d",
			   INDENT,
			   ISC_CANCEL_BLOB,
			   BY_REF, names[ISC_STATUS_VECTOR2],
			   BY_REF, names[ISC_], blob->blb_ident);
	else
		printa(names[COLUMN], TRUE,
			   "%sCALL \"%s\" USING %s, %s%s%d",
			   INDENT, ISC_CANCEL_BLOB,
			   status_vector(0), BY_REF, names[ISC_], blob->blb_ident);
}


//____________________________________________________________
//  
//		Make a blob FOR loop.
//  

static void gen_blob_for( ACT action)
{

	gen_blob_open(action);
	gen_get_segment(action);
	printa(names[COLUMN], FALSE, "PERFORM UNTIL");

	printa(names[COLUMN], FALSE, "%s%s(2) NOT = 0 AND %s(2) NOT = 335544366",
		   INDENT, names[ISC_STATUS], names[ISC_STATUS]);
}


//____________________________________________________________
//  
//		Generate the call to open (or create) a blob.
//  

static void gen_blob_open( ACT action)
{
	BLB blob;
	USHORT column;
	PAT args;
	REF reference;
	TEXT s[20];
#ifdef VMS
	TEXT *pattern1 =
		"CALL \"ISC_%IFCREATE%ELOPEN%EN_BLOB2\" USING %V1, %RF%DH%RE, %RF%RT%RE, %RF%BH%RE, %RF%FR%RE, %VF%N1%VE, %RF%I1%RE\n",
		*pattern2 =
		"CALL \"ISC_%IFCREATE%ELOPEN%EN_BLOB2\" USING %V1, %RF%DH%RE, %RF%RT%RE, %RF%BH%RE, %RF%FR%RE, %VF0%VE, %VF0%VE\n";
#else
	TEXT *pattern1 =
		"CALL \"isc_%IFcreate%ELopen%EN_blob2\" USING %V1, %RF%DH%RE, %RF%RT%RE, %RF%BH%RE, %RF%FR%RE, %VF%N1%VE, %RF%I1%RE\n",
		*pattern2 =
		"CALL \"isc_%IFcreate%ELopen%EN_blob2\" USING %V1, %RF%DH%RE, %RF%RT%RE, %RF%BH%RE, %RF%FR%RE, %VF0%VE, %VF0%VE\n";
#endif

	if (sw_auto && (action->act_flags & ACT_sql)) {
		t_start_auto(action->act_request, status_vector(action), action,
					 TRUE);
		printa(names[COLUMN], FALSE, "IF %s NOT = 0 THEN",
			   request_trans(action, action->act_request));
	}

	if (action->act_flags & ACT_sql) {
		gen_cursor_open(action, action->act_request);
		blob = (BLB) action->act_request->req_blobs;
		reference = ((OPN) action->act_object)->opn_using;
		gen_name(s, reference, TRUE);
	}
	else {
		blob = (BLB) action->act_object;
		reference = blob->blb_reference;
	}

	column = strlen(names[COLUMN]);
	args.pat_condition = action->act_type == ACT_blob_create;	/* open or create blob  */
	args.pat_vector1 = status_vector(action);	/* status vector        */
	args.pat_database = blob->blb_request->req_database;	/* database handle      */
	args.pat_request = blob->blb_request;	/* transaction handle   */
	args.pat_blob = blob;		/* blob handle          */
	args.pat_reference = reference;	/* blob identifier      */
	args.pat_ident1 = blob->blb_bpb_ident;	/* blob parameter block */

	if ((action->act_flags & ACT_sql) && action->act_type == ACT_blob_open)
		printa(names[COLUMN], FALSE, "MOVE %s TO %s",
			   reference->ref_value, s);

	if (args.pat_value1 = blob->blb_bpb_length)
		PATTERN_expand(column, pattern1, &args);
	else
		PATTERN_expand(column, pattern2, &args);

	if (action->act_flags & ACT_sql) {
		printa(names[COLUMN], FALSE, "END-IF");
		printa(names[COLUMN], FALSE, "END-IF");
		printa(names[COLUMN], FALSE, "END-IF");
		printa(names[COLUMN], FALSE, "END-IF");
		if (sw_auto)
			printa(names[COLUMN], FALSE, "END-IF");
		set_sqlcode(action);
		if (action->act_type == ACT_blob_create) {
			printa(names[COLUMN], FALSE, "IF SQLCODE = 0 THEN");
			printa(names[COLUMN], FALSE, "MOVE %s TO %s",
				   s, reference->ref_value);
			printa(names[COLUMN], FALSE, "END-IF");
		}
	}
}


//____________________________________________________________
//  
//		Callback routine for BLR pretty printer.
//  

static int gen_blr( int *user_arg, int offset, TEXT * string)
{
	int indent, length, comment, i, max_line, max_diff;
	TEXT *p, *q, c;
	BOOLEAN open_quote, first_line;

	first_line = TRUE;
	if (sw_ansi) {
		max_line = 70;
		max_diff = 7;
	}
	else {
		max_line = 255;
		max_diff = 30;
	}
	comment = strlen(names[COMMENT]);
	indent = 2 * strlen(INDENT);
	p = string;
	while (*p == ' ') {
		p++;
		indent++;
	}
	length = strlen(p);

	if (comment + indent > max_line)
		indent = max_line - comment;

	while (length > 0 && length + indent + comment > max_line) {
		/* if we did not find somewhere to break between the 200th and 256th
		   character just print out 256 characters */

		for (q = p, open_quote = FALSE; (q - p + indent + comment) < max_line;
			 q++) {
			if ((q - p + indent + comment) > max_line - max_diff && *q == ','
				&& !open_quote) break;
			if (*q == '\'' && *(q - 1) != '\\')
				open_quote = !open_quote;
		}
		c = *++q;
		*q = 0;
		ib_fprintf(out_file, "%s", names[COMMENT]);
		for (i = 0; i < indent; i++)
			ib_fputc(' ', out_file);
		ib_fprintf(out_file, "%s\n", p);
		*q = c;
		length = length - (q - p);
		p = q;
		if (first_line) {
			first_line = FALSE;
			indent += strlen(INDENT);
			if (comment + indent > max_line)
				indent = max_line - comment;
		}
	}

	ib_fprintf(out_file, "%s", names[COMMENT]);
	for (i = 0; i < indent; i++)
		ib_fputc(' ', out_file);
	ib_fprintf(out_file, "%s\n", p);
	return TRUE;
}


//____________________________________________________________
//  
//		Zap all know handles.
//  

static void gen_clear_handles( ACT action)
{
	GPRE_REQ request;

	for (request = requests; request; request = request->req_next) {
		if (!(request->req_flags & REQ_exp_hand))
			printa(names[COLUMN], TRUE, "%s = 0;", request->req_handle);
#ifdef PYXIS
		if (request->req_form_handle &&
			!(request->req_flags & REQ_exp_form_handle))
				printa(names[COLUMN], (USHORT) "%s = 0;", request->req_form_handle);
#endif
	}
}


//____________________________________________________________
//  
//		Generate text to compile a request.
//  

static void gen_compile( ACT action)
{
	GPRE_REQ request;
	DBB db;
	SYM symbol;
	BLB blob;

	request = action->act_request;
	db = request->req_database;
	symbol = db->dbb_name;

//  generate automatic ready if appropriate 

	if (sw_auto)
		t_start_auto(request, status_vector(action), action, TRUE);

//  
//  always generate a compile, a test for the success of the compile,
//  and an end to the 'if not compiled test
//  

//  generate an 'if not compiled' 

	printa(names[COLUMN], FALSE, "IF %s = 0 THEN", request->req_handle);

	if (sw_auto && action->act_error)
		printa(names[COLUMN], FALSE, "IF %s NOT = 0 THEN",
			   request_trans(action, request));

	sprintf(output_buffer,
			"%sCALL \"%s%s\" USING %s, %s%s, %s%s, %s%d%s, %s%s%d\n",
			names[COLUMN], ISC_COMPILE_REQUEST,
			(request->req_flags & REQ_exp_hand) ? "" : "2",
			status_vector(action), BY_REF, symbol->sym_string, BY_REF,
			request->req_handle, BY_VALUE, request->req_length, END_VALUE,
			BY_REF, names[ISC_], request->req_ident);

	COB_print_buffer(output_buffer, TRUE);
	if (sw_auto && action->act_error)
		printa(names[COLUMN], FALSE, "END-IF");

	set_sqlcode(action);
	printa(names[COLUMN], FALSE, "END-IF");

//  If blobs are present, zero out all of the blob handles.  After this
//  point, the handles are the user's responsibility 

	if (blob = request->req_blobs)
		for (; blob; blob = blob->blb_next) {
			sprintf(output_buffer, "%sMOVE 0 TO %s%d\n",
					names[COLUMN], names[ISC_], blob->blb_ident);

			COB_print_buffer(output_buffer, FALSE);
		}
}


//____________________________________________________________
//  
//		Generate a call to create a database.
//  

static void gen_create_database( ACT action)
{
	GPRE_REQ request;
	DBB db, dbisc;
	TEXT s1[32], s1Tmp[32], s2[32], s2Tmp[32], db_name[128];
	USHORT save_sw_auto;

	request = ((MDBB) action->act_object)->mdbb_dpb_request;
	db = (DBB) request->req_database;
	if (request) {
		sprintf(s1, "%s%dL", names[isc_], request->req_ident);
		if (request->req_flags & REQ_extend_dpb)
			sprintf(s2, "%s%dp", names[isc_], request->req_ident);
		else
			sprintf(s2, "%s%d", names[isc_], request->req_ident);

		/* if the dpb needs to be extended at runtime to include items
		   in host variables, do so here; this assumes that there is 
		   always a request generated for runtime variables */

		if (request->req_flags & REQ_extend_dpb) {
			if (request->req_length) {
				sprintf(output_buffer, "%sMOVE isc_%d to %s\n",
						names[COLUMN], request->req_ident, s2);
			}
			if (db->dbb_r_user) {
				sprintf(output_buffer,
						"%sCALL \"%s\" USING %s%s, %s%s, BY VALUE 28, %s %s, %s%d%s\n",
						names[COLUMN],
						ISC_MODIFY_DPB,
						BY_REF, s2,
						BY_REF, s1,
						BY_DESC, db->dbb_r_user,
						BY_VALUE, strlen(db->dbb_r_user) - 2, END_VALUE);
				COB_print_buffer(output_buffer, TRUE);
			}
			if (db->dbb_r_password) {
				sprintf(output_buffer,
						"%sCALL \"%s\" USING %s%s, %s%s, BY VALUE 29,  %s %s, %s%d%s\n",
						names[COLUMN],
						ISC_MODIFY_DPB,
						BY_REF, s2,
						BY_REF, s1,
						BY_DESC, db->dbb_r_password,
						BY_VALUE, strlen(db->dbb_r_password) - 2, END_VALUE);
				COB_print_buffer(output_buffer, TRUE);
			}

			/*
			   ** ==================================================
			   ** ==
			   ** ==   Process Role Name, isc_dpb_sql_role_name/60
			   ** ==
			   ** ==================================================
			 */

			if (db->dbb_r_sql_role) {
				sprintf(output_buffer,
						"%sCALL \"%s\" USING %s%s, %s%s, BY VALUE 60,  %s %s, %s%d%s\n",
						names[COLUMN],
						ISC_MODIFY_DPB,
						BY_REF, s2,
						BY_REF, s1,
						BY_DESC, db->dbb_r_sql_role,
						BY_VALUE, strlen(db->dbb_r_sql_role) - 2, END_VALUE);
				COB_print_buffer(output_buffer, TRUE);
			}

			if (db->dbb_r_lc_messages) {
				sprintf(output_buffer,
						"%sCALL \"%s\" USING %s%s, %s%s, BY VALUE 47,  %s %s, %s%d%s\n",
						names[COLUMN],
						ISC_MODIFY_DPB,
						BY_REF, s2,
						BY_REF, s1,
						BY_REF, db->dbb_r_lc_messages,
						BY_VALUE, strlen(db->dbb_r_lc_messages) - 2,
						END_VALUE);
				COB_print_buffer(output_buffer, TRUE);
			}
			if (db->dbb_r_lc_ctype) {
				sprintf(output_buffer,
						"%sCALL \"%s\" USING %s%s %s%s, BY VALUE 48,  %s %s, %s%d%s\n",
						names[COLUMN],
						ISC_MODIFY_DPB,
						BY_REF, s2,
						BY_REF, s1,
						BY_REF, db->dbb_r_lc_ctype,
						BY_VALUE, strlen(db->dbb_r_lc_ctype) - 2, END_VALUE);
				COB_print_buffer(output_buffer, TRUE);
			}
		}

		if (request->req_flags & REQ_extend_dpb) {
			sprintf(s1Tmp, "%s%s%s", BY_VALUE, s1, END_VALUE);
			sprintf(s2Tmp, "%s%s%s", BY_VALUE, s2, END_VALUE);
		}
		else {
			sprintf(s2Tmp, "%s%s", BY_REF, s2);
			sprintf(s1Tmp, "%s%d%s", BY_VALUE, request->req_length,
					END_VALUE);
		}
	}

#ifdef VMS
	sprintf(db_name, "\"%s\"", db->dbb_filename);
#else
	for (dbisc = isc_databases; dbisc; dbisc = dbisc->dbb_next)
		if (strcmp(dbisc->dbb_filename, db->dbb_filename) == 0)
			db->dbb_id = dbisc->dbb_id;
	sprintf(db_name, "isc-%ddb", db->dbb_id);
#endif

	sprintf(output_buffer,
			"%sCALL \"%s\" USING %s, %s%d%s, %s%s, %s%s, %s, %s, %s0%s\n",
			names[COLUMN],
			ISC_CREATE_DATABASE,
			status_vector(action),
			BY_VALUE, strlen(db->dbb_filename), END_VALUE,
			BY_REF, db_name,
			BY_REF, db->dbb_name->sym_string,
			(request->req_length) ? s1Tmp : OMITTED,
			(request->req_length) ? s2Tmp : OMITTED, BY_VALUE, END_VALUE);

	COB_print_buffer(output_buffer, TRUE);
//  if the dpb was extended, free it here 

	if (request && request->req_flags & REQ_extend_dpb) {
		if (request->req_length) {
			sprintf(output_buffer,
					"if (%s != isc_%d)", s2, request->req_ident);
			COB_print_buffer(output_buffer, TRUE);
		}

		sprintf(output_buffer,
				"%sCALL \"%s\" USING %s\n", names[COLUMN], ISC_FREE, s2Tmp);
		COB_print_buffer(output_buffer, TRUE);

		/* reset the length of the dpb */
		sprintf(output_buffer, "%sMOVE %d to %s",
				names[COLUMN], request->req_length, s1);
		COB_print_buffer(output_buffer, TRUE);
	}
	save_sw_auto = sw_auto;
	sw_auto = TRUE;
	printa(names[COLUMN], FALSE, "IF %s(2) = 0 THEN", names[ISC_STATUS]);
	gen_ddl(action);
	sw_auto = save_sw_auto;
	printa(names[COLUMN], FALSE, "END-IF");
	set_sqlcode(action);
}


//____________________________________________________________
//  
//		Generate substitution text for END_STREAM.
//  

static void gen_cursor_close( ACT action, GPRE_REQ request)
{

	printa(names[COLUMN], FALSE, "IF %s%dS NOT = 0 THEN",
		   names[ISC_], request->req_ident);
	printa(names[COLUMN], TRUE,
		   "CALL \"%s\" USING %s, %s%s%dS, %s%d%s",
		   ISC_DSQL_FREE,
		   status_vector(action),
		   BY_REF, names[ISC_], request->req_ident, BY_VALUE, 1, END_VALUE);
	printa(names[COLUMN], FALSE, "IF %s(2) = 0 THEN", names[ISC_STATUS]);
}


//____________________________________________________________
//  
//		Generate text to initialize a cursor.
//  

static void gen_cursor_init( ACT action)
{

//  If blobs are present, zero out all of the blob handles.  After this
//  point, the handles are the user's responsibility 

	if (action->act_request->
		req_flags & (REQ_sql_blob_open | REQ_sql_blob_create))
			printa(names[COLUMN], FALSE, "MOVE 0 TO %s%d", names[ISC_],
				   action->act_request->req_blobs->blb_ident);
}


//____________________________________________________________
//  
//		Generate text to open an embedded SQL cursor.
//  

static void gen_cursor_open( ACT action, GPRE_REQ request)
{
	TEXT s[64];

	if (action->act_type != ACT_open)
		printa(names[COLUMN], FALSE, "IF %s%dS = 0 THEN",
			   names[ISC_], request->req_ident);
	else
		printa(names[COLUMN], FALSE, "IF (%s%dS = 0) AND %s NOT = 0 THEN",
			   names[ISC_], request->req_ident, request->req_handle);
	if (sw_auto)
		printa(names[COLUMN], FALSE, "IF %s NOT = 0 THEN",
			   request->req_database->dbb_name->sym_string);
	printa(names[COLUMN], TRUE, "CALL \"%s\" USING %s, %s%s, %s%s%dS",
		   ISC_DSQL_ALLOCATE,
		   status_vector(action),
		   BY_REF, request->req_database->dbb_name->sym_string,
		   BY_REF, names[ISC_], request->req_ident);
	if (sw_auto)
		printa(names[COLUMN], FALSE, "END-IF");
	printa(names[COLUMN], FALSE, "END-IF");

	printa(names[COLUMN], FALSE, "IF %s%dS NOT = 0 THEN",
		   names[ISC_], request->req_ident);
	if (sw_auto)
		printa(names[COLUMN], FALSE, "IF %s NOT = 0 THEN",
			   request_trans(action, request));
#ifndef VMS
	make_name_formatted(s, "ISC-CONST-%s",
						((OPN) action->act_object)->opn_cursor);
#else
	make_name(s, ((OPN) action->act_object)->opn_cursor);
#endif
	printa(names[COLUMN], TRUE,
		   "CALL \"%s\" USING %s, %s%s%dS, %s%s, %s0%s",
		   ISC_DSQL_SET_CURSOR,
		   status_vector(action),
		   BY_REF, names[ISC_], request->req_ident,
		   BY_REF, s, BY_VALUE, END_VALUE);
	printa(names[COLUMN], FALSE, "IF %s(2) = 0 THEN", names[ISC_STATUS]);
	printa(names[COLUMN], TRUE,
		   "CALL \"%s\" USING %s, %s%s, %s%s%dS, %s0%s, %s, %s-1%s, %s0%s, %s",
		   ISC_DSQL_EXECUTE,
		   status_vector(action),
		   BY_REF, request_trans(action, request),
		   BY_REF, names[ISC_], request->req_ident,
		   BY_VALUE, END_VALUE,
		   OMITTED, BY_VALUE, END_VALUE, BY_VALUE, END_VALUE, OMITTED);
	printa(names[COLUMN], FALSE, "IF %s(2) = 0 THEN", names[ISC_STATUS]);
}


//____________________________________________________________
//  
//		Generate insertion text for the database statement.
//  

static void gen_database( ACT action)
{
	DBB db;
	GPRE_REQ request;
	TEXT *name, fname[80], *s, s1[40];
	USHORT count, max_count;
#ifdef PYXIS
	FORM form;
#endif
	TPB tpb;
	POR port;
	BLB blob;
	BOOLEAN all_static, all_extern, dyn_immed;
	REF reference;
	RDY ready;
	ACT act, chck_dups;
	SYM symbol, cur_stmt, dup;
	GPRE_PRC procedure;
	LLS stack_ptr;

	if (first_flag++ != 0)
		return;

	sprintf(output_buffer, "\n%s**** GDS Preprocessor Definitions ****\n\n",
			names[COMMENT]);
	COB_print_buffer(output_buffer, FALSE);

	printa(names[COLUMN_0], FALSE, "01  %s PIC S9(18) USAGE COMP VALUE IS 0.",
		   names[isc_blob_null]);
	printa(names[COLUMN_0], FALSE, "01  %s PIC S9(9) USAGE COMP EXTERNAL.",
		   names[ISC_SQLCODE]);

	all_static = TRUE;
	all_extern = TRUE;
	for (db = isc_databases, count = 0; db; db = db->dbb_next) {
		all_static = all_static && (db->dbb_scope == DBB_STATIC);
		all_extern = all_extern && (db->dbb_scope == DBB_EXTERN);
		name = db->dbb_name->sym_string;
		printa(names[COLUMN_0], FALSE, "01  %s%s PIC S9(9) USAGE COMP%s.",
			   name,
			   (all_static) ? "" : (all_extern) ? " IS EXTERNAL" :
			   " IS GLOBAL", (all_extern) ? "" : " VALUE IS 0");

		/* generate variables to hold database name strings for attach call */

#ifndef VMS
		db->dbb_id = ++count;
		if (db->dbb_runtime) {
			printa(names[COLUMN_0], FALSE,
				   "01  %s%ddb PIC X(%d) VALUE IS \"%s\".", names[isc_],
				   db->dbb_id, strlen(db->dbb_runtime), db->dbb_runtime);
		}
		else if (db->dbb_filename) {
			printa(names[COLUMN_0], FALSE,
				   "01  %s%ddb PIC X(%d) VALUE IS \"%s\".", names[isc_],
				   db->dbb_id, strlen(db->dbb_filename), db->dbb_filename);
		}
#endif

		for (tpb = db->dbb_tpbs; tpb; tpb = tpb->tpb_dbb_next)
			gen_tpb(tpb);
#ifdef PYXIS
		for (form = db->dbb_forms; form; form = form->form_next) {
			printa(names[COLUMN_0], FALSE,
				   "01  %s PIC S9(9) USAGE COMP VALUE IS 0.",
				   form->form_handle);
			printa(names[COLUMN_0], FALSE,
				   "01  %sFL PIC S9(4) USAGE COMP VALUE IS %d.",
				   form->form_handle, strlen(form->form_name->sym_string));
#ifndef VMS
			printa(names[COLUMN_0], FALSE,
				   "01  %sFN PIC X(%d) VALUE IS \"%s\".", form->form_handle,
				   strlen(form->form_name->sym_string),
				   form->form_name->sym_string);
#endif
		}
#endif
	}

#ifndef VMS
//  loop through actions: find readys to generate vars for quoted strings 

	dyn_immed = FALSE;
	for (act = action; act; act = act->act_rest)
		if (act->act_type == ACT_create_database) {
		}
		else if (act->act_type == ACT_ready) {
			for (ready = (RDY) act->act_object; ready;
				 ready = ready->rdy_next) if ((s = ready->rdy_filename)
											  && ((*s == '\'')
												  || (*s == '\''))) {
					strcpy(fname, ++s);
					s = fname + strlen(fname) - 1;
					*s = 0;
					ready->rdy_id = ++count;
					printa(names[COLUMN_0], FALSE,
						   "01  %s%ddb PIC X(%d) VALUE IS \"%s\".",
						   names[isc_], ready->rdy_id, strlen(fname), fname);
				}
		}
		else if ((act->act_flags & ACT_sql) &&
				 (act->act_type == ACT_dyn_cursor ||
				  act->act_type == ACT_dyn_prepare ||
				  act->act_type == ACT_open ||
				  act->act_type == ACT_blob_open ||
				  act->act_type == ACT_blob_create)) {
			if (act->act_type == ACT_dyn_cursor)
				cur_stmt = ((DYN) act->act_object)->dyn_cursor_name;
			else if (act->act_type == ACT_dyn_prepare)
				cur_stmt = ((DYN) act->act_object)->dyn_statement_name;
			else
				cur_stmt = ((OPN) act->act_object)->opn_cursor;

			/* Only generate one declaration per cursor or statement name */

			for (chck_dups = act->act_rest; chck_dups;
				 chck_dups = chck_dups->act_rest) {
				if (chck_dups->act_type == ACT_dyn_cursor)
					dup = ((DYN) chck_dups->act_object)->dyn_cursor_name;
				else if (chck_dups->act_type == ACT_dyn_prepare)
					dup = ((DYN) chck_dups->act_object)->dyn_statement_name;
				else if ((chck_dups->act_flags & ACT_sql) &&
						 (chck_dups->act_type == ACT_open ||
						  chck_dups->act_type == ACT_blob_open ||
						  chck_dups->act_type == ACT_blob_create))
					dup = ((OPN) chck_dups->act_object)->opn_cursor;
				else
					continue;

				if (!strcmp(dup->sym_string, cur_stmt->sym_string))
					break;
			}

			if (!chck_dups) {
				make_name(s1, cur_stmt);
				printa(names[COLUMN_0], FALSE,
					   "01  ISC-CONST-%s PIC X(%d) VALUE IS \"%s \".",
					   s1, strlen(s1) + 1, s1);
				printa(names[COLUMN_0], FALSE,
					   "01  ISC-CONST-%sL PIC S9(4) USAGE %s.", s1,
					   COMP_VALUE);
			}
		}
		else if (act->act_type == ACT_dyn_immediate) {
			if (!dyn_immed) {
				dyn_immed = TRUE;
				printa(names[COLUMN_0], FALSE,
					   "01  ISC-CONST-DYN-IMMEDL PIC S9(4) USAGE %s.",
					   COMP_VALUE);
			}
		}
		else if (act->act_type == ACT_procedure) {
			request = act->act_request;
			procedure = (GPRE_PRC) act->act_object;
			symbol = procedure->prc_symbol;
			name = symbol->sym_string;
			printa(names[COLUMN_0], FALSE,
				   "01  %s%dprc PIC X(%d) VALUE IS \"%s\".",
				   names[isc_], request->req_ident, strlen(name), name);
		}
#endif

	printa(names[COLUMN_0], FALSE, "01  %s%s PIC S9(9) USAGE COMP%s.",
		   names[ISC_TRANS],
		   (all_static) ? "" : (all_extern) ? " IS EXTERNAL" : " IS GLOBAL",
		   (all_extern) ? "" : " VALUE IS 0");
	printa(names[COLUMN_0], FALSE, "01  %s%s.",
		   names[ISC_STATUS_VECTOR],
		   (all_static) ? "" : (all_extern) ? " IS EXTERNAL" : " IS GLOBAL");
	printa(names[COLUMN], FALSE,
		   "03  %s PIC S9(9) USAGE COMP OCCURS 20 TIMES.", names[ISC_STATUS]);
	printa(names[COLUMN_0], FALSE, "01  %s%s.", names[ISC_STATUS_VECTOR2],
		   (all_static) ? "" : (all_extern) ? " IS EXTERNAL" : " IS GLOBAL");
	printa(names[COLUMN], FALSE,
		   "03  %s PIC S9(9) USAGE COMP OCCURS 20 TIMES.",
		   names[ISC_STATUS2]);
	printa(names[COLUMN_0], FALSE, "01  %s PIC S9(9) USAGE COMP.",
		   names[ISC_ARRAY_LENGTH]);

	printa(names[COLUMN_0], FALSE, "01  SQLCODE%s PIC S9(9) USAGE %s%s.",
		   (all_static) ? "" : (all_extern) ? " IS EXTERNAL" : " IS GLOBAL",
		   COMP_VALUE, (all_extern) ? "" : " VALUE IS 0");

	if (sw_pyxis) {
		printa(names[COLUMN_0], FALSE, "01  %s PIC S9(9) USAGE COMP%s.",
			   names[ISC_WINDOW],
			   (sw_window_scope == DBB_GLOBAL) ? " GLOBAL" :
			   (sw_window_scope == DBB_EXTERN) ? " EXTERNAL" : "");
		printa(names[COLUMN_0], FALSE, "01  %s PIC S9(4) USAGE COMP%s%s.",
			   names[ISC_WIDTH],
			   (sw_window_scope == DBB_GLOBAL) ? " GLOBAL" :
			   (sw_window_scope == DBB_EXTERN) ? " EXTERNAL" : "",
			   (sw_window_scope != DBB_EXTERN) ? " VALUE IS 80" : "");
		printa(names[COLUMN_0], FALSE, "01  %s PIC S9(4) USAGE COMP%s%s.",
			   names[ISC_HEIGHT],
			   (sw_window_scope == DBB_GLOBAL) ? " GLOBAL" :
			   (sw_window_scope == DBB_EXTERN) ? " EXTERNAL" : "",
			   (sw_window_scope != DBB_EXTERN) ? " VALUE IS 24" : "");
	}

	for (request = requests; request; request = request->req_next) {
		gen_request(request);
		for (port = request->req_ports; port; port = port->por_next)
			make_port(port);
		for (blob = request->req_blobs; blob; blob = blob->blb_next) {
			printa(names[COLUMN_0], FALSE, "01  %s%d PIC S9(9) USAGE COMP.",
				   names[ISC_], blob->blb_ident);
			printa(names[COLUMN_0], FALSE, "01  %s%d PIC X(%d).",
				   names[ISC_], blob->blb_buff_ident, blob->blb_seg_length);
			printa(names[COLUMN_0], FALSE, "01  %s%d PIC S9(4) USAGE %s.",
				   names[ISC_], blob->blb_len_ident, COMP_VALUE);
		}

		/*  Array declarations  */

		if (port = request->req_primary)
			for (reference = port->por_references; reference;
				 reference = reference->ref_next)
					if (reference->ref_field->fld_array_info)
						make_array_declaration(reference);
	}

//  Generate event parameter block for each event 

	max_count = 0;
	for (stack_ptr = events; stack_ptr; stack_ptr = stack_ptr->lls_next) {
		count = gen_event_block((ACT) stack_ptr->lls_object);
		max_count = MAX(count, max_count);
	}

	if (max_count) {
		printa(names[COLUMN_0], FALSE, "01  %s.", names[ISC_EVENTS_VECTOR]);
		printa(names[COLUMN], FALSE,
			   "03  %s PIC S9(9) USAGE COMP OCCURS %d TIMES.",
			   names[ISC_EVENTS], max_count);
		printa(names[COLUMN_0], FALSE, "01  %s.",
			   names[ISC_EVENT_NAMES_VECTOR]);
		printa(names[COLUMN], FALSE,
			   "03  %s PIC S9(9) USAGE COMP OCCURS %d TIMES.",
			   names[ISC_EVENT_NAMES], max_count);
		printa(names[COLUMN_0], FALSE, "01  %s.",
			   names[ISC_EVENT_NAMES_VECTOR2]);
		printa(names[COLUMN], FALSE, "03  %s PIC X(31) OCCURS %d TIMES.",
			   names[ISC_EVENT_NAMES2], max_count);
	}

	printa(names[COMMENT], FALSE, "**** end of GPRE definitions ****\n");
}


//____________________________________________________________
//  
//		Generate a call to update metadata.
//  

static void gen_ddl( ACT action)
{
	GPRE_REQ request;

//  Set up command type for call to RDB$DDL 

	request = action->act_request;

	if (sw_auto) {
		t_start_auto(0, status_vector(action), action, TRUE);
		printa(names[COLUMN], FALSE, "IF %s NOT = 0 THEN", names[ISC_TRANS]);
	}


	sprintf(output_buffer,
			"%sCALL \"%s\" USING %s, %s%s, %s%s, %s%d%s, %s%s%d\n",
			names[COLUMN], ISC_DDL, status_vector(action), BY_REF,
			request->req_database->dbb_name->sym_string, BY_REF,
			names[ISC_TRANS], BY_VALUE, request->req_length, END_VALUE,
			BY_REF, names[ISC_], request->req_ident);

	COB_print_buffer(output_buffer, TRUE);

	if (sw_auto) {
		printa(names[COLUMN], FALSE, "END-IF");
		printa(names[COLUMN], FALSE, "IF %s(2) = 0 THEN", names[ISC_STATUS]);
		printa(names[COLUMN], TRUE,
			   "CALL \"%s\" USING %s, %s%s", ISC_COMMIT_TRANSACTION,
			   status_vector(action), BY_REF, names[ISC_TRANS]);
		printa(names[COLUMN], FALSE, "END-IF");
		printa(names[COLUMN], FALSE, "IF %s(2) NOT = 0 THEN",
			   names[ISC_STATUS]);
		printa(names[COLUMN], TRUE,
			   "CALL \"%s\" USING %s, %s%s",
			   ISC_ROLLBACK_TRANSACTION, OMITTED, BY_REF, names[ISC_TRANS]);
		printa(names[COLUMN], FALSE, "END-IF");
	}
	COB_print_buffer(output_buffer, TRUE);
	set_sqlcode(action);
}


//____________________________________________________________
//  
//		Generate a dynamic SQL statement.
//  

static void gen_dyn_close( ACT action)
{
	DYN statement;
	TEXT s[64];

	statement = (DYN) action->act_object;
#ifndef VMS
	make_name_formatted(s, "ISC-CONST-%s", statement->dyn_cursor_name);
#else
	make_name(s, statement->dyn_cursor_name);
#endif
	printa(names[COLUMN], TRUE,
		   "CALL \"%s\" USING %s, %s%s",
		   ISC_CLOSE, status_vector(action), BY_REF, s);
	set_sqlcode(action);
}


//____________________________________________________________
//  
//		Generate a dynamic SQL statement.
//  

static void gen_dyn_declare( ACT action)
{
	DYN statement;
	TEXT s1[64], s2[64];

	statement = (DYN) action->act_object;

#ifndef VMS
	make_name_formatted(s1, "ISC-CONST-%s", statement->dyn_statement_name);
	make_name_formatted(s2, "ISC-CONST-%s", statement->dyn_cursor_name);
#else
	make_name(s1, statement->dyn_statement_name);
	make_name(s2, statement->dyn_cursor_name);
#endif

	printa(names[COLUMN], TRUE,
		   "CALL \"%s\" USING %s, %s%s, %s%s",
		   ISC_DECLARE, status_vector(action), BY_REF, s1, BY_REF, s2);
	set_sqlcode(action);
}


//____________________________________________________________
//  
//		Generate a dynamic SQL statement.
//  

static void gen_dyn_describe( ACT action, BOOLEAN bind_flag)
{
	DYN statement;
	TEXT s[64];

	statement = (DYN) action->act_object;

#ifndef VMS
	make_name_formatted(s, "ISC-CONST-%s", statement->dyn_statement_name);
#else
	make_name(s, statement->dyn_statement_name);
#endif

	printa(names[COLUMN], TRUE,
		   "CALL \"%s\" USING %s, %s%s, %s%d%s, %s%s",
		   bind_flag ? ISC_DESCRIBE_BIND : ISC_DESCRIBE,
		   status_vector(action),
		   BY_REF, s,
		   BY_VALUE, sw_sql_dialect, END_VALUE, BY_REF, statement->dyn_sqlda);
	set_sqlcode(action);
}


//____________________________________________________________
//  
//		Generate a dynamic SQL statement.
//  

static void gen_dyn_execute( ACT action)
{
	DYN statement;
	TEXT *transaction, s[64];
	struct gpre_req *request, req_const;

	statement = (DYN) action->act_object;
	if (statement->dyn_trans) {
		transaction = statement->dyn_trans;
		request = &req_const;
		request->req_trans = transaction;
	}
	else {
		transaction = names[ISC_TRANS];
		request = NULL;
	}

	if (sw_auto) {
		t_start_auto(request, status_vector(action), action, TRUE);
		printa(names[COLUMN], FALSE, "IF %s NOT = 0 THEN", transaction);
	}

#ifndef VMS
	make_name_formatted(s, "ISC-CONST-%s", statement->dyn_statement_name);
#else
	make_name(s, statement->dyn_statement_name);
#endif

	printa(names[COLUMN], TRUE,
		   (statement->dyn_sqlda2) ?
		   (TEXT*) "CALL \"%s\" USING %s, %s%s, %s%s, %s%d%s, %s%s, %s%s" :
		   (TEXT*) "CALL \"%s\" USING %s, %s%s, %s%s, %s%d%s, %s%s",
		   (statement->dyn_sqlda2) ? ISC_EXECUTE2 : ISC_EXECUTE,
		   status_vector(action),
		   BY_REF, transaction,
		   BY_REF, s,
		   BY_VALUE, sw_sql_dialect, END_VALUE,
		   (statement->dyn_sqlda) ? BY_REF : "",
		   (statement->dyn_sqlda) ? statement->dyn_sqlda : OMITTED,
		   (statement->dyn_sqlda2) ? BY_REF : "",
		   (statement->dyn_sqlda2) ? statement->dyn_sqlda2 : OMITTED);

	if (sw_auto)
		printa(names[COLUMN], FALSE, "END-IF");

	set_sqlcode(action);
}


//____________________________________________________________
//  
//		Generate a dynamic SQL statement.
//  

static void gen_dyn_fetch( ACT action)
{
	DYN statement;
	TEXT s[64];

#ifdef GIVING_SUPPORTED
#define FETCH_CALL_TEMPLATE	"CALL \"%s\" USING %s, %s%s, %s%d%s, %s%s GIVING SQLCODE"
#else
#define FETCH_CALL_TEMPLATE	"CALL \"%s\" USING %s, BY REFERENCE SQLCODE, %s%s, %s%d%s, %s%s"
#endif

	statement = (DYN) action->act_object;

#ifndef VMS
	make_name_formatted(s, "ISC-CONST-%s", statement->dyn_cursor_name);
#else
	make_name(s, statement->dyn_cursor_name);
#endif

	printa(names[COLUMN], TRUE,
		   FETCH_CALL_TEMPLATE,
		   ISC_FETCH,
		   status_vector(action),
		   BY_REF, s,
		   BY_VALUE, sw_sql_dialect, END_VALUE,
		   (statement->dyn_sqlda) ? BY_REF : "",
		   (statement->dyn_sqlda) ? statement->dyn_sqlda : OMITTED);

	printa(names[COLUMN], FALSE, "IF SQLCODE NOT = 100 THEN");

	set_sqlcode(action);

	printa(names[COLUMN], FALSE, "END-IF");
}


//____________________________________________________________
//  
//		Generate code for an EXECUTE IMMEDIATE dynamic SQL statement.
//  

static void gen_dyn_immediate( ACT action)
{
	DYN statement;
	DBB database;
	TEXT *transaction, s[64], *s2;
	struct gpre_req *request, req_const;

#ifdef GIVING_SUPPORTED
#define GET_LEN_CALL_TEMPLATE	"CALL %s USING %s GIVING %s"
#else
#define GET_LEN_CALL_TEMPLATE	"CALL %s USING %s, %s"
#endif

	statement = (DYN) action->act_object;
	if (statement->dyn_trans) {
		transaction = statement->dyn_trans;
		request = &req_const;
		request->req_trans = transaction;
	}
	else {
		transaction = names[ISC_TRANS];
		request = NULL;
	}

	database = statement->dyn_database;

#ifndef VMS
	s2 = "ISC-CONST-DYN-IMMEDL";
	printa(names[COLUMN], TRUE, GET_LEN_CALL_TEMPLATE,
		   STRING_LENGTH, statement->dyn_string, s2);
	sprintf(s, " %s%s%s,", BY_VALUE, s2, END_VALUE);
#else
	s[0] = 0;
#endif

	if (sw_auto) {
		t_start_auto(request, status_vector(action), action, TRUE);
		printa(names[COLUMN], FALSE, "IF %s NOT = 0 THEN", transaction);
	}

	printa(names[COLUMN], TRUE,
		   (statement->dyn_sqlda2) ?
		   (TEXT*) "CALL \"%s\" USING %s, %s%s, %s%s,%s %s%s, %s%d%s, %s%s, %s%s" :
		   (TEXT*) "CALL \"%s\" USING %s, %s%s, %s%s,%s %s%s, %s%d%s, %s%s",
		   (statement->
			dyn_sqlda2) ? ISC_EXECUTE_IMMEDIATE2 : ISC_EXECUTE_IMMEDIATE,
		   status_vector(action), BY_REF, database->dbb_name->sym_string,
		   BY_REF, transaction, s, BY_DESC, statement->dyn_string, BY_VALUE,
		   sw_sql_dialect, END_VALUE, (statement->dyn_sqlda) ? BY_REF : "",
		   (statement->dyn_sqlda) ? statement->dyn_sqlda : OMITTED,
		   (statement->dyn_sqlda2) ? BY_REF : "",
		   (statement->dyn_sqlda2) ? statement->dyn_sqlda2 : OMITTED);

	if (sw_auto)
		printa(names[COLUMN], FALSE, "END-IF");

	set_sqlcode(action);
}


//____________________________________________________________
//  
//		Generate a dynamic SQL statement.
//  

static void gen_dyn_insert( ACT action)
{
	DYN statement;
	TEXT s[64];

	statement = (DYN) action->act_object;

#ifndef VMS
	make_name_formatted(s, "ISC-CONST-%s", statement->dyn_cursor_name);
#else
	make_name(s, statement->dyn_cursor_name);
#endif

	printa(names[COLUMN], TRUE,
		   "CALL \"%s\" USING %s, %s%s, %s%d%s, %s%s",
		   ISC_INSERT,
		   status_vector(action),
		   BY_REF, s,
		   BY_VALUE, sw_sql_dialect, END_VALUE,
		   (statement->dyn_sqlda) ? BY_REF : "",
		   (statement->dyn_sqlda) ? statement->dyn_sqlda : OMITTED);

	set_sqlcode(action);
}


//____________________________________________________________
//  
//		Generate a dynamic SQL statement.
//  

static void gen_dyn_open( ACT action)
{
	DYN statement;
	TEXT *transaction, s[64];
	struct gpre_req *request, req_const;

	statement = (DYN) action->act_object;
	if (statement->dyn_trans) {
		transaction = statement->dyn_trans;
		request = &req_const;
		request->req_trans = transaction;
	}
	else {
		transaction = names[ISC_TRANS];
		request = NULL;
	}

#ifndef VMS
	make_name_formatted(s, "ISC-CONST-%s", statement->dyn_cursor_name);
#else
	make_name(s, statement->dyn_cursor_name);
#endif

	if (sw_auto) {
		t_start_auto(request, status_vector(action), action, TRUE);
		printa(names[COLUMN], FALSE, "IF %s NOT = 0 THEN", transaction);
	}

	printa(names[COLUMN], TRUE,
		   (statement->dyn_sqlda2) ?
		   (TEXT*) "CALL \"%s\" USING %s, %s%s, %s%s, %s%d%s, %s%s, %s%s" :
		   (TEXT*) "CALL \"%s\" USING %s, %s%s, %s%s, %s%d%s, %s%s",
		   (statement->dyn_sqlda2) ? ISC_OPEN2 : ISC_OPEN,
		   status_vector(action),
		   BY_REF, transaction,
		   BY_REF, s,
		   BY_VALUE, sw_sql_dialect, END_VALUE,
		   (statement->dyn_sqlda) ? BY_REF : "",
		   (statement->dyn_sqlda) ? statement->dyn_sqlda : OMITTED,
		   (statement->dyn_sqlda2) ? BY_REF : "",
		   (statement->dyn_sqlda2) ? statement->dyn_sqlda2 : OMITTED);

	if (sw_auto)
		printa(names[COLUMN], FALSE, "END-IF");

	set_sqlcode(action);
}


//____________________________________________________________
//  
//		Generate a dynamic SQL statement.
//  

static void gen_dyn_prepare( ACT action)
{
	DYN statement;
	DBB database;
	TEXT *transaction, s[64], s2[64], s3[64];
	struct gpre_req *request, req_const;

	statement = (DYN) action->act_object;
	database = statement->dyn_database;

	if (statement->dyn_trans) {
		transaction = statement->dyn_trans;
		request = &req_const;
		request->req_trans = transaction;
	}
	else {
		transaction = names[ISC_TRANS];
		request = NULL;
	}

#ifndef VMS
	make_name_formatted(s, "ISC-CONST-%s", statement->dyn_statement_name);
	sprintf(s2, "%sL", s);
	printa(names[COLUMN], TRUE, GET_LEN_CALL_TEMPLATE,
		   STRING_LENGTH, statement->dyn_string, s2);
	sprintf(s3, " %s%s%s,", BY_VALUE, s2, END_VALUE);
#else
	make_name(s, statement->dyn_statement_name);
	s3[0] = 0;
#endif

	if (sw_auto) {
		t_start_auto(request, status_vector(action), action, TRUE);
		printa(names[COLUMN], FALSE, "IF %s NOT = 0 THEN", transaction);
	}

	printa(names[COLUMN], TRUE,
		   "CALL \"%s\" USING %s, %s%s, %s%s, %s%s,%s %s%s, %s%d%s, %s%s",
		   ISC_PREPARE,
		   status_vector(action),
		   BY_REF, database->dbb_name->sym_string,
		   BY_REF, transaction,
		   BY_REF, s,
		   s3,
		   BY_DESC, statement->dyn_string,
		   BY_VALUE, sw_sql_dialect, END_VALUE,
		   (statement->dyn_sqlda) ? BY_REF : "",
		   (statement->dyn_sqlda) ? statement->dyn_sqlda : OMITTED);

	if (sw_auto)
		printa(names[COLUMN], FALSE, "END-IF");

	set_sqlcode(action);
}


//____________________________________________________________
//  
//		Generate substitution text for END_MODIFY.
//  

static void gen_emodify( ACT action)
{
	UPD modify;
	REF reference, source;
	GPRE_FLD field;
	TEXT s1[20], s2[20];

	modify = (UPD) action->act_object;

	for (reference = modify->upd_port->por_references; reference;
		 reference = reference->ref_next) {
		if (!(source = reference->ref_source))
			continue;
		field = reference->ref_field;
		gen_name(s1, source, TRUE);
		gen_name(s2, reference, TRUE);
		if (field->fld_dtype == dtype_blob ||
			field->fld_dtype == dtype_quad || field->fld_dtype == dtype_date)
			sprintf(output_buffer, "%sCALL \"isc_qtoq\" USING %s, %s\n",
					names[COLUMN], s1, s2);
		else
			sprintf(output_buffer, "%sMOVE %s TO %s\n",
					names[COLUMN], s1, s2);
		COB_print_buffer(output_buffer, TRUE);
		if (field->fld_array_info)
			gen_get_or_put_slice(action, reference, FALSE);
	}

	gen_send(action, modify->upd_port);
}


//____________________________________________________________
//  
//		Generate substitution text for END_STORE.
//  

static void gen_estore( ACT action)
{
	GPRE_REQ request;

	request = action->act_request;
	gen_start(action, request->req_primary);
}


//____________________________________________________________
//  
//		Generate end-if for AT_END if statement
//  

static void gen_end_fetch( ACT action)
{

	printa(names[COLUMN], FALSE, "END-IF");

}


//____________________________________________________________
//  
//		Generate definitions associated with a single request.
//  

static void gen_endfor( ACT action)
{
	GPRE_REQ request;

	request = action->act_request;

	if (request->req_sync)
		gen_send(action, request->req_sync);
	gen_receive(action, request->req_primary);
	printa(names[COLUMN], FALSE, "END-PERFORM");
}


//____________________________________________________________
//  
//		Generate substitution text for ERASE.
//  

static void gen_erase( ACT action)
{
	UPD erase;

	erase = (UPD) action->act_object;
	gen_send(action, erase->upd_port);
}


//____________________________________________________________
//  
//		Generate event parameter blocks for use
//		with a particular call to isc_event_wait.
//  

static SSHORT gen_event_block( ACT action)
{
	GPRE_NOD init, list;
	int ident;

	init = (GPRE_NOD) action->act_object;

	ident = CMP_next_ident();
	init->nod_arg[2] = (GPRE_NOD) ident;

	printa(names[COLUMN_0], FALSE, "01  %s%dA PIC S9(9) USAGE COMP.",
		   names[ISC_], ident);
	printa(names[COLUMN_0], FALSE, "01  %s%dB PIC S9(9) USAGE COMP.",
		   names[ISC_], ident);
	printa(names[COLUMN_0], FALSE, "01  %s%dL PIC S9(4) USAGE COMP.",
		   names[ISC_], ident);

	list = init->nod_arg[1];

	return list->nod_count;
}


//____________________________________________________________
//  
//		Generate substitution text for EVENT_INIT.
//  

static void gen_event_init( ACT action)
{
	GPRE_NOD init, event_list, *ptr, *end, node;
	REF reference;
	PAT args;
	TEXT variable[20];
	SSHORT column, count;
#ifdef GIVING_SUPPORTED
	TEXT *pattern1 =
		"CALL \"%S1\" USING %RF%S4%N1A%RE, %RF%S4%N1B%RE, %VF%N2%VE, %RF%S6%RE GIVING %S4%N1L";
#else
	TEXT *pattern1 =
		"CALL \"%S1\" USING %RF%S4%N1A%RE, %RF%S4%N1B%RE, %VF%N2%VE, %RF%S6%RE, %RF%S4%N1L%RE";
#endif
	TEXT *pattern2 =
		"CALL \"%S2\" USING %V1, %RF%DH%RE, %VF%S4%N1L%VE, %VF%S4%N1A%VE, %VF%S4%N1B%VE";
	TEXT *pattern3 =
		"CALL \"%S3\" USING %S5, %VF%S4%N1L%VE, %VF%S4%N1A%VE, %VF%S4%N1B%VE";

#ifdef GIVING_SUPPORTED
#define EVENT_MOVE_TEMPLATE	"CALL \"%s\" USING %s(%d) GIVING %s(%d)"
#else
#define EVENT_MOVE_TEMPLATE	"CALL \"%s\" USING %s(%d), %s(%d)"
#endif

	init = (GPRE_NOD) action->act_object;
	event_list = init->nod_arg[1];

	column = strlen(names[COLUMN]);

	args.pat_database = (DBB) init->nod_arg[3];
	args.pat_vector1 = status_vector(action);
	args.pat_value1 = (int) init->nod_arg[2];
	args.pat_value2 = (int) event_list->nod_count;
	args.pat_string1 = ISC_EVENT_BLOCK;
	args.pat_string2 = ISC_EVENT_WAIT;
	args.pat_string3 = ISC_EVENT_COUNTS;
	args.pat_string4 = names[ISC_];
	args.pat_string5 = names[ISC_EVENTS_VECTOR];
	args.pat_string6 = names[ISC_EVENT_NAMES_VECTOR];

//  generate call to dynamically generate event blocks 

	for (ptr = event_list->nod_arg, count = 0, end =
		 ptr + event_list->nod_count; ptr < end; ptr++) {
		count++;
		node = *ptr;
		if (node->nod_type == nod_field) {
			reference = (REF) node->nod_arg[0];
			gen_name(variable, reference, TRUE);
			printa(names[COLUMN], FALSE, "MOVE %s TO %s(%d)", variable,
				   names[ISC_EVENT_NAMES2], count);
		}
		else
			printa(names[COLUMN], FALSE, "MOVE %s TO %s(%d)",
				   node->nod_arg[0], names[ISC_EVENT_NAMES2], count);

		printa(names[COLUMN], TRUE, EVENT_MOVE_TEMPLATE, ISC_BADDRESS,
			   names[ISC_EVENT_NAMES2], count, names[ISC_EVENT_NAMES], count);
	}

	PATTERN_expand(column, pattern1, &args);

//  generate actual call to event_wait 

	PATTERN_expand(column, pattern2, &args);

//  get change in event counts, copying event parameter block for reuse 

	PATTERN_expand(column, pattern3, &args);
	set_sqlcode(action);
}


//____________________________________________________________
//  
//		Generate substitution text for EVENT_WAIT.
//  

static void gen_event_wait( ACT action)
{
	PAT args;
	GPRE_NOD event_init;
	SYM event_name, stack_name;
	DBB database;
	LLS stack_ptr;
	ACT event_action;
	SSHORT column;
	int ident;
	TEXT s[64];
	TEXT *pattern1 =
		"CALL \"%S2\" USING %V1, %RF%DH%RE, %VF%S4%N1L%VE, %VF%S4%N1A%VE, %VF%S4%N1B%VE";
	TEXT *pattern2 =
		"CALL \"%S3\" USING %S5, %VF%S4%N1L%VE, %VF%S4%N1A%VE, %VF%S4%N1B%VE";

	event_name = (SYM) action->act_object;

//  go through the stack of events, checking to see if the
//  event has been initialized and getting the event identifier 

	ident = -1;
	for (stack_ptr = events; stack_ptr; stack_ptr = stack_ptr->lls_next) {
		event_action = (ACT) stack_ptr->lls_object;
		event_init = (GPRE_NOD) event_action->act_object;
		stack_name = (SYM) event_init->nod_arg[0];
		if (!strcmp(event_name->sym_string, stack_name->sym_string)) {
			ident = (int) event_init->nod_arg[2];
			database = (DBB) event_init->nod_arg[3];
		}
	}

	if (ident < 0) {
		sprintf(s, "event handle \"%s\" not found", event_name->sym_string);
		IBERROR(s);
        return;
	}

	column = strlen(names[COLUMN]);

	args.pat_database = database;
	args.pat_vector1 = status_vector(action);
	args.pat_value1 = (int) ident;
	args.pat_string2 = ISC_EVENT_WAIT;
	args.pat_string3 = ISC_EVENT_COUNTS;
	args.pat_string4 = names[ISC_];
	args.pat_string5 = names[ISC_EVENTS_VECTOR];

//  generate calls to wait on the event and to fill out the events array 

	PATTERN_expand(column, pattern1, &args);
	PATTERN_expand(column, pattern2, &args);
	set_sqlcode(action);
}


//____________________________________________________________
//  
//		Generate replacement text for the SQL FETCH statement.  The
//		epilog FETCH statement is handled by GEN_S_FETCH (generate
//		stream fetch).
//  

static void gen_fetch( ACT action)
{
	GPRE_REQ request;
	GPRE_NOD var_list;
	int i;
	SCHAR s[20];
#ifdef SCROLLABLE_CURSORS
	POR port;
	REF reference;
	SCHAR *direction, *offset;
	VAL value;
#endif

	request = action->act_request;

#ifdef SCROLLABLE_CURSORS
	if (port = request->req_aport) {
		/* set up the reference to point to the correct value 
		   in the linked list of values, and prepare for the 
		   next FETCH statement if applicable */

		for (reference = port->por_references; reference;
			 reference = reference->ref_next) {
			value = reference->ref_values;
			reference->ref_value = value->val_value;
			reference->ref_values = value->val_next;
		}

		/* find the direction and offset parameters */

		reference = port->por_references;
		offset = reference->ref_value;
		reference = reference->ref_next;
		direction = reference->ref_value;

		/* the direction in which the engine will scroll is sticky, so check to see 
		   the last direction passed to the engine; if the direction is the same and 
		   the offset is 1, then there is no need to pass the message; this prevents 
		   extra packets and allows for batch fetches in either direction */

		printa(names[COLUMN], FALSE,
			   "IF %s%dDI MOD 2 NOT = %s || %s NOT = 1 THEN", names[ISC_],
			   request->req_ident, direction, offset);

		/* assign the direction and offset parameters to the appropriate message, 
		   then send the message to the engine */

		asgn_from(action, port->por_references);
		gen_send(action, port);
		printa(names[COLUMN], FALSE, "MOVE %s TO %s%dDI",
			   names[ISC_], direction, request->req_ident);
		printa(names[COLUMN], FALSE, "END-IF");

		printa(names[COLUMN], FALSE, "IF SQLCODE NOT = 0 THEN");
	}
#endif

	if (request->req_sync)
		gen_send(action, request->req_sync);

	gen_receive(action, request->req_primary);
	printa(names[COLUMN], FALSE,
		   "IF %s NOT =  0 THEN", gen_name(s, request->req_eof, TRUE));
	printa(names[COLUMN], FALSE, "MOVE 0 TO SQLCODE");
	if (var_list = (GPRE_NOD) action->act_object)
		for (i = 0; i < var_list->nod_count; i++) {
			asgn_to(action, (REF) var_list->nod_arg[i]);
		}
	printa(names[COLUMN], FALSE, "ELSE");
	printa(names[COLUMN], FALSE, "MOVE 100 TO SQLCODE");
	printa(names[COLUMN], FALSE, "END-IF");

#ifdef SCROLLABLE_CURSORS
	if (port)
		printa(names[COLUMN], FALSE, "END-IF");
#endif
}


//____________________________________________________________
//  
//		Generate substitution text for FINISH
//  

static void gen_finish( ACT action)
{
	DBB db;
	RDY ready;
#ifdef VMS
#define COMMIT		"COMMIT"
#define ROLLBACK	"ROLLBACK"
#else
#define COMMIT		"commit"
#define ROLLBACK	"rollback"
#endif

	if (sw_auto || ((action->act_flags & ACT_sql) &&
					(action->act_type != ACT_disconnect))) {
		printa(names[COLUMN], FALSE, "IF %s NOT = 0 THEN", names[ISC_TRANS]);
		printa(names[COLUMN], TRUE,
			   "    CALL \"%s\" USING %s, %s%s",
			   (action->act_type !=
				ACT_rfinish) ? ISC_COMMIT_TRANSACTION :
			   ISC_ROLLBACK_TRANSACTION, status_vector(action), BY_REF,
			   names[ISC_TRANS]);
		printa(names[COLUMN], FALSE, "END-IF");
	}

//  the user may have supplied one or more handles 

	db = NULL;
	for (ready = (RDY) action->act_object; ready; ready = ready->rdy_next) {
		db = ready->rdy_database;
		printa(names[COLUMN], FALSE,
			   "IF %s NOT = 0 THEN", db->dbb_name->sym_string);
		printa(names[COLUMN], TRUE,
			   "CALL \"%s\" USING %s, %s%s",
			   ISC_DETACH_DATABASE,
			   status_vector(action), BY_REF, db->dbb_name->sym_string);
		printa(names[COLUMN], FALSE, "END-IF");
	}

//  no handles, so finish all known databases 

	if (!db)
		for (db = isc_databases; db; db = db->dbb_next) {
			printa(names[COLUMN], FALSE,
				   "IF %s NOT = 0 THEN", db->dbb_name->sym_string);
			printa(names[COLUMN], TRUE,
				   "CALL \"%s\" USING %s, %s%s",
				   ISC_DETACH_DATABASE,
				   status_vector(action), BY_REF, db->dbb_name->sym_string);
			printa(names[COLUMN], FALSE, "END-IF");
		}
	set_sqlcode(action);
}


//____________________________________________________________
//  
//		Generate substitution text for FOR statement.
//  

static void gen_for( ACT action)
{
	GPRE_REQ request;
	TEXT s[20];
	REF reference;
	POR port;

	gen_s_start(action);
	request = action->act_request;
	gen_receive(action, request->req_primary);
	printa(names[COLUMN], FALSE, "PERFORM UNTIL %s = 0",
		   gen_name(s, request->req_eof, TRUE));

	if (port = action->act_request->req_primary)
		for (reference = port->por_references; reference;
			 reference =
			 reference->ref_next) if (reference->ref_field->
									  fld_array_info)
					gen_get_or_put_slice(action, reference, TRUE);
}

#ifdef PYXIS
//____________________________________________________________
//  
//		Generate code for a form interaction.
//  

static void gen_form_display( ACT action)
{
	FINT display;
	GPRE_REQ request;
	REF reference, master;
	POR port;
	DBB dbb;
	TEXT s[32], out[16];
	int code;

	display = (FINT) action->act_object;
	request = display->fint_request;
	dbb = request->req_database;
	port = request->req_ports;

//  Initialize field options 

	for (reference = port->por_references; reference;
		 reference = reference->ref_next)
			if ((master = reference->ref_master) &&
				(code = CMP_display_code(display, master)) >= 0)
			printa(names[COLUMN], FALSE,
				   "MOVE %d TO %s", code, gen_name(s, reference, TRUE));


	if (display->fint_flags & FINT_no_wait)
		strcpy(out, "0");
	else
		sprintf(out, "%s%d", names[isc_], port->por_ident);

	printa(names[COLUMN], TRUE,
		   "CALL \"%s\" USING %s, %s%s, %s%s, %s%s, %s%s, %s%s%d, %s%s",
		   PYXIS_DRIVE_FORM,
		   status_vector(action),
		   BY_REF, dbb->dbb_name->sym_string,
		   BY_REF, request->req_trans,
		   BY_REF, names[ISC_WINDOW],
		   BY_REF, request->req_handle,
		   BY_REF, names[ISC_], port->por_ident, BY_REF, out);
}
#endif
#ifdef PYXIS
//____________________________________________________________
//  
//		Generate code for a form block.
//  

static void gen_form_end( ACT action)
{

	printa(names[COLUMN], TRUE, "CALL \"%s\" USING %s%s",
		   PYXIS_POP_WINDOW, BY_REF, names[ISC_WINDOW]);
}
#endif

//____________________________________________________________
//  
//		Generate a function for free standing ANY or statistical.
//  

static void gen_function( ACT function)
{
	GPRE_REQ request;
	POR port;
	REF reference;
	GPRE_FLD field;
	ACT action;
	TEXT *dtype, s[64];

	action = (ACT) function->act_object;

	if (action->act_type != ACT_any) {
		IBERROR("can't generate function");
		return;
	}

	request = action->act_request;

	ib_fprintf(out_file, "static %s_r (request, transaction", request->req_handle);

	if (port = request->req_vport)
		for (reference = port->por_references; reference;
			 reference = reference->ref_next)
				ib_fprintf(out_file, ", %s",
						   gen_name(s, reference->ref_source, TRUE));

	ib_fprintf(out_file,
			   ")\n    isc_req_handle\trequest;\n    isc_tr_handle\ttransaction;\n");

	if (port)
		for (reference = port->por_references; reference;
			 reference = reference->ref_next) {
			field = reference->ref_field;
			switch (field->fld_dtype) {
			case dtype_short:
				dtype = "short";
				break;

			case dtype_long:
				dtype = "long";
				break;

			case dtype_cstring:
			case dtype_text:
				dtype = "char*";
				break;

			case dtype_quad:
				dtype = "ISC_QUAD";
				break;

			case dtype_date:
			case dtype_blob:
				dtype = "ISC_QUAD";
				break;

			case dtype_real:
				dtype = "float";
				break;

			case dtype_double:
				dtype = "double";
				break;

			default:
				IBERROR("gen_function: unsupported datatype");
				return;
			}
			ib_fprintf(out_file, "    %s\t%s;\n", dtype,
					   gen_name(s, reference->ref_source, TRUE));
		}

	ib_fprintf(out_file, "{\n");
	for (port = request->req_ports; port; port = port->por_next)
		make_port(port);

	ib_fprintf(out_file, "\n\n");
	gen_s_start(action);
	gen_receive(action, request->req_primary);

	for (port = request->req_ports; port; port = port->por_next)
		for (reference = port->por_references; reference;
			 reference =
			 reference->ref_next) if (reference->ref_field->
									  fld_array_info)
					gen_get_or_put_slice(action, reference, TRUE);

	port = request->req_primary;
	ib_fprintf(out_file, "\nreturn %s;\n}\n",
			   gen_name(s, port->por_references, TRUE));
}

#ifdef PYXIS
//____________________________________________________________
//  
//		Generate code for a form block.
//  

static void gen_form_for( ACT action)
{
	GPRE_REQ request;
	FORM form;
	DBB dbb;
	TEXT s[64];

	request = action->act_request;
	form = request->req_form;
	dbb = request->req_database;

//  Get database attach and transaction started 

	if (sw_auto)
		t_start_auto(0, status_vector(action), action, TRUE);

//  Get form loaded first 

	printa(names[COLUMN], FALSE, "IF %s = 0 THEN", request->req_form_handle);

#ifndef VMS
	sprintf(s, "%s%sFN", BY_REF, request->req_form_handle);
#else
	sprintf(s, "\'%s\'", form->form_name->sym_string);
#endif

	printa(names[COLUMN], TRUE,
		   "CALL \"%s\" USING %s, %s%s, %s%s, %s%s, %s%sFL, %s",
		   PYXIS_LOAD_FORM,
		   status_vector(action),
		   BY_REF, dbb->dbb_name->sym_string,
		   BY_REF, request->req_trans,
		   BY_REF, request->req_form_handle,
		   BY_REF, request->req_form_handle, s);
	printa(names[COLUMN], FALSE, "END-IF");

//  Get map compiled 

	printa(names[COLUMN], FALSE, "IF %s = 0 THEN", request->req_handle);
	printa(names[COLUMN], TRUE,
		   "CALL \"%s\" USING %s, %s%s, %s%s, %s%s%dL, %s%s%d",
		   PYXIS_COMPILE_MAP,
		   status_vector(action),
		   BY_REF, request->req_form_handle,
		   BY_REF, request->req_handle,
		   BY_REF, names[isc_], request->req_ident,
		   BY_REF, names[ISC_], request->req_ident);
	printa(names[COLUMN], FALSE, "END-IF");

//  Reset form to known state 

	printa(names[COLUMN], TRUE, "CALL \"%s\" USING %s, %s%s",
		   PYXIS_RESET_FORM,
		   status_vector(action), BY_REF, request->req_handle);
}
#endif

//____________________________________________________________
//  
//		Generate a call to isc_get_slice
//		or isc_put_slice for an array.
//  

static void gen_get_or_put_slice( ACT action, REF reference, BOOLEAN get)
{
	int column;
	PAT args;
	TEXT s1[25], s2[10], s4[10];
	TEXT *pattern1 =
		"CALL \"%S7\" USING %V1, %RF%DH%RE, %RF%S1%RE, %RF%S2%RE, %VF%N1%VE, %RF%S3%RE, %VF%N2%VE, %VF%S4%VE, %VF%L1%VE, %RF%S5%RE, %RF%S6%RE";
	TEXT *pattern2 =
		"CALL \"%S7\" USING %V1, %RF%DH%RE, %RF%S1%RE, %RF%S2%RE, %VF%N1%VE, %RF%S3%RE, %VF%N2%VE, %VF%S4%VE, %VF%L1%VE, %RF%S5%RE";

	if (!(reference->ref_flags & REF_fetch_array))
		return;

	column = strlen(names[COLUMN]);

	args.pat_condition = get;	/*  get or put slice */
	args.pat_vector1 = status_vector(action);	/*  status vector  */
	args.pat_database = action->act_request->req_database;	/* database handle */
	args.pat_string1 = action->act_request->req_trans;	/*  transaction handle */

	gen_name(s1, reference, TRUE);	/*  blob handle  */
	args.pat_string2 = s1;

	args.pat_value1 = reference->ref_sdl_length;	/*  slice descr length */

	sprintf(s2, "%s%d", names[ISC_], reference->ref_sdl_ident);	/*  slice description  */
	args.pat_string3 = s2;

	args.pat_value2 = 0;		/*  parameter length  */

	args.pat_string4 = "0";		/*  parameter  */

	args.pat_long1 = reference->ref_field->fld_array_info->ary_size;
	/*  slice size  */
	if (action->act_flags & ACT_sql) {
		args.pat_string5 = reference->ref_value;
	}
	else {
		sprintf(s4, "%s%dL", names[ISC_],
				reference->ref_field->fld_array_info->ary_ident);
		args.pat_string5 = s4;	/*  array name  */
	}

	args.pat_string6 = names[ISC_ARRAY_LENGTH];	/*  return length */
	args.pat_string7 = (get) ? (TEXT*) ISC_GET_SLICE : (TEXT*) ISC_PUT_SLICE;

	PATTERN_expand(column, (get) ? pattern1 : pattern2, &args);
	ib_fprintf(out_file, "\n");
}


//____________________________________________________________
//  
//		Generate the code to do a get segment.
//  

static void gen_get_segment( ACT action)
{
	BLB blob;
	REF into;
	TEXT buffer[128];

#ifdef GIVING_SUPPORTED
#define GET_SEG_CALL_TEMPLATE	"%sCALL \"%s\" USING %s, %s%d, %s%d, %s%d%s, %s%s%d GIVING %s (2)\n"
#else
#define GET_SEG_CALL_TEMPLATE	"%sCALL \"%s\" USING %s, %s%d, %s%d, %s%d%s, %s%s%d\n"
#endif

	if (action->act_flags & ACT_sql)
		blob = (BLB) action->act_request->req_blobs;
	else
		blob = (BLB) action->act_object;

	strcpy(buffer, GET_SEG_CALL_TEMPLATE);
	sprintf(output_buffer,
			buffer,
			names[COLUMN],
			ISC_GET_SEGMENT,
			names[ISC_STATUS_VECTOR],
			names[ISC_], blob->blb_ident,
			names[ISC_], blob->blb_len_ident,
			BY_VALUE, blob->blb_seg_length, END_VALUE,
			BY_REF, names[ISC_], blob->blb_buff_ident, names[ISC_STATUS]);

	COB_print_buffer(output_buffer, TRUE);

	if (action->act_flags & ACT_sql) {
		into = action->act_object;
		set_sqlcode(action);
		printa(names[COLUMN], FALSE, "IF SQLCODE = 0 OR SQLCODE = 101 THEN ");
		printa(names[COLUMN], FALSE, "MOVE ISC-%d TO %s",
			   blob->blb_buff_ident, into->ref_value);
		if (into->ref_null_value)
			printa(names[COLUMN], FALSE, "MOVE ISC-%d TO %s",
				   blob->blb_len_ident, into->ref_null_value);
		printa(names[COLUMN], FALSE, "END-IF");
	}
}

#ifdef PYXIS
//____________________________________________________________
//  
//		Generate end of block for PUT_ITEM and FOR_ITEM.
//  

static void gen_item_end( ACT action)
{
	GPRE_REQ request;
	REF reference;
	POR port;
	DBB dbb;
	TEXT s[32], index[16];

	request = action->act_request;

	if (request->req_type == REQ_menu) {
		gen_menu_item_end(action);
		return;
	}

	if (action->act_pair->act_type == ACT_item_for) {
		gen_name(index, request->req_index, TRUE);
		printa(names[COLUMN], FALSE, "COMPUTE %s = %s + 1", index, index);
		printa(names[COLUMN], TRUE,
			   "CALL \"%s\" USING %s, %s%s, %s%s, %s%s, %s%s%d",
			   PYXIS_FETCH,
			   status_vector(action),
			   BY_REF, request->req_database->dbb_name->sym_string,
			   BY_REF, request->req_trans,
			   BY_REF, request->req_handle,
			   BY_REF, names[ISC_], request->req_ports->por_ident);
		printa(names[COLUMN], FALSE, "END-PERFORM");
		return;
	}

	dbb = request->req_database;
	port = request->req_ports;

//  Initialize field options 

	for (reference = port->por_references; reference;
		 reference = reference->ref_next) if (reference->ref_master)
			printa(names[COLUMN], FALSE,
				   "MOVE %d TO %s", PYXIS_OPT_DISPLAY,
				   gen_name(s, reference, TRUE));

	printa(names[COLUMN], TRUE,
		   "CALL \"%s\" USING %s, %s%s, %s%s, %s%s, %s%s%d",
		   PYXIS_INSERT,
		   status_vector(action),
		   BY_REF, dbb->dbb_name->sym_string,
		   BY_REF, request->req_trans,
		   BY_REF, request->req_handle, BY_REF, names[ISC_], port->por_ident);
}
#endif
#ifdef PYXIS
//____________________________________________________________
//  
//		Generate insert text for FOR_ITEM and PUT_ITEM.
//  

static void gen_item_for( ACT action)
{
	GPRE_REQ request, parent;
	FORM form;
	TEXT index[30];

	request = action->act_request;

	if (request->req_type == REQ_menu) {
		gen_menu_item_for(action);
		return;
	}

	form = request->req_form;
	parent = form->form_parent;

//  Get map compiled 

	printa(names[COLUMN], FALSE, "IF %s = 0 THEN", request->req_handle);
	printa(names[COLUMN], TRUE,
		   "CALL \"%s\" USING %s, %s%s, %s%s, %s%s%dL, %s%s%d",
		   PYXIS_COMPILE_SUB_MAP,
		   status_vector(action),
		   BY_REF, parent->req_handle,
		   BY_REF, request->req_handle,
		   BY_REF, names[ISC_], request->req_ident,
		   BY_REF, names[ISC_], request->req_ident);
	printa(names[COLUMN], FALSE, "END-IF");

	if (action->act_type != ACT_item_for)
		return;

//  Build stuff for item loop 

	gen_name(index, request->req_index, TRUE);
	printa(names[COLUMN], FALSE, "MOVE 1 TO %s", index);
	printa(names[COLUMN], TRUE,
		   "CALL \"%s\" USING %s, %s%s, %s%s, %s%s, %s%s%d",
		   PYXIS_FETCH,
		   status_vector(action),
		   BY_REF, request->req_database->dbb_name->sym_string,
		   BY_REF, request->req_trans,
		   BY_REF, request->req_handle,
		   BY_REF, names[ISC_], request->req_ports->por_ident);
	if (action->act_error || (action->act_flags & ACT_sql))
		printa(names[COLUMN], FALSE, "IF %s(2) = 0 THEN", names[ISC_STATUS]);
	printa(names[COLUMN], FALSE, "PERFORM UNTIL %s = 0", index);
}
#endif

//____________________________________________________________
//  
//		Generate text to compile and start a SQL mass update. 
//		
//  

static void gen_loop( ACT action)
{
	GPRE_REQ request;
	POR port;
	TEXT name[20];

	gen_s_start(action);
	request = action->act_request;
	printa(names[COLUMN], FALSE, "IF SQLCODE = 0 THEN");
	port = request->req_primary;
	gen_receive(action, port);
	gen_name(name, port->por_references, TRUE);
	printa(names[COLUMN], FALSE, "IF SQLCODE = 0 AND %s = 0 THEN ", name);
	printa(names[COLUMN], FALSE, "MOVE 100 TO SQLCODE");
	printa(names[COLUMN], FALSE, "END-IF");
	printa(names[COLUMN], FALSE, "END-IF");
}

#ifdef PYXIS
//____________________________________________________________
//  
//  

static void gen_menu( ACT action)
{
	GPRE_REQ request;

	TEXT buffer[128];

#ifdef GIVING_SUPPORTED
#define PYXIS_MENU_CALL_TEMPLATE	"CALL \"%s\" USING %s, %s, %s%dL, %s%d GIVING %sS "
#else
#define PYXIS_MENU_CALL_TEMPLATE	"CALL \"%s\" USING %s, %s, %s%dL, %s%d BY REFERENCE %sS "
#endif

	request = action->act_request;
	strcpy(buffer, PYXIS_MENU_CALL_TEMPLATE);
	printa(names[COLUMN], TRUE,
		   buffer,
		   PYXIS_MENU,
		   names[ISC_WINDOW],
		   request->req_handle,
		   names[ISC_], request->req_ident,
		   names[ISC_], request->req_ident, request->req_handle);
}
#endif
#ifdef PYXIS
//____________________________________________________________
//  
//		Generate code for a menu interaction.
//  

static void gen_menu_display( ACT action)
{
	MENU menu;
	GPRE_REQ request, display_request;

	request = action->act_request;
	display_request = (GPRE_REQ) action->act_object;

	menu = NULL;

	for (action = request->req_actions; action; action = action->act_next)
		if (action->act_type == ACT_menu_for) {
			menu = (MENU) action->act_object;
			break;
		}

	printa(names[COLUMN], TRUE,
		   "CALL \"%s\" USING %s%s, %s%s, %s%s%dL, %s%s%d, %s%s%dL, %s%s%d,",
		   PYXIS_DRIVE_MENU,
		   BY_REF, names[ISC_WINDOW],
		   BY_REF, request->req_handle,
		   BY_REF, names[ISC_], display_request->req_ident,
		   BY_REF, names[ISC_], display_request->req_ident,
		   BY_REF, names[ISC_], menu->menu_title,
		   BY_REF, names[ISC_], menu->menu_title);

	printa(names[CONTINUE], TRUE,
		   "%s%s%d, %s%s%dL, %s%s%d, %s%s%d",
		   BY_REF, names[ISC_], menu->menu_terminator,
		   BY_REF, names[ISC_], menu->menu_entree_entree,
		   BY_REF, names[ISC_], menu->menu_entree_entree,
		   BY_REF, names[ISC_], menu->menu_entree_value);

}
#endif
#ifdef PYXIS
//____________________________________________________________
//  
//  

static void gen_menu_end( ACT action)
{

	GPRE_REQ request;

	request = action->act_request;
	if (request->req_flags & REQ_menu_for)
		return;

	printa(names[COLUMN], FALSE, "END-IF");
}
#endif
#ifdef PYXIS
//____________________________________________________________
//  
//  

static void gen_menu_entree( ACT action)
{
	GPRE_REQ request;

	request = action->act_request;

	if (!(action->act_flags & ACT_first_entree))
		printa(names[COLUMN], FALSE, "END-IF");
	printa(names[COLUMN], FALSE,
		   "IF %sS = %d THEN", request->req_handle, action->act_count);
}
#endif
#ifdef PYXIS
//____________________________________________________________
//  
//  
//    Generate code for a reference to a menu or entree attribute.

static void gen_menu_entree_att( ACT action)
{
	MENU menu;
	SSHORT ident, length;

	menu = (MENU) action->act_object;

	length = FALSE;
	switch (action->act_type) {
	case ACT_entree_text:
		ident = menu->menu_entree_entree;
		break;
	case ACT_entree_length:
		ident = menu->menu_entree_entree;
		length = TRUE;
		break;
	case ACT_entree_value:
		ident = menu->menu_entree_value;
		break;
	case ACT_title_text:
		ident = menu->menu_title;
		break;
	case ACT_title_length:
		ident = menu->menu_title;
		length = TRUE;
		break;
	case ACT_terminator:
		ident = menu->menu_terminator;
		break;
	default:
		ident = -1;
		break;
	}

	if (length)
		ib_fprintf(out_file, "%s%s%dL", names[COLUMN], names[isc_], ident);
	else
		ib_fprintf(out_file, "%s%s%d", names[COLUMN], names[ISC_], ident);
}
#endif
#ifdef PYXIS
//____________________________________________________________
//  
//		Generate code for a menu block.
//  

static void gen_menu_for( ACT action)
{
	GPRE_REQ request;

	request = action->act_request;

//  Get menu created 

	if (!(request->req_flags & REQ_exp_hand))
		printa(names[COLUMN], TRUE, "CALL \"%s\" USING %s%s",
			   PYXIS_INITIALIZE_MENU, BY_REF, request->req_handle);
}
#endif
#ifdef PYXIS
//____________________________________________________________
//  
//		Generate end of block for PUT_ITEM and FOR_ITEM
//		for a dynamic menu.
//  

static void gen_menu_item_end( ACT action)
{
	GPRE_REQ request;
	ENTREE entree;

	entree = (ENTREE) action->act_pair->act_object;
	request = entree->entree_request;

	if (action->act_pair->act_type == ACT_item_for) {
		printa(names[COLUMN], TRUE,
			   "CALL \"%s\" USING %s%s, %s%s%dL, %s%s%d, %s%s%d, %s%s%d",
			   PYXIS_GET_ENTREE,
			   BY_REF, request->req_handle,
			   BY_REF, names[ISC_], entree->entree_entree,
			   BY_REF, names[ISC_], entree->entree_entree,
			   BY_REF, names[ISC_], entree->entree_value,
			   BY_REF, names[ISC_], entree->entree_end);

		printa(names[COLUMN], FALSE, "END-PERFORM");
		return;
	}

	printa(names[COLUMN],
		   TRUE,
		   "CALL \"%s\" USING %s%s, %s%s%dL, %s%s%d, %s%s%d",
		   PYXIS_PUT_ENTREE,
		   BY_REF, request->req_handle,
		   BY_REF, names[ISC_], entree->entree_entree,
		   BY_REF, names[ISC_], entree->entree_entree,
		   BY_REF, names[ISC_], entree->entree_value);
}
#endif
#ifdef PYXIS
//____________________________________________________________
//  
//		Generate insert text for FOR_ITEM and PUT_ITEM
//		for a dynamic menu.
//  

static void gen_menu_item_for( ACT action)
{
	ENTREE entree;
	GPRE_REQ request;

	if (action->act_type != ACT_item_for)
		return;

//  Build stuff for item loop 

	entree = (ENTREE) action->act_object;
	request = entree->entree_request;

	printa(names[COLUMN],
		   TRUE,
		   "CALL \"%s\" USING %s%s, %s%s%dL, %s%s%d, %s%s%d, %s%s%d",
		   PYXIS_GET_ENTREE,
		   BY_REF, request->req_handle,
		   BY_REF, names[ISC_], entree->entree_entree,
		   BY_REF, names[ISC_], entree->entree_entree,
		   BY_REF, names[ISC_], entree->entree_value,
		   BY_REF, names[ISC_], entree->entree_end);

	printa(names[COLUMN], FALSE, "PERFORM UNTIL %s%d NOT = 0",
		   names[ISC_], entree->entree_end);

}
#endif
#ifdef PYXIS
//____________________________________________________________
//  
//		Generate definitions associated with a dynamic menu request.
//  

static void gen_menu_request( GPRE_REQ request)
{
	ACT action;
	MENU menu;
	ENTREE entree;

	menu = NULL;
	entree = NULL;

	for (action = request->req_actions; action; action = action->act_next) {
		if (action->act_type == ACT_menu_for) {
			menu = (MENU) action->act_object;
			break;
		}
		else if ((action->act_type == ACT_item_for)
				 || (action->act_type == ACT_item_put)) {
			entree = (ENTREE) action->act_object;
			break;
		}
	}

	if (menu) {
		menu->menu_title = CMP_next_ident();
		menu->menu_terminator = CMP_next_ident();
		menu->menu_entree_value = CMP_next_ident();
		menu->menu_entree_entree = CMP_next_ident();
		printa(names[COLUMN_0], FALSE, "01  %%dL PIC S9(4) USAGE IS COMP.",
			   names[ISC_], menu->menu_title);
		printa(names[COLUMN_0], FALSE, "01  %%d PIC X(81).",
			   names[ISC_], menu->menu_title);
		printa(names[COLUMN_0], FALSE, "01  %%d PIC S9(4) USAGE IS COMP.",
			   names[ISC_], menu->menu_terminator);
		printa(names[COLUMN_0], FALSE, "01  %%dL PIC S9(4) USAGE IS COMP.",
			   names[ISC_], menu->menu_entree_entree);
		printa(names[COLUMN_0], FALSE, "01  %%d PIC X(81).",
			   names[ISC_], menu->menu_entree_entree);
		printa(names[COLUMN_0], FALSE, "01  %%d PIC S9(9) USAGE IS COMP.",
			   names[ISC_], menu->menu_entree_value);
	}

	if (entree) {
		entree->entree_entree = CMP_next_ident();
		entree->entree_value = CMP_next_ident();
		entree->entree_end = CMP_next_ident();
		printa(names[COLUMN_0], FALSE, "01  %s%dL PIC S9(4) USAGE IS COMP.",
			   names[ISC_], entree->entree_entree);
		printa(names[COLUMN_0], FALSE, "01  %s%d PIC X(81).",
			   names[ISC_], entree->entree_entree);
		printa(names[COLUMN_0], FALSE, "01  %s%d PIC S9(9) USAGE IS COMP.",
			   names[ISC_], entree->entree_value);
		printa(names[COLUMN_0], FALSE, "01  %s%d PIC S9(4) USAGE IS COMP.",
			   names[ISC_], entree->entree_end);
	}

}
#endif

//____________________________________________________________
//  
//		Generate a name for a reference.  Name is constructed from
//		port and parameter idents.
//  

static TEXT *gen_name( TEXT * string, REF reference, BOOLEAN as_blob)
{

	if (reference->ref_field->fld_array_info && !as_blob)
		sprintf(string, "%s%d", names[ISC_],
				reference->ref_field->fld_array_info->ary_ident);
	else
		sprintf(string, "%s%d", names[isc_], reference->ref_ident);

	return string;
}


//____________________________________________________________
//  
//		Generate a block to handle errors.
//  

static void gen_on_error( ACT action)
{

	printa(names[COLUMN], FALSE, "IF %s (2) NOT = 0 THEN", names[ISC_STATUS]);
	ib_fprintf(out_file, names[COLUMN]);
}


//____________________________________________________________
//  
//		Generate code for an EXECUTE PROCEDURE.
//  

static void gen_procedure( ACT action)
{
	PAT args;
	TEXT *pattern;
	GPRE_REQ request;
	POR in_port, out_port;
	DBB dbb;
	USHORT column;

	request = action->act_request;
	in_port = request->req_vport;
	out_port = request->req_primary;

	dbb = request->req_database;
	args.pat_database = dbb;
	args.pat_request = action->act_request;
	args.pat_vector1 = status_vector(action);
	args.pat_request = request;
	args.pat_port = in_port;
	args.pat_port2 = out_port;
#ifndef VMS
	if (in_port && in_port->por_length)
		pattern =
			"CALL \"isc_transact_request\" USING %V1, %RF%DH%RE, %RF%RT%RE, %VF%RS%VE, %RF%RI%RE, %VF%PL%VE, %RF%PI%RE, %VF%QL%VE, %RF%QI%RE\n";
	else
		pattern =
			"CALL \"isc_transact_request\" USING %V1, %RF%DH%RE, %RF%RT%RE, %VF%RS%VE, %RI, %VF0%VE, 0, %VF%QL%VE, %RF%QI%RE\n";
#else
	if (in_port && in_port->por_length)
		pattern =
			"CALL \"isc_transact_request\" USING %V1, %RF%DH%RE, %RF%RT%RE, %VF%RS%VE, %RF%RI%RE, %VF%PL%VE, %RF%PI%RE, %VF%QL%VE, %RF%QI%RE\n";
	else
		pattern =
			"CALL \"isc_transact_request\" USING %V1, %RF%DH%RE, %RF%RT%RE, %VF%RS%VE, %RI, %VF0%VE, 0, %VF%QL%VE, %RF%QI%RE\n";
#endif

//  Get database attach and transaction started 

	if (sw_auto)
		t_start_auto(0, status_vector(action), action, TRUE);

//  Move in input values 

	asgn_from(action, request->req_values);

//  Execute the procedure 

	column = strlen(names[COLUMN]);
	PATTERN_expand(column, pattern, &args);

	set_sqlcode(action);

	printa(names[COLUMN], FALSE, "IF SQLCODE = 0 THEN");

//  Move out output values 

	asgn_to_proc(request->req_references);
	printa(names[COLUMN], FALSE, "END-IF");
}


//____________________________________________________________
//  
//		Generate the code to do a put segment.
//  

static void gen_put_segment( ACT action)
{
	BLB blob;
	REF from;
	TEXT buffer[128];

#ifdef GIVING_SUPPORTED
#define PUT_SEG_CALL_TEMPLATE	"%sCALL \"%s\" USING %s, %s%s%d, %s%s%d%s, %s%s%d GIVING %s (2)\n"
#else
#define PUT_SEG_CALL_TEMPLATE	"%sCALL \"%s\" USING %s, %s%s%d, %s%s%d%s, %s%s%d\n"
#endif

	if (action->act_flags & ACT_sql) {
		blob = (BLB) action->act_request->req_blobs;
		from = action->act_object;
		printa(names[COLUMN], FALSE, "MOVE %s TO ISC-%d",
			   from->ref_value, blob->blb_buff_ident);
		printa(names[COLUMN], FALSE, "MOVE %s TO ISC-%d",
			   from->ref_null_value, blob->blb_len_ident);
	}
	else
		blob = (BLB) action->act_object;

	strcpy(buffer, PUT_SEG_CALL_TEMPLATE);
	sprintf(output_buffer,
			buffer,
			names[COLUMN],
			ISC_PUT_SEGMENT,
			status_vector(action),
			BY_REF, names[ISC_], blob->blb_ident,
			BY_VALUE, names[ISC_], blob->blb_len_ident, END_VALUE,
			BY_REF, names[ISC_], blob->blb_buff_ident, names[ISC_STATUS]);
	COB_print_buffer(output_buffer, TRUE);

	set_sqlcode(action);
}


//____________________________________________________________
//  
//		Generate BLR in raw, numeric form.  Ughly but dense.
//  

static void gen_raw(
			   UCHAR * blr,
			   enum req_t request_type, int request_length, int ident)
{
	UCHAR *c;
	TEXT s[256];
	int blr_length, length;
	union {
#ifdef VMS
		SCHAR bytewise_blr[4];
		SLONG longword_blr;
#else
		UCHAR bytewise_blr[4];
		ULONG longword_blr;
#endif
	} blr_hunk;

	length = 1;
	blr_length = request_length;

	while (blr_length) {
		s[0] = 0;
		blr_hunk.longword_blr = 0;
		for (c = blr_hunk.bytewise_blr;
			 c < blr_hunk.bytewise_blr + sizeof(SLONG); c++) {
			if (--blr_length)
				*c = *blr++;
			else {
				if (request_type == REQ_slice)
					*c = isc_sdl_eoc;
				else if ((request_type == REQ_ddl) ||
						 (request_type == REQ_create_database) 
#ifdef PYXIS
					||	 (request_type == REQ_form) ||
						 (request_type == REQ_menu)
#endif
					) *c = *blr++;
				else
					*c = blr_eoc;
				break;
			}
		}

		strcat(s, names[COLUMN]);
		strcat(s, RAW_BLR_TEMPLATE);
		strcat(s, "\n");
		sprintf(output_buffer, s, names[ISC_], ident, names[UNDER], length++,
				blr_hunk.longword_blr);
		COB_print_buffer(output_buffer, FALSE);
	}
}


//____________________________________________________________
//  
//		Generate substitution text for READY
//		This becomes baroque for mpexl where we
//		must generate a variable if the user gives us
//		a string literal. mpexl cobol doesn't take
//		string literals as  CALL parameters.
//  

static void gen_ready( ACT action)
{
	RDY ready;
	DBB db, dbisc;
	TEXT *filename, *vector, dbname[96];
	USHORT namelength;

	vector = status_vector(action);

	for (ready = (RDY) action->act_object; ready; ready = ready->rdy_next) {
		db = ready->rdy_database;
		dbisc = (DBB) db->dbb_name->sym_object;
		if (!(filename = ready->rdy_filename)) {
			filename = db->dbb_runtime;
			if (filename) {
				namelength = strlen(filename);
#ifndef VMS
				sprintf(dbname, "isc-%ddb", dbisc->dbb_id);
				filename = dbname;
#endif
			}
			else
				namelength = 0;
		}
		else
			namelength = strlen(filename);
#ifndef VMS
		/* string literal or user defined variable? */

		if (ready->rdy_id) {
			sprintf(dbname, "isc-%ddb", ready->rdy_id);
			filename = dbname;
			namelength -= 2;
		}
		else
			namelength = 0;
#endif
		make_ready(db, filename, vector, ready->rdy_request, namelength);
		set_sqlcode(action);
	}

	ib_fprintf(out_file, names[COLUMN]);
}


//____________________________________________________________
//  
//		Generate substitution text for RELEASE_REQUESTS
//		For active databases, call isc_release_request.
//		for all others, just zero the handle.  For the
//		release request calls, ignore error returns, which
//		are likely if the request was compiled on a database
//		which has been released and re-readied.  If there is
//		a serious error, it will be caught on the next statement.
//  

static void gen_release( ACT action)
{
	DBB db, exp_db;
	GPRE_REQ request;

	exp_db = (DBB) action->act_object;

	for (request = requests; request; request = request->req_next) {
		db = request->req_database;
		if (exp_db && db != exp_db)
			continue;
		if (!(request->req_flags & REQ_exp_hand)) {
			printa(names[COLUMN], FALSE, "IF %s = 0 THEN",
				   db->dbb_name->sym_string);
			printa(names[COLUMN], TRUE, "CALL \"%s\" USING %s, %s%s",
				   ISC_RELEASE_REQUEST, status_vector(action), BY_REF,
				   request->req_handle);
			printa(names[COLUMN], FALSE, "END-IF");
			printa(names[COLUMN], FALSE, "MOVE 0 to %s", request->req_handle);
		}
	}
}


//____________________________________________________________
//  
//		Generate a send or receive call for a port.
//  

static void gen_receive( ACT action, POR port)
{
	GPRE_REQ request;

	request = action->act_request;
	sprintf(output_buffer,
			"%sCALL \"%s\" USING %s, %s%s, %s%d%s, %s%d%s, %s%s%d, %s%s%s\n",
			names[COLUMN],
			ISC_RECEIVE,
			status_vector(action),
			BY_REF, request->req_handle,
			BY_VALUE, port->por_msg_number, END_VALUE,
			BY_VALUE, port->por_length, END_VALUE,
			BY_REF, names[ISC_], port->por_ident,
			BY_VALUE, request->req_request_level, END_VALUE);
	COB_print_buffer(output_buffer, TRUE);

	set_sqlcode(action);
}


//____________________________________________________________
//  
//		Generate definitions associated with a single request.
//  	Requests are generated as raw BLR in longword chunks 
//  	because COBOL is a miserable excuse for a language
//  	and doesn't allow byte value assignments to character
//  	fields.
//  

static void gen_request( GPRE_REQ request)
{
	REF reference;
	BLB blob;
	POR port;
	TEXT *string_type;

	if (!
		(request->
		 req_flags & (REQ_exp_hand 
#ifdef PYXIS
		| REQ_menu_for_item 
#endif
		| REQ_sql_blob_open |
					  REQ_sql_blob_create)) && request->req_type != REQ_slice
&& request->req_type != REQ_procedure)
		printa(names[COLUMN_0], FALSE,
			   "01  %s PIC S9(9) USAGE COMP VALUE IS 0.",
			   request->req_handle);
#ifdef PYXIS
	if (request->req_type == REQ_form)
		printa(names[COLUMN_0], FALSE,
			   "01  %s%dL PIC S9(4) USAGE %s VALUE IS %d.", names[ISC_],
			   request->req_ident, COMP_VALUE, request->req_length);
#endif

	if (request->req_type == REQ_ready)
		printa(names[COLUMN_0], FALSE,
			   "01  %s%dL PIC S9(4) USAGE %s VALUE IS %d.", names[ISC_],
			   request->req_ident, COMP_VALUE, request->req_length);

//  check the case where we need to extend the dpb dynamically at runtime,
//  in which case we need dpb length and a pointer to be defined even if 
//  there is no static dpb defined 

	if (request->req_flags & REQ_extend_dpb) {
		printa(names[COLUMN_0], FALSE,
			   "01  ISC-%dP PIC S9(9) USAGE COMP-5 VALUE IS 0.",
			   request->req_ident);
	}

#ifdef PYXIS
	if (request->req_type == REQ_menu && !(request->req_flags & REQ_menu_for)) {
		printa(names[COLUMN_0], FALSE,
			   "01  %s%dL PIC S9(4) USAGE %s VALUE IS %d.", names[ISC_],
			   request->req_ident, COMP_VALUE, request->req_length);
		printa(names[COLUMN_0], FALSE,
			   "01  %sS PIC S9(4) USAGE COMP VALUE IS 0.",
			   request->req_handle);
	}
#endif
	if (request->req_flags & (REQ_sql_blob_open | REQ_sql_blob_create))
		printa(names[COLUMN_0], FALSE,
			   "01  %s%dS PIC S9(9) USAGE COMP VALUE IS 0.", names[ISC_],
			   request->req_ident);

//  generate the request as BLR long words 

	if (request->req_length) {
		if (request->req_flags & REQ_sql_cursor)
			printa(names[COLUMN_0], FALSE,
				   "01  %s%dS PIC S9(9) USAGE COMP VALUE IS 0.", names[ISC_],
				   request->req_ident);
#ifdef SCROLLABLE_CURSORS
		if (request->req_flags & REQ_scroll)
			printa(names[COLUMN_0], FALSE,
				   "01  %s%dDI PIC S9(4) USAGE COMP VALUE IS 0.", names[ISC_],
				   request->req_ident);
#endif
		printa(names[COLUMN_0], FALSE, "01  %s%d.",
			   names[ISC_], request->req_ident);
		gen_raw(request->req_blr, request->req_type, request->req_length,
				request->req_ident);
		if (!(sw_raw)) {
			printa(names[COMMENT], FALSE, " ");
			printa(names[COMMENT], FALSE, "FORMATTED REQUEST BLR FOR %s%d = ",
				   names[ISC_], request->req_ident);
			switch (request->req_type) {
			case REQ_create_database:
			case REQ_ready:
				string_type = "DPB";
				if (PRETTY_print_cdb((SCHAR*) request->req_blr, (int(*)()) gen_blr, 0, 0))
					IBERROR("internal error during parameter generation");
				break;


			case REQ_ddl:
				string_type = "DYN";
				if (PRETTY_print_dyn((SCHAR*) request->req_blr, (int(*)()) gen_blr, 0, 0))
					IBERROR("internal error during dynamic DDL generation");
				break;
#ifdef PYXIS
			case REQ_form:
				string_type = "form map";
				if (PRETTY_print_form_map((SCHAR*) request->req_blr, (int(*)()) gen_blr, 0, 0))
					IBERROR("internal error during form map generation");
				break;

			case REQ_menu:
				string_type = "menu";
				if (PRETTY_print_menu((SCHAR*) request->req_blr, (int(*)()) gen_blr, 0, 0))
					IBERROR("internal error during menu generation");
				break;
#endif
			case REQ_slice:
				string_type = "SDL";
				if (PRETTY_print_sdl((SCHAR*) request->req_blr, (int(*)()) gen_blr, 0, 0))
					IBERROR("internal error during SDL generation");
				break;

			default:
				string_type = "BLR";
				if (isc_print_blr((SCHAR*) request->req_blr, (void(*)()) gen_blr, 0, 0))
					IBERROR("internal error during BLR generation");
			}
		}
		else {
			switch (request->req_type) {
			case REQ_create_database:
			case REQ_ready:
				string_type = "DPB";
				break;

			case REQ_ddl:
				string_type = "DYN";
				break;
#ifdef PYXIS
			case REQ_form:
				string_type = "form map";
				break;

			case REQ_menu:
				string_type = "menu";
				break;
#endif
			case REQ_slice:
				string_type = "SDL";
				break;

			default:
				string_type = "BLR";
			}
		}
		printa(names[COMMENT], FALSE, " ");
		printa(names[COMMENT], FALSE, "END OF %s STRING FOR REQUEST %s%d\n",
			   string_type, names[ISC_], request->req_ident);
	}

//   Print out slice description language if there are arrays associated with request  

	for (port = request->req_ports; port; port = port->por_next)
		for (reference = port->por_references; reference;
			 reference = reference->ref_next) if (reference->ref_sdl) {
				printa(names[COLUMN_0], FALSE, "01  %s%d.", names[ISC_],
					   reference->ref_sdl_ident);
				gen_raw((UCHAR*) reference->ref_sdl, REQ_slice,
						reference->ref_sdl_length, reference->ref_sdl_ident);
				if (!sw_raw)
					if (PRETTY_print_sdl(reference->ref_sdl, (int(*)()) gen_blr, 0, 0))
						IBERROR("internal error during SDL generation");

				printa(names[COMMENT], FALSE, " ");
				printa(names[COMMENT], FALSE,
					   "END OF SDL STRING FOR %s%d */\n", names[ISC_],
					   reference->ref_sdl_ident);
			}

//  Print out any blob parameter blocks required 
	for (blob = request->req_blobs; blob; blob = blob->blb_next)
		if (blob->blb_const_from_type || blob->blb_const_to_type) {
			printa(names[COLUMN_0], FALSE, "01  %s%d.",
				   names[ISC_], blob->blb_bpb_ident);
			gen_raw(blob->blb_bpb, request->req_type, blob->blb_bpb_length,
					(int) request);
			printa(names[COMMENT], FALSE, " ");
		}
#ifdef PYXIS
	if (request->req_type == REQ_menu)
		gen_menu_request(request);
#endif
//  If this is a GET_SLICE/PUT_slice, allocate some variables 

	if (request->req_type == REQ_slice) {
		printa(names[COLUMN_0], FALSE, "01  %s%dv.", names[isc_],
			   request->req_ident);
		printa(names[COLUMN], FALSE,
			   "    03 %s%dv_3 PIC S9(9) USAGE COMP OCCURS %d TIMES.",
			   names[isc_], request->req_ident, MAX(1,
													request->req_slice->
													slc_parameters));
		printa(names[COLUMN_0], FALSE, "01  %s%ds PIC S9(9) USAGE COMP.",
			   names[isc_], request->req_ident);
	}
}


//____________________________________________________________
//  
//		Generate substitution text for END_STREAM.
//  

static void gen_s_end( ACT action)
{
	GPRE_REQ request;

	request = action->act_request;

	if (action->act_type == ACT_close)
		gen_cursor_close(action, request);

	printa(names[COLUMN], TRUE,
		   "CALL \"%s\" USING %s, %s%s, %s%s%s",
		   ISC_UNWIND_REQUEST,
		   status_vector(action),
		   BY_REF, request->req_handle,
		   BY_VALUE, request->req_request_level, END_VALUE);

	if (action->act_type == ACT_close) {
		printa(names[COLUMN], FALSE, "END-IF");
		printa(names[COLUMN], FALSE, "END-IF");
	}

	set_sqlcode(action);
}


//____________________________________________________________
//  
//		Generate substitution text for FETCH.
//  

static void gen_s_fetch( ACT action)
{
	GPRE_REQ request;

	request = action->act_request;
	if (request->req_sync)
		gen_send(action, request->req_sync);

	gen_receive(action, request->req_primary);
}


//____________________________________________________________
//  
//		Generate text to compile and start a stream.  This is
//		used both by START_STREAM and FOR
//  

static void gen_s_start( ACT action)
{
	GPRE_REQ request;
	POR port;

	request = action->act_request;

	gen_compile(action);

	if (port = request->req_vport)
		asgn_from(action, port->por_references);

	if (action->act_type == ACT_open)
		gen_cursor_open(action, request);

//  Do not call "gen_start" in case if "gen_compile" failed 

	if (action->act_error || (action->act_flags & ACT_sql)) {
		if (sw_auto)
			printa(names[COLUMN], FALSE, "IF %s NOT = 0 AND %s NOT = 0 THEN",
				   request_trans(action, request), request->req_handle);
		else
			printa(names[COLUMN], FALSE, "IF %s NOT = 0 THEN",
				   request->req_handle);
	}

	gen_start(action, port);

	if (action->act_error || (action->act_flags & ACT_sql))
		printa(names[COLUMN], FALSE, "END-IF");

	if (action->act_type == ACT_open) {
		printa(names[COLUMN], FALSE, "END-IF");
		printa(names[COLUMN], FALSE, "END-IF");
		if (sw_auto)
			printa(names[COLUMN], FALSE, "END-IF");
		printa(names[COLUMN], FALSE, "END-IF");
		set_sqlcode(action);
	}
}


//____________________________________________________________
//  
//		Generate a send call for a port.
//  

static void gen_send( ACT action, POR port)
{
	GPRE_REQ request;

	request = action->act_request;

	sprintf(output_buffer,
			"%sCALL \"%s\" USING %s, %s%s, %s%d%s, %s%d%s, %s%s%d, %s%s%s\n",
			names[COLUMN],
			ISC_SEND,
			status_vector(action),
			BY_REF, request->req_handle,
			BY_VALUE, port->por_msg_number, END_VALUE,
			BY_VALUE, port->por_length, END_VALUE,
			BY_REF, names[ISC_], port->por_ident,
			BY_VALUE, request->req_request_level, END_VALUE);

	COB_print_buffer(output_buffer, TRUE);
	set_sqlcode(action);
}


//____________________________________________________________
//  
//		Generate support for get/put slice statement.
//  

static void gen_slice( ACT action)
{
	GPRE_REQ request, parent_request;
	REF reference, upper, lower;
	SLC slice;
	USHORT column;
	PAT args;
	slc::slc_repeat *tail, *end;
	TEXT *pattern1 =
		"CALL \"%S7\" USING %V1, %RF%DH%RE, %RF%RT%RE, %RF%FR%RE, %VF%N1%VE, %RF%I1%RE, %VF%N2%VE, %RF%I1v%RE, %VF%I1s%VE, %RF%S5%RE, %RF%S6%RE";
	TEXT *pattern2 =
		"CALL \"%S7\" USING %V1, %RF%DH%RE, %RF%RT%RE, %RF%FR%RE, %VF%N1%VE, %RF%I1%RE, %VF%N2%VE, %RF%I1v%RE, %VF%I1s%VE, %RF%S5%RE";

	column = strlen(names[COLUMN]);
	request = action->act_request;
	slice = (SLC) action->act_object;
	parent_request = slice->slc_parent_request;

//  Compute array size 

	ib_fprintf(out_file, "    COMPUTE %s%ds = %d",
			   names[isc_],
			   request->req_ident, slice->slc_field->fld_array->fld_length);

	for (tail = slice->slc_rpt, end = tail + slice->slc_dimensions;
		 tail < end; ++tail)
		if (tail->slc_upper != tail->slc_lower) {
			lower = (REF) tail->slc_lower->nod_arg[0];
			upper = (REF) tail->slc_upper->nod_arg[0];
			if (lower->ref_value)
				ib_fprintf(out_file, " * ( %s - %s + 1)", upper->ref_value,
						   lower->ref_value);
			else
				ib_fprintf(out_file, " * ( %s + 1)", upper->ref_value);
		}
	ib_fprintf(out_file, "\n");

//  Make assignments to variable vector 

	for (reference = request->req_values; reference;
		 reference =
		 reference->ref_next) printa(names[COLUMN], FALSE,
									 "MOVE %s TO %s%dv (%d)",
									 reference->ref_value, names[ISC_],
									 request->req_ident, reference->ref_id);

	args.pat_reference = slice->slc_field_ref;
	args.pat_request = parent_request;	/* blob id request */
	args.pat_vector1 = status_vector(action);	/* status vector */
	args.pat_database = parent_request->req_database;	/* database handle */
	args.pat_string1 = action->act_request->req_trans;	/* transaction handle */
	args.pat_value1 = request->req_length;	/* slice descr. length */
	args.pat_ident1 = request->req_ident;	/* request name */
	args.pat_value2 = slice->slc_parameters * sizeof(SLONG);	/* parameter length */

	reference = (REF) slice->slc_array->nod_arg[0];
	args.pat_string5 = reference->ref_value;	/* array name */
	args.pat_string6 = names[ISC_ARRAY_LENGTH];
	args.pat_string7 =
		(action->act_type == ACT_get_slice) ? (TEXT*) ISC_GET_SLICE : (TEXT*) ISC_PUT_SLICE;

	PATTERN_expand(column,
				   (action->act_type == ACT_get_slice) ? pattern1 : pattern2,
				   &args);
}


//____________________________________________________________
//  
//		Substitute for a segment, segment length, or blob handle.
//  

static void gen_segment( ACT action)
{
	BLB blob;

	blob = (BLB) action->act_object;

	ib_fprintf(out_file, "%s%d",
			   names[ISC_],
			   (action->act_type == ACT_segment) ? blob->blb_buff_ident :
			   (action->act_type ==
				ACT_segment_length) ? blob->blb_len_ident : blob->blb_ident);
}


//____________________________________________________________
//  
//  

static void gen_select( ACT action)
{
	GPRE_REQ request;
	POR port;
	GPRE_NOD var_list;
	int i;
	TEXT name[20];

	request = action->act_request;
	port = request->req_primary;
	gen_name(name, request->req_eof, TRUE);

	gen_s_start(action);

//  BUG8321: Do not call "receive" in case if SQLCODE is not equal 0 
	printa(names[COLUMN], FALSE, "IF SQLCODE = 0 THEN");

	gen_receive(action, port);

	printa(names[COLUMN], FALSE, "IF %s NOT = 0 THEN", name);
	if (var_list = (GPRE_NOD) action->act_object)
		for (i = 0; i < var_list->nod_count; i++) {
			asgn_to(action, (REF) var_list->nod_arg[i]);
		}

	if (request->req_database->dbb_flags & DBB_v3) {
		gen_receive(action, port);
		printa(names[COLUMN], FALSE, "IF %s NOT = 0 THEN", name);
		printa(names[COLUMN], FALSE, "MOVE -1 TO SQLCODE");
		printa(names[COLUMN], FALSE, "ELSE");
		printa(names[COLUMN], FALSE, "MOVE 0 TO SQLCODE");
		printa(names[COLUMN], FALSE, "END-IF");
	}

	printa(names[COLUMN], FALSE, "ELSE");
	printa(names[COLUMN], FALSE, "MOVE 100 TO SQLCODE");
	printa(names[COLUMN], FALSE, "END-IF");
	printa(names[COLUMN], FALSE, "END-IF");
}


//____________________________________________________________
//  
//		Generate either a START or START_AND_SEND depending
//		on whether or a not a port is present.
//  

static void gen_start( ACT action, POR port)
{
	GPRE_REQ request;
	TEXT *vector;
	REF reference;

	request = action->act_request;
	vector = status_vector(action);

	if (port) {
		for (reference = port->por_references; reference;
			 reference =
			 reference->ref_next) if (reference->ref_field->
									  fld_array_info)
					gen_get_or_put_slice(action, reference, FALSE);

		sprintf(output_buffer,
				"%sCALL \"%s\" USING %s, %s%s, %s%s, %s%d%s, %s%d%s, %s%s%d, %s%s%s\n",
				names[COLUMN],
				ISC_START_AND_SEND,
				vector,
				BY_REF, request->req_handle,
				BY_REF, request_trans(action, request),
				BY_VALUE, port->por_msg_number, END_VALUE,
				BY_VALUE, port->por_length, END_VALUE,
				BY_REF, names[ISC_], port->por_ident,
				BY_VALUE, request->req_request_level, END_VALUE);
	}
	else
		sprintf(output_buffer,
				"%sCALL \"%s\" USING %s, %s%s, %s%s, %s%s%s\n",
				names[COLUMN],
				ISC_START_REQUEST,
				vector,
				BY_REF, request->req_handle,
				BY_REF, request_trans(action, request),
				BY_VALUE, request->req_request_level, END_VALUE);

	COB_print_buffer(output_buffer, TRUE);

	set_sqlcode(action);
}


//____________________________________________________________
//  
//		Generate text for STORE statement.  This includes the compile
//		call and any variable initialization required.
//  

static void gen_store( ACT action)
{
	GPRE_REQ request;
	REF reference;
	GPRE_FLD field;
	POR port;
	TEXT name[64];

	request = action->act_request;
	gen_compile(action);

//  Initialize any blob fields 

	port = request->req_primary;
	for (reference = port->por_references; reference;
		 reference = reference->ref_next) {
		field = reference->ref_field;
		if (field->fld_flags & FLD_blob)
			printa(names[COLUMN], TRUE, "CALL \"isc_qtoq\" USING %s, %s",
				   names[isc_blob_null], gen_name(name, reference, TRUE));
	}
}


//____________________________________________________________
//  
//		Generate substitution text for START_TRANSACTION.
//  

static void gen_t_start( ACT action)
{
	DBB db;
	GPRE_TRA trans;
	TPB tpb;
	TEXT *filename, dbname[80];
	USHORT namelength;

//  if this is a purely default transaction, just let it through 

	if (!action || !(trans = (GPRE_TRA) action->act_object)) {
		t_start_auto(0, status_vector(action), action, FALSE);
		return;
	}

//  build a complete statement, including tpb's.  Ready db's as gpre_req. 

	if (sw_auto)
		for (tpb = trans->tra_tpb; tpb; tpb = tpb->tpb_tra_next) {
			db = tpb->tpb_database;
			if ((filename = db->dbb_runtime) || !(db->dbb_flags & DBB_sqlca)) {
				printa(names[COLUMN], FALSE, "IF %s = 0 THEN",
					   db->dbb_name->sym_string);
				namelength = filename ? strlen(filename) : 0;
#ifndef VMS
				if (filename) {
					sprintf(dbname, "isc-%ddb", db->dbb_id);
					filename = dbname;
				}
#endif
				make_ready(db, filename, status_vector(action), 0,
						   namelength);
				set_sqlcode(action);
				printa(names[COLUMN], FALSE, "END-IF");
			}
		}

	printa(names[COLUMN], TRUE,
		   "CALL \"%s\" USING %s, %s%s, %s%d%s",
		   ISC_START_TRANSACTION,
		   status_vector(action),
		   BY_REF, (trans->tra_handle) ? trans->tra_handle : names[ISC_TRANS],
		   BY_VALUE, trans->tra_db_count, END_VALUE);

	for (tpb = trans->tra_tpb; tpb; tpb = tpb->tpb_tra_next)
		printa(names[CONTINUE], TRUE, ", %s%s, %s%d%s, %s%s%d",
			   BY_REF, tpb->tpb_database->dbb_name->sym_string,
			   BY_VALUE, tpb->tpb_length, END_VALUE,
			   BY_REF, names[ISC_TPB_], tpb->tpb_ident);

	set_sqlcode(action);

}


//____________________________________________________________
//  
//		Initialize a TPB in the output file
//  

static void gen_tpb( TPB tpb)
{
	UCHAR *text, *c;
	int length, char_len;
	union {
#ifdef VMS
		SCHAR bytewise_tpb[4];
#else
		UCHAR bytewise_tpb[4];
#endif
		SLONG longword_tpb;
	} tpb_hunk;

//  
//  TPBs are generated as raw BLR in longword chunks 
//  because COBOL is a miserable excuse for a language
//  and doesn't allow byte value assignments to character
//  fields.
//  

	printa(names[COLUMN_0], FALSE, "01  %s%d.",
		   names[ISC_TPB_], tpb->tpb_ident);

	text = tpb->tpb_string;
	char_len = tpb->tpb_length;
	length = 1;

	while (char_len) {
		for (c = tpb_hunk.bytewise_tpb;
			 c < tpb_hunk.bytewise_tpb + sizeof(SLONG); c++) {
			*c = *text++;
			if (!(--char_len))
				break;
		}

		printa(names[COLUMN], FALSE, RAW_TPB_TEMPLATE,
			   names[ISC_TPB_], tpb->tpb_ident,
			   names[UNDER], length++, tpb_hunk.longword_tpb);
	}

	sprintf(output_buffer, "%sEnd of data for %s%d\n",
			names[COMMENT], names[ISC_TPB_], tpb->tpb_ident);
	COB_print_buffer(output_buffer, FALSE);
}


//____________________________________________________________
//  
//		Generate substitution text for COMMIT, ROLLBACK, PREPARE, and SAVE
//  

static void gen_trans( ACT action)
{

	if (action->act_type == ACT_commit_retain_context)
		printa(names[COLUMN], TRUE, "CALL \"%s\" USING %s, %s%s",
			   ISC_COMMIT_RETAINING,
			   status_vector(action),
			   BY_REF,
			   (action->act_object) ? (TEXT *) (action->
												act_object) :
			   names[ISC_TRANS]);
	else
		printa(names[COLUMN], TRUE, "CALL \"%s\" USING %s, %s%s",
			   (action->act_type ==
				ACT_commit) ? ISC_COMMIT_TRANSACTION : (action->act_type ==
														ACT_rollback) ?
			   ISC_ROLLBACK_TRANSACTION : ISC_PREPARE_TRANSACTION,
			   status_vector(action), BY_REF,
			   (action->act_object) ? (TEXT *) (action->
												act_object) :
			   names[ISC_TRANS]);
	set_sqlcode(action);
}


//____________________________________________________________
//  
//		Substitute for a variable reference.
//  

static void gen_type( ACT action)
{

	printa(names[COLUMN], TRUE, "%ld", action->act_object);
}


//____________________________________________________________
//  
//		Generate substitution text for UPDATE ... WHERE CURRENT OF ...
//  

static void gen_update( ACT action)
{
	POR port;
	UPD modify;

	modify = (UPD) action->act_object;
	port = modify->upd_port;
	asgn_from(action, port->por_references);
	gen_send(action, port);
}


//____________________________________________________________
//  
//		Substitute for a variable reference.
//  

static void gen_variable( ACT action)
{
	TEXT s[20];
	REF reference;

	reference = action->act_object;
	ib_fprintf(out_file, "\n%s%s",
			   names[COLUMN], gen_name(s, reference, FALSE));
}


//____________________________________________________________
//  
//		Generate tests for any WHENEVER clauses that may have been declared.
//  

static void gen_whenever( SWE label)
{
	TEXT *condition;

	while (label) {
		switch (label->swe_condition) {
		case SWE_error:
			condition = "SQLCODE < 0";
			break;

		case SWE_warning:
			condition = "SQLCODE > 0 AND SQLCODE NOT = 100";
			break;

		case SWE_not_found:
			condition = "SQLCODE = 100";
			break;
		}
		ib_fprintf(out_file, "%sIF (%s) THEN GO TO %s",
				   names[COLUMN], condition, label->swe_label);
		if (label->swe_condition == SWE_warning)
			ib_fprintf(out_file, "\n%s", names[COLUMN_INDENT]);
		label = label->swe_next;
		ib_fprintf(out_file, " END-IF%s", (label) ? ".\n" : "");
	}
}

#ifdef PYXIS
//____________________________________________________________
//  
//		Create a new window.
//  

static void gen_window_create( ACT action)
{

	printa(names[COLUMN], TRUE,
		   "CALL \"%s\" USING %s%s, %s, %s, %s%s, %s%s", PYXIS_CREATE_WINDOW,
		   BY_REF, names[ISC_WINDOW],
		   OMITTED,
		   OMITTED, BY_REF, names[ISC_WIDTH], BY_REF, names[ISC_HEIGHT]);
}
#endif
#ifdef PYXIS
//____________________________________________________________
//  
//		Delete a window.
//  

static void gen_window_delete( ACT action)
{

	printa(names[COLUMN], TRUE, "CALL \"%s\" USING %s%s",
		   PYXIS_DELETE_WINDOW, BY_REF, names[ISC_WINDOW]);
}
#endif
#ifdef PYXIS
//____________________________________________________________
//  
//		Suspend a window.
//  

static void gen_window_suspend( ACT action)
{

	printa(names[COLUMN], TRUE, "CALL \"%s\" USING %s%s",
		   PYXIS_SUSPEND_WINDOW, BY_REF, names[ISC_WINDOW]);
}
#endif

//____________________________________________________________
//  
//		Generate a declaration of an array in the
//		output file.
//  

static void make_array_declaration( REF reference)
{
	GPRE_FLD field;
	TEXT *name, space[128], string1[256], *p;
	DIM dimension;
	int i, dimension_size;
	SSHORT digits, scale;

	field = reference->ref_field;
	name = field->fld_symbol->sym_string;

//  Don't generate multiple declarations for the array.  V3 Bug 569.  

	if (field->fld_array_info->ary_declared)
		return;

	field->fld_array_info->ary_declared = TRUE;

	ib_fprintf(out_file, "%s01  %s%dL.\n",
			   names[COLUMN_0], names[ISC_],
			   field->fld_array_info->ary_ident);
	strcpy(space, "       ");

//   Print out the dimension part of the declaration  
	for (dimension = field->fld_array_info->ary_dimension, i = 3;
		 dimension->dim_next; dimension = dimension->dim_next, i += 2) {
		dimension_size = dimension->dim_upper - dimension->dim_lower + 1;
		printa(space, FALSE, "%02d  %s%d%s%d OCCURS %d TIMES.",
			   i,
			   names[ISC_],
			   field->fld_array_info->ary_ident,
			   names[UNDER], i, dimension_size);
		strcat(space, "   ");
	}

	p = string1;
	dimension_size = dimension->dim_upper - dimension->dim_lower + 1;
	sprintf(p, "%02d  %s%d OCCURS %d TIMES ",
			i, names[ISC_], field->fld_array_info->ary_ident, dimension_size);
	while (*p)
		p++;

	switch (field->fld_array_info->ary_dtype) {
	case dtype_short:
	case dtype_long:
		digits = (field->fld_array_info->ary_dtype == dtype_short) ? 4 : 9;
		scale = field->fld_array->fld_scale;
		strcpy(p, "PIC S");
		while (*p)
			p++;
		if (scale >= -digits && scale <= 0) {
			if (scale > -digits)
				sprintf(p, "9(%d)", digits + scale);
			while (*p)
				p++;
			if (scale)
				sprintf(p, "V9(%d)", digits - (digits + scale));
		}
		else if (scale > 0)
			sprintf(p, "9(%d)P(%d)", digits, scale);
		else
			sprintf(p, "VP(%d)9(%d)", -(scale + digits), digits);
		while (*p)
			p++;
		strcpy(p, USAGE_COMP);
		strcat(p, ".");
		break;

	case dtype_cstring:
	case dtype_text:
	case dtype_varying:
		sprintf(p, "PIC X(%d).", field->fld_array->fld_length);
		break;

	case dtype_date:
		strcpy(p, "PIC S9(18)");
		strcat(p, USAGE_COMP);
		strcat(p, ".");
		break;

	case dtype_quad:
		sprintf(p, "PIC S9(%d)", 18 + field->fld_array->fld_scale);
		while (*p)
			p++;
		if (field->fld_array->fld_scale < 0)
			sprintf(p, "V9(%d)", -field->fld_array->fld_scale);
		else if (field->fld_array->fld_scale > 0)
			sprintf(p, "P(%d)", field->fld_array->fld_scale);
		while (*p)
			p++;
		strcpy(p, USAGE_COMP);
		strcat(p, ".");
		break;

	case dtype_float:
		strcpy(p, DCL_FLOAT);
		strcat(p, ".");
		break;

	case dtype_double:
		strcpy(p, DCL_DOUBLE);
		strcat(p, ".");
		break;

	default:
		sprintf(space, "datatype %d unknown for field %s",
				field->fld_array_info->ary_dtype, name);
		IBERROR(space);
		return;
	}

	printa(space, FALSE, string1);
}


//____________________________________________________________
//  
//		Turn a symbol into a varying string.
//  

static TEXT *make_name( TEXT * string, SYM symbol)
{

#ifndef VMS
	sprintf(string, "%s", symbol->sym_string);
	while (*string) {
		if (*string == '_')
			*string = '-';
		string++;
	}
#else
	sprintf(string, "'%s '", symbol->sym_string);
#endif

	return string;
}


//____________________________________________________________
//  
//		Turn a symbol into a varying string.
//  

static TEXT *make_name_formatted( TEXT * string, TEXT * format, SYM symbol)
{
	TEXT *s;

	sprintf(string, format, symbol->sym_string);
	for (s = string; *s; s++)
		if (*s == '_')
			*s = '-';

	return string;
}


//____________________________________________________________
//  
//		Insert a port record description in output.
//  

static void make_port( POR port)
{
	GPRE_FLD field;
	REF reference;
	SYM symbol;
	TEXT *name, s[80];
	SSHORT digits;

	printa(names[COLUMN_0], FALSE, "01  %s%d.", names[ISC_], port->por_ident);

	for (reference = port->por_references; reference;
		 reference = reference->ref_next) {
		field = reference->ref_field;
		if (symbol = field->fld_symbol)
			name = symbol->sym_string;
		else
			name = "<expression>";
		if (reference->ref_value && (reference->ref_flags & REF_array_elem))
			field = field->fld_array;
		switch (field->fld_dtype) {
		case dtype_short:
		case dtype_long:
			digits = (field->fld_dtype == dtype_short) ? 4 : 9;
			ib_fprintf(out_file, "%s03  %s%d PIC S",
					   names[COLUMN], names[ISC_], reference->ref_ident);
			if (field->fld_scale >= -digits && field->fld_scale <= 0) {
				if (field->fld_scale > -digits)
					ib_fprintf(out_file, "9(%d)", digits + field->fld_scale);
				if (field->fld_scale)
					ib_fprintf(out_file, "V9(%d)",
							   digits - (digits + field->fld_scale));
			}
			else if (field->fld_scale > 0)
				ib_fprintf(out_file, "9(%d)P(%d)", digits, field->fld_scale);
			else
				ib_fprintf(out_file, "VP(%d)9(%d)",
						   -(field->fld_scale + digits), digits);
			ib_fprintf(out_file, "%s.\n", USAGE_COMP);
			break;

		case dtype_cstring:
		case dtype_text:
			printa(names[COLUMN], FALSE, "03  %s%d PIC X(%d).",
				   names[ISC_], reference->ref_ident, field->fld_length);
			break;

		case dtype_date:
		case dtype_quad:
		case dtype_blob:
			ib_fprintf(out_file, "%s03  %s%d PIC S9(",
					   names[COLUMN], names[ISC_], reference->ref_ident);
			ib_fprintf(out_file, "%d)", 18 + field->fld_scale);
			if (field->fld_scale < 0)
				ib_fprintf(out_file, "V9(%d)", -field->fld_scale);
			else if (field->fld_scale > 0)
				ib_fprintf(out_file, "P(%d)", field->fld_scale);
			ib_fprintf(out_file, "%s.\n", USAGE_COMP);
			break;

		case dtype_float:
			printa(names[COLUMN], FALSE, "03  %s%d %s.",
				   names[ISC_], reference->ref_ident, DCL_FLOAT);
			break;

		case dtype_double:
			printa(names[COLUMN], FALSE, "03  %s%d %s.",
				   names[ISC_], reference->ref_ident, DCL_DOUBLE);
			break;

		default:
			sprintf(s, "datatype %d unknown for field %s, msg %d",
					field->fld_dtype, name, port->por_msg_number);
			CPR_error(s);
			return;
		}
	}

	printa(names[COLUMN], FALSE, " ");
}


//____________________________________________________________
//  
//		Generate the actual ready call.
//  

static void make_ready(
				  DBB db,
				  TEXT * filename,
				  TEXT * vector, GPRE_REQ request, USHORT namelength)
{
	TEXT s1[32], s1Tmp[32], s2[32], s2Tmp[32], dbname[128];
	DBB dbisc = (DBB) db->dbb_name->sym_object;

	if (request) {
		sprintf(s1, "%s%dL", names[isc_], request->req_ident);
		if (request->req_flags & REQ_extend_dpb)
			sprintf(s2, "%s%dp", names[isc_], request->req_ident);
		else
			sprintf(s2, "%s%d", names[isc_], request->req_ident);

		/* if the dpb needs to be extended at runtime to include items
		   in host variables, do so here; this assumes that there is 
		   always a request generated for runtime variables */

		if (request->req_flags & REQ_extend_dpb) {
			if (request->req_length) {
				sprintf(output_buffer, "%sMOVE isc_%d to %s\n",
						names[COLUMN], request->req_ident, s2);

			}
			if (db->dbb_r_user) {
				sprintf(output_buffer,
						"%sCALL \"%s\" USING %s%s, %s%s, BY VALUE 28, %s %s, %s%d%s\n",
						names[COLUMN],
						ISC_MODIFY_DPB,
						BY_REF, s2,
						BY_REF, s1,
						BY_DESC, db->dbb_r_user,
						BY_VALUE, strlen(db->dbb_r_user) - 2, END_VALUE);
				COB_print_buffer(output_buffer, TRUE);
			}
			if (db->dbb_r_password) {
				sprintf(output_buffer,
						"%sCALL \"%s\" USING %s%s, %s%s, BY VALUE 29,  %s %s, %s%d%s\n",
						names[COLUMN],
						ISC_MODIFY_DPB,
						BY_REF, s2,
						BY_REF, s1,
						BY_DESC, db->dbb_r_password,
						BY_VALUE, strlen(db->dbb_r_password) - 2, END_VALUE);
				COB_print_buffer(output_buffer, TRUE);
			}

			/*
			   ** ==================================================
			   ** ==
			   ** ==   Process Role Name, isc_dpb_sql_role_name/60
			   ** ==
			   ** ==================================================
			 */

			if (db->dbb_r_sql_role) {
				sprintf(output_buffer,
						"%sCALL \"%s\" USING %s%s, %s%s, BY VALUE 60,  %s %s, %s%d%s\n",
						names[COLUMN],
						ISC_MODIFY_DPB,
						BY_REF, s2,
						BY_REF, s1,
						BY_DESC, db->dbb_r_sql_role,
						BY_VALUE, strlen(db->dbb_r_sql_role) - 2, END_VALUE);
				COB_print_buffer(output_buffer, TRUE);
			}

			if (db->dbb_r_lc_messages) {
				sprintf(output_buffer,
						"%sCALL \"%s\" USING %s%s, %s%s, BY VALUE 47,  %s %s, %s%d%s\n",
						names[COLUMN],
						ISC_MODIFY_DPB,
						BY_REF, s2,
						BY_REF, s1,
						BY_REF, db->dbb_r_lc_messages,
						BY_VALUE, strlen(db->dbb_r_lc_messages) - 2,
						END_VALUE);
				COB_print_buffer(output_buffer, TRUE);
			}
			if (db->dbb_r_lc_ctype) {
				sprintf(output_buffer,
						"%sCALL \"%s\" USING %s%s %s%s, BY VALUE 48,  %s %s, %s%d%s\n",
						names[COLUMN],
						ISC_MODIFY_DPB,
						BY_REF, s2,
						BY_REF, s1,
						BY_REF, db->dbb_r_lc_ctype,
						BY_VALUE, strlen(db->dbb_r_lc_ctype) - 2, END_VALUE);
				COB_print_buffer(output_buffer, TRUE);
			}
		}

		if (request->req_flags & REQ_extend_dpb) {
			sprintf(s1Tmp, "%s%s%s", BY_VALUE, s1, END_VALUE);
			sprintf(s2Tmp, "%s%s%s", BY_VALUE, s2, END_VALUE);
		}
		else {
			sprintf(s2Tmp, "%s%s", BY_REF, s2);
			sprintf(s1Tmp, "%s%d%s", BY_VALUE, request->req_length,
					END_VALUE);
		}
	}

#ifdef VMS

	if (filename) {
		sprintf(output_buffer,
				"%sCALL \"%s\" USING %s, %s%s, %s%s, %s, %s\n",
				names[COLUMN],
				ISC_ATTACH_DATABASE_D,
				vector,
				BY_DESC, filename,
				BY_REF, db->dbb_name->sym_string,
				request ? s1Tmp : OMITTED, request ? s2Tmp : OMITTED);
	}
	else {
		sprintf(output_buffer,
				"%sCALL \"%s\" USING %s, %s%d%s, %s \"%s\", %s%s, %s, %s\n",
				names[COLUMN],
				ISC_ATTACH_DATABASE,
				vector,
				BY_VALUE, strlen(db->dbb_filename), END_VALUE,
				BY_REF, db->dbb_filename,
				BY_REF, db->dbb_name->sym_string,
				request ? s1Tmp : OMITTED, request ? s2Tmp : OMITTED);
	}

#else

	if (!filename) {
		sprintf(dbname, "%s%ddb", names[isc_], dbisc->dbb_id);
		filename = dbname;
		namelength = strlen(db->dbb_filename);

	}
	sprintf(output_buffer,
			"%sCALL \"%s\" USING %s, %s%d%s, %s %s, %s%s, %s, %s\n",
			names[COLUMN],
			ISC_ATTACH_DATABASE,
			vector,
			BY_VALUE, namelength, END_VALUE,
			BY_REF, filename,
			BY_REF, db->dbb_name->sym_string,
			request ? s1Tmp : OMITTED, request ? s2Tmp : OMITTED);

#endif

	COB_print_buffer(output_buffer, TRUE);

//  if the dpb was extended, free it here 

	if (request && request->req_flags & REQ_extend_dpb) {
		if (request->req_length) {
			sprintf(output_buffer,
					"if (%s != isc_%d)", s2, request->req_ident);
			COB_print_buffer(output_buffer, TRUE);
		}

		sprintf(output_buffer,
				"%sCALL \"%s\" USING %s\n", names[COLUMN], ISC_FREE, s2Tmp);
		COB_print_buffer(output_buffer, TRUE);

		/* reset the length of the dpb */
		sprintf(output_buffer, "%sMOVE %d to %s\n",
				names[COLUMN], request->req_length, s1);
		COB_print_buffer(output_buffer, TRUE);
	}
}


//____________________________________________________________
//  
//		Print a fixed string at a particular COLUMN.
//  

static void printa( TEXT * column, BOOLEAN call, TEXT * string, ...)
{
	va_list ptr;
	TEXT s[256];

	VA_START(ptr, string);
	strcpy(s, column);
	strcat(s, string);
	strcat(s, "\n");
	vsprintf(output_buffer, s, ptr);
	COB_print_buffer(output_buffer, call);
}


//____________________________________________________________
//  
//		Print a fixed string at a particular column.
//  

static void printb( TEXT * string, ...)
{
	va_list ptr;

	VA_START(ptr, string);
	ib_vfprintf(out_file, string, ptr);
}


//____________________________________________________________
//  
//		Generate the appropriate transaction handle.
//  

static TEXT *request_trans( ACT action, GPRE_REQ request)
{
	TEXT *trname;

	if (action->act_type == ACT_open) {
		if (!(trname = ((OPN) action->act_object)->opn_trans))
			trname = names[ISC_TRANS];
		return trname;
	}
	else
		return (request) ? request->req_trans : names[ISC_TRANS];
}


//____________________________________________________________
//  
//		generate a CALL to the appropriate SQLCODE routine.
//		Note that not all COBOLs have the concept of a function.

static void set_sqlcode( ACT action)
{
	TEXT buffer[128];

#ifdef GIVING_SUPPORTED
#define SQLCODE_CALL_TEMPLATE	"CALL \"%s\" USING %s GIVING SQLCODE"
#else
#define SQLCODE_CALL_TEMPLATE	"CALL \"%s\" USING %s, BY REFERENCE SQLCODE"
#endif

	if (action && action->act_flags & ACT_sql) {
		strcpy(buffer, SQLCODE_CALL_TEMPLATE);
		printa(names[COLUMN], TRUE,
			   buffer, ISC_SQLCODE_CALL, names[ISC_STATUS_VECTOR]);
	}
}


//____________________________________________________________
//  
//		Generate the appropriate status vector parameter for a gds
//		call depending on where or not the action has an error clause.
//  

static TEXT *status_vector( ACT action)
{

	if (action && (action->act_error || (action->act_flags & ACT_sql)))
		return names[ISC_STATUS_VECTOR];

	return (OMITTED);
}


//____________________________________________________________
//  
//		Generate substitution text for START_TRANSACTION,
//		when it's being generated automatically by a compile
//		call.
//  

static void t_start_auto( GPRE_REQ request, TEXT * vector, ACT action, SSHORT test)
{
	DBB db;
	int count, stat;
	USHORT namelength;
	TEXT *filename, dbname[80], *col, buffer[256], temp[40], *trname;

	trname = request_trans(action, request);

//  find out whether we're using a status vector or not 

	stat = !strcmp(vector, names[ISC_STATUS_VECTOR]);

//  this is a default transaction, make sure all databases are ready 

	if (sw_auto) {
		buffer[0] = 0;
		for (count = 0, db = isc_databases; db; db = db->dbb_next, count++)
			if ((filename = db->dbb_runtime) || !(db->dbb_flags & DBB_sqlca)) {
				ib_fprintf(out_file, "%sIF %s = 0", names[COLUMN],
						   db->dbb_name->sym_string);
				if (stat && buffer[0])
					ib_fprintf(out_file, " AND %s(2) = 0", names[ISC_STATUS]);
				ib_fprintf(out_file, " THEN\n");
				namelength = filename ? strlen(filename) : 0;
#ifndef VMS
				if (filename) {
					sprintf(dbname, "isc-%ddb", db->dbb_id);
					filename = dbname;
				}
#endif
				make_ready(db, filename, vector, 0, namelength);
				printa(names[COLUMN], FALSE, "END-IF");
				if (buffer[0])
					strcat(buffer, ") AND (");
				sprintf(temp, "%s NOT = 0", db->dbb_name->sym_string);
				strcat(buffer, temp);
			}
		if (test)
			if (buffer[0])
				printa(names[COLUMN], FALSE, "IF (%s) AND %s = 0 THEN",
					   buffer, trname);

			else
				printa(names[COLUMN], FALSE, "IF %s = 0 THEN", trname);
		else if (buffer[0])
			printa(names[COLUMN], FALSE, "IF (%s) THEN", buffer);
	}
	else
		for (count = 0, db = isc_databases; db; db = db->dbb_next, count++);

	col = (stat) ? names[COLUMN_INDENT] : names[COLUMN];

	printa(col, TRUE, "CALL \"%s\" USING %s, %s%s, %s%d%s",
		   ISC_START_TRANSACTION, vector, BY_REF, trname,
		   BY_VALUE, count, END_VALUE);

	for (db = isc_databases; db; db = db->dbb_next)
		printa(names[CONTINUE], TRUE, ", %s%s, %s, %s",
			   BY_REF, db->dbb_name->sym_string, OMITTED, OMITTED);

	if (sw_auto && (test || buffer[0]))
		printa(names[COLUMN], FALSE, "END-IF");


	set_sqlcode(action);
}
