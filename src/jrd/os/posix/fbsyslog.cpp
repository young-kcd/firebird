/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		fbsyslog.cpp
 *	DESCRIPTION:	System log facility (posix)
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
 *
 *  Copyright (c) 2003 Alexander Peshkoff <peshkoff@mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "../jrd/os/fbsyslog.h"

#include <syslog.h>

namespace Firebird {
void Syslog::Record(Severity level, string Msg) {
	int priority = LOG_ERR;
	switch (level) {
	case Warning:
		priority = LOG_WARNING;
		break;
	case Error:
	default:
		priority = LOG_ERR;
		break;
	}
	syslog(priority|LOG_LOCAL3, "%s (%m)", Msg.c_str());
}
}
