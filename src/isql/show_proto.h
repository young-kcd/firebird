/*
 *	PROGRAM:	Interactive SQL utility
 *	MODULE:		show_proto.h
 *	DESCRIPTION:	Prototype header file for show.e
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

#ifndef ISQL_SHOW_PROTO_H
#define ISQL_SHOW_PROTO_H

extern bool	SHOW_dbb_parameters (FRBRD *, SCHAR *, SCHAR *, USHORT, bool);
extern int	SHOW_grants (SCHAR*, const SCHAR*, USHORT);
extern int	SHOW_grants2 (SCHAR*, const SCHAR*, USHORT, TEXT*);
extern void	SHOW_grant_roles (const SCHAR*, bool*);
extern void	SHOW_grant_roles2 (const SCHAR*, bool*, TEXT*);
extern void SHOW_print_metadata_text_blob(IB_FILE *, GDS__QUAD *);
extern int SHOW_metadata(SCHAR **, SCHAR **);

#endif /* ISQL_SHOW_PROTO_H */

