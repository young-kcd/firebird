/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		gserv.cpp
 *	DESCRIPTION:	Command line interface with services manager
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
 *  The Original Code was created by Alex Peshkov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2007 Alex Peshkov <peshkoff@mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 */

#include "firebird.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jrd/common.h"
#include "../jrd/gds_proto.h"
#include "../jrd/ibase.h"
#include "../common/classes/ClumpletWriter.h"
#include "../common/utils_proto.h"

typedef bool PopulateFunction(char**&, Firebird::ClumpletWriter&, unsigned char);

bool putStringArgument(char**& av, Firebird::ClumpletWriter& spb, unsigned char tag)
{
	if (! *av)
		return false;

	char* x = *av++;
	Firebird::string s(tag == isc_spb_password ? fb_utils::get_passwd(x) : x);
	spb.insertString(tag, s);

	return true;
}

bool putNumericArgument(char**& av, Firebird::ClumpletWriter& spb, unsigned char tag)
{
	if (! *av)
		return false;

	int n = atoi(*av++);
	spb.insertInt(tag, n);

	return true;
}

bool putOption(char**& av, Firebird::ClumpletWriter& spb, unsigned char tag)
{
	if (! *av)
		return false;

	spb.insertInt(isc_spb_options, tag);

	return true;
}

bool putSingleTag(char**&, Firebird::ClumpletWriter& spb, unsigned char tag)
{
	spb.insertTag(tag);

	return true;
}

struct Switches
{
	const char* name;
	PopulateFunction* populate;
	const Switches* options;
	unsigned char tag;
	unsigned char tagInf;
};

bool populateSpbFromSwitches(char**& av, 
		Firebird::ClumpletWriter& spb, 
		const Switches* sw, 
		Firebird::ClumpletWriter* infoSpb)
{
	if (! *av)
		return false;

	Firebird::string s(*av);
	if (s[0] == '-')
	{
		s.erase(0, 1);
	}
	s.lower();

	while (sw->name)
	{
		if (s == sw->name)
		{
			av++;
			if (sw->populate(av, spb, sw->tag))
			{
				if (infoSpb && sw->tagInf)
				{
					infoSpb->insertTag(sw->tagInf);
				}
				if (sw->options)
				{
					while (populateSpbFromSwitches(av, spb, sw->options, infoSpb))
						;
					return false;
				}
				return true;
			}
			return false;
		}
		++sw;
	}

	return false;
}

const Switches attSwitch[] = {
	{"user", putStringArgument, 0, isc_spb_user_name, 0},
	{"password", putStringArgument, 0, isc_spb_password, 0},
	{"trusted", putSingleTag, 0, isc_spb_trusted_auth, 0},
	{0, 0, 0, 0, 0}
};
	
const Switches infSwitch[] = {
	{"info_server_version", putSingleTag, 0, isc_info_svc_server_version, 0},
	{"info_implementation", putSingleTag, 0, isc_info_svc_implementation, 0},
	{"info_user_dbpath", putSingleTag, 0, isc_info_svc_user_dbpath, 0},
	{"info_get_env", putSingleTag, 0, isc_info_svc_get_env, 0},
	{"info_get_env_lock", putSingleTag, 0, isc_info_svc_get_env_lock, 0},
	{"info_get_env_msg", putSingleTag, 0, isc_info_svc_get_env_msg, 0},
	{"info_svr_db_info", putSingleTag, 0, isc_info_svc_svr_db_info, 0},
	{"info_version", putSingleTag, 0, isc_info_svc_version, 0},
	{0, 0, 0, 0, 0}
};

const Switches backupOptions[] = {
	{"dbname", putStringArgument, 0, isc_spb_dbname, 0},
	{"verbose", putSingleTag, 0, isc_spb_verbose, 0},
	{"bkp_file", putStringArgument, 0, isc_spb_bkp_file, 0},
	{"bkp_length", putNumericArgument, 0, isc_spb_bkp_length, 0},
	{"bkp_factor", putNumericArgument, 0, isc_spb_bkp_factor, 0},
	{"bkp_ignore_checksums", putOption, 0, isc_spb_bkp_ignore_checksums, 0},
	{"bkp_ignore_limbo", putOption, 0, isc_spb_bkp_ignore_limbo, 0},
	{"bkp_metadata_only", putOption, 0, isc_spb_bkp_metadata_only, 0},
	{"bkp_no_garbage_collect", putOption, 0, isc_spb_bkp_no_garbage_collect, 0},
	{"bkp_old_descriptions", putOption, 0, isc_spb_bkp_old_descriptions, 0},
	{"bkp_non_transportable", putOption, 0, isc_spb_bkp_non_transportable, 0},
	{"bkp_convert", putOption, 0, isc_spb_bkp_convert, 0},
	{0, 0, 0, 0, 0}
};

const Switches actionSwitch[] = {
	{"action_backup", putSingleTag, backupOptions, isc_action_svc_backup, isc_info_svc_line},
	{"action_restore", putSingleTag, 0, isc_action_svc_restore, 0},
	{"action_properties", putSingleTag, 0, isc_action_svc_properties, 0},
	{"action_repair", putSingleTag, 0, isc_action_svc_repair, 0},
	{"action_db_stats", putSingleTag, 0, isc_action_svc_db_stats, 0},
	{"action_get_ib_log", putSingleTag, 0, isc_action_svc_get_ib_log, 0},
	{"action_display_user", putSingleTag, 0, isc_action_svc_display_user, 0},
	{"action_add_user", putSingleTag, 0, isc_action_svc_add_user, 0},
	{"action_delete_user", putSingleTag, 0, isc_action_svc_delete_user, 0},
	{"action_modify_user", putSingleTag, 0, isc_action_svc_modify_user, 0},
	{0, 0, 0, 0, 0}
};

bool printLine(const char*& p)
{
	unsigned short length = (unsigned short)
		isc_vax_integer (p, sizeof(unsigned short));
	p += sizeof (unsigned short);
	if (! length)
	{
		return false;
	}
	
	printf ("%.*s\n", length, p);
	p += length;
	return true;
}

void printString(const char*& p, const char* text)
{
	printf ("%s: ", text);
	if (!printLine(p))
	{
		printf ("<no data>\n");
	}
}

void printNumeric(const char*& p, const char* text)
{
	unsigned int num = (unsigned int)
		isc_vax_integer (p, sizeof(unsigned int));
	p += sizeof (unsigned int);
	printf ("%s: %d\n", text, num);
}

bool printInfo(const char* p)
{
	while (*p != isc_info_end)
	{
		switch (*p++)
		{
		case isc_info_svc_version:
			printNumeric(p, "Service Manager Version");
			break;
		case isc_info_svc_server_version:
			printString(p, "Server version");
			break;
		case isc_info_svc_implementation:
			printString(p, "Server implementation");
			break;
		case isc_info_svc_get_env_msg:
			printString(p, "Path to firebird.msg");
			break;
		case isc_info_svc_get_env:
			printString(p, "Server root");
			break;
		case isc_info_svc_get_env_lock:
			printString(p, "Path to lock files");
			break;
		case isc_info_svc_user_dbpath:
			printString(p, "Security database");
			break;

		case isc_info_svc_svr_db_info:
			printf ("Databases:\n");
			do {
				switch (*p++)
				{
				case isc_spb_dbname:
					printString(p, "   Database in use");
					break;
				case isc_spb_num_att:
					printNumeric(p, "   Number of attachments");
					break;
				case isc_spb_num_db:
					printNumeric(p, "   Number of databases");
					break;
				default:
					printf("Unknown code (%d) in info_svr_db_info\n", p[-1]);
					return false;
				}
			} while (*p != isc_info_flag_end);
			p++;
			break;

		case isc_info_svc_line:
			return printLine(p);
		case isc_info_truncated:
			printf ("Truncated\n");
			return false;
		default:
			printf("Unknown tag in isc_svc_query() results (%d)\n", p[-1]);
			return false;
		}
	}

	return false;
}

int main(int ac, char **av)
{
	ISC_STATUS_ARRAY status;

	try {
		const int maxbuf = 16384;
		av++;

		const char* name = *av;
		if (name)
		{
			av++;
		}	

		Firebird::ClumpletWriter spbAtt(Firebird::ClumpletWriter::SpbAttach, maxbuf, isc_spb_current_version);
		while (populateSpbFromSwitches(av, spbAtt, attSwitch, 0))
			;

		Firebird::ClumpletWriter spbStart(Firebird::ClumpletWriter::SpbStart, maxbuf);
		Firebird::ClumpletWriter spbItems(Firebird::ClumpletWriter::SpbItems, 256);
		while (populateSpbFromSwitches(av, spbStart, actionSwitch, &spbItems))
			;

		if (spbStart.getBufferLength() == 0)
		{
			while (populateSpbFromSwitches(av, spbItems, infSwitch, 0))
				;
		}

		//Here we are over with av parse, look - may be some unknown switch left
		if (*av)
		{
			printf("Unknown switch '%s'\n", *av);
			return 1;
		}

		isc_svc_handle svc_handle = 0;
		if (isc_service_attach(status, 
					0, name, &svc_handle, 
					static_cast<USHORT>(spbAtt.getBufferLength()), 
					reinterpret_cast<const char*>(spbAtt.getBuffer())))
		{
			isc_print_status(status);
			return 1;
		}
	
		if (spbStart.getBufferLength() > 0)
		{
			if (isc_service_start(status,
					&svc_handle, 0,
					static_cast<USHORT>(spbStart.getBufferLength()), 
					reinterpret_cast<const char*>(spbStart.getBuffer())))
			{
				isc_print_status(status);
				isc_service_detach(status, &svc_handle);
				return 1;
			}
		}

		if (spbItems.getBufferLength() > 0)
		{
			char results[maxbuf];
			do
			{
				if (isc_service_query(status,
						&svc_handle, 0, 0, 0,
						static_cast<USHORT>(spbItems.getBufferLength()), 
						reinterpret_cast<const char*>(spbItems.getBuffer()),
						sizeof(results), results))
				{
					isc_print_status(status);
					isc_service_detach(status, &svc_handle);
					return 1;
				}
			} while(printInfo(results));
		}

		isc_service_detach(status, &svc_handle);
		return 0;
	}
	catch(const Firebird::Exception& e)
	{
		e.stuff_exception(status);
		isc_print_status(status);
	}
	return 2;
}
