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

#include "../common/common.h"
#include "../jrd/blr.h"
#include "../jrd/dyn.h"
#include "../jrd/msg_encode.h"
#include "../dsql/node.h"
#include "../dsql/make_proto.h"
#include "../dsql/BlrWriter.h"
#include "../dsql/Nodes.h"
#include "../common/classes/array.h"
#include "../common/classes/ByteChunk.h"
#include "../common/classes/Nullable.h"
#include "../jrd/vio_proto.h"
#include "../dsql/errd_proto.h"

namespace Jrd {

class CompoundStmtNode;


class ExternalClause
{
public:
	ExternalClause(MemoryPool& p, const ExternalClause& o)
		: name(p, o.name),
		  engine(p, o.engine),
		  udfModule(p)
	{
	}

	explicit ExternalClause(MemoryPool& p)
		: name(p),
		  engine(p),
		  udfModule(p)
	{
	}

public:
	Firebird::string name;
	Firebird::MetaName engine;
	Firebird::string udfModule;
};


class TypeClause
{
public:
	TypeClause(MemoryPool& pool, dsql_fld* aField, const Firebird::MetaName& aCollate);
	virtual ~TypeClause() {}

public:
	void resolve(DsqlCompilerScratch* dsqlScratch, bool modifying = false);
	void setup(DsqlCompilerScratch* dsqlScratch);

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
	ParameterClause(MemoryPool& pool, dsql_fld* field, const Firebird::MetaName& aCollate,
		dsql_nod* dflt, dsql_nod* aLegacyParameter);

public:
	void print(Firebird::string& text) const;

public:
	Firebird::MetaName name;
	dsql_nod* legacyDefault;
	dsql_nod* legacyParameter;
	Nullable<int> udfMechanism;
};


template <typename CreateNode, typename DropNode, ISC_STATUS ERROR_CODE>
class RecreateNode : public DdlNode
{
public:
	RecreateNode(MemoryPool& p, CreateNode* aCreateNode)
		: DdlNode(p),
		  createNode(aCreateNode),
		  dropNode(p, createNode->name)
	{
		dropNode.silent = true;
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& /*nodes*/) const
	{
		text.printf("RecreateNode\n");
	}

	virtual void execute(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction)
	{
		// run all statements under savepoint control
		AutoSavePoint savePoint(tdbb, transaction);

		dropNode.execute(tdbb, dsqlScratch, transaction);
		createNode->execute(tdbb, dsqlScratch, transaction);

		savePoint.release();	// everything is ok
	}

	virtual DdlNode* dsqlPass(DsqlCompilerScratch* dsqlScratch)
	{
		createNode->dsqlPass(dsqlScratch);
		dropNode.dsqlPass(dsqlScratch);
		return DdlNode::dsqlPass(dsqlScratch);
	}

protected:
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector << Firebird::Arg::Gds(ERROR_CODE) << createNode->name;
	}

protected:
	CreateNode* createNode;
	DropNode dropNode;
};


class AlterCharSetNode : public DdlNode
{
public:
	AlterCharSetNode(MemoryPool& pool, const Firebird::MetaName& aCharSet,
				const Firebird::MetaName& aDefaultCollation)
		: DdlNode(pool),
		  charSet(pool, aCharSet),
		  defaultCollation(pool, aDefaultCollation)
	{
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction);

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
	CommentOnNode(MemoryPool& pool, int aObjType,
				const Firebird::MetaName& aObjName, const Firebird::MetaName& aSubName,
				const Firebird::string& aText, const char* aTextCharSet)
		: DdlNode(pool),
		  objType(aObjType),
		  objName(pool, aObjName),
		  subName(pool, aSubName),
		  text(pool, aText),
		  textCharSet(aTextCharSet)
	{
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction);

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


class CreateAlterFunctionNode : public DdlNode
{
public:
	CreateAlterFunctionNode(MemoryPool& pool, const Firebird::MetaName& aName)
		: DdlNode(pool),
		  name(pool, aName),
		  create(true),
		  alter(false),
		  external(NULL),
		  deterministic(false),
		  parameters(pool),
		  returnType(pool, NULL, NULL, NULL, NULL),
		  localDeclList(NULL),
		  source(pool),
		  body(NULL),
		  compiled(false),
		  invalid(false),
		  package(pool),
		  packageOwner(pool),
		  privateScope(false),
		  udfReturnPos(0)
	{
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual DdlNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void execute(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction);

protected:
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector <<
			Firebird::Arg::Gds(createAlterCode(create, alter,
					isc_dsql_create_func_failed, isc_dsql_alter_func_failed,
					isc_dsql_create_alter_func_failed)) <<
				name;
	}

private:
	bool isUdf()
	{
		return external && external->udfModule.hasData();
	}

	void executeCreate(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction);
	bool executeAlter(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction,
		bool secondPass, bool runTriggers);

	void storeArgument(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction,
		unsigned pos, const ParameterClause& parameter, const bid* comment);
	void compile(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch);
	void collectParamComments(thread_db* tdbb, jrd_tra* transaction, MetaNameBidMap& items);

public:
	Firebird::MetaName name;
	bool create;
	bool alter;
	ExternalClause* external;
	bool deterministic;
	Firebird::Array<ParameterClause> parameters;
	ParameterClause returnType;
	NestConst<CompoundStmtNode> localDeclList;
	Firebird::string source;
	NestConst<StmtNode> body;
	bool compiled;
	bool invalid;
	Firebird::MetaName package;
	Firebird::string packageOwner;
	bool privateScope;
	SLONG udfReturnPos;
};


class DropFunctionNode : public DdlNode
{
public:
	DropFunctionNode(MemoryPool& pool, const Firebird::MetaName& aName)
		: DdlNode(pool),
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
	virtual DdlNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void execute(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction);

protected:
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector << Firebird::Arg::Gds(isc_dsql_drop_func_failed) << name;
	}

public:
	Firebird::MetaName name;
	bool silent;
	Firebird::MetaName package;
};


typedef RecreateNode<CreateAlterFunctionNode, DropFunctionNode, isc_dsql_recreate_func_failed>
	RecreateFunctionNode;


class CreateAlterProcedureNode : public DdlNode
{
public:
	CreateAlterProcedureNode(MemoryPool& pool, const Firebird::MetaName& aName)
		: DdlNode(pool),
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
	virtual DdlNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void execute(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction);

protected:
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector <<
			Firebird::Arg::Gds(createAlterCode(create, alter,
					isc_dsql_create_proc_failed, isc_dsql_alter_proc_failed,
					isc_dsql_create_alter_proc_failed)) <<
				name;
	}

private:
	void executeCreate(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction);
	bool executeAlter(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction,
		bool secondPass, bool runTriggers);
	void storeParameter(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction,
		USHORT type, unsigned pos, const ParameterClause& parameter, const bid* comment);
	void compile(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch);
	void collectParamComments(thread_db* tdbb, jrd_tra* transaction, MetaNameBidMap& items);

public:
	Firebird::MetaName name;
	bool create;
	bool alter;
	ExternalClause* external;
	Firebird::Array<ParameterClause> parameters;
	Firebird::Array<ParameterClause> returns;
	Firebird::string source;
	NestConst<CompoundStmtNode> localDeclList;
	NestConst<StmtNode> body;
	bool compiled;
	bool invalid;
	Firebird::MetaName package;
	Firebird::string packageOwner;
	bool privateScope;
};


class DropProcedureNode : public DdlNode
{
public:
	DropProcedureNode(MemoryPool& pool, const Firebird::MetaName& aName)
		: DdlNode(pool),
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
	virtual DdlNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void execute(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction);

protected:
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector << Firebird::Arg::Gds(isc_dsql_drop_proc_failed) << name;
	}

public:
	Firebird::MetaName name;
	bool silent;
	Firebird::MetaName package;
};


typedef RecreateNode<CreateAlterProcedureNode, DropProcedureNode, isc_dsql_recreate_proc_failed>
	RecreateProcedureNode;


class TriggerDefinition
{
public:
	explicit TriggerDefinition(MemoryPool& p)
		: name(p),
		  relationName(p),
		  external(NULL),
		  source(p),
		  systemFlag(fb_sysflag_user),
		  fkTrigger(false)
	{
	}

	void store(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction);
	bool modify(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction);

protected:
	virtual void preModify(thread_db* /*tdbb*/, DsqlCompilerScratch* /*dsqlScratch*/,
		jrd_tra* /*transaction*/)
	{
	}

	virtual void postModify(thread_db* /*tdbb*/, DsqlCompilerScratch* /*dsqlScratch*/,
		jrd_tra* /*transaction*/)
	{
	}

public:
	Firebird::MetaName name;
	Firebird::MetaName relationName;
	Nullable<FB_UINT64> type;
	Nullable<bool> active;
	Nullable<int> position;
	ExternalClause* external;
	Firebird::string source;
	Firebird::ByteChunk blrData;
	Firebird::ByteChunk debugData;
	USHORT systemFlag;
	bool fkTrigger;
};


class CreateAlterTriggerNode : public DdlNode, public TriggerDefinition
{
public:
	CreateAlterTriggerNode(MemoryPool& p, const Firebird::MetaName& aName)
		: DdlNode(p),
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
	virtual DdlNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void execute(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction);

protected:
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector <<
			Firebird::Arg::Gds(createAlterCode(create, alter,
					isc_dsql_create_trigger_failed, isc_dsql_alter_trigger_failed,
					isc_dsql_create_alter_trigger_failed)) <<
				name;
	}

	virtual void preModify(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction)
	{
		if (alter)
		{
			executeDdlTrigger(tdbb, dsqlScratch, transaction, DTW_BEFORE,
				DDL_TRIGGER_ALTER_TRIGGER, name);
		}
	}

	virtual void postModify(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction)
	{
		if (alter)
		{
			executeDdlTrigger(tdbb, dsqlScratch, transaction, DTW_AFTER,
				DDL_TRIGGER_ALTER_TRIGGER, name);
		}
	}

private:
	void executeCreate(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction);
	void compile(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch);

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
	NestConst<CompoundStmtNode> localDeclList;
	NestConst<StmtNode> body;
	bool compiled;
	bool invalid;
};


class DropTriggerNode : public DdlNode
{
public:
	DropTriggerNode(MemoryPool& p, const Firebird::MetaName& aName)
		: DdlNode(p),
		  name(p, aName),
		  silent(false)
	{
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual DdlNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void execute(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction);

protected:
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector << Firebird::Arg::Gds(isc_dsql_drop_trigger_failed) << name;
	}

public:
	Firebird::MetaName name;
	bool silent;
};


typedef RecreateNode<CreateAlterTriggerNode, DropTriggerNode, isc_dsql_recreate_trigger_failed>
	RecreateTriggerNode;


class CreateCollationNode : public DdlNode
{
public:
	CreateCollationNode(MemoryPool& p, const Firebird::MetaName& aName,
				const Firebird::MetaName& aForCharSet)
		: DdlNode(p),
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
	virtual DdlNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void execute(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction);

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
	DropCollationNode(MemoryPool& p, const Firebird::MetaName& aName)
		: DdlNode(p),
		  name(p, aName)
	{
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction);

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
	CreateDomainNode(MemoryPool& p, const ParameterClause& aNameType)
		: DdlNode(p),
		  nameType(aNameType),
		  notNull(false),
		  check(NULL)
	{
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction);

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
	AlterDomainNode(MemoryPool& p, const Firebird::MetaName& aName)
		: DdlNode(p),
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
	virtual void execute(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction);

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
	Nullable<bool> notNullFlag;	// true = NOT NULL / false = NULL
};


class DropDomainNode : public DdlNode
{
public:
	DropDomainNode(MemoryPool& p, const Firebird::MetaName& aName)
		: DdlNode(p),
		  name(p, aName)
	{
	}

	static bool deleteDimensionRecords(thread_db* tdbb, jrd_tra* transaction,
		const Firebird::MetaName& name);

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction);

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
	CreateAlterExceptionNode(MemoryPool& p, const Firebird::MetaName& aName,
				const Firebird::string& aMessage)
		: DdlNode(p),
		  name(p, aName),
		  message(p, aMessage),
		  create(true),
		  alter(false)
	{
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction);

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
	void executeCreate(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction);
	bool executeAlter(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction);

public:
	Firebird::MetaName name;
	Firebird::string message;
	bool create;
	bool alter;
};


class DropExceptionNode : public DdlNode
{
public:
	DropExceptionNode(MemoryPool& p, const Firebird::MetaName& aName)
		: DdlNode(p),
		  name(p, aName),
		  silent(false)
	{
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction);

protected:
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector << Firebird::Arg::Gds(isc_dsql_drop_except_failed) << name;
	}

public:
	Firebird::MetaName name;
	bool silent;
};


typedef RecreateNode<CreateAlterExceptionNode, DropExceptionNode, isc_dsql_recreate_except_failed>
	RecreateExceptionNode;


class CreateSequenceNode : public DdlNode
{
public:
	CreateSequenceNode(MemoryPool& pool, const Firebird::MetaName& aName)
		: DdlNode(pool),
		  name(pool, aName)
	{
	}

	static void store(thread_db* tdbb, jrd_tra* transaction, const Firebird::MetaName& name,
		fb_sysflag sysFlag);

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction);

protected:
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector << Firebird::Arg::Gds(isc_dsql_create_sequence_failed) << name;
	}

public:
	Firebird::MetaName name;
};


class DropSequenceNode : public DdlNode
{
public:
	DropSequenceNode(MemoryPool& pool, const Firebird::MetaName&aName)
		: DdlNode(pool),
		  name(pool, aName),
		  silent(false)
	{
	}

	static void deleteIdentity(thread_db* tdbb, jrd_tra* transaction,
		const Firebird::MetaName& name);

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction);

protected:
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector << Firebird::Arg::Gds(isc_dsql_drop_sequence_failed) << name;
	}

public:
	Firebird::MetaName name;
	bool silent;
};


typedef RecreateNode<CreateSequenceNode, DropSequenceNode, isc_dsql_recreate_sequence_failed>
	RecreateSequenceNode;


class RelationNode : public DdlNode
{
public:
	class FieldDefinition
	{
	public:
		explicit FieldDefinition(MemoryPool& p)
			: name(p),
			  relationName(p),
			  fieldSource(p),
			  identitySequence(p),
			  defaultSource(p),
			  baseField(p)
		{
		}

		void modify(thread_db* tdbb, jrd_tra* transaction);
		void store(thread_db* tdbb, jrd_tra* transaction);

	public:
		Firebird::MetaName name;
		Firebird::MetaName relationName;
		Firebird::MetaName fieldSource;
		Firebird::MetaName identitySequence;
		Nullable<USHORT> collationId;
		Nullable<bool> notNullFlag;	// true = NOT NULL / false = NULL
		Nullable<USHORT> position;
		Firebird::string defaultSource;
		Firebird::ByteChunk defaultValue;
		Nullable<USHORT> viewContext;
		Firebird::MetaName baseField;
	};

	struct Constraint : public PermanentStorage
	{
		enum Type { TYPE_CHECK, TYPE_NOT_NULL, TYPE_PK, TYPE_UNIQUE, TYPE_FK };

		// Specialized BlrWriter for constraints.
		class BlrWriter : public Jrd::BlrWriter
		{
		public:
			explicit BlrWriter(MemoryPool& p)
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

		explicit Constraint(MemoryPool& p)
			: PermanentStorage(p),
			  type(TYPE_CHECK),	// Just something to initialize. Do not assume it.
			  name(p),
			  columns(p),
			  indexName(p),
			  descending(false),
			  refRelation(p),
			  refColumns(p),
			  refUpdateAction(RI_RESTRICT),
			  refDeleteAction(RI_RESTRICT),
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

	RelationNode(MemoryPool& p, dsql_nod* aDsqlNode);

	static void deleteLocalField(thread_db* tdbb, jrd_tra* transaction,
		const Firebird::MetaName& relationName, const Firebird::MetaName& fieldName);

protected:
	void storePrivileges(thread_db* tdbb, jrd_tra* transaction);
	void defineField(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction,
		const dsql_nod* element, SSHORT position, const dsql_nod* pkcols);
	void defineComputed(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, dsql_fld* field,
		dsql_nod* node, Firebird::string& source, BlrWriter::BlrData& value);
	bool defineDefault(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, dsql_fld* field,
		dsql_nod* node, Firebird::string& source, BlrWriter::BlrData& value);
	void makeConstraint(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction,
		const dsql_nod* node, Firebird::ObjectsArray<Constraint>& constraints, bool* notNull = NULL);
	void defineConstraint(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction,
		Constraint& constraint);
	void defineCheckConstraint(DsqlCompilerScratch* dsqlScratch, Constraint& constraint,
		dsql_nod* node);
	void defineCheckConstraintTrigger(DsqlCompilerScratch* dsqlScratch, Constraint& constraint,
		dsql_nod* node, FB_UINT64 triggerType);
	void defineSetDefaultTrigger(DsqlCompilerScratch* dsqlScratch, Constraint& constraint,
		bool onUpdate);
	void defineSetNullTrigger(DsqlCompilerScratch* dsqlScratch, Constraint& constraint,
		bool onUpdate);
	void defineDeleteCascadeTrigger(DsqlCompilerScratch* dsqlScratch, Constraint& constraint);
	void defineUpdateCascadeTrigger(DsqlCompilerScratch* dsqlScratch, Constraint& constraint);
	void generateUnnamedTriggerBeginning(Constraint& constraint, bool onUpdate,
		BlrWriter& blrWriter);
	void stuffDefaultBlr(const Firebird::ByteChunk& defaultBlr, BlrWriter& blrWriter);
	void stuffMatchingBlr(Constraint& constraint, BlrWriter& blrWriter);
	void stuffTriggerFiringCondition(const Constraint& constraint, BlrWriter& blrWriter);

public:
	dsql_nod* dsqlNode;
	Firebird::MetaName name;
	Firebird::Array<dsql_nod*> elements;
};


class CreateRelationNode : public RelationNode
{
public:
	CreateRelationNode(MemoryPool& p, dsql_nod* aDsqlNode,
				const Firebird::PathName* aExternalFile = NULL)
		: RelationNode(p, aDsqlNode),
		  externalFile(aExternalFile),
		  relationType(rel_persistent)
	{
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction);

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
	AlterRelationNode(MemoryPool& p, dsql_nod* aDsqlNode)
		: RelationNode(p, aDsqlNode)
	{
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction);

protected:
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector << Firebird::Arg::Gds(isc_dsql_alter_table_failed) << name;
	}

private:
	void modifyField(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction,
		const dsql_nod* element);
};


class DropRelationNode : public DdlNode
{
public:
	DropRelationNode(MemoryPool& p, const Firebird::MetaName& aName, bool aView = false)
		: DdlNode(p),
		  name(p, aName),
		  view(aView),
		  silent(false)
	{
	}

	static void deleteGlobalField(thread_db* tdbb, jrd_tra* transaction,
		const Firebird::MetaName& globalName);

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction);

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


typedef RecreateNode<CreateRelationNode, DropRelationNode, isc_dsql_recreate_table_failed>
	RecreateTableNode;


class CreateAlterViewNode : public RelationNode
{
public:
	CreateAlterViewNode(MemoryPool& p, dsql_nod* aDsqlNode, dsql_nod* aViewFields,
				dsql_nod* aSelectExpr)
		: RelationNode(p, aDsqlNode),
		  create(true),
		  alter(false),
		  viewFields(aViewFields),
		  selectExpr(aSelectExpr),
		  source(p),
		  withCheckOption(false)
	{
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual DdlNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void execute(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction);

protected:
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector <<
			Firebird::Arg::Gds(createAlterCode(create, alter,
					isc_dsql_create_view_failed, isc_dsql_alter_view_failed,
					isc_dsql_create_alter_view_failed)) <<
				name;
	}

private:
	void createCheckTriggers(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, dsql_nod* items);
	void createCheckTrigger(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch,
		dsql_nod* rse, dsql_nod* items, CompoundStmtNode* actions, TriggerType triggerType);
	void defineUpdateAction(DsqlCompilerScratch* dsqlScratch, dsql_nod** baseAndNode,
		dsql_nod** baseRelation, dsql_nod* items);
	static dsql_nod* replaceFieldNames(dsql_nod* input, dsql_nod* searchFields,
		dsql_nod* replaceFields, bool nullThem, const char* contextName);

public:
	bool create;
	bool alter;
	dsql_nod* viewFields;
	dsql_nod* selectExpr;
	Firebird::string source;
	bool withCheckOption;
};


class RecreateViewNode :
	public RecreateNode<CreateAlterViewNode, DropRelationNode, isc_dsql_recreate_view_failed>
{
public:
	RecreateViewNode(MemoryPool& p, CreateAlterViewNode* aCreateNode)
		: RecreateNode<CreateAlterViewNode, DropRelationNode, isc_dsql_recreate_view_failed>(
				p, aCreateNode)
	{
		dropNode.view = true;
	}
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
		Nullable<bool> unique;
		Nullable<bool> descending;
		Nullable<bool> inactive;
		SSHORT type;
		bid expressionBlr;
		bid expressionSource;
		Firebird::MetaName refRelation;
		Firebird::ObjectsArray<Firebird::MetaName> refColumns;
	};

	static void store(thread_db* tdbb, jrd_tra* transaction, Firebird::MetaName& name,
		const Definition& definition, Firebird::MetaName* referredIndexName = NULL);
};


class DropIndexNode
{
public:
	static bool deleteSegmentRecords(thread_db* tdbb, jrd_tra* transaction,
		const Firebird::MetaName& name);
};


} // namespace

#endif // DSQL_DDL_NODES_H
