/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		rse_proto.h
 *	DESCRIPTION:	Prototype header file for rse.cpp
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

#ifndef JRD_RSE_PROTO_H
#define JRD_RSE_PROTO_H

#include "../jrd/jrd.h"
#include "../jrd/rse.h"

class Rsb;
struct record_param;
class jrd_nod;
class Bookmark;

void RSE_close(thread_db*, Rsb*);
#ifdef PC_ENGINE
bool RSE_find_dbkey(thread_db*, Rsb*, jrd_nod*, jrd_nod*);
bool RSE_find_record(thread_db*, Rsb*, USHORT, USHORT, jrd_nod*);
#endif
bool RSE_get_record(thread_db*, Rsb*, enum rse_get_mode);
#ifdef PC_ENGINE
Bookmark* RSE_get_bookmark(thread_db*, Rsb*);
void RSE_mark_crack(thread_db*, Rsb*, USHORT);
#endif
void RSE_open(thread_db*, Rsb*);
#ifdef PC_ENGINE
bool RSE_reset_position(thread_db*, Rsb*, record_param*);
bool RSE_set_bookmark(thread_db*, Rsb*, record_param*, Bookmark*);
#endif

#ifdef PC_ENGINE
#define RSE_MARK_CRACK(t, var1, var2)	RSE_mark_crack (t, var1, var2)
#else
#define RSE_MARK_CRACK(t, var1, var2)
#endif

#endif // JRD_RSE_PROTO_H

