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
#include "../jrd/ValueImpl.h"
#include "../jrd/ValuesImpl.h"
#include "../dsql/sqlda_pub.h"
#include "../jrd/dsc.h"
#include "../jrd/jrd.h"
#include "../jrd/exe.h"
#include "../jrd/req.h"
#include "../jrd/status.h"
#include "../jrd/tra.h"
#include "../jrd/PluginManager.h"
#include "../jrd/ibase.h"
#include "../jrd/os/path_utils.h"
#include "../jrd/cvt_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/intl_proto.h"
#include "../jrd/met_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/thread_proto.h"
#include "../jrd/Function.h"
#include "../jrd/isc_proto.h"
#include "../common/classes/auto.h"
#include "../common/classes/fb_string.h"
#include "../common/classes/init.h"
#include "../common/classes/objects_array.h"
#include "../common/config/config.h"
#include "../config/ScanDir.h"

using namespace Firebird;


namespace Jrd {


class ExtEngineManager::AttachmentImpl : public Firebird::Attachment
{
public:
	AttachmentImpl(ExternalContextImpl* aContext, Handle aHandle, Jrd::Attachment* aAttachment);
	virtual ~AttachmentImpl();

public:
	virtual void FB_CALL dispose(Error* error);

	virtual Handle FB_CALL getHandle(Error* error) const;
	virtual const char* FB_CALL getUserName() const;
	virtual const char* FB_CALL getDatabaseName() const;

private:
	ExternalContextImpl* context;
	FB_API_HANDLE handle;
	Jrd::Attachment* attachment;
};


class ExtEngineManager::TransactionImpl : public Firebird::Transaction
{
public:
	TransactionImpl(Handle aHandle);
	virtual ~TransactionImpl();

public:
	virtual Handle FB_CALL getHandle(Error* error) const;

private:
	FB_API_HANDLE handle;
};


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
			Database::Checkout dcoHolder(tdbb->getDatabase());

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


ExtEngineManager::AttachmentImpl::AttachmentImpl(ExternalContextImpl* aContext, Handle aHandle,
		Jrd::Attachment* aAttachment)
	: context(aContext),
	  handle(aHandle),
	  attachment(aAttachment)
{
}

ExtEngineManager::AttachmentImpl::~AttachmentImpl()
{
	context->attachment.release();
	handle = 0;
	dispose(LogError());
}

void FB_CALL ExtEngineManager::AttachmentImpl::dispose(Error* error)
{
	ISC_STATUS_ARRAY statusVector;

	if (handle)
	{
		if (isc_detach_database(statusVector, &handle) != 0)
		{
			ErrorImpl::statusVectorToError(statusVector, error);
			return;
		}
	}

	context->attachment = NULL;
}

Handle FB_CALL ExtEngineManager::AttachmentImpl::getHandle(Error* /*error*/) const
{
	return handle;
}


const char* FB_CALL ExtEngineManager::AttachmentImpl::getUserName() const
{
	return attachment->att_user->usr_user_name.c_str();
}


const char* FB_CALL ExtEngineManager::AttachmentImpl::getDatabaseName() const
{
	return attachment->att_database->dbb_database_name.c_str();
}


//---------------------


ExtEngineManager::TransactionImpl::TransactionImpl(Handle aHandle)
	: handle(aHandle)
{
}

ExtEngineManager::TransactionImpl::~TransactionImpl()
{
}

Handle FB_CALL ExtEngineManager::TransactionImpl::getHandle(Error* /*error*/) const
{
	return handle;
}


//---------------------


ExtEngineManager::ExternalContextImpl::ExternalContextImpl(thread_db* tdbb,
		ExternalEngine* aEngine)
	: engine(aEngine),
	  internalAttachment(tdbb->getAttachment()),
	  miscInfo(*internalAttachment->att_pool),
	  traHandle(0)
{
	//// TODO: admin rights

	attHandle = internalAttachment->att_public_handle;
	clientCharSet = INTL_charset_lookup(tdbb, internalAttachment->att_client_charset)->getName();

	setTransaction(tdbb);
}

ExtEngineManager::ExternalContextImpl::~ExternalContextImpl()
{
	releaseTransaction();
}

void ExtEngineManager::ExternalContextImpl::releaseTransaction()
{
	if (traHandle)
	{
		traHandle = 0;
		transaction = NULL;
	}
}

void ExtEngineManager::ExternalContextImpl::setTransaction(thread_db* tdbb)
{
	releaseTransaction();

	jrd_tra* tra = tdbb->getTransaction();
	traHandle = tra ? tra->tra_public_handle : 0;

	transaction = FB_NEW(*internalAttachment->att_pool) TransactionImpl(traHandle);
}

ExternalEngine* ExtEngineManager::ExternalContextImpl::getEngine(Error* /*error*/)
{
	return engine;
}

Firebird::Attachment* FB_CALL ExtEngineManager::ExternalContextImpl::getAttachment(Error* /*error*/)
{
	if (!this->attachment)
	{
		thread_db* tdbb = JRD_get_thread_data();
		attachment = FB_NEW(*internalAttachment->att_pool) AttachmentImpl(this, attHandle,
			tdbb->getAttachment());
	}

	return attachment;
}

Firebird::Transaction* FB_CALL ExtEngineManager::ExternalContextImpl::getTransaction(Error* /*error*/)
{
	return transaction;
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


static InitInstance<GenericMap<Pair<Full<MetaName, ConfigFile::String> > > > enginesModules;


//---------------------


ExtEngineManager::Function::Function(thread_db* tdbb, ExtEngineManager* aExtManager,
		ExternalEngine* aEngine, ExternalFunction* aFunction,
		const Jrd::Function* aUdf)
	: extManager(aExtManager),
	  engine(aEngine),
	  function(aFunction),
	  udf(aUdf),
	  database(tdbb->getDatabase())
{
}


ExtEngineManager::Function::~Function()
{
	Database::Checkout dcoHolder(database);
	function->dispose(LogError());
}


void ExtEngineManager::Function::execute(thread_db* tdbb, const jrd_nod* args, impure_value* impure) const
{
	EngineAttachmentInfo* attInfo = extManager->getEngineAttachment(tdbb, engine);
	ContextManager<ExternalFunction> ctxManager(tdbb, attInfo, function,
		(udf->getName().package.isEmpty() ?
			CallerName(obj_udf, udf->getName().identifier) :
			CallerName(obj_package_header, udf->getName().package)));

	impure->vlu_desc.dsc_flags = DSC_null;
	MemoryPool& pool = *tdbb->getDefaultPool();
	ValueImpl result(pool, &impure->vlu_desc, "", true);

	HalfStaticArray<impure_value, 32> impureArgs;

	impure_value* impureArgsPtr = impureArgs.getBuffer(args->nod_count);
	try
	{
		ValuesImpl params(pool, args->nod_count);

		for (int i = 0; i < args->nod_count; ++i)
		{
			impureArgsPtr->vlu_desc = udf->fun_args[i + 1].fun_parameter->prm_desc;

			if (impureArgsPtr->vlu_desc.isText())
			{
				impureArgsPtr->vlu_string =
					FB_NEW_RPT(pool, impureArgsPtr->vlu_desc.getStringLength()) VaryingString();
				impureArgsPtr->vlu_desc.dsc_address = (UCHAR*) impureArgsPtr->vlu_string;
			}
			else
			{
				impureArgsPtr->vlu_string = NULL;
				impureArgsPtr->vlu_desc.dsc_address = (UCHAR*) &impureArgsPtr->vlu_misc;
			}

			dsc* arg = EVL_expr(tdbb, args->nod_arg[i]);

			if (tdbb->getRequest()->req_flags & req_null)
				impureArgsPtr->vlu_desc.dsc_flags = DSC_null;
			else
			{
				MOV_move(tdbb, arg, &impureArgsPtr->vlu_desc);
				INTL_adjust_text_descriptor(tdbb, &impureArgsPtr->vlu_desc);
			}

			params.getValue(i + 1)->make(&impureArgsPtr->vlu_desc, "", true);

			++impureArgsPtr;
		}

		{	// scope
			Database::Checkout dcoHolder(tdbb->getDatabase());
			function->execute(RaiseError(), attInfo->context, &params, &result);
		}
	}
	catch (...)
	{
		for (int i = 0; i < args->nod_count; ++i)
			delete impureArgs[i].vlu_string;

		throw;
	}

	for (int i = 0; i < args->nod_count; ++i)
		delete impureArgs[i].vlu_string;

	if (result.isNull())
		tdbb->getRequest()->req_flags |= req_null;
	else
		tdbb->getRequest()->req_flags &= ~req_null;
}


//---------------------


ExtEngineManager::Procedure::Procedure(thread_db* tdbb, ExtEngineManager* aExtManager,
	    ExternalEngine* aEngine, ExternalProcedure* aProcedure,
		const jrd_prc* aPrc)
	: extManager(aExtManager),
	  engine(aEngine),
	  procedure(aProcedure),
	  prc(aPrc),
	  database(tdbb->getDatabase())
{
}


ExtEngineManager::Procedure::~Procedure()
{
	Database::Checkout dcoHolder(database);
	procedure->dispose(LogError());
}


ExtEngineManager::ResultSet* ExtEngineManager::Procedure::open(thread_db* tdbb,
	ValuesImpl* inputParams, ValuesImpl* outputParams) const
{
	return FB_NEW(*tdbb->getDefaultPool()) ResultSet(tdbb, inputParams, outputParams, this);
}


//---------------------


ExtEngineManager::ResultSet::ResultSet(thread_db* tdbb, ValuesImpl* inputParams,
		ValuesImpl* outputParams, const ExtEngineManager::Procedure* aProcedure)
	: procedure(aProcedure),
	  database(tdbb->getDatabase()),
	  firstFetch(true)
{
	attInfo = procedure->extManager->getEngineAttachment(tdbb, procedure->engine);
	ContextManager<ExternalProcedure> ctxManager(tdbb, attInfo, procedure->procedure,
		(procedure->prc->getName().package.isEmpty() ?
			CallerName(obj_procedure, procedure->prc->getName().identifier) :
			CallerName(obj_package_header, procedure->prc->getName().package)));
	Attachment* attachment = tdbb->getAttachment();

	charSet = attachment->att_charset;

	Database::Checkout dcoHolder(tdbb->getDatabase());

	resultSet = procedure->procedure->open(RaiseError(), attInfo->context, inputParams,
		outputParams);
}


ExtEngineManager::ResultSet::~ResultSet()
{
	if (resultSet)
	{
		Database::Checkout dcoHolder(database);
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

	Database::Checkout dcoHolder(tdbb->getDatabase());
	return resultSet->fetch(RaiseError());
}


//---------------------


ExtEngineManager::Trigger::Trigger(thread_db* tdbb, ExtEngineManager* aExtManager,
			ExternalEngine* aEngine, ExternalTrigger* aTrigger,
			const Jrd::Trigger* aTrg)
	: extManager(aExtManager),
	  engine(aEngine),
	  trigger(aTrigger),
	  trg(aTrg),
	  database(tdbb->getDatabase())
{
}


ExtEngineManager::Trigger::~Trigger()
{
}


void ExtEngineManager::Trigger::execute(thread_db* tdbb, ExternalTrigger::Action action,
	record_param* oldRpb, record_param* newRpb) const
{
	EngineAttachmentInfo* attInfo = extManager->getEngineAttachment(tdbb, engine);
	ContextManager<ExternalTrigger> ctxManager(tdbb, attInfo, trigger,
		CallerName(obj_trigger, trg->name));

	Array<dsc*> descs;
	try
	{
		MemoryPool& pool = *tdbb->getDefaultPool();
		AutoPtr<ValuesImpl> oldValues, newValues;
		int valueOldCount = 0;
		int valueNewCount = 0;

		if (oldRpb)
			valueOldCount = setValues(tdbb, pool, oldValues, descs, oldRpb);

		if (newRpb)
			valueNewCount = setValues(tdbb, pool, newValues, descs, newRpb);

		{	// scope
			Database::Checkout dcoHolder(tdbb->getDatabase());

			trigger->execute(RaiseError(), attInfo->context, action, oldValues, newValues);

			for (int i = 1; i <= valueNewCount; ++i)
			{
				ValueImpl* val = newValues->getValue(i);

				if (val->isNull())
					SET_NULL(newRpb->rpb_record, val->getFieldNumber());
				else
					CLEAR_NULL(newRpb->rpb_record, val->getFieldNumber());
			}
		}
	}
	catch (...)
	{
		for (size_t i = 0; i < descs.getCount(); ++i)
			delete descs[i];
		throw;
	}

	for (size_t i = 0; i < descs.getCount(); ++i)
		delete descs[i];
}


int ExtEngineManager::Trigger::setValues(thread_db* tdbb, MemoryPool& pool,
	AutoPtr<ValuesImpl>& values, Array<dsc*>& descs,
	record_param* rpb) const
{
	if (!rpb || !rpb->rpb_record)
		return 0;

	Record* record = rpb->rpb_record;
	const Format* format = record->rec_format;

	values = FB_NEW(pool) ValuesImpl(pool, format->fmt_count);

	int start = descs.getCount();
	descs.resize(start + format->fmt_count);

	int j = 0;

	for (int i = 0; i < format->fmt_count; ++i)
	{
		descs[start + i] = FB_NEW(pool) dsc;

		if (format->fmt_desc[i].dsc_dtype != dtype_unknown)
		{
			EVL_field(rpb->rpb_relation, record, i, descs[start + i]);

			jrd_fld* field = (*rpb->rpb_relation->rel_fields)[i];
			fb_assert(field);

			values->getValue(j + 1)->make(descs[start + i], field->fld_name, true, i);
			++j;
		}
	}

	return j;
}


//---------------------


ExtEngineManager::~ExtEngineManager()
{
	fb_assert(enginesAttachments.count() == 0);

	EnginesMap::Accessor accessor(&engines);
	for (bool found = accessor.getFirst(); found; found = accessor.getNext())
	{
		ExternalEngine* engine = accessor.current()->second;
		engine->dispose(LogError());
	}
}


//---------------------


void ExtEngineManager::initialize()
{
	PathName pluginsPath = PluginManager::getPluginsDirectory();
	ScanDir dir(pluginsPath.c_str(), "*.conf");

	try
	{
		SortedArray<MetaName> conflicts;

		while (dir.next())
		{
			ConfigFile configFile(dir.getFilePath(),
				ConfigFile::EXCEPTION_ON_ERROR | ConfigFile::HAS_SUB_CONF);

			const ConfigFile::Parameters& params = configFile.getParameters();
			for (size_t n = 0; n < params.getCount(); ++n)
			{
				const ConfigFile::Parameter* el = &params[n];
				if (el->name == "external_engine")
				{
					MetaName name(el->value);

					if (enginesModules().exist(name) || conflicts.exist(name))
					{
						string s;
						s.printf("External engine %s defined more than once.", name.c_str());
						gds__log(s.c_str());

						conflicts.add(name);
						continue;
					}

					const ConfigFile::Parameter* plugin = el->sub->findParameter("plugin_module");

					if (plugin)
						enginesModules().put(name, plugin->value);
					else
						conflicts.add(name);
				}
			}
		}
	}
	catch (const Exception& ex)
	{
		string s;
		s.printf("Error in plugin config file '%s':", dir.getFilePath());
		iscLogException(s.c_str(), ex);
	}

}


void ExtEngineManager::closeAttachment(thread_db* tdbb, Attachment* /*attachment*/)
{
	Array<ExternalEngine*> enginesCopy;

	{	// scope
		ReadLockGuard readGuard(enginesLock);

		EnginesMap::Accessor accessor(&engines);
		for (bool found = accessor.getFirst(); found; found = accessor.getNext())
			enginesCopy.add(accessor.current()->second);
	}

	Database::Checkout dcoHolder(tdbb->getDatabase());

	for (Array<ExternalEngine*>::iterator i = enginesCopy.begin(); i != enginesCopy.end(); ++i)
	{
		ExternalEngine* engine = *i;
		EngineAttachmentInfo* attInfo = getEngineAttachment(tdbb, engine, true);

		if (attInfo)
		{
			ContextManager<ExternalFunction> ctxManager(tdbb, attInfo, attInfo->adminCharSet);
			engine->closeAttachment(LogError(), attInfo->context);
			delete attInfo;
		}
	}
}


ExtEngineManager::Function* ExtEngineManager::makeFunction(thread_db* tdbb, const Jrd::Function* udf,
	const MetaName& engine, const string& entryPoint, const string& body)
{
	string entryPointTrimmed = entryPoint;
	entryPointTrimmed.trim();

	EngineAttachmentInfo* attInfo = getEngineAttachment(tdbb, engine);
	ContextManager<ExternalFunction> ctxManager(tdbb, attInfo, attInfo->adminCharSet,
		(udf->getName().package.isEmpty() ?
			CallerName(obj_udf, udf->getName().identifier) :
			CallerName(obj_package_header, udf->getName().package)));

	ExternalFunction* externalFunction;

	{	// scope
		Database::Checkout dcoHolder(tdbb->getDatabase());

		externalFunction = attInfo->engine->makeFunction(RaiseError(),
			attInfo->context, udf->getName().package.nullStr(), udf->getName().identifier.c_str(),
			entryPointTrimmed.nullStr(), body.nullStr());

		if (!externalFunction)
		{
			status_exception::raise(
				Arg::Gds(isc_eem_func_not_returned) <<
					udf->getName().toString() << engine);
		}
	}

	try
	{
		return FB_NEW(getPool()) Function(tdbb, this, attInfo->engine, externalFunction, udf);
	}
	catch (...)
	{
		Database::Checkout dcoHolder(tdbb->getDatabase());
		externalFunction->dispose(LogError());
		throw;
	}
}


ExtEngineManager::Procedure* ExtEngineManager::makeProcedure(thread_db* tdbb, const jrd_prc* prc,
	const MetaName& engine, const string& entryPoint, const string& body)
{
	string entryPointTrimmed = entryPoint;
	entryPointTrimmed.trim();

	EngineAttachmentInfo* attInfo = getEngineAttachment(tdbb, engine);
	ContextManager<ExternalProcedure> ctxManager(tdbb, attInfo, attInfo->adminCharSet,
		(prc->getName().package.isEmpty() ?
			CallerName(obj_procedure, prc->getName().identifier) :
			CallerName(obj_package_header, prc->getName().package)));

	ExternalProcedure* externalProcedure;

	{	// scope
		Database::Checkout dcoHolder(tdbb->getDatabase());

		externalProcedure = attInfo->engine->makeProcedure(RaiseError(),
			attInfo->context, prc->getName().package.nullStr(), prc->getName().identifier.c_str(),
			entryPointTrimmed.nullStr(), body.nullStr());

		if (!externalProcedure)
		{
			status_exception::raise(
				Arg::Gds(isc_eem_proc_not_returned) <<
					prc->getName().toString() << engine);
		}
	}

	try
	{
		return FB_NEW(getPool()) Procedure(tdbb, this, attInfo->engine, externalProcedure, prc);
	}
	catch (...)
	{
		Database::Checkout dcoHolder(tdbb->getDatabase());
		externalProcedure->dispose(LogError());
		throw;
	}
}


ExtEngineManager::Trigger* ExtEngineManager::makeTrigger(thread_db* tdbb, const Jrd::Trigger* trg,
	const MetaName& engine, const string& entryPoint, const string& body, ExternalTrigger::Type type)
{
	string entryPointTrimmed = entryPoint;
	entryPointTrimmed.trim();

	MetaName relationNameTrimmed;
	if (trg->relation)
		relationNameTrimmed = trg->relation->rel_name;

	EngineAttachmentInfo* attInfo = getEngineAttachment(tdbb, engine);
	ContextManager<ExternalTrigger> ctxManager(tdbb, attInfo, attInfo->adminCharSet,
		CallerName(obj_trigger, trg->name));

	ExternalTrigger* externalTrigger;

	{	// scope
		Database::Checkout dcoHolder(tdbb->getDatabase());

		externalTrigger = attInfo->engine->makeTrigger(RaiseError(), attInfo->context,
			trg->name.c_str(), entryPointTrimmed.nullStr(), body.nullStr(),
			relationNameTrimmed.c_str(), type);

		if (!externalTrigger)
		{
			status_exception::raise(
				Arg::Gds(isc_eem_trig_not_returned) << trg->name << engine);
		}
	}

	try
	{
		return FB_NEW(getPool()) Trigger(tdbb, this, attInfo->engine, externalTrigger, trg);
	}
	catch (...)
	{
		Database::Checkout dcoHolder(tdbb->getDatabase());
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
			ConfigFile::String pluginName;
			if (enginesModules().get(name, pluginName))
			{
				PluginImpl* plugin = PluginManager::getPlugin(pluginName);

				EngineAttachment key(NULL, NULL);
				AutoPtr<EngineAttachmentInfo> attInfo;

				try
				{
					Database::Checkout dcoHolder(tdbb->getDatabase());

					engine = plugin->getExternalEngineFactory()->createEngine(RaiseError(),
						EXTERNAL_VERSION_1, name.c_str());

					if (engine)
					{
						int version = engine->getVersion(RaiseError());
						if (version != EXTERNAL_VERSION_1)
						{
							status_exception::raise(
								Arg::Gds(isc_eem_bad_plugin_ver) <<
									Arg::Num(version) << name);
						}

						Database::SyncGuard dsGuard(tdbb->getDatabase());

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
						engine->dispose(LogError());

					throw;
				}

				if (engine)
				{
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
			Database::Checkout dcoHolder(tdbb->getDatabase());
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
