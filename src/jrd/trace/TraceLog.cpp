/*
 *	PROGRAM:	Firebird Trace Services
 *	MODULE:		TraceLog.cpp
 *	DESCRIPTION:	Trace API shared log file
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
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_IO_H
#include <io.h>
#endif
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../../common/StatusArg.h"
#include "../../common/classes/TempFile.h"
#include "../../jrd/common.h"
#include "../../jrd/isc_proto.h"
#include "../../jrd/isc_s_proto.h"
#include "../../jrd/os/path_utils.h"
#include "../../jrd/trace/TraceLog.h"

using namespace Firebird;

namespace Jrd {

/// TraceLogImpl 

const size_t MAX_LOG_FILE_SIZE = 1024 * 1024;
const unsigned int MAX_FILE_NUM = (unsigned int) -1;

TraceLogImpl::TraceLogImpl(MemoryPool &pool, const PathName &fileName, bool reader) :
	m_baseFileName(pool)
{
	m_base = 0;
	m_fileNum = 0;
	m_fileHandle = -1;
	m_reader = reader;

	ISC_STATUS_ARRAY status;
	ISC_map_file(status, fileName.c_str(), initShMem, this, sizeof(ShMemHeader), &m_handle);
	if (!m_base)
	{
		iscLogStatus("Cannot initialize the shared memory region", status);
		Firebird::status_exception::raise(status);
	}

	PathUtils::concatPath(m_baseFileName, TempFile::getTempPath(), fileName);
	
	TraceLogGuard guard(this);
	if (m_reader) 
		m_fileNum = 0;
	else
		m_fileNum = m_base->writeFileNum;

	m_fileHandle = openFile(m_fileNum);
}

TraceLogImpl::~TraceLogImpl()
{
	::close(m_fileHandle);

	if (m_reader) 
	{
		// indicate reader is gone
		m_base->readFileNum = MAX_FILE_NUM;

		for (; m_fileNum <= m_base->writeFileNum; m_fileNum++)
			removeFile(m_fileNum);
	}
	else if (m_fileNum < m_base->readFileNum) {
		removeFile(m_fileNum);
	}

	const bool readerDone = (m_base->readFileNum == MAX_FILE_NUM);

	ISC_STATUS_ARRAY status;
	ISC_unmap_file(status, &m_handle);

	if (m_reader || readerDone) {
//		unlink(m_baseFileName.c_str());
	}
}

int TraceLogImpl::openFile(int fileNum)
{
	PathName fileName;
	fileName.printf("%s.%07ld", m_baseFileName.c_str(), fileNum);

	const int oflag = O_CREAT | O_RDWR
#ifdef WIN_NT
		| O_BINARY | O_SEQUENTIAL | _O_SHORT_LIVED
#endif
		;
	const int pflag = S_IREAD | S_IWRITE;

	int file = open(fileName.c_str(), oflag, pflag);

	return file;
}

int TraceLogImpl::removeFile(int fileNum)
{
	PathName fileName;
	fileName.printf("%s.%07ld", m_baseFileName.c_str(), fileNum);
//	return unlink(fileName.c_str());
	return 0;
}

size_t TraceLogImpl::read(void* buf, size_t size)
{
	fb_assert(m_reader);

	char* p = (char*) buf;
	unsigned int readLeft = size;
	while (readLeft)
	{
		const int reads = ::read(m_fileHandle, p, readLeft);

		if (reads == 0)
		{
			const off_t len = lseek(m_fileHandle, 0, SEEK_CUR);
			if (len >= MAX_LOG_FILE_SIZE)
			{
				// this file was reads completely, go to next one
				::close(m_fileHandle);
				removeFile(m_fileNum);

				fb_assert(m_base->readFileNum == m_fileNum);
				m_fileNum = ++m_base->readFileNum;
				m_fileHandle = openFile(m_fileNum);
			}
			else
			{
				// nothing to read, return what we have
				break;
			}
		}
		else if (reads > 0)
		{
			p += reads;
			readLeft -= reads;
		}
		else
		{
			// io error
			system_call_failed::raise("read", errno);
			break; 
		}
	}

	return (size - readLeft);
}

size_t TraceLogImpl::write(const void* buf, size_t size)
{
	fb_assert(!m_reader);

	// if reader already gone, don't write anything
	if (m_base->readFileNum == MAX_FILE_NUM)
		return size;

	TraceLogGuard guard(this);
	
	const char* p = (const char*) buf;
	unsigned int writeLeft = size;
	while (writeLeft)
	{
		const long len = lseek(m_fileHandle, 0, SEEK_END);
		const unsigned int toWrite = MIN(writeLeft, MAX_LOG_FILE_SIZE - len);
		if (!toWrite)
		{
			// While this instance of writer was idle, new log file was created.
			// More, if current file was already readed by reader, we must delete it. 
			::close(m_fileHandle);
			if (m_fileNum < m_base->readFileNum) {
				removeFile(m_fileNum);
			}
			m_fileNum = m_base->writeFileNum;
			m_fileHandle = openFile(m_fileNum);
			continue;
		}

		const int written = ::write(m_fileHandle, p, toWrite);

		p += toWrite;
		writeLeft -= toWrite;
		if (writeLeft || (len + toWrite == MAX_LOG_FILE_SIZE))
		{
			::close(m_fileHandle);
			m_fileNum = ++m_base->writeFileNum;
			m_fileHandle = openFile(m_fileNum);
		}
	}

	return size - writeLeft;
}

size_t TraceLogImpl::getApproxLogSize() const
{
	return (m_base->writeFileNum - m_base->readFileNum + 1) * 
			(MAX_LOG_FILE_SIZE / (1024 * 1024));
}

void TraceLogImpl::checkMutex(const TEXT* string, int state)
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

void TraceLogImpl::initShMem(void* arg, SH_MEM_T* shmemData, bool initialize)
{
	TraceLogImpl* log = (TraceLogImpl*) arg;

#ifdef WIN_NT
	checkMutex("init", ISC_mutex_init(&log->m_mutex, shmemData->sh_mem_name));
#endif

	ShMemHeader* const header = (ShMemHeader*) shmemData->sh_mem_address;
	log->m_base = header;

	if (initialize)
	{
		header->readFileNum = 0;
		header->writeFileNum = 0;
#ifndef WIN_NT
		checkMutex("init", ISC_mutex_init(&header->mutex));
#endif
	}
}

void TraceLogImpl::lock()
{
#ifdef WIN_NT
	checkMutex("lock", ISC_mutex_lock(&m_mutex));
#else
	checkMutex("lock", ISC_mutex_lock(&m_base->mutex));
#endif
}

void TraceLogImpl::unlock()
{
#ifdef WIN_NT
	checkMutex("unlock", ISC_mutex_unlock(&m_mutex));
#else
	checkMutex("unlock", ISC_mutex_unlock(&m_base->mutex));
#endif
}

} // namespace Jrd
