/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		llio_proto.h
 *	DESCRIPTION:	Prototype header file for llio.cpp
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

#ifndef JRD_LLIO_PROTO_H
#define JRD_LLIO_PROTO_H

int LLIO_allocate_file_space(ISC_STATUS *, TEXT *, SLONG, UCHAR, USHORT);
int LLIO_close(ISC_STATUS *, SLONG);
int LLIO_open(ISC_STATUS*, const TEXT*, USHORT, USHORT, SLONG*);
int LLIO_read(ISC_STATUS *, SLONG, TEXT *, SLONG, USHORT, UCHAR *, SLONG,
					 SLONG *);
int LLIO_seek(ISC_STATUS *, SLONG, TEXT *, SLONG, USHORT);
int LLIO_sync(ISC_STATUS *, SLONG);
int LLIO_write(ISC_STATUS *, SLONG, TEXT *, SLONG, USHORT, UCHAR *, SLONG,
					  SLONG *);

#endif // JRD_LLIO_PROTO_H

