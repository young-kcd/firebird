/*
 *	PROGRAM:	Security data base manager
 *	MODULE:		secur_proto.h
 *	DESCRIPTION:	Prototype header file for security.epp
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

#ifndef UTILITIES_SECUR_PROTO_H
#define UTILITIES_SECUR_PROTO_H

#include "../auth/AuthInterface.h"
#include "../common/classes/ImplementHelper.h"
#include "../common/classes/GetPlugins.h"

namespace Auth {

class CharField : public Firebird::StackIface<CharUserField>
{
public:
	CharField()
		: e(0), s(0), value(*getDefaultMemoryPool())
	{ }

	int FB_CARG entered()
	{
		return e;
	}

	int FB_CARG specified()
	{
		return s;
	}

	void FB_CARG setEntered(int newValue)
	{
		e = newValue;
	}

	void setSpecified(int newValue)
	{
		s = newValue;
		if (s)
		{
			value = "";
		}
	}

	const char* FB_CARG get()
	{
		return value.c_str();
	}

	void FB_CARG set(const char* newValue)
	{
		value = newValue ? newValue : "";
	}

	void FB_CARG set(const char* newValue, size_t len)
	{
		value.assign(newValue, len);
	}

	void clear()
	{
		e = s = 0;
		value = "";
	}

private:
	int e, s;
	Firebird::string value;
};

class IntField : public Firebird::StackIface<IntUserField>
{
public:
	IntField()
		: e(0), s(0), value(0)
	{ }

	int FB_CARG entered()
	{
		return e;
	}

	int FB_CARG specified()
	{
		return s;
	}

	void FB_CARG setEntered(int newValue)
	{
		e = newValue;
	}

	void setSpecified(int newValue)
	{
		s = newValue;
		if (s)
		{
			value = 0;
		}
	}

	int FB_CARG get()
	{
		return value;
	}

	void FB_CARG set(int newValue)
	{
		value = newValue;
	}

	void clear()
	{
		e = s = 0;
		value = 0;
	}

private:
	int e, s;
	int value;
};

class UserData : public User
{
public:
	UserData()
		: op(0), trustedRole(0), trustedAuth(0)
	{ }

	int FB_CARG operation()
	{
		return op;
	}

	CharUserField* FB_CARG userName()
	{
		return &user;
	}

	CharUserField* FB_CARG password()
	{
		return &pass;
	}

	CharUserField* FB_CARG firstName()
	{
		return &first;
	}

	CharUserField* FB_CARG lastName()
	{
		return &last;
	}

	CharUserField* FB_CARG middleName()
	{
		return &middle;
	}

	CharUserField* FB_CARG groupName()
	{
		return &group;
	}

	IntUserField* FB_CARG uid()
	{
		return &u;
	}

	IntUserField* FB_CARG gid()
	{
		return &g;
	}

	IntUserField* FB_CARG admin()
	{
		return &adm;
	}

	void FB_CARG clear();

	int op, trustedRole, trustedAuth;
	CharField user, pass, first, last, middle, group;
	IntField u, g, adm;
	CharField database, dba, dbaPassword, role, trustedUser;
};

class StackUserData : public Firebird::StackIface<UserData>
{
};

class Get : public Firebird::GetPlugins<Auth::Management>
{
public:
	Get(Config* firebirdConf);
};

} // namespace Auth

#endif // UTILITIES_SECUR_PROTO_H
