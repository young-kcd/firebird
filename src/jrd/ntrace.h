/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		ntrace.h
 *	DESCRIPTION:	Trace API header
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * You may obtain a copy of the Licence at
 * http://www.gnu.org/licences/lgpl.html
 * 
 * As a special exception this file can also be included in modules
 * with other source code as long as that source code has been 
 * released under an Open Source Initiative certificed licence.  
 * More information about OSI certification can be found at: 
 * http://www.opensource.org 
 * 
 * This module is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public Licence for more details.
 * 
 * This module was created by members of the firebird development 
 * team.  All individual contributions remain the Copyright (C) of 
 * those individuals and all rights are reserved.  Contributors to 
 * this file are either listed below or can be obtained from a CVS 
 * history command.
 *
 *  Created by: Nickolay Samofatov <nickolay@broadviewsoftware.com>
 *
 *  Contributor(s):
 * 
 *
 *  $Id: ntrace.h,v 1.2 2004-04-10 19:41:25 skidder Exp $
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
typedef char* (*ntrace_get_error)(ntrace_object_t tpl_object);

/* Finalize plugin interface for this particular database */
typedef void (*ntrace_finalize)(ntrace_object_t tpl_object);

/* Create/close attachment */
typedef ntrace_boolean_t (*ntrace_event_attach)(ntrace_object_t tpl_object, 
	ntrace_connection_t connection, ntrace_size_t dpb_length, const ntrace_byte_t *dpb);
typedef ntrace_boolean_t (*ntrace_event_detach)(ntrace_object_t tpl_object, 
	ntrace_connection_t connection);

/* Start/end transaction */
typedef ntrace_boolean_t (*ntrace_event_transaction_start)(ntrace_object_t tpl_object, 
	ntrace_connection_t connection, ntrace_transaction_t transaction, 
	ntrace_size_t tpb_length, const ntrace_byte_t* tpb);
typedef ntrace_boolean_t (*ntrace_event_transaction_end)(ntrace_object_t tpl_object,
	ntrace_connection_t connection, ntrace_transaction_t transaction, 
	ntrace_boolean_t commit, ntrace_boolean_t retain_context);

/* DSQL statement lifecycle */
typedef ntrace_boolean_t (*ntrace_event_dsql_prepare)(ntrace_object_t tpl_object, 
	ntrace_connection_t connection, ntrace_transaction_t transaction,
	ntrace_statement_t statement, const char* sql, ntrace_counter_t time_millis,
	ntrace_size_t access_path_length, const ntrace_byte_t* access_path);
typedef ntrace_boolean_t (*ntrace_event_dsql_execute)(ntrace_object_t tpl_object, 
	ntrace_connection_t connection, ntrace_transaction_t transaction,
	ntrace_statement_t statement, PerformanceInfo *info);
typedef ntrace_boolean_t (*ntrace_event_dsql_fetch)(ntrace_object_t tpl_object, 
	ntrace_connection_t connection, ntrace_transaction_t transaction,
	ntrace_statement_t statement, PerformanceInfo *info, ntrace_counter_t records_fetched);
typedef ntrace_boolean_t (*ntrace_event_dsql_free)(ntrace_object_t tpl_object, 
	ntrace_connection_t connection, ntrace_transaction_t transaction,
	ntrace_statement_t statement, unsigned short option);

/* Procedure execution */
typedef ntrace_boolean_t (*ntrace_event_proc_execute)(ntrace_object_t tpl_object, 
	ntrace_connection_t connection, ntrace_transaction_t transaction,
	ntrace_procedure_t procedure, PerformanceInfo *info);

/* API of trace plugin. Used to deliver notifications for each database */
struct TracePlugin {
	/* API version */
	ntrace_version_t tpl_version;
	/* Object pointer to pass to each hook */
	ntrace_object_t tpl_object;

	/* Finalize plugin. Called when database is closed by the engine */
	ntrace_finalize tpl_finalize;

	/* Function to return error string for hook failure */
	ntrace_get_error tpl_get_error;


	/* Events supported */
	ntrace_event_attach tpl_event_attach;
	ntrace_event_detach tpl_event_detach;
	ntrace_event_tra tpl_event_transaction_start;
	ntrace_event_detach tpl_event_transaction_end;
	ntrace_event_dsql_prepare tpl_event_dsql_prepare;
	ntrace_event_dsql_execute tpl_event_dsql_execute;
	ntrace_event_dsql_fetch tpl_event_dsql_fetch;
	ntrace_event_dsql_free tpl_event_dsql_free;

	ntrace_event_proc_execute tpl_event_proc_execute;
};
