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

#include "firebird.h"
#include "consts_pub.h"
#include "iberror.h"
#include "inf_pub.h"
#include "../jrd/ExtEngineManager.h"
#include "../jrd/ErrorImpl.h"
#include "../dsql/sqlda_pub.h"
#include "../common/dsc.h"
#include "../jrd/align.h"
#include "../jrd/jrd.h"
#include "../jrd/exe.h"
#include "../jrd/req.h"
#include "../jrd/status.h"
#include "../jrd/tra.h"
#include "../jrd/ibase.h"
#include "../common/os/path_utils.h"
#include "../jrd/cvt_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/intl_proto.h"
#include "../jrd/met_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/thread_proto.h"
#include "../jrd/Function.h"
#include "../common/isc_proto.h"
#include "../common/classes/auto.h"
#include "../common/classes/fb_string.h"
#include "../common/classes/init.h"
#include "../common/classes/objects_array.h"
#include "../common/config/config.h"
#include "../common/ScanDir.h"
#include "../common/utils_proto.h"
#include "../common/classes/GetPlugins.h"

using namespace Firebird;


namespace Jrd {

MakeUpgradeInfo<> upInfo;

template <typename T> class ExtEngineManager::ContextManager
{
public:
	ContextManager(thread_db* tdbb, EngineAttachmentInfo* aAttInfo, T* obj,
				CallerName aCallerName = CallerName())
		: attInfo(aAttInfo),
		  attachment(tdbb->getAttachment()),
		  transaction(tdbb->getTransaction()),
		  charSet(attachment->att_charset),
		  attInUse(attachment->att_in_use),
		  traInUse(transaction ? transaction->tra_in_use : false)
	{
		attachment->att_in_use = true;

		if (transaction)
		{
			callerName = transaction->tra_caller_name;
			transaction->tra_caller_name = aCallerName;
			++transaction->tra_callback_count;
			transaction->tra_in_use = true;
		}

		attInfo->context->setTransaction(tdbb);

		setCharSet(tdbb, attInfo, obj);
	}

	ContextManager(thread_db* tdbb, EngineAttachmentInfo* aAttInfo, USHORT aCharSet,
				CallerName aCallerName = CallerName())
		: attInfo(aAttInfo),
		  attachment(tdbb->getAttachment()),
		  transaction(tdbb->getTransaction()),
		  charSet(attachment->att_charset),
		  attInUse(attachment->att_in_use),
		  traInUse(transaction ? transaction->tra_in_use : false)
	{
		attachment->att_charset = aCharSet;
		attachment->att_in_use = true;

		if (transaction)
		{
			callerName = transaction->tra_caller_name;
			transaction->tra_caller_name = aCallerName;
			++transaction->tra_callback_count;
			transaction->tra_in_use = true;
		}

		attInfo->context->setTransaction(tdbb);
	}

	~ContextManager()
	{
		if (transaction)
		{
			--transaction->tra_callback_count;
			transaction->tra_in_use = traInUse;
			transaction->tra_caller_name = callerName;
		}

		attachment->att_in_use = attInUse;
		attachment->att_charset = charSet;
	}

private:
	void setCharSet(thread_db* tdbb, EngineAttachmentInfo* attInfo, T* obj)
	{
		attachment->att_charset = attInfo->adminCharSet;

		if (!obj)
			return;

		Utf8 charSetName[MAX_SQL_IDENTIFIER_SIZE];

		{	// scope
			Attachment::Checkout attCout(attachment);

			obj->getCharSet(RaiseError(), attInfo->context, charSetName, MAX_SQL_IDENTIFIER_LEN);
			charSetName[MAX_SQL_IDENTIFIER_LEN] = '\0';
		}

		USHORT charSetId;

		if (!MET_get_char_coll_subtype(tdbb, &charSetId,
				reinterpret_cast<const UCHAR*>(charSetName), strlen(charSetName)))
		{
			status_exception::raise(Arg::Gds(isc_charset_not_found) << Arg::Str(charSetName));
		}

		attachment->att_charset = charSetId;
	}

private:
	EngineAttachmentInfo* attInfo;
	Jrd::Attachment* attachment;
	jrd_tra* transaction;
	// These data members are to restore the original information.
	const USHORT charSet;
	const bool attInUse;
	const bool traInUse;
	CallerName callerName;
};


//---------------------


ExtEngineManager::ExternalContextImpl::ExternalContextImpl(thread_db* tdbb,
		ExternalEngine* aEngine)
	: engine(aEngine),
	  internalAttachment(tdbb->getAttachment()),
	  internalTransaction(NULL),
	  externalAttachment(NULL),
	  externalTransaction(NULL),
	  miscInfo(*internalAttachment->att_pool)
{
	//// TODO: admin rights

	clientCharSet = INTL_charset_lookup(tdbb, internalAttachment->att_client_charset)->getName();
}

ExtEngineManager::ExternalContextImpl::~ExternalContextImpl()
{
	releaseTransaction();
}

void ExtEngineManager::ExternalContextImpl::releaseTransaction()
{
	if (externalTransaction)
	{
		externalTransaction->release();
		externalTransaction = NULL;
	}

	if (externalAttachment)
	{
		externalAttachment->release();
		externalAttachment = NULL;
	}

	internalTransaction = NULL;
}

void ExtEngineManager::ExternalContextImpl::setTransaction(thread_db* tdbb)
{
	jrd_tra* newTransaction = tdbb->getTransaction();

	if (newTransaction == internalTransaction)
		return;

	releaseTransaction();
	fb_assert(!externalAttachment && !externalTransaction);

	MasterInterfacePtr master;

	if (internalAttachment)
	{
		internalAttachment->att_interface->addRef();

		externalAttachment = master->registerAttachment(JProvider::getInstance(),
			internalAttachment->att_interface);
	}

	if ((internalTransaction = newTransaction))
	{
		internalTransaction->getInterface()->addRef();

		externalTransaction = master->registerTransaction(externalAttachment,
			internalTransaction->getInterface());
	}
}

ExternalEngine* ExtEngineManager::ExternalContextImpl::getEngine(Error* /*error*/)
{
	return engine;
}

Firebird::IAttachment* FB_CALL ExtEngineManager::ExternalContextImpl::getAttachment(Error* /*error*/)
{
	return externalAttachment;
}

Firebird::ITransaction* FB_CALL ExtEngineManager::ExternalContextImpl::getTransaction(Error* /*error*/)
{
	return externalTransaction;
}

const char* FB_CALL ExtEngineManager::ExternalContextImpl::getUserName()
{
	return internalAttachment->att_user->usr_user_name.c_str();
}

const char* FB_CALL ExtEngineManager::ExternalContextImpl::getDatabaseName()
{
	return internalAttachment->att_database->dbb_database_name.c_str();
}

const Utf8* FB_CALL ExtEngineManager::ExternalContextImpl::getClientCharSet()
{
	return clientCharSet.c_str();
}

int FB_CALL ExtEngineManager::ExternalContextImpl::obtainInfoCode()
{
	static AtomicCounter counter;
	return ++counter;
}

void* FB_CALL ExtEngineManager::ExternalContextImpl::getInfo(int code)
{
	void* value = NULL;
	miscInfo.get(code, value);
	return value;
}

void* FB_CALL ExtEngineManager::ExternalContextImpl::setInfo(int code, void* value)
{
	void* oldValue = getInfo(code);
	miscInfo.put(code, value);
	return oldValue;
}


//---------------------


ExtEngineManager::Function::Function(thread_db* tdbb, ExtEngineManager* aExtManager,
		ExternalEngine* aEngine, RoutineMetadata* aMetadata, ExternalFunction* aFunction,
		const Jrd::Function* aUdf)
	: extManager(aExtManager),
	  engine(aEngine),
	  metadata(aMetadata),
	  function(aFunction),
	  udf(aUdf),
	  database(tdbb->getDatabase())
{
}


ExtEngineManager::Function::~Function()
{
	//Database::Checkout dcoHolder(database);
	function->dispose(LogError());
}


void ExtEngineManager::Function::execute(thread_db* tdbb, UCHAR* inMsg, UCHAR* outMsg) const
{
	EngineAttachmentInfo* attInfo = extManager->getEngineAttachment(tdbb, engine);
	ContextManager<ExternalFunction> ctxManager(tdbb, attInfo, function,
		(udf->getName().package.isEmpty() ?
			CallerName(obj_udf, udf->getName().identifier) :
			CallerName(obj_package_header, udf->getName().package)));

	Attachment::Checkout attCout(tdbb->getAttachment());

	function->execute(RaiseError(), attInfo->context, inMsg, outMsg);
}


//---------------------


ExtEngineManager::Procedure::Procedure(thread_db* tdbb, ExtEngineManager* aExtManager,
	    ExternalEngine* aEngine, RoutineMetadata* aMetadata, ExternalProcedure* aProcedure,
		const jrd_prc* aPrc)
	: extManager(aExtManager),
	  engine(aEngine),
	  metadata(aMetadata),
	  procedure(aProcedure),
	  prc(aPrc),
	  database(tdbb->getDatabase())
{
}


ExtEngineManager::Procedure::~Procedure()
{
	//Database::Checkout dcoHolder(database);
	procedure->dispose(LogError());
}


ExtEngineManager::ResultSet* ExtEngineManager::Procedure::open(thread_db* tdbb,
	UCHAR* inMsg, UCHAR* outMsg) const
{
	return FB_NEW(*tdbb->getDefaultPool()) ResultSet(tdbb, inMsg, outMsg, this);
}


//---------------------


ExtEngineManager::ResultSet::ResultSet(thread_db* tdbb, UCHAR* inMsg, UCHAR* outMsg,
		const ExtEngineManager::Procedure* aProcedure)
	: procedure(aProcedure),
	  attachment(tdbb->getAttachment()),
	  firstFetch(true)
{
	attInfo = procedure->extManager->getEngineAttachment(tdbb, procedure->engine);
	ContextManager<ExternalProcedure> ctxManager(tdbb, attInfo, procedure->procedure,
		(procedure->prc->getName().package.isEmpty() ?
			CallerName(obj_procedure, procedure->prc->getName().identifier) :
			CallerName(obj_package_header, procedure->prc->getName().package)));

	charSet = attachment->att_charset;

	Attachment::Checkout attCout(attachment);

	resultSet = procedure->procedure->open(RaiseError(), attInfo->context, inMsg, outMsg);
}


ExtEngineManager::ResultSet::~ResultSet()
{
	if (resultSet)
	{
		fb_assert(attachment == JRD_get_thread_data()->getAttachment());
		Attachment::Checkout attCout(attachment);
		resultSet->dispose(LogError());
	}
}


bool ExtEngineManager::ResultSet::fetch(thread_db* tdbb)
{
	bool wasFirstFetch = firstFetch;
	firstFetch = false;

	if (!resultSet)
		return wasFirstFetch;

	ContextManager<ExternalProcedure> ctxManager(tdbb, attInfo, charSet,
		(procedure->prc->getName().package.isEmpty() ?
			CallerName(obj_procedure, procedure->prc->getName().identifier) :
			CallerName(obj_package_header, procedure->prc->getName().package)));

	fb_assert(attachment == tdbb->getAttachment());
	Attachment::Checkout attCout(attachment);
	return resultSet->fetch(RaiseError());
}


//---------------------


ExtEngineManager::Trigger::Trigger(thread_db* tdbb, ExtEngineManager* aExtManager,
			ExternalEngine* aEngine, RoutineMetadata* aMetadata, ExternalTrigger* aTrigger,
			const Jrd::Trigger* aTrg)
	: extManager(aExtManager),
	  engine(aEngine),
	  metadata(aMetadata),
	  trigger(aTrigger),
	  trg(aTrg),
	  database(tdbb->getDatabase())
{
}


ExtEngineManager::Trigger::~Trigger()
{
	// hvlad: shouldn't we call trigger->dispose() here ?
}


void ExtEngineManager::Trigger::execute(thread_db* tdbb, ExternalTrigger::Action action,
	record_param* oldRpb, record_param* newRpb) const
{
	EngineAttachmentInfo* attInfo = extManager->getEngineAttachment(tdbb, engine);
	ContextManager<ExternalTrigger> ctxManager(tdbb, attInfo, trigger,
		CallerName(obj_trigger, trg->name));

	// ASF: Using Array instead of HalfStaticArray to not need to do alignment hacks here.
	Array<UCHAR> oldMsg;
	Array<UCHAR> newMsg;

	if (oldRpb)
		setValues(tdbb, oldMsg, oldRpb);

	if (newRpb)
		setValues(tdbb, newMsg, newRpb);

	Attachment::Checkout attCout(tdbb->getAttachment());

	trigger->execute(RaiseError(), attInfo->context, action,
		(oldRpb ? oldMsg.begin() : NULL), (newRpb ? newMsg.begin() : NULL));

	if (newRpb)
	{
		Record* record = newRpb->rpb_record;
		const Format* format = record->rec_format;
		const UCHAR* msg = newMsg.begin();
		unsigned pos = 0;

		for (unsigned i = 0; i < format->fmt_count; ++i)
		{
			if (format->fmt_desc[i].dsc_dtype == dtype_unknown)
				continue;

			dsc desc;
			bool readonly = !EVL_field(newRpb->rpb_relation, record, i, &desc) &&
				desc.dsc_address && !(desc.dsc_flags & DSC_null);

			unsigned align = type_alignments[desc.dsc_dtype];
			if (align)
				pos = FB_ALIGN(pos, align);

			const unsigned dataPos = pos;
			pos += desc.dsc_length;

			align = type_alignments[dtype_short];
			if (align)
				pos = FB_ALIGN(pos, align);

			if (!readonly)
			{
				if (*(USHORT*) &msg[pos])
					SET_NULL(record, i);
				else
				{
					memcpy(desc.dsc_address, msg + dataPos, desc.dsc_length);
					CLEAR_NULL(record, i);
				}
			}

			pos += sizeof(USHORT);
		}
	}
}


void ExtEngineManager::Trigger::setValues(thread_db* /*tdbb*/, Array<UCHAR>& msgBuffer,
	record_param* rpb) const
{
	if (!rpb || !rpb->rpb_record)
		return;

	Record* record = rpb->rpb_record;
	const Format* format = record->rec_format;

	for (unsigned i = 0; i < format->fmt_count; ++i)
	{
		if (format->fmt_desc[i].dsc_dtype == dtype_unknown)
			continue;

		dsc desc;
		EVL_field(rpb->rpb_relation, record, i, &desc);
		// CVC: I'm not sure why it's not important to check EVL_field's result.

		unsigned align = type_alignments[desc.dsc_dtype];
		if (align)
			msgBuffer.resize(FB_ALIGN(msgBuffer.getCount(), align));

		msgBuffer.add(desc.dsc_address, desc.dsc_length);

		align = type_alignments[dtype_short];
		if (align)
			msgBuffer.resize(FB_ALIGN(msgBuffer.getCount(), align));

		USHORT nullFlag = (desc.dsc_flags & DSC_null) != 0;
		msgBuffer.add((UCHAR*) &nullFlag, sizeof(nullFlag));
	}
}


//---------------------


ExtEngineManager::~ExtEngineManager()
{
	fb_assert(enginesAttachments.count() == 0);
/*
AP: Commented out this code due to later AV.

When engine is released, it does dlclose() plugin module (libudr_engine.so),
but that module is not actually unloaded - because UDR module (libudrcpp_example.so) is using
symbols from plugin module, therefore raising plugin module's reference count.
UDR module can be unloaded only from plugin module's global variable (ModuleMap modules) dtor,
which is not called as long as plugin module is not unloaded. As the result all this will be
unloaded only on program exit, causing at that moment AV if this code is active: it happens that
~ModuleMap dlcloses itself.

	PluginManagerInterfacePtr pi;

	EnginesMap::Accessor accessor(&engines);
	for (bool found = accessor.getFirst(); found; found = accessor.getNext())
	{
		ExternalEngine* engine = accessor.current()->second;
		pi->releasePlugin(engine);
	}
 */
}


//---------------------


void ExtEngineManager::initialize()
{
}


void ExtEngineManager::closeAttachment(thread_db* tdbb, Attachment* attachment)
{
	Array<ExternalEngine*> enginesCopy;

	{	// scope
		ReadLockGuard readGuard(enginesLock);

		EnginesMap::Accessor accessor(&engines);
		for (bool found = accessor.getFirst(); found; found = accessor.getNext())
			enginesCopy.add(accessor.current()->second);
	}

	Attachment::Checkout attCout(attachment, true);

	for (Array<ExternalEngine*>::iterator i = enginesCopy.begin(); i != enginesCopy.end(); ++i)
	{
		ExternalEngine* engine = *i;
		EngineAttachmentInfo* attInfo = getEngineAttachment(tdbb, engine, true);

		if (attInfo)
		{
			{	// scope
				ContextManager<ExternalFunction> ctxManager(tdbb, attInfo, attInfo->adminCharSet);
				engine->closeAttachment(LogError(), attInfo->context);
			}

			delete attInfo;
		}
	}
}


ExtEngineManager::Function* ExtEngineManager::makeFunction(thread_db* tdbb, const Jrd::Function* udf,
	const MetaName& engine, const string& entryPoint, const string& body,
	BlrMessage* inBlr, BlrMessage* outBlr)
{
	string entryPointTrimmed = entryPoint;
	entryPointTrimmed.trim();

	EngineAttachmentInfo* attInfo = getEngineAttachment(tdbb, engine);
	ContextManager<ExternalFunction> ctxManager(tdbb, attInfo, attInfo->adminCharSet,
		(udf->getName().package.isEmpty() ?
			CallerName(obj_udf, udf->getName().identifier) :
			CallerName(obj_package_header, udf->getName().package)));

	///MemoryPool& pool = *tdbb->getDefaultPool();
	MemoryPool& pool = *getDefaultMemoryPool();

	AutoPtr<RoutineMetadata> metadata(FB_NEW(pool) RoutineMetadata(pool));
	metadata->package = udf->getName().package;
	metadata->name = udf->getName().identifier;
	metadata->entryPoint = entryPointTrimmed;
	metadata->body = body;
	metadata->inputParameters = FB_NEW(pool) StatementMetadata::Parameters(pool);
	metadata->outputParameters = FB_NEW(pool) StatementMetadata::Parameters(pool);

	for (Array<Jrd::Function::Argument>::const_iterator i = udf->fun_args.begin();
		 i != udf->fun_args.end();
		 ++i)
	{
		SLONG sqlLen, sqlSubType, sqlScale, sqlType;
		i->fun_parameter->prm_desc.getSqlInfo(&sqlLen, &sqlSubType, &sqlScale, &sqlType);

		StatementMetadata::Parameters::Item& item = i == udf->fun_args.begin() ?
			metadata->outputParameters->items.add() :
			metadata->inputParameters->items.add();

		item.field = i->fun_parameter->prm_name.c_str();
		item.type = sqlType;
		item.subType = sqlSubType;
		item.length = sqlLen;
		item.scale = sqlScale;
		item.nullable = i->fun_parameter->prm_nullable;
	}

	ExternalFunction* externalFunction;

	{	// scope
		Attachment::Checkout attCout(tdbb->getAttachment());

		externalFunction = attInfo->engine->makeFunction(RaiseError(), attInfo->context, metadata,
			inBlr, outBlr);

		if (!externalFunction)
		{
			status_exception::raise(
				Arg::Gds(isc_eem_func_not_returned) <<
					udf->getName().toString() << engine);
		}
	}

	try
	{
		return FB_NEW(getPool()) Function(tdbb, this, attInfo->engine, metadata.release(),
			externalFunction, udf);
	}
	catch (...)
	{
		Attachment::Checkout attCout(tdbb->getAttachment());
		externalFunction->dispose(LogError());
		throw;
	}
}


ExtEngineManager::Procedure* ExtEngineManager::makeProcedure(thread_db* tdbb, const jrd_prc* prc,
	const MetaName& engine, const string& entryPoint, const string& body,
	BlrMessage* inBlr, BlrMessage* outBlr)
{
	string entryPointTrimmed = entryPoint;
	entryPointTrimmed.trim();

	EngineAttachmentInfo* attInfo = getEngineAttachment(tdbb, engine);
	ContextManager<ExternalProcedure> ctxManager(tdbb, attInfo, attInfo->adminCharSet,
		(prc->getName().package.isEmpty() ?
			CallerName(obj_procedure, prc->getName().identifier) :
			CallerName(obj_package_header, prc->getName().package)));

	///MemoryPool& pool = *tdbb->getDefaultPool();
	MemoryPool& pool = *getDefaultMemoryPool();

	AutoPtr<RoutineMetadata> metadata(FB_NEW(pool) RoutineMetadata(pool));
	metadata->package = prc->getName().package;
	metadata->name = prc->getName().identifier;
	metadata->entryPoint = entryPointTrimmed;
	metadata->body = body;
	metadata->inputParameters = FB_NEW(pool) StatementMetadata::Parameters(pool);
	metadata->outputParameters = FB_NEW(pool) StatementMetadata::Parameters(pool);

	const Array<NestConst<Parameter> >* parameters[] = {&prc->prc_input_fields, &prc->prc_output_fields};

	for (unsigned i = 0; i < 2; ++i)
	{
		for (Array<NestConst<Parameter> >::const_iterator j = parameters[i]->begin();
			 j != parameters[i]->end();
			 ++j)
		{
			SLONG sqlLen, sqlSubType, sqlScale, sqlType;
			(*j)->prm_desc.getSqlInfo(&sqlLen, &sqlSubType, &sqlScale, &sqlType);

			StatementMetadata::Parameters::Item& item = i == 1 ?
				metadata->outputParameters->items.add() :
				metadata->inputParameters->items.add();

			item.field = (*j)->prm_name.c_str();
			item.type = sqlType;
			item.subType = sqlSubType;
			item.length = sqlLen;
			item.scale = sqlScale;
			item.nullable = (*j)->prm_nullable;
		}
	}

	ExternalProcedure* externalProcedure;

	{	// scope
		Attachment::Checkout attCout(tdbb->getAttachment());

		externalProcedure = attInfo->engine->makeProcedure(RaiseError(), attInfo->context, metadata,
			inBlr, outBlr);

		if (!externalProcedure)
		{
			status_exception::raise(
				Arg::Gds(isc_eem_proc_not_returned) <<
					prc->getName().toString() << engine);
		}
	}

	try
	{
		return FB_NEW(getPool()) Procedure(tdbb, this, attInfo->engine, metadata.release(),
			externalProcedure, prc);
	}
	catch (...)
	{
		Attachment::Checkout attCout(tdbb->getAttachment());
		externalProcedure->dispose(LogError());
		throw;
	}
}


ExtEngineManager::Trigger* ExtEngineManager::makeTrigger(thread_db* tdbb, const Jrd::Trigger* trg,
	const MetaName& engine, const string& entryPoint, const string& body, ExternalTrigger::Type type)
{
	string entryPointTrimmed = entryPoint;
	entryPointTrimmed.trim();

	EngineAttachmentInfo* attInfo = getEngineAttachment(tdbb, engine);
	ContextManager<ExternalTrigger> ctxManager(tdbb, attInfo, attInfo->adminCharSet,
		CallerName(obj_trigger, trg->name));

	///MemoryPool& pool = *tdbb->getDefaultPool();
	MemoryPool& pool = *getDefaultMemoryPool();

	AutoPtr<RoutineMetadata> metadata(FB_NEW(pool) RoutineMetadata(pool));
	metadata->name = trg->name;
	metadata->entryPoint = entryPointTrimmed;
	metadata->body = body;
	metadata->triggerType = type;

	jrd_rel* relation = trg->relation;

	if (relation)
	{
		metadata->triggerTable = relation->rel_name;
		metadata->triggerFields = FB_NEW(pool) StatementMetadata::Parameters(pool);

		Format* relFormat = relation->rel_current_format;

		for (size_t i = 0; i < relation->rel_fields->count(); ++i)
		{
			jrd_fld* field = (*relation->rel_fields)[i];
			if (!field)
				continue;

			SLONG sqlLen, sqlSubType, sqlScale, sqlType;
			relFormat->fmt_desc[i].getSqlInfo(&sqlLen, &sqlSubType, &sqlScale, &sqlType);

			StatementMetadata::Parameters::Item& item = metadata->triggerFields->items.add();
			item.field = field->fld_name.c_str();
			item.type = sqlType;
			item.subType = sqlSubType;
			item.length = sqlLen;
			item.scale = sqlScale;
			item.nullable = !field->fld_not_null;
		}
	}

	ExternalTrigger* externalTrigger;

	{	// scope
		Attachment::Checkout attCout(tdbb->getAttachment());

		externalTrigger = attInfo->engine->makeTrigger(RaiseError(), attInfo->context, metadata);

		if (!externalTrigger)
		{
			status_exception::raise(
				Arg::Gds(isc_eem_trig_not_returned) << trg->name << engine);
		}
	}

	try
	{
		return FB_NEW(getPool()) Trigger(tdbb, this, attInfo->engine, metadata.release(),
			externalTrigger, trg);
	}
	catch (...)
	{
		Attachment::Checkout attCout(tdbb->getAttachment());
		externalTrigger->dispose(LogError());
		throw;
	}
}


ExternalEngine* ExtEngineManager::getEngine(thread_db* tdbb, const MetaName& name)
{
	ReadLockGuard readGuard(enginesLock);
	ExternalEngine* engine = NULL;

	if (!engines.get(name, engine))
	{
		readGuard.release();
		WriteLockGuard writeGuard(enginesLock);

		if (!engines.get(name, engine))
		{
			GetPlugins<ExternalEngine> engineControl(PluginType::ExternalEngine,
				FB_EXTERNAL_ENGINE_VERSION, upInfo, name.c_str());

			if (engineControl.hasData())
			{
				EngineAttachment key(NULL, NULL);
				AutoPtr<EngineAttachmentInfo> attInfo;

				try
				{
					Attachment::Checkout attCout(tdbb->getAttachment());

					engine = engineControl.plugin();
					if (engine)
					{
						Attachment::SyncGuard attGuard(tdbb->getAttachment());

						key = EngineAttachment(engine, tdbb->getAttachment());
						attInfo = FB_NEW(getPool()) EngineAttachmentInfo();
						attInfo->engine = engine;
						attInfo->context = FB_NEW(getPool()) ExternalContextImpl(tdbb, engine);

						setupAdminCharSet(tdbb, engine, attInfo);

						ContextManager<ExternalFunction> ctxManager(tdbb, attInfo, attInfo->adminCharSet);
						engine->openAttachment(LogError(), attInfo->context);
					}
				}
				catch (...)
				{
					if (engine)
					{
						PluginManagerInterfacePtr()->releasePlugin(engine);
					}

					throw;
				}

				if (engine)
				{
					engine->addRef();
					engines.put(name, engine);
					enginesAttachments.put(key, attInfo);
					attInfo.release();
				}
			}
		}
	}

	if (!engine)
	{
		status_exception::raise(Arg::Gds(isc_eem_engine_notfound) << name);
	}

	return engine;
}


ExtEngineManager::EngineAttachmentInfo* ExtEngineManager::getEngineAttachment(
	thread_db* tdbb, const MetaName& name)
{
	ExternalEngine* engine = getEngine(tdbb, name);
	return getEngineAttachment(tdbb, engine);
}


ExtEngineManager::EngineAttachmentInfo* ExtEngineManager::getEngineAttachment(
	thread_db* tdbb, ExternalEngine* engine, bool closing)
{
	EngineAttachment key(engine, tdbb->getAttachment());
	EngineAttachmentInfo* attInfo = NULL;

	ReadLockGuard readGuard(&enginesLock);

	if (!enginesAttachments.get(key, attInfo) && !closing)
	{
		readGuard.release();
		WriteLockGuard writeGuard(enginesLock);

		if (!enginesAttachments.get(key, attInfo))
		{
			attInfo = FB_NEW(getPool()) EngineAttachmentInfo();
			attInfo->engine = engine;
			attInfo->context = FB_NEW(getPool()) ExternalContextImpl(tdbb, engine);

			setupAdminCharSet(tdbb, engine, attInfo);

			enginesAttachments.put(key, attInfo);

			ContextManager<ExternalFunction> ctxManager(tdbb, attInfo, attInfo->adminCharSet);
			Attachment::Checkout attCout(tdbb->getAttachment());
			engine->openAttachment(LogError(), attInfo->context);
		}

		return attInfo;
	}

	if (closing && attInfo)
	{
		readGuard.release();
		WriteLockGuard writeGuard(enginesLock);
		enginesAttachments.remove(key);
	}

	return attInfo;
}


void ExtEngineManager::setupAdminCharSet(thread_db* tdbb, ExternalEngine* engine,
	EngineAttachmentInfo* attInfo)
{
	ContextManager<ExternalFunction> ctxManager(tdbb, attInfo, CS_UTF8);

	Utf8 charSetName[MAX_SQL_IDENTIFIER_SIZE] = "NONE";
	engine->open(RaiseError(), attInfo->context, charSetName,
		MAX_SQL_IDENTIFIER_LEN);
	charSetName[MAX_SQL_IDENTIFIER_LEN] = '\0';

	if (!MET_get_char_coll_subtype(tdbb, &attInfo->adminCharSet,
			reinterpret_cast<const UCHAR*>(charSetName),
			strlen(charSetName)))
	{
		status_exception::raise(
			Arg::Gds(isc_charset_not_found) <<
			Arg::Str(charSetName));
	}
}


}	// namespace Jrd
