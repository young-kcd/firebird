//____________________________________________________________
//  
//		PROGRAM:	C preprocessor
//		MODULE:		msc.cpp
//		DESCRIPTION:	Miscellaneous little stuff
//  
//  The contents of this file are subject to the Interbase Public
//  License Version 1.0 (the "License"); you may not use this file
//  except in compliance with the License. You may obtain a copy
//  of the License at http://www.Inprise.com/IPL.html
//  
//  Software distributed under the License is distributed on an
//  "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
//  or implied. See the License for the specific language governing
//  rights and limitations under the License.
//  
//  The Original Code was created by Inprise Corporation
//  and its predecessors. Portions created by Inprise Corporation are
//  Copyright (C) Inprise Corporation.
//  
//  All Rights Reserved.
//  Contributor(s): ______________________________________.
//  TMN (Mike Nordell) 11.APR.2001 - Reduce compiler warnings
//  
//
//____________________________________________________________
//
//	$Id: msc.cpp,v 1.9 2003-09-06 00:52:10 brodsom Exp $
//
//  
//$Id: msc.cpp,v 1.9 2003-09-06 00:52:10 brodsom Exp $
//  

// ***************************************************
//  THIS MODULE HAS SEVERAL KISSING COUSINS; IF YOU
//  SHOULD CHANGE ONE OF THE MODULES IN THE FOLLOWING
//  LIST, PLEASE BE SURE TO CHECK THE OTHERS FOR
//  SIMILAR CHANGES:
//  
//        gds/maint/pyxis/all.c
//                  dsql/all.c
//                  jrd/all.c
//                  pipe/allp.c
//                  qli/all.c
//                  remote/allr.c
//                  gpre/msc.c
//  
//  - THANK YOU
//**************************************************

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <string.h>
#include "../gpre/gpre.h"
#include "../gpre/parse.h"
#include "../gpre/gpre_proto.h"
#include "../gpre/msc_proto.h"
#include "../jrd/gds_proto.h"


extern ACT cur_routine;

typedef struct spc {
	spc* spc_next;
	SLONG spc_remaining;
} *SPC;

static SPC space, permanent_space;
static LLS free_lls;


//____________________________________________________________
//  
//		Make an action and link it to a request.
//  

ACT MSC_action( GPRE_REQ request, enum act_t type)
{
	ACT action;

	action = (ACT) ALLOC(ACT_LEN);
	action->act_type = type;

	if (request) {
		action->act_next = request->req_actions;
		request->req_actions = action;
		action->act_request = request;
	}

	return action;
}


//____________________________________________________________
//  
//  

UCHAR *MSC_alloc(int size)
{
	SPC next;
	int n;
	UCHAR *blk, *p, *end;

	size = FB_ALIGN(size, ALIGNMENT);

	if (!space || size > space->spc_remaining) {
		n = MAX(size, 4096);
		if (!(next = (SPC) gds__alloc((SLONG) (n + sizeof(spc)))))
			CPR_error("virtual memory exhausted");
#ifdef DEBUG_GDS_ALLOC
		/* For V4.0 we don't care about gpre specific memory leaks */
		gds_alloc_flag_unfreed(next);
#endif
		next->spc_next = space;
		next->spc_remaining = n;
		space = next;
	}

	space->spc_remaining -= size;
	p = blk = ((UCHAR *) space + sizeof(spc) + space->spc_remaining);
	end = p + size;

	while (p < end)
		*p++ = 0;

	return blk;
}


//____________________________________________________________
//  
//		Allocate a block in permanent memory.
//  

UCHAR *MSC_alloc_permanent(int size)
{
	SPC next;
	int n;
	UCHAR *blk, *p, *end;

	size = FB_ALIGN(size, ALIGNMENT);

	if (!permanent_space || size > permanent_space->spc_remaining) {
		n = MAX(size, 4096);
		if (!(next = (SPC) gds__alloc((SLONG) (n + sizeof(spc)))))
			CPR_error("virtual memory exhausted");
#ifdef DEBUG_GDS_ALLOC
		/* For V4.0 we don't care about gpre specific memory leaks */
		gds_alloc_flag_unfreed(next);
#endif
		next->spc_next = permanent_space;
		next->spc_remaining = n;
		permanent_space = next;
	}

	permanent_space->spc_remaining -= size;
	p = blk =
		((UCHAR *) permanent_space + sizeof(spc) +
		 permanent_space->spc_remaining);
	end = p + size;

	while (p < end)
		*p++ = 0;

	return blk;
}


//____________________________________________________________
//  
//		Make a binary node.
//  

GPRE_NOD MSC_binary(NOD_T type, GPRE_NOD arg1, GPRE_NOD arg2)
{
	GPRE_NOD node;

	node = MSC_node(type, 2);
	node->nod_arg[0] = arg1;
	node->nod_arg[1] = arg2;

	return node;
}


//____________________________________________________________
//  
//		Make a new context for a request and link it up to the request.
//  

GPRE_CTX MSC_context(GPRE_REQ request)
{
	GPRE_CTX context;

//  allocate and initialize 

	context = (GPRE_CTX) ALLOC(CTX_LEN);
	context->ctx_request = request;
	context->ctx_internal = request->req_internal++;
	context->ctx_scope_level = request->req_scope_level;

//  link in with the request block 

	context->ctx_next = request->req_contexts;
	request->req_contexts = context;

	return context;
}


//____________________________________________________________
//  
//		Copy one string into another.
//  

void MSC_copy( char *from, int length, char *to)
{

	if (length)
		do
			*to++ = *from++;
		while (--length);

	*to = 0;
}

//____________________________________________________________
//  
//		Copy two strings into another.
//  

void MSC_copy_cat( char *from1, int length1, char *from2, int length2, char *to)
{

	if (length1)
		do
			*to++ = *from1++;
		while (--length1);
	if (length2)
		do
			*to++ = *from2++;
		while (--length2);

	*to = 0;
}

//____________________________________________________________
//  
//		Find a symbol of a particular type.
//  

SYM MSC_find_symbol(SYM symbol, enum sym_t type)
{

	for (; symbol; symbol = symbol->sym_homonym)
		if (symbol->sym_type == type)
			return symbol;

	return NULL;
}


//____________________________________________________________
//  
//		Free a block.
//  

void MSC_free( UCHAR * block)
{

}


//____________________________________________________________
//  
//		Get rid of an erroroneously allocated request block.
//  

void MSC_free_request( GPRE_REQ request)
{

	requests = request->req_next;
	cur_routine->act_object = (REF) request->req_routine;
	MSC_free((UCHAR *) request);
}


//____________________________________________________________
//  
//		Initialize (or more properly, re-initialize) the memory
//		allocator.
//  

void MSC_init(void)
{
	SPC stuff;

	free_lls = NULL;

	while (stuff = space) {
		space = space->spc_next;
		gds__free(stuff);
	}
}


//____________________________________________________________
//  
//		Match the current token against a keyword.  If successful,
//		advance the token stream and return true.  Otherwise return
//		false.
//  

BOOLEAN MSC_match(KWWORDS keyword)
{

	if (token.tok_keyword == KW_none && token.tok_symbol) {
		SYM symbol;
		for (symbol = token.tok_symbol->sym_collision; symbol;
			 symbol = symbol->sym_collision)
		{
			if ((strcmp(symbol->sym_string, token.tok_string) ==
								 0) && symbol->sym_keyword != KW_none) 
			{
				token.tok_symbol = symbol;
				token.tok_keyword = static_cast < kwwords > (symbol->sym_keyword);
			}
		}
	}

	if ((int) token.tok_keyword != (int) keyword)
		return FALSE;

	CPR_token();

	return TRUE;
}


//____________________________________________________________
//  
//		Determinate where a specific object is
//		represented on a linked list stack.
//  

BOOLEAN MSC_member(GPRE_NOD object, LLS stack)
{

	for (; stack; stack = stack->lls_next)
		if (stack->lls_object == object)
			return TRUE;

	return FALSE;
}


//____________________________________________________________
//  
//		Allocate an initialize a syntax node.
//  

GPRE_NOD MSC_node(enum nod_t type, SSHORT count)
{
	GPRE_NOD node;

	node = (GPRE_NOD) ALLOC(NOD_LEN(count));
	node->nod_count = count;
	node->nod_type = type;

	return node;
}


//____________________________________________________________
//  
//		Pop an item off a linked list stack.  Free the stack node.
//  

GPRE_NOD MSC_pop(LLS * pointer)
{
	LLS stack;
	GPRE_NOD node;

	stack = *pointer;
	node = stack->lls_object;
	*pointer = stack->lls_next;

	stack->lls_next = free_lls;
	free_lls = stack;

	return node;
}


//____________________________________________________________
//  
//       Allocate a new privilege (grant/revoke) block.
//  

PRV MSC_privilege_block(void)
{
	PRV privilege_block;

	privilege_block = (PRV) ALLOC(PRV_LEN);
	privilege_block->prv_privileges = PRV_no_privs;
	privilege_block->prv_username = 0;
	privilege_block->prv_relation = 0;
	privilege_block->prv_fields = 0;
	privilege_block->prv_next = 0;

	return privilege_block;
}


//____________________________________________________________
//  
//		Push an arbitrary object onto a linked list stack.
//  

void MSC_push( GPRE_NOD object, LLS * pointer)
{
	LLS stack;

	if (stack = free_lls)
		free_lls = stack->lls_next;
	else
		stack = (LLS) ALLOC(LLS_LEN);

	stack->lls_object = object;
	stack->lls_next = *pointer;
	*pointer = stack;
}


//____________________________________________________________
//  
//		Generate a reference and possibly link the reference into
//		a linked list.
//  

REF MSC_reference(REF * link)
{
	REF reference;

	reference = (REF) ALLOC(REF_LEN);

	if (link) {
		reference->ref_next = *link;
		*link = reference;
	}

	return reference;
}


//____________________________________________________________
//  
//		Set up for a new request.  Make request and action
//		blocks, all linked up and ready to go.
//  

GPRE_REQ MSC_request(enum req_t type)
{
	GPRE_REQ request;

	request = (GPRE_REQ) ALLOC(REQ_LEN);
	request->req_type = type;
	request->req_next = requests;
	requests = request;

	request->req_routine = (GPRE_REQ) cur_routine->act_object;
	cur_routine->act_object = (REF) request;

	if (!(cur_routine->act_flags & ACT_main))
		request->req_flags |= REQ_local;
	if (sw_sql_dialect <= SQL_DIALECT_V5)
		request->req_flags |= REQ_blr_version4;

	return request;
}


//____________________________________________________________
//  
//		Copy a string into a permanent block.
//  

SCHAR *MSC_string(TEXT * input)
{
	TEXT *string;

	string = (TEXT *) ALLOC(strlen(input) + 1);
	strcpy(string, input);

	return string;
}


//____________________________________________________________
//  
//		Allocate and initialize a symbol block.
//  

SYM MSC_symbol(enum sym_t type, TEXT * string, USHORT length, GPRE_CTX object)
{
	SYM symbol;
	TEXT *p;

	symbol = (SYM) ALLOC(SYM_LEN + length);
	symbol->sym_type = type;
	symbol->sym_object = object;
	p = symbol->sym_string = symbol->sym_name;

	if (length)
		do
			*p++ = *string++;
		while (--length);

	return symbol;
}


//____________________________________________________________
//  
//		Make a unary node.
//  

GPRE_NOD MSC_unary(NOD_T type, GPRE_NOD arg)
{
	GPRE_NOD node;

	node = MSC_node(type, 1);
	node->nod_arg[0] = arg;

	return node;
}


//____________________________________________________________
//  
//		Set up for a new username.
//  

USN MSC_username(SCHAR * name, USHORT name_dyn)
{
	USN username;

	username = (USN) MSC_alloc(USN_LEN);
	username->usn_name = (SCHAR *) MSC_alloc(strlen(name) + 1);
	strcpy(username->usn_name, name);
	username->usn_dyn = name_dyn;

	username->usn_next = 0;

	return username;
}

