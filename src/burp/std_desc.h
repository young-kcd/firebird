/*
 **************************************************************************
 *
 *	PROGRAM:		standard descriptors for gbak & gsplit
 *	MODULE:			std_desc.h
 *	DESCRIPTION:	standard descriptors for different OS's
 *
 **************************************************************************
 *
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Alexander Peshkoff
 *  for the Firebird Open Source RDBMS project.
 *  Thanks to Achim Kalwa <achim.kalwa@winkhaus.de>.
 *
 *  Copyright (c) 2003 Alexander Peshkoff <peshkoff@mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#ifndef GBAK_STD_DESC_H
#define GBAK_STD_DESC_H

#include "firebird.h"

#ifdef WIN_NT
#include <windows.h>
#define	GBAK_STDIN_DESC			GetStdHandle(STD_INPUT_HANDLE)	/* standart input  file descriptor */
#define	GBAK_STDOUT_DESC		GetStdHandle(STD_OUTPUT_HANDLE)	/* standart output file descriptor */
#else //WIN_NT
#define	GBAK_STDIN_DESC			(int)0	/* standart input  file descriptor */
#define	GBAK_STDOUT_DESC		(int)1	/* standart output file descriptor */
#endif //WIN_NT

#endif  //GBAK_STD_DESC_H
