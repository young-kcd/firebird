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

namespace Jrd {
	class jrd_rel;
	class jrd_tra;
	class Record;
	class Rsb;
	struct record_param;
	class Savepoint;
	class fmt;
}
struct blk;

void	VIO_backout(Jrd::thread_db*, Jrd::record_param*, const Jrd::jrd_tra*);
void	VIO_bump_count(Jrd::thread_db*, USHORT, Jrd::jrd_rel*, bool);
bool	VIO_chase_record_version(Jrd::thread_db*, Jrd::record_param*, Jrd::Rsb*,
									Jrd::jrd_tra*, blk*, bool);
#ifdef PC_ENGINE
bool	VIO_check_if_updated(Jrd::thread_db*, Jrd::record_param*);
#endif
void	VIO_data(Jrd::thread_db*, Jrd::record_param*, blk*);
void	VIO_erase(Jrd::thread_db*, Jrd::record_param*, Jrd::jrd_tra*);
#ifdef GARBAGE_THREAD
void	VIO_fini(Jrd::thread_db*);
#endif
bool	VIO_garbage_collect(Jrd::thread_db*, Jrd::record_param*, const Jrd::jrd_tra*);
Jrd::Record*	VIO_gc_record(Jrd::thread_db*, Jrd::jrd_rel*);
bool	VIO_get(Jrd::thread_db*, Jrd::record_param*, Jrd::Rsb*, Jrd::jrd_tra*, blk*);
bool	VIO_get_current(Jrd::thread_db*, Jrd::record_param*, Jrd::jrd_tra*, blk*, bool);
#ifdef GARBAGE_THREAD
void	VIO_init(Jrd::thread_db*);
#endif
void	VIO_merge_proc_sav_points(Jrd::thread_db*, Jrd::jrd_tra*, Jrd::Savepoint**);
bool	VIO_writelock(Jrd::thread_db*, Jrd::record_param*, Jrd::Rsb*, Jrd::jrd_tra*);
void	VIO_modify(Jrd::thread_db*, Jrd::record_param*, Jrd::record_param*, Jrd::jrd_tra*);
bool	VIO_next_record(Jrd::thread_db*, Jrd::record_param*, Jrd::Rsb*, Jrd::jrd_tra*,
							   blk*, bool, bool);
Jrd::Record*	VIO_record(Jrd::thread_db*, Jrd::record_param*, const Jrd::fmt*, JrdMemoryPool*);
void	VIO_start_save_point(Jrd::thread_db*, Jrd::jrd_tra*);
void	VIO_store(Jrd::thread_db*, Jrd::record_param*, Jrd::jrd_tra*);
bool	VIO_sweep(Jrd::thread_db*, Jrd::jrd_tra*);
void	VIO_verb_cleanup(Jrd::thread_db*, Jrd::jrd_tra*);
SLONG	VIO_savepoint_large(const Jrd::Savepoint*, SLONG);

#endif // JRD_VIO_PROTO_H

