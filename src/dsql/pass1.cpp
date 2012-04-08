/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		pass1.cpp
 *	DESCRIPTION:	First-pass compiler for statement trees.
 *
 * The contents of this file are subject to the Interbase Public
 * License Version 1.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy
 * of the License at http://www.Inprise.com/IPL.html
 *
 * Software distributed under the License is distributed on an
 * "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
 * or implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code was created by Inprise Corporation
 * and its predecessors. Portions created by Inprise Corporation are
 * Copyright (C) Inprise Corporation.
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 *
 * 2001.5.26: Claudio Valderrama: field names should be skimmed from trailing
 *
 * 2001.5.26: Claudio Valderrama: COMPUTED fields will be skipped if a dummy
 *       "insert into tbl values(...)" sentence is issued.
 *
 * 2001.5.26: Claudio Valderrama: field names should be skimmed from trailing
 *		blanks to allow reliable comparisons in pass1_field. Same for table and
 *		and index names in plans.
 *
 * 2001.5.29: Claudio Valderrama: handle DROP VIEW case in pass1_statement().
 *
 * 2001.6.12: Claudio Valderrama: add basic BREAK capability to procedures.
 *
 * 2001.6.27: Claudio Valderrama: pass1_variable() now gives the name of the
 * variable it can't find in the error message.
 *
 * 2001.6.30: Claudio Valderrama: Enhanced again to provide (line, col), see node.h.
 *
 * 2001.7.28: John Bellardo: added code to handle nod_limit and associated fields.
 *
 * 2001.08.14 Claudio Valderrama: fixed crash with trigger and CURRENT OF <cursor> syntax.
 *
 * 2001.09.10 John Bellardo: fixed gen_rse to attribute skip/first nodes to the parent_rse
 *   if present instead of the child rse.  BUG #451798
 *
 * 2001.09.26 Claudio Valderrama: ambiguous field names are rejected from now.
 *
 * 2001.10.01 Claudio Valderrama: check constraints are allowed to have ambiguous field
 *   names because they use OLD and NEW as aliases of the same table. However, if the
 *   check constraint has an embedded ambiguous SELECT statement, it won't be detected.
 *   The code should be revisited if check constraints' before delete triggers are used
 *   for whatever reason. Currently they are never generated. The code can be improved
 *   to not report errors for fields between NEW and OLD contexts but complain otherwise.
 *
 * 2001.10.05 Neil McCalden: validate udf and parameters when comparing select list and
 *   group by list, to detect invalid SQL statements when grouping by UDFs.
 *
 * 2001.10.23 Ann Harrison:  allow reasonable checking of ambiguous names in unions.
 *   Remembering, later, that LLS_PUSH expects an object, not an LLS block.  Also
 *   stuck in the code for handling variables in pass1 - it apparently doesn't happen
 *   because the code returned an uninitialized pointer.
 *
 * 2001.11.17 Neil McCalden: Add aggregate_in_list procedure to handle cases
 *   where select statement has aggregate as a parameter to a udf which does
 *   not have to be in a group by clause.
 *
 * 2001.11.21 Claudio Valderrama: don't try to detect ambiguity in pass1_field()
 *   if the field or output procedure parameter has been fully qualified!!!
 *
 * 2001.11.27 Ann Harrison:  Redo the amiguity checking so as to give better
 *   error messages, return warnings for dialect 1, and simplify.
 *
 * 2001.11.28 Claudio Valderrama: allow udf arguments to be query parameters.
 *   Honor the code in the parser that already accepts those parameters.
 *   This closes SF Bug# 409769.
 *
 * 2001.11.29 Claudio Valderrama: make the nice new ambiguity checking code do the
 *   right thing instead of crashing the engine and restore fix from 2001.11.21.
 *
 * 2001.12.21 Claudio Valderrama: Fix SF Bug #494832 - pass1_variable() should work
 *   with def_proc, mod_proc, redef_proc, def_trig and mod_trig node types.
 *
 * 2002.07.30 Arno Brinkman: Added pass1_coalesce, pass1_simple_case, pass1_searched_case
 *   and PASS1_put_args_on_stack
 *
 * 2002.08.04 Arno Brinkman: Added ignore_cast as parameter to PASS1_node_match,
 *   Changed invalid_reference procedure for allow EXTRACT, SUBSTRING, CASE,
 *   COALESCE and NULLIF functions in GROUP BY and as select_items.
 *   Removed aggregate_in_list procedure.
 *
 * 2002.08.07 Dmitry Yemanov: Disabled BREAK statement in triggers
 *
 * 2002.08.10 Dmitry Yemanov: ALTER VIEW
 *
 * 2002.09.28 Dmitry Yemanov: Reworked internal_info stuff, enhanced
 *                            exception handling in SPs/triggers,
 *                            implemented ROWS_AFFECTED system variable
 *
 * 2002.09.29 Arno Brinkman: Adding more checking for aggregate functions
 *   and adding support for 'linking' from sub-selects to aggregate functions
 *   which are in an lower level.
 *   Modified functions pass1_field, pass1_rse, copy_field, PASS1_sort.
 *   Functions pass1_found_aggregate and pass1_found_field added.
 *
 * 2002.10.21 Nickolay Samofatov: Added support for explicit pessimistic locks
 *
 * 2002.10.25 Dmitry Yemanov: Re-allowed plans in triggers
 *
 * 2002.10.29 Nickolay Samofatov: Added support for savepoints
 *
 * 2002.12.03 Dmitry Yemanov: Implemented ORDER BY clause in subqueries
 *
 * 2002.12.18 Dmitry Yemanov: Fixed bug with BREAK and partially implemented
 *							  SQL-compliant labels and LEAVE statement
 *
 * 2003.01.11 Arno Brinkman: Reworked a lot of functions for bringing back backwards compatibilty
 *							 with sub-selects and aggregates.
 *
 * 2003.01.14 Dmitry Yemanov: Fixed bug with cursors in triggers
 *
 * 2003.01.15 Dmitry Yemanov: Added support for parametrized events
 *
 * 2003.04.05 Dmitry Yemanov: Changed logic of ORDER BY with collations
 *							  (because of the parser change)
 *
 * 2003.08.14 Arno Brinkman: Added derived table support.
 *
 * 2003.08.16 Arno Brinkman: Changed ambiguous column name checking.
 *
 * 2003.10.05 Dmitry Yemanov: Added support for explicit cursors in PSQL.
 *
 * 2004.01.16 Vlad Horsun: added support for default parameters and
 *   EXECUTE BLOCK statement
 *
 * Adriano dos Santos Fernandes
 *
 */

#include "firebird.h"
#include <string.h>
#include <stdio.h>
#include "../jrd/ibase.h"
#include "../dsql/dsql.h"
#include "../dsql/node.h"
#include "../dsql/Nodes.h"
#include "../dsql/BoolNodes.h"
#include "../dsql/ExprNodes.h"
#include "../jrd/intl.h"
#include "../jrd/blr.h"
#include "../jrd/jrd.h"
#include "../jrd/constants.h"
#include "../jrd/intl_classes.h"
#include "../jrd/RecordSourceNodes.h"
#include "../dsql/DdlNodes.h"
#include "../dsql/StmtNodes.h"
#include "../dsql/ddl_proto.h"
#include "../dsql/errd_proto.h"
#include "../dsql/gen_proto.h"
#include "../dsql/make_proto.h"
#include "../dsql/metd_proto.h"
#include "../dsql/pass1_proto.h"
#include "../dsql/utld_proto.h"
#include "../dsql/DSqlDataTypeUtil.h"
#include "../common/dsc_proto.h"
#include "../jrd/intl_proto.h"
#include "../jrd/jrd_proto.h"
#include "../jrd/thread_proto.h"
#include "../yvalve/why_proto.h"
#include "../jrd/SysFunction.h"
#include "../common/classes/array.h"
#include "../common/classes/auto.h"
#include "../common/utils_proto.h"
#include "../common/config/config.h"
#include "../common/StatusArg.h"

using namespace Jrd;
using namespace Dsql;
using namespace Firebird;

#ifdef DEV_BUILD
static void DSQL_pretty(const dsql_nod*, int);
#endif


static dsql_ctx* pass1_alias_list(DsqlCompilerScratch*, dsql_nod*);
static dsql_ctx* pass1_alias(DsqlCompilerScratch*, DsqlContextStack&, dsql_str*);
static dsql_str* pass1_alias_concat(const dsql_str*, const char*);
static dsql_rel* pass1_base_table(DsqlCompilerScratch*, const dsql_rel*, const dsql_str*);
static void pass1_expand_contexts(DsqlContextStack& contexts, dsql_ctx* context);
static dsql_nod* pass1_derived_table(DsqlCompilerScratch*, SelectExprNode*, const char*);
static dsql_nod* pass1_expand_select_list(DsqlCompilerScratch*, dsql_nod*, dsql_nod*);
static dsql_nod* pass1_group_by_list(DsqlCompilerScratch*, dsql_nod*, dsql_nod*);
static dsql_nod* pass1_make_derived_field(DsqlCompilerScratch*, thread_db*, dsql_nod*);
static RseNode* pass1_rse(DsqlCompilerScratch*, dsql_nod*, dsql_nod*, RowsClause*, bool, USHORT);
static RseNode* pass1_rse_impl(DsqlCompilerScratch*, dsql_nod*, dsql_nod*, RowsClause*, bool, USHORT);
static dsql_nod* pass1_sel_list(DsqlCompilerScratch*, dsql_nod*);
static RseNode* pass1_union(DsqlCompilerScratch*, UnionSourceNode*, dsql_nod*, RowsClause*, bool, USHORT);
static void pass1_union_auto_cast(DsqlCompilerScratch*, dsql_nod*, const dsc&, SSHORT,
	bool in_select_list = false);
static void remap_streams_to_parent_context(dsql_nod*, dsql_ctx*);


AggregateFinder::AggregateFinder(const DsqlCompilerScratch* aDsqlScratch, bool aWindow)
	: ConstDsqlNodeVisitor(false, false, &ExprNode::dsqlAggregateFinder),
	  dsqlScratch(aDsqlScratch),
	  window(aWindow),
	  currentLevel(dsqlScratch->scopeLevel),
	  deepestLevel(0),
	  ignoreSubSelects(false)
{
}

bool AggregateFinder::internalVisit(const dsql_nod* node)
{
	DEV_BLKCHK(node, dsql_type_nod);
	return node && visitChildren(node);
}


Aggregate2Finder::Aggregate2Finder(USHORT aCheckScopeLevel, FieldMatchType aMatchType, bool aWindowOnly)
	: ConstDsqlNodeVisitor(true, false, &ExprNode::dsqlAggregate2Finder),
	  checkScopeLevel(aCheckScopeLevel),
	  matchType(aMatchType),
	  windowOnly(aWindowOnly),
	  currentScopeLevelEqual(true)
{
}

bool Aggregate2Finder::internalVisit(const dsql_nod* node)
{
	DEV_BLKCHK(node, dsql_type_nod);
	return node && visitChildren(node);
}


FieldFinder::FieldFinder(USHORT aCheckScopeLevel, FieldMatchType aMatchType)
	: ConstDsqlNodeVisitor(true, false, &ExprNode::dsqlFieldFinder),
	  checkScopeLevel(aCheckScopeLevel),
	  matchType(aMatchType),
	  field(false)
{
}

bool FieldFinder::internalVisit(const dsql_nod* node)
{
	DEV_BLKCHK(node, dsql_type_nod);
	return node && visitChildren(node);
}


InvalidReferenceFinder::InvalidReferenceFinder(const dsql_ctx* aContext, const dsql_nod* aList)
	: ConstDsqlNodeVisitor(true, false, &ExprNode::dsqlInvalidReferenceFinder),
	  context(aContext),
	  list(aList),
	  insideOwnMap(false),
	  insideHigherMap(false)
{
	DEV_BLKCHK(list, dsql_type_nod);
}

bool InvalidReferenceFinder::internalVisit(const dsql_nod* node)
{
	DEV_BLKCHK(node, dsql_type_nod);

	if (!node)
		return false;

	bool invalid = false;

	// ASF: What we do in this function is the verification of all fields/dbkeys (or any parent
	// expression involving them) are present in the passed node list.
	// That makes valid:
	//   select n + 0 from table group by n			=> The n item is present in the list
	//   select n + 0 from table group by n + 0		=> The n + 0 item is present in the list
	// And makes invalid:
	//   select n + 1 from table group by n + 0		=> The n + 1 item is not present in the list

	if (list)
	{
		// Check if this node (with ignoring of CASTs) appears also
		// in the list of group by. If yes then it's allowed
		const dsql_nod* const* ptr = list->nod_arg;
		for (const dsql_nod* const* const end = ptr + list->nod_count; ptr < end; ptr++)
		{
			if (PASS1_node_match(node, *ptr, true))
				return false;
		}
	}

	switch (node->nod_type)
	{
		case nod_plan_expr:
			fb_assert(false);
			return false;

		default:
			return visitChildren(node);
	}

	return invalid;
}


FieldRemapper::FieldRemapper(DsqlCompilerScratch* aDsqlScratch, dsql_ctx* aContext, bool aWindow,
			dsql_nod* aPartitionNode, dsql_nod* aOrderNode)
	: NonConstDsqlNodeVisitor(false, false, &ExprNode::dsqlFieldRemapper),
	  dsqlScratch(aDsqlScratch),
	  context(aContext),
	  window(aWindow),
	  partitionNode(aPartitionNode),
	  orderNode(aOrderNode),
	  currentLevel(dsqlScratch->scopeLevel)
{
	DEV_BLKCHK(dsqlScratch, dsql_type_req);
	DEV_BLKCHK(context, dsql_type_ctx);
}

bool FieldRemapper::internalVisit(dsql_nod* node)
{
	DEV_BLKCHK(node, dsql_type_nod);
	return node && visitChildren(node);
}


SubSelectFinder::SubSelectFinder()
	: ConstDsqlNodeVisitor(false, true, &ExprNode::dsqlSubSelectFinder)
{
}

bool SubSelectFinder::internalVisit(const dsql_nod* node)
{
	DEV_BLKCHK(node, dsql_type_nod);

	if (!node)
		return false;

	switch (node->nod_type)
	{
		default:
			return visitChildren(node);
	}

	return false;
}


/**

 	PASS1_make_context

    @brief	Generate a context for a dsqlScratch.


    @param dsqlScratch
    @param relation_node

 **/
dsql_ctx* PASS1_make_context(DsqlCompilerScratch* dsqlScratch, const dsql_nod* relation_node)
{
	DEV_BLKCHK(dsqlScratch, dsql_type_req);
	DEV_BLKCHK(relation_node, dsql_type_nod);

	thread_db* const tdbb = JRD_get_thread_data();

	dsql_rel* relation = NULL;
	dsql_prc* procedure = NULL;

	// figure out whether this is a relation or a procedure
	// and give an error if it is neither

	MetaName relation_name;
	const ProcedureSourceNode* procNode = NULL;
	const RelationSourceNode* relNode = NULL;
	const SelectExprNode* selNode = NULL;

	if ((procNode = ExprNode::as<ProcedureSourceNode>(relation_node)))
		relation_name = procNode->dsqlName.identifier;
	else if ((relNode = ExprNode::as<RelationSourceNode>(relation_node)))
		relation_name = relNode->dsqlName;
	else if ((selNode = ExprNode::as<SelectExprNode>(relation_node)))
		relation_name = selNode->alias.c_str();

	SelectExprNode* cte = NULL;

	if (selNode)
	{
		// No processing needed here for derived tables.
	}
	else if (procNode && (procNode->dsqlName.package.hasData() || procNode->dsqlInputs))
	{
		if (procNode->dsqlName.package.isEmpty())
			procedure = dsqlScratch->getSubProcedure(procNode->dsqlName.identifier);

		if (!procedure)
		{
			procedure = METD_get_procedure(dsqlScratch->getTransaction(), dsqlScratch,
				procNode->dsqlName);
		}

		if (!procedure)
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-204) <<
					  Arg::Gds(isc_dsql_procedure_err) <<
					  Arg::Gds(isc_random) <<
					  Arg::Str(procNode->dsqlName.toString()) <<
					  Arg::Gds(isc_dsql_line_col_error) << Arg::Num(relation_node->nod_line) <<
					  									   Arg::Num(relation_node->nod_column));
		}
	}
	else if ((cte = dsqlScratch->findCTE(relation_name)))
		relation_node = MAKE_class_node(cte);
	else
	{
		if (procNode && procNode->dsqlName.package.isEmpty())
			procedure = dsqlScratch->getSubProcedure(procNode->dsqlName.identifier);

		if (!procedure)
			relation = METD_get_relation(dsqlScratch->getTransaction(), dsqlScratch, relation_name);

		if (!relation && !procedure && procNode)
		{
			procedure = METD_get_procedure(dsqlScratch->getTransaction(),
				dsqlScratch, procNode->dsqlName);
		}

		if (!relation && !procedure)
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-204) <<
					  Arg::Gds(isc_dsql_relation_err) <<
					  Arg::Gds(isc_random) << Arg::Str(relation_name) <<
					  Arg::Gds(isc_dsql_line_col_error) << Arg::Num(relation_node->nod_line) <<
					  									   Arg::Num(relation_node->nod_column));
		}
	}

	if (procedure && !procedure->prc_out_count)
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-84) <<
				  Arg::Gds(isc_dsql_procedure_use_err) << Arg::Str(procedure->prc_name.toString()) <<
				  Arg::Gds(isc_dsql_line_col_error) << Arg::Num(relation_node->nod_line) <<
				  									   Arg::Num(relation_node->nod_column));
	}

	// Set up context block.
	dsql_ctx* context = FB_NEW(*tdbb->getDefaultPool()) dsql_ctx(*tdbb->getDefaultPool());
	context->ctx_relation = relation;
	context->ctx_procedure = procedure;

	if (selNode)
		context->ctx_context = USHORT(MAX_UCHAR) + 1 + dsqlScratch->derivedContextNumber++;
	else
		context->ctx_context = dsqlScratch->contextNumber++;

	context->ctx_scope_level = dsqlScratch->scopeLevel;

	// When we're in a outer-join part mark context for it.
	if (dsqlScratch->inOuterJoin)
		context->ctx_flags |= CTX_outer_join;

	context->ctx_in_outer_join = dsqlScratch->inOuterJoin;

	// find the context alias name, if it exists.
	const char* string = NULL;

	if ((procNode = ExprNode::as<ProcedureSourceNode>(relation_node)))
		string = procNode->alias.nullStr();
	else if ((relNode = ExprNode::as<RelationSourceNode>(relation_node)))
		string = relNode->alias.nullStr();
	else if ((selNode = ExprNode::as<SelectExprNode>(relation_node)))
	{
		string = selNode->alias.nullStr();
		context->ctx_rse = selNode->querySpec;
	}

	if (string)
		context->ctx_internal_alias = string;

	if (dsqlScratch->aliasRelationPrefix && !selNode)
	{
		if (string)
			string = pass1_alias_concat(dsqlScratch->aliasRelationPrefix, string)->str_data;
		else
			string = pass1_alias_concat(dsqlScratch->aliasRelationPrefix, relation_name.c_str())->str_data;
	}

	if (string)
	{
		context->ctx_alias = string;

		// check to make sure the context is not already used at this same
		// query level (if there are no subqueries, this checks that the
		// alias is not used twice in the dsqlScratch).
		for (DsqlContextStack::iterator stack(*dsqlScratch->context); stack.hasData(); ++stack)
		{
			const dsql_ctx* conflict = stack.object();

			if (conflict->ctx_scope_level != context->ctx_scope_level)
				continue;

			const TEXT* conflict_name;
			ISC_STATUS error_code;

			if (conflict->ctx_alias.hasData())
			{
				conflict_name = conflict->ctx_alias.c_str();
				error_code = isc_alias_conflict_err;
				// alias %s conflicts with an alias in the same dsqlScratch.
			}
			else if (conflict->ctx_procedure)
			{
				conflict_name = conflict->ctx_procedure->prc_name.identifier.c_str();
				error_code = isc_procedure_conflict_error;
				// alias %s conflicts with a procedure in the same dsqlScratch.
			}
			else if (conflict->ctx_relation)
			{
				conflict_name = conflict->ctx_relation->rel_name.c_str();
				error_code = isc_relation_conflict_err;
				// alias %s conflicts with a relation in the same dsqlScratch.
			}
			else
				continue;

			if (context->ctx_alias == conflict_name)
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-204) <<
						  Arg::Gds(error_code) << Arg::Str(conflict_name));
			}
		}
	}

	if (procedure)
	{
		USHORT count = 0;

		if (procNode->dsqlInputs)
		{
			context->ctx_proc_inputs = PASS1_node_psql(dsqlScratch, procNode->dsqlInputs, false);
			count = context->ctx_proc_inputs->nod_count;
		}

		if (count > procedure->prc_in_count ||
			count < procedure->prc_in_count - procedure->prc_def_count)
		{
			ERRD_post(Arg::Gds(isc_prcmismat) << Arg::Str(procNode->dsqlName.toString()));
		}

		if (count)
		{
			// Initialize this stack variable, and make it look like a node
			AutoPtr<dsql_nod> desc_node(FB_NEW_RPT(*tdbb->getDefaultPool(), 0) dsql_nod);
			dsql_nod* const* input = context->ctx_proc_inputs->nod_arg;

			for (dsql_fld* field = procedure->prc_inputs; *input; ++input, field = field->fld_next)
			{
				DEV_BLKCHK(field, dsql_type_fld);
				DEV_BLKCHK(*input, dsql_type_nod);
				MAKE_desc_from_field(&desc_node->nod_desc, field);
				PASS1_set_parameter_type(dsqlScratch, *input, desc_node, false);
			}
		}
	}

	// push the context onto the dsqlScratch context stack
	// for matching fields against

	dsqlScratch->context->push(context);

	return context;
}


/**

 	PASS1_node

    @brief	Compile a parsed statement into something more interesting.


    @param dsqlScratch
    @param input

 **/
dsql_nod* PASS1_node(DsqlCompilerScratch* dsqlScratch, dsql_nod* input)
{
	thread_db* tdbb = JRD_get_thread_data();

	DEV_BLKCHK(dsqlScratch, dsql_type_req);
	DEV_BLKCHK(input, dsql_type_nod);

	if (!input)
		return NULL;

	dsql_nod* node;
	dsql_nod* sub1;

	// Dispatch on node type.  Fall thru on easy ones

	switch (input->nod_type)
	{
	case nod_class_exprnode:
		{
			ExprNode* exprNode = reinterpret_cast<ExprNode*>(input->nod_arg[0]);
			ProcedureSourceNode* procNode = NULL;
			RelationSourceNode* relNode = NULL;
			SelectExprNode* selNode = NULL;

			//// TODO: Move this to dsqlPass when possible.
			if ((procNode = exprNode->as<ProcedureSourceNode>()) ||
				(relNode = exprNode->as<RelationSourceNode>()))
			{
				bool couldBeCte = true;
				MetaName rel_name;
				string rel_alias;

				if (procNode)
				{
					rel_name = procNode->dsqlName.identifier;
					rel_alias = procNode->alias;
					couldBeCte = !procNode->dsqlInputs && procNode->dsqlName.package.isEmpty();
				}
				else if (relNode)
				{
					rel_name = relNode->dsqlName;
					rel_alias = relNode->alias;
				}

				if (rel_alias.isEmpty())
					rel_alias = rel_name.c_str();

				SelectExprNode* cte = couldBeCte ? dsqlScratch->findCTE(rel_name) : NULL;

				if (!cte)
					return PASS1_relation(dsqlScratch, input);

				cte->dsqlFlags |= RecordSourceNode::DFLAG_DT_CTE_USED;

				if ((dsqlScratch->flags & DsqlCompilerScratch::FLAG_RECURSIVE_CTE) &&
					 dsqlScratch->currCtes.hasData() &&
					 (dsqlScratch->currCtes.object() == cte))
				{
					ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
							  // Recursive CTE member (%s) can refer itself only in FROM clause
							  Arg::Gds(isc_dsql_cte_wrong_reference) << rel_name);
				}

				for (Stack<SelectExprNode*>::const_iterator stack(dsqlScratch->currCtes); stack.hasData(); ++stack)
				{
					SelectExprNode* cte1 = stack.object();
					if (cte1 == cte)
					{
						ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
								  // CTE %s has cyclic dependencies
								  Arg::Gds(isc_dsql_cte_cycle) << rel_name);
					}
				}

				dsql_nod* const query = cte->querySpec;
				UnionSourceNode* unionQuery = ExprNode::as<UnionSourceNode>(query);
				const bool isRecursive = unionQuery && unionQuery->recursive;

				const string saveCteName = cte->alias;
				if (!isRecursive)
					cte->alias = rel_alias;

				dsqlScratch->currCtes.push(cte);

				dsql_nod* derived_node = pass1_derived_table(dsqlScratch,
					cte, (isRecursive ? rel_alias.c_str() : NULL));

				if (!isRecursive)
					cte->alias = saveCteName;

				dsqlScratch->currCtes.pop();

				return derived_node;
			}
			else if ((selNode = exprNode->as<SelectExprNode>()))
			{
				fb_assert(selNode->dsqlFlags & RecordSourceNode::DFLAG_DERIVED);
				return pass1_derived_table(dsqlScratch, selNode, NULL);
			}

			Node* newNode = exprNode->dsqlPass(dsqlScratch);
			if (newNode != (Node*) input->nod_arg[0])
			{
				input = MAKE_node(input->nod_type, input->nod_count);
				input->nod_arg[0] = (dsql_nod*) newNode;
			}

			input->nod_line = newNode->line;
			input->nod_column = newNode->column;
		}
		return input;

	case nod_delete:
	case nod_select:
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
				  Arg::Gds(isc_dsql_command_err));

	// access plan node types

	case nod_plan_item:
		{
			dsql_ctx* context = pass1_alias_list(dsqlScratch, input->nod_arg[0]);
			RecordSourceNode* recSource = NULL;

			if (context->ctx_relation)
			{
				RelationSourceNode* relNode = FB_NEW(*tdbb->getDefaultPool()) RelationSourceNode(
					*tdbb->getDefaultPool());
				relNode->dsqlContext = context;
				recSource = relNode;
			}
			else if (context->ctx_procedure)
			{
				// ASF: Note that usage of procedure name in a PLAN clause causes errors when
				// parsing the BLR.
				ProcedureSourceNode* procNode = FB_NEW(*tdbb->getDefaultPool()) ProcedureSourceNode(
					*tdbb->getDefaultPool());
				procNode->dsqlContext = context;
				recSource = procNode;
			}

			fb_assert(recSource);

			node = MAKE_node(input->nod_type, 2);
			node->nod_arg[0] = sub1 = MAKE_node(nod_class_exprnode, 1);
			sub1->nod_arg[0] = reinterpret_cast<dsql_nod*>(recSource);
			node->nod_arg[1] = PASS1_node(dsqlScratch, input->nod_arg[1]);

			return node;
		}

	case nod_index:
		node = MAKE_node(input->nod_type, 1);
		node->nod_arg[0] = input->nod_arg[0];
		return node;

	case nod_index_order:
		node = MAKE_node(input->nod_type, 2);
		node->nod_arg[0] = input->nod_arg[0];
		node->nod_arg[1] = input->nod_arg[1];
		return node;

	default:
		break;
	}

	// Node is simply to be rebuilt -- just recurse merrily

	node = MAKE_node(input->nod_type, input->nod_count);
	const dsql_nod** ptr2 = const_cast<const dsql_nod**>(node->nod_arg);

	dsql_nod** ptr = input->nod_arg;
	for (const dsql_nod* const* const end = ptr + input->nod_count; ptr < end; ptr++)
	{
		DEV_BLKCHK(*ptr, dsql_type_nod);
		*ptr2++ = PASS1_node(dsqlScratch, *ptr);
		DEV_BLKCHK(*(ptr2 - 1), dsql_type_nod);
	}

	return node;
}


// Compile a record selection expression, bumping up the statement scope level everytime an rse is
// seen. The scope level controls parsing of aliases.
dsql_nod* PASS1_rse(DsqlCompilerScratch* dsqlScratch, dsql_nod* input, bool updateLock)
{
	DEV_BLKCHK(dsqlScratch, dsql_type_req);
	DEV_BLKCHK(input, dsql_type_nod);

	fb_assert(ExprNode::as<SelectExprNode>(input));

	dsqlScratch->scopeLevel++;
	dsql_nod* node = MAKE_class_node(pass1_rse(dsqlScratch, input, NULL, NULL, updateLock, 0));
	dsqlScratch->scopeLevel--;

	return node;
}


// Check for ambiguity in a field reference. The list with contexts where the field was found is
// checked and the necessary message is build from it.
void PASS1_ambiguity_check(DsqlCompilerScratch* dsqlScratch,
	const MetaName& name, const DsqlContextStack& ambiguous_contexts)
{
	// If there are no relations or only 1 there's no ambiguity, thus return.
	if (ambiguous_contexts.getCount() < 2)
		return;

	TEXT buffer[1024];
	USHORT loop = 0;

	buffer[0] = 0;
	TEXT* b = buffer;
	TEXT* p = NULL;

	for (DsqlContextStack::const_iterator stack(ambiguous_contexts); stack.hasData(); ++stack)
	{
		const dsql_ctx* context = stack.object();
		const dsql_rel* relation = context->ctx_relation;
		const dsql_prc* procedure = context->ctx_procedure;
		if (strlen(b) > (sizeof(buffer) - 50))
		{
			// Buffer full
			break;
		}
		// if this is the second loop add "and " before relation.
		if (++loop > 2)
			strcat(buffer, "and ");
		// Process relation when present.
		if (relation)
		{
			if (!(relation->rel_flags & REL_view))
				strcat(buffer, "table ");
			else
				strcat(buffer, "view ");
			strcat(buffer, relation->rel_name.c_str());
		}
		else if (procedure)
		{
			// Process procedure when present.
			strcat(b, "procedure ");
			strcat(b, procedure->prc_name.toString().c_str());
		}
		else
		{
			// When there's no relation and no procedure it's a derived table.
			strcat(b, "derived table ");
			if (context->ctx_alias.hasData())
				strcat(b, context->ctx_alias.c_str());
		}
		strcat(buffer, " ");
		if (!p)
			p = b + strlen(b);
	}

	if (p)
		*--p = 0;

	if (dsqlScratch->clientDialect >= SQL_DIALECT_V6)
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-204) <<
				  Arg::Gds(isc_dsql_ambiguous_field_name) << Arg::Str(buffer) << Arg::Str(++p) <<
				  Arg::Gds(isc_random) << name);
	}

	ERRD_post_warning(Arg::Warning(isc_sqlwarn) << Arg::Num(204) <<
					  Arg::Warning(isc_dsql_ambiguous_field_name) << Arg::Str(buffer) <<
																	 Arg::Str(++p) <<
					  Arg::Warning(isc_random) << name);
}


/**
	PASS1_check_unique_fields_names

	check fields (params, variables, cursors etc) names against
	sorted array
	if success, add them into array
 **/
void PASS1_check_unique_fields_names(StrArray& names, const CompoundStmtNode* fields)
{
	if (!fields)
		return;

	const NestConst<StmtNode>* ptr = fields->statements.begin();
	const NestConst<StmtNode>* const end = fields->statements.end();

	for (; ptr != end; ++ptr)
	{
		const char* name = NULL;

		const DeclareVariableNode* varNode;
		const DeclareCursorNode* cursorNode;

		if ((varNode = (*ptr)->as<DeclareVariableNode>()))
			name = varNode->dsqlDef->name.c_str();
		else if ((cursorNode = (*ptr)->as<DeclareCursorNode>()))
			name = cursorNode->dsqlName.c_str();
		else if ((*ptr)->as<DeclareSubProcNode>() || (*ptr)->as<DeclareSubFuncNode>())
			continue;

		fb_assert(name);

		size_t pos;
		if (!names.find(name, pos))
			names.insert(pos, name);
		else
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-637) <<
					  Arg::Gds(isc_dsql_duplicate_spec) << Arg::Str(name));
		}
	}
}


// Compose two booleans.
dsql_nod* PASS1_compose(dsql_nod* expr1, dsql_nod* expr2, UCHAR blrOp)
{
	thread_db* tdbb = JRD_get_thread_data();

	DEV_BLKCHK(expr1, dsql_type_nod);
	DEV_BLKCHK(expr2, dsql_type_nod);

	fb_assert(blrOp == blr_and || blrOp == blr_or);

	if (!expr1)
		return expr2;

	if (!expr2)
		return expr1;

	BinaryBoolNode* binNode = FB_NEW(*tdbb->getDefaultPool()) BinaryBoolNode(
		*tdbb->getDefaultPool(), blrOp, expr1, expr2);

	dsql_nod* node = MAKE_node(nod_class_exprnode, 1);
	node->nod_arg[0] = reinterpret_cast<dsql_nod*>(binNode);

	return node;
}


// Report a field parsing recognition error.
void PASS1_field_unknown(const TEXT* qualifier_name, const TEXT* field_name,
	const ExprNode* flawed_node)
{
	TEXT field_buffer[MAX_SQL_IDENTIFIER_SIZE * 2];

	if (qualifier_name)
	{
		sprintf(field_buffer, "%.*s.%.*s", (int) MAX_SQL_IDENTIFIER_LEN, qualifier_name,
				(int) MAX_SQL_IDENTIFIER_LEN, field_name ? field_name : "*");
		field_name = field_buffer;
	}

	if (flawed_node)
	{
		if (field_name)
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-206) <<
					  Arg::Gds(isc_dsql_field_err) <<
					  Arg::Gds(isc_random) << Arg::Str(field_name) <<
					  Arg::Gds(isc_dsql_line_col_error) << Arg::Num(flawed_node->line) <<
					  									   Arg::Num(flawed_node->column));
		}
		else
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-206) <<
					  Arg::Gds(isc_dsql_field_err) <<
					  Arg::Gds(isc_dsql_line_col_error) << Arg::Num(flawed_node->line) <<
					  									   Arg::Num(flawed_node->column));
		}
	}
	else
	{
		if (field_name)
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-206) <<
					  Arg::Gds(isc_dsql_field_err) <<
					  Arg::Gds(isc_random) << Arg::Str(field_name) <<
					  Arg::Gds(isc_dsql_unknown_pos));
		}
		else
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-206) <<
					  Arg::Gds(isc_dsql_field_err) <<
					  Arg::Gds(isc_dsql_unknown_pos));
		}
	}
}




/**

	PASS1_node_match

    @brief	Compare two nodes for equality of value.

   [2002-08-04]--- Arno Brinkman
 	If ignore_map_cast is true and the node1 is
   type nod_cast or nod_map then PASS1_node_match is
   calling itselfs again with the node1
   CASTs source or map->node.
   This is for allow CAST to other datatypes
   without complaining that it's an unknown
   column reference. (Aggregate functions)


    @param node1
    @param node2
    @param ignore_map_cast

 **/
bool PASS1_node_match(const dsql_nod* node1, const dsql_nod* node2, bool ignore_map_cast)
{
	DEV_BLKCHK(node1, dsql_type_nod);
	DEV_BLKCHK(node2, dsql_type_nod);

	if (!node1 && !node2)
		return true;

	if (!node1 || !node2)
		return false;

	const CastNode* castNode1 = ExprNode::as<CastNode>(node1);

	if (ignore_map_cast && castNode1)
	{
		const CastNode* castNode2 = ExprNode::as<CastNode>(node2);

		// If node2 is also cast and same type continue with both sources.
		if (castNode2 &&
			castNode1->castDesc.dsc_dtype == castNode2->castDesc.dsc_dtype &&
			castNode1->castDesc.dsc_scale == castNode2->castDesc.dsc_scale &&
			castNode1->castDesc.dsc_length == castNode2->castDesc.dsc_length &&
			castNode1->castDesc.dsc_sub_type == castNode2->castDesc.dsc_sub_type)
		{
			return PASS1_node_match(castNode1->dsqlSource, castNode2->dsqlSource, ignore_map_cast);
		}

		return PASS1_node_match(castNode1->dsqlSource, node2, ignore_map_cast);
	}

	const DsqlMapNode* mapNode1 = ExprNode::as<DsqlMapNode>(node1);

	if (ignore_map_cast && mapNode1)
	{
		const DsqlMapNode* mapNode2 = ExprNode::as<DsqlMapNode>(node2);

		if (mapNode2)
		{
			if (mapNode1->context != mapNode2->context)
				return false;

			return PASS1_node_match(mapNode1->map->map_node, mapNode2->map->map_node, ignore_map_cast);
		}

		return PASS1_node_match(mapNode1->map->map_node, node2, ignore_map_cast);
	}

	const DsqlAliasNode* aliasNode1 = ExprNode::as<DsqlAliasNode>(node1);
	const DsqlAliasNode* aliasNode2 = ExprNode::as<DsqlAliasNode>(node2);

	// We don't care about the alias itself but only about its field.
	if (aliasNode1 || aliasNode2)
	{
		if (aliasNode1 && aliasNode2)
			return PASS1_node_match(aliasNode1->value, aliasNode2->value, ignore_map_cast);

		if (aliasNode1)
			return PASS1_node_match(aliasNode1->value, node2, ignore_map_cast);

		if (aliasNode2)
			return PASS1_node_match(node1, aliasNode2->value, ignore_map_cast);
	}

	// Handle derived fields.

	const DerivedFieldNode* derivedField1 = ExprNode::as<DerivedFieldNode>(node1);
	const DerivedFieldNode* derivedField2 = ExprNode::as<DerivedFieldNode>(node2);

	if (derivedField1 || derivedField2)
	{
		if (derivedField1 && derivedField2)
		{
			if (derivedField1->context->ctx_context != derivedField2->context->ctx_context ||
				derivedField1->name != derivedField2->name)
			{
				return false;
			}

			return PASS1_node_match(derivedField1->dsqlValue, derivedField2->dsqlValue,
				ignore_map_cast);
		}

		if (derivedField1)
			return PASS1_node_match(derivedField1->dsqlValue, node2, ignore_map_cast);

		if (derivedField2)
			return PASS1_node_match(node1, derivedField2->dsqlValue, ignore_map_cast);
	}

	if (node1->nod_type != node2->nod_type || node1->nod_count != node2->nod_count)
		return false;

	// This is to get rid of assertion failures when trying
	// to PASS1_node_match nod_aggregate's children. This was happening because not
	// all of the children are of type "dsql_nod". Pointer to the first child
	// (argument) is actually a pointer to context structure.
	// To compare two nodes of type nod_aggregate we need first to see if they
	// both refer to same context structure. If they do not they are different
	// nodes, if they point to the same context they are the same (because
	// nod_aggregate is created for an rse that have aggregate expression,
	// group by or having clause and each rse has its own context). But just in
	// case we compare two other subtrees.

	switch (node1->nod_type)
	{
		case nod_class_exprnode:
		{
			ExprNode* expr1Node = reinterpret_cast<ExprNode*>(node1->nod_arg[0]);
			ExprNode* expr2Node = reinterpret_cast<ExprNode*>(node2->nod_arg[0]);

			if (expr1Node->type != expr2Node->type)
				return false;

			return expr1Node->dsqlMatch(expr2Node, ignore_map_cast);
		}
	} // switch

	const dsql_nod* const* ptr1 = node1->nod_arg;
	const dsql_nod* const* ptr2 = node2->nod_arg;

	for (const dsql_nod* const* const end = ptr1 + node1->nod_count; ptr1 < end; ptr1++, ptr2++)
	{
		if (!PASS1_node_match(*ptr1, *ptr2, ignore_map_cast))
			return false;
	}

	return true;
}


/**

 	PASS1_cursor_name

    @brief	Find a cursor.


    @param dsqlScratch
    @param name
	@param mask
	@param existence_flag

 **/
DeclareCursorNode* PASS1_cursor_name(DsqlCompilerScratch* dsqlScratch, const MetaName& name,
	USHORT mask, bool existence_flag)
{
	DeclareCursorNode* cursor = NULL;

	if (name.isEmpty())
	{
		if (existence_flag)
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-504) <<
					  Arg::Gds(isc_dsql_cursor_err) <<
					  Arg::Gds(isc_dsql_cursor_invalid));
		}
		else
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-502) <<
					  Arg::Gds(isc_dsql_decl_err) <<
					  Arg::Gds(isc_dsql_cursor_invalid));
		}
	}

	for (Array<DeclareCursorNode*>::iterator itr = dsqlScratch->cursors.begin();
		 itr != dsqlScratch->cursors.end();
		 ++itr)
	{
		cursor = *itr;
		if (cursor->dsqlName == name && (cursor->dsqlCursorType & mask))
			break;
		cursor = NULL;
	}

	if (!cursor && existence_flag)
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-504) <<
				  Arg::Gds(isc_dsql_cursor_err) <<
				  Arg::Gds(isc_dsql_cursor_not_found) << name);
	}
	else if (cursor && !existence_flag)
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-502) <<
				  Arg::Gds(isc_dsql_decl_err) <<
				  Arg::Gds(isc_dsql_cursor_exists) << name);
	}

	return cursor;
}


// Extract relation and procedure context and expand derived child contexts.
static void pass1_expand_contexts(DsqlContextStack& contexts, dsql_ctx* context)
{
	if (context->ctx_relation || context->ctx_procedure || context->ctx_map)
	{
		if (context->ctx_parent)
			context = context->ctx_parent;

		contexts.push(context);
	}
	else
	{
		for (DsqlContextStack::iterator i(context->ctx_childs_derived_table); i.hasData(); ++i)
			pass1_expand_contexts(contexts, i.object());
	}
}


/**

 	pass1_derived_table

    @brief	Process derived table which is part of a from clause.


    @param dsqlScratch
    @param input

 **/
static dsql_nod* pass1_derived_table(DsqlCompilerScratch* dsqlScratch, SelectExprNode* input,
	const char* cte_alias)
{
	DEV_BLKCHK(dsqlScratch, dsql_type_req);

	thread_db* tdbb = JRD_get_thread_data();
	MemoryPool& pool = *tdbb->getDefaultPool();

	const string& alias = input->alias;
	dsql_nod* inputNod = MAKE_class_node(input);

	// Create the context now, because we need to know it for the tables inside.
	dsql_ctx* const context = PASS1_make_context(dsqlScratch, inputNod);

	// Save some values to restore after rse process.
	DsqlContextStack* const req_base = dsqlScratch->context;
	dsql_str* const aliasRelationPrefix = dsqlScratch->aliasRelationPrefix;

	// Change context, because when we are processing the derived table rse
	// it may not reference to other streams in the same scope_level.
	DsqlContextStack temp;
	// Put special contexts (NEW/OLD) also on the stack
	for (DsqlContextStack::iterator stack(*dsqlScratch->context); stack.hasData(); ++stack)
	{
		dsql_ctx* local_context = stack.object();
		if ((local_context->ctx_scope_level < dsqlScratch->scopeLevel) ||
			(local_context->ctx_flags & CTX_system))
		{
			temp.push(local_context);
		}
	}

	dsql_ctx* baseContext = NULL;

	if (temp.hasData())
		baseContext = temp.object();

	dsqlScratch->context = &temp;
	dsqlScratch->aliasRelationPrefix = pass1_alias_concat(aliasRelationPrefix, alias.nullStr());

	dsql_nod* query = input->querySpec;
	UnionSourceNode* unionQuery = ExprNode::as<UnionSourceNode>(query);
	dsql_nod* rse = NULL;
	const bool isRecursive = unionQuery && unionQuery->recursive;
	USHORT recursive_map_ctx = 0;

	if (isRecursive)
	{
		// Create dummy, non-recursive select dsqlScratch by doing a union of
		// one, non-recursive member. The dummy will be replaced at the end
		// of this function.

		unionQuery->dsqlClauses->nod_count = 1;
		unionQuery->recursive = false;

		dsql_ctx* baseUnionCtx = dsqlScratch->unionContext.hasData() ?
			dsqlScratch->unionContext.object() : NULL;

		// reserve extra context number for map's secondary context
		recursive_map_ctx = dsqlScratch->contextNumber++;

		dsqlScratch->recursiveCtxId = dsqlScratch->contextNumber;
		rse = MAKE_class_node(pass1_union(dsqlScratch, unionQuery, NULL, NULL, NULL, 0));
		dsqlScratch->contextNumber = dsqlScratch->recursiveCtxId + 1;

		// recursive union always have exactly 2 members
		unionQuery->dsqlClauses->nod_count = 2;
		unionQuery->recursive = true;

		while (dsqlScratch->unionContext.hasData() &&
			   dsqlScratch->unionContext.object() != baseUnionCtx)
		{
			dsqlScratch->unionContext.pop();
		}
	}
	else
	{
		// AB: 2005-01-06
		// If our derived table contains a single query with a sub-select buried
		// inside the select items then we need a special handling, because we don't
		// want creating a new sub-select for every reference outside the derived
		// table to that sub-select.
		// To handle this we simple create a UNION ALL with derived table inside it.
		// Due this mappings are created and we simple reference to these mappings.
		// Optimizer effects:
		//   Good thing is that only 1 recordstream is made for the sub-select, but
		//   the worse thing is that a UNION currently can't be used in
		//   deciding the JOIN order.
		bool foundSubSelect = false;
		RseNode* queryNode = ExprNode::as<RseNode>(query);
		if (queryNode)
			foundSubSelect = SubSelectFinder::find(queryNode->dsqlSelectList);

		if (foundSubSelect)
		{
			UnionSourceNode* unionExpr = FB_NEW(pool) UnionSourceNode(pool);
			unionExpr->dsqlClauses = MAKE_node(nod_list, 1);
			unionExpr->dsqlClauses->nod_arg[0] = inputNod;
			unionExpr->dsqlAll = true;
			rse = MAKE_class_node(pass1_union(dsqlScratch, unionExpr, NULL, NULL, NULL, 0));
		}
		else
			rse = PASS1_rse(dsqlScratch, inputNod, false);

		USHORT minOuterJoin = MAX_USHORT;

		// Finish off by cleaning up contexts and put them into derivedContext
		// so create view (ddl) can deal with it.
		// Also add the used contexts into the childs stack.
		while (temp.hasData() && (temp.object() != baseContext))
		{
			dsql_ctx* childCtx = temp.pop();

			dsqlScratch->derivedContext.push(childCtx);
			context->ctx_childs_derived_table.push(childCtx);

			// Collect contexts that will be used for blr_derived_expr generation.
			// We want all child contexts with minimum ctx_in_outer_join.
			if (childCtx->ctx_in_outer_join <= minOuterJoin)
			{
				DsqlContextStack contexts;
				pass1_expand_contexts(contexts, childCtx);

				for (DsqlContextStack::iterator i(contexts); i.hasData(); ++i)
				{
					if (i.object()->ctx_in_outer_join < minOuterJoin)
					{
						minOuterJoin = i.object()->ctx_in_outer_join;
						context->ctx_main_derived_contexts.clear();
					}

					context->ctx_main_derived_contexts.push(i.object());
				}
			}
		}

		while (temp.hasData())
			temp.pop();
	}

	context->ctx_rse = rse;

	// CVC: prepare a truncated alias for the derived table here
	// because we need it several times.
	TEXT aliasbuffer[100] = "";
	const TEXT* aliasname = aliasbuffer;
	if (alias.hasData())
	{
		int length = alias.length();
		if (length > 99)
		{
			length = 99;
			memcpy(aliasbuffer, alias.c_str(), length);
			aliasbuffer[length] = 0;
		}
		else
			aliasname = alias.c_str();
	}
	else
		aliasname = "<unnamed>";

	dsql_nod* selectList = ExprNode::as<RseNode>(rse)->dsqlSelectList;

	// If an alias-list is specified, process it.

	const bool ignoreColumnChecks =
		(input->dsqlFlags & RecordSourceNode::DFLAG_DT_IGNORE_COLUMN_CHECK);

	if (input->columns && input->columns->nod_count)
	{
		dsql_nod* list = input->columns;

		// Do both lists have the same number of items?
		if (list->nod_count != selectList->nod_count)
		{
			// Column list by derived table %s [alias-name] has %s [more/fewer] columns
			// than the number of items.

			int errcode = isc_dsql_derived_table_less_columns;
			if (list->nod_count > selectList->nod_count)
				errcode = isc_dsql_derived_table_more_columns;

			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
					  Arg::Gds(isc_dsql_command_err) <<
					  Arg::Gds(errcode) << Arg::Str(aliasname));
		}

		// Generate derived fields and assign alias-name to them.
		for (int count = 0; count < list->nod_count; count++)
		{
			dsql_nod* select_item = selectList->nod_arg[count];
			MAKE_desc(dsqlScratch, &select_item->nod_desc, select_item);

			// Make new derived field node.

			DerivedFieldNode* derivedField = FB_NEW(pool) DerivedFieldNode(pool,
				((dsql_str*) list->nod_arg[count])->str_data, dsqlScratch->scopeLevel, select_item);

			dsql_nod* nod = selectList->nod_arg[count] = MAKE_node(nod_class_exprnode, 1);
			nod->nod_arg[0] = reinterpret_cast<dsql_nod*>(derivedField);
			nod->nod_desc = select_item->nod_desc;
		}
	}
	else
	{
		// For those select-items where no alias is specified try
		// to generate one from the field_name.
		for (int count = 0; count < selectList->nod_count; count++)
		{
			dsql_nod* select_item = pass1_make_derived_field(dsqlScratch, tdbb,
				selectList->nod_arg[count]);

			// Auto-create dummy column name for pass1_any()
			if (ignoreColumnChecks && !ExprNode::is<DerivedFieldNode>(select_item))
			{
				MAKE_desc(dsqlScratch, &select_item->nod_desc, select_item);

				// Construct dummy fieldname
				char fieldname[25];
				sprintf(fieldname, "f%d", count);

				// Make new derived field node.

				DerivedFieldNode* derivedField = FB_NEW(pool) DerivedFieldNode(pool,
					fieldname, dsqlScratch->scopeLevel, select_item);

				dsql_nod* nod = MAKE_node(nod_class_exprnode, 1);
				nod->nod_arg[0] = reinterpret_cast<dsql_nod*>(derivedField);
				nod->nod_desc = select_item->nod_desc;

				select_item = nod;
			}

			selectList->nod_arg[count] = select_item;
		}
	}

	int count;

	// Check if all root select-items have a derived field else show a message.
	for (count = 0; count < selectList->nod_count; count++)
	{
		dsql_nod* select_item = selectList->nod_arg[count];
		DerivedFieldNode* derivedField;

		if ((derivedField = ExprNode::as<DerivedFieldNode>(select_item)))
			derivedField->context = context;
		else
		{
			// no column name specified for column number %d in derived table %s

			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
					  Arg::Gds(isc_dsql_command_err) <<
					  Arg::Gds(isc_dsql_derived_field_unnamed) << Arg::Num(count + 1) <<
																  Arg::Str(aliasname));
		}
	}

	// Check for ambiguous column names inside this derived table.
	for (count = 0; count < selectList->nod_count; ++count)
	{
		const DerivedFieldNode* selectItem1 =
			ExprNode::as<DerivedFieldNode>(selectList->nod_arg[count]);

		for (int count2 = (count + 1); count2 < selectList->nod_count; ++count2)
		{
			const DerivedFieldNode* selectItem2 =
				ExprNode::as<DerivedFieldNode>(selectList->nod_arg[count2]);

			if (selectItem1->name == selectItem2->name)
			{
				// column %s was specified multiple times for derived table %s
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						  Arg::Gds(isc_dsql_command_err) <<
						  Arg::Gds(isc_dsql_derived_field_dup_name) << selectItem1->name <<
								aliasname);
			}
		}
	}

	// If we used a dummy rse before, replace it with the real one now.
	// We cannot do this earlier, because recursive processing needs a fully
	// developed context block.
	if (isRecursive)
	{
		dsqlScratch->recursiveCtx = context;
		dsqlScratch->context = &temp;

		dsqlScratch->resetCTEAlias(alias.c_str());

		rse = PASS1_rse(dsqlScratch, inputNod, false);

		// Finish off by cleaning up contexts and put them into derivedContext
		// so create view (ddl) can deal with it.
		// Also add the used contexts into the childs stack.
		while (temp.hasData() && (temp.object() != baseContext))
		{
			dsqlScratch->derivedContext.push(temp.object());
			context->ctx_childs_derived_table.push(temp.pop());
		}

		temp.clear();

		ExprNode::as<RseNode>(rse)->dsqlSelectList = ExprNode::as<RseNode>(context->ctx_rse)->dsqlSelectList;

		context->ctx_rse = rse;

		if (cte_alias)
			context->ctx_alias = cte_alias;

		dsqlScratch->context = req_base;

		// Mark union's map context as recursive and assign secondary context number to it.
		dsql_nod* items = ExprNode::as<RseNode>(rse)->dsqlSelectList;
		dsql_nod* map_item = items->nod_arg[0];
		DerivedFieldNode* derivedField;

		if ((derivedField = ExprNode::as<DerivedFieldNode>(map_item)))
			map_item = derivedField->dsqlValue;

		dsql_ctx* map_context = ExprNode::as<DsqlMapNode>(map_item)->context;

		map_context->ctx_flags |= CTX_recursive;
		map_context->ctx_recursive = recursive_map_ctx;
	}

	delete dsqlScratch->aliasRelationPrefix;
	// Restore our original values.
	dsqlScratch->context = req_base;
	dsqlScratch->aliasRelationPrefix = aliasRelationPrefix;

	ExprNode::as<RseNode>(rse)->dsqlContext = context;

	return rse;
}


/**

 	pass1_expand_select_list

    @brief	Expand asterisk nodes into fields.


    @param dsqlScratch
    @param list
    @param streams

 **/
static dsql_nod* pass1_expand_select_list(DsqlCompilerScratch* dsqlScratch, dsql_nod* list,
	dsql_nod* streams)
{
	if (!list)
		list = streams;

	DsqlNodStack stack;
	dsql_nod** ptr = list->nod_arg;

	for (const dsql_nod* const* const end = ptr + list->nod_count; ptr < end; ptr++)
		PASS1_expand_select_node(dsqlScratch, *ptr, stack, true);

	return MAKE_list(stack);
}

// Expand a select item node.
void PASS1_expand_select_node(DsqlCompilerScratch* dsqlScratch, dsql_nod* node, DsqlNodStack& stack,
	bool hide_using)
{
	DEV_BLKCHK(node, dsql_type_nod);

	RseNode* rseNode;
	ProcedureSourceNode* procNode;
	RelationSourceNode* relNode;
	FieldNode* fieldNode;

	if ((rseNode = ExprNode::as<RseNode>(node)))
	{
		dsql_nod* sub_items = rseNode->dsqlSelectList;

		if (sub_items)	// AB: Derived table support
		{
			dsql_nod** ptr = sub_items->nod_arg;

			for (const dsql_nod* const* const end = ptr + sub_items->nod_count; ptr != end; ++ptr)
			{
				// Create a new alias else mappings would be mangled.
				dsql_nod* select_item = *ptr;

				// select-item should always be a derived field!

				DerivedFieldNode* derivedField;

				if (!(derivedField = ExprNode::as<DerivedFieldNode>(select_item)))
				{
					// Internal dsql error: alias type expected by PASS1_expand_select_node
					ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
							  Arg::Gds(isc_dsql_command_err) <<
							  Arg::Gds(isc_dsql_derived_alias_select));
				}

				dsql_ctx* context = derivedField->context;
				DEV_BLKCHK(context, dsql_type_ctx);

				if (!hide_using || context->getImplicitJoinField(derivedField->name, select_item))
					stack.push(select_item);
			}
		}
		else	// joins
		{
			dsql_nod** ptr = rseNode->dsqlStreams->nod_arg;

			for (const dsql_nod* const* const end = ptr + rseNode->dsqlStreams->nod_count;
				 ptr != end;
				 ++ptr)
			{
				PASS1_expand_select_node(dsqlScratch, *ptr, stack, true);
			}
		}
	}
	else if ((procNode = ExprNode::as<ProcedureSourceNode>(node)))
	{
		dsql_ctx* context = procNode->dsqlContext;

		if (context->ctx_procedure)
		{
			for (dsql_fld* field = context->ctx_procedure->prc_outputs; field; field = field->fld_next)
			{
				DEV_BLKCHK(field, dsql_type_fld);

				dsql_nod* select_item = NULL;
				if (!hide_using || context->getImplicitJoinField(field->fld_name, select_item))
				{
					if (!select_item)
						select_item = MAKE_field(context, field, 0);
					stack.push(select_item);
				}
			}
		}
	}
	else if ((relNode = ExprNode::as<RelationSourceNode>(node)))
	{
		dsql_ctx* context = relNode->dsqlContext;

		if (context->ctx_relation)
		{
			for (dsql_fld* field = context->ctx_relation->rel_fields; field; field = field->fld_next)
			{
				DEV_BLKCHK(field, dsql_type_fld);

				dsql_nod* select_item = NULL;
				if (!hide_using || context->getImplicitJoinField(field->fld_name, select_item))
				{
					if (!select_item)
						select_item = MAKE_field(context, field, 0);
					stack.push(select_item);
				}
			}
		}
	}
	else if ((fieldNode = ExprNode::as<FieldNode>(node)))
	{
		dsql_nod* list = NULL;
		dsql_nod* value = fieldNode->internalDsqlPass(dsqlScratch, &list);

		if (list)
			PASS1_expand_select_node(dsqlScratch, list, stack, false);
		else
			stack.push(value);
	}
	else
		stack.push(node);
}


/**

 	pass1_group_by_list

    @brief	Process GROUP BY list, which may contain
			an ordinal or alias which references the
			select list.

    @param dsqlScratch
    @param input
    @param select_list

 **/
static dsql_nod* pass1_group_by_list(DsqlCompilerScratch* dsqlScratch, dsql_nod* input, dsql_nod* selectList)
{
	DEV_BLKCHK(dsqlScratch, dsql_type_req);
	DEV_BLKCHK(input, dsql_type_nod);
	DEV_BLKCHK(selectList, dsql_type_nod);

	if (input->nod_count > MAX_SORT_ITEMS) // sort, group and distinct have the same limit for now
	{
		// cannot group on more than 255 items
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
				  Arg::Gds(isc_dsql_command_err) <<
				  Arg::Gds(isc_dsql_max_group_items));
	}

	DsqlNodStack stack;
	dsql_nod** ptr = input->nod_arg;
	for (const dsql_nod* const* const end = ptr + input->nod_count; ptr < end; ptr++)
	{
		DEV_BLKCHK(*ptr, dsql_type_nod);
		dsql_nod* sub = (*ptr);
		dsql_nod* frnode = NULL;
		FieldNode* field;
		LiteralNode* literal;

		if ((field = ExprNode::as<FieldNode>(sub)))
		{
			// check for alias or field node
			if (field->dsqlQualifier.isEmpty() && field->dsqlName.hasData())
			{
				// AB: Check first against the select list for matching column.
				// When no matches at all are found we go on with our
				// normal way of field name lookup.
				frnode = PASS1_lookup_alias(dsqlScratch, field->dsqlName, selectList, true);
			}

			if (!frnode)
				frnode = field->internalDsqlPass(dsqlScratch, NULL);
		}
		else if ((literal = ExprNode::as<LiteralNode>(sub)) && (literal->litDesc.dsc_dtype == dtype_long))
		{
			const ULONG position = literal->getSlong();

			if (position < 1 || !selectList || position > (ULONG) selectList->nod_count)
			{
				// Invalid column position used in the GROUP BY clause
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						  Arg::Gds(isc_dsql_column_pos_err) << Arg::Str("GROUP BY"));
			}

			frnode = PASS1_node_psql(dsqlScratch, selectList->nod_arg[position - 1], false);
		}
		else
			frnode = PASS1_node_psql(dsqlScratch, *ptr, false);

		stack.push(frnode);
	}
	// Finally make the complete list.
	dsql_nod* node = MAKE_list(stack);

	return node;
}


// Process loop interruption.
dsql_nod* PASS1_label(DsqlCompilerScratch* dsqlScratch, bool breakContinue, dsql_nod* label)
{
	DEV_BLKCHK(dsqlScratch, dsql_type_req);
	DEV_BLKCHK(label, dsql_type_nod);

	// look for a label, if specified

	const dsql_str* string = NULL;
	USHORT position = 0;

	if (label)
	{
		fb_assert(label->nod_type == nod_label);
		string = (dsql_str*) label->nod_arg[e_label_name];
		const TEXT* label_string = string->str_data;
		int index = dsqlScratch->loopLevel;
		for (DsqlStrStack::iterator stack(dsqlScratch->labels); stack.hasData(); ++stack)
		{
			const dsql_str* obj = stack.object();
			if (obj)
			{
				const TEXT* obj_string = obj->str_data;
				if (!strcmp(label_string, obj_string))
				{
					position = index;
					break;
				}
			}
			index--;
		}
	}

	USHORT number = 0;

	if (breakContinue)
	{
		if (position > 0)
		{
			// break/continue the specified loop
			number = position;
		}
		else if (label)
		{
			// ERROR: Label %s is not found in the current scope
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
					  Arg::Gds(isc_dsql_command_err) <<
					  Arg::Gds(isc_dsql_invalid_label) << Arg::Str(string->str_data) <<
														  Arg::Str("is not found"));
		}
		else
		{
			// break/continue the current loop
			number = dsqlScratch->loopLevel;
		}
	}
	else
	{
		if (position > 0)
		{
			// ERROR: Label %s already exists in the current scope
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
					  Arg::Gds(isc_dsql_command_err) <<
					  Arg::Gds(isc_dsql_invalid_label) << Arg::Str(string->str_data) <<
														  Arg::Str("already exists"));
		}
		else
		{
			// store label name, if specified
			dsqlScratch->labels.push(string);
			number = dsqlScratch->loopLevel;
		}
	}

	fb_assert(number > 0 && number <= dsqlScratch->loopLevel);

	if (!label)
	{
		label = MAKE_node(nod_label, e_label_count);
		// this label is unnamed, i.e. its nod_arg[e_label_name] is NULL
	}

	label->nod_arg[e_label_number] = (dsql_nod*) (IPTR) number;

	return label;
}


// Process the limit clause (FIRST/SKIP/ROWS)
void PASS1_limit(DsqlCompilerScratch* dsqlScratch, dsql_nod* firstNode, dsql_nod* skipNode,
	RseNode* rse)
{
	DEV_BLKCHK(dsqlScratch, dsql_type_req);
	DEV_BLKCHK(firstNode, dsql_type_nod);
	DEV_BLKCHK(skipNode, dsql_type_nod);

	// Initialize this stack variable, and make it look like a node
	AutoPtr<dsql_nod> descNode(FB_NEW_RPT(*getDefaultMemoryPool(), 0) dsql_nod);

	if (dsqlScratch->clientDialect <= SQL_DIALECT_V5)
		descNode->nod_desc.makeLong(0);
	else
		descNode->nod_desc.makeInt64(0);

	rse->dsqlFirst = PASS1_node_psql(dsqlScratch, firstNode, false);
	PASS1_set_parameter_type(dsqlScratch, rse->dsqlFirst, descNode, false);

	rse->dsqlSkip = PASS1_node_psql(dsqlScratch, skipNode, false);
	PASS1_set_parameter_type(dsqlScratch, rse->dsqlSkip, descNode, false);
}


// Lookup a matching item in the select list. Return node if found else return NULL.
// If more matches are found we raise ambiguity error.
dsql_nod* PASS1_lookup_alias(DsqlCompilerScratch* dsqlScratch, const MetaName& name,
	dsql_nod* selectList, bool process)
{
	dsql_nod* returnNode = NULL;
	dsql_nod** ptr = selectList->nod_arg;
	const dsql_nod* const* const end = ptr + selectList->nod_count;
	for (; ptr < end; ptr++)
	{
		dsql_nod* matchingNode = NULL;
		dsql_nod* node = *ptr;
		DsqlAliasNode* aliasNode;
		FieldNode* fieldNode;
		DerivedFieldNode* derivedField;

		if ((aliasNode = ExprNode::as<DsqlAliasNode>(node)))
		{
			if (aliasNode->name == name)
				matchingNode = node;
		}
		else if ((fieldNode = ExprNode::as<FieldNode>(node)))
		{
			if (fieldNode->dsqlField->fld_name == name.c_str())
				matchingNode = node;
		}
		else if ((derivedField = ExprNode::as<DerivedFieldNode>(node)))
		{
			if (derivedField->name == name)
				matchingNode = node;
		}

		if (matchingNode)
		{
			if (process)
				matchingNode = PASS1_node_psql(dsqlScratch, matchingNode, false);

			if (returnNode)
			{
				// There was already a node matched, thus raise ambiguous field name error.
				TEXT buffer1[256];
				buffer1[0] = 0;

				if (ExprNode::is<DsqlAliasNode>(returnNode))
					strcat(buffer1, "an alias");
				else if (ExprNode::is<FieldNode>(returnNode))
					strcat(buffer1, "a field");
				else if (ExprNode::is<DerivedFieldNode>(returnNode))
					strcat(buffer1, "a derived field");
				else
					strcat(buffer1, "an item");

				TEXT buffer2[256];
				buffer2[0] = 0;

				if (ExprNode::is<DsqlAliasNode>(matchingNode))
					strcat(buffer2, "an alias");
				else if (ExprNode::is<FieldNode>(matchingNode))
					strcat(buffer2, "a field");
				else if (ExprNode::is<DerivedFieldNode>(matchingNode))
					strcat(buffer2, "a derived field");
				else
					strcat(buffer2, "an item");

				strcat(buffer2, " in the select list with name");

				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-204) <<
						  Arg::Gds(isc_dsql_ambiguous_field_name) << Arg::Str(buffer1) <<
																	 Arg::Str(buffer2) <<
						  Arg::Gds(isc_random) << name);
			}

			returnNode = matchingNode;
		}
	}

	return returnNode;
}

/**

 	pass1_make_derived_field

    @brief	Create a derived field based on underlying expressions


    @param dsqlScratch
    @param tdbb
    @param select_item

 **/
static dsql_nod* pass1_make_derived_field(DsqlCompilerScratch* dsqlScratch, thread_db* tdbb,
	dsql_nod* select_item)
{
	DEV_BLKCHK(select_item, dsql_type_nod);

	MemoryPool& pool = *tdbb->getDefaultPool();
	DsqlAliasNode* aliasNode;
	SubQueryNode* subQueryNode;
	DsqlMapNode* mapNode;
	FieldNode* fieldNode;
	DerivedFieldNode* derivedField;

	if ((aliasNode = ExprNode::as<DsqlAliasNode>(select_item)))
	{
		// Create a derived field and ignore alias node.
		DerivedFieldNode* newField = FB_NEW(pool) DerivedFieldNode(pool,
			aliasNode->name, dsqlScratch->scopeLevel, aliasNode->value);

		dsql_nod* nod = MAKE_node(nod_class_exprnode, 1);
		nod->nod_arg[0] = reinterpret_cast<dsql_nod*>(newField);
		nod->nod_desc = aliasNode->value->nod_desc;

		return nod;
	}
	else if ((subQueryNode = ExprNode::as<SubQueryNode>(select_item)))
	{
		// Try to generate derived field from sub-select
		dsql_nod* derived_field = pass1_make_derived_field(dsqlScratch, tdbb,
			subQueryNode->dsqlValue1);

		if ((derivedField = ExprNode::as<DerivedFieldNode>(derived_field)))
		{
			derivedField->dsqlValue = select_item;
			return derived_field;
		}
	}
	else if ((mapNode = ExprNode::as<DsqlMapNode>(select_item)))
	{
		// Aggregate's have map on top.
		dsql_nod* derived_field = pass1_make_derived_field(dsqlScratch, tdbb, mapNode->map->map_node);

		// If we had succesfully made a derived field node change it with orginal map.
		if ((derivedField = ExprNode::as<DerivedFieldNode>(derived_field)))
		{
			derivedField->dsqlValue = select_item;
			derivedField->scope = dsqlScratch->scopeLevel;
			derived_field->nod_desc = select_item->nod_desc;
			return derived_field;
		}
	}
	else if ((fieldNode = ExprNode::as<FieldNode>(select_item)))
	{
		// Create a derived field and hook in.

		DerivedFieldNode* newField = FB_NEW(pool) DerivedFieldNode(pool,
			fieldNode->dsqlField->fld_name, dsqlScratch->scopeLevel, select_item);

		dsql_nod* nod = MAKE_node(nod_class_exprnode, 1);
		nod->nod_arg[0] = reinterpret_cast<dsql_nod*>(newField);
		nod->nod_desc = fieldNode->dsqlDesc;

		return nod;
	}
	else if ((derivedField = ExprNode::as<DerivedFieldNode>(select_item)))
	{
		// Create a derived field that points to a derived field.

		DerivedFieldNode* newField = FB_NEW(pool) DerivedFieldNode(pool,
			derivedField->name, dsqlScratch->scopeLevel, select_item);

		dsql_nod* nod = MAKE_node(nod_class_exprnode, 1);
		nod->nod_arg[0] = reinterpret_cast<dsql_nod*>(newField);
		nod->nod_desc = select_item->nod_desc;

		return nod;
	}

	return select_item;
}


// Changes dsqlScratch->isPsql() value, calls PASS1_node and restore dsqlScratch->isPsql().
dsql_nod* PASS1_node_psql(DsqlCompilerScratch* dsqlScratch, dsql_nod* input, bool psql)
{
	PsqlChanger changer(dsqlScratch, psql);
	return PASS1_node(dsqlScratch, input);
}


// Put recursive non list nodes on the stack.
void PASS1_put_args_on_stack( DsqlCompilerScratch* dsqlScratch, dsql_nod* input, DsqlNodStack& stack)
{
	DEV_BLKCHK(dsqlScratch, dsql_type_req);
	DEV_BLKCHK(input, dsql_type_nod);

	if (input->nod_type != nod_list)
	{
		stack.push(PASS1_node(dsqlScratch, input));
		return;
	}

	dsql_nod** ptr = input->nod_arg;
	for (const dsql_nod* const* const end = ptr + input->nod_count; ptr < end; ptr++)
		PASS1_put_args_on_stack(dsqlScratch, *ptr, stack);
}


// Prepare a relation name for processing. Allocate a new relation node.
dsql_nod* PASS1_relation(DsqlCompilerScratch* dsqlScratch, dsql_nod* input)
{
	thread_db* tdbb = JRD_get_thread_data();

	DEV_BLKCHK(dsqlScratch, dsql_type_req);
	DEV_BLKCHK(input, dsql_type_nod);

	dsql_ctx* context = PASS1_make_context(dsqlScratch, input);
	RecordSourceNode* node = NULL;

	if (context->ctx_relation)
	{
		RelationSourceNode* relNode = FB_NEW(*tdbb->getDefaultPool()) RelationSourceNode(
			*tdbb->getDefaultPool(), context->ctx_relation->rel_name);
		relNode->dsqlContext = context;
		node = relNode;
	}
	else if (context->ctx_procedure)
	{
		ProcedureSourceNode* procNode = FB_NEW(*tdbb->getDefaultPool()) ProcedureSourceNode(
			*tdbb->getDefaultPool(), context->ctx_procedure->prc_name);
		procNode->dsqlContext = context;
		node = procNode;
	}

	fb_assert(node);

	dsql_nod* nod = MAKE_node(nod_class_exprnode, 1);
	nod->nod_arg[0] = reinterpret_cast<dsql_nod*>(node);

	return nod;
}


/**

 	pass1_alias_list

    @brief	The passed alias list fully specifies a relation.
 	The first alias represents a relation specified in
 	the from list at this scope levels.  Subsequent
 	contexts, if there are any, represent base relations
 	in a view stack.  They are used to fully specify a
 	base relation of a view.  The aliases used in the
 	view stack are those used in the view definition.


    @param dsqlScratch
    @param alias_list

 **/
static dsql_ctx* pass1_alias_list(DsqlCompilerScratch* dsqlScratch, dsql_nod* alias_list)
{
	DEV_BLKCHK(dsqlScratch, dsql_type_req);
	DEV_BLKCHK(alias_list, dsql_type_nod);

	dsql_nod** arg = alias_list->nod_arg;
	const dsql_nod* const* const end = arg + alias_list->nod_count;

	// Loop through every alias and find the context for that alias.
	// All aliases should have a corresponding context.
	int aliasCount = alias_list->nod_count;
	USHORT savedScopeLevel = dsqlScratch->scopeLevel;
	dsql_ctx* context = NULL;
	while (aliasCount > 0)
	{
		if (context)
		{
			if (context->ctx_rse && !context->ctx_relation && !context->ctx_procedure)
			{
				// Derived table
				dsqlScratch->scopeLevel++;
				context = pass1_alias(dsqlScratch, context->ctx_childs_derived_table, (dsql_str*) *arg);
			}
			else if (context->ctx_relation)
			{
				// This must be a VIEW
				dsql_nod** startArg = arg;
				dsql_rel* relation = context->ctx_relation;
				// find the base table using the specified alias list, skipping the first one
				// since we already matched it to the context.
				for (; arg < end; arg++, aliasCount--)
				{
					if (!(relation = pass1_base_table(dsqlScratch, relation, (dsql_str*) *arg)))
						break;
				}

				// Found base relation
				if ((aliasCount == 0) && relation)
				{
					// AB: Pretty ugly huh?
					// make up a dummy context to hold the resultant relation.
					thread_db* tdbb = JRD_get_thread_data();
					dsql_ctx* new_context =
						FB_NEW(*tdbb->getDefaultPool()) dsql_ctx(*tdbb->getDefaultPool());
					new_context->ctx_context = context->ctx_context;
					new_context->ctx_relation = relation;

					// concatenate all the contexts to form the alias name;
					// calculate the length leaving room for spaces and a null
					USHORT alias_length = alias_list->nod_count;
					dsql_nod** aliasArg = startArg;
					for (; aliasArg < end; aliasArg++)
					{
						DEV_BLKCHK(*aliasArg, dsql_type_str);
						alias_length += static_cast<USHORT>(((dsql_str*) *aliasArg)->str_length);
					}

					dsql_str* alias = FB_NEW_RPT(*tdbb->getDefaultPool(), alias_length) dsql_str;
					alias->str_length = alias_length;
					new_context->ctx_alias = alias->str_data;

					TEXT* p = alias->str_data;
					for (aliasArg = startArg; aliasArg < end; aliasArg++)
					{
						for (const TEXT* q = ((dsql_str*) *aliasArg)->str_data; *q;)
							*p++ = *q++;
						*p++ = ' ';
					}
					p[-1] = 0;

					context = new_context;
				}
				else
				{
					context = NULL;
				}
			}
			else
			{
				context = NULL;
			}
		}
		else
		{
			context = pass1_alias(dsqlScratch, *dsqlScratch->context, (dsql_str*) *arg);
		}

		if (!context) {
			break;
		}

		arg++;
		aliasCount--;
	}
	dsqlScratch->scopeLevel = savedScopeLevel;

	if (!context)
	{
		// there is no alias or table named %s at this scope level.
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
				  Arg::Gds(isc_dsql_command_err) <<
				  Arg::Gds(isc_dsql_no_relation_alias) << Arg::Str(((dsql_str*) *arg)->str_data));
	}

	return context;
}


/**

 	pass1_alias

    @brief	The passed relation or alias represents
 	a context which was previously specified
 	in the from list.  Find and return the
 	proper context.


    @param dsqlScratch
    @param alias

 **/
static dsql_ctx* pass1_alias(DsqlCompilerScratch* dsqlScratch, DsqlContextStack& stack, dsql_str* alias)
{
	dsql_ctx* relation_context = NULL;

	DEV_BLKCHK(dsqlScratch, dsql_type_req);
	DEV_BLKCHK(alias, dsql_type_str);

	// CVC: Getting rid of trailing spaces.
	if (alias && alias->str_data) {
		fb_utils::exact_name(alias->str_data);
	}

	// look through all contexts at this scope level
	// to find one that has a relation name or alias
	// name which matches the identifier passed.
	for (DsqlContextStack::iterator itr(stack); itr.hasData(); ++itr)
	{
		dsql_ctx* context = itr.object();
		if (context->ctx_scope_level != dsqlScratch->scopeLevel) {
			continue;
		}

		// check for matching alias.
		if (context->ctx_internal_alias.hasData())
		{
			if (context->ctx_internal_alias == alias->str_data)
				return context;

			continue;
		}

		// If an unnamed derived table and empty alias
		if (context->ctx_rse && !context->ctx_relation && !context->ctx_procedure &&
			alias->str_length == 0)
		{
			relation_context = context;
		}

		// check for matching relation name; aliases take priority so
		// save the context in case there is an alias of the same name;
		// also to check that there is no self-join in the query.
		if (context->ctx_relation && context->ctx_relation->rel_name == alias->str_data)
		{
			if (relation_context)
			{
				// the table %s is referenced twice; use aliases to differentiate
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						  Arg::Gds(isc_dsql_command_err) <<
						  Arg::Gds(isc_dsql_self_join) << Arg::Str(alias->str_data));
			}
			relation_context = context;
		}
	}

	return relation_context;
}


/**

 	pass1_alias_concat

    @brief	Concatenate 2 input strings together for
	a new alias string.
	Note: Both input params can be empty.


    @param input1
    @param input2

 **/
static dsql_str* pass1_alias_concat(const dsql_str* input1, const char* input2)
{
	thread_db* tdbb = JRD_get_thread_data();

	DEV_BLKCHK(input1, dsql_type_str);
	DEV_BLKCHK(input2, dsql_type_str);

	int length = (input1 ? input1->str_length : 0);

	if (input1 && input1->str_length && input2)
		++length; // Room for space character.

	if (input2)
		length += strlen(input2);

	dsql_str* output = FB_NEW_RPT(*tdbb->getDefaultPool(), length) dsql_str;
	output->str_length = length;
	TEXT* ptr = output->str_data;

	if (input1)
		strcat(ptr, input1->str_data);

	if (input1 && input1->str_length && input2)
		strcat(ptr, " ");

	if (input2)
		strcat(ptr, input2);

	return output;
}


/**

 	pass1_base_table

    @brief	Check if the relation in the passed context
 	has a base table which matches the passed alias.


    @param dsqlScratch
    @param relation
    @param alias

 **/
static dsql_rel* pass1_base_table( DsqlCompilerScratch* dsqlScratch, const dsql_rel* relation,
	const dsql_str* alias)
{

	DEV_BLKCHK(dsqlScratch, dsql_type_req);
	DEV_BLKCHK(relation, dsql_type_dsql_rel);
	DEV_BLKCHK(alias, dsql_type_str);

	return METD_get_view_relation(dsqlScratch->getTransaction(), dsqlScratch,
		relation->rel_name.c_str(), alias->str_data);
}


// Wrapper for pass1_rse_impl. Substitute recursive CTE alias (if needed) and call pass1_rse_impl.
static RseNode* pass1_rse(DsqlCompilerScratch* dsqlScratch, dsql_nod* input, dsql_nod* order,
	RowsClause* rows, bool updateLock, USHORT flags)
{
	string save_alias;
	RseNode* rseNode = ExprNode::as<RseNode>(input);
	const bool isRecursive = rseNode && (rseNode->dsqlFlags & RecordSourceNode::DFLAG_RECURSIVE);

	if (isRecursive)
	{
		fb_assert(dsqlScratch->recursiveCtx);
		save_alias = dsqlScratch->recursiveCtx->ctx_alias;

		dsqlScratch->recursiveCtx->ctx_alias = *dsqlScratch->getNextCTEAlias();
	}

	RseNode* ret = pass1_rse_impl(dsqlScratch, input, order, rows, updateLock, flags);

	if (isRecursive)
		dsqlScratch->recursiveCtx->ctx_alias = save_alias;

	return ret;
}


// Compile a record selection expression. The input node may either be a "select_expression"
// or a "list" (an implicit union) or a "query specification".
static RseNode* pass1_rse_impl(DsqlCompilerScratch* dsqlScratch, dsql_nod* input, dsql_nod* order,
	RowsClause* rows, bool updateLock, USHORT flags)
{
	DEV_BLKCHK(dsqlScratch, dsql_type_req);
	DEV_BLKCHK(input, dsql_type_nod);
	DEV_BLKCHK(order, dsql_type_nod);

	thread_db* tdbb = JRD_get_thread_data();
	MemoryPool& pool = *tdbb->getDefaultPool();

	SelectExprNode* selNode = ExprNode::as<SelectExprNode>(input);
	UnionSourceNode* unionNode = ExprNode::as<UnionSourceNode>(input);

	if (selNode)
	{
		WithClause* withClause = selNode->withClause;
		try
		{
			if (withClause)
				dsqlScratch->addCTEs(withClause);

			RseNode* ret = pass1_rse(dsqlScratch, selNode->querySpec, selNode->order,
				selNode->rowsClause, updateLock, selNode->dsqlFlags);

			if (withClause)
			{
				dsqlScratch->checkUnusedCTEs();
				dsqlScratch->clearCTEs();
			}

			return ret;
		}
		catch (const Firebird::Exception&)
		{
			if (withClause)
				dsqlScratch->clearCTEs();
			throw;
		}
	}
	else if (unionNode)
	{
		fb_assert(unionNode->dsqlClauses->nod_count > 1);
		return pass1_union(dsqlScratch, unionNode, order, rows, updateLock, flags);
	}

	RseNode* inputRse = ExprNode::as<RseNode>(input);
	fb_assert(inputRse);

	// Save the original base of the context stack and process relations

	RseNode* targetRse = FB_NEW(pool) RseNode(pool);
	RseNode* rse = targetRse;

	if (updateLock)
		rse->flags |= RseNode::FLAG_WRITELOCK;

	dsql_nod* list = rse->dsqlStreams = PASS1_node_psql(dsqlScratch, inputRse->dsqlFrom, false);

	{ // scope block
		RelationSourceNode* relNode;
		const dsql_rel* relation;

		if (updateLock &&
			(list->nod_count != 1 || !(relNode = ExprNode::as<RelationSourceNode>(list->nod_arg[0])) ||
				!(relation = relNode->dsqlContext->ctx_relation) ||
				(relation->rel_flags & REL_view) || (relation->rel_flags & REL_external)))
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
					  // Token unknown
					  Arg::Gds(isc_token_err) <<
					  Arg::Gds(isc_random) << Arg::Str("WITH LOCK"));
		}
	} // end scope block

	// Process LIMIT and/or ROWS, if any

	if ((inputRse->dsqlFirst || inputRse->dsqlSkip) && rows)
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
				  // Token unknown
				  Arg::Gds(isc_token_err) <<
				  Arg::Gds(isc_random) << Arg::Str("ROWS"));
	}
	else if (rows)
		PASS1_limit(dsqlScratch, rows->length, rows->skip, rse);
	else if (inputRse->dsqlFirst || inputRse->dsqlSkip)
		PASS1_limit(dsqlScratch, inputRse->dsqlFirst, inputRse->dsqlSkip, rse);

	// Process boolean, if any

	if (inputRse->dsqlWhere)
	{
		++dsqlScratch->inWhereClause;
		rse->dsqlWhere = PASS1_node_psql(dsqlScratch, inputRse->dsqlWhere, false);
		--dsqlScratch->inWhereClause;

		// AB: An aggregate pointing to it's own parent_context isn't
		// allowed, HAVING should be used instead
		if (Aggregate2Finder::find(dsqlScratch->scopeLevel, FIELD_MATCH_TYPE_EQUAL, false,
				rse->dsqlWhere))
		{
			// Cannot use an aggregate in a WHERE clause, use HAVING instead
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
					  Arg::Gds(isc_dsql_agg_where_err));
		}
	}

#ifdef DSQL_DEBUG
	if (DSQL_debug & 16)
	{
		dsql_trace("PASS1_rse input tree:");
		DSQL_pretty(input, 0);
	}
#endif

	// Process select list, if any. If not, generate one
	dsql_nod* selectList = inputRse->dsqlSelectList;
	// First expand select list, this will expand nodes with asterisk.
	++dsqlScratch->inSelectList;
	selectList = pass1_expand_select_list(dsqlScratch, selectList, rse->dsqlStreams);

	if ((flags & RecordSourceNode::DFLAG_VALUE) && (!selectList || selectList->nod_count > 1))
	{
		// More than one column (or asterisk) is specified in column_singleton
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
				  Arg::Gds(isc_dsql_command_err) <<
				  Arg::Gds(isc_dsql_count_mismatch));
	}

	// Pass select list
	rse->dsqlSelectList = pass1_sel_list(dsqlScratch, selectList);
	--dsqlScratch->inSelectList;

	// Process ORDER clause, if any
	if (order)
	{
		++dsqlScratch->inOrderByClause;
		rse->dsqlOrder = PASS1_sort(dsqlScratch, order, selectList);
		--dsqlScratch->inOrderByClause;
	}

	// A GROUP BY, HAVING, or any aggregate function in the select list
	// will force an aggregate
	dsql_ctx* parent_context = NULL;
	RseNode* parentRse = NULL;
	AggregateSourceNode* aggregate = NULL;

	if (inputRse->dsqlGroup ||
		inputRse->dsqlHaving ||
		(rse->dsqlSelectList && AggregateFinder::find(dsqlScratch, false, rse->dsqlSelectList)) ||
		(rse->dsqlOrder && AggregateFinder::find(dsqlScratch, false, rse->dsqlOrder)))
	{
		// dimitr: don't allow WITH LOCK for aggregates
		if (updateLock)
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
					  // Token unknown
					  Arg::Gds(isc_token_err) <<
					  Arg::Gds(isc_random) << Arg::Str("WITH LOCK"));
		}

		parent_context = FB_NEW(*tdbb->getDefaultPool()) dsql_ctx(*tdbb->getDefaultPool());
		parent_context->ctx_scope_level = dsqlScratch->scopeLevel;

		// When we're in a outer-join part mark context for it.
		if (dsqlScratch->inOuterJoin)
			parent_context->ctx_flags |= CTX_outer_join;
		parent_context->ctx_in_outer_join = dsqlScratch->inOuterJoin;

		aggregate = FB_NEW(pool) AggregateSourceNode(pool);
		aggregate->dsqlContext = parent_context;
		aggregate->dsqlRse = MAKE_class_node(rse);
		parentRse = targetRse = FB_NEW(pool) RseNode(pool);
		parentRse->dsqlStreams = list = MAKE_node(nod_list, 1);

		dsql_nod* aggregateNod = MAKE_node(nod_class_exprnode, 1);
		aggregateNod->nod_arg[0] = reinterpret_cast<dsql_nod*>(aggregate);
		list->nod_arg[0] = aggregateNod;

		if (rse->dsqlFirst)
		{
			parentRse->dsqlFirst = rse->dsqlFirst;
			rse->dsqlFirst = NULL;
		}

		if (rse->dsqlSkip)
		{
			parentRse->dsqlSkip = rse->dsqlSkip;
			rse->dsqlSkip = NULL;
		}

		dsqlScratch->context->push(parent_context);
		// replace original contexts with parent context
		remap_streams_to_parent_context(rse->dsqlStreams, parent_context);
	}

	// Process GROUP BY clause, if any
	if (inputRse->dsqlGroup)
	{
		// if there are positions in the group by clause then replace them
		// by the (newly pass) items from the select_list
		++dsqlScratch->inGroupByClause;
		aggregate->dsqlGroup = pass1_group_by_list(dsqlScratch, inputRse->dsqlGroup, selectList);
		--dsqlScratch->inGroupByClause;

		// AB: An field pointing to another parent_context isn't
		// allowed and GROUP BY items can't contain aggregates
		if (FieldFinder::find(dsqlScratch->scopeLevel, FIELD_MATCH_TYPE_LOWER, aggregate->dsqlGroup) ||
			Aggregate2Finder::find(dsqlScratch->scopeLevel, FIELD_MATCH_TYPE_LOWER_EQUAL,
				false, aggregate->dsqlGroup))
		{
			// Cannot use an aggregate in a GROUP BY clause
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
					  Arg::Gds(isc_dsql_agg_group_err));
		}
	}

	// Parse a user-specified access PLAN
	rse->dsqlPlan = PASS1_node_psql(dsqlScratch, inputRse->dsqlPlan, false);

	// AB: Pass select-items for distinct operation again, because for
	// sub-selects a new context number should be generated
	if (inputRse->dsqlDistinct)
	{
		if (updateLock)
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
					  // Token unknown
					  Arg::Gds(isc_token_err) <<
					  Arg::Gds(isc_random) << Arg::Str("WITH LOCK"));
		}

		++dsqlScratch->inSelectList;
		// ASF: We pass false to viewFields parameter here because these expressions are
		// generated inside the view body, and not in view fields.
		targetRse->dsqlDistinct = pass1_sel_list(dsqlScratch, selectList);
		--dsqlScratch->inSelectList;

		// sort, group and distinct have the same limit for now
		if (selectList->nod_count > MAX_SORT_ITEMS)
		{
			// Cannot have more than 255 items in DISTINCT list.
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
					  Arg::Gds(isc_dsql_command_err) <<
					  Arg::Gds(isc_dsql_max_distinct_items));
		}
	}

	if (parent_context)
	{
		// Reset context of select items to point to the parent stream

		parentRse->dsqlSelectList = FieldRemapper::remap(dsqlScratch, parent_context, false,
			rse->dsqlSelectList);
		rse->dsqlSelectList = NULL;

		// AB: Check for invalid constructions inside selected-items list
		list = parentRse->dsqlSelectList;

		{ // scope block
			const dsql_nod* const* ptr = list->nod_arg;
			for (const dsql_nod* const* const end = ptr + list->nod_count; ptr < end; ptr++)
			{
				if (InvalidReferenceFinder::find(parent_context, aggregate->dsqlGroup, *ptr))
				{
					// Invalid expression in the select list
					// (not contained in either an aggregate or the GROUP BY clause)
					ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
							  Arg::Gds(isc_dsql_agg_column_err) << Arg::Str("select list"));
				}
			}
		} // end scope block

		// Reset context of order items to point to the parent stream

		if (order)
		{
			parentRse->dsqlOrder = FieldRemapper::remap(dsqlScratch, parent_context,
				false, rse->dsqlOrder);
			rse->dsqlOrder = NULL;

			// AB: Check for invalid contructions inside the ORDER BY clause
			list = targetRse->dsqlOrder;
			const dsql_nod* const* ptr = list->nod_arg;
			for (const dsql_nod* const* const end = ptr + list->nod_count; ptr < end; ptr++)
			{
				if (InvalidReferenceFinder::find(parent_context, aggregate->dsqlGroup, *ptr))
				{
					// Invalid expression in the ORDER BY clause
					// (not contained in either an aggregate or the GROUP BY clause)
					ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
							  Arg::Gds(isc_dsql_agg_column_err) << Arg::Str("ORDER BY clause"));
				}
			}
		}

		// And, of course, reduction clauses must also apply to the parent
		if (inputRse->dsqlDistinct)
		{
			parentRse->dsqlDistinct = FieldRemapper::remap(dsqlScratch, parent_context,
				false, parentRse->dsqlDistinct);
		}

		// Process HAVING clause, if any

		if (inputRse->dsqlHaving)
		{
			++dsqlScratch->inHavingClause;
			parentRse->dsqlWhere = PASS1_node_psql(dsqlScratch, inputRse->dsqlHaving, false);
			--dsqlScratch->inHavingClause;

			parentRse->dsqlWhere = FieldRemapper::remap(dsqlScratch, parent_context,
				false, parentRse->dsqlWhere);

			// AB: Check for invalid contructions inside the HAVING clause

			if (InvalidReferenceFinder::find(parent_context, aggregate->dsqlGroup,
					parentRse->dsqlWhere))
			{
				// Invalid expression in the HAVING clause
				// (neither an aggregate nor contained in the GROUP BY clause)
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						  Arg::Gds(isc_dsql_agg_having_err) << Arg::Str("HAVING clause"));
			}

			if (AggregateFinder::find(dsqlScratch, true, parentRse->dsqlWhere))
			{
				// Cannot use an aggregate in a WHERE clause, use HAVING instead
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						  Arg::Gds(isc_dsql_agg_where_err));
			}
		}

		rse = parentRse;

		parent_context->ctx_context = dsqlScratch->contextNumber++;
	}

	const bool sortWindow = rse->dsqlOrder && AggregateFinder::find(dsqlScratch, true, rse->dsqlOrder);

	// WINDOW functions
	if ((rse->dsqlSelectList && AggregateFinder::find(dsqlScratch, true, rse->dsqlSelectList)) ||
		sortWindow)
	{
		AutoSetRestore<bool> autoProcessingWindow(&dsqlScratch->processingWindow, true);

		parent_context = FB_NEW(*tdbb->getDefaultPool()) dsql_ctx(*tdbb->getDefaultPool());
		parent_context->ctx_scope_level = dsqlScratch->scopeLevel;

		AggregateSourceNode* window = FB_NEW(pool) AggregateSourceNode(pool);
		window->dsqlContext = parent_context;
		window->dsqlRse = MAKE_class_node(rse);
		window->dsqlWindow = true;

		dsql_nod* windowNod = MAKE_node(nod_class_exprnode, 1);
		windowNod->nod_arg[0] = reinterpret_cast<dsql_nod*>(window);

		parentRse = targetRse = FB_NEW(pool) RseNode(pool);
		parentRse->dsqlStreams = list = MAKE_node(nod_list, 1);
		list->nod_arg[0] = windowNod;

		if (rse->flags & RseNode::FLAG_WRITELOCK)
		{
			parentRse->flags |= RseNode::FLAG_WRITELOCK;
			rse->flags &= ~RseNode::FLAG_WRITELOCK;
		}

		if (rse->dsqlFirst)
		{
			parentRse->dsqlFirst = rse->dsqlFirst;
			rse->dsqlFirst = NULL;
		}

		if (rse->dsqlSkip)
		{
			parentRse->dsqlSkip = rse->dsqlSkip;
			rse->dsqlSkip = NULL;
		}

		dsqlScratch->context->push(parent_context);
		// replace original contexts with parent context
		remap_streams_to_parent_context(rse->dsqlStreams, parent_context);

		if (aggregate)
		{
			// Check for invalid contructions inside selected-items list
			list = rse->dsqlSelectList;
			const dsql_nod* const* ptr = list->nod_arg;
			for (const dsql_nod* const* const end = ptr + list->nod_count; ptr < end; ptr++)
			{
				if (InvalidReferenceFinder::find(parent_context, aggregate->dsqlGroup, *ptr))
				{
					// Invalid expression in the select list
					// (not contained in either an aggregate or the GROUP BY clause)
					ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
							  Arg::Gds(isc_dsql_agg_column_err) << Arg::Str("select list"));
				}
			}
		}

		parentRse->dsqlSelectList = FieldRemapper::remap(dsqlScratch, parent_context, true,
			rse->dsqlSelectList);

		// Remap the nodes to the partition context.
		for (size_t i = 0, mapCount = parent_context->ctx_win_maps.getCount(); i < mapCount; ++i)
		{
			PartitionMap* partitionMap = parent_context->ctx_win_maps[i];
			if (partitionMap->partition)
			{
				partitionMap->partitionRemapped = PASS1_node(dsqlScratch, partitionMap->partition);
				partitionMap->partitionRemapped = FieldRemapper::remap(dsqlScratch, parent_context,
					true, partitionMap->partitionRemapped, partitionMap->partition,
					partitionMap->order);
			}
		}

		rse->dsqlSelectList = NULL;

		if (order)
		{
			if (aggregate)
			{
				// Check for invalid contructions inside the order-by list
				list = rse->dsqlOrder;
				const dsql_nod* const* ptr = list->nod_arg;
				for (const dsql_nod* const* const end = ptr + list->nod_count; ptr < end; ptr++)
				{
					if (InvalidReferenceFinder::find(parent_context, aggregate->dsqlGroup, *ptr))
					{
						// Invalid expression in the ORDER BY list
						// (not contained in either an aggregate or the GROUP BY clause)
						ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
								  Arg::Gds(isc_dsql_agg_column_err) << Arg::Str("ORDER BY list"));
					}
				}
			}

			parentRse->dsqlOrder = FieldRemapper::remap(dsqlScratch, parent_context,
				true, rse->dsqlOrder);

			rse->dsqlOrder = NULL;
		}

		// And, of course, reduction clauses must also apply to the parent
		if (rse->dsqlDistinct)
		{
			parentRse->dsqlDistinct = FieldRemapper::remap(dsqlScratch, parent_context,
				true, rse->dsqlDistinct);
			rse->dsqlDistinct = NULL;
		}

		rse = parentRse;
	}

	rse->dsqlFlags = flags;

	return rse;
}


/**

 	pass1_sel_list

    @brief	Compile a select list.

    @param dsqlScratch
    @param input

 **/
static dsql_nod* pass1_sel_list(DsqlCompilerScratch* dsqlScratch, dsql_nod* input)
{
	DEV_BLKCHK(dsqlScratch, dsql_type_req);
	DEV_BLKCHK(input, dsql_type_nod);

	DsqlNodStack stack;
	dsql_nod** ptr = input->nod_arg;
	for (const dsql_nod* const* const end = ptr + input->nod_count; ptr < end; ptr++)
	{
		DEV_BLKCHK(*ptr, dsql_type_nod);
		stack.push(PASS1_node_psql(dsqlScratch, *ptr, false));
	}
	dsql_nod* node = MAKE_list(stack);

	return node;
}


// Process ORDER BY list, which may contain an ordinal or alias which references the select list.
dsql_nod* PASS1_sort( DsqlCompilerScratch* dsqlScratch, dsql_nod* input, dsql_nod* selectList)
{
	DEV_BLKCHK(dsqlScratch, dsql_type_req);
	DEV_BLKCHK(input, dsql_type_nod);
	DEV_BLKCHK(selectList, dsql_type_nod);

	thread_db* tdbb = JRD_get_thread_data();
	MemoryPool& pool = *tdbb->getDefaultPool();

	if (input->nod_type != nod_list)
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
				  Arg::Gds(isc_dsql_command_err) <<
				  // invalid ORDER BY clause
				  Arg::Gds(isc_order_by_err));
	}

	if (input->nod_count > MAX_SORT_ITEMS)
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
				  Arg::Gds(isc_dsql_command_err) <<
				  Arg::Gds(isc_order_by_err) <<
				  // invalid ORDER BY clause, cannot sort on more than 255 items
				  Arg::Gds(isc_dsql_max_sort_items));
	}

	// Node is simply to be rebuilt -- just recurse merrily

	dsql_nod* node = MAKE_node(input->nod_type, input->nod_count);
	dsql_nod** ptr2 = node->nod_arg;

	for (int sortloop = 0; sortloop < input->nod_count; sortloop++)
	{
		DEV_BLKCHK(input->nod_arg[sortloop], dsql_type_nod);
		OrderNode* node1 = ExprNode::as<OrderNode>(input->nod_arg[sortloop]);
		if (!node1)
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
					  Arg::Gds(isc_dsql_command_err) <<
					  // invalid ORDER BY clause
					  Arg::Gds(isc_order_by_err));
		}

		// get node of value to be ordered by
		dsql_nod* orderValue = node1->dsqlValue;

		const CollateNode* collateNode = ExprNode::as<CollateNode>(orderValue);

		if (collateNode)
		{
			// substitute CollateNode with its argument (real value)
			orderValue = collateNode->dsqlArg;
		}

		FieldNode* field;
		LiteralNode* literal;

		if ((field = ExprNode::as<FieldNode>(orderValue)))
		{
			dsql_nod* aliasNode = NULL;

			// check for alias or field node
			if (field->dsqlQualifier.isEmpty() && field->dsqlName.hasData())
			{
				// AB: Check first against the select list for matching column.
				// When no matches at all are found we go on with our
				// normal way of field name lookup.
				aliasNode = PASS1_lookup_alias(dsqlScratch, field->dsqlName, selectList, true);
			}

			orderValue = aliasNode ? aliasNode : field->internalDsqlPass(dsqlScratch, NULL);
		}
		else if ((literal = ExprNode::as<LiteralNode>(orderValue)) && literal->litDesc.dsc_dtype == dtype_long)
		{
			const ULONG position = literal->getSlong();

			if (position < 1 || !selectList || position > (ULONG) selectList->nod_count)
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						  // Invalid column position used in the ORDER BY clause
						  Arg::Gds(isc_dsql_column_pos_err) << Arg::Str("ORDER BY"));
			}

			// substitute ordinal with appropriate field
			orderValue = PASS1_node_psql(dsqlScratch, selectList->nod_arg[position - 1], false);
		}
		else
			orderValue = PASS1_node_psql(dsqlScratch, orderValue, false);

		if (collateNode)
		{
			// Finally apply collation order, if necessary.
			orderValue = MAKE_class_node(
				CollateNode::pass1Collate(dsqlScratch, orderValue, collateNode->collation));
		}

		OrderNode* node2 = FB_NEW(pool) OrderNode(pool, orderValue);
		node2->descending = node1->descending;
		node2->nullsPlacement = node1->nullsPlacement;

		// store actual value to be ordered by
		*ptr2++ = MAKE_class_node(node2);
	}

	return node;
}


// Handle a UNION of substreams, generating a mapping of all the fields and adding an implicit
// PROJECT clause to ensure that all the records returned are unique.
static RseNode* pass1_union(DsqlCompilerScratch* dsqlScratch, UnionSourceNode* input,
	dsql_nod* order_list, RowsClause* rows, bool updateLock, USHORT flags)
{
	DEV_BLKCHK(dsqlScratch, dsql_type_req);
	DEV_BLKCHK(order_list, dsql_type_nod);

	thread_db* tdbb = JRD_get_thread_data();
	MemoryPool& pool = *tdbb->getDefaultPool();

	// set up the rse node for the union.

	UnionSourceNode* unionSource = FB_NEW(pool) UnionSourceNode(pool);
	unionSource->dsqlAll = input->dsqlAll;
	unionSource->recursive = input->recursive;

	RseNode* unionRse = FB_NEW(pool) RseNode(pool);
	unionRse->dsqlStreams = MAKE_class_node(unionSource);

	// generate a context for the union itself.
	dsql_ctx* union_context = FB_NEW(*tdbb->getDefaultPool()) dsql_ctx(*tdbb->getDefaultPool());

	if (input->recursive)
		union_context->ctx_context = dsqlScratch->recursiveCtxId;
	else
		union_context->ctx_context = dsqlScratch->contextNumber++;

	union_context->ctx_scope_level = dsqlScratch->scopeLevel;

	// When we're in a outer-join part mark context for it.
	if (dsqlScratch->inOuterJoin)
		union_context->ctx_flags |= CTX_outer_join;
	union_context->ctx_in_outer_join = dsqlScratch->inOuterJoin;

	dsqlScratch->context->push(union_context);

	unionSource->dsqlClauses = MAKE_node(nod_list, input->dsqlClauses->nod_count);

	// process all the sub-rse's.
	{ // scope block
		dsql_nod** uptr = unionSource->dsqlClauses->nod_arg;
		const DsqlContextStack::const_iterator base(*dsqlScratch->context);
		dsql_nod** ptr = input->dsqlClauses->nod_arg;

		for (const dsql_nod* const* const end = ptr + input->dsqlClauses->nod_count; ptr < end; ++ptr, ++uptr)
		{
			dsqlScratch->scopeLevel++;
			*uptr = MAKE_class_node(pass1_rse(dsqlScratch, *ptr, NULL, NULL, false, 0));
			dsqlScratch->scopeLevel--;

			while (*(dsqlScratch->context) != base)
				dsqlScratch->unionContext.push(dsqlScratch->context->pop());

			// Push recursive context after initial select has been processed.
			// Corresponding pop occurs in pass1_derived_table
			if (input->recursive && (ptr == input->dsqlClauses->nod_arg))
				dsqlScratch->context->push(dsqlScratch->recursiveCtx);
		}
	} // end scope block

	// generate the list of fields to select.
	dsql_nod* items = ExprNode::as<RseNode>(unionSource->dsqlClauses->nod_arg[0])->dsqlSelectList;

	// loop through the list nodes, checking to be sure that they have the
	// same number of items

	for (int i = 1; i < unionSource->dsqlClauses->nod_count; ++i)
	{
		const dsql_nod* nod1 = ExprNode::as<RseNode>(unionSource->dsqlClauses->nod_arg[i])->dsqlSelectList;

		if (items->nod_count != nod1->nod_count)
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
					  Arg::Gds(isc_dsql_command_err) <<
					  // overload of msg
					  Arg::Gds(isc_dsql_count_mismatch));
		}
	}

	// Comment below belongs to the old code (way a union was handled).

	// SQL II, section 9.3, pg 195 governs which data types
	// are considered equivalent for a UNION
	// The following restriction is in some ways more restrictive
	// (cannot UNION CHAR with VARCHAR for instance)
	// (or cannot union CHAR of different lengths)
	// and in someways less restrictive
	// (SCALE is not looked at)
	// Workaround: use a direct CAST() dsqlScratch in the SQL
	// dsqlScratch to force desired datatype.

	// loop through the list nodes and cast whenever possible.
	dsql_nod* tmp_list = MAKE_node(nod_list, unionSource->dsqlClauses->nod_count);

	for (int j = 0; j < items->nod_count; ++j)
	{
		for (int i = 0; i < unionSource->dsqlClauses->nod_count; ++i)
		{
			dsql_nod* nod1 = ExprNode::as<RseNode>(unionSource->dsqlClauses->nod_arg[i])->dsqlSelectList;
			MAKE_desc(dsqlScratch, &nod1->nod_arg[j]->nod_desc, nod1->nod_arg[j]);
			tmp_list->nod_arg[i] = nod1->nod_arg[j];

			// We look only at the items->nod_arg[] when creating the
			// output descriptors. Make sure that the sub-rses
			// descriptor flags are copied onto items->nod_arg[]->nod_desc.
			// Bug 65584.
			// -Sudesh 07/28/1999.
			if (i > 0)
			{
				if (nod1->nod_arg[j]->nod_desc.dsc_flags & DSC_nullable)
					items->nod_arg[j]->nod_desc.dsc_flags |= DSC_nullable;
			}
		}

		dsc desc;
		MAKE_desc_from_list(dsqlScratch, &desc, tmp_list, "UNION");
		// Only mark upper node as a NULL node when all sub-nodes are NULL
		items->nod_arg[j]->nod_desc.dsc_flags &= ~DSC_null;
		items->nod_arg[j]->nod_desc.dsc_flags |= (desc.dsc_flags & DSC_null);

		for (int i = 0; i < unionSource->dsqlClauses->nod_count; ++i)
			pass1_union_auto_cast(dsqlScratch, unionSource->dsqlClauses->nod_arg[i], desc, j);
	}

	items = ExprNode::as<RseNode>(unionSource->dsqlClauses->nod_arg[0])->dsqlSelectList;

	// Create mappings for union.

	dsql_nod* union_items = MAKE_node(nod_list, items->nod_count);

	{ // scope block
		USHORT count = 0;
		dsql_nod** uptr = items->nod_arg;
		dsql_nod** ptr = union_items->nod_arg;

		for (const dsql_nod* const* const end = ptr + union_items->nod_count; ptr < end; ptr++)
		{
			// Set up the dsql_map* between the sub-rses and the union context.
			dsql_map* map = union_context->ctx_map = FB_NEW(*tdbb->getDefaultPool()) dsql_map;
			map->map_position = count++;
			map->map_node = *uptr++;
			map->map_next = union_context->ctx_map;
			map->map_partition = NULL;

			DsqlMapNode* mapNode = FB_NEW(*tdbb->getDefaultPool()) DsqlMapNode(*tdbb->getDefaultPool(),
				union_context, map);

		    *ptr = MAKE_node(nod_class_exprnode, 1);
		    (*ptr)->nod_arg[0] = reinterpret_cast<dsql_nod*>(mapNode);
		}

		unionRse->dsqlSelectList = union_items;
	} // end scope block

	// Process ORDER clause, if any.
	if (order_list)
	{
		dsql_nod* sort = MAKE_node(nod_list, order_list->nod_count);
		dsql_nod** uptr = sort->nod_arg;
		dsql_nod** ptr = order_list->nod_arg;

		for (const dsql_nod* const* const end = ptr + order_list->nod_count; ptr < end; ptr++, uptr++)
		{
			OrderNode* order1 = ExprNode::as<OrderNode>(*ptr);
			const dsql_nod* position = order1->dsqlValue;
			const CollateNode* collateNode = ExprNode::as<CollateNode>(position);

			if (collateNode)
				position = collateNode->dsqlArg;

			const LiteralNode* literal = ExprNode::as<LiteralNode>(position);

			if (!literal || literal->litDesc.dsc_dtype != dtype_long)
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						  Arg::Gds(isc_dsql_command_err) <<
						  // invalid ORDER BY clause.
						  Arg::Gds(isc_order_by_err));
			}

			const SLONG number = literal->getSlong();

			if (number < 1 || number > union_items->nod_count)
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						  Arg::Gds(isc_dsql_command_err) <<
						  // invalid ORDER BY clause.
						  Arg::Gds(isc_order_by_err));
			}

			// make a new order node pointing at the Nth item in the select list.
			OrderNode* order2 = FB_NEW(pool) OrderNode(pool, union_items->nod_arg[number - 1]);
			*uptr = MAKE_class_node(order2);
			order2->descending = order1->descending;

			if (collateNode)
			{
				order2->dsqlValue = MAKE_class_node(CollateNode::pass1Collate(
					dsqlScratch, order2->dsqlValue, collateNode->collation));
			}

			order2->nullsPlacement = order1->nullsPlacement;
		}

		unionRse->dsqlOrder = sort;
	}

	if (rows)
		PASS1_limit(dsqlScratch, rows->length, rows->skip, unionRse);

	// PROJECT on all the select items unless UNION ALL was specified.
	if (!input->dsqlAll)
	{
		if (updateLock)
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
					  // Token unknown
					  Arg::Gds(isc_token_err) <<
					  Arg::Gds(isc_random) << Arg::Str("WITH LOCK"));
		}

		unionRse->dsqlDistinct = union_items;
	}

	if (updateLock)
		unionRse->flags |= RseNode::FLAG_WRITELOCK;

	unionRse->dsqlFlags = flags;

	return unionRse;
}


/**

 	pass1_union_auto_cast

    @brief	Auto cast types to the same type by the rules from
	MAKE_desc_from_list. SELECT X1 FROM .. UNION SELECT X2 FROM ..
	Items X1..Xn are collected together to make the cast-descriptor, this
	was done by the caller (param desc and input is the collection).
	Then is a cast generated (or reused) for every X item if it has
	another descriptor than the param desc.
	Position tells us which column-nr we are processing.

    @param input
    @param desc
    @param position
    @param in_select_list

 **/
static void pass1_union_auto_cast(DsqlCompilerScratch* dsqlScratch, dsql_nod* input,
	const dsc& desc, SSHORT position, bool in_select_list)
{
	thread_db* tdbb = JRD_get_thread_data();

	DEV_BLKCHK(input, dsql_type_nod);

	switch (input->nod_type)
	{
		case nod_list:
			if (in_select_list)
			{
				if (position < 0 || position >= input->nod_count)
				{
					// Internal dsql error: column position out of range in pass1_union_auto_cast
					ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
							  Arg::Gds(isc_dsql_command_err) <<
							  Arg::Gds(isc_dsql_auto_field_bad_pos));
				}
				else
				{
					dsql_nod* select_item = input->nod_arg[position];
					MAKE_desc(dsqlScratch, &select_item->nod_desc, select_item);

					if (select_item->nod_desc.dsc_dtype != desc.dsc_dtype ||
						select_item->nod_desc.dsc_length != desc.dsc_length ||
						select_item->nod_desc.dsc_scale != desc.dsc_scale ||
						select_item->nod_desc.dsc_sub_type != desc.dsc_sub_type)
					{
						// Because this select item has a different descriptor then
						// our finally descriptor CAST it.
						dsql_nod* cast_node = NULL;
						DsqlAliasNode* newAliasNode = NULL;
						DsqlAliasNode* aliasNode;
						DerivedFieldNode* derivedField;

						// Pick a existing cast if available else make a new one.
						if ((aliasNode = ExprNode::as<DsqlAliasNode>(select_item)) &&
							aliasNode->value && ExprNode::is<CastNode>(aliasNode->value))
						{
							cast_node = aliasNode->value;
						}
						else if ((derivedField = ExprNode::as<DerivedFieldNode>(select_item)) &&
							ExprNode::is<CastNode>(derivedField->dsqlValue))
						{
							cast_node = derivedField->dsqlValue;
						}
						else if (ExprNode::is<CastNode>(select_item))
							cast_node = select_item;
						else
						{
							CastNode* castNode = FB_NEW(*tdbb->getDefaultPool()) CastNode(
								*tdbb->getDefaultPool());

							castNode->dsqlField = FB_NEW(*tdbb->getDefaultPool()) dsql_fld(
								*tdbb->getDefaultPool());

							cast_node = MAKE_node(nod_class_exprnode, 1);
							cast_node->nod_arg[0] = reinterpret_cast<dsql_nod*>(castNode);

							// We want to leave the ALIAS node on his place, because a UNION
							// uses the select_items from the first sub-rse to determine the
							// columnname.
							if ((aliasNode = ExprNode::as<DsqlAliasNode>(select_item)))
								castNode->dsqlSource = aliasNode->value;
							else if ((derivedField = ExprNode::as<DerivedFieldNode>(select_item)))
								castNode->dsqlSource = derivedField->dsqlValue;
							else
								castNode->dsqlSource = select_item;

							// When a cast is created we're losing our fieldname, thus
							// create an alias to keep it.
							const dsql_nod* name_node = select_item;
							const DsqlMapNode* mapNode;

							while ((mapNode = ExprNode::as<DsqlMapNode>(name_node)))
							{
								// Skip all the DsqlMapNodes.
								name_node = mapNode->map->map_node;
							}

							const FieldNode* fieldNode;

							if ((fieldNode = ExprNode::as<FieldNode>(name_node)))
							{
								// Create new node for alias and copy fieldname.
								newAliasNode = FB_NEW(*tdbb->getDefaultPool()) DsqlAliasNode(
									*tdbb->getDefaultPool(), fieldNode->dsqlField->fld_name, NULL);
								// The alias value will be assigned a bit later.
							}
						}

						dsql_fld* field = ExprNode::as<CastNode>(cast_node)->dsqlField;
						// Copy the descriptor to a field, because the gen_cast
						// uses a dsql field type.
						field->fld_dtype = desc.dsc_dtype;
						field->fld_scale = desc.dsc_scale;
						field->fld_sub_type = desc.dsc_sub_type;
						field->fld_length = desc.dsc_length;
						field->fld_flags = (desc.dsc_flags & DSC_nullable) ? FLD_nullable : 0;

						if (desc.dsc_dtype <= dtype_any_text)
						{
							field->fld_ttype = desc.dsc_sub_type;
							field->fld_character_set_id = INTL_GET_CHARSET(&desc);
							field->fld_collation_id = INTL_GET_COLLATE(&desc);
						}
						else if (desc.dsc_dtype == dtype_blob)
						{
							field->fld_character_set_id = desc.dsc_scale;
							field->fld_collation_id = desc.dsc_flags >> 8;
						}

						// Finally copy the descriptors to the root nodes and swap
						// the necessary nodes.
						cast_node->nod_desc = desc;

						if (select_item->nod_desc.dsc_flags & DSC_nullable)
							cast_node->nod_desc.dsc_flags |= DSC_nullable;

						if ((aliasNode = ExprNode::as<DsqlAliasNode>(select_item)))
						{
							aliasNode->value = cast_node;
							aliasNode->value->nod_desc = desc;
							select_item->nod_desc = desc;
						}
						else if ((derivedField = ExprNode::as<DerivedFieldNode>(select_item)))
						{
							derivedField->dsqlValue = cast_node;
							derivedField->dsqlValue->nod_desc = desc;
							select_item->nod_desc = desc;
						}
						else
						{
							// If a new alias was created for keeping original field-name
							// make the alias the "top" node.
							if (newAliasNode)
							{
								newAliasNode->value = cast_node;
								newAliasNode->value->nod_desc = cast_node->nod_desc;
								input->nod_arg[position] = MAKE_node(nod_class_exprnode, 1);
								input->nod_arg[position]->nod_arg[0] =
									reinterpret_cast<dsql_nod*>(newAliasNode);
							}
							else
								input->nod_arg[position] = cast_node;
						}
					}
				}
			}
			else
			{
				dsql_nod** ptr = input->nod_arg;

				for (const dsql_nod* const* const end = ptr + input->nod_count; ptr < end; ptr++)
					pass1_union_auto_cast(dsqlScratch, *ptr, desc, position);
			}
			break;

		case nod_class_exprnode:
		{
			RseNode* rseNode;
			UnionSourceNode* unionNode;

			if ((rseNode = ExprNode::as<RseNode>(input)) && !rseNode->dsqlExplicitJoin &&
				!rseNode->dsqlContext)	// not derived table
			{
				pass1_union_auto_cast(dsqlScratch, rseNode->dsqlStreams, desc, position);

				if ((unionNode = ExprNode::as<UnionSourceNode>(rseNode->dsqlStreams)))
				{
					// We're now in a UNION under a UNION so don't change the existing mappings.
					// Only replace the node where the map points to, because they could be changed.
					dsql_nod* sub_rse_items = ExprNode::as<RseNode>(
						unionNode->dsqlClauses->nod_arg[0])->dsqlSelectList;
					dsql_map* map = ExprNode::as<DsqlMapNode>(rseNode->dsqlSelectList->nod_arg[position])->map;
					map->map_node = sub_rse_items->nod_arg[position];
					rseNode->dsqlSelectList->nod_arg[position]->nod_desc = desc;
				}
				else
					pass1_union_auto_cast(dsqlScratch, rseNode->dsqlSelectList, desc, position, true);
			}
			else if ((unionNode = ExprNode::as<UnionSourceNode>(input)))
			{
				dsql_nod** ptr = unionNode->dsqlClauses->nod_arg;

				for (const dsql_nod* const* const end = ptr + unionNode->dsqlClauses->nod_count;
					 ptr != end;
					 ++ptr)
				{
					pass1_union_auto_cast(dsqlScratch, *ptr, desc, position);
				}
			}

			break;
		}
	}
}


// Post an item to a map for a context.
dsql_nod* PASS1_post_map(DsqlCompilerScratch* dsqlScratch, ValueExprNode* node, dsql_ctx* context,
	dsql_nod* partitionNode, dsql_nod* orderNode)
{
    dsql_nod* nod = MAKE_node(nod_class_exprnode, 1);
    nod->nod_arg[0] = reinterpret_cast<dsql_nod*>(node);
    return PASS1_post_map(dsqlScratch, nod, context, partitionNode, orderNode);
}

// Post an item to a map for a context.
dsql_nod* PASS1_post_map(DsqlCompilerScratch* dsqlScratch, dsql_nod* node, dsql_ctx* context,
	dsql_nod* partitionNode, dsql_nod* orderNode)
{
	DEV_BLKCHK(node, dsql_type_nod);
	DEV_BLKCHK(context, dsql_type_ctx);

	thread_db* tdbb = JRD_get_thread_data();

	PartitionMap* partitionMap = NULL;
	dsql_map* map = NULL;

	if (dsqlScratch->processingWindow)
	{
		partitionMap = context->getPartitionMap(dsqlScratch, partitionNode, orderNode);
		map = partitionMap->map;
	}
	else
		map = context->ctx_map;

	USHORT count = 0;

	while (map)
	{
		if (PASS1_node_match(node, map->map_node, false))
			break;

		++count;
		map = map->map_next;
	}

	if (!map)
	{
		dsql_map** next = partitionMap ? &partitionMap->map : &context->ctx_map;

		if (*next)
		{
			while (*(next = &(*next)->map_next))
				;
		}

		map = *next = FB_NEW(*tdbb->getDefaultPool()) dsql_map;
		map->map_position = count;
		map->map_node = node;
		map->map_partition = partitionMap;
	}

	MAKE_desc(dsqlScratch, &node->nod_desc, node);

	DsqlMapNode* mapNode = FB_NEW(*tdbb->getDefaultPool()) DsqlMapNode(*tdbb->getDefaultPool(),
		context, map);

    dsql_nod* new_node = MAKE_node(nod_class_exprnode, 1);
    new_node->nod_arg[0] = reinterpret_cast<dsql_nod*>(mapNode);
	new_node->nod_desc = node->nod_desc;

	return new_node;
}


/**

 remap_streams_to_parent_context

    @brief	For each relation in the list, flag the relation's context
 	as having a parent context.  Be sure to handle joins
 	(Bug 6674).


    @param input
    @param parent_context

 **/
static void remap_streams_to_parent_context( dsql_nod* input, dsql_ctx* parent_context)
{
	DEV_BLKCHK(input, dsql_type_nod);
	DEV_BLKCHK(parent_context, dsql_type_ctx);

	switch (input->nod_type)
	{
	case nod_list:
		{
			dsql_nod** ptr = input->nod_arg;
			for (const dsql_nod* const* const end = ptr + input->nod_count; ptr < end; ptr++)
				remap_streams_to_parent_context(*ptr, parent_context);
		}
		break;

	default:
		{
			ProcedureSourceNode* procNode;
			RelationSourceNode* relNode;
			RseNode* rseNode;
			UnionSourceNode* unionNode;

			if ((procNode = ExprNode::as<ProcedureSourceNode>(input)))
			{
				DEV_BLKCHK(procNode->dsqlContext, dsql_type_ctx);
				procNode->dsqlContext->ctx_parent = parent_context;
			}
			else if ((relNode = ExprNode::as<RelationSourceNode>(input)))
			{
				DEV_BLKCHK(relNode->dsqlContext, dsql_type_ctx);
				relNode->dsqlContext->ctx_parent = parent_context;
			}
			else if ((rseNode = ExprNode::as<RseNode>(input)))
				remap_streams_to_parent_context(rseNode->dsqlStreams, parent_context);
			else if ((unionNode = ExprNode::as<UnionSourceNode>(input)))
			{
				dsql_nod** rse = unionNode->dsqlClauses->nod_arg;
				for (const dsql_nod* const* const end = rse + unionNode->dsqlClauses->nod_count; rse != end; ++rse)
					remap_streams_to_parent_context(ExprNode::as<RseNode>(*rse)->dsqlStreams, parent_context);
			}
			else if (!ExprNode::as<AggregateSourceNode>(input))
			{
				fb_assert(false);
			}
		}
		break;
	}
}


/**

 	PASS1_set_parameter_type

    @brief	Setup the datatype of a parameter.


    @param in_node
    @param node
    @param force_varchar

 **/
bool PASS1_set_parameter_type(DsqlCompilerScratch* dsqlScratch, dsql_nod* in_node,
	dsql_nod* node, bool force_varchar)
{
	DEV_BLKCHK(in_node, dsql_type_nod);
	DEV_BLKCHK(node, dsql_type_nod);

	if (!in_node || in_node->nod_type != nod_class_exprnode)
		return false;

	ValueExprNode* exprNode = reinterpret_cast<ValueExprNode*>(in_node->nod_arg[0]);
	if (exprNode->kind == DmlNode::KIND_VALUE)
		return exprNode->setParameterType(dsqlScratch, node, force_varchar);

	return false;
}


// Returns false for hidden fields and true for non-hidden.
// For non-hidden, change "node" if the field is part of an
// implicit join.
bool dsql_ctx::getImplicitJoinField(const Firebird::MetaName& name, dsql_nod*& node)
{
	ImplicitJoin* impJoin;
	if (ctx_imp_join.get(name, impJoin))
	{
		if (impJoin->visibleInContext == this)
		{
			node = impJoin->value;
			return true;
		}

		return false;
	}

	return true;
}

// Returns (creating, if necessary) the PartitionMap of a given partition (that may be NULL).
PartitionMap* dsql_ctx::getPartitionMap(DsqlCompilerScratch* dsqlScratch, dsql_nod* partitionNode,
	dsql_nod* orderNode)
{
	thread_db* tdbb = JRD_get_thread_data();

	PartitionMap* partitionMap = NULL;

	for (Array<PartitionMap*>::iterator i = ctx_win_maps.begin();
		 !partitionMap && i != ctx_win_maps.end();
		 ++i)
	{
		if (PASS1_node_match((*i)->partition, partitionNode, false) &&
			PASS1_node_match((*i)->order, orderNode, false))
		{
			partitionMap = *i;
		}
	}

	if (!partitionMap)
	{
		partitionMap = FB_NEW(*tdbb->getDefaultPool()) PartitionMap(partitionNode, orderNode);
		ctx_win_maps.add(partitionMap);
		partitionMap->context = dsqlScratch->contextNumber++;
	}

	return partitionMap;
}

#ifdef DSQL_DEBUG

#include <stdarg.h>

static void trace_line(const char* message, ...)
{
	char buffer[1024];
	va_list params;
	va_start(params, message);
	VSNPRINTF(buffer, sizeof(buffer), message, params);
	va_end(params);
	buffer[sizeof(buffer) - 1] = 0;
	gds__trace_raw(buffer);
}

/**

 	DSQL_pretty

    @brief	Pretty print a node tree.


    @param node
    @param column

 **/
void DSQL_pretty(const dsql_nod* node, int column)
{
	TEXT buffer[1024];

	TEXT* p = buffer;
	p += sprintf(p, "%p ", (void*) node);

	if (column)
	{
		USHORT l = column * 3;
		do {
			*p++ = ' ';
		} while (--l);
	}

	*p = 0;

	if (!node)
	{
		trace_line("%s *** null ***\n", buffer);
		return;
	}

	switch (node->getType())
	{
	case dsql_type_str:
		trace_line("%sSTRING: \"%s\"\n", buffer, ((dsql_str*) node)->str_data);
		return;

	case dsql_type_fld:
		trace_line("%sFIELD: %s\n", buffer, ((dsql_fld*) node)->fld_name.c_str());
		return;

	case dsql_type_nod:
		break;

	default:
		trace_line("%sUNKNOWN BLOCK TYPE\n", buffer);
		return;
	}

	TEXT s[64];
	const dsql_str* string;
	Firebird::string verb;
	const dsql_nod* const* ptr = node->nod_arg;
	const dsql_nod* const* end = ptr + node->nod_count;
	Firebird::Array<dsql_nod*> subNodes;

	switch (node->nod_type)
	{
	case nod_all:
		verb = "all";
		break;
	case nod_delete:
		verb = "delete";
		break;
	case nod_execute:
		verb = "execute";
		break;
	case nod_flag:
		verb = "flag";
		break;
	case nod_insert:
		verb = "insert";
		break;
	case nod_list:
		verb = "list";
		break;
	case nod_procedure_name:
		verb = "procedure name";
		break;
	case nod_package_name:
		verb = "package name";
		break;
	case nod_function_name:
		verb = "function_name";
		break;
	case nod_select:
		verb = "select";
		break;
	case nod_update:
		verb = "update";
		break;

	// IOL: missing node types
	case nod_plan_expr:
		verb = "plan";
		break;
	case nod_index:
		verb = "index";
		break;
	case nod_index_order:
		verb = "order";
		break;
	case nod_plan_item:
		verb = "item";
		break;
	case nod_natural:
		verb = "natural";
		break;
	// SKIDDER: some more missing node types
	case nod_references:
		verb = "references";
		break;
	case nod_proc_obj:
		verb = "proc_obj";
		break;
	case nod_func_obj:
		verb = "func_obj";
		break;
	case nod_trig_obj:
		verb = "trig_obj";
		break;
	case nod_view_obj:
		verb = "view_obj";
		break;
	case nod_user_group:
		verb = "user_group";
		break;
	case nod_table_lock:
		verb = "table_lock";
		break;
	case nod_lock_mode:
		verb = "lock_mode";
		break;
	case nod_role_name:
		verb = "role_name";
		break;

	case nod_label:
		verb = "label";
		DSQL_pretty(node->nod_arg[e_label_name], column + 1);
		trace_line("%s   number %d\n", buffer,
			(int)(IPTR)node->nod_arg[e_label_number]);
		return;

	case nod_class_exprnode:
		reinterpret_cast<Node*>(node->nod_arg[0])->print(verb, subNodes);
		ptr = subNodes.begin();
		end = subNodes.end();
		break;

	default:
		sprintf(s, "unknown type %d", node->nod_type);
		verb = s;
	}

	if (node->nod_desc.dsc_dtype)
	{
		trace_line("%s%s (%d,%d,%p)\n",
				buffer, verb.c_str(),
				node->nod_desc.dsc_dtype, node->nod_desc.dsc_length, node->nod_desc.dsc_address);
	}
	else
		trace_line("%s%s\n", buffer, verb.c_str());

	++column;

	while (ptr < end)
		DSQL_pretty(*ptr++, column);

	return;
}
#endif
