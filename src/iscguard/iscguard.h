/*
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
#define GUARDIAN_APP_NAME        "Firebird Guardian"
#define GUARDIAN_APP_LABEL       "Firebird Guardian"
#define GUARDIAN_CLASS_NAME      "FB_Guard"
#define	FBSERVER	    "fbserver.exe"
#define GUARDIAN_HELP_FILE "fbserver.hlp"

/* Help Constants */
#define ibs_server_directory    8060
#define ibs_guard_version       8080
#define ibs_guard_log           8090

#define WM_SWITCHICONS  WM_USER + 3

#define START_ONCE      0
#define START_FOREVER   1

#define NORMAL_EXIT	0
#define CRASHED		-1

#ifdef VMS
/* also defined in common.h, if this is changed remember to change common.h */
#define STARTUP_ERROR   46
#else /* if not VMS */
/* also defined in common.h, if this is changed remember to change common.h */
#define STARTUP_ERROR   2
#endif /* VMS */
#define args             args

typedef void (*FPTR_VOID) ();
typedef struct log_info {
	char log_action[25];
	char log_date[25];
	char log_time[25];
	struct log_info *next;
} LOG_INFO;
