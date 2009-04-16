/*
 *	PROGRAM:	Minimum replacement of JRD Access Method for utilities
 *	MODULE:		err.cpp
 *	DESCRIPTION:	Bug check routine
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
 *
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "MPEXL" port
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 */

#include "firebird.h"
#include "fb_exception.h"


void ERR_make_permanent(ISC_STATUS* s)
/**************************************
 *
 *	E R R _ m a k e _ p e r m a n e n t
 *
 **************************************
 *
 * Functional description
 *	Make strings in vector permanent
 *
 **************************************/
{
	Firebird::StringsBuffer::makeEnginePermanentVector(s);
}
