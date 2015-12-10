/*
 *	PROGRAM:		JRD access method
 *	MODULE:			CryptoManager.h
 *	DESCRIPTION:	Database encryption
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
 *  Copyright (c) 2012 Alex Peshkov <peshkoff at mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 */

#ifndef JRD_CRYPTO_MANAGER
#define JRD_CRYPTO_MANAGER

#include "../common/classes/alloc.h"
#include "../common/classes/fb_atomic.h"
#include "../common/classes/SyncObject.h"
#include "../common/classes/fb_string.h"
#include "../common/classes/objects_array.h"
#include "../common/classes/stack.h"
#include "../common/classes/condition.h"
#include "../common/ThreadStart.h"
#include "../jrd/ods.h"
#include "../jrd/status.h"
#include "firebird/Interface.h"

// forward

class Config;

namespace Ods {
	struct pag;
}

namespace Jrd {

class Database;
class Attachment;
class jrd_file;
class BufferDesc;
class thread_db;
class Lock;
class PageSpace;

class BarSync
{
public:
	class IBar
	{
	public:
		virtual void doOnTakenWriteSync(Jrd::thread_db* tdbb) = 0;
		virtual void doOnAst(Jrd::thread_db* tdbb) = 0;
	};

	BarSync(IBar* i)
		: callback(i), counter(0), lockMode(0), flagWriteLock(false)
	{ }

	class IoGuard
	{
	public:
		IoGuard(Jrd::thread_db* p_tdbb, BarSync& p_bs)
			: tdbb(p_tdbb), bs(p_bs)
		{
			bs.ioBegin(tdbb);
		}

		~IoGuard()
		{
			bs.ioEnd(tdbb);
		}

	private:
		Jrd::thread_db* tdbb;
		BarSync& bs;
	};

	class LockGuard
	{
	public:
		LockGuard(Jrd::thread_db* p_tdbb, BarSync& p_bs)
			: tdbb(p_tdbb), bs(p_bs), flagLocked(false)
		{ }

		void lock()
		{
			fb_assert(!flagLocked);
			if (!flagLocked)
			{
				bs.lockBegin(tdbb);
				flagLocked = true;
			}
		}

		~LockGuard()
		{
			if (flagLocked)
			{
				bs.lockEnd(tdbb);
			}
		}

	private:
		Jrd::thread_db* tdbb;
		BarSync& bs;
		bool flagLocked;
	};

	void ioBegin(Jrd::thread_db* tdbb)
	{
		Firebird::MutexLockGuard g(mutex, FB_FUNCTION);

		if (counter < 0)
		{
			if ((counter % BIG_VALUE == 0) && (!flagWriteLock))
			{
				if (lockMode)
				{
					// Someone is waiting for write lock
					lockCond.notifyOne();
					barCond.wait(mutex);
				}
				else
				{
					// Ast done
					callWriteLockHandler(tdbb);
					counter = 0;
				}
			}
			else if (!(flagWriteLock && (thread == getThreadId())))
				barCond.wait(mutex);
		}
		++counter;
	}

	void ioEnd(Jrd::thread_db* tdbb)
	{
		Firebird::MutexLockGuard g(mutex, FB_FUNCTION);

		if (--counter < 0 && counter % BIG_VALUE == 0)
		{
			if (!(flagWriteLock && (thread == getThreadId())))
			{
				if (lockMode)
					lockCond.notifyOne();
				else
				{
					callWriteLockHandler(tdbb);
					finishWriteLock();
				}
			}
		}
	}

	void ast(Jrd::thread_db* tdbb)
	{
		Firebird::MutexLockGuard g(mutex, FB_FUNCTION);
		if (counter >= 0)
		{
			counter -= BIG_VALUE;
		}
		callback->doOnAst(tdbb);
	}

	void lockBegin(Jrd::thread_db* tdbb)
	{
		Firebird::MutexLockGuard g(mutex, FB_FUNCTION);

		if ((counter -= BIG_VALUE) != -BIG_VALUE)
		{
			++lockMode;
			try
			{
				lockCond.wait(mutex);
			}
			catch(const Firebird::Exception&)
			{
				--lockMode;
				throw;
			}
			--lockMode;
		}

		thread = getThreadId();
		flagWriteLock = true;
	}

	void lockEnd(Jrd::thread_db* tdbb)
	{
		Firebird::MutexLockGuard g(mutex, FB_FUNCTION);

		flagWriteLock = false;
		finishWriteLock();
	}

private:
	void callWriteLockHandler(Jrd::thread_db* tdbb)
	{
		thread = getThreadId();
		flagWriteLock = true;
		callback->doOnTakenWriteSync(tdbb);
		flagWriteLock = false;
	}

	void finishWriteLock()
	{
		if ((counter += BIG_VALUE) == 0)
			barCond.notifyAll();
		else
			lockCond.notifyOne();
	}

	Firebird::Condition barCond, lockCond;
	Firebird::Mutex mutex;
	IBar* callback;
	ThreadId thread;
	int counter;
	int lockMode;
	bool flagWriteLock;

	static const int BIG_VALUE = 1000000;
};

class CryptoManager FB_FINAL : public Firebird::PermanentStorage, public BarSync::IBar
{
public:
	explicit CryptoManager(thread_db* tdbb);
	~CryptoManager();

	void shutdown(thread_db* tdbb);

	void prepareChangeCryptState(thread_db* tdbb, const Firebird::MetaName& plugName);
	void changeCryptState(thread_db* tdbb, const Firebird::string& plugName);
	void attach(thread_db* tdbb, Attachment* att);
	void detach(thread_db* tdbb, Attachment* att);

	void startCryptThread(thread_db* tdbb);
	void terminateCryptThread(thread_db* tdbb);

	bool read(thread_db* tdbb, FbStatusVector* sv, jrd_file* file, BufferDesc* bdb,
		Ods::pag* page, bool noShadows = true, PageSpace* pageSpace = NULL);
	bool write(thread_db* tdbb, FbStatusVector* sv, jrd_file* file, BufferDesc* bdb,
		Ods::pag* page);

	void cryptThread();

	ULONG getCurrentPage();

private:
	class Buffer
	{
	public:
		operator Ods::pag*()
		{
			return reinterpret_cast<Ods::pag*>(FB_ALIGN(buf, PAGE_ALIGNMENT));
		}

		Ods::pag* operator->()
		{
			return reinterpret_cast<Ods::pag*>(FB_ALIGN(buf, PAGE_ALIGNMENT));
		}

	private:
		char buf[MAX_PAGE_SIZE + PAGE_ALIGNMENT - 1];
	};

	class HolderAttachments
	{
	public:
		explicit HolderAttachments(Firebird::MemoryPool& p);
		~HolderAttachments();

		void registerAttachment(Attachment* att);
		bool unregisterAttachment(Attachment* att);

		void setPlugin(Firebird::IKeyHolderPlugin* kh);
		Firebird::IKeyHolderPlugin* getPlugin() const
		{
			return keyHolder;
		}

		bool operator==(Firebird::IKeyHolderPlugin* kh) const;

	private:
		Firebird::IKeyHolderPlugin* keyHolder;
		Firebird::HalfStaticArray<Attachment*, 32> attachments;
	};

	class KeyHolderPlugins
	{
	public:
		explicit KeyHolderPlugins(Firebird::MemoryPool& p)
			: knownHolders(p)
		{ }

		void attach(Attachment* att, Config* config);
		void detach(Attachment* att);
		void init(Firebird::IDbCryptPlugin* crypt);

	private:
		Firebird::Mutex holdersMutex;
		Firebird::ObjectsArray<HolderAttachments> knownHolders;
	};

	static int blockingAstChangeCryptState(void*);
	void blockingAstChangeCryptState();

	// IBar's pure virtual functions are implemented here
	void doOnTakenWriteSync(thread_db* tdbb);
	void doOnAst(thread_db* tdbb);

	void loadPlugin(const char* pluginName);
	ULONG getLastPage(thread_db* tdbb);
	void writeDbHeader(thread_db* tdbb, ULONG runpage, Firebird::Stack<ULONG>& pages);

	void lockAndReadHeader(thread_db* tdbb, unsigned flags = 0);
	static const unsigned CRYPT_HDR_INIT =		0x01;
	static const unsigned CRYPT_HDR_NOWAIT =	0x02;

	enum IoResult {SUCCESS_ALL, FAILED_CRYPT, FAILED_IO};
	IoResult internalRead(thread_db* tdbb, FbStatusVector* sv, jrd_file* file,
		BufferDesc* bdb, Ods::pag* page, bool noShadows, PageSpace* pageSpace);
	IoResult internalWrite(thread_db* tdbb, FbStatusVector* sv, jrd_file* file,
		BufferDesc* bdb, Ods::pag* page);

	BarSync sync;
	Firebird::AtomicCounter currentPage;
	Firebird::Mutex pluginLoadMtx, cryptThreadMtx;
	KeyHolderPlugins keyHolderPlugins;
	Thread::Handle cryptThreadId;
	Firebird::IDbCryptPlugin* cryptPlugin;
	Database& dbb;
	Lock* stateLock;
	Lock* threadLock;
	SINT64 slowIO;
	bool crypt, process, down;
};

} // namespace Jrd


#endif // JRD_CRYPTO_MANAGER
