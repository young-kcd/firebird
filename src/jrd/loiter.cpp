/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		loiter.c
 *	DESCRIPTION:	Waits for the lock to become free
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

#include <windows.h>
#include "../jrd/common.h"
#include "../jrd/thd.h"
#include "../jrd/thd_prot.h"
#include "../jrd/loiter.h"

/**************************************************************
 * LOITER_loiter yields control to the Windows non-pre-emptive
 * "operating system." It returns TRUE if the calling function is
 * to continue its processing, and FALSE if the calling function
 * is to break out of what-ever it was doing and return to the
 * master Executable.
 **************************************************************/
BOOLEAN API_ROUTINE LOITER_loiter(void)
{
	MSG msg;
/* save the context */
	THDD save_context;
	save_context = THD_get_specific();

	THD_restore_specific();

/* Empty the message queue and in the process yield control */
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			/* 
			 * Reintroduce the message so it can be processed at the
			 * right level, and break out of this low level message loop.
			 */
			PostMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
			return FALSE;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
/* restore the context */
	THD_put_specific(save_context);
	return TRUE;
}
