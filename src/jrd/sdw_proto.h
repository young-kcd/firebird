/*
 *	PROGRAM:	JRD Access method
 *	MODULE:		sdw_proto.h
 *	DESCRIPTION:	Prototype Header file for sdw.cpp
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

#ifndef JRD_SDW_PROTO_H
#define JRD_SDW_PROTO_H

void	SDW_add(const TEXT*, USHORT, USHORT);
int		SDW_add_file(const TEXT*, SLONG, USHORT);
void	SDW_check(void);
bool	SDW_check_conditional(void);
void	SDW_close(void);
void	SDW_dump_pages(void);
void	SDW_get_shadows(void);
void	SDW_init(bool, bool);
bool	SDW_lck_update(SLONG);
void	SDW_notify(void);
bool	SDW_rollover_to_shadow(class jrd_file*, const bool);
void	SDW_shutdown_shadow(class Shadow*);
void	SDW_start(const TEXT*, USHORT, USHORT, bool);
int		SDW_start_shadowing(void* ast_object);

#endif // JRD_SDW_PROTO_H

