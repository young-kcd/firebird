/*
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
 * Adriano dos Santos Fernandes
 */

#include "firebird.h"
#include "../jrd/common.h"
#include "../jrd/align.h"
#include "../jrd/RecordSourceNodes.h"
#include "../jrd/DataTypeUtil.h"
#include "../jrd/Optimizer.h"
#include "../jrd/recsrc/RecordSource.h"
#include "../jrd/btr_proto.h"
#include "../jrd/cmp_proto.h"
#include "../jrd/dbg_proto.h"
#include "../jrd/dsc_proto.h"
#include "../jrd/met_proto.h"
#include "../jrd/opt_proto.h"
#include "../jrd/par_proto.h"

using namespace Firebird;
using namespace Jrd;


static jrd_nod* parseMap(thread_db* tdbb, CompilerScratch* csb, USHORT stream);
static SSHORT strcmpSpace(const char* p, const char* q);
static void processSource(thread_db* tdbb, CompilerScratch* csb, RseNode* rse,
	RecordSourceNode* source, jrd_nod** boolean, RecordSourceNodeStack& stack);
static void processMap(thread_db* tdbb, CompilerScratch* csb, jrd_nod* map, Format** input_format);
static void genDeliverUnmapped(thread_db* tdbb, NodeStack* deliverStack, jrd_nod* map,
	NodeStack* parentStack, UCHAR shellStream);
static void markIndices(CompilerScratch::csb_repeat* csb_tail, SSHORT relation_id);
static void sortIndicesBySelectivity(CompilerScratch::csb_repeat* csb_tail);


//--------------------


// Parse a relation reference.
RelationSourceNode* RelationSourceNode::parse(thread_db* tdbb, CompilerScratch* csb,
	SSHORT blrOp, bool parseContext)
{
	SET_TDBB(tdbb);

	// Make a relation reference node

	RelationSourceNode* node = FB_NEW(*tdbb->getDefaultPool()) RelationSourceNode(
		*tdbb->getDefaultPool());

	// Find relation either by id or by name
	string* alias_string = NULL;
	MetaName name;

	switch (blrOp)
	{
		case blr_rid:
		case blr_rid2:
		{
			const SSHORT id = csb->csb_blr_reader.getWord();

			if (blrOp == blr_rid2)
			{
				alias_string = FB_NEW(csb->csb_pool) string(csb->csb_pool);
				PAR_name(csb, *alias_string);
			}

			if (!(node->relation = MET_lookup_relation_id(tdbb, id, false)))
				name.printf("id %d", id);

			break;
		}

		case blr_relation:
		case blr_relation2:
		{
			PAR_name(csb, name);

			if (blrOp == blr_relation2)
			{
				alias_string = FB_NEW(csb->csb_pool) string(csb->csb_pool);
				PAR_name(csb, *alias_string);
			}

			node->relation = MET_lookup_relation(tdbb, name);
			break;
		}

		default:
			fb_assert(false);
	}

	if (!node->relation)
		PAR_error(csb, Arg::Gds(isc_relnotdef) << Arg::Str(name), false);

	// if an alias was passed, store with the relation

	if (alias_string)
		node->alias = stringDup(*tdbb->getDefaultPool(), *alias_string);

	// Scan the relation if it hasn't already been scanned for meta data

	if ((!(node->relation->rel_flags & REL_scanned) || (node->relation->rel_flags & REL_being_scanned)) &&
		((node->relation->rel_flags & REL_force_scan) || !(csb->csb_g_flags & csb_internal)))
	{
		node->relation->rel_flags &= ~REL_force_scan;
		MET_scan_relation(tdbb, node->relation);
	}
	else if (node->relation->rel_flags & REL_sys_triggers)
		MET_parse_sys_trigger(tdbb, node->relation);

	// generate a stream for the relation reference, assuming it is a real reference

	if (parseContext)
	{
		node->stream = PAR_context(csb, &node->context);
		fb_assert(node->stream <= MAX_STREAMS);

		csb->csb_rpt[node->stream].csb_relation = node->relation;
		csb->csb_rpt[node->stream].csb_alias = alias_string;

		if (csb->csb_g_flags & csb_get_dependencies)
			PAR_dependency(tdbb, csb, node->stream, (SSHORT) -1, "");
	}
	else
		delete alias_string;

	return node;
}

RelationSourceNode* RelationSourceNode::copy(thread_db* tdbb, NodeCopier& copier)
{
	if (!copier.remap)
		BUGCHECK(221);	// msg 221 (CMP) copy: cannot remap

	RelationSourceNode* newSource = FB_NEW(*tdbb->getDefaultPool()) RelationSourceNode(
		*tdbb->getDefaultPool());

	// Last entry in the remap contains the the original stream number.
	// Get that stream number so that the flags can be copied
	// into the newly created child stream.

	const int relative_stream = stream ? copier.remap[stream - 1] : stream;
	newSource->stream = copier.csb->nextStream();
	copier.remap[stream] = (UCHAR) newSource->stream;

	newSource->context = context;
	newSource->relation = relation;
	newSource->view = view;

	CompilerScratch::csb_repeat* element = CMP_csb_element(copier.csb, newSource->stream);
	element->csb_relation = (jrd_rel*) newSource->relation;
	element->csb_view = newSource->view;
	element->csb_view_stream = copier.remap[0];

	/** If there was a parent stream no., then copy the flags
		from that stream to its children streams. (Bug 10164/10166)
		For e.g.
		consider a view V1 with 2 streams
				stream #1 from table T1
			stream #2 from table T2
		consider a procedure P1 with 2 streams
				stream #1  from table X
			stream #2  from view V1

		During pass1 of procedure request, the engine tries to expand
		all the views into their base tables. It creates a compilier
		scratch block which initially looks like this
				stream 1  -------- X
				stream 2  -------- V1
			while expanding V1 the engine calls copy() with nod_relation.
			A new stream 3 is created. Now the CompilerScratch looks like
				stream 1  -------- X
				stream 2  -------- V1  map [2,3]
				stream 3  -------- T1
			After T1 stream has been created the flags are copied from
			stream #1 because V1's definition said the original stream
			number for T1 was 1. However since its being merged with
			the procedure request, stream #1 belongs to a different table.
			The flags should be copied from stream 2 i.e. V1. We can get
			this info from variable remap.

			Since we didn't do this properly before, V1's children got
			tagged with whatever flags X possesed leading to various
			errors.

			We now store the proper stream no in relative_stream and
			later use it to copy the flags. -Sudesh (03/05/99)
	**/

	copier.csb->csb_rpt[newSource->stream].csb_flags |=
		copier.csb->csb_rpt[relative_stream].csb_flags & csb_no_dbkey;

	return newSource;
}

void RelationSourceNode::ignoreDbKey(thread_db* tdbb, CompilerScratch* csb, const jrd_rel* view) const
{
	csb->csb_rpt[stream].csb_flags |= csb_no_dbkey;
	const CompilerScratch::csb_repeat* tail = &csb->csb_rpt[stream];
	const jrd_rel* relation = tail->csb_relation;

	if (relation)
	{
		CMP_post_access(tdbb, csb, relation->rel_security_name,
			(tail->csb_view) ? tail->csb_view->rel_id : (view ? view->rel_id : 0),
			SCL_read, SCL_object_table, relation->rel_name);
	}
}

void RelationSourceNode::pass1Source(thread_db* tdbb, CompilerScratch* csb, RseNode* rse,
	jrd_nod** boolean, RecordSourceNodeStack& stack)
{
	stack.push(this);	// Assume that the source will be used. Push it on the final stream stack.

	// We have a view or a base table;
	// prepare to check protection of relation when a field in the stream of the
	// relation is accessed.

	jrd_rel* const parent_view = csb->csb_view;
	const USHORT view_stream = csb->csb_view_stream;

	jrd_rel* relationView = relation;
	CMP_post_resource(&csb->csb_resources, relationView, Resource::rsc_relation, relationView->rel_id);
	view = parent_view;

	CompilerScratch::csb_repeat* const element = CMP_csb_element(csb, stream);
	element->csb_view = parent_view;
	fb_assert(view_stream <= MAX_STREAMS);
	element->csb_view_stream = (UCHAR) view_stream;

	// in the case where there is a parent view, find the context name

	if (parent_view)
	{
		const ViewContexts& ctx = parent_view->rel_view_contexts;
		const USHORT key = context;
		size_t pos;

		if (ctx.find(key, pos))
		{
			element->csb_alias = FB_NEW(csb->csb_pool)
				string(csb->csb_pool, ctx[pos]->vcx_context_name);
		}
	}

	// check for a view - if not, nothing more to do

	RseNode* view_rse = relationView->rel_view_rse;
	if (!view_rse)
		return;

	// we've got a view, expand it

	DEBUG;
	stack.pop();
	UCHAR* map = CMP_alloc_map(tdbb, csb, stream);

	AutoSetRestore<USHORT> autoRemapVariable(&csb->csb_remap_variable,
		(csb->csb_variables ? csb->csb_variables->count() : 0) + 1);
	AutoSetRestore<jrd_rel*> autoView(&csb->csb_view, relationView);
	AutoSetRestore<USHORT> autoViewStream(&csb->csb_view_stream, stream);

	// We don't expand the view in two cases:
	// 1) If the view has a projection, sort, first/skip or explicit plan.
	// 2) If it's part of an outer join.

	if (rse->rse_jointype || // view_rse->rse_jointype || ???
		view_rse->rse_sorted || view_rse->rse_projection || view_rse->rse_first ||
		view_rse->rse_skip || view_rse->rse_plan)
	{
		NodeCopier copier(csb, map);
		RseNode* copy = view_rse->copy(tdbb, copier);
		DEBUG;
		copy->pass1(tdbb, csb, csb->csb_view);
		stack.push(copy);
		DEBUG;
		return;
	}

	// if we have a projection which we can bubble up to the parent rse, set the
	// parent rse to our projection temporarily to flag the fact that we have already
	// seen one so that lower-level views will not try to map their projection; the
	// projection will be copied and correctly mapped later, but we don't have all
	// the base streams yet

	if (view_rse->rse_projection)
		rse->rse_projection = view_rse->rse_projection;

	// disect view into component relations

	NestConst<RecordSourceNode>* arg = view_rse->rse_relations.begin();
	for (const NestConst<RecordSourceNode>* const end = view_rse->rse_relations.end(); arg != end; ++arg)
	{
		// this call not only copies the node, it adds any streams it finds to the map
		NodeCopier copier(csb, map);
		RecordSourceNode* node = (*arg)->copy(tdbb, copier);

		// Now go out and process the base table itself. This table might also be a view,
		// in which case we will continue the process by recursion.
		processSource(tdbb, csb, rse, node, boolean, stack);
	}

	// When there is a projection in the view, copy the projection up to the query RseNode.
	// In order to make this work properly, we must remap the stream numbers of the fields
	// in the view to the stream number of the base table. Note that the map at this point
	// contains the stream numbers of the referenced relations, since it was added during the call
	// to copy() above. After the copy() below, the fields in the projection will reference the
	// base table(s) instead of the view's context (see bug #8822), so we are ready to context-
	// recognize them in CMP_pass1() - that is, replace the field nodes with actual field blocks.

	if (view_rse->rse_projection)
	{
		rse->rse_projection =
			CMP_pass1(tdbb, csb, NodeCopier::copy(tdbb, csb, view_rse->rse_projection, map));
	}

	// if we encounter a boolean, copy it and retain it by ANDing it in with the
	// boolean on the parent view, if any

	if (view_rse->rse_boolean)
	{
		jrd_nod* node = CMP_pass1(tdbb, csb, NodeCopier::copy(tdbb,
			csb, view_rse->rse_boolean, map));

		if (*boolean)
		{
			// The order of the nodes here is important! The
			// boolean from the view must appear first so that
			// it gets expanded first in pass1.

			jrd_nod* additional = PAR_make_node(tdbb, 2);
			additional->nod_type = nod_and;
			additional->nod_arg[0] = node;
			additional->nod_arg[1] = *boolean;
			*boolean = additional;
		}
		else
			*boolean = node;
	}
}

void RelationSourceNode::pass2Rse(thread_db* tdbb, CompilerScratch* csb)
{
	fb_assert(stream <= MAX_STREAMS);
	csb->csb_rpt[stream].csb_flags |= csb_active;

	pass2(tdbb, csb);
}

RecordSource* RelationSourceNode::compile(thread_db* tdbb, CompilerScratch* csb, OptimizerBlk* opt,
	RseNode* rse, NodeStack* parent_stack, stream_array_t& beds, stream_array_t& key_streams,
	stream_array_t& local_streams, NodeStack& conjunct_stack, stream_array_t& streams,
	jrd_nod* sort, jrd_nod* aggregate, StreamsArray& outerStreams, SLONG conjunct_count,
	bool innerSubStream)
{
	fb_assert(stream <= MAX_UCHAR);
	fb_assert(beds[0] < MAX_STREAMS && beds[0] < MAX_UCHAR); // debug check
	//if (beds[0] >= MAX_STREAMS) // all builds check
	//	ERR_post(Arg::Gds(isc_too_many_contexts));

	beds[++beds[0]] = (UCHAR) stream;

	// we have found a base relation; record its stream
	// number in the streams array as a candidate for
	// merging into a river

	// TMN: Is the intention really to allow streams[0] to overflow?
	// I must assume that is indeed not the intention (not to mention
	// it would make code later on fail), so I added the following fb_assert.
	fb_assert(streams[0] < MAX_STREAMS && streams[0] < MAX_UCHAR);

	streams[++streams[0]] = (UCHAR) stream;

	if (rse->rse_jointype == blr_left)
		outerStreams.add(stream);

	// if we have seen any booleans or sort fields, we may be able to
	// use an index to optimize them; retrieve the current format of
	// all indices at this time so we can determine if it's possible
	// AB: if a parent_stack was available and conjunct_count was 0
	// then no indices where retrieved. Added also OR check on
	// parent_stack below. SF BUG # [ 508594 ]

	if (conjunct_count || sort || aggregate || parent_stack)
	{
		jrd_rel* relation = (jrd_rel*) this->relation;

		if (relation && !relation->rel_file && !relation->isVirtual())
		{
			csb->csb_rpt[stream].csb_indices =
				BTR_all(tdbb, relation, &csb->csb_rpt[stream].csb_idx, relation->getPages(tdbb));
			sortIndicesBySelectivity(&csb->csb_rpt[stream]);
			markIndices(&csb->csb_rpt[stream], relation->rel_id);
		}
		else
			csb->csb_rpt[stream].csb_indices = 0;

		const Format* format = CMP_format(tdbb, csb, stream);
		csb->csb_rpt[stream].csb_cardinality = OPT_getRelationCardinality(tdbb, relation, format);
	}

	return NULL;
}


//--------------------


// Parse an procedural view reference.
ProcedureSourceNode* ProcedureSourceNode::parse(thread_db* tdbb, CompilerScratch* csb,
	SSHORT blrOp)
{
	SET_TDBB(tdbb);

	jrd_prc* procedure = NULL;
	string* alias_string = NULL;
	QualifiedName name;

	switch (blrOp)
	{
		case blr_pid:
		case blr_pid2:
		{
			const SSHORT pid = csb->csb_blr_reader.getWord();

			if (blrOp == blr_pid2)
			{
				alias_string = FB_NEW(csb->csb_pool) string(csb->csb_pool);
				PAR_name(csb, *alias_string);
			}

			if (!(procedure = MET_lookup_procedure_id(tdbb, pid, false, false, 0)))
				name.identifier.printf("id %d", pid);

			break;
		}

		case blr_procedure:
		case blr_procedure2:
		case blr_procedure3:
		case blr_procedure4:
		{
			if (blrOp == blr_procedure3 || blrOp == blr_procedure4)
				PAR_name(csb, name.package);

			PAR_name(csb, name.identifier);

			if (blrOp == blr_procedure2 || blrOp == blr_procedure4)
			{
				alias_string = FB_NEW(csb->csb_pool) string(csb->csb_pool);
				PAR_name(csb, *alias_string);
			}

			procedure = MET_lookup_procedure(tdbb, name, false);

			break;
		}

		default:
			fb_assert(false);
	}

	if (!procedure)
		PAR_error(csb, Arg::Gds(isc_prcnotdef) << Arg::Str(name.toString()));

	if (procedure->prc_type == prc_executable)
	{
		const string name = procedure->getName().toString();

		if (tdbb->getAttachment()->att_flags & ATT_gbak_attachment)
			PAR_warning(Arg::Warning(isc_illegal_prc_type) << Arg::Str(name));
		else
			PAR_error(csb, Arg::Gds(isc_illegal_prc_type) << Arg::Str(name));
	}

	ProcedureSourceNode* node = FB_NEW(*tdbb->getDefaultPool()) ProcedureSourceNode(
		*tdbb->getDefaultPool());

	node->procedure = procedure->getId();
	node->stream = PAR_context(csb, &node->context);

	csb->csb_rpt[node->stream].csb_procedure = procedure;
	csb->csb_rpt[node->stream].csb_alias = alias_string;

	PAR_procedure_parms(tdbb, csb, procedure, node->in_msg.getAddress(),
		node->inputs.getAddress(), true);

	if (csb->csb_g_flags & csb_get_dependencies)
		PAR_dependency(tdbb, csb, node->stream, (SSHORT) -1, "");

	return node;
}

ProcedureSourceNode* ProcedureSourceNode::copy(thread_db* tdbb, NodeCopier& copier)
{
	if (!copier.remap)
		BUGCHECK(221);	// msg 221 (CMP) copy: cannot remap

	ProcedureSourceNode* newSource = FB_NEW(*tdbb->getDefaultPool()) ProcedureSourceNode(
		*tdbb->getDefaultPool());

	// dimitr: See the appropriate code and comment in NodeCopier (in nod_argument).
	// We must copy the message first and only then use the new pointer to
	// copy the inputs properly.
	newSource->in_msg = copier.copy(tdbb, in_msg);

	{	// scope
		AutoSetRestore<jrd_nod*> autoMessage(&copier.message, newSource->in_msg);
		newSource->inputs = copier.copy(tdbb, inputs);
	}

	newSource->stream = copier.csb->nextStream();
	copier.remap[stream] = (UCHAR) newSource->stream;
	newSource->context = context;
	newSource->procedure = procedure;
	newSource->view = view;
	CompilerScratch::csb_repeat* element = CMP_csb_element(copier.csb, newSource->stream);
	// SKIDDER: Maybe we need to check if we really found a procedure?
	element->csb_procedure = MET_lookup_procedure_id(tdbb, newSource->procedure, false, false, 0);
	element->csb_view = (jrd_rel*) newSource->view;
	element->csb_view_stream = copier.remap[0];

	copier.csb->csb_rpt[newSource->stream].csb_flags |= copier.csb->csb_rpt[stream].csb_flags & csb_no_dbkey;

	return newSource;
}

void ProcedureSourceNode::pass1(thread_db* tdbb, CompilerScratch* csb, jrd_rel* view)
{
	inputs = CMP_pass1(tdbb, csb, inputs);
	in_msg = CMP_pass1(tdbb, csb, in_msg);
}

void ProcedureSourceNode::pass1Source(thread_db* tdbb, CompilerScratch* csb, RseNode* rse,
	jrd_nod** boolean, RecordSourceNodeStack& stack)
{
	stack.push(this);	// Assume that the source will be used. Push it on the final stream stack.

	pass1(tdbb, csb, csb->csb_view);

	jrd_prc* const proc = MET_lookup_procedure_id(tdbb, procedure, false, false, 0);
	CMP_post_procedure_access(tdbb, csb, proc);
	CMP_post_resource(&csb->csb_resources, proc, Resource::rsc_procedure, proc->getId());

	jrd_rel* const parent_view = csb->csb_view;
	const USHORT view_stream = csb->csb_view_stream;
	view = parent_view;

	CompilerScratch::csb_repeat* const element = CMP_csb_element(csb, stream);
	element->csb_view = parent_view;
	fb_assert(view_stream <= MAX_STREAMS);
	element->csb_view_stream = (UCHAR) view_stream;

	// in the case where there is a parent view, find the context name

	if (parent_view)
	{
		const ViewContexts& ctx = parent_view->rel_view_contexts;
		const USHORT key = context;
		size_t pos;

		if (ctx.find(key, pos))
		{
			element->csb_alias = FB_NEW(csb->csb_pool) string(
				csb->csb_pool, ctx[pos]->vcx_context_name);
		}
	}
}

void ProcedureSourceNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	CMP_pass2(tdbb, csb, inputs, NULL);
	CMP_pass2(tdbb, csb, in_msg, NULL);
}

void ProcedureSourceNode::pass2Rse(thread_db* tdbb, CompilerScratch* csb)
{
	fb_assert(stream <= MAX_STREAMS);
	csb->csb_rpt[stream].csb_flags |= csb_active;

	pass2(tdbb, csb);
}

RecordSource* ProcedureSourceNode::compile(thread_db* tdbb, CompilerScratch* csb, OptimizerBlk* opt,
	RseNode* rse, NodeStack* parent_stack, stream_array_t& beds, stream_array_t& key_streams,
	stream_array_t& local_streams, NodeStack& conjunct_stack, stream_array_t& streams,
	jrd_nod* sort, jrd_nod* aggregate, StreamsArray& outerStreams, SLONG conjunct_count,
	bool innerSubStream)
{
	fb_assert(stream <= MAX_UCHAR);
	fb_assert(beds[0] < MAX_STREAMS && beds[0] < MAX_UCHAR); // debug check
	//if (beds[0] >= MAX_STREAMS) // all builds check
	//	ERR_post(Arg::Gds(isc_too_many_contexts));

	beds[++beds[0]] = (UCHAR) stream;

	RecordSource* rsb = generate(tdbb, opt);
	fb_assert(local_streams[0] < MAX_STREAMS && local_streams[0] < MAX_UCHAR);
	local_streams[++local_streams[0]] = stream;

	return rsb;
}

// Compile and optimize a record selection expression into a set of record source blocks (rsb's).
ProcedureScan* ProcedureSourceNode::generate(thread_db* tdbb, OptimizerBlk* opt)
{
	DEV_BLKCHK(opt, type_opt);
	DEV_BLKCHK(node, type_nod);
	SET_TDBB(tdbb);

	jrd_prc* const proc = MET_lookup_procedure_id(tdbb, procedure, false, false, 0);

	CompilerScratch* const csb = opt->opt_csb;
	CompilerScratch::csb_repeat* const csb_tail = &csb->csb_rpt[stream];
	const string alias = OPT_make_alias(tdbb, csb, csb_tail);

	return FB_NEW(*tdbb->getDefaultPool()) ProcedureScan(csb, alias, stream, proc, inputs, in_msg);
}

bool ProcedureSourceNode::computable(CompilerScratch* csb, SSHORT stream, bool idx_use,
	bool allowOnlyCurrentStream, jrd_nod* value)
{
	if (inputs)
	{
		fb_assert(inputs->nod_type == nod_asn_list);
		jrd_nod* const* ptr = inputs->nod_arg;

		for (const jrd_nod* const* const end = ptr + inputs->nod_count; ptr < end; ptr++)
		{
			if (!OPT_computable(csb, *ptr, stream, idx_use, allowOnlyCurrentStream))
				return false;
		}
	}

	return true;
}

void ProcedureSourceNode::findDependentFromStreams(const OptimizerRetrieval* optRet,
	SortedStreamList* streamList)
{
	if (inputs)
	{
		fb_assert(inputs->nod_type == nod_asn_list);
		jrd_nod* const* ptr = inputs->nod_arg;

		for (const jrd_nod* const* const end = ptr + inputs->nod_count; ptr < end; ptr++)
			optRet->findDependentFromStreams(*ptr, streamList);
	}
}


//--------------------


// Parse an aggregate reference.
AggregateSourceNode* AggregateSourceNode::parse(thread_db* tdbb, CompilerScratch* csb)
{
	SET_TDBB(tdbb);

	AggregateSourceNode* node = FB_NEW(*tdbb->getDefaultPool()) AggregateSourceNode(
		*tdbb->getDefaultPool());

	node->stream = PAR_context(csb, NULL);
	fb_assert(node->stream <= MAX_STREAMS);
	node->rse = RseNode::getFrom(PAR_parse_node(tdbb, csb, TYPE_RSE));
	node->group = PAR_parse_node(tdbb, csb, OTHER);
	node->map = parseMap(tdbb, csb, node->stream);

	return node;
}

AggregateSourceNode* AggregateSourceNode::copy(thread_db* tdbb, NodeCopier& copier)
{
	if (!copier.remap)
		BUGCHECK(221);	// msg 221 (CMP) copy: cannot remap

	AggregateSourceNode* newSource = FB_NEW(*tdbb->getDefaultPool()) AggregateSourceNode(
		*tdbb->getDefaultPool());

	fb_assert(stream <= MAX_STREAMS);
	newSource->stream = copier.csb->nextStream();
	// fb_assert(newSource->stream <= MAX_UCHAR);
	copier.remap[stream] = (UCHAR) newSource->stream;
	CMP_csb_element(copier.csb, newSource->stream);

	copier.csb->csb_rpt[newSource->stream].csb_flags |=
		copier.csb->csb_rpt[stream].csb_flags & csb_no_dbkey;

	newSource->rse = rse->copy(tdbb, copier);
	newSource->group = copier.copy(tdbb, group);
	newSource->map = copier.copy(tdbb, map);

	return newSource;
}

void AggregateSourceNode::ignoreDbKey(thread_db* tdbb, CompilerScratch* csb, const jrd_rel* view) const
{
	rse->ignoreDbKey(tdbb, csb, view);
}

void AggregateSourceNode::pass1(thread_db* tdbb, CompilerScratch* csb, jrd_rel* view)
{
	fb_assert(stream <= MAX_STREAMS);
	csb->csb_rpt[stream].csb_flags |= csb_no_dbkey;
	rse->ignoreDbKey(tdbb, csb, view);
	rse->pass1(tdbb, csb, csb->csb_view);
	map = CMP_pass1(tdbb, csb, map);
	group = CMP_pass1(tdbb, csb, group);
}

void AggregateSourceNode::pass1Source(thread_db* tdbb, CompilerScratch* csb, RseNode* rse,
	jrd_nod** boolean, RecordSourceNodeStack& stack)
{
	stack.push(this);	// Assume that the source will be used. Push it on the final stream stack.

	fb_assert(stream <= MAX_STREAMS);
	pass1(tdbb, csb, csb->csb_view);
}

void AggregateSourceNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	rse->pass2Rse(tdbb, csb);
	CMP_pass2(tdbb, csb, map, NULL);
	CMP_pass2(tdbb, csb, group, NULL);

	fb_assert(stream <= MAX_STREAMS);

	if (map)
	{
		processMap(tdbb, csb, map, &csb->csb_rpt[stream].csb_internal_format);
		csb->csb_rpt[stream].csb_format = csb->csb_rpt[stream].csb_internal_format;
	}
}

void AggregateSourceNode::pass2Rse(thread_db* tdbb, CompilerScratch* csb)
{
	fb_assert(stream <= MAX_STREAMS);
	csb->csb_rpt[stream].csb_flags |= csb_active;

	pass2(tdbb, csb);
}

bool AggregateSourceNode::containsStream(USHORT checkStream) const
{
	// for aggregates, check current RseNode, if not found then check
	// the sub-rse

	if (checkStream == stream)
		return true;		// do not mark as variant

	if (rse->containsStream(checkStream))
		return true;		// do not mark as variant

	return false;
}

RecordSource* AggregateSourceNode::compile(thread_db* tdbb, CompilerScratch* csb, OptimizerBlk* opt,
	RseNode* rse, NodeStack* parent_stack, stream_array_t& beds, stream_array_t& key_streams,
	stream_array_t& local_streams, NodeStack& conjunct_stack, stream_array_t& streams,
	jrd_nod* sort, jrd_nod* aggregate, StreamsArray& outerStreams, SLONG conjunct_count,
	bool innerSubStream)
{
	fb_assert(stream <= MAX_UCHAR);
	fb_assert(beds[0] < MAX_STREAMS && beds[0] < MAX_UCHAR); // debug check
	//if (beds[0] >= MAX_STREAMS) // all builds check
	//	ERR_post(Arg::Gds(isc_too_many_contexts));

	beds[++beds[0]] = (UCHAR) stream;

	NodeStack::const_iterator stack_end;
	if (parent_stack)
		stack_end = conjunct_stack.merge(*parent_stack);

	RecordSource* rsb = generate(tdbb, opt, &conjunct_stack, stream);

	if (parent_stack)
		conjunct_stack.split(stack_end, *parent_stack);

	fb_assert(local_streams[0] < MAX_STREAMS && local_streams[0] < MAX_UCHAR);
	local_streams[++local_streams[0]] = stream;

	return rsb;
}

// Generate an RecordSource (Record Source Block) for each aggregate operation.
// Generate an AggregateSort (Aggregate SortedStream Block) for each DISTINCT aggregate.
RecordSource* AggregateSourceNode::generate(thread_db* tdbb, OptimizerBlk* opt,
	NodeStack* parent_stack, UCHAR shellStream)
{
	DEV_BLKCHK(opt, type_opt);
	DEV_BLKCHK(node, type_nod);
	SET_TDBB(tdbb);

	CompilerScratch* const csb = opt->opt_csb;
	rse->rse_sorted = group;

	// AB: Try to distribute items from the HAVING CLAUSE to the WHERE CLAUSE.
	// Zip thru stack of booleans looking for fields that belong to shellStream.
	// Those fields are mappings. Mappings that hold a plain field may be used
	// to distribute. Handle the simple cases only.
	NodeStack deliverStack;
	genDeliverUnmapped(tdbb, &deliverStack, map, parent_stack, shellStream);

	// try to optimize MAX and MIN to use an index; for now, optimize
	// only the simplest case, although it is probably possible
	// to use an index in more complex situations
	jrd_nod** ptr;
	AggNode* aggNode = NULL;

	if (map && map->nod_count == 1 && (ptr = map->nod_arg) &&
		(aggNode = ExprNode::as<AggNode>((*ptr)->nod_arg[e_asgn_from])) &&
		(aggNode->aggInfo.blr == blr_agg_min || aggNode->aggInfo.blr == blr_agg_max))
	{
		// generate a sort block which the optimizer will try to map to an index

		jrd_nod* aggregate = PAR_make_node(tdbb, 3);
		aggregate->nod_type = nod_sort;
		aggregate->nod_count = 1;
		aggregate->nod_arg[0] = aggNode->arg;
		// in the max case, flag the sort as descending
		if (aggNode->aggInfo.blr == blr_agg_max)
			aggregate->nod_arg[1] = (jrd_nod*) TRUE;
		// 10-Aug-2004. Nickolay Samofatov - Unneeded nulls seem to be skipped somehow.
		aggregate->nod_arg[2] = (jrd_nod*)(IPTR) rse_nulls_default;
		rse->rse_aggregate = aggregate;
	}

	RecordSource* const next_rsb = OPT_compile(tdbb, csb, rse, &deliverStack);

	fb_assert(stream <= MAX_STREAMS);
	fb_assert(stream <= MAX_UCHAR);

	// allocate and optimize the record source block

	AggregatedStream* const rsb = FB_NEW(*tdbb->getDefaultPool()) AggregatedStream(csb, stream,
		group, map, next_rsb);

	if (rse->rse_aggregate)
	{
		// The rse_aggregate is still set. That means the optimizer
		// was able to match the field to an index, so flag that fact
		// so that it can be handled in EVL_group
		aggNode->indexed = true;
	}

	OPT_gen_aggregate_distincts(tdbb, csb, map);

	return rsb;
}

bool AggregateSourceNode::computable(CompilerScratch* csb, SSHORT stream, bool idx_use,
	bool allowOnlyCurrentStream, jrd_nod* value)
{
	rse->rse_sorted = group;
	return rse->computable(csb, stream, idx_use, allowOnlyCurrentStream, NULL);
}

void AggregateSourceNode::findDependentFromStreams(const OptimizerRetrieval* optRet,
	SortedStreamList* streamList)
{
	rse->rse_sorted = group;
	rse->findDependentFromStreams(optRet, streamList);
}


//--------------------


// Parse a union reference.
UnionSourceNode* UnionSourceNode::parse(thread_db* tdbb, CompilerScratch* csb, SSHORT blrOp)
{
	SET_TDBB(tdbb);

	// Make the node, parse the context number, get a stream assigned,
	// and get the number of sub-RseNode's.

	UnionSourceNode* node = FB_NEW(*tdbb->getDefaultPool()) UnionSourceNode(
		*tdbb->getDefaultPool());

	node->recursive = blrOp == blr_recurse;

	node->stream = PAR_context(csb, NULL);
	fb_assert(node->stream <= MAX_STREAMS);

	// assign separate context for mapped record if union is recursive
	USHORT map_stream = node->stream;

	if (node->recursive)
	{
		map_stream = PAR_context(csb, 0);
		node->mapStream = map_stream;
	}

	SSHORT count = (unsigned int) csb->csb_blr_reader.getByte();

	// Pick up the sub-RseNode's and maps.

	while (--count >= 0)
	{
		node->clauses.push(RseNode::getFrom(PAR_parse_node(tdbb, csb, TYPE_RSE)));
		node->maps.push(parseMap(tdbb, csb, map_stream));
	}

	return node;
}

UnionSourceNode* UnionSourceNode::copy(thread_db* tdbb, NodeCopier& copier)
{
	if (!copier.remap)
		BUGCHECK(221);		// msg 221 (CMP) copy: cannot remap

	UnionSourceNode* newSource = FB_NEW(*tdbb->getDefaultPool()) UnionSourceNode(
		*tdbb->getDefaultPool());
	newSource->recursive = recursive;

	fb_assert(stream <= MAX_STREAMS);
	newSource->stream = copier.csb->nextStream();
	copier.remap[stream] = (UCHAR) newSource->stream;
	CMP_csb_element(copier.csb, newSource->stream);

	USHORT oldStream = stream;
	USHORT newStream = newSource->stream;

	if (newSource->recursive)
	{
		oldStream = mapStream;
		fb_assert(oldStream <= MAX_STREAMS);
		newStream = copier.csb->nextStream();
		newSource->mapStream = newStream;
		copier.remap[oldStream] = (UCHAR) newStream;
		CMP_csb_element(copier.csb, newStream);
	}

	copier.csb->csb_rpt[newStream].csb_flags |=
		copier.csb->csb_rpt[oldStream].csb_flags & csb_no_dbkey;

	NestConst<RseNode>* ptr = clauses.begin();
	NestConst<jrd_nod>* ptr2 = maps.begin();

	for (NestConst<RseNode>* const end = clauses.end(); ptr != end; ++ptr, ++ptr2)
	{
		newSource->clauses.add((*ptr)->copy(tdbb, copier));
		newSource->maps.add(copier.copy(tdbb, *ptr2));
	}

	return newSource;
}

void UnionSourceNode::ignoreDbKey(thread_db* tdbb, CompilerScratch* csb, const jrd_rel* view) const
{
	const NestConst<RseNode>* ptr = clauses.begin();

	for (const NestConst<RseNode>* const end = clauses.end(); ptr != end; ++ptr)
		(*ptr)->ignoreDbKey(tdbb, csb, view);
}

void UnionSourceNode::pass1Source(thread_db* tdbb, CompilerScratch* csb, RseNode* rse,
	jrd_nod** boolean, RecordSourceNodeStack& stack)
{
	stack.push(this);	// Assume that the source will be used. Push it on the final stream stack.

	NestConst<RseNode>* ptr = clauses.begin();
	NestConst<jrd_nod>* ptr2 = maps.begin();

	for (NestConst<RseNode>* const end = clauses.end(); ptr != end; ++ptr, ++ptr2)
	{
		(*ptr)->pass1(tdbb, csb, csb->csb_view);
		*ptr2 = CMP_pass1(tdbb, csb, *ptr2);
	}
}

// Process a union clause of an RseNode.
void UnionSourceNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	SET_TDBB(tdbb);
	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(node, type_nod);

	// make up a format block sufficiently large to hold instantiated record

	const USHORT id = getStream();
	Format** format = &csb->csb_rpt[id].csb_internal_format;

	// Process RseNodes and map blocks.

	NestConst<RseNode>* ptr = clauses.begin();
	NestConst<jrd_nod>* ptr2 = maps.begin();

	for (NestConst<RseNode>* const end = clauses.end(); ptr != end; ++ptr, ++ptr2)
	{
		(*ptr)->pass2Rse(tdbb, csb);

		CMP_pass2(tdbb, csb, *ptr2, NULL);
		processMap(tdbb, csb, *ptr2, format);
		csb->csb_rpt[id].csb_format = *format;
	}

	if (recursive)
		csb->csb_rpt[mapStream].csb_format = *format;
}

void UnionSourceNode::pass2Rse(thread_db* tdbb, CompilerScratch* csb)
{
	fb_assert(stream <= MAX_STREAMS);
	csb->csb_rpt[stream].csb_flags |= csb_active;

	pass2(tdbb, csb);
}

bool UnionSourceNode::containsStream(USHORT checkStream) const
{
	// for unions, check current RseNode, if not found then check
	// all sub-rse's

	if (checkStream == stream)
		return true;		// do not mark as variant

	const NestConst<RseNode>* ptr = clauses.begin();

	for (const NestConst<RseNode>* const end = clauses.end(); ptr != end; ++ptr)
	{
		if ((*ptr)->containsStream(checkStream))
			return true;
	}

	return false;
}

RecordSource* UnionSourceNode::compile(thread_db* tdbb, CompilerScratch* csb, OptimizerBlk* opt,
	RseNode* rse, NodeStack* parent_stack, stream_array_t& beds, stream_array_t& key_streams,
	stream_array_t& local_streams, NodeStack& conjunct_stack, stream_array_t& streams,
	jrd_nod* sort, jrd_nod* aggregate, StreamsArray& outerStreams, SLONG conjunct_count,
	bool innerSubStream)
{
	const SSHORT i = (SSHORT) key_streams[0];
	computeDbKeyStreams(key_streams);

	NodeStack::const_iterator stack_end;
	if (parent_stack)
		stack_end = conjunct_stack.merge(*parent_stack);

	RecordSource* rsb = generate(tdbb, opt, key_streams + i + 1,
		(USHORT) (key_streams[0] - i), &conjunct_stack, stream);

	if (parent_stack)
		conjunct_stack.split(stack_end, *parent_stack);

	fb_assert(local_streams[0] < MAX_STREAMS && local_streams[0] < MAX_UCHAR);
	local_streams[++local_streams[0]] = stream;

	return rsb;
}

// Generate a union complex.
RecordSource* UnionSourceNode::generate(thread_db* tdbb, OptimizerBlk* opt, UCHAR* streams,
	USHORT nstreams, NodeStack* parent_stack, UCHAR shellStream)
{
	DEV_BLKCHK(opt, type_opt);
	DEV_BLKCHK(union_node, type_nod);

	SET_TDBB(tdbb);

	CompilerScratch* csb = opt->opt_csb;
	HalfStaticArray<RecordSource*, OPT_STATIC_ITEMS> rsbs;

	const SLONG base_impure = CMP_impure(csb, 0);

	NestConst<RseNode>* ptr = clauses.begin();
	NestConst<jrd_nod>* ptr2 = maps.begin();

	for (NestConst<RseNode>* const end = clauses.end(); ptr != end; ++ptr, ++ptr2)
	{
		RseNode* rse = *ptr;
		jrd_nod* map = *ptr2;

		// AB: Try to distribute booleans from the top rse for an UNION to
		// the WHERE clause of every single rse.
		// hvlad: don't do it for recursive unions else they will work wrong !
		NodeStack deliverStack;
		if (!recursive)
			genDeliverUnmapped(tdbb, &deliverStack, map, parent_stack, shellStream);

		rsbs.add(OPT_compile(tdbb, csb, rse, &deliverStack));

		// hvlad: activate recursive union itself after processing first (non-recursive)
		// member to allow recursive members be optimized
		if (recursive)
			csb->csb_rpt[stream].csb_flags |= csb_active;
	}

	if (recursive)
	{
		fb_assert(rsbs.getCount() == 2 && maps.getCount() == 2);
		// hvlad: save size of inner impure area and context of mapped record
		// for recursive processing later
		return FB_NEW(*tdbb->getDefaultPool()) RecursiveStream(csb, stream, mapStream,
			rsbs[0], rsbs[1], maps[0], maps[1], nstreams, streams, base_impure);
	}

	return FB_NEW(*tdbb->getDefaultPool()) Union(csb, stream, clauses.getCount(), rsbs.begin(),
		maps.begin(), nstreams, streams);
}

// Identify all of the streams for which a dbkey may need to be carried through a sort.
void UnionSourceNode::computeDbKeyStreams(UCHAR* streams) const
{
	const NestConst<RseNode>* ptr = clauses.begin();

	for (const NestConst<RseNode>* const end = clauses.end(); ptr != end; ++ptr)
		(*ptr)->computeDbKeyStreams(streams);
}

bool UnionSourceNode::computable(CompilerScratch* csb, SSHORT stream, bool idx_use,
	bool allowOnlyCurrentStream, jrd_nod* value)
{
	NestConst<RseNode>* ptr = clauses.begin();

	for (NestConst<RseNode>* const end = clauses.end(); ptr != end; ++ptr)
	{
		if (!(*ptr)->computable(csb, stream, idx_use, allowOnlyCurrentStream, NULL))
			return false;
	}

	return true;
}

void UnionSourceNode::findDependentFromStreams(const OptimizerRetrieval* optRet,
	SortedStreamList* streamList)
{
	NestConst<RseNode>* ptr = clauses.begin();

	for (NestConst<RseNode>* const end = clauses.end(); ptr != end; ++ptr)
		(*ptr)->findDependentFromStreams(optRet, streamList);
}


//--------------------


// Parse a window reference.
WindowSourceNode* WindowSourceNode::parse(thread_db* tdbb, CompilerScratch* csb)
{
	SET_TDBB(tdbb);

	WindowSourceNode* node = FB_NEW(*tdbb->getDefaultPool()) WindowSourceNode(
		*tdbb->getDefaultPool());

	node->rse = RseNode::getFrom(PAR_parse_node(tdbb, csb, TYPE_RSE));

	unsigned partitionCount = csb->csb_blr_reader.getByte();
	NodeStack stack;

	for (unsigned i = 0; i < partitionCount; ++i)
		stack.push(parsePartitionBy(tdbb, csb));

	node->windows = PAR_make_list(tdbb, stack);

	return node;
}

// Parse PARTITION BY subclauses of window functions.
jrd_nod* WindowSourceNode::parsePartitionBy(thread_db* tdbb, CompilerScratch* csb)
{
	SET_TDBB(tdbb);

	if (csb->csb_blr_reader.getByte() != blr_partition_by)
		PAR_syntax_error(csb, "blr_partition_by");

	SSHORT context;
	SSHORT partitionStream;
	partitionStream = PAR_context(csb, &context);

	jrd_nod* list = PAR_make_node(tdbb, e_part_length);
	list->nod_type = nod_list;
	list->nod_count = e_part_count;

	const UCHAR count = csb->csb_blr_reader.getByte();

	if (count != 0)
	{
		jrd_nod*& groupNode = list->nod_arg[e_part_group];
		jrd_nod*& regroupNode = list->nod_arg[e_part_regroup];

		groupNode = PAR_args(tdbb, csb, VALUE, count, count * 3);
		regroupNode = PAR_args(tdbb, csb, VALUE, count, count);

		// We have allocated groupNode with bigger length than expressions. This is to use in
		// OPT_gen_sort. Now fill that info.

		groupNode->nod_type = nod_sort;

		for (unsigned i = 0; i < count; ++i)
		{
			groupNode->nod_arg[count + i] = (jrd_nod*)(IPTR) false;	// ascending
			groupNode->nod_arg[count * 2 + i] = (jrd_nod*)(IPTR) rse_nulls_first;
		}
	}

	if (csb->csb_blr_reader.getByte() != blr_sort)
		PAR_syntax_error(csb, "blr_sort");

	list->nod_arg[e_part_order] = PAR_sort(tdbb, csb, true, true);
	list->nod_arg[e_part_map] = parseMap(tdbb, csb, partitionStream);
	list->nod_arg[e_part_stream] = (jrd_nod*)(IPTR) partitionStream;

	return list;
}

WindowSourceNode* WindowSourceNode::copy(thread_db* tdbb, NodeCopier& copier)
{
	if (!copier.remap)
		BUGCHECK(221);		// msg 221 (CMP) copy: cannot remap

	WindowSourceNode* newSource = FB_NEW(*tdbb->getDefaultPool()) WindowSourceNode(
		*tdbb->getDefaultPool());

	newSource->rse = rse->copy(tdbb, copier);

	jrd_nod* inputWindows = windows;

	jrd_nod* copyWindows = newSource->windows = PAR_make_node(tdbb, inputWindows->nod_count);
	copyWindows->nod_type = inputWindows->nod_type;
	copyWindows->nod_count = inputWindows->nod_count;

	for (unsigned i = 0; i < inputWindows->nod_count; ++i)
	{
		jrd_nod* inputPartition = inputWindows->nod_arg[i];

		jrd_nod* copyPartition = copyWindows->nod_arg[i] = PAR_make_node(tdbb, e_part_length);
		copyPartition->nod_type = inputPartition->nod_type;
		copyPartition->nod_count = inputPartition->nod_count;

		USHORT oldStream = (USHORT)(IPTR) inputPartition->nod_arg[e_part_stream];
		fb_assert(oldStream <= MAX_STREAMS);

		USHORT newStream = copier.csb->nextStream();

		copyPartition->nod_arg[e_part_stream] = (jrd_nod*)(IPTR) newStream;
		// fb_assert(newStream <= MAX_UCHAR);
		copier.remap[oldStream] = (UCHAR) newStream;
		CMP_csb_element(copier.csb, newStream);

		for (unsigned j = 0; j < copyPartition->nod_count; ++j)
			copyPartition->nod_arg[j] = copier.copy(tdbb, inputPartition->nod_arg[j]);
	}

	return newSource;
}

void WindowSourceNode::ignoreDbKey(thread_db* tdbb, CompilerScratch* csb, const jrd_rel* view) const
{
	rse->ignoreDbKey(tdbb, csb, view);
}

void WindowSourceNode::pass1(thread_db* tdbb, CompilerScratch* csb, jrd_rel* view)
{
	const jrd_nod* nodWindows = windows;

	for (unsigned i = 0; i < nodWindows->nod_count; ++i)
	{
		USHORT partStream = (USHORT)(IPTR) nodWindows->nod_arg[i]->nod_arg[e_part_stream];
		fb_assert(partStream <= MAX_STREAMS);
		csb->csb_rpt[partStream].csb_flags |= csb_no_dbkey;
	}

	rse->ignoreDbKey(tdbb, csb, view);
	rse->pass1(tdbb, csb, csb->csb_view);
	windows = CMP_pass1(tdbb, csb, windows);
}

void WindowSourceNode::pass1Source(thread_db* tdbb, CompilerScratch* csb, RseNode* rse,
	jrd_nod** boolean, RecordSourceNodeStack& stack)
{
	stack.push(this);	// Assume that the source will be used. Push it on the final stream stack.

	pass1(tdbb, csb, csb->csb_view);
}

void WindowSourceNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	rse->pass2Rse(tdbb, csb);

	jrd_nod* nodWindows = windows;

	for (unsigned i = 0; i < nodWindows->nod_count; ++i)
	{
		CMP_pass2(tdbb, csb, nodWindows->nod_arg[i]->nod_arg[e_part_map], nodWindows->nod_arg[i]);
		CMP_pass2(tdbb, csb, nodWindows->nod_arg[i]->nod_arg[e_part_group], nodWindows->nod_arg[i]);
		CMP_pass2(tdbb, csb, nodWindows->nod_arg[i]->nod_arg[e_part_order], nodWindows->nod_arg[i]);

		const USHORT partStream = (SSHORT)(IPTR) nodWindows->nod_arg[i]->nod_arg[e_part_stream];
		fb_assert(partStream <= MAX_STREAMS);

		processMap(tdbb, csb, nodWindows->nod_arg[i]->nod_arg[e_part_map],
			&csb->csb_rpt[partStream].csb_internal_format);
		csb->csb_rpt[partStream].csb_format = csb->csb_rpt[partStream].csb_internal_format;
	}

	for (unsigned i = 0; i < nodWindows->nod_count; ++i)
		CMP_pass2(tdbb, csb, nodWindows->nod_arg[i]->nod_arg[e_part_regroup], nodWindows->nod_arg[i]);
}

void WindowSourceNode::pass2Rse(thread_db* tdbb, CompilerScratch* csb)
{
	const jrd_nod* nodWindows = windows;

	pass2(tdbb, csb);

	for (unsigned i = 0; i < nodWindows->nod_count; ++i)
	{
		const SSHORT partStream = (USHORT)(IPTR) nodWindows->nod_arg[i]->nod_arg[e_part_stream];
		csb->csb_rpt[partStream].csb_flags |= csb_active;
	}
}

bool WindowSourceNode::containsStream(USHORT checkStream) const
{
	const jrd_nod* nodWindows = windows;

	for (unsigned i = 0; i < nodWindows->nod_count; ++i)
	{
		if (checkStream == (USHORT)(IPTR) nodWindows->nod_arg[i]->nod_arg[e_part_stream])
			return true;		// do not mark as variant
	}

	if (rse->containsStream(checkStream))
		return true;		// do not mark as variant

	return false;
}

RecordSource* WindowSourceNode::compile(thread_db* tdbb, CompilerScratch* csb, OptimizerBlk* opt,
	RseNode* rse, NodeStack* parent_stack, stream_array_t& beds, stream_array_t& key_streams,
	stream_array_t& local_streams, NodeStack& conjunct_stack, stream_array_t& streams,
	jrd_nod* sort, jrd_nod* aggregate, StreamsArray& outerStreams, SLONG conjunct_count,
	bool innerSubStream)
{
	const jrd_nod* nodWindows = windows;

	for (unsigned i = 0; i < nodWindows->nod_count; ++i)
	{
		SSHORT partStream = (USHORT)(IPTR) nodWindows->nod_arg[i]->nod_arg[e_part_stream];

		fb_assert(partStream <= MAX_UCHAR);
		fb_assert(beds[0] < MAX_STREAMS && beds[0] < MAX_UCHAR); // debug check
		//if (beds[0] >= MAX_STREAMS) // all builds check
		//	ERR_post(Arg::Gds(isc_too_many_contexts));

		beds[++beds[0]] = (UCHAR) partStream;
	}

	NodeStack deliverStack;

	RecordSource* rsb = FB_NEW(*tdbb->getDefaultPool()) WindowedStream(csb, windows,
		OPT_compile(tdbb, csb, this->rse, &deliverStack));

	StreamsArray rsbStreams;
	rsb->findUsedStreams(rsbStreams);

	for (StreamsArray::iterator i = rsbStreams.begin(); i != rsbStreams.end(); ++i)
	{
		fb_assert(local_streams[0] < MAX_STREAMS && local_streams[0] < MAX_UCHAR);
		local_streams[++local_streams[0]] = *i;
	}

	return rsb;
}

bool WindowSourceNode::computable(CompilerScratch* csb, SSHORT stream, bool idx_use,
	bool allowOnlyCurrentStream, jrd_nod* value)
{
	return rse->computable(csb, stream, idx_use, allowOnlyCurrentStream, NULL);
}

void WindowSourceNode::getStreams(StreamsArray& list) const
{
	for (unsigned i = 0; i < windows->nod_count; ++i)
		list.add((int)(IPTR) windows->nod_arg[i]->nod_arg[e_part_stream]);
}

void WindowSourceNode::findDependentFromStreams(const OptimizerRetrieval* optRet,
	SortedStreamList* streamList)
{
	rse->findDependentFromStreams(optRet, streamList);
}


//--------------------


RseNode* RseNode::copy(thread_db* tdbb, NodeCopier& copier)
{
	RseNode* newSource = FB_NEW(*tdbb->getDefaultPool()) RseNode(*tdbb->getDefaultPool());

	NestConst<RecordSourceNode>* ptr = rse_relations.begin();

	for (NestConst<RecordSourceNode>* const end = rse_relations.end(); ptr != end; ++ptr)
		newSource->rse_relations.add((*ptr)->copy(tdbb, copier));

	newSource->flags = flags;
	newSource->rse_jointype = rse_jointype;
	newSource->rse_first = copier.copy(tdbb, rse_first);
	newSource->rse_skip = copier.copy(tdbb, rse_skip);
	newSource->rse_boolean = copier.copy(tdbb, rse_boolean);
	newSource->rse_sorted = copier.copy(tdbb, rse_sorted);
	newSource->rse_projection = copier.copy(tdbb, rse_projection);

	return newSource;
}

// For each relation or aggregate in the RseNode, mark it as not having a dbkey.
void RseNode::ignoreDbKey(thread_db* tdbb, CompilerScratch* csb, const jrd_rel* view) const
{
	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(view, type_rel);

	const NestConst<RecordSourceNode>* ptr = rse_relations.begin();

	for (const NestConst<RecordSourceNode>* const end = rse_relations.end(); ptr != end; ++ptr)
		(*ptr)->ignoreDbKey(tdbb, csb, view);
}

// Process a record select expression during pass 1 of compilation.
// Mostly this involves expanding views.
void RseNode::pass1(thread_db* tdbb, CompilerScratch* csb, jrd_rel* view)
{
	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(view, type_rel);

	// for scoping purposes, maintain a stack of RseNode's which are
	// currently being parsed; if there are none on the stack as
	// yet, mark the RseNode as variant to make sure that statement-
	// level aggregates are not treated as invariants -- bug #6535

	bool top_level_rse = true;

	for (LegacyNodeOrRseNode* node = csb->csb_current_nodes.begin();
		 node != csb->csb_current_nodes.end(); ++node)
	{
		if (node->rseNode)
		{
			top_level_rse = false;
			break;
		}
	}

	if (top_level_rse)
		flags |= FLAG_VARIANT;

	csb->csb_current_nodes.push(this);

	RecordSourceNodeStack stack;
	jrd_nod* boolean = NULL;
	jrd_nod* sort = rse_sorted;
	jrd_nod* project = rse_projection;
	jrd_nod* first = rse_first;
	jrd_nod* skip = rse_skip;
	jrd_nod* plan = rse_plan;

	// zip thru RseNode expanding views and inner joins
	NestConst<RecordSourceNode>* arg = rse_relations.begin();
	for (const NestConst<RecordSourceNode>* const end = rse_relations.end(); arg != end; ++arg)
		processSource(tdbb, csb, this, *arg, &boolean, stack);

	// Now, rebuild the RseNode block.

	rse_relations.resize(stack.getCount());
	arg = rse_relations.end();

	while (stack.hasData())
		*--arg = stack.pop();

	AutoSetRestore<bool> autoValidateExpr(&csb->csb_validate_expr, false);

	// finish of by processing other clauses

	if (first)
		rse_first = CMP_pass1(tdbb, csb, first);

	if (skip)
		rse_skip = CMP_pass1(tdbb, csb, skip);

	if (boolean)
	{
		if (rse_boolean)
		{
			jrd_nod* additional = PAR_make_node(tdbb, 2);
			additional->nod_type = nod_and;
			additional->nod_arg[0] = boolean;
			additional->nod_arg[1] = CMP_pass1(tdbb, csb, rse_boolean);
			rse_boolean = additional;
		}
		else
			rse_boolean = boolean;
	}
	else
		rse_boolean = CMP_pass1(tdbb, csb, rse_boolean);

	if (sort)
		rse_sorted = CMP_pass1(tdbb, csb, sort);

	if (project)
		rse_projection = CMP_pass1(tdbb, csb, project);

	if (plan)
		rse_plan = plan;

	// we are no longer in the scope of this RseNode
	csb->csb_current_nodes.pop();
}

void RseNode::pass1Source(thread_db* tdbb, CompilerScratch* csb, RseNode* rse,
	jrd_nod** boolean, RecordSourceNodeStack& stack)
{
	// in the case of an RseNode, it is possible that a new RseNode will be generated,
	// so wait to process the source before we push it on the stack (bug 8039)

	// The addition of the JOIN syntax for specifying inner joins causes an
	// RseNode tree to be generated, which is undesirable in the simplest case
	// where we are just trying to inner join more than 2 streams. If possible,
	// try to flatten the tree out before we go any further.

	if (!rse->rse_jointype && !rse_jointype && !rse_sorted && !rse_projection &&
		!rse_first && !rse_skip && !rse_plan)
	{
		NestConst<RecordSourceNode>* arg = rse_relations.begin();
		for (const NestConst<RecordSourceNode>* const end = rse_relations.end(); arg != end; ++arg)
			processSource(tdbb, csb, rse, *arg, boolean, stack);

		// fold in the boolean for this inner join with the one for the parent

		if (rse_boolean)
		{
			jrd_nod* node = CMP_pass1(tdbb, csb, rse_boolean);

			if (*boolean)
			{
				jrd_nod* additional = PAR_make_node(tdbb, 2);
				additional->nod_type = nod_and;
				additional->nod_arg[0] = node;
				additional->nod_arg[1] = *boolean;
				*boolean = additional;
			}
			else
				*boolean = node;
		}

		return;
	}

	pass1(tdbb, csb, csb->csb_view);
	stack.push(this);
}

// Perform the first half of record selection expression compilation.
// The actual optimization is done in "post_rse".
void RseNode::pass2Rse(thread_db* tdbb, CompilerScratch* csb)
{
	SET_TDBB(tdbb);
	DEV_BLKCHK(csb, type_csb);

	// Maintain stack of RSEe for scoping purposes
	csb->csb_current_nodes.push(this);

	if (rse_first)
		CMP_pass2(tdbb, csb, rse_first, NULL);

	if (rse_skip)
	    CMP_pass2(tdbb, csb, rse_skip, NULL);

	NestConst<RecordSourceNode>* ptr = rse_relations.begin();

	for (const NestConst<RecordSourceNode>* const end = rse_relations.end(); ptr != end; ++ptr)
		(*ptr)->pass2Rse(tdbb, csb);

	if (rse_boolean)
		CMP_pass2(tdbb, csb, rse_boolean, NULL);

	if (rse_sorted)
		CMP_pass2(tdbb, csb, rse_sorted, NULL);

	if (rse_projection)
		CMP_pass2(tdbb, csb, rse_projection, NULL);

	// If the user has submitted a plan for this RseNode, check it for correctness.

	if (rse_plan)
	{
		planSet(csb, rse_plan);
		planCheck(csb);
	}

	csb->csb_current_nodes.pop();
}

// Return true if stream is contained in the specified RseNode.
bool RseNode::containsStream(USHORT checkStream) const
{
	// Look through all relation nodes in this RseNode to see
	// if the field references this instance of the relation.

	const NestConst<RecordSourceNode>* ptr = rse_relations.begin();

	for (const NestConst<RecordSourceNode>* const end = rse_relations.end(); ptr != end; ++ptr)
	{
		const RecordSourceNode* sub = *ptr;

		if (sub->containsStream(checkStream))
			return true;		// do not mark as variant
	}

	return false;
}

RecordSource* RseNode::compile(thread_db* tdbb, CompilerScratch* csb, OptimizerBlk* opt,
	RseNode* rse, NodeStack* parent_stack, stream_array_t& beds, stream_array_t& key_streams,
	stream_array_t& local_streams, NodeStack& conjunct_stack, stream_array_t& streams,
	jrd_nod* sort, jrd_nod* aggregate, StreamsArray& outerStreams, SLONG conjunct_count,
	bool innerSubStream)
{
	// for nodes which are not relations, generate an rsb to
	// represent that work has to be done to retrieve them;
	// find all the substreams involved and compile them as well

	computeRseStreams(csb, beds);
	computeRseStreams(csb, local_streams);
	computeDbKeyStreams(key_streams);

	RecordSource* rsb;

	// pass RseNode boolean only to inner substreams because join condition
	// should never exclude records from outer substreams
	if (rse->rse_jointype == blr_inner || (rse->rse_jointype == blr_left && innerSubStream))
	{
		// AB: For an (X LEFT JOIN Y) mark the outer-streams (X) as
		// active because the inner-streams (Y) are always "dependent"
		// on the outer-streams. So that index retrieval nodes could be made.
		// For an INNER JOIN mark previous generated RecordSource's as active.
		if (rse->rse_jointype == blr_left)
		{
			for (StreamsArray::iterator i = outerStreams.begin(); i != outerStreams.end(); ++i)
				csb->csb_rpt[*i].csb_flags |= csb_active;
		}

		//const NodeStack::iterator stackSavepoint(conjunct_stack);
		NodeStack::const_iterator stack_end;
		NodeStack deliverStack;

		if (rse->rse_jointype != blr_inner)
		{
			// Make list of nodes that can be delivered to an outer-stream.
			// In fact these are all nodes except when a IS NULL (nod_missing)
			// comparision is done.
			// Note! Don't forget that this can be burried inside a expression
			// such as "CASE WHEN (FieldX IS NULL) THEN 0 ELSE 1 END = 0"
			NodeStack::iterator stackItem;
			if (parent_stack)
				stackItem = *parent_stack;

			for (; stackItem.hasData(); ++stackItem)
			{
				jrd_nod* deliverNode = stackItem.object();

				if (!PossibleUnknownFinder::find(deliverNode))
					deliverStack.push(deliverNode);
			}
			stack_end = conjunct_stack.merge(deliverStack);
		}
		else
		{
			if (parent_stack)
				stack_end = conjunct_stack.merge(*parent_stack);
		}

		rsb = OPT_compile(tdbb, csb, this, &conjunct_stack);

		if (rse->rse_jointype != blr_inner)
		{
			// Remove previously added parent conjuctions from the stack.
			conjunct_stack.split(stack_end, deliverStack);
		}
		else
		{
			if (parent_stack)
				conjunct_stack.split(stack_end, *parent_stack);
		}

		if (rse->rse_jointype == blr_left)
		{
			for (StreamsArray::iterator i = outerStreams.begin(); i != outerStreams.end(); ++i)
				csb->csb_rpt[*i].csb_flags &= ~csb_active;
		}
	}
	else
		rsb = OPT_compile(tdbb, csb, this, parent_stack);

	return rsb;
}

// Identify the streams that make up an RseNode.
void RseNode::computeRseStreams(const CompilerScratch* csb, UCHAR* streams) const
{
	DEV_BLKCHK(csb, type_csb);

	const NestConst<RecordSourceNode>* ptr = rse_relations.begin();

	for (const NestConst<RecordSourceNode>* const end = rse_relations.end(); ptr != end; ++ptr)
	{
		const RecordSourceNode* node = *ptr;

		if (node->type == RseNode::TYPE)
			static_cast<const RseNode*>(node)->computeRseStreams(csb, streams);
		else
		{
			StreamsArray sourceStreams;
			node->getStreams(sourceStreams);

			for (StreamsArray::iterator i = sourceStreams.begin(); i != sourceStreams.end(); ++i)
			{
				fb_assert(streams[0] < MAX_STREAMS && streams[0] < MAX_UCHAR);
				streams[++streams[0]] = (UCHAR) *i;
			}
		}
	}
}

// Check that all streams in the RseNode have a plan specified for them.
// If they are not, there are streams in the RseNode which were not mentioned in the plan.
void RseNode::planCheck(const CompilerScratch* csb) const
{
	DEV_BLKCHK(csb, type_csb);

	// if any streams are not marked with a plan, give an error

	const NestConst<RecordSourceNode>* ptr = rse_relations.begin();
	for (const NestConst<RecordSourceNode>* const end = rse_relations.end(); ptr != end; ++ptr)
	{
		const RecordSourceNode* node = *ptr;

		if (node->type == RelationSourceNode::TYPE)
		{
			const USHORT stream = node->getStream();

			if (!(csb->csb_rpt[stream].csb_plan))
			{
				ERR_post(Arg::Gds(isc_no_stream_plan) <<
					Arg::Str(csb->csb_rpt[stream].csb_relation->rel_name));
			}
		}
		else if (node->type == RseNode::TYPE)
			static_cast<const RseNode*>(node)->planCheck(csb);
	}
}

// Go through the streams in the plan, find the corresponding streams in the RseNode and store the
// plan for that stream. Do it once and only once to make sure there is a one-to-one correspondence
// between streams in the query and streams in the plan.
void RseNode::planSet(CompilerScratch* csb, jrd_nod* plan)
{
	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(plan, type_nod);

	if (plan->nod_type == nod_join)
	{
		for (jrd_nod** ptr = plan->nod_arg, **end = ptr + plan->nod_count; ptr < end; ptr++)
			planSet(csb, *ptr);
	}

	if (plan->nod_type != nod_retrieve)
		return;

	fb_assert(plan->nod_arg[e_retrieve_relation]->nod_type == nod_class_recsrcnode_jrd);
	RelationSourceNode* recSource = reinterpret_cast<RelationSourceNode*>(
		plan->nod_arg[e_retrieve_relation]->nod_arg[0]);
	fb_assert(recSource->type == RelationSourceNode::TYPE);

	const jrd_rel* view_relation = NULL;
	const jrd_rel* plan_relation = (jrd_rel*) recSource->relation;
	const char* plan_alias = (const char *) recSource->alias;

	// find the tail for the relation specified in the RseNode

	const USHORT stream = recSource->getStream();
	CompilerScratch::csb_repeat* tail = &csb->csb_rpt[stream];

	// if the plan references a view, find the real base relation
	// we are interested in by searching the view map */
	UCHAR* map = NULL;

	if (tail->csb_map)
	{
		const TEXT* p = plan_alias;

		// if the user has specified an alias, skip past it to find the alias
		// for the base table (if multiple aliases are specified)
		if (p && *p &&
			((tail->csb_relation && !strcmpSpace(tail->csb_relation->rel_name.c_str(), p)) ||
			 (tail->csb_alias && !strcmpSpace(tail->csb_alias->c_str(), p))))
		{
			while (*p && *p != ' ')
				p++;

			if (*p == ' ')
				p++;
		}

		// loop through potentially a stack of views to find the appropriate base table
		UCHAR* map_base;

		while ( (map_base = tail->csb_map) )
		{
			map = map_base;
			tail = &csb->csb_rpt[*map];
			view_relation = tail->csb_relation;

			// if the plan references the view itself, make sure that
			// the view is on a single table; if it is, fix up the plan
			// to point to the base relation

			if (view_relation->rel_id == plan_relation->rel_id)
			{
				if (!map_base[2])
				{
					map++;
					tail = &csb->csb_rpt[*map];
				}
				else
				{
					// view %s has more than one base relation; use aliases to distinguish
					ERR_post(Arg::Gds(isc_view_alias) << Arg::Str(plan_relation->rel_name));
				}

				break;
			}

			view_relation = NULL;

			// if the user didn't specify an alias (or didn't specify one
			// for this level), check to make sure there is one and only one
			// base relation in the table which matches the plan relation

			if (!*p)
			{
				const jrd_rel* duplicate_relation = NULL;
				UCHAR* duplicate_map = map_base;
				map = NULL;
				for (duplicate_map++; *duplicate_map; duplicate_map++)
				{
					CompilerScratch::csb_repeat* duplicate_tail = &csb->csb_rpt[*duplicate_map];
					const jrd_rel* relation = duplicate_tail->csb_relation;
					if (relation && relation->rel_id == plan_relation->rel_id)
					{
						if (duplicate_relation)
						{
							// table %s is referenced twice in view; use an alias to distinguish
							ERR_post(Arg::Gds(isc_duplicate_base_table) <<
								Arg::Str(duplicate_relation->rel_name));
						}
						else
						{
							duplicate_relation = relation;
							map = duplicate_map;
							tail = duplicate_tail;
						}
					}
				}

				break;
			}

			// look through all the base relations for a match

			map = map_base;
			for (map++; *map; map++)
			{
				tail = &csb->csb_rpt[*map];
				const jrd_rel* relation = tail->csb_relation;

				// match the user-supplied alias with the alias supplied
				// with the view definition; failing that, try the base
				// table name itself

				// CVC: I found that "relation" can be NULL, too. This may be an
				// indication of a logic flaw while parsing the user supplied SQL plan
				// and not an oversight here. It's hard to imagine a csb->csb_rpt with
				// a NULL relation. See exe.h for CompilerScratch struct and its inner csb_repeat struct.

				if ((tail->csb_alias && !strcmpSpace(tail->csb_alias->c_str(), p)) ||
					(relation && !strcmpSpace(relation->rel_name.c_str(), p)))
				{
					break;
				}
			}

			// skip past the alias

			while (*p && *p != ' ')
				p++;

			if (*p == ' ')
				p++;

			if (!*map)
			{
				// table %s is referenced in the plan but not the from list
				ERR_post(Arg::Gds(isc_stream_not_found) << Arg::Str(plan_relation->rel_name));
			}
		}

		// fix up the relation node to point to the base relation's stream

		if (!map || !*map)
		{
			// table %s is referenced in the plan but not the from list
			ERR_post(Arg::Gds(isc_stream_not_found) << Arg::Str(plan_relation->rel_name));
		}

		recSource->setStream(*map);
	}

	// make some validity checks

	if (!tail->csb_relation)
	{
		// table %s is referenced in the plan but not the from list
		ERR_post(Arg::Gds(isc_stream_not_found) << Arg::Str(plan_relation->rel_name));
	}

	if ((tail->csb_relation->rel_id != plan_relation->rel_id) && !view_relation)
	{
		// table %s is referenced in the plan but not the from list
		ERR_post(Arg::Gds(isc_stream_not_found) << Arg::Str(plan_relation->rel_name));
	}

	// check if we already have a plan for this stream

	if (tail->csb_plan)
	{
		// table %s is referenced more than once in plan; use aliases to distinguish
		ERR_post(Arg::Gds(isc_stream_twice) << Arg::Str(tail->csb_relation->rel_name));
	}

	tail->csb_plan = plan;
}

// Identify all of the streams for which a dbkey may need to be carried through a sort.
void RseNode::computeDbKeyStreams(UCHAR* streams) const
{
	const NestConst<RecordSourceNode>* ptr = rse_relations.begin();

	for (const NestConst<RecordSourceNode>* const end = rse_relations.end(); ptr != end; ++ptr)
		(*ptr)->computeDbKeyStreams(streams);
}

bool RseNode::computable(CompilerScratch* csb, SSHORT stream, bool idx_use,
	bool allowOnlyCurrentStream, jrd_nod* value)
{
	jrd_nod* sub;

	if ((sub = rse_first) && !OPT_computable(csb, sub, stream, idx_use, allowOnlyCurrentStream))
		return false;

    if ((sub = rse_skip) && !OPT_computable(csb, sub, stream, idx_use, allowOnlyCurrentStream))
        return false;

	const NestConst<RecordSourceNode>* const end = rse_relations.end();
	NestConst<RecordSourceNode>* ptr;

	// Set sub-streams of rse active

	for (ptr = rse_relations.begin(); ptr != end; ++ptr)
	{
		const RecordSourceNode* const node = *ptr;

		StreamsArray streams;
		node->getStreams(streams);

		for (StreamsArray::iterator i = streams.begin(); i != streams.end(); ++i)
			csb->csb_rpt[*i].csb_flags |= (csb_active | csb_sub_stream);
	}

	bool result = true;

	// Check sub-stream
	if (((sub = rse_boolean) && !OPT_computable(csb, sub, stream, idx_use, allowOnlyCurrentStream)) ||
	    ((sub = rse_sorted) && !OPT_computable(csb, sub, stream, idx_use, allowOnlyCurrentStream)) ||
	    ((sub = rse_projection) && !OPT_computable(csb, sub, stream, idx_use, allowOnlyCurrentStream)))
	{
		result = false;
	}

	for (ptr = rse_relations.begin(); ptr != end && result; ++ptr)
	{
		if (!(*ptr)->computable(csb, stream, idx_use, allowOnlyCurrentStream, NULL))
			result = false;
	}

	// Check value expression, if any
	if (result && value && !OPT_computable(csb, value, stream, idx_use, allowOnlyCurrentStream))
		result = false;

	// Reset streams inactive

	for (ptr = rse_relations.begin(); ptr != end; ++ptr)
	{
		const RecordSourceNode* const node = *ptr;

		StreamsArray streams;
		node->getStreams(streams);

		for (StreamsArray::iterator i = streams.begin(); i != streams.end(); ++i)
			csb->csb_rpt[*i].csb_flags &= ~(csb_active | csb_sub_stream);
	}

	return result;
}

void RseNode::findDependentFromStreams(const OptimizerRetrieval* optRet,
	SortedStreamList* streamList)
{
	jrd_nod* sub;

	if (sub = rse_first)
		optRet->findDependentFromStreams(sub, streamList);

    if (sub = rse_skip)
        optRet->findDependentFromStreams(sub, streamList);

	if (sub = rse_boolean)
		optRet->findDependentFromStreams(sub, streamList);

	if (sub = rse_sorted)
		optRet->findDependentFromStreams(sub, streamList);

	if (sub = rse_projection)
		optRet->findDependentFromStreams(sub, streamList);

	NestConst<RecordSourceNode>* ptr;
	const NestConst<RecordSourceNode>* end;

	for (ptr = rse_relations.begin(), end = rse_relations.end(); ptr != end; ++ptr)
		(*ptr)->findDependentFromStreams(optRet, streamList);
}


//--------------------


// Parse a MAP clause for a union or global aggregate expression.
static jrd_nod* parseMap(thread_db* tdbb, CompilerScratch* csb, USHORT stream)
{
	SET_TDBB(tdbb);

	if (csb->csb_blr_reader.getByte() != blr_map)
		PAR_syntax_error(csb, "blr_map");

	SSHORT count = csb->csb_blr_reader.getWord();
	NodeStack map;

	while (--count >= 0)
	{
		jrd_nod* assignment = PAR_make_node(tdbb, e_asgn_length);
		assignment->nod_type = nod_assignment;
		assignment->nod_count = e_asgn_length;
		assignment->nod_arg[e_asgn_to] = PAR_gen_field(tdbb, stream, csb->csb_blr_reader.getWord());
		assignment->nod_arg[e_asgn_from] = PAR_parse_node(tdbb, csb, VALUE);
		map.push(assignment);
	}

	jrd_nod* node = PAR_make_list(tdbb, map);
	node->nod_type = nod_map;

	return node;
}

// Compare two strings, which could be either space-terminated or null-terminated.
static SSHORT strcmpSpace(const char* p, const char* q)
{
	for (; *p && *p != ' ' && *q && *q != ' '; p++, q++)
	{
		if (*p != *q)
			break;
	}

	if ((!*p || *p == ' ') && (!*q || *q == ' '))
		return 0;

	return (*p > *q) ? 1 : -1;
}

// Process a single record source stream from an RseNode.
// Obviously, if the source is a view, there is more work to do.
static void processSource(thread_db* tdbb, CompilerScratch* csb, RseNode* rse,
	RecordSourceNode* source, jrd_nod** boolean, RecordSourceNodeStack& stack)
{
	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(*boolean, type_nod);

	Database* dbb = tdbb->getDatabase();
	CHECK_DBB(dbb);

	AutoSetRestore<bool> autoValidateExpr(&csb->csb_validate_expr, false);

	source->pass1Source(tdbb, csb, rse, boolean, stack);
}

// Translate a map block into a format. If the format is missing or incomplete, extend it.
static void processMap(thread_db* tdbb, CompilerScratch* csb, jrd_nod* map, Format** input_format)
{
	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(map, type_nod);
	DEV_BLKCHK(*input_format, type_fmt);

	SET_TDBB(tdbb);

	Format* format = *input_format;
	if (!format)
		format = *input_format = Format::newFormat(*tdbb->getDefaultPool(), map->nod_count);

	// process alternating rse and map blocks
	dsc desc2;
	jrd_nod** ptr = map->nod_arg;

	for (const jrd_nod* const* const end = ptr + map->nod_count; ptr < end; ptr++)
	{
		jrd_nod* assignment = *ptr;
		jrd_nod* field = assignment->nod_arg[e_asgn_to];
		const USHORT id = (USHORT)(IPTR) field->nod_arg[e_fld_id];

		if (id >= format->fmt_count)
			format->fmt_desc.resize(id + 1);

		dsc* desc = &format->fmt_desc[id];
		CMP_get_desc(tdbb, csb, assignment->nod_arg[e_asgn_from], &desc2);
		const USHORT min = MIN(desc->dsc_dtype, desc2.dsc_dtype);
		const USHORT max = MAX(desc->dsc_dtype, desc2.dsc_dtype);

		if (!min)
		{
			// eg: dtype_unknown
			*desc = desc2;
		}
		else if (max == dtype_blob)
		{
			desc->dsc_dtype = dtype_blob;
			desc->dsc_length = sizeof(ISC_QUAD);
			desc->dsc_scale = 0;
			desc->dsc_sub_type = DataTypeUtil::getResultBlobSubType(desc, &desc2);
			desc->dsc_flags = 0;
		}
		else if (min <= dtype_any_text)
		{
			// either field a text field?
			const USHORT len1 = DSC_string_length(desc);
			const USHORT len2 = DSC_string_length(&desc2);
			desc->dsc_dtype = dtype_varying;
			desc->dsc_length = MAX(len1, len2) + sizeof(USHORT);

			// pick the max text type, so any transparent casts from ints are
			// not left in ASCII format, but converted to the richer text format

			desc->setTextType(MAX(INTL_TEXT_TYPE(*desc), INTL_TEXT_TYPE(desc2)));
			desc->dsc_scale = 0;
			desc->dsc_flags = 0;
		}
		else if (DTYPE_IS_DATE(max) && !DTYPE_IS_DATE(min))
		{
			desc->dsc_dtype = dtype_varying;
			desc->dsc_length = DSC_convert_to_text_length(max) + sizeof(USHORT);
			desc->dsc_ttype() = ttype_ascii;
			desc->dsc_scale = 0;
			desc->dsc_flags = 0;
		}
		else if (max != min)
		{
			// different numeric types: if one is inexact use double,
			// if both are exact use int64
			if ((!DTYPE_IS_EXACT(max)) || (!DTYPE_IS_EXACT(min)))
			{
				desc->dsc_dtype = DEFAULT_DOUBLE;
				desc->dsc_length = sizeof(double);
				desc->dsc_scale = 0;
				desc->dsc_sub_type = 0;
				desc->dsc_flags = 0;
			}
			else
			{
				desc->dsc_dtype = dtype_int64;
				desc->dsc_length = sizeof(SINT64);
				desc->dsc_scale = MIN(desc->dsc_scale, desc2.dsc_scale);
				desc->dsc_sub_type = MAX(desc->dsc_sub_type, desc2.dsc_sub_type);
				desc->dsc_flags = 0;
			}
		}
	}

	// flesh out the format of the record

	ULONG offset = FLAG_BYTES(format->fmt_count);

	Format::fmt_desc_iterator desc3 = format->fmt_desc.begin();
	for (const Format::fmt_desc_const_iterator end_desc = format->fmt_desc.end();
		 desc3 < end_desc; ++desc3)
	{
		const USHORT align = type_alignments[desc3->dsc_dtype];

		if (align)
			offset = FB_ALIGN(offset, align);

		desc3->dsc_address = (UCHAR *) (IPTR) offset;
		offset += desc3->dsc_length;
	}

	if (offset > MAX_FORMAT_SIZE)
		ERR_post(Arg::Gds(isc_imp_exc) << Arg::Gds(isc_blktoobig));

	format->fmt_length = (USHORT) offset;
	format->fmt_count = format->fmt_desc.getCount();
}

// Make new boolean nodes from nodes that contain a field from the given shellStream.
// Those fields are references (mappings) to other nodes and are used by aggregates and union rse's.
static void genDeliverUnmapped(thread_db* tdbb, NodeStack* deliverStack, jrd_nod* map,
	NodeStack* parentStack, UCHAR shellStream)
{
	DEV_BLKCHK(map, type_nod);
	SET_TDBB(tdbb);

	for (NodeStack::iterator stack1(*parentStack); stack1.hasData(); ++stack1)
	{
		jrd_nod* boolean = stack1.object();

		// Reduce to simple comparisons
		if (!((boolean->nod_type == nod_eql) ||
			  (boolean->nod_type == nod_gtr) ||
			  (boolean->nod_type == nod_geq) ||
			  (boolean->nod_type == nod_leq) ||
			  (boolean->nod_type == nod_lss) ||
			  (boolean->nod_type == nod_starts) ||
			  (boolean->nod_type == nod_missing)))
		{
			continue;
		}

		// At least 1 mapping should be used in the arguments
		int indexArg;
		bool mappingFound = false;

		for (indexArg = 0; (indexArg < boolean->nod_count) && !mappingFound; indexArg++)
		{
			jrd_nod* booleanNode = boolean->nod_arg[indexArg];
			if ((booleanNode->nod_type == nod_field) &&
				((USHORT)(IPTR) booleanNode->nod_arg[e_fld_stream] == shellStream))
			{
				mappingFound = true;
			}
		}

		if (!mappingFound)
			continue;

		// Create new node and assign the correct existing arguments
		jrd_nod* deliverNode = PAR_make_node(tdbb, boolean->nod_count);
		deliverNode->nod_count = boolean->nod_count;
		deliverNode->nod_type = boolean->nod_type;
		deliverNode->nod_flags = boolean->nod_flags;
		deliverNode->nod_impure = boolean->nod_impure;
		bool wrongNode = false;

		for (indexArg = 0; (indexArg < boolean->nod_count) && (!wrongNode); indexArg++)
		{
			jrd_nod* booleanNode = UnmappedNodeGetter::get(
				map, shellStream, boolean->nod_arg[indexArg]);

			wrongNode = (booleanNode == NULL);

			if (!wrongNode)
				deliverNode->nod_arg[indexArg] = booleanNode;
		}

		if (wrongNode)
			delete deliverNode;
		else
			deliverStack->push(deliverNode);
	}
}

// Mark indices that were not included in the user-specified access plan.
static void markIndices(CompilerScratch::csb_repeat* csb_tail, SSHORT relation_id)
{
	const jrd_nod* plan = csb_tail->csb_plan;

	if (!plan)
		return;
	if (plan->nod_type != nod_retrieve)
		return;

	// find out how many indices were specified; if
	// there were none, this is a sequential retrieval
	USHORT plan_count = 0;
	const jrd_nod* access_type = plan->nod_arg[e_retrieve_access_type];

	if (access_type)
		plan_count = access_type->nod_count;

	// go through each of the indices and mark it unusable
	// for indexed retrieval unless it was specifically mentioned
	// in the plan; also mark indices for navigational access

	index_desc* idx = csb_tail->csb_idx->items;

	for (USHORT i = 0; i < csb_tail->csb_indices; i++)
	{
		if (access_type)
		{
			const jrd_nod* const* arg = access_type->nod_arg;
			const jrd_nod* const* const end = arg + plan_count;

			for (; arg < end; arg += e_access_type_length)
			{
				if (relation_id != (SSHORT)(IPTR) arg[e_access_type_relation])
				{
					// index %s cannot be used in the specified plan
					const char* iname = reinterpret_cast<const char*>(arg[e_access_type_index_name]);
					ERR_post(Arg::Gds(isc_index_unused) << Arg::Str(iname));
				}

				if (idx->idx_id == (USHORT)(IPTR) arg[e_access_type_index])
				{
					if (access_type->nod_type == nod_navigational && arg == access_type->nod_arg)
					{
						// dimitr:	navigational access can use only one index,
						//			hence the extra check added (see the line above)
						idx->idx_runtime_flags |= idx_plan_navigate;
					}
					else
					{
						// nod_indices
						break;
					}
				}
			}

			if (arg == end)
				idx->idx_runtime_flags |= idx_plan_dont_use;
		}
		else
			idx->idx_runtime_flags |= idx_plan_dont_use;

		++idx;
	}
}

// Sort SortedStream indices based on there selectivity. Lowest selectivy as first, highest as last.
static void sortIndicesBySelectivity(CompilerScratch::csb_repeat* csb_tail)
{
	if (csb_tail->csb_plan)
		return;

	index_desc* selected_idx = NULL;
	Array<index_desc> idx_sort(csb_tail->csb_indices);
	bool same_selectivity = false;

	// Walk through the indices and sort them into into idx_sort
	// where idx_sort[0] contains the lowest selectivity (best) and
	// idx_sort[csb_tail->csb_indices - 1] the highest (worst)

	if (csb_tail->csb_idx && (csb_tail->csb_indices > 1))
	{
		for (USHORT j = 0; j < csb_tail->csb_indices; j++)
		{
			float selectivity = 1; // Maximum selectivity is 1 (when all keys are the same)
			index_desc* idx = csb_tail->csb_idx->items;

			for (USHORT i = 0; i < csb_tail->csb_indices; i++)
			{
				// Prefer ASC indices in the case of almost the same selectivities
				if (selectivity > idx->idx_selectivity)
					same_selectivity = ((selectivity - idx->idx_selectivity) <= 0.00001);
				else
					same_selectivity = ((idx->idx_selectivity - selectivity) <= 0.00001);

				if (!(idx->idx_runtime_flags & idx_marker) &&
					 (idx->idx_selectivity <= selectivity) &&
					 !((idx->idx_flags & idx_descending) && same_selectivity))
				{
					selectivity = idx->idx_selectivity;
					selected_idx = idx;
				}

				++idx;
			}

			// If no index was found than pick the first one available out of the list
			if ((!selected_idx) || (selected_idx->idx_runtime_flags & idx_marker))
			{
				idx = csb_tail->csb_idx->items;

				for (USHORT i = 0; i < csb_tail->csb_indices; i++)
				{
					if (!(idx->idx_runtime_flags & idx_marker))
					{
						selected_idx = idx;
						break;
					}

					++idx;
				}
			}

			selected_idx->idx_runtime_flags |= idx_marker;
			idx_sort.add(*selected_idx);
		}

		// Finally store the right order in cbs_tail->csb_idx
		index_desc* idx = csb_tail->csb_idx->items;

		for (USHORT j = 0; j < csb_tail->csb_indices; j++)
		{
			idx->idx_runtime_flags &= ~idx_marker;
			memcpy(idx, &idx_sort[j], sizeof(index_desc));
			++idx;
		}
	}
}
