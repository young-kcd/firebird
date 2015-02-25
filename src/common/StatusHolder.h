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

unsigned makeDynamicStrings(unsigned length, ISC_STATUS* const dst, const ISC_STATUS* const src);
void freeDynamicStrings(unsigned length, ISC_STATUS* ptr);


// This trivial container is used when we need to grow vector element by element w/o any conversions
template <unsigned S = ISC_STATUS_LENGTH>
class SimpleStatusVector : public HalfStaticArray<ISC_STATUS, S>
{
public:
	SimpleStatusVector()
		: HalfStaticArray<ISC_STATUS, S>()
	{ }

	SimpleStatusVector(MemoryPool& p)
		: HalfStaticArray<ISC_STATUS, S>(p)
	{ }
};


// DynamicVector owns strings, contained in it
template <unsigned S>
class DynamicVector : private SimpleStatusVector<S>
{
public:
	DynamicVector(MemoryPool& p)
		: SimpleStatusVector<S>(p)
	{
		fb_utils::init_status(this->getBuffer(3));
	}

	~DynamicVector()
	{
		freeDynamicStrings(this->getCount(), this->begin());
	}

	void clear()
	{
		freeDynamicStrings(this->getCount(), this->begin());
		this->resize(0);
		fb_utils::init_status(this->getBuffer(3));
	}

	void save(unsigned int length, const ISC_STATUS* status)
	{
		clear();
		this->resize(0);
		unsigned newLen = makeDynamicStrings(length, this->getBuffer(length), status);

		fb_assert(newLen <= length);

		// Sanity check
		if (newLen < 2)
			fb_utils::init_status(this->getBuffer(3));
		else
			this->resize(newLen);
	}

	ISC_STATUS save(const ISC_STATUS* status)
	{
		save(fb_utils::statusLength(status), status);
		return status[1];
	}

	const ISC_STATUS* value() const
	{
		return SimpleStatusVector<S>::begin();
	}

	ISC_STATUS operator[](unsigned int index) const
	{
		return SimpleStatusVector<S>::operator[](index);
	}
};


// DynamicStatusVector performs error/success checks and load from IStatus
class DynamicStatusVector : public DynamicVector<ISC_STATUS_LENGTH>
{
public:
	DynamicStatusVector()
		: DynamicVector(*getDefaultMemoryPool())
	{ }

	ISC_STATUS merge(const IStatus* status);

	ISC_STATUS getError() const
	{
		return value()[1];
	}

	bool isSuccess() const
	{
		return getError() == 0;
	}
};


// Implements IStatus using 2 DynamicVectors
template <class Final>
class BaseStatus : public IStatusImpl<Final, CheckStatusWrapper>
{
public:
	// IStatus implementation
	void init()
	{
		errors.clear();
		warnings.clear();
	}

	void setErrors(const ISC_STATUS* value)
	{
		errors.save(fb_utils::statusLength(value), value);
	}

	void setErrors2(unsigned int length, const ISC_STATUS* value)
	{
		errors.save(length, value);
	}

	void setWarnings(const ISC_STATUS* value)
	{
		warnings.save(fb_utils::statusLength(value), value);
	}

	void setWarnings2(unsigned int length, const ISC_STATUS* value)
	{
		warnings.save(length, value);
	}

	const ISC_STATUS* getErrors() const
	{
		return errors.value();
	}

	const ISC_STATUS* getWarnings() const
	{
		return warnings.value();
	}

	unsigned getState() const
	{
		return (errors.value()[1] ? IStatus::STATE_ERRORS : 0) |
			   (warnings.value()[1] ? IStatus::STATE_WARNINGS  : 0);
	}

	IStatus* clone() const
	{
		Final* ret = new Final();

		ret->setWarnings(getWarnings());
		ret->setErrors(getErrors());

		return ret;
	}

public:
	BaseStatus(MemoryPool& p)
		: errors(p), warnings(p)
	{
		init();
	}

	BaseStatus()
		: errors(*getDefaultMemoryPool()), warnings(*getDefaultMemoryPool())
	{
		init();
	}

	void check()
	{
		const ISC_STATUS* s = errors.value();
		if (s[0] == isc_arg_gds && s[1] != isc_arg_end)
			status_exception::raise(s);
	}

protected:
	void clear()
	{
		errors.clear();
		warnings.clear();
	}

private:
	DynamicVector<11> errors;		// gds-code (2) + 3 args (6) + stack trace (2) + end (1)
	DynamicVector<3> warnings;		// no warnings (empty vector)
};


// Hold status vector and raise error on request
class StatusHolder : public BaseStatus<StatusHolder>
{
public:
	StatusHolder()
		: BaseStatus(*getDefaultMemoryPool()), m_raised(false)
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
		if (m_raised)
			clear();

		return this;
	}

	bool isSuccess()
	{
		return getError() == 0;
	}

	const StatusHolder& operator=(const StatusHolder& val)
	{
		setErrors(val.getErrors());
		setWarnings(val.getWarnings());
		m_raised = val.m_raised;
		return *this;
	}

public:
	void dispose()
	{
	}

private:
	bool m_raised;
};


// Status interface to be used on stack when entering engine
class LocalStatus : public BaseStatus<LocalStatus>
{
public:
	LocalStatus()
		: BaseStatus<LocalStatus>(AutoStorage::getAutoMemoryPool())
	{ }

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

} // namespace Firebird


#endif // FB_STATUS_HOLDER
