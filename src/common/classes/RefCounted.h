/*
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
 *  The Original Code was created by Vlad Horsun
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2008 Vlad Horsun <hvlad@users.sf.net>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *  Dmitry Yemanov <dimitr@users.sf.net>
 */

#ifndef COMMON_REF_COUNTED_H
#define COMMON_REF_COUNTED_H

#include "../common/classes/fb_atomic.h"

namespace Firebird
{
	class RefCounted
	{
	public:
		virtual int addRef()
		{
			return ++m_refCnt;
		}

		virtual int release()
		{
			const int refCnt = --m_refCnt;
			if (!refCnt)
				delete this;
			return refCnt;
		}

	protected:
		RefCounted() : m_refCnt(0) {}
		virtual ~RefCounted() {}

	private:
		Firebird::AtomicCounter m_refCnt;
	};

}; // namespace

#endif // COMMON_REF_COUNTED_H
