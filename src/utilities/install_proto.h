/*
 *	PROGRAM:		Windows NT GDS Client Library Installation
 *	MODULE:	 		gdsclient_proto.h
 *	DESCRIPTION:	Prototypes for GDS32.DLL installation functions
 *
 *  The contents of this file are subject to the Initial Developer's 
 *  Public License Version 1.0 (the "License"); you may not use this 
 *  file except in compliance with the License. You may obtain a copy 
 *  of the License here:
 *
 *    http://www.ibphoenix.com?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed on an "AS 
 *  IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or 
 *  implied. See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code is (C) 2003 Olivier Mascia.
 *  
 *  The Initial Developer of the Original Code is Olivier Mascia.
 *
 *  All Rights Reserved.
 *  
 *  Contributor(s): ______________________________________. 
 *
 */

#ifndef _UTILITIES_INSTALL_PROTO_H_
#define _UTILITIES_INSTALL_PROTO_H_

extern USHORT CLIENT_install(const TEXT* rootdirectory, USHORT client,
	bool sw_force, USHORT(*err_handler)(ULONG, const TEXT*));

extern USHORT CLIENT_remove(const TEXT* rootdirectory, USHORT client,
	bool sw_force, USHORT(*err_handler)(ULONG, const TEXT*));

extern USHORT CLIENT_query(USHORT client, ULONG& verMS, ULONG& verLS,
	ULONG& sharedCount, USHORT(*err_handler)(ULONG, const TEXT *));

#endif /* _UTILITIES_GDSCLIENT_PROTO_H_ */
