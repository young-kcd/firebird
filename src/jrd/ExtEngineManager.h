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
 *  The Original Code was created by Adriano dos Santos Fernandes
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2008 Adriano dos Santos Fernandes <adrianosf@uol.com.br>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#ifndef JRD_EXT_ENGINE_MANAGER_H
#define JRD_EXT_ENGINE_MANAGER_H

#include "FirebirdApi.h"
#include "FirebirdExternalApi.h"
#include "../jrd/common.h"
#include "../common/classes/array.h"
#include "../common/classes/fb_string.h"
#include "../common/classes/GenericMap.h"
#include "../common/classes/MetaName.h"
#include "../common/classes/auto.h"
#include "../common/classes/rwlock.h"

struct dsc;

namespace Jrd {

class thread_db;
class jrd_nod;
class jrd_prc;
class Attachment;
class Database;
class Format;
class Trigger;
class Function;
class ValueImpl;
class ValuesImpl;
struct impure_value;
struct record_param;


class ExtEngineManager : public Firebird::PermanentStorage
{
private:
	class AttachmentImpl;
	template <typename T> class ContextManager;
	class TransactionImpl;

	class ExternalContextImpl : public Firebird::ExternalContext
	{
	friend class AttachmentImpl;

	public:
		ExternalContextImpl(thread_db* tdbb, Firebird::ExternalEngine* aEngine);
		virtual ~ExternalContextImpl();

		void releaseTransaction();
		void setTransaction(thread_db* tdbb);

		virtual Firebird::ExternalEngine* FB_CALL getEngine(Firebird::Error* error);
		virtual Firebird::Attachment* FB_CALL getAttachment(Firebird::Error* error);
		virtual Firebird::Transaction* FB_CALL getTransaction(Firebird::Error* error);
		virtual const Firebird::Utf8* FB_CALL getClientCharSet();
		virtual int FB_CALL obtainInfoCode();
		virtual void* FB_CALL getInfo(int code);
		virtual void* FB_CALL setInfo(int code, void* value);

	private:
		Firebird::ExternalEngine* engine;
		Attachment* internalAttachment;
		Firebird::GenericMap<Firebird::NonPooled<int, void*> > miscInfo;
		FB_API_HANDLE traHandle;
		FB_API_HANDLE attHandle;
		Firebird::MetaName clientCharSet;
		Firebird::AutoPtr<AttachmentImpl> attachment;
		Firebird::AutoPtr<TransactionImpl> transaction;
	};

	struct EngineAttachment
	{
		EngineAttachment(Firebird::ExternalEngine* aEngine, Jrd::Attachment* aAttachment)
			: engine(aEngine),
			  attachment(aAttachment)
		{
		}

		static bool greaterThan(const EngineAttachment& i1, const EngineAttachment& i2)
		{
			return (i1.engine > i2.engine) ||
				(i1.engine == i2.engine && i1.attachment > i2.attachment);
		}

		Firebird::ExternalEngine* engine;
		Jrd::Attachment* attachment;
	};

	struct EngineAttachmentInfo
	{
		EngineAttachmentInfo()
			: engine(NULL),
			  context(NULL),
			  adminCharSet(0)
		{
		}

		Firebird::ExternalEngine* engine;
		Firebird::AutoPtr<ExternalContextImpl> context;
		USHORT adminCharSet;
	};

public:
	class Function
	{
	public:
		Function(thread_db* tdbb, ExtEngineManager* aExtManager,
			Firebird::ExternalEngine* aEngine,
			Firebird::ExternalFunction* aFunction,
			const Jrd::Function* aUdf);
		~Function();

		void execute(thread_db* tdbb, const jrd_nod* args, impure_value* impure);

	private:
		ExtEngineManager* extManager;
		Firebird::ExternalEngine* engine;
		Firebird::ExternalFunction* function;
		const Jrd::Function* udf;
		Database* database;
	};

	class ResultSet;

	class Procedure
	{
	public:
		Procedure(thread_db* tdbb, ExtEngineManager* aExtManager,
			Firebird::ExternalEngine* aEngine,
			Firebird::ExternalProcedure* aProcedure,
			const jrd_prc* aPrc);
		~Procedure();

		ResultSet* open(thread_db* tdbb, ValuesImpl* inputParams, ValuesImpl* outputParams) const;

	private:
		ExtEngineManager* extManager;
		Firebird::ExternalEngine* engine;
		Firebird::ExternalProcedure* procedure;
		const jrd_prc* prc;
		Database* database;

	friend class ResultSet;
	};

	class ResultSet
	{
	public:
		ResultSet(thread_db* tdbb, ValuesImpl* inputParams, ValuesImpl* outputParams,
			const Procedure* aProcedure);
		~ResultSet();

		bool fetch(thread_db* tdbb);

	private:
		const Procedure* procedure;
		Database* database;
		bool firstFetch;
		EngineAttachmentInfo* attInfo;
		Firebird::ExternalResultSet* resultSet;
		USHORT charSet;
	};

	class Trigger
	{
	public:
		Trigger(thread_db* tdbb, ExtEngineManager* aExtManager,
			Firebird::ExternalEngine* aEngine,
			Firebird::ExternalTrigger* aTrigger,
			const Jrd::Trigger* aTrg);
		~Trigger();

		void execute(thread_db* tdbb, Firebird::ExternalTrigger::Action action,
			record_param* oldRpb, record_param* newRpb);

	private:
		int setValues(thread_db* tdbb, Firebird::MemoryPool& pool,
			Firebird::AutoPtr<ValuesImpl>& values, Firebird::Array<dsc*>& descs,
			record_param* rpb);

		ExtEngineManager* extManager;
		Firebird::ExternalEngine* engine;
		Firebird::ExternalTrigger* trigger;
		const Jrd::Trigger* trg;
		Database* database;
	};

public:
	explicit ExtEngineManager(MemoryPool& p)
		: PermanentStorage(p),
		  engines(p),
		  enginesAttachments(p)
	{
	}

	~ExtEngineManager();

public:
	static void initialize();

public:
	void closeAttachment(thread_db* tdbb, Attachment* attachment);

	Function* makeFunction(thread_db* tdbb, const Jrd::Function* udf,
		const Firebird::MetaName& engine, const Firebird::string& entryPoint,
		const Firebird::string& body);
	Procedure* makeProcedure(thread_db* tdbb, const jrd_prc* prc,
		const Firebird::MetaName& engine, const Firebird::string& entryPoint,
		const Firebird::string& body);
	Trigger* makeTrigger(thread_db* tdbb, const Jrd::Trigger* trg,
		const Firebird::MetaName& engine, const Firebird::string& entryPoint,
		const Firebird::string& body, Firebird::ExternalTrigger::Type type);

private:
	Firebird::ExternalEngine* getEngine(thread_db* tdbb,
		const Firebird::MetaName& name);
	EngineAttachmentInfo* getEngineAttachment(thread_db* tdbb,
		const Firebird::MetaName& name);
	EngineAttachmentInfo* getEngineAttachment(thread_db* tdbb,
		Firebird::ExternalEngine* engine, bool closing = false);
	void setupAdminCharSet(thread_db* tdbb, Firebird::ExternalEngine* engine,
		EngineAttachmentInfo* attInfo);

private:
	typedef Firebird::GenericMap<Firebird::Pair<
		Firebird::Left<Firebird::MetaName, Firebird::ExternalEngine*> > > EnginesMap;
	typedef Firebird::GenericMap<Firebird::Pair<Firebird::NonPooled<
		EngineAttachment, EngineAttachmentInfo*> >, EngineAttachment> EnginesAttachmentsMap;

	Firebird::RWLock enginesLock;
	EnginesMap engines;
	EnginesAttachmentsMap enginesAttachments;
};


}	// namespace Jrd

#endif	// JRD_EXT_ENGINE_MANAGER_H
