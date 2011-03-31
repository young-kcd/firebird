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
#include "FirebirdUdr.h"
#include "FirebirdUdrCpp.h"
#include "FirebirdPluginApi.h"
#include "FirebirdApi.h"
#include "FirebirdExternalApi.h"
#include "../common/classes/alloc.h"
#include "../common/classes/array.h"
#include "../common/classes/init.h"
#include "../common/classes/fb_string.h"
#include "../common/classes/GenericMap.h"
#include "../common/classes/objects_array.h"
#include "../common/os/mod_loader.h"
#include "../common/os/path_utils.h"
#include "../common/classes/ImplementHelper.h"


namespace Firebird
{
	namespace Udr
	{
//------------------------------------------------------------------------------


struct Node
{
	Node()
		: module(*getDefaultMemoryPool())
	{
	}

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
	explicit Engine(IFactoryParameter* par)
		: functions(getPool()),
		  procedures(getPool()),
		  triggers(getPool())
	{
		IConfig* defaultConfig = par->getDefaultConfig();

		if (defaultConfig)
		{
			// this plugin is not ready to support different configurations
			// therefore keep legacy approach

			IConfigParameter* icp = NULL;

			for (int n = 0; (icp = defaultConfig->findPos("path", n)); ++n)
			{
				PathName newPath(icp->value());
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
	void loadModule(const string& str, PathName* moduleName, string* entryPoint, string* info);
	template <typename NodeType, typename ObjType, typename SharedObjType> ObjType* getChild(
		GenericMap<Pair<NonPooled<ExternalContext*, ObjType*> > >& children, SharedObjType* sharedObj,
		ExternalContext* context, NodeType* nodes, SortedArray<SharedObjType*>& sharedObjs,
		const PathName& moduleName);
	template <typename ObjType> void deleteChildren(
		GenericMap<Pair<NonPooled<ExternalContext*, ObjType*> > >& children);

	template <typename T, typename T2> T* findNode(T* nodes, const PathName& moduleName,
		T2* metaInfo);

private:
	template <typename T, typename T2, typename T3> T2* getNode(T* nodes, const PathName& moduleName,
		T3* metaInfo);

public:
	virtual int FB_CALL getVersion(Error* error);
	virtual void FB_CALL open(Error* error, ExternalContext* context, Utf8* name, uint nameSize);
	virtual void FB_CALL openAttachment(Error* error, ExternalContext* context);
	virtual void FB_CALL closeAttachment(Error* error, ExternalContext* context);
	virtual ExternalFunction* FB_CALL makeFunction(Error* error, ExternalContext* context,
		const char* package, const char* name, const char* entryPoint, const char* body);
	virtual ExternalProcedure* FB_CALL makeProcedure(Error* error, ExternalContext* context,
		const char* package, const char* name, const char* entryPoint, const char* body);
	virtual ExternalTrigger* FB_CALL makeTrigger(Error* error, ExternalContext* context,
		const char* name, const char* entryPoint, const char* body, const char* table,
		ExternalTrigger::Type type);

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
	SharedFunction(Engine* aEngine, const string& aPackage, const string& aName,
				const string& aEntryPoint, const string& aBody)
		: engine(aEngine),
		  package(*getDefaultMemoryPool(), aPackage),
		  name(*getDefaultMemoryPool(), aName),
		  entryPoint(*getDefaultMemoryPool()),
		  body(*getDefaultMemoryPool(), aBody),
		  moduleName(*getDefaultMemoryPool()),
		  info(*getDefaultMemoryPool()),
		  children(*getDefaultMemoryPool())
	{
		engine->loadModule(aEntryPoint, &moduleName, &entryPoint, &info);
		metaInfo.package = package.nullStr();
		metaInfo.name = name.c_str();
		metaInfo.entryPoint = entryPoint.c_str();
		metaInfo.body = body.c_str();
		metaInfo.info = info.c_str();

		engine->findNode<FunctionNode, MetaInfo>(registeredFunctions, moduleName, &metaInfo);
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

	virtual void FB_CALL execute(Error* error, ExternalContext* context, Values* params,
		Value* result)
	{
		ExternalFunction* function = engine->getChild<FunctionNode, ExternalFunction>(
			children, this, context, registeredFunctions, engine->functions, moduleName);
		if (function)
			function->execute(error, context, params, result);
	}

public:
	MetaInfo metaInfo;
	Engine* engine;
	string package;
	string name;
	string entryPoint;
	string body;
	PathName moduleName;
	string info;
	GenericMap<Pair<NonPooled<ExternalContext*, ExternalFunction*> > > children;
};


//--------------------------------------


class SharedProcedure : public ExternalProcedure
{
public:
	SharedProcedure(Engine* aEngine, const string& aPackage, const string& aName,
				const string& aEntryPoint, const string& aBody)
		: engine(aEngine),
		  package(*getDefaultMemoryPool(), aPackage),
		  name(*getDefaultMemoryPool(), aName),
		  entryPoint(*getDefaultMemoryPool(), aEntryPoint),
		  body(*getDefaultMemoryPool(), aBody),
		  moduleName(*getDefaultMemoryPool()),
		  info(*getDefaultMemoryPool()),
		  children(*getDefaultMemoryPool())
	{
		engine->loadModule(aEntryPoint, &moduleName, &entryPoint, &info);
		metaInfo.package = package.nullStr();
		metaInfo.name = name.c_str();
		metaInfo.entryPoint = entryPoint.c_str();
		metaInfo.body = body.c_str();
		metaInfo.info = info.c_str();

		engine->findNode<ProcedureNode, MetaInfo>(registeredProcedures, moduleName, &metaInfo);
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
		Values* params, Values* results)
	{
		try
		{
			ExternalProcedure* procedure = engine->getChild<ProcedureNode, ExternalProcedure>(
				children, this, context, registeredProcedures, engine->procedures, moduleName);
			return procedure ? procedure->open(error, context, params, results) : NULL;
		}
		catch (const ThrowError::Exception& e)
		{
			e.stuff(error);
			return NULL;
		}
	}

public:
	MetaInfo metaInfo;
	Engine* engine;
	string package;
	string name;
	string entryPoint;
	string body;
	PathName moduleName;
	string info;
	GenericMap<Pair<NonPooled<ExternalContext*, ExternalProcedure*> > > children;
};


//--------------------------------------


class SharedTrigger : public ExternalTrigger
{
public:
	SharedTrigger(Engine* aEngine, const string& aName, const string& aEntryPoint,
				const string& aBody, const string& aTable, ExternalTrigger::Type aType)
		: engine(aEngine),
		  name(*getDefaultMemoryPool(), aName),
		  entryPoint(*getDefaultMemoryPool(), aEntryPoint),
		  body(*getDefaultMemoryPool(), aBody),
		  moduleName(*getDefaultMemoryPool()),
		  info(*getDefaultMemoryPool()),
		  table(*getDefaultMemoryPool(), aTable),
		  type(aType),
		  children(*getDefaultMemoryPool())
	{
		engine->loadModule(aEntryPoint, &moduleName, &entryPoint, &info);
		metaInfo.package = NULL;
		metaInfo.name = name.c_str();
		metaInfo.entryPoint = entryPoint.c_str();
		metaInfo.body = body.c_str();
		metaInfo.info = info.c_str();
		metaInfo.type = type;
		metaInfo.table = table.nullStr();

		engine->findNode<TriggerNode, TriggerMetaInfo>(registeredTriggers, moduleName, &metaInfo);
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
		ExternalTrigger::Action action, const Values* oldValues, Values* newValues)
	{
		ExternalTrigger* trigger = engine->getChild<TriggerNode, ExternalTrigger>(
			children, this, context, registeredTriggers, engine->triggers, moduleName);
		if (trigger)
			trigger->execute(error, context, action, oldValues, newValues);
	}

public:
	TriggerMetaInfo metaInfo;
	Engine* engine;
	string name;
	string entryPoint;
	string body;
	PathName moduleName;
	string info;
	string table;
	ExternalTrigger::Type type;
	GenericMap<Pair<NonPooled<ExternalContext*, ExternalTrigger*> > > children;
};


//--------------------------------------


extern "C" void fbUdrRegFunction(FunctionFactory* factory)
{
	FunctionNode* node = new FunctionNode();
	node->module = loadingModule();
	node->factory = factory;
	node->next = registeredFunctions;
	registeredFunctions = node;
}


extern "C" void fbUdrRegProcedure(ProcedureFactory* factory)
{
	ProcedureNode* node = new ProcedureNode();
	node->module = loadingModule();
	node->factory = factory;
	node->next = registeredProcedures;
	registeredProcedures = node;
}


extern "C" void fbUdrRegTrigger(TriggerFactory* factory)
{
	TriggerNode* node = new TriggerNode();
	node->module = loadingModule();
	node->factory = factory;
	node->next = registeredTriggers;
	registeredTriggers = node;
}


extern "C" void* fbUdrGetFunction(const char* symbol)
{
	return libraryModule->findSymbol(symbol);
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


void Engine::loadModule(const string& str, PathName* moduleName, string* entryPoint, string* info)
{
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
	*info = (n == string::npos ? "" : entryPoint->substr(n + 1));
	*entryPoint = (n == string::npos ? *entryPoint : entryPoint->substr(0, n));

	MutexLockGuard guard(modulesMutex);

	if (modules->exist(*moduleName))
		return;

	loadingModule() = *moduleName;

	for (ObjectsArray<PathName>::iterator i = paths->begin(); i != paths->end(); ++i)
	{
		PathName path;
		PathUtils::concatPath(path, *i, *moduleName);

		ModuleLoader::Module* module = ModuleLoader::loadModule(path);
		if (!module)
		{
			ModuleLoader::doctorModuleExtension(path);
			module = ModuleLoader::loadModule(path);
		}

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
		obj = getNode<NodeType, ObjType>(nodes, moduleName, &sharedObj->metaInfo);
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


template <typename T, typename T2> T* Engine::findNode(T* nodes,
		const PathName& moduleName, T2* params)
{
	const string entryPoint(params->entryPoint);

	for (T* node = nodes; node; node = node->next)
	{
		if (node->module == moduleName && entryPoint == node->factory->getName())
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


template <typename T, typename T2, typename T3> T2* Engine::getNode(T* nodes,
		const PathName& moduleName, T3* params)
{
	T* node = findNode<T, T3>(nodes, moduleName, params);
	return node->factory->newItem(params);
}


int FB_CALL Engine::getVersion(Error* /*error*/)
{
	return EXTERNAL_VERSION_1;
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
	const char* package, const char* name, const char* entryPoint, const char* body)
{
	try
	{
		return new SharedFunction(this, (package ? package : ""), name,
			(entryPoint ? entryPoint : ""), (body ? body : ""));
	}
	catch (const ThrowError::Exception& e)
	{
		e.stuff(error);
		return NULL;
	}
}


ExternalProcedure* FB_CALL Engine::makeProcedure(Error* error, ExternalContext* /*context*/,
	const char* package, const char* name, const char* entryPoint, const char* body)
{
	try
	{
		return new SharedProcedure(this, (package ? package : ""), name,
			(entryPoint ? entryPoint : ""), (body ? body : ""));
	}
	catch (const ThrowError::Exception& e)
	{
		e.stuff(error);
		return NULL;
	}
}


ExternalTrigger* FB_CALL Engine::makeTrigger(Error* error, ExternalContext* /*context*/,
	const char* name, const char* entryPoint, const char* body, const char* table,
	ExternalTrigger::Type type)
{
	try
	{
		return new SharedTrigger(this, name, (entryPoint ? entryPoint : ""), (body ? body : ""),
			table, type);
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
static Firebird::UnloadDetector unloadDetector;

extern "C" void FB_PLUGIN_ENTRY_POINT(Firebird::IMaster* master)
{
	PluginInterface pi;
	pi->registerPlugin(PluginType::ExternalEngine, "UDR", &factory);
	pi->setModuleCleanup(&unloadDetector);

	libraryName->assign("fbclient");
	ModuleLoader::doctorModuleExtension(libraryName);

	libraryModule = ModuleLoader::loadModule(libraryName);
}


//------------------------------------------------------------------------------
	}	// namespace Udr
}	// namespace Firebird
