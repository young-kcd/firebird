/*
 *	PROGRAM:	Firebird Trace Services
 *	MODULE:		TraceLog.h
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

#ifndef TRACE_LOG
#define TRACE_LOG

#include "../../common/classes/fb_string.h"
#include "../../jrd/ntrace.h"
#include "../../jrd/isc.h"

namespace Jrd {

class TraceLogImpl : public TraceLogWriter
{
public:
	TraceLogImpl(Firebird::MemoryPool &pool, const Firebird::PathName &fileName, bool reader);
	virtual ~TraceLogImpl();

	size_t read(void* buf, size_t size);
	virtual size_t write(const void* buf, size_t size);
	virtual void release()
	{ delete this; }

	// returns approximate log size in MB
	size_t getApproxLogSize() const;

private:
	static void checkMutex(const TEXT*, int);
	static void initShMem(void*, SH_MEM_T*, bool);

	void lock();
	void unlock();

	int openFile(int fileNum);
	int removeFile(int fileNum);

	struct ShMemHeader
	{
		volatile unsigned int readFileNum;
		volatile unsigned int writeFileNum;
#ifndef WIN_NT
		struct mtx mutex;
#endif
	};

	SH_MEM_T m_handle;
	volatile ShMemHeader *m_base;
#ifdef WIN_NT
	struct mtx m_mutex;
#endif

	Firebird::PathName m_baseFileName;
	unsigned int m_fileNum;
	int m_fileHandle;
	bool m_reader;

friend class TraceLogGuard;
};


class TraceLogGuard
{
public:
	TraceLogGuard(TraceLogImpl* log) : m_log(log)
	{
		m_log->lock();
	}

	~TraceLogGuard() 
	{
		m_log->unlock();
	}

private:
	TraceLogImpl *m_log;
};


} // namespace Jrd
#endif // TRACE_LOG
