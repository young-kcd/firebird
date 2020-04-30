/*
*	PROGRAM:	Firebird interface.
*	MODULE:		TimerImpl.h
*	DESCRIPTION:	ITimer implementaton
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
*  The Original Code was created by Khorsun Vladyslav
*  for the Firebird Open Source RDBMS project.
*
*  Copyright (c) 2020 Khorsun Vladyslav <hvlad@users.sourceforge.net>
*  and all contributors signed below.
*
*  All Rights Reserved.
*  Contributor(s): ______________________________________.
*
*/

#ifndef FB_CLASSES_TIMER_IMPL
#define FB_CLASSES_TIMER_IMPL

#include "../../common/classes/ImplementHelper.h"
#include "../../common/classes/locks.h"

namespace Firebird {

class TimerImpl :
	public RefCntIface<ITimerImpl<TimerImpl, CheckStatusWrapper> >
{
public:
	TimerImpl() :
		m_fireTime(0),
		m_expTime(0)
	{ }

	// ITimer implementation
	void handler();
	int release();

	// Set timeout, seconds
	void reset(unsigned int timeout);
	void stop();

	SINT64 getExpireClock() const
	{
		return m_expTime;
	}

protected:
	// Descendants must override it
	virtual void onTimer(TimerImpl*) = 0;

private:
	Mutex m_mutex;
	SINT64 m_fireTime;		// when ITimer will fire, could be less than m_expTime
	SINT64 m_expTime;		// when actual idle timeout will expire
};


// Call member function of some class T::Fn()
template <typename T, void (T::*Fn)(TimerImpl*)>
class TimerTmpl : public TimerImpl
{
public:
	TimerTmpl(T* obj) : m_obj(obj) {}

protected:
	void onTimer(TimerImpl* arg)
	{
		(m_obj->*Fn)(arg);
	}

private:
	T* m_obj;
};


// Call static function with argument - instance of some RefCounted class T
template <typename T, void (Fn)(TimerImpl*, T*)>
class TimerTmplRef : public TimerImpl
{
public:
	TimerTmplRef(T* obj) : m_obj(obj) {}

protected:
	void onTimer(TimerImpl* arg1)
	{
		Fn(arg1, m_obj);
	}

private:
	RefPtr<T> m_obj;
};

} // namespace Firebird

#endif