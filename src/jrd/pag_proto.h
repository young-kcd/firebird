/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		pag_proto.h
 *	DESCRIPTION:	Prototype header file for pag.c
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

#ifndef _JRD_PAG_PROTO_H_
#define _JRD_PAG_PROTO_H_

#ifdef __cplusplus
extern "C" {
#endif

extern int PAG_add_clump(SLONG, USHORT, USHORT, UCHAR *, USHORT, USHORT);
extern USHORT PAG_add_file(TEXT *, SLONG);
extern int PAG_add_header_entry(struct hdr *, USHORT, SSHORT, UCHAR *);
extern struct pag *PAG_allocate(struct win *);
extern SLONG PAG_attachment_id(void);
extern int PAG_delete_clump_entry(SLONG, USHORT);
extern void PAG_format_header(void);
extern void PAG_format_log(void);
extern void PAG_format_pip(void);
extern int PAG_get_clump(SLONG, USHORT, USHORT *, UCHAR *);
extern void PAG_header(TEXT *, USHORT);
extern void PAG_init(void);
extern void PAG_init2(USHORT);
extern SLONG PAG_last_page(void);
extern void PAG_modify_log(SLONG, SLONG);
extern void PAG_release_page(SLONG, SLONG);
extern void PAG_set_force_write(struct dbb *, SSHORT);
extern void PAG_set_no_reserve(struct dbb *, USHORT);
extern void PAG_set_db_readonly(struct dbb *, SSHORT);
extern void PAG_set_db_SQL_dialect(struct dbb *, SSHORT);
extern void PAG_set_page_buffers(ULONG);
extern void PAG_sweep_interval(SLONG);
extern int PAG_unlicensed(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _JRD_PAG_PROTO_H_ */
