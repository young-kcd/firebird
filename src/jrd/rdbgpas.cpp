/*
 *	PROGRAM:	rdbvmsinit
 *	MODULE:		rdbvmsinit.c
 *	DESCRIPTION:	imitate rdb$vmsinitpas
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
#include descrip
#include ssdef
#include "../jrd/common.h"
#include "../jrd/rdb.h"

/* Transaction element block */

typedef struct teb {
	int *teb_database;
	int teb_tpb_length;
	SCHAR *teb_tpb;
} TEB;

int RDB$MSG_VECTOR[20];

static int gds_to_rdb(int *, int *);
static int set_status(int *, int);

static ISC_STATUS status_vector[ISC_STATUS_LENGTH];

static SLONG codes[] = {
#include "rdbcodes.h"
};


int rdb$vmspas_init(int dbcount, int *d)
{
/**************************************
 *
 *	r d b $ v m s p a s _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Mimic RdB's rdb$vmspas_init
 *
 **************************************/
	TEB tebs[16], *teb, *end;
	struct dsc$descriptor_s *dbname;
	int *dbhandle, *trhandle, stat, **v, i;

	stat = 1;
	v = &d;
	for (i = 0; i < dbcount; i++) {
		dbname = *v++;
		tebs[i].teb_database = dbhandle = *v++;
		tebs[i].teb_tpb_length = 0;
		tebs[i].teb_tpb = 0;
		if (!(*dbhandle)) {
			if (!(stat = rdb$attach_database(RDB$MSG_VECTOR, dbname, dbhandle,
											 0, 0, 0)))
				return stat;
		}
	}
	trhandle = *v++;
	if (trhandle && (!(*trhandle))) {
		gds__start_multiple(status_vector, trhandle, dbcount, tebs);
		stat = gds_to_rdb(status_vector, RDB$MSG_VECTOR);
	}

	return stat;
}


static int gds_to_rdb(int *status_vector, int *user_status)
{
/**************************************
 *
 *	g d s _ t o _ r d b
 *
 **************************************
 *
 * Functional description
 *	Translate a status vector from the Groton world to the RDB world.
 *
 **************************************/
	int code, trans;
	USHORT fac = 0, class_ = 0;

	code = gds__decode(status_vector[1], &fac, &class_);
	trans = codes[code];

	return set_status(user_status, trans);
}


static int set_status(int *status, int code)
{
/**************************************
 *
 *	s e t _ s t a t u s
 *
 **************************************
 *
 * Functional description
 *	Generate an error for a bad handle.
 *
 **************************************/

	*status++ = 1;
	*status++ = code;
	*status = 0;

	return code;
}
