/*
 *	PROGRAM:	Preprocessor
 *	MODULE:		par_proto.h
 *	DESCRIPTION:	Prototype header file for par.c
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

#ifndef _GPRE_PAR_PROTO_H_
#define _GPRE_PAR_PROTO_H_

extern ACT PAR_action(TEXT*);
extern SSHORT PAR_blob_subtype(DBB);
extern ACT PAR_database(USHORT, TEXT*);
extern BOOLEAN PAR_end(void);
extern void PAR_error(TEXT *);
extern ACT PAR_event_init(USHORT);
extern ACT PAR_event_wait(USHORT);
extern void PAR_fini(void);
extern TOK PAR_get_token(void);
extern void PAR_init(void);
extern TEXT *PAR_native_value(USHORT, USHORT);
extern GPRE_FLD PAR_null_field(void);
extern void PAR_reserving(USHORT, SSHORT);
extern GPRE_REQ PAR_set_up_dpb_info(RDY, ACT, USHORT);
extern SYM PAR_symbol(enum sym_t);
extern void PAR_unwind(void);
extern void PAR_using_db(void);

#endif /* _GPRE_PAR_PROTO_H_ */
