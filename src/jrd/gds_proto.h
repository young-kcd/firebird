/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		gds_proto.h
 *	DESCRIPTION:	Prototype header file for gds.c
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

#ifndef JRD_GDS_PROTO_H
#define JRD_GDS_PROTO_H

#ifndef GDS_VAL
#define GDS_VAL(val)	(val)
#define GDS_REF(val)	(&val)
#endif

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

#define gds__alloc(s)		gds__alloc_debug ((s),(TEXT *)__FILE__,(ULONG)__LINE__)
extern UCHAR *API_ROUTINE gds__alloc_debug(SLONG, TEXT *, ULONG);
extern void API_ROUTINE gds_alloc_flag_unfreed(void *);
extern void API_ROUTINE gds_alloc_report(ULONG, char *, int);

#define	ALLOC_dont_report	(1L << 0)	/* Don't report this block */
#define	ALLOC_silent		(1L << 1)	/* Don't report new leaks */
#define	ALLOC_verbose		(1L << 2)	/* Report all leaks, even old */
#define	ALLOC_mark_current	(1L << 3)	/* Mark all current leaks */
#define ALLOC_check_each_call	(1L << 4)	/* Check memory integrity on each alloc/free call */
#define ALLOC_dont_check	(1L << 5)	/* Stop checking integrity on each call */

#else /* DEBUG_GDS_ALLOC */

extern UCHAR *API_ROUTINE gds__alloc(SLONG);

#endif /* DEBUG_GDS_ALLOC */

#if !defined(__cplusplus)
typedef GDS_QUAD GDS__QUAD;
#endif

extern STATUS API_ROUTINE gds__decode(STATUS, USHORT *, USHORT *);
extern void API_ROUTINE isc_decode_date(GDS_QUAD *, void *);
extern void API_ROUTINE isc_decode_sql_date(GDS_DATE *, void *);
extern void API_ROUTINE isc_decode_sql_time(GDS_TIME *, void *);
extern void API_ROUTINE isc_decode_timestamp(GDS_TIMESTAMP *, void *);
extern STATUS API_ROUTINE gds__encode(STATUS, USHORT);
extern void API_ROUTINE isc_encode_date(void *, GDS_QUAD *);
extern void API_ROUTINE isc_encode_sql_date(void *, GDS_DATE *);
extern void API_ROUTINE isc_encode_sql_time(void *, GDS_TIME *);
extern void API_ROUTINE isc_encode_timestamp(void *, GDS_TIMESTAMP *);
extern ULONG API_ROUTINE gds__free(void *);
extern SLONG API_ROUTINE gds__interprete(char *, STATUS **);
extern void API_ROUTINE gds__interprete_a(SCHAR *, SSHORT *, STATUS *,
										  SSHORT *);
extern void API_ROUTINE gds__log(TEXT *, ...);
extern void API_ROUTINE gds__log_status(TEXT *, STATUS *);
extern int API_ROUTINE gds__msg_close(void *);
extern SSHORT API_ROUTINE gds__msg_format(void *, USHORT, USHORT, USHORT,
										  TEXT *, TEXT *, TEXT *, TEXT *,
										  TEXT *, TEXT *);
extern SSHORT API_ROUTINE gds__msg_lookup(void *, USHORT, USHORT, USHORT,
										  TEXT *, USHORT *);
extern int API_ROUTINE gds__msg_open(void **, TEXT *);
extern void API_ROUTINE gds__msg_put(void *, USHORT, USHORT, TEXT *, TEXT *,
									 TEXT *, TEXT *, TEXT *);
#ifdef NETWARE_386
extern void API_ROUTINE gds__prefix(TEXT *, void *);
extern void API_ROUTINE gds__prefix_lock(TEXT *, void *);
#else
extern void API_ROUTINE gds__prefix(TEXT *, TEXT *);
extern void API_ROUTINE gds__prefix_lock(TEXT *, TEXT *);
extern void API_ROUTINE gds__prefix_msg(TEXT *, TEXT *);
#endif
extern SLONG API_ROUTINE gds__get_prefix(SSHORT, TEXT *);
extern STATUS API_ROUTINE gds__print_status(STATUS *);
extern USHORT API_ROUTINE gds__parse_bpb(USHORT, UCHAR *, USHORT *, USHORT *);
extern USHORT API_ROUTINE gds__parse_bpb2(USHORT, UCHAR *, USHORT *, USHORT *,
										  USHORT *, USHORT *);
extern SLONG API_ROUTINE gds__ftof(SCHAR *, USHORT GDS_VAL(length1), SCHAR *,
								   USHORT GDS_VAL(length2));
extern int API_ROUTINE gds__print_blr(UCHAR *, FPTR_VOID, SCHAR *, SSHORT);
extern void API_ROUTINE gds__put_error(TEXT *);
extern void API_ROUTINE gds__qtoq(void *, void *);
extern void API_ROUTINE gds__register_cleanup(FPTR_VOID_PTR, void *);
extern SLONG API_ROUTINE gds__sqlcode(STATUS *);
extern void API_ROUTINE gds__sqlcode_s(STATUS *, ULONG *);
extern UCHAR *API_ROUTINE gds__sys_alloc(SLONG);
extern SLONG API_ROUTINE gds__sys_free(void *);
extern void *API_ROUTINE gds__temp_file(BOOLEAN, TEXT *, TEXT *);
extern void API_ROUTINE gds__unregister_cleanup(FPTR_VOID_PTR, void *);
extern BOOLEAN API_ROUTINE gds__validate_lib_path(TEXT *, TEXT *, TEXT *,
												  SLONG);
extern SLONG API_ROUTINE gds__vax_integer(UCHAR *, SSHORT);
extern void API_ROUTINE gds__vtof(SCHAR *, SCHAR *, USHORT);
extern void API_ROUTINE gds__vtov(CONST SCHAR *, SCHAR *, SSHORT);
extern void API_ROUTINE isc_print_sqlerror(SSHORT, STATUS *);
extern void API_ROUTINE isc_sql_interprete(SSHORT, TEXT *, SSHORT);
extern void *gds__tmp_file2(BOOLEAN, TEXT *, TEXT *, TEXT *);
extern SINT64 API_ROUTINE isc_portable_integer(UCHAR *, SSHORT);
extern void gds__cleanup(void);

#ifdef NETWARE_386
extern SCHAR *mktemp(SCHAR *);
#endif

#ifdef VMS
extern int unlink(SCHAR *);
#endif


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* JRD_GDS_PROTO_H */
