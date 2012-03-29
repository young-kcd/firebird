/*
 *	PROGRAM:	JRD International support
 *	MODULE:		intl_classes.h
 *	DESCRIPTION:	International text handling definitions
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
 *  The Original Code was created by Nickolay Samofatov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2004 Nickolay Samofatov <nickolay@broadviewsoftware.com>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 */

#ifndef JRD_INTL_CLASSES_H
#define JRD_INTL_CLASSES_H

#include "firebird.h"
#include "../jrd/jrd.h"
#include "../jrd/intlobj_new.h"
#include "../jrd/constants.h"
#include "../jrd/unicode_util.h"

typedef SSHORT CHARSET_ID;
typedef SSHORT COLLATE_ID;
typedef USHORT TTYPE_ID;

namespace Jrd {

class LikeObject {
public:
	virtual void reset() = 0;
	virtual bool process(thread_db*, TextType*, const UCHAR*, SLONG) = 0;
	virtual bool result() = 0;
	virtual ~LikeObject() {}
};

class ContainsObject {
public:
	virtual void reset() = 0;
	virtual bool process(Jrd::thread_db*, Jrd::TextType*, const UCHAR*, SLONG) = 0;
	virtual bool result() = 0;
	virtual ~ContainsObject() {}
};

} //namespace Jrd


#include "../jrd/CsConvert.h"
#include "../jrd/CharSet.h"
#include "../jrd/TextType.h"
#include "../jrd/Collation.h"


#endif	// JRD_INTL_CLASSES_H
