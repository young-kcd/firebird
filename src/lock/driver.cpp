/*
 *	PROGRAM:	JRD Lock Manager
 *	MODULE:		driver.c
 *	DESCRIPTION:	Stand alone test driver
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

#include "../jrd/common.h"
#include "../jrd/gds.h"
#include "../jrd/isc.h"
#include "../lock/lock.h"

#ifdef WIN_NT
#include <process.h>
#endif

static void ast(int);
static int lookup_agg(UCHAR *);
static int lookup_lock(UCHAR *);
static void print_help(void);

static struct tbl {
	SCHAR *tbl_string;
	SSHORT tbl_code;
} types[] = {
	"null", LCK_null,
		"pr", LCK_PR,
		"sr", LCK_SR,

		"pw", LCK_PW, "sw", LCK_SW, "ex", LCK_EX, NULL, LCK_none};

static struct tagg {
	SCHAR *tagg_string;
	SSHORT tagg_code;
} aggs[] = {
	"min", LCK_MIN,
		"max", LCK_MAX,
		"cnt", LCK_CNT,
		"sum", LCK_SUM, "avg", LCK_AVG, "any", LCK_ANY, NULL, NULL};

static int wait, sw_release, locks[100], levels[100];
static SLONG lck_owner_handle = 0;


void main( int argc, char **argv)
{
/**************************************
 *
 *	m a i n 
 *
 **************************************
 *
 * Functional description
 *	Test driver for lock manager.
 *
 **************************************/
	UCHAR op[500], arg[500];
	SSHORT type, slot, n, agg;
	SLONG lock, parent, data;
	ISC_STATUS status;
	ISC_STATUS_ARRAY status_vector;

	ib_printf("\nStand alone Lock Manager driver program.\n");
	ib_printf("****************************************\n\n");
	ib_printf("pid = %d\n\n", getpid());
	ib_printf("\n");

	if (LOCK_init(status_vector, TRUE,
				  (SLONG) getpid(), 1, &lck_owner_handle)) {
		ib_printf("LOCK_init failed\n");
		gds__print_status(status_vector);
		exit(0);
	}

	wait = 1;
	sw_release = 1;
	slot = 0;

/* Force a dummy parent lock to test query data functionality */

	parent = LOCK_enq(NULL,		/* prior request */
					  NULL,		/* parent request */
					  0,		/* series */
					  "parent",	/* value */
					  strlen("parent"),	/* length of key */
					  LCK_SR,	/* lock type */
					  NULL, NULL,	/* AST and argument */
					  0, wait, status_vector, lck_owner_handle);

	while (TRUE) {
		ib_printf("Request: ");
		status = ib_scanf("%s%s", op, arg);
		if (status == EOF)
			continue;
		if (!strcmp(op, "rel")) {
			n = atoi(arg);
			if (n < slot && (lock = locks[n])) {
				LOCK_deq(lock);
				locks[n] = 0;
			}
			else
				ib_printf("*** BAD LOCK\n");
			continue;
		}
		else if (!strcmp(op, "ar")) {
			sw_release = atoi(arg);
			continue;
		}
		else if (!strcmp(op, "w")) {
			wait = atoi(arg);
			continue;
		}
		else if (!strcmp(op, "q"))
			exit(0);
		else if (!strcmp(op, "rd")) {
			n = atoi(arg);
			if (n >= slot || !(lock = locks[n])) {
				ib_printf("bad lock\n");
				continue;
			}
			data = LOCK_read_data(lock);
			ib_printf("lock data = %d\n", data);
			continue;
		}
		else if (!strcmp(op, "wd")) {
			n = atoi(arg);
			if (n >= slot || !(lock = locks[n])) {
				ib_printf("bad lock\n");
				continue;
			}
			ib_scanf("%s", arg);
			data = atoi(arg);
			LOCK_write_data(lock, data);
			continue;
		}
		else if (!strcmp(op, "qd")) {
			if (!(agg = lookup_agg(arg))) {
				ib_printf("bad query aggregate\n");
				continue;
			}
			data = LOCK_query_data(parent, 0, agg);
			if (agg == LCK_ANY)
				ib_printf("%s = %s\n", arg, ((data) ? "true" : "false"));
			else
				ib_printf("%s = %d\n", arg, data);
			continue;
		}
		else if (!strcmp(op, "cvt")) {
			n = atoi(arg);
			ib_scanf("%s", op);
			if (!(type = lookup_lock(op))) {
				ib_printf("bad lock type\n");
				continue;
			}
			if (n >= slot || !(lock = locks[n])) {
				ib_printf("bad lock\n");
				continue;
			}
			if (!LOCK_convert(lock, type, wait, NULL, 0, status_vector)) {
				ib_printf("*** CONVERSION FAILED: status_vector[1] = %d",
						  status_vector[1]);
				if (status_vector[1] == gds__lock_timeout)
					ib_printf(" (gds__lock_timeout)\n");
				else if (status_vector[1] == gds__lock_conflict)
					ib_printf(" (gds__lock_conflict)\n");
				else if (status_vector[1] == gds__deadlock)
					ib_printf(" (gds__deadlock)\n");
				else
					ib_printf("\n");
			}
			continue;
		}
		else if (type = lookup_lock(op)) {
			lock = LOCK_enq(NULL,	/* prior request */
							parent,	/* parent request */
							0,	/* series */
							arg,	/* value */
							strlen(arg),	/* length of key */
							type,	/* lock type */
							(sw_release ? ast : NULL), slot,	/* AST and argument */
							0, wait, status_vector, lck_owner_handle);
			if (lock) {
				ib_printf("lock# %d = %d\n", slot, lock);
				levels[slot] = type;
				locks[slot++] = lock;
			}
			else {
				ib_printf("*** LOCK REJECTED: status_vector[1] = %d",
						  status_vector[1]);
				if (status_vector[1] == gds__lock_timeout)
					ib_printf(" (gds__lock_timeout)\n");
				else if (status_vector[1] == gds__lock_conflict)
					ib_printf(" (gds__lock_conflict)\n");
				else if (status_vector[1] == gds__deadlock)
					ib_printf(" (gds__deadlock)\n");
				else
					ib_printf("\n");
			}
		}
		else {
			print_help();
			continue;
		}
	}

}


static void ast( int slot)
{
/**************************************
 *
 *	a s t
 *
 **************************************
 *
 * Functional description
 *	Dummy blocking ast.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;
	int sw_release_use = sw_release;

	ib_printf("*** blocking AST for lock# %d ", slot);

	if (sw_release < 0) {
		ib_printf("In the AST routine.\n");
		ib_printf("Enter ar value [1=nothing, 2=release, 3=downgrade]:");
		ib_scanf("%d", &sw_release_use);
	}

	if (sw_release_use == 1) {
		ib_printf("-- ignored ***\n");
		return;
	}

	if (sw_release_use > 2 && levels[slot] == LCK_EX) {
		LOCK_convert(locks[slot], LCK_SR, wait, NULL, 0, status_vector);
		levels[slot] = LCK_SR;
		ib_printf("-- down graded to SR ***\n");
		return;
	}

	LOCK_deq(locks[slot]);
	ib_printf("-- released ***\n");
}


static lookup_agg( UCHAR * string)
{
/**************************************
 *
 *	l o o k u p _ a g g
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	for (tagg* ptr = aggs; ptr->tagg_string; ptr++)
		if (strcmp(ptr->tagg_string, string) == 0)
			return ptr->tagg_code;

	return NULL;
}


static lookup_lock( UCHAR * string)
{
/**************************************
 *
 *	l o o k u p _ l o c k
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	for (tbl* ptr = types; ptr->tbl_string; ptr++)
		if (strcmp(ptr->tbl_string, string) == 0)
			return ptr->tbl_code;

	return LCK_none;
}


static void print_help(void)
{
/**************************************
*                                                       
*      p r i n t _ h e l p
*
**************************************
*
* Functional description
*      Prints the command syntax.
*
***************************************/

	ib_printf("Command syntax is:\n");
	ib_printf
		("\tnull <key>\tAcquire an 'null' lock on <key> (returns <lock#>)\n");
	ib_printf
		("\tsr <key>\tAcquire an 'sr' lock on <key> (returns <lock#>)\n");
	ib_printf
		("\tpr <key>\tAcquire an 'pr' lock on <key> (returns <lock#>)\n");
	ib_printf
		("\tsw <key>\tAcquire an 'sw' lock on <key> (returns <lock#>)\n");
	ib_printf
		("\tpw <key>\tAcquire an 'pw' lock on <key> (returns <lock#>)\n");
	ib_printf
		("\tex <key>\tAcquire an 'ex' lock on <key> (returns <lock#>)\n");
	ib_printf("\tw <value>\tSet wait parameter to LOCK_enq:\n");
	ib_printf("\t\t\t\tvalue>0   willing to wait forever [default]\n");
	ib_printf("\t\t\t\tvalue=0   not willing to wait\n");
	ib_printf("\t\t\t\tvalue<0   willing to wait for that many seconds\n");
	ib_printf("\tar <value>\tControls the AST\n");
	ib_printf
		("\t\t\t\tvalue=1   AST will not release nor downgrade [default]\n");
	ib_printf("\t\t\t\tvalue=2   AST will release lock\n");
	ib_printf("\t\t\t\tvalue=3   AST will attempt to downgrade first\n");
	ib_printf("\t\t\t\tvalue<0   AST will prompt\n");
	ib_printf("\t\t\t\tvalue=0   AST is not supplied\n");
	ib_printf("\trel <lock#>\tRelease lock <lock#>\n");
	ib_printf
		("\tcvt <lock#> <lock-type>\n\t\t\tConvert lock <lock#> to type <lock-type>\n");
	ib_printf("\trd <lock#>\tRead data <lock#>\n");
	ib_printf("\twd <lock#> <data>\tWrite <data> to <lock#>\n");
	ib_printf
		("\tqd <agg>\tQuery data for <agg := min,max,cnt,sum,avg,any>\n");
	ib_printf("\tq <don't-care>\tQuit\n");
	ib_printf("\nRemember the isc_config modifier:\n");
	ib_printf("\tV4_LOCK_GRANT_ORDER {0 | 1}\n");
	ib_printf("\t\t\t0 = disable lock-grant-order (no lock-fairness)\n");
	ib_printf("\t\t\t1 = enable lock-grant-order [default]\n");
}


SCH_ast(void)
{
/**************************************
 *
 *	S C H _ a s t 
 *
 **************************************
 *
 * Functional description
 *	Stub to prevent link error
 *
 **************************************/
}
