/*
 *	PROGRAM:		Firebird interface.
 *	MODULE:			firebird/Interface.h
 *	DESCRIPTION:	Base class for all FB interfaces / plugins.
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
 *  Copyright (c) 2010 Alex Peshkov <peshkoff at mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 */

#ifndef FB_INTERFACE_H
#define FB_INTERFACE_H

#include "ibase.h"
#include <assert.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define CLOOP_CARG __cdecl
#endif

struct dsc;
struct PerformanceInfo;

#include "IdlFbInterfaces.h"

namespace Firebird
{
	class FbException
	{
	public:
		FbException(IStatus* aStatus, const ISC_STATUS* vector)
		{
			aStatus->setErrors(vector);
			status = aStatus->clone();
		}

		FbException(IStatus* aStatus)
			: status(aStatus->clone())
		{
		}

		FbException(const FbException& copy)
			: status(copy.status->clone())
		{
		}

		FbException& operator =(const FbException& copy)
		{
			status->dispose();
			status = copy.status->clone();
			return *this;
		}

		virtual ~FbException()
		{
			status->dispose();
		}

	public:
		static void check(ISC_STATUS code, IStatus* status, const ISC_STATUS* vector)
		{
			if (code != 0 && vector[1])
				throw FbException(status, vector);
		}

	public:
		IStatus* getStatus() const
		{
			return status;
		}

	private:
		IStatus* status;
	};

	template <typename T>
	class BaseStatusWrapper : public IStatusImpl<T, T>
	{
	public:
		BaseStatusWrapper(IStatus* aStatus)
			: status(aStatus),
			  dirty(false)
		{
		}

	public:
		static void catchException(IStatus* status)
		{
			try
			{
				throw;
			}
			catch (const FbException& e)
			{
				status->setErrors(e.getStatus()->getErrors());
			}
			catch (...)
			{
				ISC_STATUS statusVector[] = {
					isc_arg_gds, isc_random,
					isc_arg_string, (ISC_STATUS) "Unrecognized C++ exception",
					isc_arg_end};
				status->setErrors(statusVector);
			}
		}

		static void setVersionError(IStatus* status, const char* /*interfaceName*/,
			unsigned currentVersion, unsigned expectedVersion)
		{
			//// TODO: use interfaceName

			intptr_t codes[] = {
				isc_arg_gds,
				isc_interface_version_too_old,
				isc_arg_number,
				(intptr_t) expectedVersion,
				isc_arg_number,
				(intptr_t) currentVersion,
				isc_arg_end
			};

			status->setErrors(codes);
		}

	public:
		virtual void dispose()
		{
			// Disposes only the delegated status. Let the user destroy this instance.
			status->dispose();
		}

		virtual void init()
		{
			status->init();
		}

		virtual unsigned getStatus() const
		{
			return dirty ? status->getStatus() : 0;
		}

		virtual void setErrors2(unsigned length, const intptr_t* value)
		{
			dirty = true;
			status->setErrors2(length, value);
		}

		virtual void setWarnings2(unsigned length, const intptr_t* value)
		{
			dirty = true;
			status->setWarnings2(length, value);
		}

		virtual void setErrors(const intptr_t* value)
		{
			dirty = true;
			status->setErrors(value);
		}

		virtual void setWarnings(const intptr_t* value)
		{
			dirty = true;
			status->setWarnings(value);
		}

		virtual const intptr_t* getErrors() const
		{
			return status->getErrors();
		}

		virtual const intptr_t* getWarnings() const
		{
			return status->getWarnings();
		}

		virtual IStatus* clone() const
		{
			return status->clone();
		}

	protected:
		IStatus* status;
		bool dirty;
	};

	class CheckStatusWrapper : public BaseStatusWrapper<CheckStatusWrapper>
	{
	public:
		CheckStatusWrapper(IStatus* aStatus)
			: BaseStatusWrapper(aStatus)
		{
		}

	public:
		static void checkException(CheckStatusWrapper* status)
		{
		}
	};

	class ThrowStatusWrapper : public BaseStatusWrapper<ThrowStatusWrapper>
	{
	public:
		ThrowStatusWrapper(IStatus* aStatus)
			: BaseStatusWrapper(aStatus)
		{
		}

	public:
		static void checkException(ThrowStatusWrapper* status)
		{
			if (status->dirty && (status->getStatus() & IStatus::FB_HAS_ERRORS))
				throw FbException(status->status);
		}
	};

	// Additional API function.
	// Should be used only in non-plugin modules.
	// All plugins including providers should use passed at init time interface instead.
	extern "C" IMaster* ISC_EXPORT fb_get_master_interface();

//// TODO: Move checks of INCLUDE_Firebird_H to out of public files.
#ifdef INCLUDE_Firebird_H		// Building internal module

// This item is for ISC API emulation only
// It may be gone in future versions
// Please do not use it!
	static IMessageMetadata* const DELAYED_OUT_FORMAT = reinterpret_cast<IMessageMetadata*>(1);

#endif //INCLUDE_Firebird_H

} // namespace Firebird

#define FB_PLUGIN_ENTRY_POINT firebird_plugin

#endif // FB_INTERFACE_H
