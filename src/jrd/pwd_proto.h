/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		pwd_proto.h
 *	DESCRIPTION:	Prototype header file for pwd.c
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
 * $Id: pwd_proto.h,v 1.2 2001-12-24 02:50:52 tamlin Exp $
 */

#ifndef _JRD_PWD_PROTO_H_
#define _JRD_PWD_PROTO_H_

#ifdef __cplusplus
extern "C" {
#endif

void	PWD_get_user_dbpath(TEXT*);
void	PWD_verify_user(TEXT*, TEXT*, TEXT*, TEXT*,
						int*, int*, int*);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _JRD_PWD_PROTO_H_ */
