#include "firebird.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jrd/common.h"
#include "../jrd/gds_proto.h"
#include "../jrd/ibase.h"
#include "../common/classes/ClumpletWriter.h"

typedef bool PopulateFunction(char**&, Firebird::ClumpletWriter&, unsigned char);

bool putStringArgument(char**& av, Firebird::ClumpletWriter& spb, unsigned char tag)
{
	if (! *av)
		return false;

	Firebird::string s(*av++);
	spb.insertString(tag, s);

	return true;
}

bool putSingleTag(char**&, Firebird::ClumpletWriter& spb, unsigned char tag)
{
	spb.insertTag(tag);

	return true;
}

struct switches
{
	const char* name;
	PopulateFunction* populate;
	unsigned char tag;
};

bool switchTable(char**& av, Firebird::ClumpletWriter& spb, const switches* sw)
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
			return sw->populate(av, spb, sw->tag);
		}
		++sw;
	}

	return false;
}

const switches attSwitch[] = {
	{"user", putStringArgument, isc_spb_user_name},
	{"password", putStringArgument, isc_spb_password},
	{0, 0, 0}
};
	
const switches infSwitch[] = {
	{"info_server_version", putSingleTag, isc_info_svc_server_version},
	{"info_implementation", putSingleTag, isc_info_svc_implementation},
	{"info_user_dbpath", putSingleTag, isc_info_svc_user_dbpath},
	{"info_get_env", putSingleTag, isc_info_svc_get_env},
	{"info_get_env_lock", putSingleTag, isc_info_svc_get_env_lock},
	{"info_get_env_msg", putSingleTag, isc_info_svc_get_env_msg},
	{"info_svr_db_info", putSingleTag, isc_info_svc_svr_db_info},
	{"info_version", putSingleTag, isc_info_svc_version},
	{0, 0, 0}
};

void printString(char*& p, const char* text)
{
	unsigned short length = (unsigned short)
		isc_vax_integer (p, sizeof(unsigned short));
	p += sizeof (unsigned short);
	char x = p[length];
	p[length] = 0;
	printf ("%s: %s\n", text, p);
	p += length;
	*p = x;
}

void printNumeric(char*& p, const char* text)
{
	unsigned int num = (unsigned int)
		isc_vax_integer (p, sizeof(unsigned int));
	p += sizeof (unsigned int);
	printf ("%s: %d\n", text, num);
}

int main(int ac, char **av)
{
	ISC_STATUS_ARRAY status;
	av++;

	char* name = *av;
	if (name)
	{
		av++;
	}	

	Firebird::ClumpletWriter spbAtt(Firebird::ClumpletWriter::SpbAttach, 16384, isc_spb_current_version);
	while (switchTable(av, spbAtt, attSwitch))
		;
	isc_svc_handle svc_handle = 0;
	if (isc_service_attach(status, 
				0, name, &svc_handle, 
				static_cast<USHORT>(spbAtt.getBufferLength()), 
				reinterpret_cast<const char*>(spbAtt.getBuffer())))
	{
		isc_print_status(status);
		return 1;
	}
	
	Firebird::ClumpletWriter spbItems(Firebird::ClumpletWriter::SpbItems, 256);
	while (switchTable(av, spbItems, infSwitch))
		;
	if (spbItems.getBufferLength() > 0)
	{
		char results[16384];
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
		
		char *p = results;
		do
		{
			switch (*p++)
			{
			case isc_info_truncated:
				printf ("Truncated\n");
				break;
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
					}
				} while (*p != isc_info_flag_end);
				break;
			}
		}  while (*p);
	}

	isc_service_detach(status, &svc_handle);
	return 0;
}
