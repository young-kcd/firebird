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
 *  Copyright (c) 2009 Adriano dos Santos Fernandes <adrianosf@uol.com.br>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#ifndef DSQL_PACKAGE_NODES_H
#define DSQL_PACKAGE_NODES_H

#include "../jrd/common.h"
#include "../dsql/DdlNodes.h"
#include "../common/classes/array.h"

namespace Jrd {


class CreateAlterPackageNode : public DdlNode
{
public:
	struct Item
	{
		static Item create(CreateAlterFunctionNode* function)
		{
			Item item;
			item.type = FUNCTION;
			item.function = function;
			return item;
		}

		static Item create(CreateAlterProcedureNode* procedure)
		{
			Item item;
			item.type = PROCEDURE;
			item.procedure = procedure;
			return item;
		}

		enum
		{
			FUNCTION,
			PROCEDURE
		} type;

		union
		{
			CreateAlterFunctionNode* function;
			CreateAlterProcedureNode* procedure;
		};
	};

public:
	explicit CreateAlterPackageNode(MemoryPool& pool, const Firebird::string& sqlText,
				const Firebird::MetaName& aName)
		: DdlNode(pool, sqlText),
		  name(pool, aName),
		  create(true),
		  alter(false),
		  source(pool),
		  items(NULL),
		  functionNames(pool),
		  procedureNames(pool),
		  owner(pool)
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
					isc_dsql_create_pack_failed, isc_dsql_alter_pack_failed,
					isc_dsql_create_alter_pack_failed)) <<
				name;
	}

	virtual DdlNode* internalDsqlPass();

private:
	void executeCreate(thread_db* tdbb, jrd_tra* transaction);
	bool executeAlter(thread_db* tdbb, jrd_tra* transaction);

public:
	Firebird::MetaName name;
	bool create;
	bool alter;
	Firebird::string source;
	Firebird::Array<Item>* items;
	Firebird::SortedArray<Firebird::MetaName> functionNames;
	Firebird::SortedArray<Firebird::MetaName> procedureNames;

private:
	Firebird::string owner;
};


class DropPackageNode : public DdlNode
{
public:
	explicit DropPackageNode(MemoryPool& pool, const Firebird::string& sqlText,
				const Firebird::MetaName& aName)
		: DdlNode(pool, sqlText),
		  name(pool, aName),
		  silent(false)
	{
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, jrd_tra* transaction);

protected:
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector << Firebird::Arg::Gds(isc_dsql_drop_pack_failed) << name;
	}

public:
	Firebird::MetaName name;
	bool silent;
};


class RecreatePackageNode : public DdlNode
{
public:
	explicit RecreatePackageNode(MemoryPool& p, const Firebird::string& sqlText,
				CreateAlterPackageNode* aCreateNode)
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
		statusVector << Firebird::Arg::Gds(isc_dsql_recreate_pack_failed) << createNode->name;
	}

	virtual DdlNode* internalDsqlPass();

private:
	CreateAlterPackageNode* createNode;
	DropPackageNode dropNode;
};


class CreatePackageBodyNode : public DdlNode
{
public:
	explicit CreatePackageBodyNode(MemoryPool& pool, const Firebird::string& sqlText,
				const Firebird::MetaName& aName)
		: DdlNode(pool, sqlText),
		  name(pool, aName),
		  source(pool),
		  declaredItems(NULL),
		  items(NULL),
		  owner(pool)
	{
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, jrd_tra* transaction);

protected:
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector << Firebird::Arg::Gds(isc_dsql_create_pack_body_failed) << name;
	}

	virtual DdlNode* internalDsqlPass();

public:
	Firebird::MetaName name;
	Firebird::string source;
	Firebird::Array<CreateAlterPackageNode::Item>* declaredItems;
	Firebird::Array<CreateAlterPackageNode::Item>* items;

private:
	Firebird::string owner;
};


class DropPackageBodyNode : public DdlNode
{
public:
	explicit DropPackageBodyNode(MemoryPool& pool, const Firebird::string& sqlText,
				const Firebird::MetaName& aName)
		: DdlNode(pool, sqlText),
		  name(pool, aName)
	{
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, jrd_tra* transaction);

protected:
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector << Firebird::Arg::Gds(isc_dsql_drop_pack_body_failed) << name;
	}

public:
	Firebird::MetaName name;
};


class RecreatePackageBodyNode : public DdlNode
{
public:
	explicit RecreatePackageBodyNode(MemoryPool& p, const Firebird::string& sqlText,
				CreatePackageBodyNode* aCreateNode)
		: DdlNode(p, sqlText),
		  createNode(aCreateNode),
		  dropNode(p, sqlText, createNode->name)
	{
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void execute(thread_db* tdbb, jrd_tra* transaction);

protected:
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector)
	{
		statusVector << Firebird::Arg::Gds(isc_dsql_recreate_pack_body_failed) << createNode->name;
	}

	virtual DdlNode* internalDsqlPass();

private:
	CreatePackageBodyNode* createNode;
	DropPackageBodyNode dropNode;
};


} // namespace

#endif // DSQL_PACKAGE_NODES_H
