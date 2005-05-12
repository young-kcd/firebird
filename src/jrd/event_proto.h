/*
 *	PROGRAM:	JRD Access method
 *	MODULE:		event_proto.h
 *	DESCRIPTION:	Prototype Header file for event.cpp
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
 *
 * 23-Feb-2002 Dmitry Yemanov - Events wildcarding
 *
 */

#ifndef JRD_EVENT_PROTO_H
#define JRD_EVENT_PROTO_H

void EVENT_cancel(SLONG);
SLONG EVENT_create_session(ISC_STATUS*);
void EVENT_delete_session(SLONG);
void EVENT_deliver();
struct evh* EVENT_init(ISC_STATUS*, bool);
int EVENT_post(ISC_STATUS*, USHORT, const TEXT*, USHORT, const TEXT*, USHORT);
SLONG EVENT_que(ISC_STATUS*, SLONG, USHORT, const TEXT*, USHORT, const UCHAR*,
				FPTR_EVENT_CALLBACK, void*);

#endif // JRD_EVENT_PROTO_H

