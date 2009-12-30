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
 * Adriano dos Santos Fernandes
 */

#ifndef JRD_ROUTINE_H
#define JRD_ROUTINE_H

#include "../common/classes/array.h"
#include "../common/classes/alloc.h"
#include "../common/classes/MetaName.h"

namespace Jrd
{
	class jrd_req;

	class Routine : public Firebird::PermanentStorage
	{
	protected:
		explicit Routine(MemoryPool& p)
			: PermanentStorage(p),
			  id(0),
			  name(p),
			  securityName(p),
			  request(NULL)
		{
		}

	public:
		virtual ~Routine()
		{
		}

	public:
		USHORT getId() const { return id; }
		void setId(USHORT value) { id = value; }

		const Firebird::QualifiedName& getName() const { return name; }
		void setName(const Firebird::QualifiedName& value) { name = value; }

		const Firebird::MetaName& getSecurityName() const { return securityName; }
		void setSecurityName(const Firebird::MetaName& value) { securityName = value; }

		/*const*/ jrd_req* getRequest() const { return request; }
		void setRequest(jrd_req* value) { request = value; }

	public:
		virtual int getObjectType() const = 0;
		virtual const char* getSclType() const = 0;

	private:
		USHORT id;							// routine ID
		Firebird::QualifiedName name;		// routine name
		Firebird::MetaName securityName;	// security class name
		jrd_req* request;					// compiled routine request
	};
}

#endif // JRD_ROUTINE_H
