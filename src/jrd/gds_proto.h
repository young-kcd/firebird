/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		gds_proto.h
 *	DESCRIPTION:	Prototype header file for gds.cpp
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
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 */

#ifndef JRD_GDS_PROTO_H
#define JRD_GDS_PROTO_H

#ifndef IB_PREFIX_TYPE
#define IB_PREFIX_TYPE 0
#define IB_PREFIX_LOCK_TYPE 1
#define IB_PREFIX_MSG_TYPE 2
#endif

#ifndef INCLUDE_FB_TYPES_H
#include "../include/fb_types.h"
#endif

#include "../jrd/fil.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifdef DEBUG_GDS_ALLOC

#define gds__alloc(s)		gds__alloc_debug ((s),(TEXT*)__FILE__,(ULONG)__LINE__)
void*	API_ROUTINE gds__alloc_debug(SLONG, TEXT*, ULONG);
void	API_ROUTINE gds_alloc_flag_unfreed(void*);
void	API_ROUTINE gds_alloc_report(ULONG, char*, int);

#define	ALLOC_dont_report	(1L << 0)	/* Don't report this block */
#define	ALLOC_silent		(1L << 1)	/* Don't report new leaks */
#define	ALLOC_verbose		(1L << 2)	/* Report all leaks, even old */
#define	ALLOC_mark_current	(1L << 3)	/* Mark all current leaks */
#define ALLOC_check_each_call	(1L << 4)	/* Check memory integrity on each alloc/free call */
#define ALLOC_dont_check	(1L << 5)	/* Stop checking integrity on each call */

#else /* DEBUG_GDS_ALLOC */

void*	API_ROUTINE gds__alloc(SLONG);

#endif /* DEBUG_GDS_ALLOC */

#if !defined(__cplusplus)
typedef GDS_QUAD GDS__QUAD;
#endif

ISC_STATUS	API_ROUTINE gds__decode(ISC_STATUS, USHORT*, USHORT*);
void	API_ROUTINE isc_decode_date(const GDS_QUAD*, void*);
void	API_ROUTINE isc_decode_sql_date(const GDS_DATE*, void*);
void	API_ROUTINE isc_decode_sql_time(const GDS_TIME*, void*);
void	API_ROUTINE isc_decode_timestamp(const GDS_TIMESTAMP*, void*);
ISC_STATUS	API_ROUTINE gds__encode(ISC_STATUS, USHORT);
void	API_ROUTINE isc_encode_date(const void*, GDS_QUAD*);
void	API_ROUTINE isc_encode_sql_date(const void*, GDS_DATE*);
void	API_ROUTINE isc_encode_sql_time(const void*, GDS_TIME*);
void	API_ROUTINE isc_encode_timestamp(const void*, GDS_TIMESTAMP*);
ULONG	API_ROUTINE gds__free(void*);

/* CVC: This function was created to be used inside the engine, but I don't see
a problem if it's used from outside, too. */
SLONG	API_ROUTINE gds_interprete_cpp(char* const, const ISC_STATUS**);
/* CVC: This non-const signature is needed for compatibility, see gds.cpp. */
SLONG	API_ROUTINE gds__interprete(char*, ISC_STATUS**);
void	API_ROUTINE gds__interprete_a(SCHAR*, SSHORT*, ISC_STATUS*, SSHORT*);

void	API_ROUTINE gds__log(const TEXT*, ...);
void	API_ROUTINE gds__trace(const char*);
void	API_ROUTINE gds__trace_raw(const char*, unsigned int = 0);
void	API_ROUTINE gds__log_status(TEXT*, const ISC_STATUS*);
int		API_ROUTINE gds__msg_close(void*);
SSHORT	API_ROUTINE gds__msg_format(void*  handle,
									USHORT facility,
									USHORT msgNumber,
									USHORT bufsize,
									TEXT*  buffer,
									const TEXT* arg1,
									const TEXT* arg2,
									const TEXT* arg3,
									const TEXT* arg4,
									const TEXT* arg5);
SSHORT	API_ROUTINE gds__msg_lookup(void*, USHORT, USHORT, USHORT,
									  TEXT*, USHORT*);
int		API_ROUTINE gds__msg_open(void**, TEXT*);
void	API_ROUTINE gds__msg_put(void*, USHORT, USHORT, TEXT*, TEXT*,
									 TEXT*, TEXT*, TEXT*);
void	API_ROUTINE gds__prefix(TEXT*, const TEXT*);
void	API_ROUTINE gds__prefix_lock(TEXT*, const TEXT*);
void	API_ROUTINE gds__prefix_msg(TEXT*, const TEXT*);

SLONG	API_ROUTINE gds__get_prefix(SSHORT, TEXT*);
ISC_STATUS	API_ROUTINE gds__print_status(const ISC_STATUS*);
USHORT	API_ROUTINE gds__parse_bpb(USHORT, UCHAR*, USHORT*, USHORT*);
USHORT	API_ROUTINE gds__parse_bpb2(USHORT, UCHAR*, SSHORT*, SSHORT*,
									  USHORT*, USHORT*);
SLONG	API_ROUTINE gds__ftof(SCHAR*, USHORT length1, SCHAR*,
							   USHORT length2);
int		API_ROUTINE gds__print_blr(UCHAR*, 
							FPTR_PRINT_CALLBACK, 
							void*, SSHORT);
void	API_ROUTINE gds__put_error(TEXT*);
void	API_ROUTINE gds__qtoq(void*, void*);
void	API_ROUTINE gds__register_cleanup(FPTR_VOID_PTR, void*);
SLONG	API_ROUTINE gds__sqlcode(const ISC_STATUS*);
void	API_ROUTINE gds__sqlcode_s(const ISC_STATUS*, ULONG*);
void	API_ROUTINE gds__temp_dir(TEXT*);
void*	API_ROUTINE gds__temp_file(BOOLEAN, const TEXT*, TEXT*, TEXT* = NULL,
	BOOLEAN = FALSE);
void		API_ROUTINE gds__unregister_cleanup(FPTR_VOID_PTR, void*);
BOOLEAN	API_ROUTINE gds__validate_lib_path(TEXT*, TEXT*, TEXT*,
											  SLONG);
SLONG	API_ROUTINE gds__vax_integer(const UCHAR*, SSHORT);
void	API_ROUTINE gds__vtof(SCHAR*, SCHAR*, USHORT);
void	API_ROUTINE gds__vtov(const SCHAR*, char*, SSHORT);
void	API_ROUTINE isc_print_sqlerror(SSHORT, ISC_STATUS*);
void	API_ROUTINE isc_sql_interprete(SSHORT, TEXT*, SSHORT);
SINT64	API_ROUTINE isc_portable_integer(const UCHAR*, SSHORT);
void	gds__cleanup(void);
void	gds__ulstr(char* buffer, ULONG value, int maxlen, char filler);

void	gds__default_printer(void*, SSHORT, const TEXT*);
void	gds__trace_printer(void*, SSHORT, const TEXT*);


#if (defined SOLARIS && !defined(MAP_ANON))
UCHAR*   mmap_anon(SLONG);
#endif



#ifdef VMS
int		unlink(SCHAR*);
#endif


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* JRD_GDS_PROTO_H */
