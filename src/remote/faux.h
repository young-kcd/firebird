/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		faux.h
 *	DESCRIPTION:	Replacement TCP/IP function definitions
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

extern int (*FAUX_gethostname_var)();
#define FAUX_gethostname (int) (*FAUX_gethostname_var)

extern struct servent * (*FAUX_getservbyname_var)();
#define FAUX_getservbyname(name,protocol) \
  (struct servent *) (*FAUX_getservbyname_var)( &nwSocketCtx, (name), (protocol))

extern struct hostent * (*FAUX_gethostbyname_var)();
#define FAUX_gethostbyname(name) \
  (struct hostent *) (*FAUX_gethostbyname_var)( &nwSocketCtx, (name) )
