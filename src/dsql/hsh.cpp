/*
 *	PROGRAM:	Dynamic SQL runtime support
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
#include <string.h>
#include "../dsql/dsql.h"
#include "../dsql/sym.h"
#include "../jrd/gds.h"
#include "../jrd/gds_proto.h"
#include "../dsql/alld_proto.h"
#include "../dsql/errd_proto.h"
#include "../dsql/hsh_proto.h"
#include "../jrd/sch_proto.h"
#include "../jrd/thd_proto.h"


ASSERT_FILENAME
#define HASH_SIZE 211
static SSHORT hash(SCHAR *, USHORT);
static bool remove_symbol(struct sym **, struct sym *);
static bool scompare(TEXT *, USHORT, TEXT *, USHORT);

static SYM *hash_table;

/*
   SUPERSERVER can end up with many hands in the pie, so some
   protection is provided via a mutex.   This ensures the integrity
   of lookups, inserts and removals, and also allows teh traversing
   of symbol chains for marking of relations and procedures.
   Otherwise, one DSQL user won't know what the other is doing
   to the same object.
*/

#ifdef  SUPERSERVER
static MUTX_T hash_mutex;
static USHORT hash_mutex_inited = 0;
#define LOCK_HASH       THD_mutex_lock (&hash_mutex)
#define UNLOCK_HASH     THD_mutex_unlock (&hash_mutex);
#else
#define LOCK_HASH
#define UNLOCK_HASH
#endif


/**
  
 	HSHD_init
  
    @brief	create a new hash table
 


 **/
void HSHD_init(void)
{
	UCHAR *p;

#ifdef SUPERSERVER
	if (!hash_mutex_inited) {
		hash_mutex_inited = 1;
		THD_mutex_init(&hash_mutex);
	}
#endif

	p = (UCHAR *) gds__alloc(sizeof(SYM) * HASH_SIZE);
	memset(p, 0, sizeof(SYM) * HASH_SIZE);

	hash_table = (SYM *) p;
}


#ifdef DEV_BUILD

#include "../jrd/ib_stdio.h"

/**
  
 	HSHD_debug
  
    @brief	Print out the hash table for debugging.
 


 **/
void HSHD_debug(void)
{
	SYM collision;
	SYM homptr;
	SSHORT h;

/* dump each hash table entry */

	LOCK_HASH;
	for (h = 0; h < HASH_SIZE; h++) {
		for (collision = hash_table[h]; collision;
			 collision = collision->sym_collision) {
			/* check any homonyms first */

			ib_fprintf(ib_stderr, "Symbol type %d: %s %p\n",
					   collision->sym_type, collision->sym_string,
					   collision->sym_dbb);
			for (homptr = collision->sym_homonym; homptr;
				 homptr = homptr->sym_homonym) {
				ib_fprintf(ib_stderr, "Homonym Symbol type %d: %s %p\n",
						   homptr->sym_type, homptr->sym_string,
						   homptr->sym_dbb);
			}
		}
	}
	UNLOCK_HASH;
}
#endif


/**
  
 	HSHD_fini
  
    @brief	Clear out the symbol table.  All the 
 	symbols are deallocated with their pools.
 


 **/
void HSHD_fini(void)
{

	for (SSHORT i = 0; i < HASH_SIZE; i++)
	{
		hash_table[i] = NULL;
	}

	gds__free(hash_table);
	hash_table = NULL;
}


/**
  
 	HSHD_finish
  
    @brief	Remove symbols used by a particular database.
 	Don't bother to release them since their pools
 	will be released.
 

    @param database

 **/
void HSHD_finish( void *database)
{
	SYM *collision;
	SYM *homptr;
	SYM symbol;
	SYM chain;
	SSHORT h;

/* check each hash table entry */

	LOCK_HASH;
	for (h = 0; h < HASH_SIZE; h++) {
		for (collision = &hash_table[h]; *collision;) {
			/* check any homonyms first */

			chain = *collision;
			for (homptr = &chain->sym_homonym; *homptr;) {
				symbol = *homptr;
				if (symbol->sym_dbb == database) {
					*homptr = symbol->sym_homonym;
					symbol = symbol->sym_homonym;
				}
				else
					homptr = &symbol->sym_homonym;
			}

			/* now, see if the root entry has to go */

			if (chain->sym_dbb == database) {
				if (chain->sym_homonym) {
					chain->sym_homonym->sym_collision = chain->sym_collision;
					*collision = chain->sym_homonym;
				}
				else
					*collision = chain->sym_collision;
				chain = *collision;
			}
			else
				collision = &chain->sym_collision;
		}
	}
	UNLOCK_HASH;
}


/**
  
 	HSHD_insert
  
    @brief	Insert a symbol into the hash table.
 

    @param symbol

 **/
void HSHD_insert(SYM symbol)
{
	SSHORT h;
	void *database;
	SYM old;

	LOCK_HASH;
	h = hash(symbol->sym_string, symbol->sym_length);
	database = symbol->sym_dbb;

	assert(symbol->sym_type >= SYM_statement && symbol->sym_type <= SYM_eof);

	for (old = hash_table[h]; old; old = old->sym_collision)
		if ((!database || (database == old->sym_dbb)) &&
			scompare(symbol->sym_string, symbol->sym_length, old->sym_string,
					 old->sym_length)) 
		{
			symbol->sym_homonym = old->sym_homonym;
			old->sym_homonym = symbol;
			UNLOCK_HASH;
			return;
		}

	symbol->sym_collision = hash_table[h];
	hash_table[h] = symbol;
	UNLOCK_HASH;
}


/**
  
 	HSHD_lookup
  
    @brief	Perform a string lookup against hash table.
 	Make sure to only return a symbol of the desired type.
 

    @param database
    @param string
    @param length
    @param type
    @param parser_version

 **/
SYM HSHD_lookup(void*    database,
				TEXT*    string,
				SSHORT   length,
				SYM_TYPE type,
				USHORT   parser_version)
{

	LOCK_HASH;
	SSHORT h = hash(string, length);
	for (SYM symbol = hash_table[h]; symbol; symbol = symbol->sym_collision)
	{
		if ((database == symbol->sym_dbb) &&
			scompare(string, length, symbol->sym_string, symbol->sym_length))
		{
			/* Search for a symbol of the proper type */
			while (symbol && symbol->sym_type != type) {
				symbol = symbol->sym_homonym;
			}
			UNLOCK_HASH;

			/* If the symbol found was not part of the list of keywords for the
			 * client connecting, then assume nothing was found
			 */
			if (symbol)
			{
				if (parser_version < symbol->sym_version &&
					type == SYM_keyword)
				{
					return NULL;
				}
			}
			return symbol;
		}
	}

	UNLOCK_HASH;
	return NULL;
}


/**
  
 	HSHD_remove
  
    @brief	Remove a symbol from the hash table.
 

    @param symbol

 **/
void HSHD_remove( SYM symbol)
{
	SYM *collision;
	SSHORT h;

	LOCK_HASH;
	h = hash(symbol->sym_string, symbol->sym_length);

	for (collision = &hash_table[h]; *collision;
		 collision = &(*collision)->sym_collision)
	{
		if (remove_symbol(collision, symbol)) {
			UNLOCK_HASH;
			return;
		}
	}

	UNLOCK_HASH;
	IBERROR(-1, "HSHD_remove failed");
}


/**
  
 HSHD_set_flag
  
    @brief      Set a flag in all similar objects in a chain.   This
       is used primarily to mark relations and procedures
       as deleted.   The object must have the same name and
       type, but not the same database, and must belong to
       some database.   Later access to such an object by
       another user or thread should result in that object's
       being refreshed.   Note that even if the relation name
       and ID, or the procedure name and ID both match, it
       may still not represent an exact match.   This is because
       there's no way at present for DSQL to tell if two databases
       as represented in DSQL are attachments to the same physical
       database.
 

    @param database
    @param string
    @param length
    @param type
    @param flag

 **/
void HSHD_set_flag(
				   void *database,
				   TEXT * string, SSHORT length, SYM_TYPE type, SSHORT flag)
{
	SYM symbol, homonym;
	SSHORT h;
	DSQL_REL sym_rel;
	DSQL_PRC sym_prc;


/* as of now, there's no work to do if there is no database or if
   the type is not a relation or procedure */

	if (!database)
		return;
	switch (type) {
	case SYM_relation:
	case SYM_procedure:
		break;
	default:
		return;
	}

	LOCK_HASH;
	h = hash(string, length);
	for (symbol = hash_table[h]; symbol; symbol = symbol->sym_collision) {
		if (symbol->sym_dbb && (database != symbol->sym_dbb) &&
			scompare(string, length, symbol->sym_string, symbol->sym_length)) {

			/* the symbol name matches and it's from a different database */

			for (homonym = symbol; homonym; homonym = homonym->sym_homonym) {
				if (homonym->sym_type == type) {

					/* the homonym is of the correct type */

					/* the next check is for the same relation or procedure ID,
					   which indicates that it MAY be the same relation or
					   procedure */

					switch (type) {
					case SYM_relation:
						sym_rel = (DSQL_REL) homonym->sym_object;
						sym_rel->rel_flags |= flag;
						break;

					case SYM_procedure:
						sym_prc = (DSQL_PRC) homonym->sym_object;
						sym_prc->prc_flags |= flag;
						break;
					}
				}
			}
		}
	}
	UNLOCK_HASH;
}


/**
  
 	hash
  
    @brief	Returns the hash function of a string.
 

    @param 
    @param 

 **/
static SSHORT hash(SCHAR * string, USHORT length)
{
	SLONG value;
	SCHAR c;

	value = 0;

	while (length--) {
		c = *string++;
		value = (value << 1) + (c);
	}

	return ((value >= 0) ? value : -value) % HASH_SIZE;
}


/**
  
 	remove_symbol
  
    @brief	Given the address of a collision,
 	remove a symbol from the collision 
 	and homonym linked lists.
 

    @param collision
    @param symbol

 **/
static bool remove_symbol( SYM * collision, SYM symbol)
{
	SYM *ptr, homonym;

	if (symbol == *collision) {
		if ((homonym = symbol->sym_homonym) != NULL) {
			homonym->sym_collision = symbol->sym_collision;
			*collision = homonym;
		}
		else
			*collision = symbol->sym_collision;

		return true;
	}

	for (ptr = &(*collision)->sym_homonym; *ptr; ptr = &(*ptr)->sym_homonym)
		if (symbol == *ptr) {
			*ptr = symbol->sym_homonym;
			return true;
		}

	return false;
}


/**
  
 	scompare
  
    @brief	Compare two symbolic strings
 	The character set for these strings is either ASCII or
 	Unicode in UTF format.
 	Symbols are case-significant - so no uppercase operation
 	is performed.
 

    @param string1
    @param length1
    @param string2
    @param length2

 **/
static bool scompare(TEXT * string1,
						USHORT length1,
						TEXT * string2, USHORT length2)
{

	if (length1 != length2)
		return false;

	while (length1--) {
		if ((*string1++) != (*string2++))
			return false;
	}

	return true;
}

