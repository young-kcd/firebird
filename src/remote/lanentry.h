/*++
   File:    lanentry.h
   Author:  Charles McElwain
	Created: December 13, 1988   Last Changed: December 16, 1988  18:27.47
	Description: Table of the MSLAN dll entrypoints used by Interbase
   History:
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

#ifndef _REMOTE_LANENTRY_H_
#define _REMOTE_LANENTRY_H_

#define LAN_SERVERGETINFO	0
#define LAN_SHAREGETINFO	1
#define LAN_WAITNMPIPE		2
#define LAN_SETNMPHANDSTATE	3
#define LAN_MAKENMPIPE		4
#define LAN_CONNECTNMPIPE	5
#define LAN_DISCONNECTNMPIPE	6
#define LAN_SETNMPIPESEM	7
#define PROC_count		8

#define ENTRYNAME(module, name, address)	module, name, NULL,

typedef struct {
   TEXT *module;
   TEXT	*name;
   PFN	address;
} ENTRY;

static ENTRY	entrypoints [PROC_count] = {
    ENTRYNAME ("NETAPI", "NETSERVERGETINFO",	NETSERVERGETINFO)
    ENTRYNAME ("NETAPI", "NETSHAREGETINFO", NETSHAREGETINFO)
    ENTRYNAME ("NAMPIPES", "DOSWAITNMPIPE", DOSWAITNMPIPE)
    ENTRYNAME ("NAMPIPES", "DOSSETNMPHANDSTATE", DOSSETNMPHANDSTATE)
    ENTRYNAME ("NAMPIPES", "DOSMAKENMPIPE", DOSMAKENMPIPE)
    ENTRYNAME ("NAMPIPES", "DOSCONNECTNMPIPE", DOSCONNECTNMPIPE)
    ENTRYNAME ("NAMPIPES", "DOSDISCONNECTNMPIPE", DOSDISCONNECTNMPIPE)
    ENTRYNAME ("NAMPIPES", "DOSSETNMPIPESEM", DOSSETNMPIPESEM)
};

#define CALL(proc)  (entrypoints[proc].address)

#endif /* _REMOTE_LANENTRY_H_ */
