/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		dbg.c
 *	DESCRIPTION:	Debugging routines
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
 */

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <string.h>
#include <signal.h>

#include "../jrd/jrd.h"
#include "../jrd/lck.h"
#include "../jrd/ods.h"
#include "../jrd/cch.h"
#include "../jrd/dbg.h"
#include "../jrd/val.h"
#include "../jrd/all.h"
#include "../jrd/exe.h"
#include "../jrd/req.h"
#include "../jrd/rse.h"
#include "../jrd/btr.h"
#include "../jrd/sort.h"
#include "../jrd/que.h"
#include "../jrd/cch_proto.h"
#include "../jrd/dbg_proto.h"
#include "../jrd/err_proto.h"

#ifdef SUPERSERVER
#include "../jrd/thd_proto.h"
#include "../jrd/err_proto.h"
#endif


/* Given pointer a field in the block, find the block */
#define BLOCK(fld_ptr, type, fld) (type)((SCHAR*) fld_ptr - OFFSET (type, fld))


#ifndef DEBUG
int debug;
#endif

extern int *dbt_blocks[], dbt_window[], dbt_rpb[];
extern SLONG gds_delta_alloc, gds_max_alloc;

typedef int (*DBG_PFN_V) ();
typedef int (*DBG_PFN_I) (int);

int (*dbg_all) () = DBG_all;
int (*dbg_analyze) (int) = DBG_analyze;
int (*dbg_block) (blk *) = DBG_block;
int (*dbg_eval) (int) = DBG_eval;
int (*dbg_open) () = DBG_open;
int (*dbg_close) () = DBG_close;
int (*dbg_pool) (JrdMemoryPool*) = DBG_pool;
int (*dbg_pretty) (jrd_nod *, int) = DBG_pretty;
int (*dbg_window) (int *) = DBG_window;
int (*dbg_rpb) (rpb *) = DBG_rpb;
static int (*dbg_bdbs) () = DBG_bdbs;
int (*dbg_examine) (int *) = DBG_examine;
int (*dbg_check) (int) = DBG_check;


#ifdef LINUX
IB_FILE *dbg_file = NULL;
#else
IB_FILE *dbg_file = ib_stdout;
#endif
int DBG_supervisor(int);
int *prior_frame;
static SLONG perm_pool_mem;
static SLONG req_pool_mem;
static SLONG trans_pool_mem;
static SLONG other_pool_mem;

static void go_column(int);
static void prt_dsc(DSC *, int);
static int prt_fields(SCHAR *, int *);
static int prt_que(SCHAR *, QUE);
static int rsb_pretty(RSB, int);

/* Pick up node names */

#define NODE(type, name, keyword) "name",

static TEXT *node_names[] = {
#include "../jrd/nod.h"
	0
};
#undef NODE

/* rsb types */

static TEXT *rsb_names[] = {
	"boolean",
	"cross",
	"dbkey",
	"first",
	"indexed",
	"merge",
	"multiple",
	"project",
	"sequential",
	"sort",
	"union",
	"aggregate",
	"ext_sequential",			/* External sequential access */
	"ext_indexed",				/* External indexed access */
	"ext_dbkey",
	"navigation",
	"bit_sieve",
	"left_cross",
	"procedure"
};


int DBG_all(void)
{
/**************************************
 *
 *	D B G _ a l l
 *
 **************************************
 *
 * Functional description
 *	Print all known blocks.
 *
 **************************************/

	DBB dbb = GET_DBB;

	if (!dbg_file) {
		dbg_file = ib_fopen("tt:", "w");
	}
//	if (!dbb || !(vector = dbb->dbb_pools))
	if (!dbb || !dbb->dbb_pools.size())
	{
		return TRUE;
	}
	dbb::pool_vec_type::iterator itr;
	dbb::pool_vec_type::iterator end = dbb->dbb_pools.end();
	for (itr = dbb->dbb_pools.begin(); itr < end; ++itr) {
		DBG_pool(*itr);
	}
	return TRUE;
}


int DBG_analyze(int pool_id)
{
/**************************************
 *
 *	D B G _ a n a l y z e
 *
 **************************************
 *
 * Functional description
 *	Analyze pool by block type and sub-type.
 *
 **************************************/
	DBB dbb;
	HNK hunk;
	BLK block;
	VEC vector;
	PLB pool;
	SCHAR *hunk_end;
	SSHORT type;
	SLONG length;
	SLONG total_length = 0;
	TEXT **fields;
	TEXT *node_name;
	TEXT name_padded[32];
	int pool_type;
	int i;
	struct {
		int sum_count;
		SLONG sum_length;
	} blocks[type_MAX], nodes[nod_MAX], *p, *end;

	dbb = GET_DBB;

	if (!(vector = dbb->dbb_pools))
		return TRUE;

	for (p = blocks, end = p + (int) type_MAX; p < end; p++) {
		p->sum_count = 0;
		p->sum_length = 0;
	}

	for (p = nodes, end = p + (int) nod_MAX; p < end; p++) {
		p->sum_count = 0;
		p->sum_length = 0;
	}

	if (pool = (PLB) vector->vec_object[pool_id]) {
		for (hunk = pool->plb_hunks; hunk; hunk = hunk->hnk_next) {
			hunk_end = ((char*)hunk->hnk_address) + hunk->hnk_length;
			for (block = (BLK) hunk->hnk_address; block != (BLK) hunk_end;
				 block = (BLK) ((SCHAR *) block + length)) {
				type = block->blk_type;
				length = block->blk_length << SHIFT;
				total_length += length;
				if (type <= (SSHORT) type_MIN || type >= (SSHORT) type_MAX) {
					ib_fprintf(dbg_file, "***punt***\n");
					break;
				}
				p = blocks + type;
				p->sum_count++;
				p->sum_length += length;
				if (type == (SSHORT) type_nod) {
					p = nodes + (int) ((JRD_NOD) block)->nod_type;
					p->sum_count++;
					p->sum_length += length;
				}
			}
		}
	}

	if (pool->plb_pool_id == 0) {
		pool_type = 1;
		perm_pool_mem = total_length / 1024;
		ib_fprintf(dbg_file, "\nPool %d (Permanent pool)\n",
				   pool->plb_pool_id);
	}
	else {
		for (p = blocks, end = p + (int) type_MAX, type = 0; p < end;
			 p++, type++) {
			if (p->sum_count)
				fields = reinterpret_cast < char **>(dbt_blocks[type]);
			if (!strcmp(*fields, "TRANSACTION") && p->sum_count) {
				pool_type = 2;
				trans_pool_mem += (total_length / 1024);
				ib_fprintf(dbg_file, "\nPool %d (Transaction pool)\n",
						   pool->plb_pool_id);
				break;
			}
			if (!strcmp(*fields, "REQUEST") && p->sum_count) {
				pool_type = 3;
				req_pool_mem += (total_length / 1024);
				ib_fprintf(dbg_file, "\nPool %d (Request pool)\n",
						   pool->plb_pool_id);
				break;
			}
		}
	}
	if (!pool_type) {
		other_pool_mem += (total_length / 1024);
		ib_fprintf(dbg_file, "\nPool %d\n", pool->plb_pool_id);
	}
	ib_fprintf(dbg_file,
			   "Summary by block types: (total length of pool = %ldk)\n",
			   total_length / 1024);

	for (p = blocks, end = p + (int) type_MAX, type = 0; p < end; p++, type++)
		if (p->sum_count) {
			fields = reinterpret_cast < char **>(dbt_blocks[type]);
			for (i = 0; i < 31; name_padded[i++] = ' ');
			name_padded[i] = '\0';
			for (i = 0; (*fields)[i]; i++)
				name_padded[i] = (*fields)[i];
			ib_fprintf(dbg_file, "\t%s\t%d\t%ld\n", name_padded, p->sum_count,
					   p->sum_length);
		}

	ib_fprintf(dbg_file, "Summary by node types:\n");

	for (p = nodes, end = p + (int) nod_MAX, type = 0; p < end; p++, type++)
		if (p->sum_count) {
			node_name = node_names[type];
			for (i = 0; i < 31; name_padded[i++] = ' ');
			name_padded[i] = '\0';
			for (i = 0; node_name[i]; i++)
				name_padded[i] = node_name[i];
			ib_fprintf(dbg_file, "\t%s\t%d\t%ld\n", name_padded, p->sum_count,
					   p->sum_length);
		}

	return pool_type;
}


int DBG_bdbs(void)
{
/**************************************
 *
 *	D B G _ b d b s
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	DBB dbb;
	BCB bcb;

	dbb = GET_DBB;

	bcb = dbb->dbb_bcb;
	for (unsigned int i = 0; i < bcb->bcb_count; i++)
		DBG_block(reinterpret_cast < blk * >(bcb->bcb_rpt[i].bcb_bdb));

	return TRUE;
}


int DBG_precedence(void)
{
/**************************************
 *
 *	D B G _ p r e c e d e n c e
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	DBB dbb;
	BCB bcb;
	BDB bdb;
	QUE que;
	PRE precedence;
	BDB hi_bdb;
	BDB lo_bdb;

	dbb = GET_DBB;

	bcb = dbb->dbb_bcb;
	for (unsigned int i = 0; i < bcb->bcb_count; i++) {
		bdb = bcb->bcb_rpt[i].bcb_bdb;
		if (bdb->bdb_flags || bdb->bdb_ast_flags) {
			ib_fprintf(dbg_file, "BDB %d:\tpage %"SLONGFORMAT"", i, bdb->bdb_page);
			if (bdb->bdb_flags & BDB_dirty)
				ib_fprintf(dbg_file, ", dirty");
			if (bdb->bdb_ast_flags & BDB_blocking)
				ib_fprintf(dbg_file, ", blocking");
			if (bdb->bdb_flags & BDB_writer)
				ib_fprintf(dbg_file, ", writer");
			if (bdb->bdb_flags & BDB_marked)
				ib_fprintf(dbg_file, ", marked");
			if (bdb->bdb_flags & BDB_must_write)
				ib_fprintf(dbg_file, ", must_write");
			if (bdb->bdb_flags & BDB_faked)
				ib_fprintf(dbg_file, ", faked");
			if (bdb->bdb_flags & BDB_journal)
				ib_fprintf(dbg_file, ", journal");
			if (bdb->bdb_flags & BDB_system_dirty)
				ib_fprintf(dbg_file, ", system_dirty");
			if (bdb->bdb_flags & BDB_io_error)
				ib_fprintf(dbg_file, ", io_error");
			if (bdb->bdb_flags & BDB_read_pending)
				ib_fprintf(dbg_file, ", read_pending");
			if (bdb->bdb_flags & BDB_free_pending)
				ib_fprintf(dbg_file, ", free_pending");
			if (bdb->bdb_flags & BDB_not_valid)
				ib_fprintf(dbg_file, ", not_valid");
			if (bdb->bdb_flags & BDB_db_dirty)
				ib_fprintf(dbg_file, ", db_dirty");
			if (bdb->bdb_flags & BDB_checkpoint)
				ib_fprintf(dbg_file, ", checkpoint");
			ib_fprintf(dbg_file, "\n");
			if (QUE_NOT_EMPTY(bdb->bdb_higher)) {
				ib_fprintf(dbg_file, "\tdirect higher precedence pages:");
				for (que = bdb->bdb_higher.que_forward;
					 que != &bdb->bdb_higher; que = que->que_forward) {
					precedence = BLOCK(que, PRE, pre_higher);
					hi_bdb = precedence->pre_hi;
					ib_fprintf(dbg_file, " %"SLONGFORMAT"", hi_bdb->bdb_page);
					if (precedence->pre_flags & PRE_cleared)
						ib_fprintf(dbg_file, "(cleared)");
				}
				ib_fprintf(dbg_file, "\n");
			}
			if (QUE_NOT_EMPTY(bdb->bdb_lower)) {
				ib_fprintf(dbg_file, "\tdirect lower precedence pages:");
				for (que = bdb->bdb_lower.que_forward; que != &bdb->bdb_lower;
					 que = que->que_forward) {
					precedence = BLOCK(que, PRE, pre_lower);
					lo_bdb = precedence->pre_low;
					ib_fprintf(dbg_file, " %"SLONGFORMAT"", lo_bdb->bdb_page);
					if (precedence->pre_flags & PRE_cleared)
						ib_fprintf(dbg_file, "(cleared)");
				}
				ib_fprintf(dbg_file, "\n");
			}
		}
	}

	return TRUE;
}


int DBG_block(BLK block)
{
/**************************************
 *
 *	D B G _ b l o c k
 *
 **************************************
 *
 * Functional description
 *	Print a formatted block
 *
 **************************************/
	int *fields;
	int i;
	SCHAR s[10], string[100], *p;
	DSC *desc;

#undef BLOCK
#define BLOCK(struct)	((struct) block)
	if (!block) {
		ib_fprintf(dbg_file, "*** NULL ***\n");
		return FALSE;
	}

	if (block->blk_type <= (SCHAR) type_MIN
		|| block->blk_type >= (SCHAR) type_MAX) {
		ib_fprintf(dbg_file, "%X\t*** BAD BLOCK (%d) ***\n", block,
				   block->blk_type);
		return FALSE;
	}

	if (!block->blk_length) {
		ib_fprintf(dbg_file, "%X\t*** BAD BLOCK LENGTH (%d) ***\n", block,
				   block->blk_length);
		return FALSE;
	}

	fields = dbt_blocks[block->blk_type];
	ib_fprintf(dbg_file, "\n%X\t%s (%d)", block, *fields++,
			   block->blk_length);
	if (block->blk_type == (SCHAR) type_nod)
		ib_fprintf(dbg_file, " -- %s",
				   node_names[(int) ((JRD_NOD) block)->nod_type]);

	prt_fields(reinterpret_cast < char *>(block), fields);

	switch ((enum blk_t) block->blk_type) {
	case type_vec:
		ib_fprintf(dbg_file, "\t");
		p = string;
		*p = 0;
		for (i = 0; i < ((VEC) block)->vec_count; i++) {
			sprintf(p, "%X, ", ((VEC) block)->vec_object[i]);
			if (strlen(string) > 60) {
				ib_fprintf(dbg_file, "%s\n", string);
				strcpy(string, "\t\t");
			}
			p = string + strlen(string);
		}
		ib_fprintf(dbg_file, "%s\n", string);
		break;

	case type_vcl:
		ib_fprintf(dbg_file, "\t");
		p = string;
		*p = 0;
		for (i = 0; i < ((VCL) block)->vcl_count; i++) {
			sprintf(p, "%X, ", ((VCL) block)->vcl_long[i]);
			if (strlen(string) > 60) {
				ib_fprintf(dbg_file, "%s\n", string);
				strcpy(string, "\t\t");
			}
			p = string + strlen(string);
		}
		ib_fprintf(dbg_file, "%s\n", string);
		break;

	case type_bcb:
		prt_que("Empty", &BLOCK(BCB)->bcb_empty);
		for (i = 0; i < BLOCK(BCB)->bcb_count; i++) {
			sprintf(s, "mod %d", i);
			prt_que(s, &BLOCK(BCB)->bcb_rpt[i].bcb_page_mod);
		}
		break;

	case type_bdb:
		ib_fprintf(dbg_file,
				   "\tUse count: %d, page: %d, flags: %x, ast flags: %x\n",
				   ((BDB) block)->bdb_use_count, ((BDB) block)->bdb_page,
				   ((BDB) block)->bdb_flags, ((BDB) block)->bdb_ast_flags);
		ib_fprintf(dbg_file,
				   "\tParent: %X, left: %X, right: %X, dirty mask: %X\n",
				   ((BDB) block)->bdb_parent, ((BDB) block)->bdb_left,
				   ((BDB) block)->bdb_right, ((BDB) block)->bdb_transactions);
		prt_que("Que", &BLOCK(BDB)->bdb_que);
		prt_que("Higher", &BLOCK(BDB)->bdb_higher);
		prt_que("Lower", &BLOCK(BDB)->bdb_lower);
		break;

	case type_pre:
		prt_que("Higher", &BLOCK(PRE)->pre_higher);
		prt_que("Lower", &BLOCK(PRE)->pre_lower);
		break;

	case type_fmt:
		ib_fprintf(dbg_file, "\t");
		for (i = 0, desc = BLOCK(FMT)->fmt_desc;
			 i < BLOCK(FMT)->fmt_count; desc++, i++) {
			prt_dsc(desc, (i % 4) * 20);
			if (i % 4 == 3)
				ib_fprintf(dbg_file, "\n\t");
		}
		ib_fprintf(dbg_file, "\n");
		break;
        default:    /* Shut up compiler warnings */
                break;
	}

	return TRUE;
}


int DBG_check(int pool_id)
{
/**************************************
 *
 *	D B G _ c h e c k
 *
 **************************************
 *
 * Functional description
 *	Check pool for integrity.
 *
 **************************************/
	DBB dbb;
	HNK hunk;
	BLK block;
	VEC vector;
	PLB pool;
	SCHAR *hunk_end;
	USHORT corrupt;

	dbb = GET_DBB;

	corrupt = 0;

	if (!(vector = dbb->dbb_pools))
		return corrupt;

	if ( (pool = (PLB) vector->vec_object[pool_id]) ) {
		for (hunk = pool->plb_hunks; hunk; hunk = hunk->hnk_next) {
			hunk_end = ((char*)hunk->hnk_address) + hunk->hnk_length;
			for (block = (BLK) hunk->hnk_address; block != (BLK) hunk_end;
				 block =
				 (BLK) ((SCHAR *) block + (block->blk_length << SHIFT))) {
				if (block->blk_pool_id != (UCHAR) pool_id) {
					ib_fprintf(dbg_file, "%X\t*** BAD POOL ID (%d) ***\n",
							   block, block->blk_pool_id);
					++corrupt;
					break;
				}
				if (block->blk_type <= (SCHAR) type_MIN
					|| block->blk_type >= (SCHAR) type_MAX) {
					ib_fprintf(dbg_file, "%X\t*** BAD BLOCK (%d) ***\n",
							   block, block->blk_type);
					++corrupt;
					break;
				}
				if (!block->blk_length) {
					ib_fprintf(dbg_file,
							   "%X\t*** BAD BLOCK LENGTH (%d) ***\n", block,
							   block->blk_length);
					++corrupt;
					break;
				}

			}
		}
	}

	return corrupt;
}


int DBG_close(void)
{
/**************************************
 *
 *	D B G _ c l o s e
 *
 **************************************
 *
 * Functional description
 *	Close the debugging file.
 *
 **************************************/

	ib_fprintf(dbg_file, "\014\014");
	ib_fclose(dbg_file);
	dbg_file = ib_stdout;
	return TRUE;
}


int DBG_eval(int n)
{
/**************************************
 *
 *	D B G _ e v a l
 *
 **************************************
 *
 * Functional description
 *	Examine a value.
 *
 **************************************/

	ib_fprintf(dbg_file, "octal = %X, decimal = %d, hex = %x\n", n, n, n);
	return TRUE;
}


int DBG_examine(int *n)
{
/**************************************
 *
 *	D B G _ e x a m i n e
 *
 **************************************
 *
 * Functional description
 *	Examine a value.
 *
 **************************************/

	ib_fprintf(dbg_file, "octal = %X, decimal = %d, hex = %x\n", *n, *n, *n);
	return TRUE;
}


int DBG_init(void)
{
/**************************************
 *
 *	D B G _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

/*
sigset  (2, &DBG_supervisor);
*/
	return TRUE;
}


int DBG_open(void)
{
/**************************************
 *
 *	D B G _ o p e n
 *
 **************************************
 *
 * Functional description
 *	Open a debugging output file.
 *
 **************************************/
	SCHAR filename[64];

	ib_printf("Debug file: ");
	ib_scanf("%s", filename);
	dbg_file = ib_fopen(filename, "w");
	return TRUE;
}


int DBG_pool(JrdMemoryPool *pool)
{
/**************************************
 *
 *	D B G _ p o o l
 *
 **************************************
 *
 * Functional description
 *	Print all known blocks.
 *
 **************************************/
	pool->print_memory_pool_info(dbg_file, 0, DBG_block);
	return TRUE;
}


int DBG_pretty(JRD_NOD node, int column)
{
/**************************************
 *
 *	D B G _ p r e t t y
 *
 **************************************
 *
 * Functional description
 *	Pretty print a node tree.
 *
 **************************************/
	RSE rse;
	JRD_REL relation;
	JRD_PRC procedure;
	JRD_NOD *ptr, *end;
	IRB retrieval;
	int i;

#define NODE(struct)	((struct) node)

	if (node && node->blk_type == (SCHAR) type_rsb)
		return rsb_pretty(reinterpret_cast < Rsb * >(node), column);

	ib_fprintf(dbg_file, "%8X\t", node);
	for (i = 0; i < column; i++)
		ib_putc(' ', dbg_file);

	if (node == NULL)
		return ib_fprintf(dbg_file, "*** null ***\n");

	if (node->blk_type != (SCHAR) type_nod)
		return ib_fprintf(dbg_file, "*** bad node ***\n");

	ib_fprintf(dbg_file, "%s (%"SLONGFORMAT")", node_names[(int) node->nod_type],
			   node->nod_impure);
	column += 4;

	switch (node->nod_type) {
	case nod_rse:
		rse = (RSE) node;
		ib_fprintf(dbg_file, "\n");
		if (rse->rse_rsb)
			DBG_pretty(reinterpret_cast < jrd_nod * >(rse->rse_rsb), column);
		else {
			DBG_pretty(rse->rse_first, column);
			DBG_pretty(rse->rse_boolean, column);
			DBG_pretty(rse->rse_sorted, column);
			DBG_pretty(rse->rse_projection, column);
			for (ptr = rse->rse_relation, end = ptr + rse->rse_count;
				 ptr < end; ptr++)
				DBG_pretty(*ptr, column);
		}
		break;

	case nod_argument:
		ib_fprintf(dbg_file, ", id: %d, message: %X\n",
				   node->nod_arg[e_arg_number], node->nod_arg[e_arg_message]);
		if (node->nod_arg[e_arg_flag]) {
			for (i = 0; i < column + 10; i++)
				ib_putc(' ', dbg_file);
			ib_fprintf(dbg_file, "flag:\n");
			DBG_pretty(node->nod_arg[e_arg_flag], column);
		}
		if (node->nod_arg[e_arg_indicator]) {
			for (i = 0; i < column + 10; i++)
				ib_putc(' ', dbg_file);
			ib_fprintf(dbg_file, "indicator:\n");
			DBG_pretty(node->nod_arg[e_arg_flag], column);
		}
		return TRUE;

	case nod_message:
		ib_fprintf(dbg_file, ", number: %d, format: %X\n",
				   node->nod_arg[e_msg_number], node->nod_arg[e_msg_format]);
		return TRUE;

	case nod_field:
		ib_fprintf(dbg_file, ", stream: %d, id: %d\n",
				   node->nod_arg[e_fld_stream], node->nod_arg[e_fld_id]);
		return TRUE;

	case nod_index:
		retrieval = (IRB) node->nod_arg[e_idx_retrieval];
		ib_fprintf(dbg_file, ", id: %d\n", retrieval->irb_index);
		for (ptr = retrieval->irb_value, end =
			 ptr + retrieval->irb_lower_count; ptr < end; ptr++)
			DBG_pretty(*ptr, column);
		for (end = ptr + retrieval->irb_upper_count; ptr < end; ptr++)
			DBG_pretty(*ptr, column);
		return TRUE;

	case nod_relation:
		relation = (JRD_REL) node->nod_arg[e_rel_relation];
		ib_fprintf(dbg_file, ", stream: %d, %s (%X)\n",
				   node->nod_arg[e_rel_stream], relation->rel_name, relation);
		return TRUE;

	case nod_procedure:
		SSHORT procedure_id = (SSHORT)(SLONG) node->nod_arg[e_prc_procedure];
		ib_fprintf(dbg_file, ", stream: %d, prc_id: %d\n",
				   node->nod_arg[e_prc_stream], procedure_id);
		if (node->nod_arg[e_prc_inputs])
			DBG_pretty(node->nod_arg[e_prc_inputs], column);
		return TRUE;

	case nod_exec_proc:
		procedure = (JRD_PRC) node->nod_arg[e_esp_procedure];
		ib_fprintf(dbg_file, ", name: %s (%X)\n",
				   procedure->prc_name->str_data, procedure);
		for (ptr = node->nod_arg, end = ptr + node->nod_count; ptr < end;
			 ptr++)
			DBG_pretty(*ptr, column);
		return TRUE;

	case nod_union:
		ib_fprintf(dbg_file, ", stream: %d\n", node->nod_arg[e_uni_stream]);
		DBG_pretty(node->nod_arg[e_uni_clauses], column);
		return TRUE;

	case nod_aggregate:
		ib_fprintf(dbg_file, ", stream: %d\n", node->nod_arg[e_agg_stream]);
		DBG_pretty(node->nod_arg[e_agg_rse], column);
		DBG_pretty(node->nod_arg[e_agg_group], column);
		DBG_pretty(node->nod_arg[e_agg_map], column);
		return TRUE;

	case nod_max:
	case nod_min:
	case nod_average:
	case nod_total:
	case nod_count:
		ib_fprintf(dbg_file, "\n");
		DBG_pretty(node->nod_arg[e_stat_rse], column);
		DBG_pretty(node->nod_arg[e_stat_value], column);
		DBG_pretty(node->nod_arg[e_stat_default], column);
		return TRUE;

	default:
		ib_fprintf(dbg_file, "\n");
		for (ptr = node->nod_arg, end = ptr + node->nod_count; ptr < end;
			 ptr++)
			DBG_pretty(*ptr, column);
	}

	if (node->nod_type == nod_for && node->nod_arg[e_for_rsb])
		rsb_pretty(reinterpret_cast < Rsb * >(node->nod_arg[e_for_rsb]),
				   column);
	return TRUE;

    return FALSE;
}

int DBG_supervisor(int arg)
{
/**************************************
 *
 *	D B G _ s u p e r v i s o r
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	prior_frame = (int *) *(&arg - 2);

	debug = 0;

#ifndef VMS
	ib_fprintf(dbg_file, "\nEntering JRD diagnostic DBG_supervisor\n");
	int yyparse();
	yyparse();
	ib_fprintf(dbg_file, "\nLeaving JRD diagnostic DBG_supervisor\n");
	DBG_init();
#endif

	return TRUE;
}


int DBG_rpb(RPB * rpb)
{
/**************************************
 *
 *	D B G _ r p b
 *
 **************************************
 *
 * Functional description
 *	Print a record paramter block
 *
 **************************************/

	ib_fprintf(dbg_file, "\n%X\tRECORD PARAMETER BLOCK", rpb);
	prt_fields(reinterpret_cast < char *>(rpb), dbt_rpb);
	DBG_window(reinterpret_cast < int *>(&rpb->rpb_window));
	return TRUE;
}


int DBG_smb(SMB smb, int column)
{
/**************************************
 *
 *	D B G _ s m b
 *
 **************************************
 *
 * Functional description
 *	Pretty print an smb (Sort Memory Block)
 *
 **************************************/

	smb::smb_repeat * ptr;
	int i;

	go_column(column);
	ib_fprintf(dbg_file, "SMB\n");
	go_column(column);
	ib_fprintf(dbg_file,
			   "keys = %d, count = %d length = %d, key_length = %d\n",
			   smb->smb_keys, smb->smb_count, smb->smb_length,
			   smb->smb_key_length);
	for (i = 0; i < smb->smb_keys; i++) {
		go_column(column + 2);
		ib_fprintf(dbg_file,
				   "key [%d] dtype = %d flags = %d length = %d offset = %d, vary_offset = %d\n",
				   i,
				   smb->smb_key_desc[i].skd_dtype,
				   smb->smb_key_desc[i].skd_flags,
				   smb->smb_key_desc[i].skd_length,
				   smb->smb_key_desc[i].skd_offset,
				   smb->smb_key_desc[i].skd_vary_offset);
	}
	for (i = 0; i < smb->smb_count; i++) {
		ptr = &smb->smb_rpt[i];
		go_column(column + 2);
		ib_fprintf(dbg_file, "fld [%d] flag = %d stream = %d field = %d\n",
				   i, ptr->smb_flag_offset, ptr->smb_stream,
				   ptr->smb_field_id);
		prt_dsc(&ptr->smb_desc, column + 4);
		DBG_pretty(ptr->smb_node, column + 4);
	}
	return TRUE;
}


int DBG_verify(void)
{
/**************************************
 *
 *	D B G _ v e r i f y
 *
 **************************************
 *
 * Functional description
 *	Verify integrity of all pools.
 *
 **************************************/
	DBB dbb;
	VEC vector;
	int i;

	dbb = GET_DBB;

	if (!dbg_file)
		dbg_file = ib_fopen("tt:", "w");

	if (!dbb || !(vector = dbb->dbb_pools))
		return TRUE;

	for (i = 0; i < vector->vec_count; i++)
		DBG_check(i);

	return TRUE;
}


int DBG_window(int *window)
{
/**************************************
 *
 *	D B G _ w i n d o w
 *
 **************************************
 *
 * Functional description
 *	Print a window paramter block
 *
 **************************************/
	ib_fprintf(dbg_file, "\n%X\tWINDOW", window);
	prt_fields(reinterpret_cast < char *>(window), dbt_window);
	return TRUE;
}


int DBG_memory(void)
{
/**************************************
 *
 *	D B G _ m e m o r y
 *
 **************************************
 *
 * Functional description
 *	Print memory usage
 *
 **************************************/
	DBB dbb;
	VEC vector;
	PLB pool;
	int pool_id;
	int trans_pools = 0;
	int req_pools = 0;
	int other_pools = 0;
	int pool_type;


	dbb = GET_DBB;

	ib_fprintf(dbg_file, "MEMORY UTILIZATION for database\n\n");
#ifdef V4_THREADING
	V4_RW_LOCK_LOCK(dbb->dbb_rw_locks + DBB_WLCK_pools, WLCK_read);
#endif
/* walk through all the pools in the database */

	perm_pool_mem = 0;
	req_pool_mem = 0;
	trans_pool_mem = 0;
	other_pool_mem = 0;
	vector = dbb->dbb_pools;
	for (pool_id = 0; pool_id < vector->vec_count; pool_id++) {
		pool = (PLB) vector->vec_object[pool_id];
		if (!pool)
			continue;
		pool_type = DBG_analyze(pool_id);
		switch (pool_type) {
		case 1:
			break;
		case 2:
			trans_pools++;
			break;
		case 3:
			req_pools++;
			break;
		default:
			other_pools++;
			break;
		}
	}

	ib_fprintf(dbg_file, "\nTotal memory used in the PERMANENT pool = %ldk\n",
			   perm_pool_mem);
	ib_fprintf(dbg_file,
			   "Total memory used in the %d TRANSACTION pools combined = %ldk\n",
			   trans_pools, trans_pool_mem);
	ib_fprintf(dbg_file,
			   "Total memory used in the %d REQUEST pools combined = %ldk\n",
			   req_pools, req_pool_mem);
	ib_fprintf(dbg_file,
			   "Total memory used in the %d OTHER pools combined = %ldk\n",
			   other_pools, other_pool_mem);
	ib_fprintf(dbg_file,
			   "Total memory used in all the pools combined = %ldk\n",
			   trans_pool_mem + req_pool_mem + other_pool_mem +
			   perm_pool_mem);

	ib_fprintf(dbg_file, "\nMemory malloc-ed = %ldk\n", gds_max_alloc / 1024);
	ib_fprintf(dbg_file, "Memory used of the malloc-ed memory = %ldk\n",
			   gds_delta_alloc / 1024);

#ifdef V4_THREADING
	V4_RW_LOCK_UNLOCK(dbb->dbb_rw_locks + DBB_WLCK_pools);
#endif
	return TRUE;
}


static void go_column(int column)
{
/**************************************
 *
 *	g o _ c o l u m n
 *
 **************************************
 *
 * Functional description
 *	Utility function to print a bunch of spaces.
 *
 **************************************/

	while (column-- > 0)
		ib_fprintf(dbg_file, " ");
}


static void prt_dsc(DSC * desc, int column)
{
/**************************************
 *
 *	p r t _ d s c 
 *
 **************************************
 *
 * Functional description
 *	Pretty print an dsc.
 *
 **************************************/
	go_column(column);
	ib_fprintf(dbg_file, "(a=%d, t=%d, l=%d, scale=%d, subtype=%d)\n",
			   desc->dsc_address, desc->dsc_dtype, desc->dsc_length,
			   desc->dsc_scale, desc->dsc_sub_type);
}


static int prt_fields(SCHAR * block, int *fields)
{
/**************************************
 *
 *	p r t _ f i e l d s
 *
 **************************************
 *
 * Functional description
 *	Print structured block.
 *
 **************************************/
	int length, column, offset;
	TEXT *string, *ptr, *p, s[80];

	column = 99;

	while ( (string = (TEXT *) * fields++) ) {
		offset = *fields++;
		length = *fields++;
		ptr = (SCHAR *) block + offset;
		switch (length) {
		case 0:
		case 1:
			sprintf(s, string, *ptr);
			break;

		case 2:
			sprintf(s, string, *(SSHORT *) ptr);
			break;

		case 4:
			sprintf(s, string, *(SLONG *) ptr);
			break;
		}
		for (p = s, length = 0; *p++;)
			length++;
		if ((column += length + 1) >= 60) {
			ib_fprintf(dbg_file, "\n\t");
			column = length + 1;
		}
		ib_fprintf(dbg_file, "%s ", s);
	}

	ib_fprintf(dbg_file, "\n");
	return TRUE;
}


static int prt_que(SCHAR * string, QUE que)
{
/**************************************
 *
 *	p r t _ q u e
 *
 **************************************
 *
 * Functional description
 *	Print a formatted que entry.
 *
 **************************************/

	ib_fprintf(dbg_file, "\t%X %s forward: %X, backward: %X\n",
			   que, string, que->que_forward, que->que_backward);
	return TRUE;
}


static int rsb_pretty(RSB rsb, int column)
{
/**************************************
 *
 *	r s b _ p r e t t y
 *
 **************************************
 *
 * Functional description
 *	Pretty print an rsb tree.
 *
 **************************************/
	JRD_REL relation;
	RSB *ptr, *end;
	USHORT i;

	ib_fprintf(dbg_file, "%X\t", rsb);
	for (i = 0; i < column; i++)
		ib_putc(' ', dbg_file);

	if (rsb == NULL)
		return ib_fprintf(dbg_file, "*** null ***\n");

	if (rsb->blk_type != (SCHAR) type_rsb)
		return ib_fprintf(dbg_file, "*** bad rsb ***\n");

	ib_fprintf(dbg_file, "%s (%d), stream: %d",
			   rsb_names[(int) rsb->rsb_type], rsb->rsb_impure,
			   rsb->rsb_stream);

	if ( (relation = rsb->rsb_relation) )
		ib_fprintf(dbg_file, " %s", relation->rel_name);

	column += 4;

	ib_fprintf(dbg_file, "\n");

	if (rsb->rsb_type == rsb_merge)
		for (ptr = rsb->rsb_arg, end = ptr + rsb->rsb_count * 2; ptr < end;
			 ptr += 2)
			DBG_pretty(reinterpret_cast < jrd_nod * >(*ptr), column);
	else if (rsb->rsb_type != rsb_left_cross)
		for (ptr = rsb->rsb_arg, end = ptr + rsb->rsb_count; ptr < end; ptr++)
			DBG_pretty(reinterpret_cast < jrd_nod * >(*ptr), column);
	else
		for (ptr = rsb->rsb_arg, end = ptr + rsb->rsb_count + 1; ptr < end;
			 ptr++)
			DBG_pretty(reinterpret_cast < jrd_nod * >(*ptr), column);

	if (rsb->rsb_next)
		DBG_pretty(reinterpret_cast < jrd_nod * >(rsb->rsb_next), column);
	return TRUE;
}


void yyerror(const char *string)
{
/**************************************
 *
 *	y y e r r o r
 *
 **************************************
 *
 * Functional description
 *	YACC error function.  Boring.
 *
 **************************************/

	ib_fprintf(dbg_file, "%s\n", string);
}


int yywrap(void)
{
/**************************************
 *
 *	 y y w r a p
 *
 **************************************
 *
 * Functional description
 *	Wrapup function for YACC.
 *
 **************************************/

	return (1);
}

