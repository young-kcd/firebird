/*
 *	PROGRAM:	Firebird Trace Services
 *	MODULE:		TraceConfigStorage.cpp
 *	DESCRIPTION:	Trace API shared configurations storage
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
 *  The Original Code was created by Khorsun Vladyslav
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2008 Khorsun Vladyslav <hvlad@users.sourceforge.net>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 */

#include "firebird.h"
#include "../../common/classes/TempFile.h"
#include "../../common/StatusArg.h"
#include "../../jrd/common.h"
#include "../../jrd/err_proto.h"
#include "../../jrd/isc_proto.h"
#include "../../jrd/isc_s_proto.h"
#include "../../jrd/jrd.h"
#include "../../jrd/os/path_utils.h"
#include "../../jrd/os/config_root.h"
#include "../../jrd/trace/TraceConfigStorage.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_IO_H
#include <io.h>
#endif
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>

#ifndef O_BINARY
#define O_BINARY 0
#endif

using namespace Firebird;

namespace Jrd {

ConfigStorage::ConfigStorage()
{
	m_base = NULL;
	m_cfg_file = -1;
	m_dirty = false;
	
	PathName filename;
	filename.printf(TRACE_FILE); // TODO: it must be per engine instance

	ISC_STATUS_ARRAY status;
	ISC_map_file(status, filename.c_str(), initShMem, this, sizeof(ShMemHeader), &m_handle);
	if (!m_base)
	{
		iscLogStatus("Cannot initialize the shared memory region", status);
		status_exception::raise(status);
	}

	fb_assert(m_base->version == 1);
	
	StorageGuard guard(this);
	checkFile();
}

ConfigStorage::~ConfigStorage()
{
	::close(m_cfg_file);
	m_cfg_file = -1;
#ifdef WIN_NT
	unlink((char*) &m_base->cfg_file_name[0]);
#endif
	
	ISC_STATUS_ARRAY status;
	ISC_unmap_file(status, &m_handle);
}

void ConfigStorage::checkMutex(const TEXT* string, int state)
{
	if (state)
	{
		TEXT msg[BUFFER_TINY];

		sprintf(msg, "TRACE: mutex %s error, status = %d", string, state);
		gds__log(msg);

		fprintf(stderr, "%s\n", msg);
		exit(FINI_ERROR);
	}
}

void ConfigStorage::initShMem(void* arg, SH_MEM_T* shmemData, bool initialize)
{
	ConfigStorage* const storage = (ConfigStorage*) arg;
	fb_assert(storage);

#ifdef WIN_NT
	checkMutex("init", ISC_mutex_init(&storage->m_mutex, shmemData->sh_mem_name));
#endif

	ShMemHeader* const header = (ShMemHeader*) shmemData->sh_mem_address;
	storage->m_base = header;

	// Initialize the shared data header
	if (initialize)
	{
		header->version = 1;
		header->change_number = 0;
		header->session_number = 1;
#ifndef WIN_NT
		checkMutex("init", ISC_mutex_init(&header->mutex));
#endif
	}
}

void ConfigStorage::checkFile()
{
	if (m_cfg_file >= 0)
		return;

	char* cfg_file_name = &m_base->cfg_file_name[0];

	if (!(*cfg_file_name))
	{
		PathName filename = TempFile::create("fb_trace_");
		filename.copyTo(cfg_file_name, sizeof(m_base->cfg_file_name));
		m_cfg_file = ::open(cfg_file_name, O_CREAT | O_RDWR | O_BINARY, S_IREAD | S_IWRITE);
	}
	else 
	{
		m_cfg_file = ::open(cfg_file_name, O_RDWR | O_BINARY);
	}

	if (m_cfg_file < 0) {
		ERR_post(Arg::Gds(isc_io_error) << Arg::Str("open") << Arg::Str(cfg_file_name) <<
				 Arg::Gds(isc_io_open_err) << SYS_ERR(errno));
	}

#ifndef WIN_NT
	unlink(cfg_file_name);
#endif

	// put default (audit) trace file contents into storage
	if (m_base->change_number == 0)
	{
		PathName configFileName(Config::getAuditTraceConfigFile());

		if (configFileName.empty())
			return;

		if (PathUtils::isRelative(configFileName)) {
			configFileName.insert(0, Config::getRootDirectory());
		}

		FILE* cfgFile = fopen(configFileName.c_str(), "rb");
		if (!cfgFile) 
		{
			Arg::Gds temp(isc_io_error); 
			temp << Arg::Str("fopen") << Arg::Str(configFileName.c_str());
			temp << Arg::Gds(isc_io_open_err) << SYS_ERR(errno);

			iscLogStatus("Cannot open audit configuration file", temp.value());
			return;
		}

		TraceSession session(*getDefaultMemoryPool());

		fseek(cfgFile, 0, SEEK_END);
		const long len = ftell(cfgFile);
		if (len)
		{
			fseek(cfgFile, 0, SEEK_SET);
			char* p = session.ses_config.getBuffer(len+1);
			if (fread(p, 1, len, cfgFile) != len)
			{
				Arg::Gds temp(isc_io_error); 
				temp << Arg::Str("fopen") << Arg::Str(configFileName.c_str());
				temp << Arg::Gds(isc_io_open_err) << SYS_ERR(errno);

				iscLogStatus("Cannot read audit configuration file", temp.value());
			}
			p[len] = 0;
		}
		else {
			gds__log("Audit configuration file is empty");
		}
		fclose(cfgFile);

		session.ses_user = SYSDBA_USER_NAME;
		session.ses_name = "Firebird Audit"; 
		session.ses_flags = trs_admin | trs_system;

		addSession(session);
	}
}


void ConfigStorage::acquire()
{
#ifdef WIN_NT
	checkMutex("lock", ISC_mutex_lock(&m_mutex));
#else
	checkMutex("lock", ISC_mutex_lock(&m_base->mutex));
#endif
}

void ConfigStorage::release()
{
	checkDirty();

#ifdef WIN_NT
	checkMutex("unlock", ISC_mutex_unlock(&m_mutex));
#else
	checkMutex("unlock", ISC_mutex_unlock(&m_base->mutex));
#endif
}

void ConfigStorage::addSession(TraceSession& session)
{
	setDirty();
	session.ses_id = m_base->session_number++;
	session.ses_flags |= trs_active;
	time(&session.ses_start);

	const long pos1 = lseek(m_cfg_file, 0, SEEK_END);
	if (pos1 < 0) 
	{
		const char* fn = &m_base->cfg_file_name[0];
		ERR_post(Arg::Gds(isc_io_error) << Arg::Str("lseek") << Arg::Str(fn) <<
			Arg::Gds(isc_io_read_err) << SYS_ERR(errno));
	}

	putItem(tagID, sizeof(session.ses_id), &session.ses_id);
	if (!session.ses_name.empty()) {
		putItem(tagName, session.ses_name.length(), session.ses_name.c_str());
	}
	putItem(tagUserName, session.ses_user.length(), session.ses_user.c_str());
	putItem(tagFlags, sizeof(session.ses_flags), &session.ses_flags);
	putItem(tagConfig, session.ses_config.length(), session.ses_config.c_str());
	putItem(tagStartTS, sizeof(session.ses_start), &session.ses_start);
	if (!session.ses_logfile.empty()) {
		putItem(tagLogFile, session.ses_logfile.length(), session.ses_logfile.c_str());
	}
	putItem(tagEnd, 0, NULL);

	// const long pos2 = lseek(m_cfg_file, 0, SEEK_END);
	// m_base->used_space += pos2 - pos1;
}

bool ConfigStorage::getNextSession(TraceSession& session)
{
	ITEM tag = tagID;
	size_t len;
	session.clear();

	while (true)
	{
		if (!getItemLength(tag, len))
			return false;

		if (tag == tagEnd)
		{
			if (session.ses_id != 0)
				return true;

			continue;
		}

		void* p = NULL;

		switch (tag)
		{
			case tagID:
				fb_assert(len == sizeof(session.ses_id));
				p = &session.ses_id;
				break;

			case tagName:
				if (session.ses_id)
					p = session.ses_name.getBuffer(len);
				break;

			case tagUserName:
				if (session.ses_id)
					p = session.ses_user.getBuffer(len);
				break;

			case tagFlags:
				fb_assert(len == sizeof(session.ses_flags));
				if (session.ses_id)
					p = &session.ses_flags;
				break;

			case tagConfig:
				if (session.ses_id)
					p = session.ses_config.getBuffer(len);
				break;

			case tagStartTS:
				fb_assert(len == sizeof(session.ses_start));
				if (session.ses_id)
					p = &session.ses_start;
				break;

			case tagLogFile:
				if (session.ses_id)
					p = session.ses_logfile.getBuffer(len);
				break;

			default:
				fb_assert(false);
		}

		bool err;
		if (p)
			err = (::read(m_cfg_file, p, len) != len);
		else
			err = (lseek(m_cfg_file, len, SEEK_CUR) < 0);

		if (err) 
		{
			const char* fn = &m_base->cfg_file_name[0];
			ERR_post(Arg::Gds(isc_io_error) << Arg::Str("read") << Arg::Str(fn) <<
				Arg::Gds(isc_io_read_err) << SYS_ERR(errno));
		}
	}

	return true;
}

void ConfigStorage::removeSession(ULONG id)
{
	ITEM tag = tagID;
	size_t len;

	restart();
	while (true)
	{
		if (!getItemLength(tag, len))
			return;

		bool err;
		if (tag == tagID)
		{
			ULONG currID;
			fb_assert(len == sizeof(currID));
			err = (::read(m_cfg_file, &currID, len) != len);
			if (!err && currID == id)
			{
				setDirty();

				currID = 0;
				lseek(m_cfg_file, -len, SEEK_CUR);
				write(m_cfg_file, &currID, len);
				break;
			}
		}
		else
		{
			err = (lseek(m_cfg_file, len, SEEK_CUR) < 0);
		}

		if (err)
		{
			const char* fn = &m_base->cfg_file_name[0];
			ERR_post(Arg::Gds(isc_io_error) << Arg::Str("read") << Arg::Str(fn) <<
				Arg::Gds(isc_io_read_err) << SYS_ERR(errno));
		}
}
}

void ConfigStorage::restart()
{
	checkDirty();
	if (lseek(m_cfg_file, 0, SEEK_SET) < 0)
	{
		const char* fn = &m_base->cfg_file_name[0];
		ERR_post(Arg::Gds(isc_io_error) << Arg::Str("lseek") << Arg::Str(fn) <<
			Arg::Gds(isc_io_read_err) << SYS_ERR(errno));
	}
}


void ConfigStorage::updateSession(Firebird::TraceSession& session)
{
	restart();

	ITEM tag;
	size_t len;
	ULONG currID = 0;

	while (true)
	{
		if (!getItemLength(tag, len))
			return;

		void* p = NULL;
		switch (tag)
		{
			case tagID:
				fb_assert(len == sizeof(currID));
				read(m_cfg_file, &currID, len);
				continue;
				break;

			case tagFlags:
				fb_assert(len == sizeof(session.ses_flags));
				if (currID == session.ses_id)
					p = &session.ses_flags;
				break;

			case tagEnd:
				if (currID == session.ses_id)
					return;
				len = 0;
				break;
		}

		if (p) 
		{
			setDirty();
			write(m_cfg_file, p, len);
		}
		else if (len)
		{
			lseek(m_cfg_file, len, SEEK_CUR);
		}
	}
}


void ConfigStorage::putItem(ITEM tag, size_t len, const void* data)
{
	const char tag_data = (char) tag;
	write(m_cfg_file, &tag_data, sizeof(tag_data));

	if (tag == tagEnd)
		return;

	write(m_cfg_file, &len, sizeof(len));
	if (len) {
		write(m_cfg_file, data, len);
	}
}

bool ConfigStorage::getItemLength(ITEM& tag, size_t& len)
{
	char data;
	const int cnt_read = read(m_cfg_file, &data, sizeof(data));

	if (cnt_read == 0)
		return false;

	if (cnt_read < 0)
		system_call_failed::raise("read", errno);

	tag = (ITEM) data;

	if (tag == tagEnd)
		len = 0;
	else
		read(m_cfg_file, &len, sizeof(len));

	return true;
}

} // namespace Jrd 
