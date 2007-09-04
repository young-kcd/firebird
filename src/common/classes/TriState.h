/*
 *	PROGRAM:		Firebird aux classes.
 *	MODULE:			StatusHolder.cpp
 *	DESCRIPTION:	Firebird's SQL tri-state emulation class.
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
 *  The Original Code was created by Claudio Valderrama on 28-Aug-2007
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2007 Claudio Valderrama
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 */

#include "firebird.h"


class TriState
{
public:
	TriState();
	explicit TriState(bool input);
	void operator=(bool input);
	bool asBool() const;
	void reset();
	bool assignOnce(bool input);
	bool isUnknown() const;
	bool isAssigned() const;
	bool toggle();
private:
	bool m_init, m_val;
};

// The var is left uninitialized.
TriState::TriState()
	: m_init(false), m_val(false)
{
}

// The var is initialized to the explicit value.
TriState::TriState(bool input)
	: m_init(true), m_val(input)
{
}

// The var receives a T/F value.
void TriState::operator=(bool input)
{
	m_init = true;
	m_val = input;
}

// The var is coerced to a T/F value as result.
bool TriState::asBool() const
{
	return m_init && m_val;
}

// The var is returned to its uninitialized state.
void TriState::reset()
{
	m_init = m_val = false;
}

// The assignment succeeds only if the var is uninitialized.
bool TriState::assignOnce(bool input)
{
	if (m_init)
		return false;

	m_init = true;
	m_val = input;
	return true;
}

// Tests whether the var is uninitialized.
bool TriState::isUnknown() const
{
	return !m_init;
}

// Tests whether the var is initialized.
bool TriState::isAssigned() const
{
	return m_init;
}

// The var is toggled between T and F only if it's already initialized.
bool TriState::toggle()
{
	if (!m_init)
		return false;
		
	m_val = !m_val;
	return true;
}

