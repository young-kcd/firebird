/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		fbsyslog.cpp
 *	DESCRIPTION:	System log facility (win32)
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

#include <Windows.h>

namespace {
typedef HANDLE WINAPI tRegisterEventSource(
		LPCTSTR lpUNCServerName, 
		LPCTSTR lpSourceName);
typedef BOOL WINAPI tReportEvent(
		HANDLE hEventLog,
		WORD wType,
		WORD wCategory,
		DWORD dwEventID,
		PSID lpUserSid,
		WORD wNumStrings,
		DWORD dwDataSize,
		LPCTSTR *lpStrings,
		LPVOID lpRawData);

class SyslogAccess {
private:
	CRITICAL_SECTION cs;
	HANDLE LogHandle;
	tReportEvent *fReportEvent;
	bool InitFlag;
public:
	SyslogAccess() {
		InitializeCriticalSection(&cs); 
		InitFlag = false;
		LogHandle = 0;
	}
	~SyslogAccess() {
		DeleteCriticalSection(&cs);
	}
	void Record(WORD wType, Firebird::string Msg);
};

void SyslogAccess::Record(WORD wType, Firebird::string Msg) {
	EnterCriticalSection(&cs);
	if (! InitFlag) {
		InitFlag = true;
		HINSTANCE hLib = LoadLibrary("Advapi32");
		tRegisterEventSource *fRegisterEventSource = hLib ?
			(tRegisterEventSource *) GetProcAddress(hLib, "RegisterEventSourceA") : 0;
		fReportEvent = hLib ?
			(tReportEvent *) GetProcAddress(hLib, "ReportEventA") : 0;
		LogHandle = fRegisterEventSource && fReportEvent ?
			fRegisterEventSource(0, "Firebird SQL Server") : 0;
	}
	bool use9x = true;
	if (LogHandle) {
		LPCTSTR sb[1];
		sb[0] = Msg.c_str();
		if (fReportEvent(LogHandle, wType, 0, 0, 0, 1, 0, sb, 0)) {
			use9x = false;
		}
	}
	if (use9x) {
		::MessageBox(0, Msg.c_str(), "Firebird Error", MB_ICONSTOP);
	}
	LeaveCriticalSection(&cs);
}

class SyslogAccess iSyslogAccess;
}

namespace Firebird {
void Syslog::Record(Severity level, string Msg) {
	WORD wType = EVENTLOG_ERROR_TYPE;
	switch (level) {
	case Warning:
		wType = EVENTLOG_INFORMATION_TYPE;
		break;
	case Error:
	default:
		wType = EVENTLOG_ERROR_TYPE;
		break;
	}
	iSyslogAccess.Record(wType, Msg);
}
}
