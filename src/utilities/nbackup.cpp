/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		nbackup.cpp
 *	DESCRIPTION:	Command line utility for physical backup/restore
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
 *  $Id: nbackup.cpp,v 1.38 2004-11-10 04:25:51 robocop Exp $
 *
 */
 

// firebird.h must go first because otherwise we'll not get 64-bit IO
#include "firebird.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../jrd/common.h"
#include "../jrd/db_alias.h"
#include "../jrd/ods.h"
#include "../jrd/nbak.h"
#include "../jrd/gds_proto.h"
#include "../jrd/os/path_utils.h"
#include "../jrd/os/guid.h"
#include "../jrd/ibase.h"
#include "../common/classes/array.h"
#include <typeinfo>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

#ifndef O_LARGEFILE
#define O_LARGEFILE 0
#endif

const int EXIT_OK		= 0;
const int EXIT_ERROR	= 1;

void usage()
{
	fprintf(stderr, 
		"Physical Backup Manager    Copyright (C) 2004 Firebird development team\n"
		"  Original idea is of Sean Leyne <sean@broadviewsoftware.com>\n"
		"  Designed and implemented by Nickolay Samofatov <skidder@bssys.com>\n"
		"  This work was funded through a grant from BroadView Software, Inc.\n\n"
		"Usage: nbackup <options>\n"
		"valid options are: \n"
		"  -L <database>                         Lock database for filesystem copy\n"
		"  -N <database>                         Unlock previously locked database\n"
		"  -F <database>                         Fixup database after filesystem copy\n"
		"  -B <level> <database> [<filename>]    Create incremental backup\n"
		"  -R <database> [<file0> [<file1>...]]  Restore incremental backup\n"
		"  -U <user>                             User name\n"
		"  -P <password>                         Password\n"
		"Notes:\n"
		"  <database> may specify database alias\n"
		"  incremental backups of multi-file databases are not supported yet\n"
		"  \"stdout\" may be used as a value of <filename> for -B option\n"
					);
	exit(EXIT_ERROR);
}

void missing_parameter_for_switch(const char* sw) {
	fprintf(stderr, "ERROR: Missing parameter for switch %s.\n\n", sw);
	usage();
} 

class b_error : public std::exception
{
public:
	explicit b_error(const char* message) {
		size_t len = sizeof(txt) - 1;
		strncpy(txt, message, len);
		txt[len] = 0;
	}
	enum {MSG_LEN = 1024};
	virtual ~b_error() throw() {}
	virtual const char* what() const throw()
		{ return txt; }
	static void raise(const char* message, ...) {
		char temp[MSG_LEN];
		va_list params;
		va_start(params, message);
		VSNPRINTF(temp, sizeof(temp), message, params);
		temp[sizeof(temp) - 1] = 0;
		va_end(params);
		fprintf(stderr, "Failure: %s\n", temp);
		throw b_error(temp);
	}
private:
	char txt[MSG_LEN];
};


#ifdef WIN_NT
#define FILE_HANDLE HANDLE
// INVALID_SET_FILE_POINTER is not defined in MSVC6
#ifndef INVALID_SET_FILE_POINTER
#define INVALID_SET_FILE_POINTER ((DWORD)-1)
#endif
#else
#define FILE_HANDLE int
#endif


const char local_prefix[] = "localhost:";

const char backup_signature[4] = {'N','B','A','K'};

struct inc_header {
	char signature[4];		// 'NBAK'
	SSHORT version;			// Incremental backup format version.
	SSHORT level;			// Backup level.
	// \\\\\ ---- this is 8 bytes. should not cause alignment problems
	FB_GUID backup_guid;	// GUID of this backup
	FB_GUID prev_guid;		// GUID of previous level backup
	ULONG page_size;		// Size of pages in the database and backup file
	// These fields are currently filled, but not used. May be used in future versions
	ULONG backup_scn;		// SCN of this backup
	ULONG prev_scn;			// SCN of previous level backup
};

class nbackup {
public:
	nbackup(const char* _database, const char* _username, const char* _password)
	{
		if (_username) username = _username;
		if (_password) password = _password;
		database = _database;

		dbase = 0;
		backup = 0;
		newdb = 0;
		trans = 0;
		// Recognition of local prefix allows to work with
		// database using TCP/IP loopback while reading file locally.
		// This makes NBACKUP compatible with Windows CS with XNET disabled
		if (strncmp(_database, local_prefix, sizeof(local_prefix) - 1) == 0) {
			_database += sizeof(local_prefix) - 1;
		}
		if (!ResolveDatabaseAlias(_database, dbname))
			dbname = _database;
	}
	// External calls must clean up resources after themselves
	void fixup_database();
	void lock_database();
	void unlock_database();
	void backup_database(int level, const char* fname);
	void restore_database(int filecount, const char* const* files);
private:
    ISC_STATUS_ARRAY status; /* status vector */
	isc_db_handle newdb; /* database handle */
    isc_tr_handle trans; /* transaction handle */
	
	Firebird::PathName database;
	Firebird::string username;
	Firebird::string password;

	Firebird::PathName dbname; // Database file name
	Firebird::PathName bakname;
	FILE_HANDLE dbase;
	FILE_HANDLE backup;
	
	// IO functions
	size_t read_file(FILE_HANDLE &file, void *buffer, size_t bufsize);		
	void write_file(FILE_HANDLE &file, void *buffer, size_t bufsize);		
	void seek_file(FILE_HANDLE &file, SINT64 pos);
	
	static void pr_error(const ISC_STATUS* status, const char* operation);
	
	void internal_lock_database();
	void internal_unlock_database();
	void attach_database();
	void detach_database();
	
	// Create/open database and backup
	void open_database_write();
	void open_database_scan();
	void create_database();
	void close_database();
		
	void open_backup_scan();
	void create_backup();
	void close_backup();
};

size_t nbackup::read_file(FILE_HANDLE &file, void *buffer, size_t bufsize)
{
#ifdef WIN_NT
	DWORD bytesDone;
	if (!ReadFile(file, buffer, bufsize, &bytesDone, NULL))
		b_error::raise("IO error (%d) reading file: %s", 
			GetLastError(),
			&file == &dbase ? dbname.c_str() :
			&file == &backup ? bakname.c_str() : "unknown");
	return bytesDone;
#else
	const ssize_t res = read(file, buffer, bufsize);
	if (res < 0)
		b_error::raise("IO error (%d) reading file: %s", 
			errno,
			&file == &dbase ? dbname.c_str() :
			&file == &backup ? bakname.c_str() : "unknown");
	return res;
#endif
}

void nbackup::write_file(FILE_HANDLE &file, void *buffer, size_t bufsize)
{
#ifdef WIN_NT
	DWORD bytesDone;
	if (!WriteFile(file, buffer, bufsize, &bytesDone, NULL) ||
		bytesDone != bufsize)
	{
		b_error::raise("IO error (%d) writing file: %s", 
			GetLastError(),
			&file == &dbase ? dbname.c_str() :
			&file == &backup ? bakname.c_str() : "unknown");
	}
#else
	if (write(file, buffer, bufsize) != (ssize_t)bufsize)
		b_error::raise("IO error (%d) writing file: %s", 
			errno,
			&file == &dbase ? dbname.c_str() :
			&file == &backup ? bakname.c_str() : "unknown");
#endif
}

void nbackup::seek_file(FILE_HANDLE &file, SINT64 pos)
{
#ifdef WIN_NT
	LARGE_INTEGER offset;
	offset.QuadPart = pos;
	DWORD error;
	if (SetFilePointer(dbase, offset.LowPart, &offset.HighPart, 
					    FILE_BEGIN) == INVALID_SET_FILE_POINTER &&
		 (error = GetLastError()) != NO_ERROR)
	{
		b_error::raise("IO error (%d) seeking file: %s", 
			error,
			&file == &dbase ? dbname.c_str() :
			&file == &backup ? bakname.c_str() : "unknown");
	}
#else
	if (lseek(file, pos, SEEK_SET) == (off_t) - 1)
		b_error::raise("IO error (%d) seeking file: %s", 
			errno,
			&file == &dbase ? dbname.c_str() :
			&file == &backup ? bakname.c_str() : "unknown");
#endif
}

void nbackup::open_database_write()
{
#ifdef WIN_NT
	dbase = CreateFile(dbname.c_str(), GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (dbase == INVALID_HANDLE_VALUE)
		b_error::raise("Error (%d) opening database file: %s", GetLastError(), dbname.c_str());
#else
	dbase = open(dbname.c_str(), O_RDWR | O_LARGEFILE);
	if (dbase < 0)
		b_error::raise("Error (%d) opening database file: %s", errno, dbname.c_str());	
#endif
}

void nbackup::open_database_scan()
{
#ifdef WIN_NT
	dbase = CreateFile(dbname.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (dbase == INVALID_HANDLE_VALUE)
		b_error::raise("Error (%d) opening database file: %s", GetLastError(), dbname.c_str());
#else
	dbase = open(dbname.c_str(), O_RDONLY | O_LARGEFILE);
	if (dbase < 0)
		b_error::raise("Error (%d) opening database file: %s", errno, dbname.c_str());	
#endif
}

void nbackup::create_database()
{
#ifdef WIN_NT
	dbase = CreateFile(dbname.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_DELETE, 
		NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (dbase == INVALID_HANDLE_VALUE)
		b_error::raise("Error (%d) creating database file: %s", GetLastError(), dbname.c_str());
#else
	dbase = open(dbname.c_str(), O_RDWR | O_CREAT | O_EXCL | O_LARGEFILE, 0660);
	if (dbase < 0)
		b_error::raise("Error (%d) creating database file: %s", errno, dbname.c_str());	
#endif
}

void nbackup::close_database()
{
#ifdef WIN_NT
	CloseHandle(dbase);
#else
	close(dbase);
#endif
}

void nbackup::open_backup_scan()
{
#ifdef WIN_NT
	backup = CreateFile(bakname.c_str(), GENERIC_READ, 0, 
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (backup == INVALID_HANDLE_VALUE)
		b_error::raise("Error (%d) opening backup file: %s", GetLastError(), bakname.c_str());
#else
	backup = open(bakname.c_str(), O_RDONLY | O_LARGEFILE);
	if (backup < 0)
		b_error::raise("Error (%d) opening backup file: %s", errno, bakname.c_str());	
#endif
}

void nbackup::create_backup()
{
#ifdef WIN_NT
	if (bakname == "stdout") {
		backup = GetStdHandle(STD_OUTPUT_HANDLE);
	}
	else {		
		backup = CreateFile(bakname.c_str(), GENERIC_WRITE, FILE_SHARE_DELETE, 
			NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	}
	if (backup == INVALID_HANDLE_VALUE)
		b_error::raise("Error (%d) creating backup file: %s", GetLastError(), bakname.c_str());
#else
	if (bakname == "stdout") {
		backup = 1 /* Posix file handle for stdout */;
	}
	else {
		backup = open(bakname.c_str(), O_WRONLY | O_CREAT | O_EXCL | O_LARGEFILE, 0660);
		if (backup < 0)
			b_error::raise("Error (%d) creating backup file: %s", errno, bakname.c_str());
	}
#endif
}

void nbackup::close_backup()
{
	if (bakname == "stdout")
		return;
#ifdef WIN_NT
	CloseHandle(backup);
#else
	close(backup);
#endif
}

void nbackup::fixup_database()
{
	open_database_write();
	Ods::header_page header;
	if (read_file(dbase, &header, sizeof(header)) != sizeof(header))
		b_error::raise("Unexpected end of database file", errno);
	const int backup_state = header.hdr_flags & Ods::hdr_backup_mask;
	if (backup_state != Jrd::nbak_state_stalled)	
		b_error::raise("Database is not in state (%d) to be safely fixed up", backup_state);
	header.hdr_flags = (header.hdr_flags & ~Ods::hdr_backup_mask) | Jrd::nbak_state_normal;
	seek_file(dbase, 0);
	write_file(dbase, &header, sizeof(header));	
	close_database();
}

/*
 *    Print the status, the SQLCODE, and exit.
 *    Also, indicate which operation the error occured on.
 */
void nbackup::pr_error (const ISC_STATUS* status, const char* operation)
{
    printf("[\n");
    printf("PROBLEM ON \"%s\".\n", operation);

    isc_print_status(status);

    printf("SQLCODE:%ld\n", isc_sqlcode(status));

    printf("]\n");

	b_error::raise("Database error");
}

void nbackup::attach_database()
{
	if (username.length() > 255 || password.length() > 255)
		b_error::raise("Username or password is too long");


	Firebird::HalfStaticArray<char, 100> dpbArray;

	char *dpb = dpbArray.getBuffer(
		1 + // isc_dpb_version1
		1 + 1 + // isc_dpb_user_name + length
		username.length() + 
		1 + 1 + // isc_dpb_password + length
		password.length());
	
	*dpb++ = isc_dpb_version1;
	if (!username.isEmpty()) {
		*dpb++ = isc_dpb_user_name;
		*dpb++ = username.length();
		memcpy(dpb, username.c_str(), username.length());
		dpb += username.length();
	}

	if (!password.isEmpty()) {
		*dpb++ = isc_dpb_password;
		*dpb++ = password.length();
		memcpy(dpb, password.c_str(), password.length());
		dpb += password.length();
	}

	if (isc_attach_database(status, 0, database.c_str(), &newdb, dpb - dpbArray.begin(), dpbArray.begin()))
        pr_error(status, "attach database");
}

void nbackup::detach_database()
{
	if (trans) {
		if (isc_rollback_transaction(status, &trans))
			pr_error(status, "rollback transaction");
	}
	if (isc_detach_database(status, &newdb))
		pr_error(status, "detach database");
}

void nbackup::internal_lock_database()
{
    if (isc_start_transaction(status, &trans, 1, &newdb, 0, NULL))
		pr_error(status, "start transaction");
	if (isc_dsql_execute_immediate(status, &newdb, &trans, 0, 
		"ALTER DATABASE BEGIN BACKUP", 1, NULL))
		pr_error(status, "begin backup");
	if (isc_commit_transaction(status, &trans))
		pr_error(status, "begin backup: commit");
}

void nbackup::internal_unlock_database()
{
    if (isc_start_transaction(status, &trans, 1, &newdb, 0, NULL))
		pr_error(status, "start transaction");
	if (isc_dsql_execute_immediate(status, &newdb, &trans, 0, 
		"ALTER DATABASE END BACKUP", 1, NULL))
		pr_error(status, "end backup");
	if (isc_commit_transaction(status, &trans))
		pr_error(status, "end backup: commit");
}

void nbackup::lock_database()
{
	attach_database();
	try {
		internal_lock_database();
	} catch(const std::exception& ex) {
		if (typeid(ex) != typeid(b_error)) {
			fprintf(stderr, "Unexpected error %s: %s\n", typeid(ex).name(), ex.what());
		}
		detach_database();		
		throw;
	}
	detach_database();
}

void nbackup::unlock_database()
{
	attach_database();
	try {
		internal_unlock_database();
	} catch(const std::exception& ex) {
		if (typeid(ex) != typeid(b_error)) {
			fprintf(stderr, "Unexpected error %s: %s\n", typeid(ex).name(), ex.what());
		}
		detach_database();		
		throw;
	}
	detach_database();
}

void nbackup::backup_database(int level, const char* fname)
{
	bool database_locked = false;
	// We set this flag when backup file is in inconsistent state
	bool delete_backup = false;
	ULONG prev_scn = 0;
	char prev_guid[GUID_BUFF_SIZE] = "";
	Ods::pag* page_buff = NULL;
	attach_database();
	try {
		// Look for SCN and GUID of previous-level backup in history table
		if (level) {
		    if (isc_start_transaction(status, &trans, 1, &newdb, 0, NULL))
				pr_error(status, "start transaction");
			char out_sqlda_data[XSQLDA_LENGTH(2)];
			XSQLDA *out_sqlda = (XSQLDA*)out_sqlda_data;
			out_sqlda->version = SQLDA_VERSION1;
			out_sqlda->sqln = 2;
		
			isc_stmt_handle stmt = 0;
			if (isc_dsql_allocate_statement(status, &newdb, &stmt))
				pr_error(status, "allocate statement");
			char str[200];
			sprintf(str, "select rdb$guid, rdb$scn from rdb$backup_history "
				"where rdb$backup_id="
				  "(select max(rdb$backup_id) from rdb$backup_history "
				   "where rdb$backup_level=%d)", level - 1);
			if (isc_dsql_prepare(status, &trans, &stmt, 0, str, 1, NULL))
				pr_error(status, "prepare history query");
			if (isc_dsql_describe(status, &stmt, 1, out_sqlda))
				pr_error(status, "describe history query");
			short guid_null, scn_null;
			out_sqlda->sqlvar[0].sqlind = &guid_null;
			out_sqlda->sqlvar[0].sqldata = prev_guid;
			out_sqlda->sqlvar[1].sqlind = &scn_null;
			out_sqlda->sqlvar[1].sqldata = (char*)&prev_scn;
			if (isc_dsql_execute(status, &trans, &stmt, 1, NULL))
				pr_error(status, "execute history query");
				
			switch (isc_dsql_fetch(status, &stmt, 1, out_sqlda)) {
			case 100: /* No more records available */
				b_error::raise("Cannot find record for database \"%s\" backup level %d "
					"in the backup history", database, level - 1);
			case 0: 
				if (guid_null || scn_null)
					b_error::raise("Internal error. History query returned null SCN or GUID");
				prev_guid[sizeof(prev_guid) - 1] = 0;
				break;
			default:
				pr_error(status, "fetch history query");
			}
			isc_dsql_free_statement(status, &stmt, DSQL_close);
			if (isc_commit_transaction(status, &trans))
				pr_error(status, "commit history query");
		}
	
		// Lock database for backup
		internal_lock_database();
		database_locked = true;	

		time_t _time = time(NULL);
		const struct tm *today = localtime(&_time);
	
		if (fname)
			bakname = fname;
		else {
			// Let's generate nice new filename
			Firebird::PathName begin, fil;
			PathUtils::splitLastComponent(begin, fil, database);
			char temp[MAXPATHLEN];
			SNPRINTF(temp, sizeof(temp), "%s-%d-%04d%02d%02d-%02d%02d.nbk", fil.c_str(), level,
				today->tm_year + 1900, today->tm_mon + 1, today->tm_mday,
				today->tm_hour, today->tm_min);
			temp[sizeof(temp) - 1] = 0;
			bakname = temp;
			printf("%s", bakname.c_str()); // Print out generated filename for script processing
		}

		// Level 0 backup is a full reconstructed database image that can be
		// used directly after fixup. Incremenal backups of other levels are 
		// consisted of header followed by page data. Each page is preceded
		// by 4-byte integer page number
	
		// Actual IO is optimized to get maximum performance 
		// from the IO subsystem while taking as little CPU time as possible
		
		// NOTE: this is still possible to improve performance by implementing
		// version using asynchronous unbuffered IO on NT series of OS.
		// But this task is for another day. 02 Aug 2003, Nickolay Samofatov.
		
		// Create backup file and open database file
		create_backup();
		delete_backup = true;
		
		open_database_scan();
		
		// Read database header
		Ods::header_page header;	
		if (read_file(dbase, &header, sizeof(header)) != sizeof(header))
			b_error::raise("Unexpected end of file when reading header of database file");
		if ((header.hdr_flags & Ods::hdr_backup_mask) != Jrd::nbak_state_stalled)
		{
			b_error::raise("Internal error. Database file is not locked. Flags are %d",
				header.hdr_flags);
		}
	
		page_buff = reinterpret_cast<Ods::pag*>(FB_NEW(*getDefaultMemoryPool()) UCHAR[header.hdr_page_size]);
		
		seek_file(dbase, 0);
		
		if (read_file(dbase, page_buff, header.hdr_page_size) != header.hdr_page_size)
			b_error::raise("Unexpected end of file when reading header of database file (stage 2)");
		
		FB_GUID backup_guid;
		bool guid_found = false;
		const UCHAR* p = reinterpret_cast<Ods::header_page*>(page_buff)->hdr_data;
		while (true) {
			switch (*p) {
			case Ods::HDR_backup_guid:
				if (p[1] != sizeof(FB_GUID))
					break;
				memcpy(&backup_guid, p + 2, sizeof(FB_GUID));
				guid_found = true;
				break;
			case Ods::HDR_difference_file:
				p += p[1] + 2;
				continue;
			}
			break;
		}
	
		if (!guid_found)
			b_error::raise("Internal error. Cannot get backup guid clumplet");
	
	
		// Write data to backup file
		ULONG backup_scn = header.hdr_header.pag_scn - 1;
		if (level) {
			inc_header bh;
			memcpy(bh.signature, backup_signature, sizeof(backup_signature));
			bh.version = 1;
			bh.level = level;
			bh.backup_guid = backup_guid;
			StringToGuid(&bh.prev_guid, prev_guid);
			bh.page_size = header.hdr_page_size;
			bh.backup_scn = backup_scn;
			bh.prev_scn = prev_scn;
			write_file(backup, &bh, sizeof(bh));
		}
	
		ULONG curPage = 0;
		while (true) {
			if (curPage && page_buff->pag_scn > backup_scn)
				b_error::raise("Internal error. Database page %d had been changed during backup"
							   " (page SCN=%d, backup SCN=%d)", curPage,
							   page_buff->pag_scn, backup_scn);
			if (level) {
				if (page_buff->pag_scn > prev_scn) {
					write_file(backup, &curPage, sizeof(curPage));
					write_file(backup, page_buff, header.hdr_page_size);
				}
			}
			else
				write_file(backup, page_buff, header.hdr_page_size);
			const size_t bytesDone = read_file(dbase, page_buff, header.hdr_page_size);
			if (bytesDone == 0)
				break;
			if (bytesDone != header.hdr_page_size)
				b_error::raise("Database file size is not a multiply of page size");
			curPage++;
		}		
		close_database();
		close_backup();
		
		delete_backup = false; // Backup file is consistent. No need to delete it
	
		// Write about successful backup to backup history table
	    if (isc_start_transaction(status, &trans, 1, &newdb, 0, NULL))
			pr_error(status, "start transaction");
		char in_sqlda_data[XSQLDA_LENGTH(4)];
		XSQLDA *in_sqlda = (XSQLDA *)in_sqlda_data;
		in_sqlda->version = SQLDA_VERSION1;
		in_sqlda->sqln = 4;
		isc_stmt_handle stmt = 0;
		if (isc_dsql_allocate_statement(status, &newdb, &stmt))
			pr_error(status, "allocate statement");
		if (isc_dsql_prepare(status, &trans, &stmt, 0, 
			"insert into rdb$backup_history(rdb$backup_id, rdb$timestamp,"
			  "rdb$backup_level, rdb$guid, rdb$scn, rdb$file_name)"
			"values(gen_id(rdb$backup_history, 1), 'now', ?, ?, ?, ?)",
			1, NULL))
		{
			pr_error(status, "prepare history insert");
		}
		if (isc_dsql_describe_bind(status, &stmt, 1, in_sqlda))
			pr_error(status, "bind history insert");
		short null_flag = 0;
		in_sqlda->sqlvar[0].sqldata = (char*)&level;
		in_sqlda->sqlvar[0].sqlind = &null_flag;
		char temp[GUID_BUFF_SIZE];
		GuidToString(temp, &backup_guid);
		in_sqlda->sqlvar[1].sqldata = temp;
		in_sqlda->sqlvar[1].sqlind = &null_flag;
		in_sqlda->sqlvar[2].sqldata = (char*)&backup_scn;
		in_sqlda->sqlvar[2].sqlind = &null_flag;
		// Pad filename with spaces before storing
		char buff[256]; // RDB$FILE_NAME has length of 253
		size_t len = bakname.length();
		if (len > 253)
			len = 253;
		buff[0] = len;
		buff[1] = 0;
		memcpy(buff + 2, bakname.c_str(), len);
		in_sqlda->sqlvar[3].sqldata = buff;
		in_sqlda->sqlvar[3].sqlind = &null_flag;
		if (isc_dsql_execute(status, &trans, &stmt, 1, in_sqlda))
			pr_error(status, "execute history insert");
		if (isc_commit_transaction(status, &trans))
			pr_error(status, "commit history insert");
		
	} catch (const std::exception& ex) {
		if (typeid(ex) != typeid(b_error)) {
			fprintf(stderr, "Unexpected error %s: %s\n", typeid(ex).name(), ex.what());
		}
		delete[] reinterpret_cast<UCHAR*>(page_buff);
		if (delete_backup)
			unlink(bakname.c_str());
		if (trans) {
			if (isc_rollback_transaction(status, &trans))
				pr_error(status, "rollback transaction");
		}
		if (database_locked)
			internal_unlock_database();
		detach_database();
		throw;
	}
	delete[] reinterpret_cast<UCHAR*>(page_buff);
	internal_unlock_database();
	detach_database();
}

void nbackup::restore_database(int filecount, const char* const* files)
{
	// We set this flag when database file is in inconsistent state
	bool delete_database = false; 
#ifndef WIN_NT
	create_database();
	delete_database = true;
#endif
	UCHAR *page_buffer = NULL;
	try {
		int curLevel = 0;
		FB_GUID prev_guid;
		while (true) {
			if (!filecount) {
				while (true) {
					printf("Enter name of the backup file of level %d "
						   "(\".\" - do not restore further): \n", curLevel);
					char temp[256];
					scanf("%255s", temp);
					bakname = temp;
					if (bakname == ".") 
					{
						close_database();
						if (!curLevel) {
							unlink(dbname.c_str());
							b_error::raise("Level 0 backup is not restored");
						}
						fixup_database();
						delete[] page_buffer;
						return;
					}
					try {
						open_backup_scan();
						break;
					} catch (const std::exception& e) {
						printf("%s\n", e.what());
					}
#ifdef WIN_NT
					if (curLevel)
						close_backup();
#endif
				}
			}
			else {
				if (curLevel >= filecount) {
					close_database();
					fixup_database();
					delete[] page_buffer;
					return;
				}
				else {
					bakname = files[curLevel];
#ifdef WIN_NT
					if (curLevel)
#endif
						open_backup_scan();
				}
			}
		
			if (curLevel) {
				inc_header bakheader;
				if (read_file(backup, &bakheader, sizeof(bakheader)) != sizeof(bakheader))
					b_error::raise("Unexpected end of file when reading header of backup file: %s", bakname.c_str());					
				if (memcmp(bakheader.signature, backup_signature, sizeof(backup_signature)) != 0)				
					b_error::raise("Invalid incremental backup file: %s", bakname.c_str());
				if (bakheader.version != 1)
					b_error::raise("Unsupported version %d of incremental backup file: %s", bakheader.version, bakname.c_str());
				if (bakheader.level != curLevel)
					b_error::raise("Invalid level %d of incremental backup file: %s, expected %d", 
						bakheader.level, bakname.c_str(), curLevel);
				// We may also add SCN check, but GUID check covers this case too
				if (memcmp(&bakheader.prev_guid, &prev_guid, sizeof(FB_GUID)) != 0)
				{
					b_error::raise(
						"Wrong order of backup files or "
						"invalid incremental backup file detected, file: %s", bakname.c_str());
				}
				delete_database = true;
				prev_guid = bakheader.backup_guid;
				while (true) {
					ULONG pageNum;
					const size_t bytesDone = read_file(backup, &pageNum, sizeof(pageNum));
					if (bytesDone == 0)
						break;					
					if (bytesDone != sizeof(pageNum) || 
						read_file(backup, page_buffer, bakheader.page_size) != bakheader.page_size) 
					{
						b_error::raise("Unexpected end of backup file: %s", bakname.c_str());
					}
					seek_file(dbase, ((SINT64)pageNum)*bakheader.page_size);
					write_file(dbase, page_buffer, bakheader.page_size);
				}
				delete_database = false;
			}
			else {
#ifdef WIN_NT
				if (!CopyFile(bakname.c_str(), dbname.c_str(), FALSE)) {
					b_error::raise("Error (%d) creating database file: %s via copying from: %s", 
						GetLastError(), dbname.c_str(), bakname.c_str());
				}
				delete_database = true; // database is possibly broken
				open_database_write();
#else
				// Use relatively small buffer to make use of prefetch and lazy flush
				char buffer[65536]; 
				while (true) {
					const size_t bytesRead = read_file(backup, buffer, sizeof(buffer));
					if (bytesRead == 0)
						break;
					write_file(dbase, buffer, bytesRead);
				}
				seek_file(dbase, 0);
#endif				
				// Read database header
				Ods::header_page header;
				if (read_file(dbase, &header, sizeof(header)) != sizeof(header))
					b_error::raise("Unexpected end of file when reading restored database header");
				page_buffer = FB_NEW(*getDefaultMemoryPool()) UCHAR[header.hdr_page_size];
		
				seek_file(dbase, 0);
		
				if (read_file(dbase, page_buffer, header.hdr_page_size) != header.hdr_page_size)
					b_error::raise("Unexpected end of file when reading header of restored database file (stage 2)");
				
				bool guid_found = false;
				const UCHAR* p = reinterpret_cast<Ods::header_page*>(page_buffer)->hdr_data;
				while (true) {
					switch (*p) {
					case Ods::HDR_backup_guid:
						if (p[1] != sizeof(FB_GUID))
							break;
						memcpy(&prev_guid, p + 2, sizeof(FB_GUID));
						guid_found = true;
						break;
					case Ods::HDR_difference_file:
						p += p[1] + 2;
						continue;
					}
					break;
				}
				if (!guid_found)
					b_error::raise("Cannot get backup guid clumplet from L0 backup");
				// We are likely to have normal database here
				delete_database = false;
			}
			close_backup();
			curLevel++;
		}
	} catch(const std::exception& ex) {
		if (typeid(ex) != typeid(b_error)) {
			fprintf(stderr, "Unexpected error %s: %s\n", typeid(ex).name(), ex.what());
		}
		delete[] page_buffer;
		if (delete_database)
			unlink(dbname.c_str());
		throw;
	}
}

int main( int argc, char *argv[] )
{
#if defined DEV_BUILD && !defined WIN_NT
	fprintf(stderr, "Using %d-bit UNIX IO\n", sizeof(off_t) * 8);
#endif
	bool matched = false;
	// Do not constify. GCC 3.4.0 chokes on minus below in this case
	char **end = argv + argc, **argp;

	const char *username = NULL, *password = NULL;
	
	try {
		// Read global command line parameters
		for (argp = argv + 1; argp < end; argp++) {
			// Ignore unrecognized options for the moment
			if (**argp != '-')
				continue;

			switch (UPPER((*argp)[1])) {
			case 'U':
				if (++argp >= end)
					missing_parameter_for_switch(argp[-1]);

				username = *argp;
				break;

			case 'P':
				if (++argp >= end)
					missing_parameter_for_switch(argp[-1]);

				password = *argp;
				break;
			default:
				break;
			}
		}
	
		// Read global command line parameters
		for (argp = argv + 1; argp < end; argp++) {
			// We must recognize all parameters here
			if (**argp != '-') {
				fprintf(stderr, "ERROR: Unrecognized parameter %s.\n\n", *argp);
				usage();
			}

			switch (UPPER((*argp)[1])) {
			case 'U':
			case 'P':
				// Skip parameters recognized earlier
				argp++;
				break;

			case 'F':
				if (++argp >= end)
					missing_parameter_for_switch(argp[-1]);

				nbackup(*argp, username, password).fixup_database();
				matched = true;
				break;

			case 'L':
				if (++argp >= end)
					missing_parameter_for_switch(argp[-1]);

				nbackup(*argp, username, password).lock_database();
				matched = true;
				break;

			case 'N':
				if (++argp >= end)
					missing_parameter_for_switch(argp[-1]);

				nbackup(*argp, username, password).unlock_database();
				matched = true;
				break;

			case 'B': {
					if (++argp >= end)
						missing_parameter_for_switch(argp[-1]);

					int level = atoi(*argp);

					if (++argp >= end)
						missing_parameter_for_switch(argp[-1]);

					nbackup(*argp, username, password).backup_database(level, argp + 1 >= end ? NULL : argp[1]);
					matched = true;
				}
				break;

			case 'R':
				if (++argp >= end)
					missing_parameter_for_switch(argp[-1]);

				nbackup(*argp, username, password).restore_database(end - argp - 1, argp + 1);
				matched = true;
				break;

			default:
				fprintf(stderr, "ERROR: Unknown switch %s.\n\n", *argp);
				usage();
				break;
			}
		}
	} catch (const std::exception&) {
		// It must have been printed out. No need to repeat the task
		return EXIT_ERROR;
	}
	
    if (!matched)
	    usage();
	return EXIT_OK;
}

