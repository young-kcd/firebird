/*
 *	PROGRAM:	JRD access method
 *	MODULE:		ibsetjmp.h
 *	DESCRIPTION:	Defines for setjmp/longjmp
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

#ifndef JRD_IBSETJMP_H
#define JRD_IBSETJMP_H


#ifdef HAVE_SETJMP_H
#include <setjmp.h>
#endif

#define SETJMP	setjmp
#define LONGJMP	longjmp
#define	JMP_BUF	jmp_buf

#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif

#define SIGSETJMP 	sigsetjmp
#define SIGLONGJMP 	siglongjmp
#define	SIGJMP_BUF	sigjmp_buf

#ifdef UNIX
#ifdef SUPERSERVER
#define START_CHECK_FOR_EXCEPTIONS(err)	{ \
					SIGJMP_BUF	sigenv; \
					int		sig; \
					if (!Config::getBugcheckAbort()) { \
						memcpy(&tdbb->tdbb_sigsetjmp, &sigenv, sizeof sigenv); \
						if (sig = SIGSETJMP (sigenv, 1)) \
					    	ISC_exception_post(sig, err); \
						ISC_sync_signals_set(); \
					}
#define END_CHECK_FOR_EXCEPTIONS(err)   if (!Config::getBugcheckAbort()) ISC_sync_signals_reset(); }
#endif /* SUPER_SERVER */

#endif /* UNIX */

#if defined(WIN_NT) && !defined(MINGW)
#ifdef SUPERSERVER
#include <excpt.h>
#define START_CHECK_FOR_EXCEPTIONS(err)	__try {
#define  END_CHECK_FOR_EXCEPTIONS(err) 	} __except ( ISC_exception_post(GetExceptionCode(), err)) { }
#endif /* SUPER_SERVER */
#endif /* WIN_NT */

/* generic macros */
#ifndef START_CHECK_FOR_EXCEPTIONS
#define START_CHECK_FOR_EXCEPTIONS(err)
#endif

#ifndef END_CHECK_FOR_EXCEPTIONS
#define END_CHECK_FOR_EXCEPTIONS(err)
#endif


#endif /* JRD_IBSETJMP_H */
