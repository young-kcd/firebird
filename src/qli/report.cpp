/*
 *	PROGRAM:	JRD Command Oriented Query Language
 *	MODULE:		report.c
 *	DESCRIPTION:	Report writer runtime control
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
#include "../qli/dtr.h"
#include "../qli/exe.h"
#include "../qli/compile.h"
#include "../qli/report.h"
#include "../qli/all_proto.h"
#include "../qli/eval_proto.h"
#include "../qli/exe_proto.h"
#include "../qli/forma_proto.h"
#include "../qli/mov_proto.h"
#include "../qli/repor_proto.h"

static void bottom_break(BRK, PRT);
static void increment_break(BRK);
static void initialize_break(BRK);
static bool test_break(BRK, RPT, QLI_MSG);
static void top_break(BRK, PRT);
static void top_of_page(PRT, bool);

#define SWAP(a,b)	{temp = a; a = b; b = temp;}


void RPT_report( QLI_NOD loop)
{
/**************************************
 *
 *	R P T _ r e p o r t
 *
 **************************************
 *
 * Functional description
 *	Execute a FOR loop.  This may require that a request get
 *	started, a message sent, and a message received for each
 *	record.  At the other end of the spectrum, there may be
 *	absolutely nothing to do.
 *
 **************************************/
	RPT report;
	QLI_NOD node;
	QLI_REQ request;
	QLI_MSG message;
	DSC *desc;
	BRK control;
	STR string;
	PRT print;
	UCHAR *temp;

// Get to actual report node 

	node = loop->nod_arg[e_for_statement];
	report = (RPT) node->nod_arg[e_prt_list];
	print = (PRT) node->nod_arg[e_prt_output];
	print->prt_new_page = (int(*)()) top_of_page;
	print->prt_page_number = 0;

// Get to actual report node 

/* If there is a request associated  with the loop, start it and possibly
   send a message slong with it. */

	if (request = (QLI_REQ) loop->nod_arg[e_for_request])
		EXEC_start_request(request, (QLI_MSG) loop->nod_arg[e_for_send]);
	else if (message = (QLI_MSG) loop->nod_arg[e_for_send])
		EXEC_send(message);

/* Receive messages in a loop until the end of file field comes up
   true. */

	message = (QLI_MSG) loop->nod_arg[e_for_receive];

/* Get the first record of the record.  If there isn't anything,
   don't worry about anything. */

	desc = EXEC_receive(message, (PAR) loop->nod_arg[e_for_eof]);
	if (*(USHORT *) desc->dsc_address)
		return;

	if (!report->rpt_buffer) {
		string = (STR) ALLOCDV(type_str, message->msg_length);
		report->rpt_buffer = (UCHAR *) string->str_data;
	}

	MOVQ_fast((SCHAR*) message->msg_buffer, (SCHAR*) report->rpt_buffer,
			  (SLONG) message->msg_length);

	if (control = report->rpt_top_rpt)
		FMT_print((QLI_NOD) control->brk_line, print);

	top_of_page(print, true);

	initialize_break(report->rpt_bottom_breaks);
	initialize_break(report->rpt_bottom_page);
	initialize_break(report->rpt_bottom_rpt);

// Force TOP breaks for all fields 

	for (control = report->rpt_top_breaks; control;
		 control = control->brk_next) FMT_print((QLI_NOD) control->brk_line, print);

	for (;;) {
		/* Check for bottom breaks.  If we find one, force all lower breaks. */

		for (control = report->rpt_bottom_breaks; control;
			 control = control->brk_next)
		{
			if (test_break(control, report, message)) {
				SWAP(message->msg_buffer, report->rpt_buffer);
				bottom_break(control, print);
				SWAP(message->msg_buffer, report->rpt_buffer);
				initialize_break(control);
				break;
			}
		}

		if (print->prt_lines_remaining <= 0)
			top_of_page(print, false);

		/* Now check for top breaks. */

		for (control = report->rpt_top_breaks; control;
			 control = control->brk_next)
		{
			if (test_break(control, report, message)) {
				top_break(control, print);
				break;
			}
		}

		/* Increment statisticals and print detail line, if any */

		increment_break(report->rpt_bottom_breaks);
		increment_break(report->rpt_bottom_page);
		increment_break(report->rpt_bottom_rpt);

		if (node = report->rpt_detail_line)
			FMT_print(node, print);

		/* Get the next record.  If we're at end, we're almost done. */

		SWAP(message->msg_buffer, report->rpt_buffer);
		desc = EXEC_receive(message, (PAR) loop->nod_arg[e_for_eof]);
		if (*(USHORT *) desc->dsc_address)
			break;
	}

// Force BOTTOM breaks for all fields 

	SWAP(message->msg_buffer, report->rpt_buffer);
	bottom_break(report->rpt_bottom_breaks, print);
	bottom_break(report->rpt_bottom_rpt, print);

	if (control = report->rpt_bottom_page)
		FMT_print((QLI_NOD) control->brk_line, print);
}


static void bottom_break( BRK control, PRT print)
{
/**************************************
 *
 *	b o t t o m  _ b r e a k
 *
 **************************************
 *
 * Functional description
 *	Force all lower breaks then take break.
 *
 **************************************/
	LLS stack;

	if (!control)
		return;

	if (control->brk_next)
		bottom_break(control->brk_next, print);

	for (stack = control->brk_statisticals; stack; stack = stack->lls_next)
		EVAL_break_compute((QLI_NOD) stack->lls_object);

	FMT_print((QLI_NOD) control->brk_line, print);
}


static void increment_break( BRK control)
{
/**************************************
 *
 *	i n c r e m e n t _ b r e a k
 *
 **************************************
 *
 * Functional description
 *	Toss another record into running computations.
 *
 **************************************/
	LLS stack;

	for (; control; control = control->brk_next)
		for (stack = control->brk_statisticals; stack;
			 stack = stack->lls_next) EVAL_break_increment((QLI_NOD) stack->lls_object);
}


static void initialize_break( BRK control)
{
/**************************************
 *
 *	i n i t i a l i z e _ b r e a k
 *
 **************************************
 *
 * Functional description
 *	Execute a control break.
 *
 **************************************/
	LLS stack;

	for (; control; control = control->brk_next)
		for (stack = control->brk_statisticals; stack;
			 stack = stack->lls_next) EVAL_break_init((QLI_NOD) stack->lls_object);
}


static bool test_break(BRK control,
					   RPT report,
					   QLI_MSG message)
{
/**************************************
 *
 *	t e s t _ b r e a k
 *
 **************************************
 *
 * Functional description
 *	Check to see if there has been a control break for an expression.
 *
 **************************************/
	DSC desc1, desc2, *ptr1, *ptr2;
	UCHAR *p1, *p2;
	USHORT l;

// Evaluate the two versions of the expression 

	if (ptr1 = EVAL_value((QLI_NOD) control->brk_field))
		desc1 = *ptr1;

	p1 = message->msg_buffer;
	message->msg_buffer = report->rpt_buffer;

	if (ptr2 = EVAL_value((QLI_NOD) control->brk_field))
		desc2 = *ptr2;

	message->msg_buffer = p1;

// Check for consistently missing 

	if (!ptr1 || !ptr2)
		return (ptr1 != ptr2);

/* Both fields are present.  Check values.  Luckily, there's no need
   to worry about datatypes. */

	p1 = desc1.dsc_address;
	p2 = desc2.dsc_address;
	l = desc1.dsc_length;

	if (desc1.dsc_dtype == dtype_varying)
		l = 2 + *(USHORT *) p1;

	if (l)
		do
			if (*p1++ != *p2++)
				return true;
		while (--l);

	return false;
}


static void top_break( BRK control, PRT print)
{
/**************************************
 *
 *	 t o p _ b r e a k
 *
 **************************************
 *
 * Functional description
 *	Execute a control break.
 *
 **************************************/
	LLS stack;

	for (; control; control = control->brk_next) {
		for (stack = control->brk_statisticals; stack;
			 stack = stack->lls_next) EVAL_break_compute((QLI_NOD) stack->lls_object);
		FMT_print((QLI_NOD) control->brk_line, print);
	}
}


static void top_of_page(PRT print,
						bool first_flag)
{
/**************************************
 *
 *	t o p _ o f _ p a g e
 *
 **************************************
 *
 * Functional description
 *	Handle top of page condition.
 *
 **************************************/
	RPT report;
	BRK control;

	++print->prt_page_number;
	report = print->prt_report;

	if (!first_flag) {
		if (control = report->rpt_bottom_page)
			FMT_print((QLI_NOD) control->brk_line, print);
		FMT_put("\f", print);
	}

	print->prt_lines_remaining = print->prt_lines_per_page;

	if (control = report->rpt_top_page)
		FMT_print((QLI_NOD) control->brk_line, print);
	else if (report->rpt_column_header) {
		if (report->rpt_header)
			FMT_put(report->rpt_header, print);
		if (report->rpt_column_header)
			FMT_put(report->rpt_column_header, print);
	}

	if (report->rpt_bottom_page)
		initialize_break(report->rpt_bottom_page);
}
