/*
 *	PROGRAM:	JRD Command Oriented Query Language
 *	MODULE:		err_proto.h
 *	DESCRIPTION:	Prototype header file for err.cpp
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

#ifndef QLI_ERR_PROTO_H
#define QLI_ERR_PROTO_H

extern void	ERRQ_bugcheck (USHORT);
extern void	ERRQ_database_error(dbb*, ISC_STATUS*);
extern void	ERRQ_error (USHORT, const TEXT*, const TEXT*, const TEXT*,
	const TEXT*, const TEXT*);
extern void	ERRQ_error_format (USHORT, const TEXT*, const TEXT*, const TEXT*,
	const TEXT*, const TEXT*);
extern void	ERRQ_exit (int);
extern void	ERRQ_msg_format (USHORT, USHORT, TEXT*, const TEXT*, const TEXT*,
	const TEXT*, const TEXT*, const TEXT*);
extern int	ERRQ_msg_get (USHORT, TEXT*);
extern void	ERRQ_msg_partial (USHORT, const TEXT*, const TEXT*, const TEXT*,
	const TEXT*, const TEXT*);
extern void	ERRQ_msg_put (USHORT, const TEXT*, const TEXT*, const TEXT*,
	const TEXT*, const TEXT*);
extern void	ERRQ_pending (void);
extern void	ERRQ_print_error (USHORT, const TEXT*, const TEXT*, const TEXT*,
	const TEXT*, const TEXT*);
extern void	ERRQ_syntax (USHORT);

#endif /* QLI_ERR_PROTO_H */

