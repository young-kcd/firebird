/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		flu.h
 *	DESCRIPTION:	Function lookup definitions
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
 *
 * 2002.10.28 Sean Leyne - Completed removal of obsolete "HP700" port
 *
 */

#ifndef JRD_FLU_H
#define JRD_FLU_H

/* External modules for UDFs/BLOB filters/y-valve loader */

#include "../common/classes/objects_array.h"
#include "../jrd/os/mod_loader.h"

namespace Jrd
{
	class Module
	{
	private:
		class InternalModule
		{
		private:
			InternalModule(const InternalModule &im);
			void operator=(const InternalModule &im);

		public:
			long useCount;
			ModuleLoader::Module* handle;
			Firebird::PathName originalName, loadName;

			void *findSymbol(const Firebird::string& name)
			{
				if (! handle)
				{
					return 0;
				}
				return handle->findSymbol(name);
			}

/*			explicit InternalModule(MemoryPool& p)
				: useCount(0), handle(0), 
				originalName(p), loadName(p) { } */
			InternalModule(MemoryPool& p, 
						   ModuleLoader::Module* h,
						   const Firebird::PathName& on, 
						   const Firebird::PathName& ln)
				: useCount(0), handle(h), 
				originalName(p, on), loadName(p, ln) { }

			~InternalModule()
			{
				fb_assert(useCount == 0);
				if (handle)
				{
					delete handle;
				}
			}

			bool operator==(const Firebird::PathName &pn) const
			{
				return originalName == pn || loadName == pn;
			}

			bool inUse() const
			{
				return useCount > 0;
			}

			void aquire()
			{
				fb_assert(handle);
				++useCount;
			}

			void release()
			{
				fb_assert(useCount > 0);
				--useCount;
			}
		};

		InternalModule* interMod;

		Module(InternalModule* h) : interMod(h)
		{
			if (interMod)
			{
				interMod->aquire();
			}
		}

		static Module lookupModule(const char*, bool);

		static InternalModule* scanModule(const Firebird::PathName& name);
			
	public:
		typedef Firebird::Array<InternalModule*> LoadedModules;

		Module() : interMod(0) { }

		Module(MemoryPool&) : interMod(0) { }

		Module(MemoryPool&, const Module& m) : interMod(m.interMod)
		{
			if (interMod)
			{
				interMod->aquire();
			}
		}

		Module(const Module& m) : interMod(m.interMod)
		{
			if (interMod)
			{
				interMod->aquire();
			}
		}

		~Module();

		// used for UDF/BLOB Filter
		static FPTR_INT lookup(const char*, const char*, Firebird::SortedObjectsArray<Module>&);

		// used in y-valve
		static FPTR_INT lookup(const char*, const char*);

		bool operator>(const Module &im) const
		{
			// we need it to sort on some key
			return interMod - im.interMod > 0;
		}

		void *lookupSymbol(const Firebird::string& name)
		{
			if (! interMod)
			{
				return 0;
			}
			return interMod->findSymbol(name);
		}

		operator bool()
		{
			return interMod;
		}
	};

	typedef Firebird::SortedObjectsArray<Module> DatabaseModules;

} // namespace Jrd

#endif /* JRD_FLU_H */
