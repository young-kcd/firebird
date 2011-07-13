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
#include "../jrd/ibase.h"
#include "firebird/UdrEngine.h"
#include "firebird/UdrCppEngine.h"
#include "firebird/Plugin.h"
#include "firebird/ExternalEngine.h"
#include "../common/classes/alloc.h"
#include "../common/classes/array.h"
#include "../common/classes/init.h"
#include "../common/classes/fb_string.h"
#include "../common/classes/GenericMap.h"
#include "../common/classes/objects_array.h"
#include "../common/os/mod_loader.h"
#include "../common/os/path_utils.h"
#include "../common/classes/ImplementHelper.h"
#include "../common/StatusHolder.h"


namespace Firebird
{
	namespace Udr
	{
//------------------------------------------------------------------------------


struct Node
{
	Node()
		: name(*getDefaultMemoryPool()),
		  module(*getDefaultMemoryPool())
	{
	}

	string name;
	PathName module;
};

struct FunctionNode : public Node
{
	FunctionFactory* factory;
	FunctionNode* next;
};

struct ProcedureNode : public Node
{
	ProcedureFactory* factory;
	ProcedureNode* next;
};

struct TriggerNode : public Node
{
	TriggerFactory* factory;
	TriggerNode* next;
};


static GlobalPtr<ObjectsArray<PathName> > paths;

class Engine : public StdPlugin<ExternalEngine, FB_EXTERNAL_ENGINE_VERSION>
{
public:
	explicit Engine(IPluginConfig* par)
		: functions(getPool()),
		  procedures(getPool()),
		  triggers(getPool())
	{
		IConfig* defaultConfig = par->getDefaultConfig();

		if (defaultConfig)
		{
			// this plugin is not ready to support different configurations
			// therefore keep legacy approach

			IConfigEntry* icp = NULL;

			for (int n = 0; (icp = defaultConfig->findPos("path", n)); ++n)
			{
				PathName newPath(icp->getValue());
				icp->release();

				bool found = false;

				for (ObjectsArray<PathName>::iterator i = paths->begin(); i != paths->end(); ++i)
				{
					if (*i == newPath)
					{
						found = true;
						break;
					}
				}

				if (!found)
				{
					paths->add(newPath);
				}
			}

			defaultConfig->release();
		}
	}

	int FB_CARG release()
	{
		if (--refCounter == 0)
		{
			delete this;
			return 0;
		}
		return 1;
	}

public:
	void loadModule(const IRoutineMetadata* metadata, PathName* moduleName, string* entryPoint);
	template <typename NodeType, typename ObjType, typename SharedObjType> ObjType* getChild(
		GenericMap<Pair<NonPooled<ExternalContext*, ObjType*> > >& children, SharedObjType* sharedObj,
		ExternalContext* context, NodeType* nodes, SortedArray<SharedObjType*>& sharedObjs,
		const PathName& moduleName);
	template <typename ObjType> void deleteChildren(
		GenericMap<Pair<NonPooled<ExternalContext*, ObjType*> > >& children);

	template <typename T> T* findNode(T* nodes, const PathName& moduleName,
		const string& entryPoint);

private:
	template <typename T, typename T2> T2* getNode(T* nodes, const PathName& moduleName,
		const IRoutineMetadata* metadata, const string& entryPoint);

public:
	virtual void FB_CALL open(Error* error, ExternalContext* context, Utf8* name, uint nameSize);
	virtual void FB_CALL openAttachment(Error* error, ExternalContext* context);
	virtual void FB_CALL closeAttachment(Error* error, ExternalContext* context);
	virtual ExternalFunction* FB_CALL makeFunction(Error* error, ExternalContext* context,
		const IRoutineMetadata* metadata, BlrMessage* inBlr, BlrMessage* outBlr);
	virtual ExternalProcedure* FB_CALL makeProcedure(Error* error, ExternalContext* context,
		const IRoutineMetadata* metadata, BlrMessage* inBlr, BlrMessage* outBlr);
	virtual ExternalTrigger* FB_CALL makeTrigger(Error* error, ExternalContext* context,
		const IRoutineMetadata* metadata);

public:
	virtual void FB_CALL dispose(Error* error);

private:
	Mutex childrenMutex;

public:
	SortedArray<class SharedFunction*> functions;
	SortedArray<class SharedProcedure*> procedures;
	SortedArray<class SharedTrigger*> triggers;
};


class ModulesMap : public GenericMap<Pair<Left<PathName, ModuleLoader::Module*> > >
{
public:
	explicit ModulesMap(MemoryPool& p)
		: GenericMap<Pair<Left<PathName, ModuleLoader::Module*> > >(p)
	{
	}

	~ModulesMap();
};


//--------------------------------------


static ModuleLoader::Module* libraryModule = NULL;

static GlobalPtr<PathName> libraryName;

static GlobalPtr<Mutex> modulesMutex;
static GlobalPtr<ModulesMap> modules;

static InitInstance<PathName> loadingModule;
static FunctionNode* registeredFunctions = NULL;
static ProcedureNode* registeredProcedures = NULL;
static TriggerNode* registeredTriggers = NULL;


//--------------------------------------


class SharedFunction : public ExternalFunction
{
public:
	SharedFunction(Engine* aEngine, const IRoutineMetadata* aMetadata,
				BlrMessage* inBlr, BlrMessage* outBlr)
		: engine(aEngine),
		  metadata(aMetadata),
		  moduleName(*getDefaultMemoryPool()),
		  entryPoint(*getDefaultMemoryPool()),
		  info(*getDefaultMemoryPool()),
		  children(*getDefaultMemoryPool())
	{
		engine->loadModule(metadata, &moduleName, &entryPoint);
		FunctionNode* node = engine->findNode<FunctionNode>(registeredFunctions, moduleName, entryPoint);
		node->factory->setup(metadata, inBlr, outBlr);
	}

	virtual ~SharedFunction()
	{
		engine->deleteChildren(children);
	}

public:
	virtual void FB_CALL dispose(Firebird::Error* /*error*/)
	{
		delete this;
	}

public:
	virtual void FB_CALL getCharSet(Error* error, ExternalContext* context,
		Utf8* name, uint nameSize)
	{
		strncpy(name, context->getClientCharSet(), nameSize);

		try
		{
			ExternalFunction* function = engine->getChild<FunctionNode, ExternalFunction>(
				children, this, context, registeredFunctions, engine->functions, moduleName);
			if (function)
				function->getCharSet(error, context, name, nameSize);
		}
		catch (const ThrowError::Exception& e)
		{
			e.stuff(error);
		}
	}

	virtual void FB_CALL execute(Error* error, ExternalContext* context, void* inMsg, void* outMsg)
	{
		ExternalFunction* function = engine->getChild<FunctionNode, ExternalFunction>(
			children, this, context, registeredFunctions, engine->functions, moduleName);
		if (function)
			function->execute(error, context, inMsg, outMsg);
	}

public:
	Engine* engine;
	const IRoutineMetadata* metadata;
	PathName moduleName;
	string entryPoint;
	string info;
	GenericMap<Pair<NonPooled<ExternalContext*, ExternalFunction*> > > children;
};


//--------------------------------------


class SharedProcedure : public ExternalProcedure
{
public:
	SharedProcedure(Engine* aEngine, const IRoutineMetadata* aMetadata,
				BlrMessage* inBlr, BlrMessage* outBlr)
		: engine(aEngine),
		  metadata(aMetadata),
		  moduleName(*getDefaultMemoryPool()),
		  entryPoint(*getDefaultMemoryPool()),
		  info(*getDefaultMemoryPool()),
		  children(*getDefaultMemoryPool())
	{
		engine->loadModule(metadata, &moduleName, &entryPoint);
		ProcedureNode* node = engine->findNode<ProcedureNode>(registeredProcedures, moduleName, entryPoint);
		node->factory->setup(metadata, inBlr, outBlr);
	}

	virtual ~SharedProcedure()
	{
		engine->deleteChildren(children);
	}

public:
	virtual void FB_CALL dispose(Firebird::Error* /*error*/)
	{
		delete this;
	}

public:
	virtual void FB_CALL getCharSet(Error* error, ExternalContext* context,
		Utf8* name, uint nameSize)
	{
		strncpy(name, context->getClientCharSet(), nameSize);

		try
		{
			ExternalProcedure* procedure = engine->getChild<ProcedureNode, ExternalProcedure>(
				children, this, context, registeredProcedures, engine->procedures, moduleName);
			if (procedure)
				procedure->getCharSet(error, context, name, nameSize);
		}
		catch (const ThrowError::Exception& e)
		{
			e.stuff(error);
		}
	}

	virtual ExternalResultSet* FB_CALL open(Error* error, ExternalContext* context,
		void* inMsg, void* outMsg)
	{
		try
		{
			ExternalProcedure* procedure = engine->getChild<ProcedureNode, ExternalProcedure>(
				children, this, context, registeredProcedures, engine->procedures, moduleName);
			return procedure ? procedure->open(error, context, inMsg, outMsg) : NULL;
		}
		catch (const ThrowError::Exception& e)
		{
			e.stuff(error);
			return NULL;
		}
	}

public:
	Engine* engine;
	const IRoutineMetadata* metadata;
	PathName moduleName;
	string entryPoint;
	string info;
	GenericMap<Pair<NonPooled<ExternalContext*, ExternalProcedure*> > > children;
};


//--------------------------------------


class SharedTrigger : public ExternalTrigger
{
public:
	SharedTrigger(Engine* aEngine, const IRoutineMetadata* aMetadata)
		: engine(aEngine),
		  metadata(aMetadata),
		  moduleName(*getDefaultMemoryPool()),
		  entryPoint(*getDefaultMemoryPool()),
		  info(*getDefaultMemoryPool()),
		  children(*getDefaultMemoryPool())
	{
		engine->loadModule(metadata, &moduleName, &entryPoint);
		TriggerNode* node = engine->findNode<TriggerNode>(registeredTriggers, moduleName, entryPoint);
		node->factory->setup(metadata);
	}

	virtual ~SharedTrigger()
	{
		engine->deleteChildren(children);
	}

public:
	virtual void FB_CALL dispose(Firebird::Error* /*error*/)
	{
		delete this;
	}

public:
	virtual void FB_CALL getCharSet(Error* error, ExternalContext* context,
		Utf8* name, uint nameSize)
	{
		strncpy(name, context->getClientCharSet(), nameSize);

		try
		{
			ExternalTrigger* trigger = engine->getChild<TriggerNode, ExternalTrigger>(
				children, this, context, registeredTriggers, engine->triggers, moduleName);
			if (trigger)
				trigger->getCharSet(error, context, name, nameSize);
		}
		catch (const ThrowError::Exception& e)
		{
			e.stuff(error);
		}
	}

	virtual void FB_CALL execute(Error* error, ExternalContext* context,
		ExternalTrigger::Action action, void* oldMsg, void* newMsg)
	{
		ExternalTrigger* trigger = engine->getChild<TriggerNode, ExternalTrigger>(
			children, this, context, registeredTriggers, engine->triggers, moduleName);
		if (trigger)
			trigger->execute(error, context, action, oldMsg, newMsg);
	}

public:
	Engine* engine;
	const IRoutineMetadata* metadata;
	PathName moduleName;
	string entryPoint;
	string info;
	GenericMap<Pair<NonPooled<ExternalContext*, ExternalTrigger*> > > children;
};


//--------------------------------------


extern "C" void fbUdrRegFunction(const char* name, FunctionFactory* factory)
{
	FunctionNode* node = new FunctionNode();
	node->name = name;
	node->module = loadingModule();
	node->factory = factory;
	node->next = registeredFunctions;
	registeredFunctions = node;
}


extern "C" void fbUdrRegProcedure(const char* name, ProcedureFactory* factory)
{
	ProcedureNode* node = new ProcedureNode();
	node->name = name;
	node->module = loadingModule();
	node->factory = factory;
	node->next = registeredProcedures;
	registeredProcedures = node;
}


extern "C" void fbUdrRegTrigger(const char* name, TriggerFactory* factory)
{
	TriggerNode* node = new TriggerNode();
	node->name = name;
	node->module = loadingModule();
	node->factory = factory;
	node->next = registeredTriggers;
	registeredTriggers = node;
}


ModulesMap::~ModulesMap()
{
	FunctionNode* func = registeredFunctions;

	while (func)
	{
		FunctionNode* del = func;
		func = func->next;
		delete del;
	}

	ProcedureNode* proc = registeredProcedures;

	while (proc)
	{
		ProcedureNode* del = proc;
		proc = proc->next;
		delete del;
	}

	TriggerNode* trig = registeredTriggers;

	while (trig)
	{
		TriggerNode* del = trig;
		trig = trig->next;
		delete del;
	}

	registeredFunctions = NULL;
	registeredProcedures = NULL;
	registeredTriggers = NULL;

	Accessor accessor(this);
	for (bool cont = accessor.getFirst(); cont; cont = accessor.getNext())
		delete accessor.current()->second;

	delete libraryModule;
}


//--------------------------------------


void Engine::loadModule(const IRoutineMetadata* metadata, PathName* moduleName, string* entryPoint)
{
	LocalStatus status;
	const string str(metadata->getEntryPoint(&status));
	ThrowError::check(status.get());

	const size_t pos = str.find('!');
	if (pos == string::npos)
	{
		static const ISC_STATUS statusVector[] = {
			isc_arg_gds,
			isc_random,
			isc_arg_string, (ISC_STATUS) "Invalid entry point",
			isc_arg_end
		};

		ThrowError::check(statusVector);
	}

	*moduleName = PathName(str.substr(0, pos).c_str());
	// Do not allow module names with directory separators as a security measure.
	if (moduleName->find_first_of("/\\") != string::npos)
	{
		static const ISC_STATUS statusVector[] = {
			isc_arg_gds,
			isc_random,
			isc_arg_string, (ISC_STATUS) "Invalid module name",
			isc_arg_end
		};

		ThrowError::check(statusVector);
	}

	*entryPoint = str.substr(pos + 1);

	size_t n = entryPoint->find('!');
	*entryPoint = (n == string::npos ? *entryPoint : entryPoint->substr(0, n));

	MutexLockGuard guard(modulesMutex);

	if (modules->exist(*moduleName))
		return;

	loadingModule() = *moduleName;

	for (ObjectsArray<PathName>::iterator i = paths->begin(); i != paths->end(); ++i)
	{
		PathName path;
		PathUtils::concatPath(path, *i, *moduleName);

		ModuleLoader::Module* module = ModuleLoader::fixAndLoadModule(path);

		if (module)
		{
			modules->put(*moduleName, module);
			break;
		}
		else
		{
			static const ISC_STATUS statusVector[] = {
				isc_arg_gds,
				isc_random,
				isc_arg_string, (ISC_STATUS) "Module not found",
				isc_arg_end
			};

			ThrowError::check(statusVector);
		}
	}
}


template <typename NodeType, typename ObjType, typename SharedObjType> ObjType* Engine::getChild(
	GenericMap<Pair<NonPooled<ExternalContext*, ObjType*> > >& children, SharedObjType* sharedObj,
	ExternalContext* context, NodeType* nodes, SortedArray<SharedObjType*>& sharedObjs,
	const PathName& moduleName)
{
	MutexLockGuard guard(childrenMutex);

	if (!sharedObjs.exist(sharedObj))
		sharedObjs.add(sharedObj);

	ObjType* obj;
	if (!children.get(context, obj))
	{
		obj = getNode<NodeType, ObjType>(nodes, moduleName, sharedObj->metadata, sharedObj->entryPoint);
		if (obj)
			children.put(context, obj);
	}

	return obj;
}


template <typename ObjType> void Engine::deleteChildren(
	GenericMap<Pair<NonPooled<ExternalContext*, ObjType*> > >& children)
{
	// No need to lock childrenMutex as if there are more threads simultaneously accessing
	// these children in this moment there will be a memory corruption anyway.

	typedef typename GenericMap<Pair<NonPooled<ExternalContext*, ObjType*> > >::Accessor ChildrenAccessor;
	ChildrenAccessor accessor(&children);
	for (bool found = accessor.getFirst(); found; found = accessor.getNext())
		delete accessor.current()->second;
}


template <typename T> T* Engine::findNode(T* nodes, const PathName& moduleName,
		const string& entryPoint)
{
	for (T* node = nodes; node; node = node->next)
	{
		if (node->module == moduleName && entryPoint == node->name)
			return node;
	}

	static const ISC_STATUS statusVector[] = {
		isc_arg_gds,
		isc_random,
		isc_arg_string, (ISC_STATUS) "Entry point not found",
		isc_arg_end
	};

	ThrowError::check(statusVector);

	return NULL;
}


template <typename T, typename T2> T2* Engine::getNode(T* nodes, const PathName& moduleName,
		const IRoutineMetadata* metadata, const string& entryPoint)
{
	T* node = findNode<T>(nodes, moduleName, entryPoint);
	return node->factory->newItem(metadata);
}


void FB_CALL Engine::open(Error* /*error*/, ExternalContext* /*context*/, Utf8* name, uint nameSize)
{
	strncpy(name, "UTF-8", nameSize);
}


void FB_CALL Engine::openAttachment(Error* /*error*/, ExternalContext* /*context*/)
{
}


void FB_CALL Engine::closeAttachment(Error* error, ExternalContext* context)
{
	MutexLockGuard guard(childrenMutex);

	for (SortedArray<SharedFunction*>::iterator i = functions.begin(); i != functions.end(); ++i)
	{
		ExternalFunction* function;
		if ((*i)->children.get(context, function))
		{
			function->dispose(error);
			(*i)->children.remove(context);
		}
	}

	for (SortedArray<SharedProcedure*>::iterator i = procedures.begin(); i != procedures.end(); ++i)
	{
		ExternalProcedure* procedure;
		if ((*i)->children.get(context, procedure))
		{
			procedure->dispose(error);
			(*i)->children.remove(context);
		}
	}

	for (SortedArray<SharedTrigger*>::iterator i = triggers.begin(); i != triggers.end(); ++i)
	{
		ExternalTrigger* trigger;
		if ((*i)->children.get(context, trigger))
		{
			trigger->dispose(error);
			(*i)->children.remove(context);
		}
	}
}


ExternalFunction* FB_CALL Engine::makeFunction(Error* error, ExternalContext* /*context*/,
	const IRoutineMetadata* metadata, BlrMessage* inBlr, BlrMessage* outBlr)
{
	try
	{
		return new SharedFunction(this, metadata, inBlr, outBlr);
	}
	catch (const ThrowError::Exception& e)
	{
		e.stuff(error);
		return NULL;
	}
}


ExternalProcedure* FB_CALL Engine::makeProcedure(Error* error, ExternalContext* /*context*/,
	const IRoutineMetadata* metadata, BlrMessage* inBlr, BlrMessage* outBlr)
{
	try
	{
		return new SharedProcedure(this, metadata, inBlr, outBlr);
	}
	catch (const ThrowError::Exception& e)
	{
		e.stuff(error);
		return NULL;
	}
}


ExternalTrigger* FB_CALL Engine::makeTrigger(Error* error, ExternalContext* /*context*/,
	const IRoutineMetadata* metadata)
{
	try
	{
		return new SharedTrigger(this, metadata);
	}
	catch (const ThrowError::Exception& e)
	{
		e.stuff(error);
		return NULL;
	}
}


void FB_CALL Engine::dispose(Error* /*error*/)
{
	delete this;
}


//--------------------------------------


/***
class ExternalEngineFactoryImpl : public ExternalEngineFactory
{
public:
	virtual ExternalEngine* FB_CALL createEngine(Error* error, int *version*,
		const char* name)
	{
		if (strcmp(name, "UDR") == 0)
			return new Engine();

		const char* const msg = "Engine not implemented";

		error->addCode(isc_arg_gds);
		error->addCode(isc_random);
		error->addString(msg, strlen(msg));
		return NULL;
	}
***/

class ExternalEngineFactoryImpl : public SimpleFactory<Engine>
{
} factory;

extern "C" void FB_PLUGIN_ENTRY_POINT(Firebird::IMaster* master)
{
	PluginManagerInterfacePtr pi;
	pi->registerPluginFactory(PluginType::ExternalEngine, "UDR", &factory);
	myModule->registerMe();

	libraryName->assign("fbclient");
	ModuleLoader::doctorModuleExtension(libraryName);

	libraryModule = ModuleLoader::loadModule(libraryName);
}


//------------------------------------------------------------------------------
	}	// namespace Udr
}	// namespace Firebird
