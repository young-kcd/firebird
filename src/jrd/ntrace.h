/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		ntrace.h
 *	DESCRIPTION:	Trace API header
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
 *  The Original Code was created by Nickolay Samofatov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2004 Nickolay Samofatov <nickolay@broadviewsoftware.com>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 *  $Id: ntrace.h,v 1.3 2004-06-30 01:38:57 skidder Exp $
 *
 */

/* Keep this header written using C language, please */


/* Version of API, used for version fields in TracePlugin structure */
#define NTRACE_VERSION 1

#define NTRACE_TRUE 1
#define NTRACE_FALSE 0

/* Database objects*/

typedef int ntrace_connection_t;

typedef int ntrace_transaction_t;

typedef int ntrace_procedure_t;

typedef void* ntrace_statement_t;

/* Plugin-specific argument. Passed by the engine to each hook */
typedef void* ntrace_object_t;

/* Structure version*/
typedef int ntrace_version_t;

/* Boolean type */
typedef int ntrace_boolean_t;

/* Performance counter */
typedef unsigned int ntrace_counter_t;

/* Used for arrays with binary data */
typedef unsigned char ntrace_byte_t;

/* Lengths of various arrays and structures */
typedef unsigned int ntrace_size_t;

/* Performance counters for individual table */
struct TableStats {
	ntrace_counter_t tin_id; /* Relation ID */
	ntrace_counter_t tin_indexed_reads;
	ntrace_counter_t tin_nonindexed_reads;
	ntrace_counter_t tin_updates;
	ntrace_counter_t tin_deletes;
	ntrace_counter_t tin_inserts;
};

/* Performance statistics for operation */
struct PerformanceInfo {
	ntrace_counter_t pin_time; /* Total operation time in milliseconds */
	ntrace_counter_t pin_reads; /* Page reads for operation */
	ntrace_counter_t pin_writes; /* Page writes for operation */
	ntrace_counter_t pin_fetches; /* Page fetches from OS */

	ntrace_size_t pin_count; /* Number of relations involved in analysis */
	struct TableStats *pin_tables; /* Pointer to array with table stats */
};

/* Get error string for hook failure that happened */
typedef char* (*ntrace_get_error_t)(ntrace_object_t tpl_object);

/* Finalize plugin interface for this particular database */
typedef void (*ntrace_finalize_t)(ntrace_object_t tpl_object);

/* Create/close attachment */
typedef ntrace_boolean_t (*ntrace_event_attach_t)(ntrace_object_t tpl_object, 
	ntrace_connection_t connection, ntrace_size_t dpb_length, const ntrace_byte_t *dpb);
typedef ntrace_boolean_t (*ntrace_event_detach_t)(ntrace_object_t tpl_object, 
	ntrace_connection_t connection);

/* Start/end transaction */
typedef ntrace_boolean_t (*ntrace_event_transaction_start_t)(ntrace_object_t tpl_object, 
	ntrace_connection_t connection, ntrace_transaction_t transaction, 
	ntrace_size_t tpb_length, const ntrace_byte_t* tpb);
typedef ntrace_boolean_t (*ntrace_event_transaction_end_t)(ntrace_object_t tpl_object,
	ntrace_connection_t connection, ntrace_transaction_t transaction, 
	ntrace_boolean_t commit, ntrace_boolean_t retain_context);

/* DSQL statement lifecycle */
typedef ntrace_boolean_t (*ntrace_event_dsql_prepare_t)(ntrace_object_t tpl_object, 
	ntrace_connection_t connection, ntrace_transaction_t transaction,
	ntrace_statement_t statement, const char* sql, ntrace_counter_t time_millis,
	ntrace_size_t access_path_length, const ntrace_byte_t* access_path);
typedef ntrace_boolean_t (*ntrace_event_dsql_execute_t)(ntrace_object_t tpl_object, 
	ntrace_connection_t connection, ntrace_transaction_t transaction,
	ntrace_statement_t statement, ntrace_size_t paramcount, struct paramdsc* parameters, 
    PerformanceInfo *info);
typedef ntrace_boolean_t (*ntrace_event_dsql_fetch_t)(ntrace_object_t tpl_object, 
	ntrace_connection_t connection, ntrace_transaction_t transaction,
	ntrace_statement_t statement, PerformanceInfo *info, ntrace_counter_t records_fetched);
typedef ntrace_boolean_t (*ntrace_event_dsql_free_t)(ntrace_object_t tpl_object, 
	ntrace_connection_t connection, ntrace_transaction_t transaction,
	ntrace_statement_t statement, unsigned short option);

/* Procedure execution */
typedef ntrace_boolean_t (*ntrace_event_proc_execute_t)(ntrace_object_t tpl_object, 
	ntrace_connection_t connection, ntrace_transaction_t transaction,
	ntrace_procedure_t procedure, ntrace_size_t argcount, struct paramdsc* arguments, 
	PerformanceInfo *info);

/* API of trace plugin. Used to deliver notifications for each database */
struct TracePlugin {
	/* API version */
	ntrace_version_t tpl_version;
	/* Object pointer to pass to each hook */
	ntrace_object_t tpl_object;

	/* Finalize plugin. Called when database is closed by the engine */
	ntrace_finalize_t tpl_finalize;

	/* Function to return error string for hook failure */
	ntrace_get_error_t tpl_get_error;


	/* Events supported */
	ntrace_event_attach_t tpl_event_attach;
	ntrace_event_detach_t tpl_event_detach;
	ntrace_event_transaction_start_t tpl_event_transaction_start;
	ntrace_event_transaction_end_t tpl_event_transaction_end;
	ntrace_event_dsql_prepare_t tpl_event_dsql_prepare;
	ntrace_event_dsql_execute_t tpl_event_dsql_execute;
	ntrace_event_dsql_fetch_t tpl_event_dsql_fetch;
	ntrace_event_dsql_free_t tpl_event_dsql_free;

	ntrace_event_proc_execute_t tpl_event_proc_execute;
};

/* Trace API plugin entrypoint type */
typedef ntrace_boolean_t (*ntrace_attach_t)(const char* db_filename, const TracePlugin** plugin);
