/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		aif_proto.h
 *	DESCRIPTION:	Prototype header file for aif.c
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

#ifndef _JRD_AIF_PROTO_H_
#define _JRD_AIF_PROTO_H_

extern SLONG AIF_close_port(SLONG);
extern SLONG AIF_get_pid(SLONG, SLONG, SLONG *);
extern SLONG AIF_lookup_file(SCHAR *);
extern SLONG AIF_interrupt_port(SLONG, SLONG);
extern SLONG AIF_open_port(SLONG *, TEXT *, TEXT *, SLONG, SLONG[],
						   SLONG ^[]);
extern SLONG AIF_receive_port(SLONG, SLONG *, SCHAR *, SLONG *, SLONG[],
							  SLONG ^[]);
extern SLONG AIF_send_port(SLONG, SLONG, SCHAR *, SLONG, SLONG[], SLONG ^[]);

#endif /* _JRD_AIF_PROTO_H_ */
