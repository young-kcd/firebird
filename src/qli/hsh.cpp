/*
 *	PROGRAM:	JRD Command Oriented Query Language
 *	MODULE:		hsh.c
 *	DESCRIPTION:	Hash table and symbol manager
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
#include "../qli/dtr.h"
#include "../qli/parse.h"
#include "../qli/all_proto.h"
#include "../qli/err_proto.h"
#include "../qli/hsh_proto.h"

#define HASH_SIZE	224

static int hash(SCHAR *, int);
static bool scompare(SCHAR *, int, SCHAR *, int);

static SYM hash_table[HASH_SIZE];
static SYM key_symbols;

struct word {
	KWWORDS id;
	SCHAR *keyword;
} keywords[] = {
#include "../qli/words.h"
   {KW_continuation, "-\n"}
};


void HSH_fini(void)
{
/**************************************
 *
 *	H S H _ f i n i
 *
 **************************************
 *
 * Functional description
 *	Release space used by keywords.
 *
 **************************************/
	SYM symbol;

	while (key_symbols) {
		symbol = key_symbols;
		key_symbols = (SYM) key_symbols->sym_object;
		HSH_remove(symbol);
		ALL_release((FRB) symbol);
	}
}


void HSH_init(void)
{
/**************************************
 *
 *	H S H _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize the hash table.  This mostly involves
 *	inserting all known keywords.
 *
 **************************************/
	SCHAR *string;
	SYM symbol;
	int i;
	struct word *word;

	for (i = 0, word = keywords; i < FB_NELEM(keywords); i++, word++) {
		for (string = word->keyword; *string; string++);
		symbol = (SYM) ALLOCPV(type_sym, 0);
		symbol->sym_type = SYM_keyword;
		symbol->sym_length = string - word->keyword;
		symbol->sym_string = word->keyword;
		symbol->sym_keyword = (int) word->id;
		HSH_insert(symbol);
		symbol->sym_object = (BLK) key_symbols;
		key_symbols = symbol;
	}
}


void HSH_insert( SYM symbol)
{
/**************************************
 *
 *	H S H _ i n s e r t
 *
 **************************************
 *
 * Functional description
 *	Insert a symbol into the hash table.
 *
 **************************************/
	int h;
	SYM old;

	h = hash(symbol->sym_string, symbol->sym_length);

	for (old = hash_table[h]; old; old = old->sym_collision)
		if (scompare(symbol->sym_string, symbol->sym_length,
					 old->sym_string, old->sym_length)) {
			symbol->sym_homonym = old->sym_homonym;
			old->sym_homonym = symbol;
			return;
		}

	symbol->sym_collision = hash_table[h];
	hash_table[h] = symbol;
}


SYM HSH_lookup(SCHAR * string, int length)
{
/**************************************
 *
 *	H S H _ l o o k u p
 *
 **************************************
 *
 * Functional description
 *	Perform a string lookup against hash table.
 *
 **************************************/
	SYM symbol;

	for (symbol = hash_table[hash(string, length)]; symbol;
		 symbol = symbol->sym_collision)
			if (scompare
				(string, length, symbol->sym_string,
				 symbol->sym_length)) return symbol;

	return NULL;
}


void HSH_remove( SYM symbol)
{
/**************************************
 *
 *	H S H _ r e m o v e 
 *
 **************************************
 *
 * Functional description
 *	Remove a symbol from the hash table.
 *
 **************************************/
	int h;
	SYM *next, *ptr, homonym;

	h = hash(symbol->sym_string, symbol->sym_length);

	for (next = &hash_table[h]; *next; next = &(*next)->sym_collision)
		if (symbol == *next)
			if (homonym = symbol->sym_homonym) {
				homonym->sym_collision = symbol->sym_collision;
				*next = homonym;
				return;
			}
			else {
				*next = symbol->sym_collision;
				return;
			}
		else
			for (ptr = &(*next)->sym_homonym; *ptr;
				 ptr = &(*ptr)->sym_homonym) if (symbol == *ptr) {
					*ptr = symbol->sym_homonym;
					return;
				}

	ERRQ_error(27, NULL, NULL, NULL, NULL, NULL);	/* Msg 27 HSH_remove failed */
}


static int hash( SCHAR * string, int length)
{
/**************************************
 *
 *	h a s h
 *
 **************************************
 *
 * Functional description
 *	Returns the hash function of a string.
 *
 **************************************/
	int value;
	SCHAR c;

	value = 0;

	while (length--) {
		c = *string++;
		value = (value << 1) + UPPER(c);
	}

	return ((value >= 0) ? value : -value) % HASH_SIZE;
}


static bool scompare(SCHAR * string1,
					 int length1,
					 SCHAR * string2,
					 int length2)
{
/**************************************
 *
 *	s c o m p a r e
 *
 **************************************
 *
 * Functional description
 *	Compare two strings
 *
 **************************************/
	SCHAR c1, c2;

	if (length1 != length2)
		return false;

	while (length1--)
		if ((c1 = *string1++) != (c2 = *string2++) && UPPER(c1) != UPPER(c2))
			return false;

	return true;
}
