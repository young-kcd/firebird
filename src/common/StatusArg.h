/*
 *	PROGRAM:		Firebird exceptions classes
 *	MODULE:			StatusArg.h
 *	DESCRIPTION:	Build status vector with variable number of elements
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
 *  The Original Code was created by Alex Peshkov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2008 Alex Peshkov <peshkoff at mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 */

#ifndef FB_STATUS_ARG
#define FB_STATUS_ARG


namespace Firebird {

class AbstractString;
class MetaName;

namespace Arg {

// forward
class Warning;

class Base
{
public:
	ISC_STATUS kind, value;

protected:
	Base(ISC_STATUS k, ISC_STATUS v) throw() : kind(k), value(v) { }
};

class StatusVector : public Base
{
protected:
	StatusVector(ISC_STATUS k, ISC_STATUS v) throw() : Base(k, v)
	{ 
		clear();
		operator<<(*(static_cast<Base*>(this)));
	}

public:
	explicit StatusVector(const ISC_STATUS* s) throw() : Base(0, 0)
	{ 
		clear();
		append(s, FB_NELEM(m_status_vector) - 1);
	}

	StatusVector() throw() : Base(0, 0)
	{ 
		clear();
	}

	~StatusVector() { }

	const ISC_STATUS* value() const throw() { return m_status_vector; }
	int length() const throw() { return m_length; }
	bool hasData() const throw() { return m_length > 0; }

	void clear() throw();
	void append(const StatusVector& v) throw();
	void raise() const;
	ISC_STATUS copyTo(ISC_STATUS* dest) const throw();

	// generic argument insert
	StatusVector& operator<<(const Base& arg) throw();
	// warning special case - to setup first warning location
	StatusVector& operator<<(const Warning& arg) throw();
	// Str special case - make the code simpler & better readable
	StatusVector& operator<<(const char* text) throw();
	StatusVector& operator<<(const AbstractString& text) throw();
	StatusVector& operator<<(const MetaName& text) throw();

private:
	ISC_STATUS_ARRAY m_status_vector;
	int m_length, m_warning;

private:
	bool appendErrors(const StatusVector* v) throw();
	bool appendWarnings(const StatusVector* v) throw();
	bool append(const ISC_STATUS* from, int count) throw();
};


class Gds : public StatusVector
{
public:
	explicit Gds(ISC_STATUS s) throw();
};

class Str : public Base
{
public:
	explicit Str(const char* text) throw();
	explicit Str(const AbstractString& text) throw();
	explicit Str(const MetaName& text) throw();
};

class Num : public Base
{
public:
	explicit Num(ISC_STATUS s) throw();
};

class Interpreted : public StatusVector
{
public:
	explicit Interpreted(const char* text) throw();
	explicit Interpreted(const AbstractString& text) throw();
};

class Unix : public Base
{
public:
	explicit Unix(ISC_STATUS s) throw();
};

class Windows : public Base
{
public:
	explicit Windows(ISC_STATUS s) throw();
};

class Warning : public StatusVector
{
public:
	explicit Warning(ISC_STATUS s) throw();
};

class SqlState : public Base
{
public:
	explicit SqlState(const char* text) throw();
	explicit SqlState(const AbstractString& text) throw();
};

} // namespace Arg

} // namespace Firebird


#endif // FB_STATUS_ARG
