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
#include "../jrd/blr.h"
#include "../jrd/dyn.h"
#include "../jrd/msg_encode.h"
#include "../dsql/node.h"
#include "../dsql/make_proto.h"
#include "../dsql/BlrWriter.h"
#include "../dsql/Nodes.h"
#include "../common/classes/array.h"
#include "../common/classes/ByteChunk.h"
#include "../common/classes/TriState.h"
#include "../dsql/errd_proto.h"

namespace Jrd {


class ExternalClause
{
public:
	ExternalClause(MemoryPool& p, const ExternalClause& o)
		: name(p, o.name),
		  engine(p, o.engine)
	{
	}

	explicit ExternalClause(MemoryPool& p)
		: name(p),
		  engine(p)
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
	void resolve(DsqlCompilerScratch* dsqlScratch, bool modifying = false);

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

protected:
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector << Firebird::Arg::Gds(isc_dsql_alter_charset_failed) << charSet;
	}

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

protected:
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		/*** ASF: FIXME: When returning, str is destroyed but its pointer is recorded.
		Firebird::string str(objName.c_str());
		if (subName.hasData())
			str.append(".").append(subName.c_str());
		statusVector << Firebird::Arg::Gds(isc_dsql_comment_on_failed) << str;
		***/
		statusVector << Firebird::Arg::Gds(isc_dsql_comment_on_failed) << objName;
	}

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
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector <<
			Firebird::Arg::Gds(createAlterCode(create, alter,
					isc_dsql_create_func_failed, isc_dsql_alter_func_failed,
					isc_dsql_create_alter_func_failed)) <<
				name;
	}

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
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector << Firebird::Arg::Gds(isc_dsql_drop_func_failed) << name;
	}

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
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector << Firebird::Arg::Gds(isc_dsql_recreate_func_failed) << createNode->name;
	}

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
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector <<
			Firebird::Arg::Gds(createAlterCode(create, alter,
					isc_dsql_create_proc_failed, isc_dsql_alter_proc_failed,
					isc_dsql_create_alter_proc_failed)) <<
				name;
	}

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
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector << Firebird::Arg::Gds(isc_dsql_drop_proc_failed) << name;
	}

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
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector << Firebird::Arg::Gds(isc_dsql_recreate_proc_failed) << createNode->name;
	}

	virtual DdlNode* internalDsqlPass();

private:
	CreateAlterProcedureNode* createNode;
	DropProcedureNode dropNode;
};


class TriggerDefinition
{
public:
	TriggerDefinition(MemoryPool& p)
		: name(p),
		  relationName(p),
		  external(NULL),
		  source(p),
		  systemFlag(fb_sysflag_user),
		  fkTrigger(false)
	{
	}

	void store(thread_db* tdbb, jrd_tra* transaction);
	bool modify(thread_db* tdbb, jrd_tra* transaction);

protected:
	virtual void preModify(thread_db* tdbb, jrd_tra* transaction)
	{
	}

	virtual void postModify(thread_db* tdbb, jrd_tra* transaction)
	{
	}

public:
	Firebird::MetaName name;
	Firebird::MetaName relationName;
	TriStateType<FB_UINT64> type;
	TriStateType<bool> active;
	TriStateType<int> position;
	ExternalClause* external;
	Firebird::string source;
	Firebird::ByteChunk blrData;
	Firebird::ByteChunk debugData;
	USHORT systemFlag;
	bool fkTrigger;
};


class CreateAlterTriggerNode : public DdlNode, public BlockNode, public TriggerDefinition
{
public:
	explicit CreateAlterTriggerNode(MemoryPool& p, const Firebird::string& sqlText,
				const Firebird::MetaName& aName)
		: DdlNode(p, sqlText),
		  BlockNode(p, false),
		  TriggerDefinition(p),
		  create(true),
		  alter(false),
		  localDeclList(NULL),
		  body(NULL),
		  compiled(false),
		  invalid(false)
	{
		name = aName;
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, jrd_tra* transaction);

protected:
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector <<
			Firebird::Arg::Gds(createAlterCode(create, alter,
					isc_dsql_create_trigger_failed, isc_dsql_alter_trigger_failed,
					isc_dsql_create_alter_trigger_failed)) <<
				name;
	}

	virtual DdlNode* internalDsqlPass();

	virtual void preModify(thread_db* tdbb, jrd_tra* transaction)
	{
		if (alter)
			executeDdlTrigger(tdbb, transaction, DTW_BEFORE, DDL_TRIGGER_ALTER_TRIGGER, name);
	}

	virtual void postModify(thread_db* tdbb, jrd_tra* transaction)
	{
		if (alter)
			executeDdlTrigger(tdbb, transaction, DTW_AFTER, DDL_TRIGGER_ALTER_TRIGGER, name);
	}

private:
	void executeCreate(thread_db* tdbb, jrd_tra* transaction);
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
	bool create;
	bool alter;
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
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector << Firebird::Arg::Gds(isc_dsql_drop_trigger_failed) << name;
	}

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
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector << Firebird::Arg::Gds(isc_dsql_recreate_trigger_failed) << createNode->name;
	}

	virtual DdlNode* internalDsqlPass();

private:
	CreateAlterTriggerNode* createNode;
	DropTriggerNode dropNode;
};


class CreateCollationNode : public DdlNode
{
public:
	explicit CreateCollationNode(MemoryPool& p, const Firebird::string& sqlText,
				const Firebird::MetaName& aName, const Firebird::MetaName& aForCharSet)
		: DdlNode(p, sqlText),
		  name(p, aName),
		  forCharSet(p, aForCharSet),
		  fromName(p),
		  fromExternal(p),
		  specificAttributes(p),
		  attributesOn(0),
		  attributesOff(0),
		  forCharSetId(0),
		  fromCollationId(0)
	{
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, jrd_tra* transaction);

	void setAttribute(USHORT attribute)
	{
		if ((attributesOn | attributesOff) & attribute)
		{
			// msg: 222: "Invalid collation attributes"
			using namespace Firebird;
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
				Arg::Gds(ENCODE_ISC_MSG(222, DYN_MSG_FAC)));
		}

		attributesOn |= attribute;
	}

	void unsetAttribute(USHORT attribute)
	{
		if ((attributesOn | attributesOff) & attribute)
		{
			// msg: 222: "Invalid collation attributes"
			using namespace Firebird;
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
				Arg::Gds(ENCODE_ISC_MSG(222, DYN_MSG_FAC)));
		}

		attributesOff |= attribute;
	}

protected:
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector << Firebird::Arg::Gds(isc_dsql_create_collation_failed) << name;
	}

	virtual DdlNode* internalDsqlPass();

public:
	Firebird::MetaName name;
	Firebird::MetaName forCharSet;
	Firebird::MetaName fromName;
	Firebird::string fromExternal;
	Firebird::UCharBuffer specificAttributes;

private:
	USHORT attributesOn;
	USHORT attributesOff;
	USHORT forCharSetId;
	USHORT fromCollationId;
};


class DropCollationNode : public DdlNode
{
public:
	explicit DropCollationNode(MemoryPool& p, const Firebird::string& sqlText,
				const Firebird::MetaName& aName)
		: DdlNode(p, sqlText),
		  name(p, aName)
	{
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, jrd_tra* transaction);

protected:
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector << Firebird::Arg::Gds(isc_dsql_drop_collation_failed) << name;
	}

public:
	Firebird::MetaName name;
};


class CreateDomainNode : public DdlNode
{
public:
	explicit CreateDomainNode(MemoryPool& p, const Firebird::string& sqlText,
				const ParameterClause& aNameType)
		: DdlNode(p, sqlText),
		  nameType(aNameType),
		  notNull(false),
		  check(NULL)
	{
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, jrd_tra* transaction);

protected:
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector << Firebird::Arg::Gds(isc_dsql_create_domain_failed) << nameType.name;
	}

public:
	ParameterClause nameType;
	bool notNull;
	dsql_nod* check;
};


class AlterDomainNode : public DdlNode
{
public:
	explicit AlterDomainNode(MemoryPool& p, const Firebird::string& sqlText,
				const Firebird::MetaName& aName)
		: DdlNode(p, sqlText),
		  name(p, aName),
		  dropConstraint(false),
		  dropDefault(false),
		  setConstraint(NULL),
		  setDefault(NULL),
		  renameTo(p)
	{
	}

public:
	static void checkUpdate(const dyn_fld& origFld, const dyn_fld& newFld);
	static ULONG checkUpdateNumericType(const dyn_fld& origFld, const dyn_fld& newFld);
	static void getDomainType(thread_db* tdbb, jrd_tra* transaction, dyn_fld& dynFld);
	static void modifyLocalFieldIndex(thread_db* tdbb, jrd_tra* transaction,
		const Firebird::MetaName& relationName, const Firebird::MetaName& fieldName,
		const Firebird::MetaName& newFieldName);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, jrd_tra* transaction);

protected:
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector << Firebird::Arg::Gds(isc_dsql_alter_domain_failed) << name;
	}

private:
	void rename(thread_db* tdbb, jrd_tra* transaction, SSHORT dimensions);

public:
	Firebird::MetaName name;
	bool dropConstraint;
	bool dropDefault;
	dsql_nod* setConstraint;
	dsql_nod* setDefault;
	Firebird::MetaName renameTo;
	Firebird::AutoPtr<TypeClause> type;
};


class DropDomainNode : public DdlNode
{
public:
	explicit DropDomainNode(MemoryPool& p, const Firebird::string& sqlText,
				const Firebird::MetaName& aName)
		: DdlNode(p, sqlText),
		  name(p, aName)
	{
	}

	static bool deleteDimensionRecords(thread_db* tdbb, jrd_tra* transaction,
		const Firebird::MetaName& name);

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, jrd_tra* transaction);

protected:
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector << Firebird::Arg::Gds(isc_dsql_drop_domain_failed) << name;
	}

private:
	void check(thread_db* tdbb, jrd_tra* transaction);

public:
	Firebird::MetaName name;
};


class CreateAlterExceptionNode : public DdlNode
{
public:
	explicit CreateAlterExceptionNode(MemoryPool& p, const Firebird::string& sqlText,
				const Firebird::MetaName& aName, const Firebird::string& aMessage)
		: DdlNode(p, sqlText),
		  name(p, aName),
		  message(p, aMessage),
		  create(true),
		  alter(false)
	{
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, jrd_tra* transaction);

protected:
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector <<
			Firebird::Arg::Gds(createAlterCode(create, alter,
					isc_dsql_create_except_failed, isc_dsql_alter_except_failed,
					isc_dsql_create_alter_except_failed)) <<
				name;
	}

private:
	void executeCreate(thread_db* tdbb, jrd_tra* transaction);
	bool executeAlter(thread_db* tdbb, jrd_tra* transaction);

public:
	Firebird::MetaName name;
	Firebird::string message;
	bool create;
	bool alter;
};


class DropExceptionNode : public DdlNode
{
public:
	explicit DropExceptionNode(MemoryPool& p, const Firebird::string& sqlText,
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
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector << Firebird::Arg::Gds(isc_dsql_drop_except_failed) << name;
	}

public:
	Firebird::MetaName name;
	bool silent;
};


class RecreateExceptionNode : public DdlNode
{
public:
	explicit RecreateExceptionNode(MemoryPool& p, const Firebird::string& sqlText,
				CreateAlterExceptionNode* aCreateNode)
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
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector << Firebird::Arg::Gds(isc_dsql_recreate_except_failed) << createNode->name;
	}

	virtual DdlNode* internalDsqlPass();

private:
	CreateAlterExceptionNode* createNode;
	DropExceptionNode dropNode;
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

	static void store(thread_db* tdbb, jrd_tra* transaction, const Firebird::MetaName& name,
		fb_sysflag sysFlag);

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, jrd_tra* transaction);

protected:
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector << Firebird::Arg::Gds(isc_dsql_create_sequence_failed) << name;
	}

private:
	Firebird::MetaName name;
};


class DropSequenceNode
{
public:
	static bool deleteGenerator(thread_db* tdbb, jrd_tra* transaction,
		const Firebird::MetaName& name);
};


class RelationNode : public DdlNode
{
public:
	struct Constraint : public PermanentStorage
	{
		enum Type { TYPE_CHECK, TYPE_NOT_NULL, TYPE_PK, TYPE_UNIQUE, TYPE_FK };

		// Specialized BlrWriter for constraints.
		class BlrWriter : public Jrd::BlrWriter
		{
		public:
			BlrWriter(MemoryPool& p)
				: Jrd::BlrWriter(p),
				  dsqlScratch(NULL)
			{
			}

			void init(DsqlCompilerScratch* aDsqlScratch)
			{
				dsqlScratch = aDsqlScratch;
				dsqlScratch->getBlrData().clear();
				dsqlScratch->getDebugData().clear();
				appendUChar(isVersion4() ? blr_version4 : blr_version5);
			}

			virtual bool isVersion4()
			{
				return dsqlScratch->isVersion4();
			}

		protected:
			virtual bool isDdlDyn()
			{
				return false;
			}

		private:
			DsqlCompilerScratch* dsqlScratch;
		};

		Constraint(MemoryPool& p)
			: PermanentStorage(p),
			  type(TYPE_CHECK),	// Just something to initialize. Do not assume it.
			  name(p),
			  columns(p),
			  indexName(p),
			  descending(false),
			  refRelation(p),
			  refColumns(p),
			  refUpdateAction(RI_ACTION_NONE),
			  refDeleteAction(RI_ACTION_NONE),
			  triggers(p),
			  blrWritersHolder(p)
		{
		}

		Constraint::Type type;
		Firebird::MetaName name;
		Firebird::ObjectsArray<Firebird::MetaName> columns;
		Firebird::MetaName indexName;
		bool descending;
		Firebird::MetaName refRelation;
		Firebird::ObjectsArray<Firebird::MetaName> refColumns;
		const char* refUpdateAction;
		const char* refDeleteAction;
		Firebird::ObjectsArray<TriggerDefinition> triggers;
		Firebird::ObjectsArray<BlrWriter> blrWritersHolder;
	};

	explicit RelationNode(MemoryPool& p, const Firebird::string& sqlText, dsql_nod* aDsqlNode)
		: DdlNode(p, sqlText),
		  dsqlNode(aDsqlNode),
		  name(p, ((dsql_str*) aDsqlNode->nod_arg[Dsql::e_rln_name])->str_data),
		  elements(p)
	{
	}

	static void deleteLocalField(thread_db* tdbb, jrd_tra* transaction,
		const Firebird::MetaName& relationName, const Firebird::MetaName& fieldName);

protected:
	void defineField(thread_db* tdbb, jrd_tra* transaction, const dsql_nod* element,
		SSHORT position, const dsql_nod* pkcols);
	void defineComputed(thread_db* tdbb, dsql_fld* field, dsql_nod* node,
		Firebird::string& source, BlrWriter::BlrData& value);
	bool defineDefault(thread_db* tdbb, dsql_fld* field, dsql_nod* node,
		Firebird::string& source, BlrWriter::BlrData& value);
	void makeConstraint(thread_db* tdbb, jrd_tra* transaction, const dsql_nod* node,
		Firebird::ObjectsArray<Constraint>& constraints, bool* notNull = NULL);
	void defineConstraint(thread_db* tdbb, jrd_tra* transaction, Constraint& constraint);
	void defineCheckConstraint(Constraint& constraint, dsql_nod* node);
	void defineCheckConstraintTrigger(Constraint& constraint, dsql_nod* node,
		FB_UINT64 triggerType);
	void defineSetDefaultTrigger(Constraint& constraint, bool onUpdate);
	void defineSetNullTrigger(Constraint& constraint, bool onUpdate);
	void defineDeleteCascadeTrigger(Constraint& constraint);
	void defineUpdateCascadeTrigger(Constraint& constraint);
	void generateUnnamedTriggerBeginning(Constraint& constraint, bool onUpdate, BlrWriter& blrWriter);
	void stuffDefaultBlr(const Firebird::ByteChunk& defaultBlr, BlrWriter& blrWriter);
	void stuffMatchingBlr(Constraint& constraint, BlrWriter& blrWriter);
	void stuffTriggerFiringCondition(Constraint& constraint, BlrWriter& blrWriter);

public:
	dsql_nod* dsqlNode;
	Firebird::MetaName name;
	Firebird::Array<dsql_nod*> elements;
};


class CreateRelationNode : public RelationNode
{
public:
	explicit CreateRelationNode(MemoryPool& p, const Firebird::string& sqlText,
				dsql_nod* aDsqlNode, const Firebird::PathName* aExternalFile = NULL)
		: RelationNode(p, sqlText, aDsqlNode),
		  externalFile(aExternalFile),
		  relationType(rel_persistent)
	{
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, jrd_tra* transaction);

protected:
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector << Firebird::Arg::Gds(isc_dsql_create_table_failed) << name;
	}

private:
	const dsql_nod* findPkColumns();

public:
	const Firebird::PathName* externalFile;
	rel_t relationType;
};


class AlterRelationNode : public RelationNode
{
public:
	explicit AlterRelationNode(MemoryPool& p, const Firebird::string& sqlText, dsql_nod* aDsqlNode)
		: RelationNode(p, sqlText, aDsqlNode)
	{
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, jrd_tra* transaction);

protected:
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector << Firebird::Arg::Gds(isc_dsql_alter_table_failed) << name;
	}

private:
	void modifyField(thread_db* tdbb, jrd_tra* transaction, const dsql_nod* element);
};


class DropRelationNode : public DdlNode
{
public:
	explicit DropRelationNode(MemoryPool& p, const Firebird::string& sqlText,
				const Firebird::MetaName& aName, bool aView)
		: DdlNode(p, sqlText),
		  name(p, aName),
		  view(aView),
		  silent(false)
	{
	}

	static void deleteGlobalField(thread_db* tdbb, jrd_tra* transaction,
		const Firebird::MetaName& globalName);

	static void erase(thread_db* tdbb, jrd_tra* transaction, const Firebird::MetaName& name,
		bool view, const Firebird::string& sqlText);

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, jrd_tra* transaction);

protected:
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector << Firebird::Arg::Gds(isc_dsql_drop_table_failed) << name;
	}

public:
	Firebird::MetaName name;
	bool view;
	bool silent;
};


class RecreateRelationNode : public DdlNode
{
public:
	explicit RecreateRelationNode(MemoryPool& p, const Firebird::string& sqlText,
				CreateRelationNode* aCreateNode)
		: DdlNode(p, sqlText),
		  createNode(aCreateNode),
		  dropNode(p, sqlText, createNode->name, false)
	{
		dropNode.silent = true;
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, jrd_tra* transaction);

protected:
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector << Firebird::Arg::Gds(isc_dsql_recreate_table_failed) << createNode->name;
	}

	virtual DdlNode* internalDsqlPass();

private:
	CreateRelationNode* createNode;
	DropRelationNode dropNode;
};


class CreateIndexNode
{
public:
	struct Definition
	{
		Definition()
			: type(0)
		{
			expressionBlr.clear();
			expressionSource.clear();
		}

		Firebird::MetaName relation;
		Firebird::ObjectsArray<Firebird::MetaName> columns;
		TriStateType<bool> unique;
		TriStateType<bool> descending;
		TriStateType<bool> inactive;
		SSHORT type;
		bid expressionBlr;
		bid expressionSource;
		Firebird::MetaName refRelation;
		Firebird::ObjectsArray<Firebird::MetaName> refColumns;
	};

	static void store(thread_db* tdbb, jrd_tra* transaction, Firebird::MetaName& name,
		Definition& definition, Firebird::MetaName* referredIndexName = NULL);
};


class DropIndexNode
{
public:
	static bool deleteSegmentRecords(thread_db* tdbb, jrd_tra* transaction,
		const Firebird::MetaName& name);
};


} // namespace

#endif // DSQL_DDL_NODES_H
