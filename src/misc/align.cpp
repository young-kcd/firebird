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
 */
/*
$Id: align.cpp,v 1.1 2003-07-14 09:29:10 brodsom Exp $
*/

#include "firebird.h"
#include "../jrd/ibsetjmp.h"
#include <signal.h>
#include "../jrd/ib_stdio.h"

typedef struct xyz {
	char a;
	double b;
} *XYZ;

#define MAJOR_MINOR	"((n + b - 1) & ~(b - 1))"
#define EVEN		"((n+1) & ~1)"
#define NO_OP		"(n)"

typedef struct alignment {
	short rule_offset;
	short rule_length;
	short rule_faults;
	short rule_base_align;
	short rule_double_align;
	char *rule_rule;
	char *rule_system;
} ALIGNMENT;

static ALIGNMENT rules[] = {
	1, 9, 0, 4, 4, NO_OP, "VMS",	/* VMS */
	2, 10, 0, 4, 4, EVEN, "MC 68K",	/* Generic Motorola */
	4, 12, 0, 4, 4, MAJOR_MINOR, "VAX Ultrix, 386i, RT",	/* VAX Ultrix */
	8, 16, 1, 8, 8, MAJOR_MINOR, "HP Precision",	/* HP Precision */
	8, 16, 0, 4, 8, MAJOR_MINOR, "Sparc",	/* MIPS, Sun Sparc */
	0, 0, 0, 0, 0
};

static void check_byte_order(void);
static int check_double(void);
static void handler(void);

static JMP_BUF env;


int main(int argc, char *argv[])
{
	double *p, d1;
#if SIZEOF_LONG == 8
	int vector[3];
#else
	long vector[3];
#endif
	short offset, length, faults;
	ALIGNMENT *rule;

	offset = (int) &((XYZ) 0)->b;
	length = sizeof(struct xyz);
	faults = check_double();

	for (rule = rules; rule->rule_offset; ++rule)
		if (rule->rule_offset == offset &&
			rule->rule_length == length && rule->rule_faults == faults) {
			ib_printf("\n/* %s */\n\n", rule->rule_system);
			ib_printf("#define ALIGNMENT\t%d\n", rule->rule_base_align);
			ib_printf("#define DOUBLE_ALIGN\t%d\n", rule->rule_double_align);
			ib_printf("#define FB_ALIGN(n,b)\t%s\n", rule->rule_rule);
			check_byte_order();
			ib_printf("\n");
			return 1;
		}

	ib_printf("Unknown system, double offset %d, structure length %d\n",
			  offset, length);
	check_byte_order();

	return 1;
}

static void check_byte_order(void)
{
	union {
		short s;
		char c[2];
	} order_test;

	order_test.c[0] = 1;
	order_test.c[1] = 2;

	if (order_test.s == 0x0102)
		ib_printf("#define WORDS_BIGENDIAN\t\t1\n");
}

static int check_double(void)
{
	double *p, d1;
#if SIZEOF_LONG == 8
	int vector[3];
#else
	long vector[3];
#endif

	try {
		signal(SIGBUS, handler);
		signal(SIGSEGV, handler);
		p = (double *) &vector[0];
		*p = 3;
		*p *= 2.5;
		p = (double *) &vector[1];
		*p = 3;
		*p *= 2.5;
	}
	catch(const std::exception&) {
		return 1;
	}

	return 0;
}

static void handler()
{
	Firebird::status_exception::raise(1);
}
