/*
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
 *  The Original Code was created by Alexander Peshkoff
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2002 Alexander Peshkoff <peshkoff@mail.ru>
 *  and all contributors signed below.
 *
 *  All rights reserved.
 *
 *  Contributor(s): ______________________________
 *
 */

#ifndef JRD_Y_HANDLE_H
#define JRD_Y_HANDLE_H

#include "../common/classes/alloc.h"
#include "../common/classes/array.h"
#include "../common/classes/fb_string.h"
#include "../dsql/sqlda_pub.h"
#include "../dsql/sqlda.h"
#include "../jrd/thread_proto.h"

#include "gen/iberror.h"


namespace Jrd {
	class Attachment;
	class jrd_tra;
	class jrd_req;
}

class dsql_req;

namespace YValve 
{
	// flags
	const UCHAR HANDLE_TRANSACTION_limbo	= 0x01;
	const UCHAR HANDLE_BLOB_filter			= 0x02;	// Blob is locally filtered
	const UCHAR HANDLE_STATEMENT_local		= 0x04;	// Process DSQL statement locally
	const UCHAR HANDLE_STATEMENT_prepared	= 0x08;
	const UCHAR HANDLE_shutdown				= 0x10;	// Database shutdown

	// forwards
	class Attachment;
	class Transaction;
	class Request;
	class Blob;
	class Statement;
	class Service;

	// force use of default memory pool for Y-Valve objects
	class DefaultMemory
	{
	public:
		void* operator new(size_t size)
		{
			return getDefaultMemoryPool()->allocate(size);
		}
		
		void operator delete(void* mem)
		{
			getDefaultMemoryPool()->deallocate(mem);
		}
	};
	
	// stored handle types
	typedef Jrd::jrd_tra StoredTra;
	typedef void StoredReq;
	typedef void StoredBlb;
	typedef Jrd::Attachment StoredAtt;
	typedef dsql_req StoredStm;
	typedef void StoredSvc;

	template <typename CleanupRoutine, typename CleanupArg>
	class Clean : public DefaultMemory
	{
	private:
		struct st_clean
		{
			CleanupRoutine *Routine;
			void* clean_arg;
			st_clean(CleanupRoutine *r, void* a)
				: Routine(r), clean_arg(a) { }
			st_clean()
				: Routine(0), clean_arg(0) { }
		};
		Firebird::HalfStaticArray<st_clean, 1> calls;

	public:
		Clean() : calls(*getDefaultMemoryPool()) { }

		void add(CleanupRoutine *r, void* a)
		{
			for (size_t i = 0; i < calls.getCount(); ++i)
			{
				if (calls[i].Routine == r && 
					calls[i].clean_arg == a)
				{
					return;
				}
			}
			calls.add(st_clean(r, a));
		}

		void call(CleanupArg public_handle)
		{
			for (size_t i = 0; i < calls.getCount(); ++i)
			{
				if (calls[i].Routine)
				{
					THREAD_EXIT();
					calls[i].Routine(public_handle, calls[i].clean_arg);
					THREAD_ENTER();
				}
			}
		}
	};

	class BaseHandle : public DefaultMemory
	{
	public:
		UCHAR			type;
		UCHAR			flags;
		USHORT			implementation;
		FB_API_HANDLE	public_handle;
		Attachment*		parent;
    	FB_API_HANDLE*	user_handle;
		
	protected:
		BaseHandle(UCHAR t, FB_API_HANDLE* pub, Attachment* par, USHORT imp = ~0);
		
	public:
		static BaseHandle* translate(FB_API_HANDLE);
		Jrd::Attachment* getAttachmentHandle();
		void cancel();
		void release_user_handle();
		~BaseHandle();

		// required to put pointers to it into the tree
		static const FB_API_HANDLE& generate(const void* sender, BaseHandle* value) {
			return value->public_handle;
		}
	};
	
	template <typename HType>
		void toParent(Firebird::SortedArray<HType*>& members, HType* newMember)
	{
		members.add(newMember);
	}

	template <typename HType>
		void fromParent(Firebird::SortedArray<HType*>& members, HType* newMember)
	{
		size_t pos;
		if (members.find(newMember, pos))
		{
			members.remove(pos);
		}
#ifdef DEV_BUILD
		else
		{
			//Attempt to deregister not registered member
			fb_assert(false);
		}
#endif
	}

	template <typename ToHandle>
		ToHandle* translate(FB_API_HANDLE* handle)
	{
		if (handle && *handle)
		{
			BaseHandle* rc = BaseHandle::translate(*handle);
			if (rc && rc->type == ToHandle::hType())
			{
				return static_cast<ToHandle*>(rc);
			}
		}

		Firebird::status_exception::raise(ToHandle::hError(), 
										  isc_arg_end);
		// compiler warning silencer
		return 0;
	}

	class Attachment : public BaseHandle
	{
	public:
		Firebird::SortedArray<Transaction*> transactions;
		Firebird::SortedArray<Request*> requests;
		Firebird::SortedArray<Blob*> blobs;
		Firebird::SortedArray<Statement*> statements;

		Clean<AttachmentCleanupRoutine, FB_API_HANDLE*> cleanup;
		StoredAtt* handle;
		Firebird::PathName db_path;
		Firebird::Array<SCHAR> db_prepare_buffer;

		static ISC_STATUS hError()
		{
			return isc_bad_db_handle;
		}

		static UCHAR hType()
		{
			return 1;
		}

	public:
		Attachment(StoredAtt*, FB_API_HANDLE*, USHORT);
		void cancel2();
		~Attachment();
	};

	class Transaction : public BaseHandle
	{
	public:
		Clean<TransactionCleanupRoutine, FB_API_HANDLE> cleanup;
		Transaction* next;
		StoredTra* handle;

		static ISC_STATUS hError()
		{
			return isc_bad_trans_handle;
		}

		static UCHAR hType()
		{
			return 2;
		}

	public:
		Transaction(StoredTra* h, FB_API_HANDLE* pub, Attachment* par)
			: BaseHandle(hType(), pub, par), 
			  next(0), handle(h)
		{
			toParent<Transaction>(parent->transactions, this);
		}

		Transaction(FB_API_HANDLE* pub, USHORT a_implementation)
			: BaseHandle(hType(), pub, 0, a_implementation), 
			  next(0), handle(0)
		{
		}

		~Transaction()
		{
			cleanup.call(public_handle);
			if (parent)
			{
				fromParent<Transaction>(parent->transactions, this);
			}
		}
	};
	
	class Request : public BaseHandle
	{
	public:
		StoredReq* handle;

		static ISC_STATUS hError()
		{
			return isc_bad_req_handle;
		}

		static UCHAR hType()
		{
			return 3;
		}

	public:
		Request(StoredReq* h, FB_API_HANDLE* pub, Attachment* par)
			: BaseHandle(hType(), pub, par), handle(h)
		{
			toParent<Request>(parent->requests, this);
		}

		~Request() 
		{ 
			fromParent<Request>(parent->requests, this);
		}
	};
	
	class Blob : public BaseHandle
	{
	public:
		StoredBlb* handle;

		static ISC_STATUS hError()
		{
			return isc_bad_segstr_handle;
		}

		static UCHAR hType()
		{
			return 4;
		}

	public:
		Blob(StoredBlb* h, FB_API_HANDLE* pub, Attachment* par)
			: BaseHandle(hType(), pub, par), handle(h)
		{
			toParent<Blob>(parent->blobs, this);
		}

		~Blob() 
		{ 
			fromParent<Blob>(parent->blobs, this);
		}
	};
	
	class Statement : public BaseHandle
	{
	public:
		StoredStm* handle;
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
		Statement(StoredStm* h, FB_API_HANDLE* pub, Attachment* par)
			: BaseHandle(hType(), pub, par), handle(h)
		{
			toParent<Statement>(parent->statements, this);
			memset(&das, 0, sizeof das);
		}

		void checkPrepared()
		{
			if (!(flags & HANDLE_STATEMENT_prepared))
			{
				Firebird::status_exception::raise(isc_unprepared_stmt, isc_arg_end);
			}
		}

		~Statement() 
		{ 
			fromParent<Statement>(parent->statements, this);
		}
	};

	class Service : public BaseHandle
	{
	public:
		Clean<AttachmentCleanupRoutine, FB_API_HANDLE*> cleanup;
		StoredSvc* handle;

		static ISC_STATUS hError()
		{
			return isc_bad_svc_handle;
		}

		static UCHAR hType()
		{
			return 6;
		}

	public:
		Service(StoredSvc* h, FB_API_HANDLE* pub, USHORT impl)
			: BaseHandle(hType(), pub, 0, impl), handle(h)
			  
		{
		}

		~Service() 
		{ 
			cleanup.call(&public_handle);
		}
	};

}	

#endif // JRD_Y_HANDLE_H
