/*
 *	PROGRAM:	REPLAY Debugging Utility
 *	MODULE:		arg.h
 *	DESCRIPTION:	Arguments for OSRI calls
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
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 *
 */

#ifndef JRD_ARG_H
#define JRD_ARG_H

/* This table will be used to pull off the argument types
   for recording in the log. The status vector is assumed
   as the first argument 

  Key to datatypes: 

   s = short by value
   l = long by value
   p = pointer
   r = short by reference
   b = buffer which has been preceded by a length of type s
   n = possibly null-terminated buffer if preceding value is 0
   f = definitely null-terminated buffer, no preceding length
   d = double long, such as a blob id
   t = transaction existence block
   o = short which requires a buffer of the specified size to
       be generated, to be filled in by the call
*/

static const SCHAR arg_types1[log_max][10] = {
	"snpsbs",					/* attach_database */
	"psbsb",					/* blob_info */
	"p",						/* cancel_blob */
	"p",						/* close_blob */
	"p",						/* commit */
	"ppsb",						/* compile */
	"pppd",						/* create_blob */
	"snpsbs",					/* create_database */
	"psbsb",					/* database_info */
	"p",						/* detach */
	"prsb",						/* get_segment */
	"pppd",						/* open_blob */
	"p",						/* prepare */
	"psb",						/* put_segment */
	"pssbs",					/* receive */
	"ppsb",						/* reconnect */
	"p",						/* release_request */
	"pssbsb",					/* request_info */
	"p",						/* rollback */
	"pssbs",					/* send */
	"ppssbs",					/* start_and_send */
	"pps",						/* start */
	"pst",						/* start_multiple */
	"psbsb",					/* transaction_info */
	"ps",						/* unwind */

	"p",						/* handle_returned */
	"lll",						/* statistics */
	"",							/* error */

	"snpsbf",					/* attach2 */
	"pp",						/* cancel_events */
	"p",						/* commit_retaining */
	"pppdsb",					/* create_blob2 */
	"snpsbsf",					/* create_database2 */
	"pppsbsbsb",				/* get_slice */
	"pppdsb",					/* open_blob2 */
	"psb",						/* prepare2 */
	"pppsbsbsb",				/* put_slice */
	"psb",						/* que_events */
	"psl",						/* blob_seek */

	"pro",						/* get_segment2 */
	"pppsbsbo",					/* get_slice2 */
	"psos",						/* receive2 */
	"psbo",						/* blob_info2 */
	"psbo",						/* database_info2 */
	"pssbo",					/* request_info2 */
	"psbo",						/* transaction_info2 */

	"ppsb",						/* ddl */
	"ppsbsbs",					/* transact_request */
	"p"							/* drop_database */
};


/* this is the replay log filename definition */

#ifdef VMS
#define LOG_FILE_NAME	"[SYSMGR]replay.log"
#endif

#if defined (WIN_NT)
#define LOG_FILE_NAME	"replay.log"
#endif /* WIN_NT */

#ifndef LOG_FILE_NAME
#define LOG_FILE_NAME   FB_PREFIX "/replay.log"
#endif

/* these are the modes for opening the log file */

#if (defined WIN_NT)
#define MODE_READ	"rb"
#define MODE_WRITE	"wb"
#define MODE_APPEND	"ab"
#endif

#ifndef MODE_READ
#define MODE_READ	"r"
#define MODE_WRITE	"w"
#define MODE_APPEND	"a"
#endif

#endif /* JRD_ARG_H */
