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

#ifndef DSQL_DDL_NODES_H
#define DSQL_DDL_NODES_H

#include "../jrd/common.h"
#include "../dsql/node.h"
#include "../dsql/make_proto.h"
#include "../dsql/Nodes.h"
#include "../common/classes/array.h"
#include "../common/classes/TriState.h"

namespace Jrd {


class ExternalClause
{
public:
	explicit ExternalClause(MemoryPool& pool)
		: name(pool),
		  engine(pool)
	{
	}

public:
	Firebird::string name;
	Firebird::MetaName engine;
};


class TypeClause
{
public:
	explicit TypeClause(dsql_fld* aField, const Firebird::MetaName& aCollate);
	virtual ~TypeClause() {}

public:
	void resolve(DsqlCompilerScratch* dsqlScratch);

public:
	virtual void print(Firebird::string& text) const;

public:
	USHORT type;
	FLD_LENGTH length;
	SSHORT scale;
	SSHORT subType;
	USHORT segLength;
	USHORT precision;
	USHORT charLength;
	SSHORT charSetId;
	SSHORT collationId;
	SSHORT textType;
	bool collateSpecified;
	bool fullDomain;
	bool notNull;
	Firebird::MetaName fieldSource;
	Firebird::MetaName typeOfTable;
	Firebird::MetaName typeOfName;

public:
	dsql_fld* legacyField;
	Firebird::MetaName collate;
};


class ParameterClause : public TypeClause
{
public:
	explicit ParameterClause(dsql_fld* field, const Firebird::MetaName& aCollate, dsql_nod* dflt,
		dsql_nod* aLegacyParameter);

public:
	void print(Firebird::string& text) const;

public:
	Firebird::MetaName name;
	dsql_nod* legacyDefault;
	dsql_nod* legacyParameter;
};


class AlterCharSetNode : public DdlNode
{
public:
	explicit AlterCharSetNode(MemoryPool& pool, const Firebird::string& sqlText,
				const Firebird::MetaName& aCharSet, const Firebird::MetaName& aDefaultCollation)
		: DdlNode(pool, sqlText),
		  charSet(pool, aCharSet),
		  defaultCollation(pool, aDefaultCollation)
	{
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, jrd_tra* transaction);

private:
	Firebird::MetaName charSet;
	Firebird::MetaName defaultCollation;
};


class CommentOnNode : public DdlNode
{
public:
	explicit CommentOnNode(MemoryPool& pool, const Firebird::string& sqlText, int aObjType,
				const Firebird::MetaName& aObjName, const Firebird::MetaName& aSubName,
				const Firebird::string& aText, const char* aTextCharSet)
		: DdlNode(pool, sqlText),
		  objType(aObjType),
		  objName(pool, aObjName),
		  subName(pool, aSubName),
		  text(pool, aText),
		  textCharSet(aTextCharSet)
	{
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, jrd_tra* transaction);

private:
	int objType;
	Firebird::MetaName objName;
	Firebird::MetaName subName;
	Firebird::string text;
	const char* textCharSet;
};


class CreateAlterFunctionNode : public DdlNode, public BlockNode
{
public:
	explicit CreateAlterFunctionNode(MemoryPool& pool, const Firebird::string& sqlText,
				const Firebird::MetaName& aName)
		: DdlNode(pool, sqlText),
		  BlockNode(pool, false),
		  name(pool, aName),
		  create(true),
		  alter(false),
		  external(NULL),
		  invariant(false),
		  parameters(pool),
		  returnType(NULL, NULL),
		  localDeclList(NULL),
		  source(pool),
		  body(NULL),
		  compiled(false),
		  invalid(false),
		  package(pool),
		  packageOwner(pool),
		  privateScope(false)
	{
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, jrd_tra* transaction);

protected:
	virtual DdlNode* internalDsqlPass();

private:
	void executeCreate(thread_db* tdbb, jrd_tra* transaction);
	bool executeAlter(thread_db* tdbb, jrd_tra* transaction, bool secondPass, bool runTriggers);

	void storeArgument(thread_db* tdbb, jrd_tra* transaction,
		unsigned pos, const ParameterClause& parameter);
	void compile(thread_db* tdbb, jrd_tra* transaction);

public:
	Firebird::MetaName name;
	bool create;
	bool alter;
	ExternalClause* external;
	bool invariant;
	Firebird::Array<ParameterClause> parameters;
	TypeClause returnType;
	dsql_nod* localDeclList;
	Firebird::string source;
	dsql_nod* body;
	bool compiled;
	bool invalid;
	Firebird::MetaName package;
	Firebird::string packageOwner;
	bool privateScope;
};


class DropFunctionNode : public DdlNode
{
public:
	explicit DropFunctionNode(MemoryPool& pool, const Firebird::string& sqlText,
				const Firebird::MetaName& aName)
		: DdlNode(pool, sqlText),
		  name(pool, aName),
		  silent(false),
		  package(pool)
	{
	}

public:
	static void dropArguments(thread_db* tdbb, jrd_tra* transaction,
		const Firebird::MetaName& functionName, const Firebird::MetaName& packageName);

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, jrd_tra* transaction);

protected:
	virtual DdlNode* internalDsqlPass();

public:
	Firebird::MetaName name;
	bool silent;
	Firebird::MetaName package;
};


class RecreateFunctionNode : public DdlNode
{
public:
	explicit RecreateFunctionNode(MemoryPool& p, const Firebird::string& sqlText,
				CreateAlterFunctionNode* aCreateNode)
		: DdlNode(p, sqlText),
		  createNode(aCreateNode),
		  dropNode(p, sqlText, createNode->name)
	{
		dropNode.silent = true;
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, jrd_tra* transaction);

protected:
	virtual DdlNode* internalDsqlPass();

private:
	CreateAlterFunctionNode* createNode;
	DropFunctionNode dropNode;
};


class CreateAlterProcedureNode : public DdlNode, public BlockNode
{
public:
	explicit CreateAlterProcedureNode(MemoryPool& pool, const Firebird::string& sqlText,
				const Firebird::MetaName& aName)
		: DdlNode(pool, sqlText),
		  BlockNode(pool, true),
		  name(pool, aName),
		  create(true),
		  alter(false),
		  external(NULL),
		  parameters(pool),
		  returns(pool),
		  source(pool),
		  localDeclList(NULL),
		  body(NULL),
		  compiled(false),
		  invalid(false),
		  package(pool),
		  packageOwner(pool),
		  privateScope(false)
	{
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, jrd_tra* transaction);

protected:
	virtual DdlNode* internalDsqlPass();

private:
	void executeCreate(thread_db* tdbb, jrd_tra* transaction);
	bool executeAlter(thread_db* tdbb, jrd_tra* transaction, bool secondPass, bool runTriggers);
	void storeParameter(thread_db* tdbb, jrd_tra* transaction,
		USHORT type, unsigned pos, const ParameterClause& parameter);
	void compile(thread_db* tdbb, jrd_tra* transaction);

public:
	Firebird::MetaName name;
	bool create;
	bool alter;
	ExternalClause* external;
	Firebird::Array<ParameterClause> parameters;
	Firebird::Array<ParameterClause> returns;
	Firebird::string source;
	dsql_nod* localDeclList;
	dsql_nod* body;
	bool compiled;
	bool invalid;
	Firebird::MetaName package;
	Firebird::string packageOwner;
	bool privateScope;
};


class DropProcedureNode : public DdlNode
{
public:
	explicit DropProcedureNode(MemoryPool& pool, const Firebird::string& sqlText,
				const Firebird::MetaName& aName)
		: DdlNode(pool, sqlText),
		  name(pool, aName),
		  silent(false),
		  package(pool)
	{
	}

public:
	static void dropParameters(thread_db* tdbb, jrd_tra* transaction,
		const Firebird::MetaName& procedureName, const Firebird::MetaName& packageName);

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, jrd_tra* transaction);

protected:
	virtual DdlNode* internalDsqlPass();

public:
	Firebird::MetaName name;
	bool silent;
	Firebird::MetaName package;
};


class RecreateProcedureNode : public DdlNode
{
public:
	explicit RecreateProcedureNode(MemoryPool& p, const Firebird::string& sqlText,
				CreateAlterProcedureNode* aCreateNode)
		: DdlNode(p, sqlText),
		  createNode(aCreateNode),
		  dropNode(p, sqlText, createNode->name)
	{
		dropNode.silent = true;
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, jrd_tra* transaction);

protected:
	virtual DdlNode* internalDsqlPass();

private:
	CreateAlterProcedureNode* createNode;
	DropProcedureNode dropNode;
};


class CreateAlterTriggerNode : public DdlNode, public BlockNode
{
public:
	explicit CreateAlterTriggerNode(MemoryPool& p, const Firebird::string& sqlText,
				const Firebird::MetaName& aName)
		: DdlNode(p, sqlText),
		  BlockNode(p, false),
		  name(p, aName),
		  create(true),
		  alter(false),
		  relationName(p),
		  external(NULL),
		  source(p),
		  localDeclList(NULL),
		  body(NULL),
		  compiled(false),
		  invalid(false)
	{
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, jrd_tra* transaction);

protected:
	virtual DdlNode* internalDsqlPass();

private:
	void executeCreate(thread_db* tdbb, jrd_tra* transaction);
	bool executeAlter(thread_db* tdbb, jrd_tra* transaction, bool runTriggers);
	void compile(thread_db* tdbb, jrd_tra* transaction);

	static inline bool hasOldContext(const unsigned value)
	{
		const unsigned val1 = ((value + 1) >> 1) & 3;
		const unsigned val2 = ((value + 1) >> 3) & 3;
		const unsigned val3 = ((value + 1) >> 5) & 3;
		return (val1 && val1 != 1) || (val2 && val2 != 1) || (val3 && val3 != 1);
	}

	static inline bool hasNewContext(const unsigned value)
	{
		const unsigned val1 = ((value + 1) >> 1) & 3;
		const unsigned val2 = ((value + 1) >> 3) & 3;
		const unsigned val3 = ((value + 1) >> 5) & 3;
		return (val1 && val1 != 3) || (val2 && val2 != 3) || (val3 && val3 != 3);
	}

public:
	Firebird::MetaName name;
	bool create;
	bool alter;
	Firebird::MetaName relationName;
	TriStateType<FB_UINT64> type;
	TriStateType<bool> active;
	TriStateType<int> position;
	ExternalClause* external;
	Firebird::string source;
	dsql_nod* localDeclList;
	dsql_nod* body;
	bool compiled;
	bool invalid;
};


class DropTriggerNode : public DdlNode
{
public:
	explicit DropTriggerNode(MemoryPool& p, const Firebird::string& sqlText,
				const Firebird::MetaName& aName)
		: DdlNode(p, sqlText),
		  name(p, aName),
		  silent(false)
	{
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, jrd_tra* transaction);

protected:
	virtual DdlNode* internalDsqlPass();

public:
	Firebird::MetaName name;
	bool silent;
};


class RecreateTriggerNode : public DdlNode
{
public:
	explicit RecreateTriggerNode(MemoryPool& p, const Firebird::string& sqlText,
				CreateAlterTriggerNode* aCreateNode)
		: DdlNode(p, sqlText),
		  createNode(aCreateNode),
		  dropNode(p, sqlText, createNode->name)
	{
		dropNode.silent = true;
	}

protected:
	virtual CreateAlterTriggerNode* getCreateAlterNode()
	{
		return createNode;
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, jrd_tra* transaction);

protected:
	virtual DdlNode* internalDsqlPass();

private:
	CreateAlterTriggerNode* createNode;
	DropTriggerNode dropNode;
};


class CreateSequenceNode : public DdlNode
{
public:
	explicit CreateSequenceNode(MemoryPool& pool, const Firebird::string& sqlText,
				const Firebird::MetaName& aName)
		: DdlNode(pool, sqlText),
		  name(pool, aName)
	{
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, jrd_tra* transaction);

public:
	static void store(thread_db* tdbb, jrd_tra* transaction, const Firebird::MetaName& name,
		fb_sysflag sysFlag);

private:
	Firebird::MetaName name;
};


} // namespace

#endif // DSQL_DDL_NODES_H
