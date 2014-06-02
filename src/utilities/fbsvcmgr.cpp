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
#include "../common/classes/MsgPrint.h"

// Here we define main control structure

typedef bool PopulateFunction(char**&, Firebird::ClumpletWriter&, unsigned int);

struct Switches
{
	const char* name;
	PopulateFunction* populate;
	const Switches* options;
	unsigned int tag;
	unsigned char tagInf;
};

// Get message from security database

Firebird::string getMessage(int n)
{
	char buffer[256];
	const int FACILITY = 22;

	fb_msg_format(0, FACILITY, n, sizeof(buffer), buffer, MsgFormat::SafeArg());

	return Firebird::string(buffer);
}

Firebird::string prepareSwitch(const char* arg)
{
	Firebird::string s(arg);
	if (s[0] == '-')
	{
		s.erase(0, 1);
	}
	s.lower();

	return s;
}

// add string tag to spb

bool putStringArgument(char**& av, Firebird::ClumpletWriter& spb, unsigned int tag)
{
	if (! *av)
		return false;

	char* x = *av++;
	Firebird::string s(tag == isc_spb_password ? fb_utils::get_passwd(x) : x);
	spb.insertString(tag, s);

	return true;
}

// add some special format tags to spb

bool putSpecTag(char**& av, Firebird::ClumpletWriter& spb, unsigned int tag,
				const Switches* sw, ISC_STATUS errorCode)
{
	if (! *av)
		return false;

	const Firebird::string s(prepareSwitch(*av++));
	for (; sw->name; ++sw)
	{
		if (s == sw->name)
		{
			spb.insertByte(tag, sw->tag);
			return true;
		}
	}

	Firebird::status_exception::raise(errorCode, isc_arg_end);
	return false;	// compiler warning silencer
}

const Switches amSwitch[] = {
	{"prp_am_readonly", 0, 0, isc_spb_prp_am_readonly, 0},
	{"prp_am_readwrite", 0, 0, isc_spb_prp_am_readwrite, 0},
	{0, 0, 0, 0, 0}
};

bool putAccessMode(char**& av, Firebird::ClumpletWriter& spb, unsigned int tag)
{
	return putSpecTag(av, spb, tag, amSwitch, isc_fbsvcmgr_bad_am);
}

const Switches wmSwitch[] = {
	{"prp_wm_async", 0, 0, isc_spb_prp_wm_async, 0},
	{"prp_wm_sync", 0, 0, isc_spb_prp_wm_sync, 0},
	{0, 0, 0, 0, 0}
};

bool putWriteMode(char**& av, Firebird::ClumpletWriter& spb, unsigned int tag)
{
	return putSpecTag(av, spb, tag, wmSwitch, isc_fbsvcmgr_bad_wm);
}

const Switches rsSwitch[] = {
	{"prp_res_use_full", 0, 0, isc_spb_prp_res_use_full, 0},
	{"prp_res", 0, 0, isc_spb_prp_res, 0},
	{0, 0, 0, 0, 0}
};

bool putReserveSpace(char**& av, Firebird::ClumpletWriter& spb, unsigned int tag)
{
	return putSpecTag(av, spb, tag, rsSwitch, isc_fbsvcmgr_bad_rs);
}

// add numeric (int32) tag to spb

bool putNumericArgument(char**& av, Firebird::ClumpletWriter& spb, unsigned int tag)
{
	if (! *av)
		return false;

	int n = atoi(*av++);
	spb.insertInt(tag, n);

	return true;
}

// add boolean option to spb

bool putOption(char**&, Firebird::ClumpletWriter& spb, unsigned int tag)
{
	spb.insertInt(isc_spb_options, tag);

	return true;
}

// add argument-less tag to spb

bool putSingleTag(char**&, Firebird::ClumpletWriter& spb, unsigned int tag)
{
	spb.insertTag(tag);

	return true;
}

// populate spb with tags according to user-defined command line switches
// and programmer-defined set of Switches array

bool populateSpbFromSwitches(char**& av, 
		Firebird::ClumpletWriter& spb, 
		const Switches* sw, 
		Firebird::ClumpletWriter* infoSpb)
{
	if (! *av)
		return false;

	const Firebird::string s(prepareSwitch(*av));

	for (; sw->name; ++sw)
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
	}

	return false;
}

const Switches attSwitch[] = {
	{"user", putStringArgument, 0, isc_spb_user_name, 0},
	{"user_name", putStringArgument, 0, isc_spb_user_name, 0},
	{"password", putStringArgument, 0, isc_spb_password, 0},
	{"trusted_auth", putSingleTag, 0, isc_spb_trusted_auth, 0},
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

const Switches restoreOptions[] = {
	{"bkp_file", putStringArgument, 0, isc_spb_bkp_file, 0},
	{"dbname", putStringArgument, 0, isc_spb_dbname, 0},
	{"res_length", putNumericArgument, 0, isc_spb_res_length, 0},
	{"verbose", putSingleTag, 0, isc_spb_verbose, 0},
	{"res_buffers", putNumericArgument, 0, isc_spb_res_buffers, 0},
	{"res_page_size", putNumericArgument, 0, isc_spb_res_page_size, 0},
	{"res_access_mode", putAccessMode, 0, isc_spb_res_access_mode, 0},
	{"res_deactivate_idx", putOption, 0, isc_spb_res_deactivate_idx, 0},
	{"res_no_shadow", putOption, 0, isc_spb_res_no_shadow, 0},
	{"res_no_validity", putOption, 0, isc_spb_res_no_validity, 0},
	{"res_one_at_a_time", putOption, 0, isc_spb_res_one_at_a_time, 0},
	{"res_replace", putOption, 0, isc_spb_res_replace, 0},
	{"res_create", putOption, 0, isc_spb_res_create, 0},
	{"res_use_all_space", putOption, 0, isc_spb_res_use_all_space, 0},
	{0, 0, 0, 0, 0}
};

const Switches propertiesOptions[] = {
	{"dbname", putStringArgument, 0, isc_spb_dbname, 0},
	{"prp_page_buffers", putNumericArgument, 0, isc_spb_prp_page_buffers, 0},
	{"prp_sweep_interval", putNumericArgument, 0, isc_spb_prp_sweep_interval, 0},
	{"prp_shutdown_db", putNumericArgument, 0, isc_spb_prp_shutdown_db, 0},
	{"prp_deny_new_transactions", putNumericArgument, 0, isc_spb_prp_deny_new_transactions, 0},
	{"prp_deny_new_attachments", putNumericArgument, 0, isc_spb_prp_deny_new_attachments, 0},
	{"prp_set_sql_dialect", putNumericArgument, 0, isc_spb_prp_set_sql_dialect, 0},
	{"prp_access_mode", putAccessMode, 0, isc_spb_prp_access_mode, 0},
	{"prp_reserve_space", putReserveSpace, 0, isc_spb_prp_reserve_space, 0},
	{"prp_write_mode", putWriteMode, 0, isc_spb_prp_write_mode, 0},
	{"prp_activate", putOption, 0, isc_spb_prp_activate, 0},
	{"prp_db_online", putOption, 0, isc_spb_prp_db_online, 0},
	{0, 0, 0, 0, 0}
};

const Switches repairOptions[] = {
	{"dbname", putStringArgument, 0, isc_spb_dbname, 0},
	{"rpr_commit_trans", putNumericArgument, 0, isc_spb_rpr_commit_trans, 0},
	{"rpr_rollback_trans", putNumericArgument, 0, isc_spb_rpr_rollback_trans, 0},
	{"rpr_recover_two_phase", putNumericArgument, 0, isc_spb_rpr_recover_two_phase, 0},
	{"rpr_check_db", putOption, 0, isc_spb_rpr_check_db, 0},
	{"rpr_ignore_checksum", putOption, 0, isc_spb_rpr_ignore_checksum, 0},
	{"rpr_kill_shadows", putOption, 0, isc_spb_rpr_kill_shadows, 0},
	{"rpr_mend_db", putOption, 0, isc_spb_rpr_mend_db, 0},
	{"rpr_validate_db", putOption, 0, isc_spb_rpr_validate_db, 0},
	{"rpr_full", putOption, 0, isc_spb_rpr_full, 0},
	{"rpr_sweep_db", putOption, 0, isc_spb_rpr_sweep_db, 0},
	{"rpr_list_limbo_trans", putOption, 0, isc_spb_rpr_list_limbo_trans, isc_info_svc_limbo_trans},
	{0, 0, 0, 0, 0}
};

const Switches statisticsOptions[] = {
	{"dbname", putStringArgument, 0, isc_spb_dbname, 0},
	{"sts_record_versions", putOption, 0, isc_spb_sts_record_versions, 0},
	{"sts_nocreation", putOption, 0, isc_spb_sts_nocreation, 0},
	{"sts_data_pages", putOption, 0, isc_spb_sts_data_pages, 0},
	{"sts_hdr_pages", putOption, 0, isc_spb_sts_hdr_pages, 0},
	{"sts_idx_pages", putOption, 0, isc_spb_sts_idx_pages, 0},
	{"sts_sys_relations", putOption, 0, isc_spb_sts_sys_relations, 0},
	{0, 0, 0, 0, 0}
};

const Switches dispdelOptions[] = {
	{"dbname", putStringArgument, 0, isc_spb_dbname, 0},
	{"sec_username", putStringArgument, 0, isc_spb_sec_username, 0},
	{"sql_role_name", putStringArgument, 0, isc_spb_sql_role_name, 0},
	{0, 0, 0, 0, 0}
};

const Switches addmodOptions[] = {
	{"dbname", putStringArgument, 0, isc_spb_dbname, 0},
	{"sec_username", putStringArgument, 0, isc_spb_sec_username, 0},
	{"sql_role_name", putStringArgument, 0, isc_spb_sql_role_name, 0},
	{"sec_password", putStringArgument, 0, isc_spb_sec_password, 0},
	{"sec_groupname", putStringArgument, 0, isc_spb_sec_groupname, 0},
	{"sec_firstname", putStringArgument, 0, isc_spb_sec_firstname, 0},
	{"sec_middlename", putStringArgument, 0, isc_spb_sec_middlename, 0},
	{"sec_lastname", putStringArgument, 0, isc_spb_sec_lastname, 0},
	{"sec_userid", putNumericArgument, 0, isc_spb_sec_userid, 0},
	{"sec_groupid", putNumericArgument, 0, isc_spb_sec_groupid, 0},
	{0, 0, 0, 0, 0}
};

const Switches actionSwitch[] = {
	{"action_backup", putSingleTag, backupOptions, isc_action_svc_backup, isc_info_svc_line},
	{"action_restore", putSingleTag, restoreOptions, isc_action_svc_restore, isc_info_svc_line},
	{"action_properties", putSingleTag, propertiesOptions, isc_action_svc_properties, 0},
	{"action_repair", putSingleTag, repairOptions, isc_action_svc_repair, 0},
	{"action_db_stats", putSingleTag, statisticsOptions, isc_action_svc_db_stats, isc_info_svc_line},
	{"action_get_ib_log", putSingleTag, 0, isc_action_svc_get_ib_log, isc_info_svc_line},
	{"action_display_user", putSingleTag, dispdelOptions, isc_action_svc_display_user, isc_info_svc_get_users},
	{"action_add_user", putSingleTag, addmodOptions, isc_action_svc_add_user, 0},
	{"action_delete_user", putSingleTag, dispdelOptions, isc_action_svc_delete_user, 0},
	{"action_modify_user", putSingleTag, addmodOptions, isc_action_svc_modify_user, 0},
	{0, 0, 0, 0, 0}
};

// print information, returned by isc_svc_query() call

bool getLine(Firebird::string& dest, const char*& p)
{
	unsigned short length = (unsigned short)
		isc_vax_integer (p, sizeof(unsigned short));
	p += sizeof (unsigned short);
	dest.assign(p, length);
	p += length;
	return length > 0;
}

int getNumeric(const char*& p)
{
	unsigned int num = (unsigned int)
		isc_vax_integer (p, sizeof(unsigned int));
	p += sizeof (unsigned int);
	return num;
}

bool printLine(const char*& p)
{
	Firebird::string s;
	bool rc = getLine(s, p);
	printf ("%s\n", s.c_str());
	return rc;
}

void printString(const char*& p, int num)
{
	printf ("%s: ", getMessage(num).c_str());
	if (!printLine(p))
	{
		printf ("<no data>\n");
	}
}

void printMessage(int num)
{
	printf ("%s\n", getMessage(num).c_str());
}

void printNumeric(const char*& p, int num)
{
	printf ("%s: %d\n", getMessage(num).c_str(), getNumeric(p));
}

class UserPrint
{
public:
	Firebird::string login, first, middle, last;
	int gid, uid;

private:
	bool hasData;

public:
	UserPrint() : hasData(false)
	{
		clear();
	}

	~UserPrint()
	{
		// print data, accumulated for last user
		newUser();
	}

	void clear()
	{
		login = first = middle = last = "";
		gid = uid = 0;
	}

	void newUser()
	{
		if (!hasData)
		{
			hasData = true;
			return;
		}
		printf("%-28.28s %-40.40s %4d %4d\n", login.c_str(),
			(first + " " + middle + " " + last).c_str(), uid, gid);
		clear();
	}
};

bool printInfo(const char* p, UserPrint& up)
{
	while (*p != isc_info_end)
	{
		switch (*p++)
		{
		case isc_info_svc_version:
			printNumeric(p, 7);
			break;
		case isc_info_svc_server_version:
			printString(p, 8);
			break;
		case isc_info_svc_implementation:
			printString(p, 9);
			break;
		case isc_info_svc_get_env_msg:
			printString(p, 10);
			break;
		case isc_info_svc_get_env:
			printString(p, 11);
			break;
		case isc_info_svc_get_env_lock:
			printString(p, 12);
			break;
		case isc_info_svc_user_dbpath:
			printString(p, 13);
			break;

		case isc_info_svc_svr_db_info:
			printf ("%s:\n", getMessage(14).c_str());
			while (*p != isc_info_flag_end) {
				switch (*p++)
				{
				case isc_spb_dbname:
					printString(p, 15);
					break;
				case isc_spb_num_att:
					printNumeric(p, 16);
					break;
				case isc_spb_num_db:
					printNumeric(p, 17);
					break;
				default:
					Firebird::status_exception::raise(isc_fbsvcmgr_info_err, isc_arg_number, 
						static_cast<unsigned char>(p[-1]), isc_arg_end);
				}
			}
			p++;
			break;

		case isc_info_svc_limbo_trans:
			while (*p != isc_info_flag_end)
			{
				switch (*p++)
				{
				case isc_spb_tra_host_site:
					printString(p, 36);
					break;
				case isc_spb_tra_state:
					switch(*p++)
					{
					case isc_spb_tra_state_limbo:
			            printMessage(38);
						break;
					case isc_spb_tra_state_commit:
			            printMessage(39);
						break;
					case isc_spb_tra_state_rollback:
			            printMessage(40);
						break;
					case isc_spb_tra_state_unknown:
			            printMessage(41);
						break;
					default:
						Firebird::status_exception::raise(isc_fbsvcmgr_info_err, isc_arg_number, 
							static_cast<unsigned char>(p[-1]), isc_arg_end);
					}
					break;
				case isc_spb_tra_remote_site:
					printString(p, 42);
					break;
				case isc_spb_tra_db_path:
					printString(p, 43);
					break;
				case isc_spb_tra_advise:
					switch(*p++)
					{
					case isc_spb_tra_advise_commit:
			            printMessage(44);
						break;
					case isc_spb_tra_advise_rollback:
			            printMessage(45);
						break;
					case isc_spb_tra_advise_unknown:
			            printMessage(46);
						break;
					default:
						Firebird::status_exception::raise(isc_fbsvcmgr_info_err, isc_arg_number, 
							static_cast<unsigned char>(p[-1]), isc_arg_end);
					}
					break;
				case isc_spb_multi_tra_id:
					printNumeric(p, 35);
					break;
				case isc_spb_single_tra_id:
					printNumeric(p, 34);
					break;
				case isc_spb_tra_id:
					printNumeric(p, 37);
					break;
				default:
					Firebird::status_exception::raise(isc_fbsvcmgr_info_err, isc_arg_number, 
						static_cast<unsigned char>(p[-1]), isc_arg_end);
				}
			}
			p++;
			break;

		case isc_info_svc_get_users:
			p += sizeof(unsigned short);
			break;
		case isc_spb_sec_username:
			up.newUser();
			getLine(up.login, p);
			break;
		case isc_spb_sec_firstname:
			getLine(up.first, p);
			break;
		case isc_spb_sec_middlename:
			getLine(up.middle, p);
			break;
		case isc_spb_sec_lastname:
			getLine(up.last, p);
			break;
		case isc_spb_sec_groupid:
			up.gid = getNumeric(p);
			break;
		case isc_spb_sec_userid:
			up.uid = getNumeric(p);
			break;

		case isc_info_svc_line:
			return printLine(p);

		case isc_info_truncated:
			printf ("%s\n", getMessage(18).c_str());
			return false;

		default:
			Firebird::status_exception::raise(isc_fbsvcmgr_query_err, isc_arg_number, 
				static_cast<unsigned char>(p[-1]), isc_arg_end);
		}
	}

	return false;
}

// short usage from firebird.msg

void usage()
{
	for (int i = 19; i <= 33; ++i)
	{
		printf("%s\n", getMessage(i).c_str());
	}
}

// simple main function

int main(int ac, char **av)
{
	if (ac < 2)
	{
		usage();
		return 1;
	}

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
		// single action per one utility run, it may populate info items also
		populateSpbFromSwitches(av, spbStart, actionSwitch, &spbItems);

		if (spbStart.getBufferLength() == 0)
		{
			while (populateSpbFromSwitches(av, spbItems, infSwitch, 0))
				;
		}

		//Here we are over with av parse, look - may be unknown switch left
		if (*av)
		{
			Firebird::status_exception::raise(isc_fbsvcmgr_switch_unknown, isc_arg_string, *av, isc_arg_end);
		}

		if (!spbAtt.find(isc_spb_trusted_auth))
		{
			// Populate SPB with environment variables
			Firebird::string env;
			if ((!spbAtt.find(isc_spb_user_name)) && fb_utils::readenv("ISC_USER", env))
			{
				spbAtt.insertString(isc_spb_user_name, env);
			}
			if ((!spbAtt.find(isc_spb_password)) && fb_utils::readenv("ISC_PASSWORD", env))
			{
				spbAtt.insertString(isc_spb_password, env);
			}
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
			UserPrint up;
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
			} while(printInfo(results, up));
		}

		isc_service_detach(status, &svc_handle);
		return 0;
	}
	catch (const Firebird::Exception& e)
	{
		e.stuff_exception(status);
		isc_print_status(status);
	}
	return 2;
}
