/*
 *	PROGRAM:	Central Server Interface
 *	MODULE:		css_proto.h
 *	DESCRIPTION:	Prototype header file for css.c
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

#ifndef _CSV_CSS_PROTO_H_
#define _CSV_CSS_PROTO_H_

extern CSH CSS_acquire(void);
extern FRB CSS_alloc_message(USHORT, int);
extern BLK CSS_alloc_local(USHORT, USHORT);
extern int CSS_check_partner(PTR, USHORT);
extern PTR CSS_connect(SLONG);
extern PTR CSS_create_process(USHORT);
extern void CSS_disconnect(SLONG);
extern void CSS_free_global(FRB);
extern void CSS_free_local(BLK);
extern PTR CSS_find_process(SLONG);
extern CSV_MSG CSS_get_message(PTR, CSV_MSG, SSHORT);
#ifndef PIPE_SERVER
extern CSH CSS_init(STATUS *, USHORT);
#else
extern CSH CSS_init(STATUS *, USHORT, SSHORT);
#endif
extern void CSS_probe_processes(void);
extern int CSS_put_message(PTR, CSV_MSG, CSV_MSG);
extern void CSS_release(void);
extern void CSS_validate(void);

#endif /* _CSV_CSS_PROTO_H_ */
