/*
 *	PROGRAM:	Dynamic  SQL RUNTIME SUPPORT
 *	MODULE:		errd_proto.h
 *	DESCRIPTION:	Prototype Header file for errd_proto.h
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

#ifndef DSQL_ERRD_PROTO_H
#define DSQL_ERRD_PROTO_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEV_BUILD
void ERRD_assert_msg(const char*, const char*, ULONG);
#endif

void ERRD_bugcheck(const char*);
void ERRD_error(int, const char*);
void ERRD_post(ISC_STATUS, ...);
BOOLEAN ERRD_post_warning(ISC_STATUS, ...);
void ERRD_punt(void);

#ifdef __cplusplus
}	/* extern "C" */
#endif

// macros for error generation

#define BUGCHECK(string)	ERRD_bugcheck(string)
#define IBERROR(code, string)	ERRD_error(code, string)
//#define BLKCHK(blk, type) if (blk->blk_type != (SCHAR) type) BUGCHECK ("expected type")
#define BLKCHK(blk, type) if (MemoryPool::blk_type(blk) != (SSHORT) type) BUGCHECK ("expected type")

// Macros for DEV_BUILD internal consistancy checking

#ifdef DEV_BUILD

/* Verifies that a pointed to block matches the expected type.
 Useful to find coding errors & memory globbers.

#define DEV_BLKCHK(blk, typ)	\
	{ \
	if ((blk) && (((BLK) (blk))->blk_type != (typ))) \
	    ERRD_assert_msg (assert_blkchk_msg, assert_filename, (ULONG) __LINE__); \
	}
*/

#define DEV_BLKCHK(blk, typ)	{						\
		if ((blk) && MemoryPool::blk_type(blk) != (SSHORT)typ) {	\
			ERRD_assert_msg((char*)assert_blkchk_msg,			\
							(char*)assert_filename,			\
							(ULONG) __LINE__);			\
		}												\
	}


#define _assert(ex)	{if (!(ex)){ERRD_assert_msg (NULL, (char*)assert_filename, __LINE__);}}
#undef assert
#define assert(ex)	_assert(ex)
#define ASSERT_FAIL ERRD_assert_msg (NULL, (char*)assert_filename, __LINE__)

// Define the assert_filename as a static variable to save on codespace

#define	ASSERT_FILENAME static UCHAR assert_filename[] = __FILE__; 
#define	ASSERT_BLKCHK_MSG static UCHAR assert_blkchk_msg[] = "Unexpected memory block type";	// NTX: dev

#else // PROD_BUILD

#define	ASSERT_FILENAME
#define	ASSERT_BLKCHK_MSG
#define DEV_BLKCHK(blk, typ)
#define _assert(ex)
#undef assert
#define assert(ex)
#define ASSERT_FAIL
#define	ASSERT_FILENAME

#endif // DEV_BUILD

#endif /* DSQL_ERRD_PROTO_H */
