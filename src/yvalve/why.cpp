/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		why.cpp
 *	DESCRIPTION:	Universal Y-valve
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
 * 23-Feb-2002 Dmitry Yemanov - Events wildcarding
 *
 *
 * 2002-02-23 Sean Leyne - Code Cleanup, removed old Win3.1 port (Windows_Only)
 * 2002-02-23 Sean Leyne - Code Cleanup, removed old M88K and NCR3000 port
 *
 * 2002.10.27 Sean Leyne - Completed removal of obsolete "IMP" port
 * 2002.10.27 Sean Leyne - Completed removal of obsolete "DG_X86" port
 *
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "MPEXL" port
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "DecOSF" port
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "SGI" port
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 *
 * 2002.12.10 Alex Peshkoff: 1. Moved struct hndl declaration to h-file
 * 							 2. renamed all specific objects to WHY_*
 *
 */

#include "firebird.h"
#include "memory_routines.h"	// needed for get_long
#include "consts_pub.h"
#include "ProviderInterface.h"

#include <stdlib.h>
#include <string.h>
#include "../common/common.h"
#include <stdarg.h>

#include <stdio.h>
#include "../common/gdsassert.h"

// includes specific for DSQL

#include "../dsql/sqlda.h"
#include "../dsql/sqlda_pub.h"

// end DSQL-specific includes

#include "../yvalve/why_proto.h"
#include "../yvalve/prepa_proto.h"
#include "../yvalve/utly_proto.h"
#include "../common/classes/alloc.h"
#include "../common/classes/tree.h"
#include "../common/classes/array.h"
#include "../common/classes/fb_string.h"
#include "../common/classes/RefCounted.h"
#include "../jrd/thread_proto.h"
#include "gen/iberror.h"
#include "../jrd/msg_encode.h"
#include "gen/msg_facs.h"
#include "../jrd/acl.h"
#include "../jrd/inf_pub.h"
#include "../common/db_alias.h"
#include "../common/os/path_utils.h"
#include "../common/classes/ClumpletWriter.h"
#include "../common/utils_proto.h"
#include "../common/StatusHolder.h"

#include "../jrd/flu_proto.h"
#include "../yvalve/gds_proto.h"
#include "../common/isc_proto.h"
#include "../common/isc_f_proto.h"
#include "../common/os/isc_i_proto.h"
#include "../common/isc_s_proto.h"
#include "../yvalve/utl_proto.h"
#include "../common/classes/rwlock.h"
#include "../common/classes/auto.h"
#include "../common/classes/init.h"
#include "../common/classes/semaphore.h"
#include "../common/classes/fb_atomic.h"
#include "../common/classes/FpeControl.h"
#include "../jrd/constants.h"
#include "../common/ThreadStart.h"
#include "../common/classes/GetPlugins.h"
#include "../remote/client/interface.h"
#include "../yvalve/PluginManager.h"
#include "../yvalve/MasterImplementation.h"

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif

#ifdef HAVE_FLOCK
#include <sys/file.h>			// for flock()
#endif

#ifdef WIN_NT
#include <windows.h>
#endif

#ifdef HAVE_SYS_TIMEB_H
#include <sys/timeb.h>
#endif

using namespace Firebird;

const int IO_RETRY = 20;

inline bool is_network_error(const ISC_STATUS* vector)
{
	return vector[1] == isc_network_error || vector[1] == isc_net_write_err ||
		vector[1] == isc_net_read_err;
}

inline void bad_handle(ISC_STATUS code)
{
	status_exception::raise(Arg::Gds(code));
}

inline void nullCheck(const FB_API_HANDLE* ptr, ISC_STATUS code)
{
	// this function is called for incoming API handles,
	// proposed to be created by the call
	if (!ptr || *ptr)
	{
		bad_handle(code);
	}
}

// Transaction element block

struct teb
{
	FB_API_HANDLE* teb_database;
	int teb_tpb_length;
	const UCHAR* teb_tpb;
};
typedef teb TEB;

#ifdef DEBUG_GDS_ALLOC
#define alloc(x) alloc_debug(x, __FILE__, __LINE__)
static SCHAR *alloc_debug(SLONG, const char*, int);
#else
static SCHAR *alloc(SLONG);
#endif
static ISC_STATUS detach_or_drop_database(ISC_STATUS* user_status, FB_API_HANDLE* handle,
										  bool drop, const ISC_STATUS specCode = 0);
static void release_dsql_support(sqlda_sup&);

namespace Why
{
	// process shutdown flag
	bool shutdownStarted = false;

	// flags
	const UCHAR HANDLE_TRANSACTION_limbo	= 0x01;
	const UCHAR HANDLE_STATEMENT_prepared	= 0x02;

	// forwards
	class CAttachment;
	class CTransaction;
	class CRequest;
	class CBlob;
	class CStatement;
	class CService;
	class CEvents;

	typedef RefPtr<CAttachment> Attachment;
	typedef RefPtr<CTransaction> Transaction;
	typedef RefPtr<CRequest> Request;
	typedef RefPtr<CBlob> Blob;
	typedef RefPtr<CStatement> Statement;
	typedef RefPtr<CService> Service;
	typedef RefPtr<CEvents> Events;

	// force use of default memory pool for Y-Valve objects
	typedef GlobalStorage DefaultMemory;

	// This mutex is used to work with attachless handles, first of all with services
	static GlobalPtr<Mutex> servicesMutex;

	template <typename CleanupRoutine, typename CleanupArg>
	class Clean : public DefaultMemory
	{
	private:
		struct st_clean
		{
			CleanupRoutine *Routine;
			void* clean_arg;
			st_clean(CleanupRoutine *r, void* a)
				: Routine(r), clean_arg(a)
			{ }
			st_clean()
				: Routine(0), clean_arg(0)
			{ }
		};
		HalfStaticArray<st_clean, 1> calls;
		Mutex mutex;

	public:
		Clean() : calls(*getDefaultMemoryPool()) { }

		void add(CleanupRoutine *r, void* a)
		{
			MutexLockGuard guard(mutex);
			for (size_t i = 0; i < calls.getCount(); ++i)
			{
				if (calls[i].Routine == r && calls[i].clean_arg == a)
				{
					return;
				}
			}
			calls.add(st_clean(r, a));
		}

		void call(CleanupArg public_handle)
		{
			MutexLockGuard guard(mutex);
			for (size_t i = 0; i < calls.getCount(); ++i)
			{
				if (calls[i].Routine)
				{
					calls[i].Routine(public_handle, calls[i].clean_arg);
				}
			}
		}
	};

	class BaseHandle : public DefaultMemory, public RefCounted
	{
	public:
		UCHAR			type;
		UCHAR			flags;
		FB_API_HANDLE	public_handle;
		Attachment		parent;
    	FB_API_HANDLE*	user_handle;
    	Plugin*			provider;

	protected:
		BaseHandle(UCHAR t, FB_API_HANDLE* pub, Attachment par, Plugin* prov = NULL);

	public:
		static BaseHandle* translate(FB_API_HANDLE);

		void release_user_handle()
		{
			if (user_handle)
			{
				*user_handle = 0;
			}
		}

		void releaseAll();
		Mutex* getProviderInterfaceCleanupMutex();

		// required to put pointers to it into the tree
		static const FB_API_HANDLE& generate(const void* /*sender*/, const BaseHandle* value)
		{
			return value->public_handle;
		}

		static void drop(BaseHandle*);

	protected:
		virtual void releaseProviderInterface() = 0;
		virtual ~BaseHandle();
	};

	template <typename T>
	class HandleArray
	{
	public:
		explicit HandleArray(MemoryPool& p) : arr(p) { }
		HandleArray() : arr(*getDefaultMemoryPool()) { }

		void destroy()
		{
			MutexLockGuard guard(mtx);

			size_t i;
			while ((i = arr.getCount()))
			{
				T::destroy(arr[i - 1]);
			}
		}

		void toParent(T* newMember)
		{
			MutexLockGuard guard(mtx);

			arr.add(newMember);
		}

		void fromParent(T* oldMember)
		{
			MutexLockGuard guard(mtx);

			size_t pos;
			if (arr.find(oldMember, pos))
			{
				arr.remove(pos);
			}
#ifdef DEV_BUILD
			else
			{
				//Attempt to deregister not registered member
				fb_assert(false);
			}
#endif
		}

	private:
		SortedArray<T*> arr;
		Mutex mtx;
	};

	class CAttachment : public BaseHandle
	{
	public:
		HandleArray<CTransaction> transactions;
		HandleArray<CRequest> requests;
		HandleArray<CBlob> blobs;
		HandleArray<CStatement> statements;
		HandleArray<CEvents> events;

		int enterCount;
		Mutex enterMutex;

		Clean<AttachmentCleanupRoutine, FB_API_HANDLE*> cleanup;
		// TODO: After fixing handles in engine they should become RefCounted here.
		//RefCounted<IAttachment> providerInterface;
		IAttachment* providerInterface;
		StatusHolder status;		// Do not use raise() method of this class in yValve
		PathName db_path;

		static ISC_STATUS hError()
		{
			return isc_bad_db_handle;
		}

		static UCHAR hType()
		{
			return 1;
		}

	public:
		CAttachment(IAttachment*, FB_API_HANDLE*, Plugin*);

		static void destroy(CAttachment*);

		bool destroying() const
		{
			return flagDestroying;
		}

	private:
		~CAttachment() { }

		void releaseProviderInterface()
		{
			providerInterface = NULL;
		}

		bool flagDestroying;
	};

	class CTransaction : public BaseHandle
	{
	public:
		Clean<TransactionCleanupRoutine, FB_API_HANDLE> cleanup;
		Transaction next;
		// TODO: After fixing handles in engine they should become RefCounted here.
		//RefCounted<ITransaction> providerInterface;
		ITransaction* providerInterface;
		HandleArray<CBlob> blobs;

		static ISC_STATUS hError()
		{
			return isc_bad_trans_handle;
		}

		static UCHAR hType()
		{
			return 2;
		}

	public:
		CTransaction(ITransaction* h, FB_API_HANDLE* pub, Attachment par)
			: BaseHandle(hType(), pub, par), next(0), providerInterface(h),
			blobs(getPool())
		{
			parent->transactions.toParent(this);
		}

		CTransaction(FB_API_HANDLE* pub)
			: BaseHandle(hType(), pub, Attachment(0)), next(0), providerInterface(0),
			blobs(getPool())
		{
		}

		static void destroy(CTransaction*);

	private:
		~CTransaction() { }

		void releaseProviderInterface()
		{
			providerInterface = NULL;
		}
	};

	class CRequest : public BaseHandle
	{
	public:
		// TODO: After fixing handles in engine they should become RefCounted here.
		//RefCounted<IRequest> providerInterface;
		IRequest* providerInterface;

		static ISC_STATUS hError()
		{
			return isc_bad_req_handle;
		}

		static UCHAR hType()
		{
			return 3;
		}

	public:
		CRequest(IRequest* h, FB_API_HANDLE* pub, Attachment par)
			: BaseHandle(hType(), pub, par), providerInterface(h)
		{
			parent->requests.toParent(this);
		}

		static void destroy(CRequest* h)
		{
			h->release_user_handle();
			h->parent->requests.fromParent(h);
			drop(h);
		}

	private:
		~CRequest() { }

		void releaseProviderInterface()
		{
			providerInterface = NULL;
		}
	};

	class CBlob : public BaseHandle
	{
	public:
		// TODO: After fixing handles in engine they should become RefCounted here.
		//RefCounted<IBlob> providerInterface;
		IBlob* providerInterface;
		Transaction tra;

		static ISC_STATUS hError()
		{
			return isc_bad_segstr_handle;
		}

		static UCHAR hType()
		{
			return 4;
		}

	public:
		CBlob(IBlob* h, FB_API_HANDLE* pub, Attachment par, Transaction t)
			: BaseHandle(hType(), pub, par), providerInterface(h), tra(t)
		{
			parent->blobs.toParent(this);
			tra->blobs.toParent(this);
		}

		static void destroy(CBlob* h)
		{
			h->tra->blobs.fromParent(h);
			h->parent->blobs.fromParent(h);
			drop(h);
		}

	private:
		~CBlob() { }

		void releaseProviderInterface()
		{
			providerInterface = NULL;
		}
	};

	class CStatement : public BaseHandle
	{
	public:
		// TODO: After fixing handles in engine they should become RefCounted here.
		//RefCounted<IStatement> providerInterface;
		IStatement* providerInterface;
		struct sqlda_sup das;

		static ISC_STATUS hError()
		{
			return isc_bad_stmt_handle;
		}

		static UCHAR hType()
		{
			return 5;
		}

	public:
		CStatement(IStatement* h, FB_API_HANDLE* pub, Attachment par)
			: BaseHandle(hType(), pub, par), providerInterface(h)
		{
			parent->statements.toParent(this);
			memset(&das, 0, sizeof das);
		}

		void checkPrepared() const
		{
			if (!(flags & HANDLE_STATEMENT_prepared))
			{
				status_exception::raise(Arg::Gds(isc_unprepared_stmt));
			}
		}

		static void destroy(CStatement* h)
		{
			h->release_user_handle();
			h->parent->statements.fromParent(h);
			drop(h);
		}

	private:
		~CStatement()
		{
			if (parent->destroying())
			{
				release_dsql_support(das);
			}
		}

		void releaseProviderInterface()
		{
			providerInterface = NULL;
		}
	};

	class CService : public BaseHandle
	{
	public:
		Clean<AttachmentCleanupRoutine, FB_API_HANDLE*> cleanup;
		// TODO: After fixing handles in engine they should become RefCounted here.
		//RefCounted<IService> providerInterface;
		IService* providerInterface;

		static ISC_STATUS hError()
		{
			return isc_bad_svc_handle;
		}

		static UCHAR hType()
		{
			return 6;
		}

	public:
		CService(IService* h, FB_API_HANDLE* pub, Plugin* prov)
			: BaseHandle(hType(), pub, Attachment(0), prov), providerInterface(h)
		{
		}

		static void destroy(CService* h)
		{
			h->cleanup.call(&h->public_handle);
			drop(h);
		}

	private:
		void releaseProviderInterface()
		{
			providerInterface = NULL;
		}

		~CService() { }
	};

	class CEvents : public BaseHandle, public EventCallback
	{
	public:
		// TODO: After fixing handles in engine they should become RefCounted here.
		//RefCounted<IEvents> providerInterface;
		IEvents* providerInterface;
		EventCallback* callBack;

		static ISC_STATUS hError()
		{
			return isc_bad_events_handle;
		}

		static UCHAR hType()
		{
			return 7;
		}

	public:
		CEvents(FB_API_HANDLE* pub, Attachment par, FPTR_EVENT_CALLBACK f, void* a)
			: BaseHandle(hType(), pub, par), providerInterface(NULL), func(f), arg(a)
		{
			parent->events.toParent(this);
		}

		static void destroy(CEvents* h)
		{
			h->release_user_handle();
			h->parent->events.fromParent(h);
			drop(h);
		}

		void FB_CARG eventCallbackFunction(unsigned int length, const UCHAR* events)
		{
			func(arg, length, events);
		}

		~CEvents()
		{ }

	private:
		void releaseProviderInterface()
		{
			providerInterface = NULL;
		}

		FPTR_EVENT_CALLBACK func;
		void* arg;
	};

	class WaitEvents : public CEvents
	{
	public:
		WaitEvents(Attachment par, FPTR_EVENT_CALLBACK f, void* a)
			: CEvents(NULL, par, f, a)
		{ }

		void FB_CARG eventCallbackFunction(unsigned int length, const UCHAR* events)
		{
			CEvents::eventCallbackFunction(length, events);
			sem.release();
		}

		void wait()
		{
			sem.enter();
		}

	private:
		Semaphore sem;
	};

	typedef BePlusTree<BaseHandle*, FB_API_HANDLE, MemoryPool, BaseHandle> HandleMapping;

	GlobalPtr<HandleMapping> handleMapping;
	ULONG handle_sequence_number = 0;
	GlobalPtr<RWLock> handleMappingLock;

	InitInstance<HandleArray<CAttachment> > attachments;
	GlobalPtr<Mutex> shutdownCallbackMutex;

	class ShutChain : public GlobalStorage
	{
	private:
		ShutChain(ShutChain* link, FB_SHUTDOWN_CALLBACK cb, const int m, void* a)
			: next(link), callBack(cb), mask(m), arg(a)
		{ }

		~ShutChain() { }

	private:
		static ShutChain* list;
		ShutChain* next;
		FB_SHUTDOWN_CALLBACK callBack;
		int mask;
		void* arg;

	public:
		static void add(FB_SHUTDOWN_CALLBACK cb, const int m, void* a)
		{
			MutexLockGuard guard(shutdownCallbackMutex);

			for (ShutChain* chain = list; chain; chain = chain->next)
			{
				if (chain->callBack == cb && chain->arg == a)
				{
					if (m)
					{
						chain->mask |= m;
					}
					else
					{
						chain->mask = 0;
					}
					return;
				}
			}

			list = new ShutChain(list, cb, m, a);
		}

		static int run(const int m, const int reason)
		{
			int rc = FB_SUCCESS;
			MutexLockGuard guard(shutdownCallbackMutex);

			for (ShutChain* chain = list; chain; chain = chain->next)
			{
				if ((chain->mask & m) && (chain->callBack(reason, m, chain->arg) != FB_SUCCESS))
				{
					rc = FB_FAILURE;
				}
			}

			return rc;
		}
	};

	ShutChain* ShutChain::list = 0;


	BaseHandle::BaseHandle(UCHAR t, FB_API_HANDLE* pub, Attachment par, Plugin* prov)
		: type(t), flags(0), parent(par), user_handle(0), provider(prov)
	{
		addRef();

		{ // scope for write lock on handleMappingLock
			WriteLockGuard sync(handleMappingLock);
			// Loop until we find an empty handle slot.
			// This is to care of case when counter rolls over
			do {
				// Generate handle number using rolling counter.
				// This way we tend to give out unique handle numbers and closed
				// handles do not appear as valid to our clients.
				ULONG temp = ++handle_sequence_number;

				// Avoid generating NULL handle when sequence number wraps
				if (!temp)
					temp = ++handle_sequence_number;
				public_handle = (FB_API_HANDLE)(IPTR)temp;
			} while (!handleMapping->add(this));
		}

		if (pub)
		{
			*pub = public_handle;
		}
		if (provider)
		{
			provider->addRef();
		}
	}

	BaseHandle* BaseHandle::translate(FB_API_HANDLE handle)
	{
		HandleMapping::Accessor accessor(&handleMapping);
		if (accessor.locate(handle))
		{
			return accessor.current();
		}

		return 0;
	}

	Mutex* BaseHandle::getProviderInterfaceCleanupMutex()
	{
		return parent.hasData() ? &(parent->enterMutex) : &servicesMutex;
	}

	void BaseHandle::releaseAll()
	{
		{ // providerInterface cleanup mutex scope
			MutexLockGuard guard(*getProviderInterfaceCleanupMutex());
			releaseProviderInterface();
		}

		if (provider)
		{
			PluginInterface()->releasePlugin(provider);
			provider = NULL;
		}
	}

	template <typename ToHandle> RefPtr<ToHandle> translate(FB_API_HANDLE* handle, bool checkAttachment = true)
	{
		if (shutdownStarted)
		{
			status_exception::raise(Arg::Gds(isc_att_shutdown));
		}

		if (handle && *handle)
		{
			ReadLockGuard sync(handleMappingLock);

			BaseHandle* rc = BaseHandle::translate(*handle);
			if (rc && rc->type == ToHandle::hType())
			{
				if (checkAttachment)
				{
					Attachment attachment = rc->parent;
					if (attachment && attachment->status.getError())
					{
						status_exception::raise(attachment->status.value());
					}
				}

				return RefPtr<ToHandle>(static_cast<ToHandle*>(rc));
			}
		}

		status_exception::raise(Arg::Gds(ToHandle::hError()));
		// compiler warning silencer
		return RefPtr<ToHandle>(0);
	}

	BaseHandle::~BaseHandle()
	{
		if (provider)
		{
			PluginInterface pi;
			pi->releasePlugin(provider);
		}
	}

	void BaseHandle::drop(BaseHandle* h)
	{
		WriteLockGuard sync(handleMappingLock);

		// Silently ignore bad handles for PROD_BUILD
		if (handleMapping->locate(h->public_handle))
		{
			handleMapping->fastRemove();
		}
#ifdef DEV_BUILD
		else
		{
			//Attempt to release bad handle
			fb_assert(false);
		}
#endif
		h->release();
	}

	CAttachment::CAttachment(IAttachment* h, FB_API_HANDLE* pub, Plugin* prov)
		: BaseHandle(hType(), pub, Attachment(0), prov),
		  transactions(getPool()),
		  requests(getPool()),
		  blobs(getPool()),
		  statements(getPool()),
		  events(getPool()),
		  enterCount(0),
		  providerInterface(h),
		  db_path(getPool()),
		  flagDestroying(false)
	{
		attachments().toParent(this);
		parent = this;
	}

	void CAttachment::destroy(CAttachment* h)
	{
		h->cleanup.call(&h->public_handle);

		// cleanup
		try
		{
			h->flagDestroying = true;

			h->requests.destroy();
			h->statements.destroy();
			h->blobs.destroy();
			h->events.destroy();
			// There should not be transactions at this point,
			// but it's no danger in cleaning empty array
			h->transactions.destroy();
			h->parent = NULL;

			h->flagDestroying = false;
		}
		catch(const Exception&)
		{
			h->flagDestroying = false;
			throw;
		}

		attachments().fromParent(h);
		drop(h);
	}

	void CTransaction::destroy(CTransaction* h)
	{
		h->cleanup.call(h->public_handle);
		h->blobs.destroy();

		if (h->parent)
		{
			h->parent->transactions.fromParent(h);
		}

		CTransaction* sub = h->next;

		drop(h);

		if (sub)
		{
			CTransaction::destroy(sub);
		}
	}

	template <typename T>
	void destroy(RefPtr<T> h)
	{
		if (h)
		{
			T::destroy(h);
		}
	}

	template <typename T>
	void destroyNoThrow(RefPtr<T> h) throw()
	{
		// This form of destroy is used in catch handlers,
		// when we already have probably more interesting status to return.
		try
		{
			if (h)
			{
				T::destroy(h);
			}
		}
		catch (const Exception&) { }
	}
} // namespace Why

using namespace Why;

#ifdef DEV_BUILD
static void check_status_vector(const ISC_STATUS*);
#endif

static void event_ast(void*, USHORT, const UCHAR*);

static Transaction find_transaction(Attachment, Transaction);

inline Transaction findTransaction(FB_API_HANDLE* public_handle, Attachment a)
{
	Transaction t = find_transaction(a, translate<CTransaction>(public_handle));
	if (! t)
	{
		bad_handle(isc_bad_trans_handle);
	}

	return t;
}

static int get_database_info(Transaction, unsigned char**);
static USHORT sqlda_buffer_size(USHORT, const XSQLDA*, USHORT);
static ISC_STATUS get_transaction_info(ISC_STATUS*, Transaction, unsigned char**);

static void iterative_sql_info(ISC_STATUS*, FB_API_HANDLE*, USHORT, const SCHAR*, SSHORT,
							   SCHAR*, USHORT, XSQLDA*);
static ISC_STATUS open_blob(ISC_STATUS*, FB_API_HANDLE*, FB_API_HANDLE*, FB_API_HANDLE*, ISC_QUAD*,
						USHORT, const UCHAR*, bool);
static ISC_STATUS prepare(ISC_STATUS*, Transaction);
static void save_error_string(ISC_STATUS*);
static bool set_path(const PathName&, PathName&);

/*
 * A client-only API call, isc_reset_fpe() is deprecated - we do not use
 * the FPE handler anymore, it can't be used in multithreaded library.
 * Parameter is ignored, it always returns FPE_RESET_ALL_API_CALL,
 * this is the most close code to what we are doing now.
 */

//static const USHORT FPE_RESET_INIT_ONLY		= 0x0;	// Don't reset FPE after init
//static const USHORT FPE_RESET_NEXT_API_CALL	= 0x1;	// Reset FPE on next gds call
static const USHORT FPE_RESET_ALL_API_CALL		= 0x2;	// Reset FPE on all gds call

// Global array to store string from the status vector in save_error_string.

const int MAXERRORSTRINGLENGTH	= 250;
static TEXT glbstr1[MAXERRORSTRINGLENGTH];
static const TEXT glbunknown[10] = "<unknown>";

const USHORT PREPARE_BUFFER_SIZE	= 32768;	// size of buffer used in isc_dsql_prepare call
const USHORT DESCRIBE_BUFFER_SIZE	= 1024;		// size of buffer used in isc_dsql_describe_xxx call

namespace
{
	// Status:	Provides correct status vector for operation and init() it.
	class StatusVector : public StackIface<Status, FB_STATUS_VERSION>
	{
	public:
		explicit StatusVector(ISC_STATUS* v) throw()
			: local_vector(v ? v : local_status)
		{
			init();
		}

		operator ISC_STATUS*()
		{
			return local_vector;
		}

		~StatusVector()
		{
#ifdef DEV_BUILD
			check_status_vector(local_vector);
#endif
		}

		void FB_CARG set(unsigned int length, const ISC_STATUS* value)
		{
			fb_utils::copyStatus(local_vector, FB_NELEM(local_status), value, length);
		}

		void FB_CARG set(const ISC_STATUS* value)
		{
			set(fb_utils::statusLength(value), value);
		}

		void FB_CARG init()
		{
			fb_utils::init_status(local_vector);
		}

		const ISC_STATUS* FB_CARG get() const
		{
			return local_vector;
		}

		int FB_CARG isSuccess() const
		{
			switch (local_vector[1])
			{
				case 0:
				case 100:
				case 101:
					return true;
				default:
					break;
			}
			return false;
		}

	private:
		ISC_STATUS_ARRAY local_status;
		ISC_STATUS* local_vector;
	};

#ifdef UNIX
	int killed;
	bool procInt, procTerm;
	SignalSafeSemaphore* shutdownSemaphore = NULL;

	const int SHUTDOWN_TIMEOUT = 5000;	// 5 sec

	void atExitShutdown()
	{
		if (!shutdownStarted)	// static not protected by mutex flag is OK here - works in dtors
			fb_shutdown(SHUTDOWN_TIMEOUT, fb_shutrsn_exit_called);
	}

	THREAD_ENTRY_DECLARE shutdownThread(THREAD_ENTRY_PARAM)
	{
		for (;;)
		{
			killed = 0;
			try {
				if (shutdownSemaphore)
				{
					shutdownSemaphore->enter();
				}
			}
			catch (status_exception& e)
			{
				TEXT buffer[BUFFER_LARGE];
				const ISC_STATUS* vector = e.value();
				if (! (vector && fb_interpret(buffer, sizeof(buffer), &vector)))
				{
					strcpy(buffer, "Unknown failure in shutdown thread in shutSem:enter()");
				}
				gds__log("%s", buffer);
				exit(0);
			}

			if (! killed)
			{
				break;
			}

			// perform shutdown
			if (fb_shutdown(SHUTDOWN_TIMEOUT, fb_shutrsn_signal) == FB_SUCCESS)
			{
				InstanceControl::registerShutdown(0);
				break;	//exit(0);
			}
		}

		return 0;
	}

	void handler(int sig)
	{
		if (killed)
		{
			return;
		}
		if (shutdownSemaphore)
		{
			killed = sig;
			shutdownSemaphore->release();
		}
	}

	void handlerInt(void*)
	{
		handler(SIGINT);
	}

	void handlerTerm(void*)
	{
		handler(SIGTERM);
	}

	class CtrlCHandler
	{
	public:
		SignalSafeSemaphore semaphore;

		explicit CtrlCHandler(MemoryPool&)
		{
			InstanceControl::registerShutdown(atExitShutdown);

			Thread::start(shutdownThread, 0, 0, &handle);

			procInt = ISC_signal(SIGINT, handlerInt, 0);
			procTerm = ISC_signal(SIGTERM, handlerTerm, 0);
			shutdownSemaphore = &semaphore;
		}

		~CtrlCHandler()
		{
			ISC_signal_cancel(SIGINT, handlerInt, 0);
			ISC_signal_cancel(SIGTERM, handlerTerm, 0);

			if (! killed)
			{
				// must be done to let shutdownThread close
				shutdownSemaphore->release();
				shutdownSemaphore = NULL;
				Thread::waitForCompletion(handle);
			}
		}
	private:
		Thread::Handle handle;
	};
#endif //UNIX

	class BuiltinRegister
	{
	public:
		static void init()
		{
			PluginInterface pi;
			Remote::registerRedirector(pi);
		}

		static void cleanup()
		{ }
	};

	// YEntry:	Tracks FP exceptions state (via FpeControl).
	//			Accounts activity per different attachments.
	class YEntry : public FpeControl
	{
	public:
		explicit YEntry(StatusVector& _status, BaseHandle* primary)
			: att(primary->parent), status(&_status)
		{
			init();
		}

		explicit YEntry()
			: att(0), status(0)
		{
			init();
		}

		void init()
		{
#ifdef UNIX
			static GlobalPtr<CtrlCHandler> ctrlCHandler;
#endif //UNIX

			static InitMutex<BuiltinRegister> registerBuiltinPlugins;
			registerBuiltinPlugins.init();

			if (att)
			{
				MutexLockGuard guard(att->enterMutex);
				att->enterCount++;
			}
		}

		~YEntry()
		{
			if (att)
			{
				MutexLockGuard guard(att->enterMutex);
				att->enterCount--;

				if (!att->status.getError())
				{
					const ISC_STATUS err = (*status)[1];
					if (err == isc_shutdown || err == isc_att_shutdown)
					{
						// Save exact error to report it at all following calls
						// of this attachment (except of detach). This is correct
						// as server already closed its connection with client at
						// this time.
						att->status.save(*status);
					}
				}
			}
		}

	private:
		YEntry(const YEntry&);	// prohibit copy constructor

		Attachment att;
		StatusVector* status;
	};

} // anonymous namespace


// Information items for two phase commit

static const UCHAR prepare_tr_info[] =
{
	isc_info_tra_id,
	isc_info_end
};

// Information items for DSQL prepare

static const SCHAR sql_prepare_info[] =
{
	isc_info_sql_select,
	isc_info_sql_describe_vars,
	isc_info_sql_sqlda_seq,
	isc_info_sql_type,
	isc_info_sql_sub_type,
	isc_info_sql_scale,
	isc_info_sql_length,
	isc_info_sql_field,
	isc_info_sql_relation,
	isc_info_sql_owner,
	isc_info_sql_alias,
	isc_info_sql_describe_end
};

// Information items for SQL info

static const SCHAR describe_select_info[] =
{
	isc_info_sql_select,
	isc_info_sql_describe_vars,
	isc_info_sql_sqlda_seq,
	isc_info_sql_type,
	isc_info_sql_sub_type,
	isc_info_sql_scale,
	isc_info_sql_length,
	isc_info_sql_field,
	isc_info_sql_relation,
	isc_info_sql_owner,
	isc_info_sql_alias,
	isc_info_sql_describe_end
};

static const SCHAR describe_bind_info[] =
{
	isc_info_sql_bind,
	isc_info_sql_describe_vars,
	isc_info_sql_sqlda_seq,
	isc_info_sql_type,
	isc_info_sql_sub_type,
	isc_info_sql_scale,
	isc_info_sql_length,
	isc_info_sql_field,
	isc_info_sql_relation,
	isc_info_sql_owner,
	isc_info_sql_alias,
	isc_info_sql_describe_end
};

static const SCHAR sql_prepare_info2[] =
{
	isc_info_sql_stmt_type,

	// describe_select_info
	isc_info_sql_select,
	isc_info_sql_describe_vars,
	isc_info_sql_sqlda_seq,
	isc_info_sql_type,
	isc_info_sql_sub_type,
	isc_info_sql_scale,
	isc_info_sql_length,
	isc_info_sql_field,
	isc_info_sql_relation,
	isc_info_sql_owner,
	isc_info_sql_alias,
	isc_info_sql_describe_end,

	// describe_bind_info
	isc_info_sql_bind,
	isc_info_sql_describe_vars,
	isc_info_sql_sqlda_seq,
	isc_info_sql_type,
	isc_info_sql_sub_type,
	isc_info_sql_scale,
	isc_info_sql_length,
	isc_info_sql_field,
	isc_info_sql_relation,
	isc_info_sql_owner,
	isc_info_sql_alias,
	isc_info_sql_describe_end
};

namespace
{
	class NoEntrypoint
	{
	public:
		virtual void FB_CARG noEntrypoint(Status* s)
		{
			s->set(Arg::Gds(isc_unavailable).value());
		}
	};
}

ISC_STATUS API_ROUTINE isc_attach_database(ISC_STATUS* user_status,
										   SSHORT file_length,
										   const TEXT* file_name,
										   FB_API_HANDLE* public_handle,
										   SSHORT dpb_length,
										   const SCHAR* dpb)
{
/**************************************
 *
 *	g d s _ $ a t t a c h _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Attach a database through the first subsystem
 *	that recognizes it.
 *
 **************************************/
	StatusVector temp(NULL);
	Attachment attachment(0);

	StatusVector status(user_status);
	StatusVector* ptr = &status;

	try
	{
		YEntry entryGuard;

		nullCheck(public_handle, isc_bad_db_handle);

		if (shutdownStarted)
		{
			status_exception::raise(Arg::Gds(isc_att_shutdown));
		}

		if (!file_name)
		{
			status_exception::raise(Arg::Gds(isc_bad_db_format) << Arg::Str(""));
		}

		if (dpb_length > 0 && !dpb)
		{
			status_exception::raise(Arg::Gds(isc_bad_dpb_form));
		}

		// copy the file name to a temp buffer, since some of the following utilities can modify it

		PathName org_filename(file_name, file_length ? file_length : strlen(file_name));
		ClumpletWriter newDpb(ClumpletReader::dpbList, MAX_DPB_SIZE,
			reinterpret_cast<const UCHAR*>(dpb), dpb_length);

		bool utfFilename = newDpb.find(isc_dpb_utf8_filename);

		if (utfFilename)
			ISC_utf8ToSystem(org_filename);
		else
		{
			newDpb.insertTag(isc_dpb_utf8_filename);

			for (newDpb.rewind(); !newDpb.isEof(); newDpb.moveNext())
			{
				UCHAR tag = newDpb.getClumpTag();
				switch (tag)
				{
					case isc_dpb_user_name:
					case isc_dpb_password:
					case isc_dpb_sql_role_name:
					case isc_dpb_trusted_auth:
					case isc_dpb_trusted_role:
					case isc_dpb_working_directory:
					case isc_dpb_set_db_charset:
					case isc_dpb_process_name:
					{
						string s;
						newDpb.getString(s);
						ISC_systemToUtf8(s);
						newDpb.deleteClumplet();
						newDpb.insertString(tag, s);
						break;
					}
				}
			}
		}

		setLogin(newDpb);
		org_filename.rtrim();

		PathName expanded_filename;
		bool unescaped = false;

		if (!set_path(org_filename, expanded_filename))
		{
			expanded_filename = org_filename;
			ISC_systemToUtf8(expanded_filename);
			ISC_unescape(expanded_filename);
			unescaped = true;
			ISC_utf8ToSystem(expanded_filename);
			ISC_expand_filename(expanded_filename, true);
		}

		ISC_systemToUtf8(org_filename);
		ISC_systemToUtf8(expanded_filename);

		if (unescaped)
			ISC_escape(expanded_filename);

		if (org_filename != expanded_filename && !newDpb.find(isc_dpb_org_filename))
		{
			newDpb.insertPath(isc_dpb_org_filename, org_filename);
		}

		Firebird::PathName dummy;
		RefPtr<Config> config;
		ResolveDatabaseAlias(expanded_filename, dummy, &config);
		for (GetPlugins<PProvider, NoEntrypoint> providerIterator(PluginType::Provider,
																  FB_P_PROVIDER_VERSION, config);
			 providerIterator.hasData(); providerIterator.next())
		{
			PProvider* p = providerIterator.plugin();

			attachment = new CAttachment(NULL, public_handle, p);
			attachment->db_path = expanded_filename;

			p->attachDatabase(ptr, &attachment->providerInterface, *public_handle, expanded_filename.c_str(),
							  newDpb.getBufferLength(), newDpb.getBuffer());
			if (ptr->isSuccess())
			{
				status[0] = isc_arg_gds;
				status[1] = 0;

				/* Check to make sure that status[2] is not a warning.  If it is, then
				 * the rest of the vector should be untouched.  If it is not, then make
				 * this element isc_arg_end
				 *
				 * This cleanup is done to remove any erroneous errors from trying multiple
				 * protocols for a database attach
				 */
				if (status[2] != isc_arg_warning) {
					status[2] = isc_arg_end;
				}

				return status[1];
			}
			else
			{
				*public_handle = 0;
				destroy(attachment);
				attachment = 0;
			}

			if ((*ptr)[1] != isc_unavailable)
			{
				ptr = &temp;
			}
		}

		if (status[1] == 0)
		{
			Arg::Gds(isc_unavailable).raise();
		}
	}
	catch (const Exception& e)
	{
		if (attachment)
		{
			{
				MutexLockGuard guard(*attachment->getProviderInterfaceCleanupMutex());

				if (attachment->providerInterface)
				{
					attachment->providerInterface->detach(&temp);
					attachment->providerInterface = NULL;
				}
			}
			destroyNoThrow(attachment);
		}

  		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_blob_info(ISC_STATUS*	user_status,
									 FB_API_HANDLE*		blob_handle,
									 SSHORT		item_length,
									 const SCHAR*		items,
									 SSHORT		buffer_length,
									 SCHAR*		buffer)
{
/**************************************
 *
 *	g d s _ $ b l o b _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Provide information on blob object.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Blob blob = translate<CBlob>(blob_handle);
		YEntry entryGuard(status, blob);
		blob->providerInterface->getInfo(&status, item_length, reinterpret_cast<const unsigned char*>(items),
							  buffer_length, reinterpret_cast<unsigned char*>(buffer));
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_cancel_blob(ISC_STATUS* user_status,
									   FB_API_HANDLE* blob_handle)
{
/**************************************
 *
 *	g d s _ $ c a n c e l _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Abort a partially completed blob.
 *
 **************************************/
	if (!*blob_handle)
	{
		if (user_status)
		{
			fb_utils::init_status(user_status);
		}
		return FB_SUCCESS;
	}

	StatusVector status(user_status);

	try
	{
		Blob blob = translate<CBlob>(blob_handle);
		YEntry entryGuard(status, blob);

		{
			MutexLockGuard guard(*blob->getProviderInterfaceCleanupMutex());

			if (blob->providerInterface)
			{
				blob->providerInterface->cancel(&status);
				if (status.isSuccess())
				{
					blob->providerInterface = NULL;
				}
			}
		}

		if (status.isSuccess())
		{
			destroy(blob);
			*blob_handle = 0;
		}
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_cancel_events(ISC_STATUS* user_status,
										 FB_API_HANDLE* handle,
										 SLONG* id)
{
/**************************************
 *
 *	g d s _ $ c a n c e l _ e v e n t s
 *
 **************************************
 *
 * Functional description
 *	Try to cancel an event.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Attachment attachment = translate<CAttachment>(handle);
		// This cast appears awful, but as long as handles are 32-bit entities it's OK
		Events events = translate<CEvents>((FB_API_HANDLE*)id);
		if (events->parent != attachment)
		{
			Arg::Gds(isc_bad_events_handle).raise();
		}
		YEntry entryGuard(status, events->parent);

		{
			MutexLockGuard guard(*events->getProviderInterfaceCleanupMutex());
			if (events->providerInterface)
			{
				events->providerInterface->cancel(&status);
				if (status.isSuccess())
				{
					events->providerInterface = NULL;
				}
			}
		}

		if (status.isSuccess())
		{
			destroy(events);
			*id = 0;
		}
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE fb_cancel_operation(ISC_STATUS* user_status,
											FB_API_HANDLE* handle,
											USHORT option)
{
/**************************************
 *
 *	g d s _ $ c a n c e l _ o p e r a t i o n
 *
 **************************************
 *
 * Functional description
 *	Try to cancel an operation.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		YEntry entryGuard;
		Attachment attachment = translate<CAttachment>(handle);
		// mutex will be locked here for a really long time
		MutexLockGuard guard(attachment->enterMutex);
		if (attachment->enterCount || option != fb_cancel_raise)
		{
			attachment->providerInterface->cancelOperation(&status, option);
		}
		else
		{
			status_exception::raise(Arg::Gds(isc_nothing_to_cancel));
		}
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_close_blob(ISC_STATUS* user_status,
									  FB_API_HANDLE* blob_handle)
{
/**************************************
 *
 *	g d s _ $ c l o s e _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Close a blob opened either for reading or writing (creation).
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Blob blob = translate<CBlob>(blob_handle);
		YEntry entryGuard(status, blob);

		{
			MutexLockGuard guard(*blob->getProviderInterfaceCleanupMutex());

			if (blob->providerInterface)
			{
				blob->providerInterface->close(&status);
				if (status.isSuccess())
				{
					blob->providerInterface = NULL;
				}
			}
		}

		if (status.isSuccess())
		{
			destroy(blob);
			*blob_handle = 0;
		}
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_commit_transaction(ISC_STATUS* user_status, FB_API_HANDLE* tra_handle)
{
/**************************************
 *
 *	g d s _ $ c o m m i t
 *
 **************************************
 *
 * Functional description
 *	Commit a transaction.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Transaction transaction = translate<CTransaction>(tra_handle);
		Transaction sub;
		YEntry entryGuard(status, transaction);

		{
			MutexLockGuard guard(*transaction->getProviderInterfaceCleanupMutex());

			if (transaction->providerInterface)
			{
				// Handle single transaction case
				transaction->providerInterface->commit(&status);
				if (!status.isSuccess())
				{
					return status[1];
				}
			}
			else
			{
				// Handle two phase commit.  Start by putting everybody into
				// limbo.  If anybody fails, punt
				if (!(transaction->flags & HANDLE_TRANSACTION_limbo))
				{
					if (prepare(status, transaction))
					{
						return status[1];
					}
				}

				// Everybody is in limbo, now commit everybody.
				// In theory, this can't fail
				for (sub = transaction->next; sub; sub = sub->next)
				{
					sub->providerInterface->commit(&status);
					if (!status.isSuccess())
					{
						return status[1];
					}
				}
			}
		}

		destroy(transaction);

		*tra_handle = 0;
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_commit_retaining(ISC_STATUS* user_status, FB_API_HANDLE* tra_handle)
{
/**************************************
 *
 *	g d s _ $ c o m m i t _ r e t a i n i n g
 *
 **************************************
 *
 * Functional description
 *	Do a commit retaining.
 *
 * N.B., the transaction cleanup handlers are NOT
 * called here since, conceptually, the transaction
 * is still running.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Transaction transaction = translate<CTransaction>(tra_handle);
		YEntry entryGuard(status, transaction);

		for (Transaction sub = transaction; sub; sub = sub->next)
		{
			if (sub->providerInterface)
			{
				sub->providerInterface->commitRetaining(&status);
				if (!status.isSuccess())
				{
					return status[1];
				}
			}
		}

		transaction->flags |= HANDLE_TRANSACTION_limbo;
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_compile_request(ISC_STATUS* user_status,
								   FB_API_HANDLE* db_handle,
								   FB_API_HANDLE* req_handle,
								   USHORT blr_length,
								   const SCHAR* blr)
{
/**************************************
 *
 *	g d s _ $ c o m p i l e
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	StatusVector status(user_status);
	Attachment attachment(NULL);
	IRequest* rq = NULL;

	try
	{
		attachment = translate<CAttachment>(db_handle);
		YEntry entryGuard(status, attachment);
		nullCheck(req_handle, isc_bad_req_handle);

		rq = attachment->providerInterface->compileRequest(&status, blr_length,
												reinterpret_cast<const unsigned char*>(blr));
		if (!status.isSuccess())
		{
			return status[1];
		}

		new CRequest(rq, req_handle, attachment);
	}
	catch (const Exception& e)
	{
		if (rq)
		{
			rq->free(&status);
			*req_handle = 0;
		}
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_compile_request2(ISC_STATUS* user_status,
									FB_API_HANDLE* db_handle,
									FB_API_HANDLE* req_handle,
									USHORT blr_length,
									const SCHAR* blr)
{
/**************************************
 *
 *	g d s _ $ c o m p i l e 2
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		if (isc_compile_request(status, db_handle, req_handle, blr_length, blr))
		{
			return status[1];
		}

		Request request = translate<CRequest>(req_handle);
		request->user_handle = req_handle;
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_create_blob(ISC_STATUS* user_status,
									   FB_API_HANDLE* db_handle,
									   FB_API_HANDLE* tra_handle,
									   FB_API_HANDLE* blob_handle,
									   ISC_QUAD* blob_id)
{
/**************************************
 *
 *	g d s _ $ c r e a t e _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Open an existing blob.
 *
 **************************************/

	return open_blob(user_status, db_handle, tra_handle, blob_handle, blob_id, 0, 0, true);
}


ISC_STATUS API_ROUTINE isc_create_blob2(ISC_STATUS* user_status,
										FB_API_HANDLE* db_handle,
										FB_API_HANDLE* tra_handle,
										FB_API_HANDLE* blob_handle,
										ISC_QUAD* blob_id,
										SSHORT bpb_length,
										const UCHAR* bpb)
{
/**************************************
 *
 *	g d s _ $ c r e a t e _ b l o b 2
 *
 **************************************
 *
 * Functional description
 *	Open an existing blob.
 *
 **************************************/

	return open_blob(user_status, db_handle, tra_handle, blob_handle, blob_id, bpb_length, bpb, true);
}



ISC_STATUS API_ROUTINE isc_create_database(ISC_STATUS* user_status,
										   USHORT file_length,
										   const TEXT* file_name,
										   FB_API_HANDLE* public_handle,
										   SSHORT dpb_length,
										   const UCHAR* dpb,
										   USHORT /*db_type*/)
{
/**************************************
 *
 *	g d s _ $ c r e a t e _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Create a nice, squeaky clean database, uncorrupted by user data.
 *
 **************************************/
	StatusVector temp(NULL);
	Attachment attachment(0);

	StatusVector status(user_status);

	try
	{
		YEntry entryGuard;

		nullCheck(public_handle, isc_bad_db_handle);

		if (shutdownStarted)
		{
			status_exception::raise(Arg::Gds(isc_att_shutdown));
		}

		if (!file_name)
		{
			status_exception::raise(Arg::Gds(isc_bad_db_format) << Arg::Str(""));
		}

		if (dpb_length > 0 && !dpb)
		{
			status_exception::raise(Arg::Gds(isc_bad_dpb_form));
		}

		StatusVector* ptr = &status;

		// copy the file name to a temp buffer, since some of the following utilities can modify it

		PathName org_filename(file_name, file_length ? file_length : strlen(file_name));
		ClumpletWriter newDpb(ClumpletReader::dpbList, MAX_DPB_SIZE, dpb, dpb_length);

		bool utfFilename = newDpb.find(isc_dpb_utf8_filename);

		if (utfFilename)
			ISC_utf8ToSystem(org_filename);
		else
			newDpb.insertTag(isc_dpb_utf8_filename);

		setLogin(newDpb);
		org_filename.rtrim();

		PathName expanded_filename;
		bool unescaped = false;

		if (!set_path(org_filename, expanded_filename))
		{
			expanded_filename = org_filename;
			ISC_systemToUtf8(expanded_filename);
			ISC_unescape(expanded_filename);
			unescaped = true;
			ISC_utf8ToSystem(expanded_filename);
			ISC_expand_filename(expanded_filename, true);
		}

		ISC_systemToUtf8(org_filename);
		ISC_systemToUtf8(expanded_filename);

		if (unescaped)
			ISC_escape(expanded_filename);

		if (org_filename != expanded_filename && !newDpb.find(isc_dpb_org_filename))
		{
			newDpb.insertPath(isc_dpb_org_filename, org_filename);
		}

		for (GetPlugins<PProvider, NoEntrypoint> providerIterator(PluginType::Provider, FB_P_PROVIDER_VERSION);
			 providerIterator.hasData(); providerIterator.next())
		{
			PProvider* p = providerIterator.plugin();
			attachment = new CAttachment(NULL, public_handle, p);
#ifdef WIN_NT
			attachment->db_path = expanded_filename;
#else
			attachment->db_path = org_filename;
#endif

			p->createDatabase(ptr, &attachment->providerInterface, *public_handle, expanded_filename.c_str(),
							  newDpb.getBufferLength(), newDpb.getBuffer());
			if (ptr->isSuccess())
			{
#ifdef WIN_NT
            	// Now we can expand, the file exists
				expanded_filename = org_filename;
				ISC_unescape(expanded_filename);
				ISC_utf8ToSystem(expanded_filename);
				ISC_expand_filename(expanded_filename, true);
				ISC_systemToUtf8(expanded_filename);
#endif

				status[0] = isc_arg_gds;
				status[1] = 0;
				if (status[2] != isc_arg_warning)
					status[2] = isc_arg_end;

				return status[1];
			}
			else
			{
				*public_handle = 0;
				destroy(attachment);
				attachment = 0;
			}

			if ((*ptr)[1] != isc_unavailable)
			{
				ptr = &temp;
			}
		}

		if (status[1] == 0)
		{
			Arg::Gds(isc_unavailable).raise();
		}
	}
	catch (const Exception& e)
	{
		if (attachment)
		{
			if (attachment->providerInterface)
			{
				attachment->providerInterface->drop(&temp);
			}

			destroyNoThrow(attachment);
		}
  		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_database_cleanup(ISC_STATUS* user_status,
											 FB_API_HANDLE* handle,
											 AttachmentCleanupRoutine * routine,
											 void* arg)
{
/**************************************
 *
 *	g d s _ $ d a t a b a s e _ c l e a n u p
 *
 **************************************
 *
 * Functional description
 *	Register an attachment specific cleanup handler.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Attachment attachment = translate<CAttachment>(handle);
		YEntry entryGuard(status, attachment);

		attachment->cleanup.add(routine, arg);
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_database_info(ISC_STATUS* user_status,
										 FB_API_HANDLE* handle,
										 SSHORT item_length,
										 const SCHAR* items,
										 SSHORT buffer_length,
										 SCHAR* buffer)
{
/**************************************
 *
 *	g d s _ $ d a t a b a s e _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Provide information on database object.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Attachment attachment = translate<CAttachment>(handle);
		YEntry entryGuard(status, attachment);
		attachment->providerInterface->getInfo(&status,
									item_length, reinterpret_cast<const unsigned char*>(items),
									buffer_length, reinterpret_cast<unsigned char*>(buffer));
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_ddl(ISC_STATUS* user_status,
							   FB_API_HANDLE* db_handle,
							   FB_API_HANDLE* tra_handle,
							   SSHORT length,
							   const UCHAR* ddl)
{
/**************************************
 *
 *	g d s _ $ d d l
 *
 **************************************
 *
 * Functional description
 *	Do meta-data update.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Attachment attachment = translate<CAttachment>(db_handle);
		YEntry entryGuard(status, attachment);
		Transaction transaction = findTransaction(tra_handle, attachment);

		transaction->providerInterface->ddl(&status, length, ddl);
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_detach_database(ISC_STATUS* user_status,
										   FB_API_HANDLE* handle)
{
/**************************************
 *
 *	g d s _ d e t a c h
 *
 **************************************
 *
 * Functional description
 *	Close down an attachment.
 *
 **************************************/
	return detach_or_drop_database(user_status, handle, false);
}


static ISC_STATUS detach_or_drop_database(ISC_STATUS* user_status, FB_API_HANDLE* handle,
										  bool dropping, const ISC_STATUS specCode)
{
/**************************************
 *
 *	d e t a c h _ o r _ d r o p _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Common code for that calls.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		YEntry entryGuard;
		Attachment attachment = translate<CAttachment>(handle, dropping);

		{
			MutexLockGuard guard(*attachment->getProviderInterfaceCleanupMutex());

			if (attachment->providerInterface)
			{
				if (dropping)
					attachment->providerInterface->drop(&status);
				else
					attachment->providerInterface->detach(&status);
				if ((!status.isSuccess()) && status[1] != specCode)
				{
					return status[1];
				}
				attachment->providerInterface = NULL;
			}
		}

		destroy(attachment);
		*handle = 0;
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}

int API_ROUTINE gds__disable_subsystem(TEXT* subsystem)
{
/**************************************
 *
 *	g d s _ $ d i s a b l e _ s u b s y s t e m
 *
 **************************************
 *
 * Functional description
 *	Disable access to a specific subsystem.  If no subsystem
 *	has been explicitly disabled, all are available.
 *
 **************************************/
	return FALSE;
}


ISC_STATUS API_ROUTINE isc_drop_database(ISC_STATUS* user_status,
										 FB_API_HANDLE* handle)
{
/**************************************
 *
 *	i s c _ d r o p _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Close down a database and then purge it.
 *
 **************************************/
	return detach_or_drop_database(user_status, handle, true, isc_drdb_completed_with_errs);
}


ISC_STATUS API_ROUTINE isc_dsql_alloc_statement(ISC_STATUS* user_status,
									  FB_API_HANDLE* db_handle,
									  FB_API_HANDLE* stmt_handle)
{
/**************************************
 *
 *	i s c _ d s q l _ a l l o c _ s t a t e m e n t
 *
 **************************************/

	return isc_dsql_allocate_statement(user_status, db_handle, stmt_handle);
}


ISC_STATUS API_ROUTINE isc_dsql_alloc_statement2(ISC_STATUS* user_status,
									   FB_API_HANDLE* db_handle,
									   FB_API_HANDLE* stmt_handle)
{
/**************************************
 *
 *	i s c _ d s q l _ a l l o c _ s t a t e m e n t 2
 *
 **************************************
 *
 * Functional description
 *	Allocate a statement handle.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		if (isc_dsql_allocate_statement(status, db_handle, stmt_handle))
		{
			return status[1];
		}

		Statement statement = translate<CStatement>(stmt_handle);
		statement->user_handle = stmt_handle;
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_dsql_allocate_statement(ISC_STATUS* user_status,
										 FB_API_HANDLE* db_handle,
										 FB_API_HANDLE* public_stmt_handle)
{
/**************************************
 *
 *	i s c _ d s q l _ a l l o c a t e _ s t a t e m e n t
 *
 **************************************
 *
 * Functional description
 *	Allocate a statement handle.
 *
 **************************************/
	StatusVector status(user_status);
	Attachment attachment(NULL);
	IStatement* stmt_handle = NULL;

	try
	{
		attachment = translate<CAttachment>(db_handle);
		YEntry entryGuard(status, attachment);
		// check the statement handle to make sure it's NULL and then initialize it.
		nullCheck(public_stmt_handle, isc_bad_stmt_handle);

		stmt_handle = attachment->providerInterface->allocateStatement(&status);
		if (!status.isSuccess())
		{
			return status[1];
		}

		//Statement statement =
		new CStatement(stmt_handle, public_stmt_handle, attachment);
	}
	catch (const Exception& e)
	{
		if (stmt_handle)
		{
			*public_stmt_handle = 0;
			stmt_handle->free(&status, DSQL_drop);
		}
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_dsql_describe(ISC_STATUS* user_status,
										 FB_API_HANDLE* stmt_handle,
										 USHORT dialect,
										 XSQLDA* sqlda)
{
/**************************************
 *
 *	i s c _ d s q l _ d e s c r i b e
 *
 **************************************
 *
 * Functional description
 *	Describe output parameters for a prepared statement.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Statement statement = translate<CStatement>(stmt_handle);

		statement->checkPrepared();
		sqlda_sup::dasup_clause& clause = statement->das.dasup_clauses[DASUP_CLAUSE_select];

		if (clause.dasup_info_len && clause.dasup_info_buf)
		{
			iterative_sql_info(	status,
								stmt_handle,
								sizeof(describe_select_info),
								describe_select_info,
								clause.dasup_info_len,
								clause.dasup_info_buf,
								dialect,
								sqlda);
		}
		else
		{
			HalfStaticArray<SCHAR, DESCRIBE_BUFFER_SIZE> local_buffer;
			const USHORT buffer_len = sqlda_buffer_size(DESCRIBE_BUFFER_SIZE, sqlda, dialect);
			SCHAR *buffer = local_buffer.getBuffer(buffer_len);

			if (!isc_dsql_sql_info(	status,
									stmt_handle,
									sizeof(describe_select_info),
									describe_select_info,
									buffer_len,
									buffer))
			{
				iterative_sql_info(	status,
									stmt_handle,
									sizeof(describe_select_info),
									describe_select_info,
									buffer_len,
									buffer,
									dialect,
									sqlda);
			}
		}
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_dsql_describe_bind(ISC_STATUS* user_status,
											  FB_API_HANDLE* stmt_handle,
											  USHORT dialect,
											  XSQLDA* sqlda)
{
/**************************************
 *
 *	i s c _ d s q l _ d e s c r i b e _ b i n d
 *
 **************************************
 *
 * Functional description
 *	Describe input parameters for a prepared statement.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Statement statement = translate<CStatement>(stmt_handle);

		sqlda_sup::dasup_clause& clause = statement->das.dasup_clauses[DASUP_CLAUSE_bind];

		if (clause.dasup_info_len && clause.dasup_info_buf)
		{
			iterative_sql_info(	status,
								stmt_handle,
								sizeof(describe_bind_info),
								describe_bind_info,
								clause.dasup_info_len,
								clause.dasup_info_buf,
								dialect,
								sqlda);
		}
		else
		{
			HalfStaticArray<SCHAR, DESCRIBE_BUFFER_SIZE> local_buffer;
			const USHORT buffer_len = sqlda_buffer_size(DESCRIBE_BUFFER_SIZE, sqlda, dialect);
			SCHAR *buffer = local_buffer.getBuffer(buffer_len);

			if (!isc_dsql_sql_info(	status,
									stmt_handle,
									sizeof(describe_bind_info),
									describe_bind_info,
									buffer_len,
									buffer))
			{
				iterative_sql_info(	status,
									stmt_handle,
									sizeof(describe_bind_info),
									describe_bind_info,
									buffer_len,
									buffer,
									dialect,
									sqlda);
			}
		}
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_dsql_execute(ISC_STATUS* user_status,
										FB_API_HANDLE* tra_handle,
										FB_API_HANDLE* stmt_handle,
										USHORT dialect,
										const XSQLDA* sqlda)
{
/**************************************
 *
 *	i s c _ d s q l _ e x e c u t e
 *
 **************************************
 *
 * Functional description
 *	Execute a non-SELECT dynamic SQL statement.
 *
 **************************************/

	return isc_dsql_execute2(user_status, tra_handle, stmt_handle, dialect,
							 sqlda, NULL);
}


ISC_STATUS API_ROUTINE isc_dsql_execute2(ISC_STATUS* user_status,
										 FB_API_HANDLE* tra_handle,
										 FB_API_HANDLE* stmt_handle,
										 USHORT dialect,
										 const XSQLDA* in_sqlda,
										 const XSQLDA* out_sqlda)
{
/**************************************
 *
 *	i s c _ d s q l _ e x e c u t e 2
 *
 **************************************
 *
 * Functional description
 *	Execute a non-SELECT dynamic SQL statement.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		USHORT in_blr_length, in_msg_type, in_msg_length,
			out_blr_length, out_msg_type, out_msg_length;

		Statement statement = translate<CStatement>(stmt_handle);

		sqlda_sup* dasup = &(statement->das);
		statement->checkPrepared();

		if (UTLD_parse_sqlda(status, dasup, &in_blr_length, &in_msg_type, &in_msg_length,
							 dialect, in_sqlda, DASUP_CLAUSE_bind))
		{
			return status[1];
		}

		if (UTLD_parse_sqlda(status, dasup, &out_blr_length, &out_msg_type, &out_msg_length,
							 dialect, out_sqlda, DASUP_CLAUSE_select))
		{
			return status[1];
		}

		if (isc_dsql_execute2_m(status, tra_handle, stmt_handle,
								in_blr_length,
								dasup->dasup_clauses[DASUP_CLAUSE_bind].dasup_blr,
								in_msg_type, in_msg_length,
								dasup->dasup_clauses[DASUP_CLAUSE_bind].dasup_msg,
								out_blr_length,
								dasup->dasup_clauses[DASUP_CLAUSE_select].
								dasup_blr, out_msg_type, out_msg_length,
								dasup->dasup_clauses[DASUP_CLAUSE_select].
								dasup_msg))
		{
			return status[1];
		}

		if (UTLD_parse_sqlda(status, dasup, NULL, NULL, NULL, dialect, out_sqlda, DASUP_CLAUSE_select))
		{
			return status[1];
		}
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_dsql_execute_m(ISC_STATUS* user_status,
										  FB_API_HANDLE* tra_handle,
										  FB_API_HANDLE* stmt_handle,
										  USHORT blr_length,
										  const SCHAR* blr,
										  USHORT msg_type,
										  USHORT msg_length,
										  SCHAR* msg)
{
/**************************************
 *
 *	i s c _ d s q l _ e x e c u t e _ m
 *
 **************************************
 *
 * Functional description
 *	Execute a non-SELECT dynamic SQL statement.
 *
 **************************************/

	return isc_dsql_execute2_m(user_status, tra_handle, stmt_handle, blr_length, blr,
							   msg_type, msg_length, msg, 0, NULL, 0, 0, NULL);
}


ISC_STATUS API_ROUTINE isc_dsql_execute2_m(ISC_STATUS* user_status,
										   FB_API_HANDLE* tra_handle,
										   FB_API_HANDLE* stmt_handle,
										   USHORT in_blr_length,
										   const SCHAR* in_blr,
										   USHORT in_msg_type,
										   USHORT in_msg_length,
										   const SCHAR* in_msg,
										   USHORT out_blr_length,
										   SCHAR* out_blr,
										   USHORT out_msg_type,
										   USHORT out_msg_length,
										   SCHAR* out_msg)
{
/**************************************
 *
 *	i s c _ d s q l _ e x e c u t e 2 _ m
 *
 **************************************
 *
 * Functional description
 *	Execute a non-SELECT dynamic SQL statement.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Statement statement = translate<CStatement>(stmt_handle);
		YEntry entryGuard(status, statement);

		Transaction transaction(NULL);
		ITransaction* tra = NULL;

		if (tra_handle && *tra_handle)
		{
			transaction = translate<CTransaction>(tra_handle);
			Transaction t = find_transaction(statement->parent, transaction);
			if (!t)
			{
				bad_handle(isc_bad_trans_handle);
			}
			tra = t->providerInterface;
		}

		Message inMessage(in_blr_length, reinterpret_cast<const unsigned char*>(in_blr), in_msg_length);
		MessageBuffer inMsgBuffer(&inMessage, reinterpret_cast<const unsigned char*>(in_msg));
		Message outMessage(out_blr_length, reinterpret_cast<unsigned char*>(out_blr), out_msg_length);
		MessageBuffer outMsgBuffer(&outMessage, reinterpret_cast<unsigned char*>(out_msg));

		tra = statement->providerInterface->executeMessage(&status, tra, in_msg_type, &inMsgBuffer,
			out_msg_type, &outMsgBuffer);

		if (status.isSuccess())
		{
			if (transaction && !tra)
			{
				destroy(transaction);
				*tra_handle = 0;
			}
			else if (!transaction && tra)
			{
				transaction = new CTransaction(tra, tra_handle, statement->parent);
			}
		}
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


// Is this really API function? Where is it declared?
ISC_STATUS API_ROUTINE isc_dsql_exec_immediate(ISC_STATUS* user_status,
										   FB_API_HANDLE* db_handle,
										   FB_API_HANDLE* tra_handle,
										   USHORT length,
										   const SCHAR* string,
										   USHORT dialect,
										   const XSQLDA* sqlda)
{
/**************************************
 *
 *	i s c _ d s q l _ e x e c _ i m m e d
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return isc_dsql_execute_immediate(user_status, db_handle, tra_handle, length, string,
								  dialect, sqlda);
}


ISC_STATUS API_ROUTINE isc_dsql_execute_immediate(ISC_STATUS* user_status,
											  FB_API_HANDLE* db_handle,
											  FB_API_HANDLE* tra_handle,
											  USHORT length,
											  const SCHAR* string,
											  USHORT dialect,
											  const XSQLDA* sqlda)
{
/**************************************
 *
 *	i s c _ d s q l _ e x e c u t e _ i m m e d i a t e
 *
 **************************************
 *
 * Functional description
 *	Prepare a statement for execution.
 *
 **************************************/

	return isc_dsql_exec_immed2(user_status, db_handle, tra_handle, length, string,
								dialect, sqlda, NULL);
}


ISC_STATUS API_ROUTINE isc_dsql_exec_immed2(ISC_STATUS* user_status,
											FB_API_HANDLE* db_handle,
											FB_API_HANDLE* tra_handle,
											USHORT length,
											const SCHAR* string,
											USHORT dialect,
											const XSQLDA* in_sqlda,
											const XSQLDA* out_sqlda)
{
/**************************************
 *
 *	i s c _ d s q l _ e x e c _ i m m e d 2
 *
 **************************************
 *
 * Functional description
 *	Prepare a statement for execution.
 *
 **************************************/
	StatusVector status(user_status);
	ISC_STATUS s = 0;
	sqlda_sup dasup;
	memset(&dasup, 0, sizeof(sqlda_sup));

	try
	{
		if (!string)
		{
			Arg::Gds(isc_command_end_err).raise();
		}

		USHORT in_blr_length, in_msg_type, in_msg_length,
			out_blr_length, out_msg_type, out_msg_length;

		if (UTLD_parse_sqlda(status, &dasup, &in_blr_length, &in_msg_type, &in_msg_length,
							 dialect, in_sqlda, DASUP_CLAUSE_bind))
		{
			return status[1];
		}

		if (UTLD_parse_sqlda(status, &dasup, &out_blr_length, &out_msg_type, &out_msg_length,
							 dialect, out_sqlda, DASUP_CLAUSE_select))
		{
			return status[1];
		}

		s = isc_dsql_exec_immed2_m(status, db_handle, tra_handle,
								 length, string, dialect,
								 in_blr_length,
								 dasup.dasup_clauses[DASUP_CLAUSE_bind].dasup_blr,
								 in_msg_type, in_msg_length,
								 dasup.dasup_clauses[DASUP_CLAUSE_bind].dasup_msg,
								 out_blr_length,
								 dasup.dasup_clauses[DASUP_CLAUSE_select].dasup_blr,
								 out_msg_type, out_msg_length,
								 dasup.dasup_clauses[DASUP_CLAUSE_select].dasup_msg);
		if (!s)
		{
			s =	UTLD_parse_sqlda(status, &dasup, NULL, NULL, NULL, dialect,
								 out_sqlda, DASUP_CLAUSE_select);
		}
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
		s = status[1];
	}

	release_dsql_support(dasup);
	return s;
}


// Is this really API function? Where is it declared?
ISC_STATUS API_ROUTINE isc_dsql_exec_immediate_m(ISC_STATUS* user_status,
										   FB_API_HANDLE* db_handle,
										   FB_API_HANDLE* tra_handle,
										   USHORT length,
										   const SCHAR* string,
										   USHORT dialect,
										   USHORT blr_length,
										   USHORT msg_type,
										   USHORT msg_length,
										   SCHAR* blr,
										   SCHAR* msg)
{
/**************************************
 *
 *	i s c _ d s q l _ e x e c _ i m m e d _ m
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return isc_dsql_execute_immediate_m(user_status, db_handle, tra_handle,
								  length, string, dialect, blr_length, blr,
								  msg_type, msg_length, msg);
}


ISC_STATUS API_ROUTINE isc_dsql_execute_immediate_m(ISC_STATUS* user_status,
											  FB_API_HANDLE* db_handle,
											  FB_API_HANDLE* tra_handle,
											  USHORT length,
											  const SCHAR* string,
											  USHORT dialect,
											  USHORT blr_length,
											  SCHAR* blr,
											  USHORT msg_type,
											  USHORT msg_length,
											  SCHAR* msg)
{
/**************************************
 *
 *	i s c _ d s q l _ e x e c u t e _ i m m e d i a t e _ m
 *
 **************************************
 *
 * Functional description
 *	Prepare a statement for execution.
 *
 **************************************/

	return isc_dsql_exec_immed2_m(user_status, db_handle, tra_handle,
								length, string, dialect, blr_length, blr,
								msg_type, msg_length, msg, 0, NULL, 0, 0, NULL);
}


ISC_STATUS API_ROUTINE isc_dsql_exec_immed2_m(ISC_STATUS* user_status,
											FB_API_HANDLE* db_handle,
											FB_API_HANDLE* tra_handle,
											USHORT length,
											const SCHAR* string,
											USHORT dialect,
											USHORT in_blr_length,
											SCHAR* in_blr,
											USHORT in_msg_type,
											USHORT in_msg_length,
											const SCHAR* in_msg,
											USHORT out_blr_length,
											SCHAR* out_blr,
											USHORT out_msg_type,
											USHORT out_msg_length,
											SCHAR* out_msg)
{
/**************************************
 *
 *	i s c _ d s q l _ e x e c _ i m m 2 _ m
 *
 **************************************
 *
 * Functional description
 *	Prepare a statement for execution.
 *
 **************************************/
	StatusVector status(user_status);

	bool stmt_eaten;
	if (PREPARSE_execute(status, db_handle, tra_handle, length, string, &stmt_eaten, dialect))
	{
		if (status[1])
		{
			return status[1];
		}

		ISC_STATUS_ARRAY temp_status;
		FB_API_HANDLE crdb_trans_handle = 0;
		if (isc_start_transaction(status, &crdb_trans_handle, 1, db_handle, 0, 0))
		{
			save_error_string(status);
			isc_drop_database(temp_status, db_handle);
			*db_handle = 0;
			return status[1];
		}

		bool ret_v3_error = false;
		if (!stmt_eaten) {
			// Check if against < 4.0 database

			const SCHAR ch = isc_info_base_level;
			SCHAR buffer[16];
			if (!isc_database_info(status, db_handle, 1, &ch, sizeof(buffer), buffer))
			{
				if ((buffer[0] != isc_info_base_level) || (buffer[4] > 3))
				{
					isc_dsql_exec_immed3_m(status, db_handle, &crdb_trans_handle, length, string,
										 dialect, in_blr_length, in_blr,
										 in_msg_type, in_msg_length, in_msg,
										 out_blr_length, out_blr,
										 out_msg_type, out_msg_length, out_msg);
				}
				else
					ret_v3_error = true;
			}
		}

		if (status[1]) {
			isc_rollback_transaction(temp_status, &crdb_trans_handle);
			save_error_string(status);
			isc_drop_database(temp_status, db_handle);
			*db_handle = 0;
			return status[1];
		}

		if (isc_commit_transaction(status, &crdb_trans_handle)) {
			isc_rollback_transaction(temp_status, &crdb_trans_handle);
			save_error_string(status);
			isc_drop_database(temp_status, db_handle);
			*db_handle = 0;
			return status[1];
		}

		if (ret_v3_error) {
			Arg::Gds(isc_srvr_version_too_old).copyTo(status);
			return status[1];
		}

		return status[1];
	}

	return isc_dsql_exec_immed3_m(user_status, db_handle, tra_handle, length, string, dialect,
								in_blr_length, in_blr, in_msg_type, in_msg_length, in_msg,
								out_blr_length, out_blr, out_msg_type, out_msg_length, out_msg);
}


ISC_STATUS API_ROUTINE isc_dsql_exec_immed3_m(ISC_STATUS* user_status,
											FB_API_HANDLE* db_handle,
											FB_API_HANDLE* tra_handle,
											USHORT length,
											const SCHAR* string,
											USHORT dialect,
											USHORT in_blr_length,
											const SCHAR* in_blr,
											USHORT in_msg_type,
											USHORT in_msg_length,
											const SCHAR* in_msg,
											USHORT out_blr_length,
											SCHAR* out_blr,
											USHORT out_msg_type,
											USHORT out_msg_length,
											SCHAR* out_msg)
{
/**************************************
 *
 *	i s c _ d s q l _ e x e c _ i m m 3 _ m
 *
 **************************************
 *
 * Functional description
 *	Prepare a statement for execution.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		if (!string)
		{
			Arg::Gds(isc_command_end_err).raise();
		}

		Attachment attachment = translate<CAttachment>(db_handle);
		YEntry entryGuard(status, attachment);

		Transaction transaction(NULL);
		ITransaction* tra = NULL;

		if (tra_handle && *tra_handle)
		{
			transaction = translate<CTransaction>(tra_handle);
			Transaction t = find_transaction(attachment, transaction);
			if (!t)
			{
				bad_handle(isc_bad_trans_handle);
			}
			tra = t->providerInterface;
		}

		Message inMessage(in_blr_length, reinterpret_cast<const unsigned char*>(in_blr), in_msg_length);
		MessageBuffer inMsgBuffer(&inMessage, reinterpret_cast<const unsigned char*>(in_msg));
		Message outMessage(out_blr_length, reinterpret_cast<unsigned char*>(out_blr), out_msg_length);
		MessageBuffer outMsgBuffer(&outMessage, reinterpret_cast<unsigned char*>(out_msg));

		tra = attachment->providerInterface->execute(&status, tra, length, string, dialect,
										 in_msg_type, &inMsgBuffer, out_msg_type, &outMsgBuffer);

		if (status.isSuccess())
		{
			if (transaction && !tra)
			{
				destroy(transaction);
				*tra_handle = 0;
			}
			else if (!transaction && tra)
			{
				transaction = new CTransaction(tra, tra_handle, attachment);
			}
		}
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_dsql_fetch(ISC_STATUS* user_status,
									  FB_API_HANDLE* stmt_handle,
									  USHORT dialect,
									  const XSQLDA* sqlda)
{
/**************************************
 *
 *	i s c _ d s q l _ f e t c h
 *
 **************************************
 *
 * Functional description
 *	Fetch next record from a dynamic SQL cursor
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		if (!sqlda)
		{
			status_exception::raise(Arg::Gds(isc_dsql_sqlda_err));
		}

		Statement statement = translate<CStatement>(stmt_handle);

		statement->checkPrepared();
		sqlda_sup& dasup = statement->das;

		USHORT blr_length, msg_type, msg_length;
		if (UTLD_parse_sqlda(status, &dasup, &blr_length, &msg_type, &msg_length,
							 dialect, sqlda, DASUP_CLAUSE_select))
		{
			return status[1];
		}

		ISC_STATUS s = isc_dsql_fetch_m(status, stmt_handle, blr_length,
								dasup.dasup_clauses[DASUP_CLAUSE_select].dasup_blr,
								0, msg_length,
								dasup.dasup_clauses[DASUP_CLAUSE_select].dasup_msg);
		if (s && s != 101)
		{
			return s;
		}

		if (UTLD_parse_sqlda(status, &dasup, NULL, NULL, NULL, dialect, sqlda, DASUP_CLAUSE_select))
		{
			return status[1];
		}
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_dsql_fetch_m(ISC_STATUS* user_status,
										FB_API_HANDLE* stmt_handle,
										USHORT blr_length,
										SCHAR* blr,
										USHORT msg_type,
										USHORT msg_length,
										SCHAR* msg)
{
/**************************************
 *
 *	i s c _ d s q l _ f e t c h _ m
 *
 **************************************
 *
 * Functional description
 *	Fetch next record from a dynamic SQL cursor
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Statement statement = translate<CStatement>(stmt_handle);
		YEntry entryGuard(status, statement);
		Message message(blr_length, reinterpret_cast<unsigned char*>(blr), msg_length);
		MessageBuffer msgBuffer(&message, reinterpret_cast<unsigned char*>(msg));

		int s = statement->providerInterface->fetchMessage(&status, msg_type, &msgBuffer);

		if (s == 100 || s == 101)
		{
			return s;
		}
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_dsql_free_statement(ISC_STATUS* user_status,
									 FB_API_HANDLE* stmt_handle,
									 USHORT option)
{
/*****************************************
 *
 *	i s c _ d s q l _ f r e e _ s t a t e m e n t
 *
 *****************************************
 *
 * Functional Description
 *	release request for an sql statement
 *
 *****************************************/
	StatusVector status(user_status);

	try
	{
		Statement statement = translate<CStatement>(stmt_handle);
		YEntry entryGuard(status, statement);

		{
			MutexLockGuard guard(*statement->getProviderInterfaceCleanupMutex());

			if (statement->providerInterface)
			{
				statement->providerInterface->free(&status, option);
				if (!status.isSuccess())
				{
					return status[1];
				}
				if (option & DSQL_drop)
				{
					statement->providerInterface = NULL;
				}
			}
		}

		if (option & DSQL_drop)
		{
			release_dsql_support(statement->das);
			destroy(statement);
			*stmt_handle = 0;
		}
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_dsql_insert(ISC_STATUS* user_status,
									   FB_API_HANDLE* stmt_handle,
									   USHORT dialect,
									   XSQLDA* sqlda)
{
/**************************************
 *
 *	i s c _ d s q l _ i n s e r t
 *
 **************************************
 *
 * Functional description
 *	Insert next record into a dynamic SQL cursor
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Statement statement = translate<CStatement>(stmt_handle);

		statement->checkPrepared();
		sqlda_sup& dasup = statement->das;
		USHORT blr_length, msg_type, msg_length;
		if (UTLD_parse_sqlda(status, &dasup, &blr_length, &msg_type, &msg_length,
							 dialect, sqlda, DASUP_CLAUSE_bind))
		{
			return status[1];
		}

		return isc_dsql_insert_m(status, stmt_handle, blr_length,
								 dasup.dasup_clauses[DASUP_CLAUSE_bind].
								 dasup_blr, 0, msg_length,
								 dasup.dasup_clauses[DASUP_CLAUSE_bind].
								 dasup_msg);
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_dsql_insert_m(ISC_STATUS* user_status,
										 FB_API_HANDLE* stmt_handle,
										 USHORT blr_length,
										 const SCHAR* blr,
										 USHORT msg_type,
										 USHORT msg_length,
										 const SCHAR* msg)
{
/**************************************
 *
 *	i s c _ d s q l _ i n s e r t _ m
 *
 **************************************
 *
 * Functional description
 *	Insert next record into a dynamic SQL cursor
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Statement statement = translate<CStatement>(stmt_handle);
		YEntry entryGuard(status, statement);

		statement->checkPrepared();
		//sqlda_sup& dasup = statement->das;

		Message message(blr_length, reinterpret_cast<const unsigned char*>(blr), msg_length);
		MessageBuffer msgBuffer(&message, reinterpret_cast<const unsigned char*>(msg));

		statement->providerInterface->insertMessage(&status, msg_type, &msgBuffer);
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_dsql_prepare(ISC_STATUS* user_status,
										FB_API_HANDLE* tra_handle,
										FB_API_HANDLE* stmt_handle,
										USHORT length,
										const SCHAR* string,
										USHORT dialect,
										XSQLDA* sqlda)
{
/**************************************
 *
 *	i s c _ d s q l _ p r e p a r e
 *
 **************************************
 *
 * Functional description
 *	Prepare a statement for execution.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Statement statement = translate<CStatement>(stmt_handle);
		sqlda_sup& dasup = statement->das;

		const USHORT buffer_len = sqlda_buffer_size(PREPARE_BUFFER_SIZE, sqlda, dialect);
		//Attachment attachment = statement->parent;
		Array<SCHAR> db_prepare_buffer;
		SCHAR* const buffer = db_prepare_buffer.getBuffer(buffer_len);

		if (!isc_dsql_prepare_m(status,
								tra_handle,
								stmt_handle,
								length,
								string,
								dialect,
								sizeof(sql_prepare_info2),
								sql_prepare_info2,
								buffer_len,
								buffer))
		{
			statement->flags &= ~HANDLE_STATEMENT_prepared;
			release_dsql_support(dasup);
			memset(&dasup, 0, sizeof(dasup));

			dasup.dasup_dialect = dialect;

			SCHAR* p = buffer;

			dasup.dasup_stmt_type = 0;
			if (*p == isc_info_sql_stmt_type)
			{
				const USHORT len = gds__vax_integer((UCHAR*)p + 1, 2);
				dasup.dasup_stmt_type = gds__vax_integer((UCHAR*)p + 3, len);
				p += 3 + len;
			}

			sqlda_sup::dasup_clause &das_select = dasup.dasup_clauses[DASUP_CLAUSE_select];
			sqlda_sup::dasup_clause &das_bind = dasup.dasup_clauses[DASUP_CLAUSE_bind];
			das_select.dasup_info_buf = das_bind.dasup_info_buf = 0;
			das_select.dasup_info_len = das_bind.dasup_info_len = 0;

			SCHAR* buf_select = 0; // pointer in the buffer where isc_info_sql_select starts
			USHORT len_select = 0; // length of isc_info_sql_select part
			if (*p == isc_info_sql_select)
			{
				das_select.dasup_info_buf = p;
				buf_select = p;
				len_select = buffer_len - (buf_select - buffer);
			}

			das_bind.dasup_info_buf = UTLD_skip_sql_info(p);

			p = das_select.dasup_info_buf;
			if (p)
			{
				SCHAR* p2 = das_bind.dasup_info_buf;
				if (p2)
				{
					const SLONG len =  p2 - p;

					p2 = alloc(len + 1);
					memmove(p2, p, len);
					p2[len] = isc_info_end;
					das_select.dasup_info_buf = p2;
					das_select.dasup_info_len = len + 1;

					buf_select = das_select.dasup_info_buf;
					len_select = das_select.dasup_info_len;
				}
				else
				{
					das_select.dasup_info_buf = 0;
					das_select.dasup_info_len = 0;
				}
			}

			p = das_bind.dasup_info_buf;
			if (p)
			{
				SCHAR* p2 = UTLD_skip_sql_info(p);
				if (p2)
				{
					const SLONG len =  p2 - p;

					p2 = alloc(len + 1);
					memmove(p2, p, len);
					p2[len] = isc_info_end;
					das_bind.dasup_info_buf = p2;
					das_bind.dasup_info_len = len + 1;
				}
				else
				{
					das_bind.dasup_info_buf = 0;
					das_bind.dasup_info_len = 0;
				}
			}

			iterative_sql_info(status, stmt_handle, sizeof(sql_prepare_info),
				sql_prepare_info, len_select, buf_select, dialect, sqlda);

			// statement prepared OK
			statement->flags |= HANDLE_STATEMENT_prepared;
		}
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_dsql_prepare_m(ISC_STATUS* user_status,
										  FB_API_HANDLE* tra_handle,
										  FB_API_HANDLE* stmt_handle,
										  USHORT length,
										  const SCHAR* string,
										  USHORT dialect,
										  USHORT item_length,
										  const SCHAR* items,
										  USHORT buffer_length,
										  SCHAR* buffer)
{
/**************************************
 *
 *	i s c _ d s q l _ p r e p a r e _ m
 *
 **************************************
 *
 * Functional description
 *	Prepare a statement for execution.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		if (!string)
		{
			Arg::Gds(isc_command_end_err).raise();
		}

		Statement statement = translate<CStatement>(stmt_handle);
		YEntry entryGuard(status, statement);

		ITransaction* tra = NULL;

		if (tra_handle && *tra_handle)
		{
			Transaction transaction = translate<CTransaction>(tra_handle);
			transaction = find_transaction(statement->parent, transaction);
			if (!transaction)
			{
				bad_handle(isc_bad_trans_handle);
			}
			tra = transaction->providerInterface;
		}

		IStatement* newStatement = statement->providerInterface->prepare(&status, tra, length, string,
			dialect, item_length, reinterpret_cast<const unsigned char*>(items),
			buffer_length, reinterpret_cast<unsigned char*>(buffer));

		if (status.isSuccess())
		{
			statement->providerInterface = newStatement;
		}
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_dsql_set_cursor_name(ISC_STATUS* user_status,
										   FB_API_HANDLE* stmt_handle,
										   const SCHAR* cursorName,
										   USHORT cursorType)
{
/**************************************
 *
 *	i s c _ d s q l _ s e t _ c u r s o r
 *
 **************************************
 *
 * Functional description
 *	Set a cursor name for a dynamic request.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Statement statement = translate<CStatement>(stmt_handle);
		YEntry entryGuard(status, statement);

		statement->providerInterface->setCursor(&status, cursorName, cursorType);
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_dsql_sql_info(ISC_STATUS* user_status,
										 FB_API_HANDLE* stmt_handle,
										 SSHORT item_length,
										 const SCHAR* items,
										 SSHORT buffer_length,
										 SCHAR* buffer)
{
/**************************************
 *
 *	i s c _ d s q l _ s q l _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Provide information on sql statement.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Statement statement = translate<CStatement>(stmt_handle);
		YEntry entryGuard(status, statement);

		if (( (item_length == 1) && (items[0] == isc_info_sql_stmt_type) ||
				(item_length == 2) && (items[0] == isc_info_sql_stmt_type) &&
				(items[1] == isc_info_end || items[1] == 0) ) &&
			(statement->flags & HANDLE_STATEMENT_prepared) && statement->das.dasup_stmt_type)
		{
			if (USHORT(buffer_length) >= 8)
			{
				*buffer++ = isc_info_sql_stmt_type;
				put_vax_short((UCHAR*) buffer, 4);
				buffer += 2;
				put_vax_long((UCHAR*) buffer, statement->das.dasup_stmt_type);
				buffer += 4;
				*buffer = isc_info_end;
			}
			else
			{
				*buffer = isc_info_truncated;
			}
		}
		else
		{
			statement->providerInterface->getInfo(&status,
									   item_length, reinterpret_cast<const unsigned char*>(items),
									   buffer_length, reinterpret_cast<unsigned char*>(buffer));
		}
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


int API_ROUTINE gds__enable_subsystem(TEXT* subsystem)
{
/**************************************
 *
 *	g d s _ $ e n a b l e _ s u b s y s t e m
 *
 **************************************
 *
 * Functional description
 *	Enable access to a specific subsystem.  If no subsystem
 *	has been explicitly enabled, all are available.
 *
 **************************************/
	return FALSE;
}


ISC_STATUS API_ROUTINE isc_wait_for_event(ISC_STATUS* user_status,
									  FB_API_HANDLE* handle,
									  USHORT length,
									  const UCHAR* events,
									  UCHAR* buffer)
{
/**************************************
 *
 *	g d s _ $ e v e n t _ w a i t
 *
 **************************************
 *
 * Functional description
 *	Que request for event notification.
 *
 **************************************/
	StatusVector status(user_status);
	RefPtr<WaitEvents> evnt;

	try
	{
		Attachment attachment = translate<CAttachment>(handle);
		YEntry entryGuard(status, attachment);

		evnt = new WaitEvents(attachment, event_ast, buffer);
		evnt->providerInterface = attachment->providerInterface->queEvents(&status, evnt, length, events);

		if (status[1])
		{
			status_exception::raise(status);
		}

		evnt->wait();
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	if (evnt)
	{
		{
			MutexLockGuard guard(*evnt->getProviderInterfaceCleanupMutex());

			if (evnt->providerInterface)
			{
				evnt->providerInterface->release();
				evnt->providerInterface = NULL;
			}
		}
		destroyNoThrow(evnt);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_get_segment(ISC_STATUS* user_status,
									   FB_API_HANDLE* blob_handle,
									   USHORT* return_length,
									   USHORT buffer_length,
									   UCHAR* buffer)
{
/**************************************
 *
 *	g d s _ $ g e t _ s e g m e n t
 *
 **************************************
 *
 * Functional description
 *	Get a segment from a blob opened for reading.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Blob blob = translate<CBlob>(blob_handle);
		YEntry entryGuard(status, blob);

		unsigned int length = blob->providerInterface->getSegment(&status, buffer_length, buffer);
		if ((status.isSuccess() || status[1] == isc_segment || status[1] == isc_segstr_eof) && return_length)
		{
			*return_length = length;
		}
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_get_slice(ISC_STATUS* user_status,
									 FB_API_HANDLE* db_handle,
									 FB_API_HANDLE* tra_handle,
									 ISC_QUAD* array_id,
									 USHORT sdl_length,
									 const UCHAR* sdl,
									 USHORT param_length,
									 const UCHAR* param,
									 SLONG slice_length,
									 UCHAR* slice,
									 SLONG* return_length)
{
/**************************************
 *
 *	g d s _ $ g e t _ s l i c e
 *
 **************************************
 *
 * Functional description
 *	Snatch a slice of an array.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Attachment attachment = translate<CAttachment>(db_handle);
		YEntry entryGuard(status, attachment);
		Transaction transaction = findTransaction(tra_handle, attachment);

		int length = transaction->providerInterface->getSlice(&status, array_id, sdl_length, sdl, param_length, param, slice_length, slice);
		if (status.isSuccess() && return_length)
		{
			*return_length = length;
		}
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE fb_disconnect_transaction(ISC_STATUS* user_status,
												 FB_API_HANDLE* tra_handle)
{
/**************************************
 *
 *	g d s _ $ h a n d l e _ c l e a n u p
 *
 **************************************
 *
 * Functional description
 *	Clean up a dangling transaction handle.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Transaction transaction = translate<CTransaction>(tra_handle);

		if (!(transaction->flags & HANDLE_TRANSACTION_limbo))
		{
			status_exception::raise(Arg::Gds(isc_no_recon));
		}

		destroy(transaction);
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_open_blob(ISC_STATUS* user_status,
									 FB_API_HANDLE* db_handle,
									 FB_API_HANDLE* tra_handle,
									 FB_API_HANDLE* blob_handle,
									 ISC_QUAD* blob_id)
{
/**************************************
 *
 *	g d s _ $ o p e n _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Open an existing blob.
 *
 **************************************/

	return open_blob(user_status, db_handle, tra_handle, blob_handle, blob_id, 0, 0, false);
}


ISC_STATUS API_ROUTINE isc_open_blob2(ISC_STATUS* user_status,
									  FB_API_HANDLE* db_handle,
									  FB_API_HANDLE* tra_handle,
									  FB_API_HANDLE* blob_handle,
									  ISC_QUAD* blob_id,
									  SSHORT bpb_length,
									  const UCHAR* bpb)
{
/**************************************
 *
 *	g d s _ $ o p e n _ b l o b 2
 *
 **************************************
 *
 * Functional description
 *	Open an existing blob (extended edition).
 *
 **************************************/

	return open_blob(user_status, db_handle, tra_handle, blob_handle, blob_id, bpb_length, bpb, false);
}


ISC_STATUS API_ROUTINE isc_prepare_transaction(ISC_STATUS* user_status,
								   FB_API_HANDLE* tra_handle)
{
/**************************************
 *
 *	g d s _ $ p r e p a r e
 *
 **************************************
 *
 * Functional description
 *	Prepare a transaction for commit.  First phase of a two
 *	phase commit.
 *
 **************************************/
	return isc_prepare_transaction2(user_status, tra_handle, 0, 0);
}


ISC_STATUS API_ROUTINE isc_prepare_transaction2(ISC_STATUS* user_status,
									FB_API_HANDLE* tra_handle,
									USHORT msg_length,
									const UCHAR* msg)
{
/**************************************
 *
 *	g d s _ $ p r e p a r e 2
 *
 **************************************
 *
 * Functional description
 *	Prepare a transaction for commit.  First phase of a two
 *	phase commit.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Transaction transaction = translate<CTransaction>(tra_handle);
		YEntry entryGuard(status, transaction);

		for (Transaction sub = transaction; sub; sub = sub->next)
		{
			if (sub->providerInterface)
			{
				sub->providerInterface->prepare(&status, msg_length, msg);
				if (!status.isSuccess())
				{
					return status[1];
				}
			}
		}

		transaction->flags |= HANDLE_TRANSACTION_limbo;
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_put_segment(ISC_STATUS* user_status,
									   FB_API_HANDLE* blob_handle,
									   USHORT buffer_length,
									   const UCHAR* buffer)
{
/**************************************
 *
 *	g d s _ $ p u t _ s e g m e n t
 *
 **************************************
 *
 * Functional description
 *	Put a segment in a blob opened for writing (creation).
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Blob blob = translate<CBlob>(blob_handle);
		YEntry entryGuard(status, blob);

		blob->providerInterface->putSegment(&status, buffer_length, buffer);
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_put_slice(ISC_STATUS* user_status,
									 FB_API_HANDLE* db_handle,
									 FB_API_HANDLE* tra_handle,
									 ISC_QUAD* array_id,
									 USHORT sdl_length,
									 const UCHAR* sdl,
									 USHORT param_length,
									 const SLONG* param,
									 SLONG slice_length,
									 UCHAR* slice)
{
/**************************************
 *
 *	g d s _ $ p u t _ s l i c e
 *
 **************************************
 *
 * Functional description
 *	Put a slice in an array.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Attachment attachment = translate<CAttachment>(db_handle);
		YEntry entryGuard(status, attachment);
		Transaction transaction = findTransaction(tra_handle, attachment);

		transaction->providerInterface->putSlice(&status, array_id, sdl_length, sdl,
									  param_length, reinterpret_cast<const unsigned char*>(param),
									  slice_length, slice);
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_que_events(ISC_STATUS* user_status,
									  FB_API_HANDLE* handle,
									  SLONG* id,
									  USHORT length,
									  const UCHAR* events,
									  FPTR_EVENT_CALLBACK ast,
									  void* arg)
{
/**************************************
 *
 *	g d s _ $ q u e _ e v e n t s
 *
 **************************************
 *
 * Functional description
 *	Que request for event notification.
 *
 **************************************/
	StatusVector status(user_status);
	Events evnt;

	try
	{
		Attachment attachment = translate<CAttachment>(handle);
		YEntry entryGuard(status, attachment);

		// This cast appears awful, but as long as handles are 32-bit entities it's OK
		evnt = new CEvents((FB_API_HANDLE*)id, attachment, ast, arg);
		evnt->providerInterface = attachment->providerInterface->queEvents(&status, evnt, length, events);

		if (status[1])
		{
			status_exception::raise(status);
		}
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);

		if (evnt)
		{
			destroyNoThrow(evnt);
		}
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_receive(ISC_STATUS* user_status,
								   FB_API_HANDLE* req_handle,
								   USHORT msg_type,
								   USHORT msg_length,
								   SCHAR* msg,
								   SSHORT level)
{
/**************************************
 *
 *	g d s _ $ r e c e i v e
 *
 **************************************
 *
 * Functional description
 *	Send a record to the host program.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Request request = translate<CRequest>(req_handle);
		YEntry entryGuard(status, request);

		request->providerInterface->receive(&status, level, msg_type, msg_length,
								 reinterpret_cast<unsigned char*>(msg));
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_reconnect_transaction(ISC_STATUS* user_status,
									 FB_API_HANDLE* db_handle,
									 FB_API_HANDLE* tra_handle,
									 SSHORT length,
									 const UCHAR* id)
{
/**************************************
 *
 *	g d s _ $ r e c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Connect to a transaction in limbo.
 *
 **************************************/
	StatusVector status(user_status);
	ITransaction* tra = NULL;

	try
	{
		nullCheck(tra_handle, isc_bad_trans_handle);
		Attachment attachment = translate<CAttachment>(db_handle);
		YEntry entryGuard(status, attachment);

		tra = attachment->providerInterface->reconnectTransaction(&status, length, id);
		if (!status.isSuccess())
		{
			return status[1];
		}

		Transaction transaction(new CTransaction(tra, tra_handle, attachment));
		transaction->flags |= HANDLE_TRANSACTION_limbo;
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
		if (tra)
		{
			*tra_handle = 0;
		}
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_release_request(ISC_STATUS* user_status,
										   FB_API_HANDLE* req_handle)
{
/**************************************
 *
 *	g d s _ $ r e l e a s e _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Release a request.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Request request = translate<CRequest>(req_handle);
		YEntry entryGuard(status, request);

		{
			MutexLockGuard guard(*request->getProviderInterfaceCleanupMutex());

			request->providerInterface->free(&status);
			if (!status.isSuccess())
			{
				return status[1];
			}
			request->providerInterface = NULL;
		}

		destroy(request);
		*req_handle = 0;
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_request_info(ISC_STATUS* user_status,
										FB_API_HANDLE* req_handle,
										SSHORT level,
										SSHORT item_length,
										const SCHAR* items,
										SSHORT buffer_length,
										SCHAR* buffer)
{
/**************************************
 *
 *	g d s _ $ r e q u e s t _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Provide information on blob object.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Request request = translate<CRequest>(req_handle);
		YEntry entryGuard(status, request);

		request->providerInterface->getInfo(&status, level,
								 item_length, reinterpret_cast<const unsigned char*>(items),
								 buffer_length, reinterpret_cast<unsigned char*>(buffer));
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}

#if defined (SOLARIS) || defined (WIN_NT)
extern "C"
#endif

SLONG API_ROUTINE isc_reset_fpe(USHORT /*fpe_status*/)
{
/**************************************
 *
 *	i s c _ r e s e t _ f p e
 *
 **************************************
 *
 * Functional description
 *	API to be used to tell Firebird to reset it's
 *	FPE handler - eg: client has an FPE of it's own
 *	and just changed it.
 *
 * Returns
 *	Prior setting of the FPE reset flag
 *
 **************************************/
	return FPE_RESET_ALL_API_CALL;
}


ISC_STATUS API_ROUTINE isc_rollback_retaining(ISC_STATUS* user_status,
											  FB_API_HANDLE* tra_handle)
{
/**************************************
 *
 *	i s c _ r o l l b a c k _ r e t a i n i n g
 *
 **************************************
 *
 * Functional description
 *	Abort a transaction, but keep all cursors open.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Transaction transaction = translate<CTransaction>(tra_handle);
		YEntry entryGuard(status, transaction);

		for (Transaction sub = transaction; sub; sub = sub->next)
		{
			if (sub->providerInterface)
			{
				sub->providerInterface->rollbackRetaining(&status);
				if (!status.isSuccess())
				{
					return status[1];
				}
			}
		}

		transaction->flags |= HANDLE_TRANSACTION_limbo;
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_rollback_transaction(ISC_STATUS* user_status,
									FB_API_HANDLE* tra_handle)
{
/**************************************
 *
 *	g d s _ $ r o l l b a c k
 *
 **************************************
 *
 * Functional description
 *	Abort a transaction.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Transaction transaction = translate<CTransaction>(tra_handle);
		YEntry entryGuard(status, transaction);

		{
			MutexLockGuard guard(*transaction->getProviderInterfaceCleanupMutex());

			for (Transaction sub = transaction; sub; sub = sub->next)
			{
				if (sub->providerInterface)
				{
					sub->providerInterface->rollback(&status);
					if (!status.isSuccess() &&
					    (!is_network_error(status) ||
						 (transaction->flags & HANDLE_TRANSACTION_limbo)))
					{
						return status[1];
					}
					sub->providerInterface = NULL;
				}
			}
		}

		if (is_network_error(status))
		{
			fb_utils::init_status(status);
		}

		destroy(transaction);

		*tra_handle = 0;
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_seek_blob(ISC_STATUS* user_status,
									 FB_API_HANDLE* blob_handle,
									 SSHORT mode,
									 SLONG offset,
									 SLONG* result)
{
/**************************************
 *
 *	g d s _ $ s e e k _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Seek a blob.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Blob blob = translate<CBlob>(blob_handle);
		YEntry entryGuard(status, blob);

		unsigned int pos = blob->providerInterface->seek(&status, mode, offset);
		if (status.isSuccess() && result)
		{
			*result = pos;
		}
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_send(ISC_STATUS* user_status,
								FB_API_HANDLE* req_handle,
								USHORT msg_type,
								USHORT msg_length,
								const SCHAR* msg,
								SSHORT level)
{
/**************************************
 *
 *	g d s _ $ s e n d
 *
 **************************************
 *
 * Functional description
 *	Get a record from the host program.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Request request = translate<CRequest>(req_handle);
		YEntry entryGuard(status, request);

		request->providerInterface->send(&status, level, msg_type, msg_length,
							  reinterpret_cast<const unsigned char*>(msg));
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_service_attach(ISC_STATUS* user_status,
										  USHORT service_length,
										  const TEXT* service_name,
										  FB_API_HANDLE* public_handle,
										  USHORT spb_length,
										  const SCHAR* spb)
{
/**************************************
 *
 *	i s c _ s e r v i c e _ a t t a c h
 *
 **************************************
 *
 * Functional description
 *	Attach a service through the first subsystem
 *	that recognizes it.
 *
 **************************************/
	Service service(0);
	StatusVector temp(NULL);
	StatusVector status(user_status);
	IService* svc = NULL;

	try
	{
		YEntry entryGuard;

		nullCheck(public_handle, isc_bad_svc_handle);

		if (shutdownStarted)
		{
			status_exception::raise(Arg::Gds(isc_att_shutdown));
		}

		if (!service_name)
		{
			status_exception::raise(Arg::Gds(isc_service_att_err) << Arg::Gds(isc_svc_name_missing));
		}

		if (spb_length > 0 && !spb)
		{
			status_exception::raise(Arg::Gds(isc_bad_spb_form));
		}

		string svcname(service_name, service_length ? service_length : strlen(service_name));
		svcname.rtrim();

		StatusVector* ptr = &status;
		for (GetPlugins<PProvider, NoEntrypoint> providerIterator(PluginType::Provider, FB_P_PROVIDER_VERSION);
			 providerIterator.hasData(); providerIterator.next())
		{
			PProvider* p = providerIterator.plugin();
			svc = p->attachServiceManager(ptr, svcname.c_str(),
										  spb_length, reinterpret_cast<const unsigned char*>(spb));
			if (ptr->isSuccess())
			{
				service = new CService(svc, public_handle, p);
				status[0] = isc_arg_gds;
				status[1] = 0;
				if (status[2] != isc_arg_warning)
				{
					status[2] = isc_arg_end;
				}
				return status[1];
			}
			if ((*ptr)[1] != isc_unavailable)
			{
				ptr = &temp;
			}
		}

		if (status[1] == 0)
		{
			Arg::Gds(isc_unavailable).raise();
		}
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
		if (svc)
		{
			svc->detach(&temp);
			destroyNoThrow(service);
			*public_handle = 0;
		}
	}

	if (status[1] == isc_unavailable)
	{
		status[1] = isc_service_att_err;
	}
	return status[1];
}


ISC_STATUS API_ROUTINE isc_service_detach(ISC_STATUS* user_status, FB_API_HANDLE* handle)
{
/**************************************
 *
 *	i s c _ s e r v i c e _ d e t a c h
 *
 **************************************
 *
 * Functional description
 *	Close down a service.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		YEntry entryGuard;
		Service service = translate<CService>(handle);

		{
			MutexLockGuard guard(*service->getProviderInterfaceCleanupMutex());

			service->providerInterface->detach(&status);
			if (!status.isSuccess())
			{
				return status[1];
			}
			service->providerInterface = NULL;
		}

		destroy(service);
		*handle = 0;
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_service_query(ISC_STATUS* user_status,
										 FB_API_HANDLE* handle,
										 ULONG* /*reserved*/,
										 USHORT send_item_length,
										 const SCHAR* send_items,
										 USHORT recv_item_length,
										 const SCHAR* recv_items,
										 USHORT buffer_length,
										 SCHAR* buffer)
{
/**************************************
 *
 *	i s c _ s e r v i c e _ q u e r y
 *
 **************************************
 *
 * Functional description
 *	Provide information on service object.
 *
 *	NOTE: The parameter RESERVED must not be used
 *	for any purpose as there are networking issues
 *	involved (as with any handle that goes over the
 *	network).  This parameter will be implemented at
 *	a later date.
 **************************************/
	StatusVector status(user_status);

	try
	{
		YEntry entryGuard;
		Service service = translate<CService>(handle);

		service->providerInterface->query(&status, send_item_length, reinterpret_cast<const unsigned char*>(send_items),
							   recv_item_length, reinterpret_cast<const unsigned char*>(recv_items),
							   buffer_length, reinterpret_cast<unsigned char*>(buffer));
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_service_start(ISC_STATUS* user_status,
										 FB_API_HANDLE* handle,
										 ULONG* /*reserved*/,
										 USHORT spb_length,
										 const SCHAR* spb)
{
/**************************************
 *
 *	i s c _ s e r v i c e _ s t a r t
 *
 **************************************
 *
 * Functional description
 *	Starts a service thread
 *
 *	NOTE: The parameter RESERVED must not be used
 *	for any purpose as there are networking issues
 *	involved (as with any handle that goes over the
 *	network).  This parameter will be implemented at
 *	a later date.
 **************************************/
	StatusVector status(user_status);

	try
	{
		YEntry entryGuard;
		Service service = translate<CService>(handle);

		service->providerInterface->start(&status, spb_length, reinterpret_cast<const unsigned char*>(spb));
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_start_and_send(ISC_STATUS* user_status,
										  FB_API_HANDLE* req_handle,
										  FB_API_HANDLE* tra_handle,
										  USHORT msg_type,
										  USHORT msg_length,
										  const SCHAR* msg,
										  SSHORT level)
{
/**************************************
 *
 *	g d s _ $ s t a r t _ a n d _ s e n d
 *
 **************************************
 *
 * Functional description
 *	Get a record from the host program.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Request request = translate<CRequest>(req_handle);
		YEntry entryGuard(status, request);
		Transaction transaction = findTransaction(tra_handle, request->parent);

		request->providerInterface->startAndSend(&status, transaction->providerInterface, level, msg_type, msg_length,
									  reinterpret_cast<const unsigned char*>(msg));
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_start_request(ISC_STATUS* user_status,
								 FB_API_HANDLE* req_handle,
								 FB_API_HANDLE* tra_handle,
								 SSHORT level)
{
/**************************************
 *
 *	g d s _ $ s t a r t
 *
 **************************************
 *
 * Functional description
 *	Get a record from the host program.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Request request = translate<CRequest>(req_handle);
		YEntry entryGuard(status, request);
		Transaction transaction = findTransaction(tra_handle, request->parent);

		request->providerInterface->start(&status, transaction->providerInterface, level);
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_start_multiple(ISC_STATUS* user_status,
										  FB_API_HANDLE* tra_handle,
										  SSHORT count,
//										  TEB* vector)
										  void* vec)
{
/**************************************
 *
 *	g d s _ $ s t a r t _ m u l t i p l e
 *
 **************************************
 *
 * Functional description
 *	Start a transaction.
 *
 **************************************/
	TEB* vector = (TEB*) vec;
	ISC_STATUS_ARRAY temp;
	Transaction transaction(NULL);
	Attachment attachment(NULL);

	StatusVector status(user_status);

	try
	{
		YEntry entryGuard;
		nullCheck(tra_handle, isc_bad_trans_handle);

		if (count <= 0 || !vector)
			status_exception::raise(Arg::Gds(isc_bad_teb_form));

		Transaction* ptr = &transaction;

		for (USHORT n = 0; n < count; n++, ptr = &(*ptr)->next, vector++)
		{
			if (vector->teb_tpb_length < 0 || (vector->teb_tpb_length > 0 && !vector->teb_tpb))
				status_exception::raise(Arg::Gds(isc_bad_tpb_form));

			attachment = translate<CAttachment>(vector->teb_database);
			YEntry attGuard(status, attachment);

			*ptr = new CTransaction(0, 0, attachment);

			(*ptr)->providerInterface = attachment->providerInterface->startTransaction(&status,
																  vector->teb_tpb_length, vector->teb_tpb,
																  (*ptr)->public_handle);
			if (!status.isSuccess())
			{
				status_exception::raise(status);
			}
		}

		if (transaction->next)
		{
			Transaction sub(new CTransaction(tra_handle));
			sub->next = transaction;
		}
		else
		{
			*tra_handle = transaction->public_handle;
		}
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);

		if (transaction)
		{
			*tra_handle = 0;

			MutexLockGuard guard(*transaction->getProviderInterfaceCleanupMutex());

			while (transaction)
			{
				StatusVector temp(NULL);
				Transaction sub = transaction;
				transaction = sub->next;
				if (sub->providerInterface)
				{
					sub->providerInterface->rollback(&temp);
				}
				sub->providerInterface = NULL;
			}
		}

		destroyNoThrow(transaction);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE_VARARG isc_start_transaction(ISC_STATUS* user_status,
													FB_API_HANDLE* tra_handle,
													SSHORT count, ...)
{
/**************************************
 *
 *	g d s _ $ s t a r t _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Start a transaction.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		HalfStaticArray<TEB, 16> tebs;
		TEB* teb = tebs.getBuffer(count);

		const TEB* const end = teb + count;
		va_list ptr;
		va_start(ptr, count);

		for (TEB* teb_iter = teb; teb_iter < end; teb_iter++) {
			teb_iter->teb_database = va_arg(ptr, FB_API_HANDLE*);
			teb_iter->teb_tpb_length = va_arg(ptr, int);
			teb_iter->teb_tpb = va_arg(ptr, UCHAR *);
		}
		va_end(ptr);

		isc_start_multiple(status, tra_handle, count, teb);
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_transact_request(ISC_STATUS* user_status,
											FB_API_HANDLE* db_handle,
											FB_API_HANDLE* tra_handle,
											USHORT blr_length,
											SCHAR* blr,
											USHORT in_msg_length,
											SCHAR* in_msg,
											USHORT out_msg_length,
											SCHAR* out_msg)
{
/**************************************
 *
 *	i s c _ t r a n s a c t _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Execute a procedure.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Attachment attachment = translate<CAttachment>(db_handle);
		YEntry entryGuard(status, attachment);
		Transaction transaction = findTransaction(tra_handle, attachment);

		transaction->providerInterface->transactRequest(&status, blr_length, reinterpret_cast<unsigned char*>(blr),
											 in_msg_length, reinterpret_cast<unsigned char*>(in_msg),
											 out_msg_length, reinterpret_cast<unsigned char*>(out_msg),
											 attachment->providerInterface);
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE gds__transaction_cleanup(ISC_STATUS* user_status,
												FB_API_HANDLE* tra_handle,
												TransactionCleanupRoutine *routine,
												void* arg)
{
/**************************************
 *
 *	g d s _ $ t r a n s a c t i o n _ c l e a n u p
 *
 **************************************
 *
 * Functional description
 *	Register a transaction specific cleanup handler.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Transaction transaction = translate<CTransaction>(tra_handle);
		transaction->cleanup.add(routine, arg);
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_transaction_info(ISC_STATUS* user_status,
											FB_API_HANDLE* tra_handle,
											SSHORT item_length,
											const SCHAR* p_items,
											SSHORT buffer_length,
											UCHAR* buffer)
{
/**************************************
 *
 *	g d s _ $ t r a n s a c t i o n _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Provide information on transaction object.
 *
 **************************************/
	StatusVector status(user_status);
	const unsigned char* items = reinterpret_cast<const unsigned char*>(p_items);

	try
	{
		Transaction transaction = translate<CTransaction>(tra_handle);
		YEntry entryGuard(status, transaction);

		if (transaction->providerInterface)
		{
			transaction->providerInterface->getInfo(&status, item_length, items, buffer_length, buffer);
		}
		else
		{
			SSHORT item_len = item_length;
			SSHORT buffer_len = buffer_length;
			for (Transaction sub = transaction->next; sub; sub = sub->next)
			{
				sub->providerInterface->getInfo(&status, item_length, items, buffer_length, buffer);
				if (!status.isSuccess())
				{
					return status[1];
				}

				UCHAR* ptr = buffer;
				const UCHAR* const end = buffer + buffer_len;
				while (ptr < end && *ptr == isc_info_tra_id)
				{
					ptr += 3 + gds__vax_integer(ptr + 1, 2);
				}

				if (ptr >= end || *ptr != isc_info_end)
				{
					return status[1];
				}

				buffer_len = end - ptr;
				buffer = ptr;
			}
		}
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE isc_unwind_request(ISC_STATUS* user_status,
								  FB_API_HANDLE* req_handle,
								  SSHORT level)
{
/**************************************
 *
 *	g d s _ $ u n w i n d
 *
 **************************************
 *
 * Functional description
 *	Unwind a running request.  This is potentially nasty since it can be called
 *	asynchronously.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Request request = translate<CRequest>(req_handle);
		YEntry entryGuard(status, request);

		request->providerInterface->unwind(&status, level);
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}

#ifdef DEBUG_GDS_ALLOC
static SCHAR *alloc_debug(SLONG length, const char* file, int line)
#else
static SCHAR *alloc(SLONG length)
#endif
{
/**************************************
 *
 *	a l l o c
 *
 **************************************
 *
 * Functional description
 *	Allocate some memory.
 *
 **************************************/
	SCHAR *block;

#ifdef DEBUG_GDS_ALLOC
	if (block = static_cast<SCHAR*>(gds__alloc_debug((SLONG) (sizeof(SCHAR) * length), file, line)))
#else
	if (block = static_cast<SCHAR*>(gds__alloc((SLONG) (sizeof(SCHAR) * length))))
#endif
		memset(block, 0, length);
	else
		BadAlloc::raise();
	return block;
}


#ifdef DEV_BUILD
static void check_status_vector(const ISC_STATUS* status)
 {
/**************************************
 *
 *	c h e c k _ s t a t u s _ v e c t o r
 *
 **************************************
 *
 * Functional description
 *	Validate that a status vector looks valid.
 *
 **************************************/

#define SV_MSG(x)	{ gds__log ("%s %d check_status_vector: %s", __FILE__, __LINE__, (x)); BREAKPOINT (__LINE__); }

	const ISC_STATUS* s = status;
	if (!s) {
		SV_MSG("Invalid status vector");
		return;
	}

	if (*s != isc_arg_gds) {
		SV_MSG("Must start with isc_arg_gds");
		return;
	}

	// Vector [2] could either end the vector, or start a warning
	// in either case the status vector is a success
	if ((s[1] == FB_SUCCESS) && (s[2] != isc_arg_end) && (s[2] != isc_arg_gds) &&
		(s[2] != isc_arg_warning))
	{
		SV_MSG("Bad success vector format");
	}

	ULONG length;

	while (*s != isc_arg_end)
	{
		const ISC_STATUS code = *s++;
		switch (code)
		{
		case isc_arg_warning:
		case isc_arg_gds:
			// The next element must either be 0 (indicating no error) or a
			// valid isc error message, let's check
			if (*s && (*s & ISC_MASK) != ISC_MASK) {
				if (code == isc_arg_warning) {
					SV_MSG("warning code not a valid ISC message");
				}
				else {
					SV_MSG("error code not a valid ISC message");
				}
			}

			// If the error code is valid, then I better be able to retrieve a
			// proper facility code from it ... let's find out
			if (*s && (*s & ISC_MASK) == ISC_MASK) {
				bool found = false;

				const struct _facilities* facs = facilities;
				const int fac_code = GET_FACILITY(*s);
				while (facs->facility) {
					if (facs->fac_code == fac_code) {
						found = true;
						break;
					}
					facs++;
				}
				if (!found)
					if (code == isc_arg_warning) {
						SV_MSG("warning code does not contain a valid facility");
					}
					else {
						SV_MSG("error code does not contain a valid facility");
					}
			}
			s++;
			break;

		case isc_arg_interpreted:
		case isc_arg_string:
		case isc_arg_sql_state:
			length = strlen((const char*) *s);
			// This check is heuristic, not deterministic
			if (length > 1024 - 1)
				SV_MSG("suspect length value");
			if (*((const UCHAR *) * s) == 0xCB)
				SV_MSG("string in freed memory");
			s++;
			break;

		case isc_arg_cstring:
			length = (ULONG) * s;
			s++;
			// This check is heuristic, not deterministic
			// Note: This can never happen anyway, as the length is coming from a byte value
			if (length > 1024 - 1)
				SV_MSG("suspect length value");
			if (*((const UCHAR *) * s) == 0xCB)
				SV_MSG("string in freed memory");
			s++;
			break;

		case isc_arg_number:
		case isc_arg_vms:
		case isc_arg_unix:
		case isc_arg_win32:
			s++;
			break;

		default:
			SV_MSG("invalid status code");
			return;
		}
		if ((s - status) >= ISC_STATUS_LENGTH)
			SV_MSG("vector too long");
	}

#undef SV_MSG

}
#endif


static void event_ast(void* buffer_void, USHORT length, const UCHAR* items)
{
/**************************************
 *
 *	e v e n t _ a s t
 *
 **************************************
 *
 * Functional description
 *	We're had an event complete.
 *
 **************************************/
	memcpy(buffer_void, items, length);
}


static Transaction find_transaction(Attachment attachment, Transaction transaction)
{
/**************************************
 *
 *	f i n d _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Find the element of a possible multiple database transaction
 *	that corresponds to the current attachment.
 *
 **************************************/

	for (; transaction; transaction = transaction->next)
	{
		if (transaction->parent == attachment)
			return transaction;
	}

	return Transaction(NULL);
}


static int get_database_info(Transaction transaction, unsigned char** ptr)
{
/**************************************
 *
 *	g e t _ d a t a b a s e _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Get the full database pathname
 *	and put it in the transaction
 *	description record.
 *
 **************************************/

	// Look at the changed code: we don't support here more than 254 bytes in the path
	// so it's better to truncate or we'll have corrupt data in the trans desc record:
	// the length in one byte would wrap and we would copy more bytes that expected.
	// Our caller (prepare) assumed each call consumes at most 256 bytes (item, len, data)
	// hence if we don't check here, we have a B.O.
	unsigned char* p = *ptr;
	Attachment attachment = transaction->parent;
	*p++ = TDR_DATABASE_PATH;
	const TEXT* q = attachment->db_path.c_str();
	size_t len = strlen(q);
	if (len > 254)
		len = 254;

	*p++ = (unsigned char) len;
	memcpy(p, q, len);
	*ptr = p + len;

	return FB_SUCCESS;
}


static USHORT sqlda_buffer_size(USHORT min_buffer_size, const XSQLDA* sqlda, USHORT dialect)
{
/**************************************
 *
 *	s q l d a _ b u f f e r _ s i z e
 *
 **************************************
 *
 * Functional description
 *	Calculate size of a buffer that is large enough
 *	to store the info items relating to an SQLDA.
 *
 **************************************/
	USHORT n_variables;

	// If dialect / 10 == 0, then it has not been combined with the
	// parser version for a prepare statement.  If it has been combined, then
	// the dialect needs to be pulled out to compare to DIALECT_xsqlda

	USHORT sql_dialect = dialect / 10;
	if (sql_dialect == 0)
		sql_dialect = dialect;

	if (!sqlda)
		n_variables = 0;
	else if (sql_dialect >= DIALECT_xsqlda)
		n_variables = sqlda->sqln;
	else
		n_variables = ((SQLDA *) sqlda)->sqln;

	ULONG length = 32 + ULONG(n_variables) * 172;
	if (length < min_buffer_size)
		length = min_buffer_size;

	return (USHORT)((length > 65500L) ? 65500L : length);
}


static ISC_STATUS get_transaction_info(ISC_STATUS* user_status,
									   Transaction transaction,
									   unsigned char** ptr)
{
/**************************************
 *
 *	g e t _ t r a n s a c t i o n _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Put a transaction's id into the transaction
 *	description record.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		unsigned char buffer[16];
		unsigned char* p = *ptr;

		transaction->providerInterface->getInfo(&status, sizeof(prepare_tr_info), prepare_tr_info,
									 sizeof(buffer), buffer);
		if (!status.isSuccess())
		{
			return status[1];
		}

		unsigned char* q = buffer + 3;
		*p++ = TDR_TRANSACTION_ID;

		USHORT length = (USHORT)gds__vax_integer(buffer + 1, 2);

		// Prevent information out of sync.
		if (length > MAX_UCHAR)
			length = MAX_UCHAR;

		*p++ = length;
		memcpy(p, q, length);
		*ptr = p + length;
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


static void iterative_sql_info(ISC_STATUS* user_status,
							   FB_API_HANDLE* stmt_handle,
							   USHORT item_length,
							   const SCHAR* items,
							   SSHORT buffer_length,
							   SCHAR* buffer,
							   USHORT dialect,
							   XSQLDA* sqlda)
{
/**************************************
 *
 *	i t e r a t i v e _ s q l _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Turn an sql info buffer into an SQLDA.  If the info
 *	buffer was incomplete, make another request, beginning
 *	where the previous info call left off.
 *
 **************************************/
	USHORT last_index;
	SCHAR new_items[32];

	while (UTLD_parse_sql_info(user_status, dialect, buffer, sqlda, &last_index) && last_index)
	{
		char* p = new_items;
		*p++ = isc_info_sql_sqlda_start;
		*p++ = 2;
		*p++ = last_index;
		*p++ = last_index >> 8;
		fb_assert(p + item_length <= new_items + sizeof(new_items));
		memcpy(p, items, item_length);
		p += item_length;
		if (isc_dsql_sql_info(user_status, stmt_handle, (SSHORT) (p - new_items), new_items,
								buffer_length, buffer))
		{
			break;
		}
	}
}


static ISC_STATUS open_blob(ISC_STATUS* user_status,
							FB_API_HANDLE* db_handle,
							FB_API_HANDLE* tra_handle,
							FB_API_HANDLE* public_blob_handle,
							ISC_QUAD* blob_id,
							USHORT bpb_length,
							const UCHAR* bpb,
							bool createFlag)
{
/**************************************
 *
 *	o p e n _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Open an existing blob (extended edition).
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		nullCheck(public_blob_handle, isc_bad_segstr_handle);

		Attachment attachment = translate<CAttachment>(db_handle);
		YEntry entryGuard(status, attachment);
		Transaction transaction = findTransaction(tra_handle, attachment);

		IBlob* blob_handle = createFlag ?
			transaction->providerInterface->createBlob(&status, blob_id, bpb_length, bpb, attachment->providerInterface) :
			transaction->providerInterface->openBlob(&status, blob_id, bpb_length, bpb, attachment->providerInterface);

		if (!status.isSuccess())
		{
			return status[1];
		}

		Blob blob(new CBlob(blob_handle, public_blob_handle, attachment, transaction));
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


static ISC_STATUS prepare(ISC_STATUS* user_status, Transaction transaction)
{
/**************************************
 *
 *	p r e p a r e
 *
 **************************************
 *
 * Functional description
 *	Perform the first phase of a two-phase commit
 *	for a multi-database transaction.
 *
 **************************************/
	StatusVector status(user_status);

	Transaction sub;
	HalfStaticArray<unsigned char, 1024> tdr_buffer;
	size_t length = 0;
	int transcount = 0;

	for (sub = transaction->next; sub; sub = sub->next)
	{
		length += 256;
		++transcount;
	}
	// To do: use transcount to check the maximum allowed dbs in a two phase commit.

	TEXT host[64];
	ISC_get_host(host, sizeof(host));
	const size_t hostlen = strlen(host);
	length += hostlen + 3; // TDR_version + TDR_host_site + UCHAR(strlen(host))

	unsigned char* const description = tdr_buffer.getBuffer(length);

	// build a transaction description record containing
	// the host site and database/transaction
	// information for the target databases.

	unsigned char* p = description;
	if (!p)
	{
		Arg::Gds(isc_virmemexh).copyTo(status);
		return status[1];
	}

	*p++ = TDR_VERSION;
	*p++ = TDR_HOST_SITE;
	*p++ = UCHAR(hostlen);
	memcpy(p, host, hostlen);
	p += hostlen;

	// Get database and transaction stuff for each sub-transaction

	for (sub = transaction->next; sub; sub = sub->next) {
		get_database_info(sub, &p);
		get_transaction_info(status, sub, &p);
	}

	// So far so good -- prepare each sub-transaction

	length = p - description;

	for (sub = transaction->next; sub; sub = sub->next)
	{
		sub->providerInterface->prepare(&status, length, description);
		if (!status.isSuccess())
		{
			return status[1];
		}
	}

	return FB_SUCCESS;
}


inline void why_priv_gds__free_if_set(SCHAR** pMem)
{
	if (*pMem)
	{
		gds__free(*pMem);
		*pMem = 0;
	}
}

static void release_dsql_support(sqlda_sup& dasup)
{
/**************************************
 *
 *	r e l e a s e _ d s q l _ s u p p o r t
 *
 **************************************
 *
 * Functional description
 *	Release some memory.
 *
 **************************************/
	sqlda_sup::dasup_clause* pClauses = dasup.dasup_clauses;

	why_priv_gds__free_if_set(&pClauses[DASUP_CLAUSE_bind].dasup_blr);
	why_priv_gds__free_if_set(&pClauses[DASUP_CLAUSE_select].dasup_blr);
	why_priv_gds__free_if_set(&pClauses[DASUP_CLAUSE_bind].dasup_msg);
	why_priv_gds__free_if_set(&pClauses[DASUP_CLAUSE_select].dasup_msg);
	why_priv_gds__free_if_set(&pClauses[DASUP_CLAUSE_bind].dasup_info_buf);
	why_priv_gds__free_if_set(&pClauses[DASUP_CLAUSE_select].dasup_info_buf);
}


static void save_error_string(ISC_STATUS* status)
{
/**************************************
 *
 *	s a v e _ e r r o r _ s t r i n g
 *
 **************************************
 *
 * Functional description
 *	This is need because there are cases
 *	where the memory allocated for strings
 *	in the status vector is freed prior to
 *	surfacing them to the user.  This is an
 *	attempt to save off 1 string to surface to
 *  	the user.  Any other strings will be set to
 *	a standard <Unknown> string.
 *
 **************************************/
	fb_assert(status != NULL);

	TEXT* p = glbstr1;
	ULONG len = sizeof(glbstr1) - 1;

	while (*status != isc_arg_end)
	{
		ULONG l;
		switch (*status++)
		{
		case isc_arg_cstring:
			l = (ULONG) * status;
			if (l < len)
			{
				status++;		// Length is unchanged

				// This strncpy should really be a memcpy
				strncpy(p, reinterpret_cast<char*>(*status), l);
				*status++ = (ISC_STATUS) p;	// string in static memory
				p += l;
				len -= l;
			}
			else {
				*status++ = (ISC_STATUS) strlen(glbunknown);
				*status++ = (ISC_STATUS) glbunknown;
			}
			break;

		case isc_arg_interpreted:
		case isc_arg_string:
		case isc_arg_sql_state:
			l = (ULONG) strlen(reinterpret_cast<char*>(*status)) + 1;
			if (l < len)
			{
				strncpy(p, reinterpret_cast<char*>(*status), l);
				*status++ = (ISC_STATUS) p;	// string in static memory
				p += l;
				len -= l;
			}
			else
			{
				*status++ = (ISC_STATUS) glbunknown;
			}
			break;

		default:
			fb_assert(FALSE);
		case isc_arg_gds:
		case isc_arg_number:
		case isc_arg_vms:
		case isc_arg_unix:
		case isc_arg_win32:
			status++;			// Skip parameter
			break;
		}
	}
}


static bool set_path(const PathName& file_name, PathName& expanded_name)
{
/**************************************
 *
 *	s e t _ p a t h
 *
 **************************************
 *
 * Functional description
 *	Set a prefix to a filename based on
 *	the ISC_PATH user variable.
 *
 **************************************/

	// look for the environment variables to tack
	// onto the beginning of the database path
	PathName pathname;
	if (!fb_utils::readenv("ISC_PATH", pathname))
		return false;

	// if the file already contains a remote node
	// or any path at all forget it
	for (const TEXT* p = file_name.c_str(); *p; p++)
	{
		if (*p == ':' || *p == '/' || *p == '\\')
			return false;
	}

	// concatenate the strings

	expanded_name = pathname;

	// CVC: Make the concatenation work if no slash is present.
	char lastChar = expanded_name[expanded_name.length() - 1];
	if (lastChar != ':' && lastChar != '/' && lastChar != '\\') {
		expanded_name.append(1, PathUtils::dir_sep);
	}

	expanded_name.append(file_name);

	return true;
}


static GlobalPtr<Mutex> singleShutdown;

int API_ROUTINE fb_shutdown(unsigned int timeout, const int reason)
{
/**************************************
 *
 *	f b _ s h u t d o w n
 *
 **************************************
 *
 * Functional description
 *	Shutdown firebird.
 *
 **************************************/
	MutexLockGuard guard(singleShutdown);

	if (shutdownStarted)
	{
		return FB_SUCCESS;
	}

	StatusVector status(NULL);
	int rc = FB_SUCCESS;

#ifdef DEV_BUILD
	// make timeout huge in debug build: hard to debug something during 5-10 sec
	if (timeout)
	{
		timeout = 10 * 3600 * 1000;		// 10 hours
	}
#endif

	try
	{
		// In some cases it's OK to stop plugin manager even now
		if (reason == fb_shutrsn_exit_called)
		{
			PluginManager::shutdown();
		}

		// Ask clients about shutdown confirmation
		if (ShutChain::run(fb_shut_confirmation, reason) != FB_SUCCESS)
		{
			return FB_FAILURE;	// Do not perform former shutdown
		}

		// Shut known components, internal in yValve
		PluginManager::shutdown();
		shutdownTimers();

		// Shutdown clients before providers
		if (ShutChain::run(fb_shut_preproviders, reason) != FB_SUCCESS)
		{
			rc = FB_FAILURE;
		}

		// shutdown yValve
		shutdownStarted = true;	// since this moment no new thread will be able to enter yValve
								// unfortunately existing threads continue to run inside it

		// Shutdown providers (if any present)
		for (GetPlugins<PProvider, NoEntrypoint> providerIterator(PluginType::Provider, FB_P_PROVIDER_VERSION);
			 providerIterator.hasData(); providerIterator.next())
		{
			PProvider* p = providerIterator.plugin();
			StatusVector status(NULL);
			p->shutdown(&status, timeout, reason);
			if (status[1])
			{
				rc = FB_FAILURE;
			}
		}

		// Close all known interfaces from providers ...

		{ // guard scope
			WriteLockGuard sync(handleMappingLock);
			HandleMapping::Accessor a(&handleMapping);

			if (a.getFirst())
			{
				do
				{
					BaseHandle* handle = a.current();
					handle->releaseAll();
				} while (a.getNext());
			}
		}

		// ... and wait for all providers to go away
		PluginManager::waitForType(PluginType::Provider);

		// Shutdown clients after providers
		if (ShutChain::run(fb_shut_postproviders, reason) != FB_SUCCESS)
		{
			rc = FB_FAILURE;
		}

		// Finish shutdown
		if (ShutChain::run(fb_shut_finish, reason) != FB_SUCCESS)
		{
			rc = FB_FAILURE;
		}

		// At this step callbacks are welcome to exit (or take actions to make main thread do it)
		if (ShutChain::run(fb_shut_exit, reason) != FB_SUCCESS)
		{
			rc = FB_FAILURE;
		}
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
		gds__log_status(0, status);
		return FB_FAILURE;
	}

	return rc;
}


ISC_STATUS API_ROUTINE fb_shutdown_callback(ISC_STATUS* user_status,
											FB_SHUTDOWN_CALLBACK callBack,
											const int mask,
											void* arg)
{
/**************************************
 *
 *	f b _ s h u t d o w n _ c a l l b a c k
 *
 **************************************
 *
 * Functional description
 *	Register client callback to be called when FB is going down.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		ShutChain::add(callBack, mask, arg);
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}


ISC_STATUS API_ROUTINE fb_ping(ISC_STATUS* user_status, FB_API_HANDLE* db_handle)
{
/**************************************
 *
 *	f b _ p i n g
 *
 **************************************
 *
 * Functional description
 *	Check the attachment handle for persistent errors.
 *
 **************************************/
	StatusVector status(user_status);

	try
	{
		Attachment attachment = translate<CAttachment>(db_handle);
		YEntry entryGuard(status, attachment);

		attachment->providerInterface->ping(&status);
		if (!status.isSuccess())
		{
			if (!attachment->status.getError())
			{
				attachment->status.save(status);
			}

			{
				MutexLockGuard guard(*attachment->getProviderInterfaceCleanupMutex());
				if (attachment->providerInterface)
				{
					attachment->providerInterface->detach(&status);
					attachment->providerInterface = NULL;
				}
			}
			status_exception::raise(attachment->status.value());
		}
	}
	catch (const Exception& e)
	{
		e.stuffException(&status);
	}

	return status[1];
}
