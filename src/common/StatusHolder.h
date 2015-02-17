/*
 *	PROGRAM:		Firebird exceptions classes
 *	MODULE:			StatusHolder.h
 *	DESCRIPTION:	Firebird's exception classes
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
 *  The Original Code was created by Vlad Khorsun
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2007 Vlad Khorsun <hvlad at users.sourceforge.net>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 */

#ifndef FB_STATUS_HOLDER
#define FB_STATUS_HOLDER

#include "firebird/Interface.h"
#include "../common/utils_proto.h"
#include "../common/classes/ImplementHelper.h"
#include "../common/classes/array.h"

namespace Firebird {

template <class Final>
class BaseStatus : public IStatusImpl<Final, CheckStatusWrapper>
{
public:
	// IStatus implementation
	void init()
	{
		errors.init();
		warnings.init();
	}

	void setErrors(const ISC_STATUS* value)
	{
		errors.set(fb_utils::statusLength(value), value);
	}

	void setErrors2(unsigned int length, const ISC_STATUS* value)
	{
		errors.set(length, value);
	}

	void setWarnings(const ISC_STATUS* value)
	{
		warnings.set(fb_utils::statusLength(value), value);
	}

	void setWarnings2(unsigned int length, const ISC_STATUS* value)
	{
		warnings.set(length, value);
	}

	const ISC_STATUS* getErrors() const
	{
		return errors.get();
	}

	const ISC_STATUS* getWarnings() const
	{
		return warnings.get();
	}

	unsigned getStatus() const
	{
		return (errors.vector[1] ? IStatus::FB_HAS_ERRORS : 0) |
			   (warnings.vector[1] ? IStatus::FB_HAS_WARNINGS  : 0);
	}

	IStatus* clone() const
	{
		Final* ret = new Final();

		ret->setWarnings(getWarnings());
		ret->setErrors(getErrors());

		return ret;
	}

public:
	BaseStatus()
	{ }

	void check()
	{
		errors.check();
	}

private:
	class ErrorVector
	{
	public:
		ErrorVector()
		{
			init();
		}

		~ErrorVector() { }

		void set(unsigned int length, const ISC_STATUS* value)
		{
			fb_utils::copyStatus(vector, FB_NELEM(vector), value, length);
		}

		const ISC_STATUS* get() const
		{
			return vector;
		}

		void init()
		{
			fb_utils::init_status(vector);
		}

		void check()
		{
			if (vector[1])
				status_exception::raise(get());
		}

		ISC_STATUS vector[40];	// FixMe - may be a kind of dynamic storage will be better?
	};

	ErrorVector errors, warnings;
};

class LocalStatus : public AutoIface<BaseStatus<LocalStatus> >
{
public:
	void dispose()
	{
	}

public:
	static void checkException(LocalStatus* status)
	{
	}

	static void catchException(LocalStatus* status)
	{
		BaseStatusWrapper<LocalStatus>::catchException(status);
	}

	static void setVersionError(IStatus* status, const char* interfaceName,
		unsigned currentVersion, unsigned expectedVersion)
	{
		BaseStatusWrapper<LocalStatus>::setVersionError(
			status, interfaceName,currentVersion, expectedVersion);
	}
};


// This trivial container is used when we need to grow vector element by element w/o any conversions
typedef HalfStaticArray<ISC_STATUS, ISC_STATUS_LENGTH> SimpleStatusVector;


// DynamicStatusVector owns strings, contained in it
class DynamicStatusVector
{
public:
	DynamicStatusVector()
		: m_status_vector(*getDefaultMemoryPool())
	{
		ISC_STATUS* s = m_status_vector.getBuffer(ISC_STATUS_LENGTH);
		fb_utils::init_status(s);
	}

	~DynamicStatusVector()
	{
		clear();
	}

	ISC_STATUS save(const ISC_STATUS* status);
	ISC_STATUS save(const IStatus* status);
	void clear();

	ISC_STATUS getError() const
	{
		return value()[1];
	}

	const ISC_STATUS* value() const
	{
		return m_status_vector.begin();
	}

	bool isSuccess() const
	{
		return getError() == 0;
	}

	ISC_STATUS& operator[](unsigned int index)
	{
		return m_status_vector[index];
	}

private:
	SimpleStatusVector m_status_vector;
};


class StatusHolder
{
public:
	StatusHolder()
		: m_raised(false)
	{ }

	ISC_STATUS save(IStatus* status);
	void clear();
	void raise();

	ISC_STATUS getError()
	{
		return value()->getErrors()[1];
	}

	const IStatus* value()
	{
		if (m_raised) {
			clear();
		}

		m_rc.init();
		m_rc.setErrors(m_error.value());
		m_rc.setWarnings(m_warning.value());
		return &m_rc;
	}

	bool isSuccess()
	{
		return getError() == 0;
	}

	const StatusHolder& operator=(const StatusHolder& val)
	{
		m_error.save(val.m_error.value());
		m_warning.save(val.m_warning.value());
		m_raised = val.m_raised;
		return *this;
	}

private:
	DynamicStatusVector m_error, m_warning;
	LocalStatus m_rc;
	bool m_raised;
};


} // namespace Firebird


#endif // FB_STATUS_HOLDER
