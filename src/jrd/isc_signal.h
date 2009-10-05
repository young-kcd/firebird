/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		isc_signal.h
 *	DESCRIPTION:	InterBase pseudo-signal definitions
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

#ifndef _JRD_ISC_SIGNAL_H_
#define _JRD_ISC_SIGNAL_H_

#ifdef WIN_NT
/* There is no interprocess signaling on Windows NT.  We simulate it
   by sending messages through named pipes. */

#define BLOCKING_SIGNAL		1000	/* Lock manager */
#define WAKEUP_SIGNAL		1100	/* Lock manager */
#define EVENT_SIGNAL		1200	/* Event manager */
#define CACHE_SIGNALS		1300	/* Shared cache */
#define WAL_SIGNALS		1400	/* Write ahead log */
#endif

#endif /* _JRD_ISC_SIGNAL_H_ */
