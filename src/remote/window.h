/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		window.h
 *	DESCRIPTION:	Common descriptions
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

#ifndef WINDOW_H
#define WINDOW_H

#define APP_HSIZE			220
#define APP_VSIZE			150

/* This is the title used by the hidden msgs window in the engine.
   Both fbclient.dll and fbserver.exe should use the same. */

#define APP_NAME			"Firebird Server"
#define APP_LABEL			"Firebird Server"

#define ON_NOTIFYICON		WM_USER + 2

#define WIN_TEXTLEN			128
#define MSG_STRINGLEN		64
#define DRV_STRINGLEN		32
#define TMP_STRINGLEN		512

#define CHECK_VOLUME(a)		((a)->dbcv_devicetype == DBT_DEVTYP_VOLUME)
#define CHECK_USAGE(a)		((a)->dbcv_unitmask & ulInUseMask)

/* This is the class name used by the hidden msgs window in the engine.
   Both fbclient.dll and fbserver.exe should use the same. */

static char *szClassName = "FB_Server";
static char *szWindowName = "Firebird Server";

#endif // WINDOW_H
