/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		aif.c
 *	DESCRIPTION:	Interface to MPE/XL Architected Interface Facility
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

#pragma intrinsic	GETPRIVMODE, GETUSERMODE
#pragma intrinsic_file	"aifintr.pub.sys"
#pragma intrinsic	AIFPORTCLOSE, AIFPORTOPEN, AIFPORTRECEIVE, AIFPORTSEND, AIFPORTINT
#pragma intrinsic	AIFFILEGGET, AIFPROCGET, AIFACCESSON

#define USER_ID		1129269070

static void initialize(void);

static SSHORT initialized;

#ifdef PRIVMODE_CODE
#define GET_PRIVMODE	if (privmode_flag < 0) privmode_flag = ISC_check_privmode();\
			if (privmode_flag > 0) GETPRIVMODE();
#define GET_USERMODE	if (privmode_flag > 1) GETUSERMODE();

static SSHORT privmode_flag = -1;
#else
#define GET_PRIVMODE
#define GET_USERMODE
#endif


SLONG AIF_close_port(SLONG port)
{
/**************************************
 *
 *	A I F _ c l o s e _ p o r t
 *
 **************************************
 *
 * Functional description
 *	Remove a connection to a Port.
 *
 **************************************/
	SLONG status;

	GET_PRIVMODE AIFPORTCLOSE(&status, &port,);
	GET_USERMODE return status;
}


SLONG AIF_get_pid(SLONG pin, SLONG pid, SLONG * found_pid)
{
/**************************************
 *
 *	A I F _ g e t _ p i d
 *
 **************************************
 *
 * Functional description
 *	Find out if a process still exists or
 *	get the PID (process id, which is different
 *	from the PIN, process id number) of a process.
 *
 **************************************/
	SLONG itemnum[2], ^item[2], itemstatus[2];
	SLONG status;
	struct {
		SLONG high;
		SLONG low;
	} process_id, found_id;

	if (!initialized)
		initialize();

	process_id.high = pin;
	process_id.low = pid;

	itemnum[0] = 2001;
	item[0] = &found_id;
	itemnum[1] = 0;

	GET_PRIVMODE if (pid)
		  AIFPROCGET(&status, itemnum, item, itemstatus,, process_id,);
	else
		AIFPROCGET(&status, itemnum, item, itemstatus, pin,,);
	GET_USERMODE if (found_pid)
		 *found_pid = found_id.low;

	return status;
}


SLONG AIF_lookup_file(TEXT * fid)
{
/**************************************
 *
 *	A I F _ l o o k u p _ f i l e
 *
 **************************************
 *
 * Functional description
 *	Find out if a file exists.
 *
 **************************************/
	SLONG itemnum[2], ^item[2], itemstatus[2];
	SLONG status;
	SCHAR filename[3][16], *p, *q, *comp_end, *end;

	if (!initialized)
		initialize();

	comp_end = filename[0] + sizeof(filename[0]);
	end = filename[0] + sizeof(filename);
	for (p = filename[0]; p < end; comp_end += sizeof(filename[0])) {
		while (*fid && *fid != '.')
			*p++ = *fid++;
		while (p < comp_end)
			*p++ = ' ';
		if (*fid == '.')
			fid++;
	}

	itemnum[0] = 5001;
	item[0] = filename;
	itemnum[1] = 0;

	GET_PRIVMODE AIFFILEGGET(&status, itemnum, item, itemstatus,, filename,,);
	GET_USERMODE return status;
}


SLONG AIF_interrupt_port(SLONG port, SLONG newstate)
{
/**************************************
 *
 *	A I F _ i n t e r r u p t _ p o r t
 *
 **************************************
 *
 * Functional description
 *	Change the interrupt state of a single port.
 *
 **************************************/
	SLONG status, port_list[2];
	SCHAR state[2];

	port_list[0] = port;
	port_list[1] = 0;
	state[0] = newstate;

	GET_PRIVMODE AIFPORTINT(&status, port_list, state,);
	GET_USERMODE return status;
}


SLONG AIF_open_port(port, port_name, port_pass, access_mode, itemnum, item)
	 SLONG *port,
		 TEXT port_name[16],
		 TEXT port_pass[16],
		 SLONG access_mode, SLONG itemnum[], SLONG ^ item[]) {
/**************************************
 *
 *	A I F _ o p e n _ p o r t
 *
 **************************************
 *
 * Functional description
 *	Open, and possibly create, a Port.
 *
 **************************************/
		 SLONG status, itemstatus[7];

		 if (!initialized)
			 initialize();

		 GET_PRIVMODE
			 * port = AIFPORTOPEN(&status, port_name, port_pass, access_mode,,
								  itemnum, item, itemstatus);
		 GET_USERMODE return status;
	 }
		 SLONG AIF_receive_port(SLONG port,
								SLONG * msg_code,
								SCHAR * msg,
								SLONG * msg_len,
								SLONG itemnum[], SLONG ^ item[])
{
/**************************************
 *
 *	A I F _ r e c e i v e _ p o r t
 *
 **************************************
 *
 * Functional description
 *	Receive a message through a previously opened Port.
 *
 **************************************/
	SLONG status, itemstatus[4];

	GET_PRIVMODE
		AIFPORTRECEIVE(&status, &port, msg, msg_len, msg_code,, itemnum, item,
					   itemstatus);
	GET_USERMODE return status;
}


SLONG AIF_send_port(SLONG port,
					SLONG msg_code,
					SCHAR * msg,
					SLONG msg_len, SLONG itemnum[], SLONG ^ item[]) {
/**************************************
 *
 *	A I F _ s e n d _ p o r t
 *
 **************************************
 *
 * Functional description
 *	Send a message through a previously opened Port.
 *
 **************************************/
	SLONG status, itemstatus[4];

	GET_PRIVMODE
		AIFPORTSEND(&status, &port, msg, msg_len, msg_code,, itemnum, item,
					itemstatus);
	GET_USERMODE return status;
}


static void initialize(void) {
/**************************************
 *
 *	i n i t i a l i z e
 *
 **************************************
 *
 * Functional description
 *	Initialize the Architected Interface Facility.
 *
 **************************************/ SLONG status;

	  initialized = 1;

	GET_PRIVMODE AIFACCESSON(&status, USER_ID);
  GET_USERMODE}
