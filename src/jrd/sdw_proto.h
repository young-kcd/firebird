/*
 *	PROGRAM:	JRD Access method
 *	MODULE:		sdw_proto.h
 *	DESCRIPTION:	Prototype Header file for sdw.c
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

#ifdef __cplusplus
extern "C" {
#endif

extern void SDW_add(TEXT *, USHORT, USHORT);
extern int SDW_add_file(TEXT *, SLONG, USHORT);
extern void SDW_check(void);
extern BOOLEAN SDW_check_conditional(void);
extern void SDW_close(void);
extern void SDW_dump_pages(void);
extern void SDW_get_shadows(void);
extern void SDW_init(USHORT, USHORT, struct sbm *);
extern BOOLEAN SDW_lck_update(SLONG);
extern void SDW_notify(void);
extern BOOLEAN SDW_rollover_to_shadow(struct fil *, BOOLEAN);
extern void SDW_shutdown_shadow(struct sdw *);
extern void SDW_start(TEXT *, USHORT, USHORT, USHORT);
extern int SDW_start_shadowing(void *ast_object);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* JRD_SDW_PROTO_H */
