/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		vio_proto.h
 *	DESCRIPTION:	Prototype header file for vio.cpp
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
 * 2002.10.21 Nickolay Samofatov: Added support for explicit pessimistic locks
 * 2002.10.29 Nickolay Samofatov: Added support for savepoints
 */

#ifndef JRD_VIO_PROTO_H
#define JRD_VIO_PROTO_H

class jrd_rel;
class jrd_tra;
class Record;
class Rsb;
struct record_param;
class Savepoint;
struct blk;

void	VIO_backout(thread_db*, record_param*, const jrd_tra*);
void	VIO_bump_count(thread_db*, USHORT, jrd_rel*, bool);
bool	VIO_chase_record_version(thread_db*, record_param*, Rsb*,
									jrd_tra*, blk*, bool);
#ifdef PC_ENGINE
bool	VIO_check_if_updated(thread_db*, record_param*);
#endif
void	VIO_data(thread_db*, record_param*, blk*);
void	VIO_erase(thread_db*, record_param*, jrd_tra*);
#ifdef GARBAGE_THREAD
void	VIO_fini(thread_db*);
#endif
bool	VIO_garbage_collect(thread_db*, record_param*, const jrd_tra*);
Record*	VIO_gc_record(thread_db*, jrd_rel*);
bool	VIO_get(thread_db*, record_param*, Rsb*, jrd_tra*, blk*);
bool	VIO_get_current(thread_db*, record_param*, jrd_tra*, blk*, bool);
#ifdef GARBAGE_THREAD
void	VIO_init(thread_db*);
#endif
void	VIO_merge_proc_sav_points(thread_db*, jrd_tra*, Savepoint**);
bool	VIO_writelock(thread_db*, record_param*, Rsb*, jrd_tra*);
void	VIO_modify(thread_db*, record_param*, record_param*, jrd_tra*);
bool	VIO_next_record(thread_db*, record_param*, Rsb*, jrd_tra*,
							   blk*, bool, bool);
Record*	VIO_record(thread_db*, record_param*, const class fmt*, JrdMemoryPool*);
void	VIO_start_save_point(thread_db*, jrd_tra*);
void	VIO_store(thread_db*, record_param*, jrd_tra*);
bool	VIO_sweep(thread_db*, jrd_tra*);
void	VIO_verb_cleanup(thread_db*, jrd_tra*);
SLONG	VIO_savepoint_large(const Savepoint*, SLONG);

#endif // JRD_VIO_PROTO_H

