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
 */

#ifndef JRD_FUNCTION_H
#define JRD_FUNCTION_H

#include "../common/classes/array.h"
#include "../jrd/dsc.h"
#include "../jrd/val.h"

namespace Jrd
{
	struct fun_repeat
	{
		Parameter* fun_parameter;	// parameter info
		FUN_T fun_mechanism;		// passing mechanism
	};

	class Function : public pool_alloc<type_fun>
	{
		static const USHORT MAX_ALTER = 64;	// Number of times an in-cache function can be altered
		static const char* const EXCEPTION_MESSAGE;

	public:
		static Function* lookup(thread_db* tdbb, USHORT id, bool return_deleted, bool noscan, USHORT flags);
		static Function* lookup(thread_db* tdbb, const Firebird::QualifiedName& name, bool noscan);

		void addRef();
		void release(thread_db* tdbb);

		bool isUsed() const
		{
			return (fun_use_count != 0);
		}

		void releaseLocks(thread_db* tdbb);
		dsc* execute(thread_db* tdbb, jrd_nod* args, impure_value* value);

	private:
		explicit Function(MemoryPool& p)
			: fun_name(p), fun_security_name(p), fun_args(p), fun_exception_message(p),
			  fun_legacy(true), fun_invariant(false)
		{}

		void remove(thread_db* tdbb);
		void parseBlr(thread_db* tdbb, bid* blob_id, CompilerScratch* csb);

		static Function* loadMetadata(thread_db* tdbb, USHORT id, bool noscan, USHORT flags);
		static int blockingAst(void*);

	public:
		USHORT fun_id;							// function ID
		Firebird::QualifiedName fun_name;		// function name
		Firebird::MetaName fun_security_name;	// security class name

		int (*fun_entrypoint)();				// function entrypoint
		USHORT fun_inputs;						// input arguments
		USHORT fun_return_arg;					// return argument
		ULONG fun_temp_length;					// temporary space required

		Firebird::Array<fun_repeat> fun_args;

		USHORT fun_flags;						// flags
		USHORT fun_use_count;					// requests compiled with function
		Lock* fun_existence_lock;				// existence lock, if any
		USHORT fun_alter_count;					// number of times function was altered
		jrd_req* fun_request;					// compiled function request

		Firebird::string fun_exception_message;	// message containing the exception error message

		bool fun_legacy;
		bool fun_invariant;
		ExtEngineManager::Function* fun_external;
	};

	const USHORT FUN_scanned			= 1;	// Field expressions scanned
	const USHORT FUN_obsolete			= 2;	// Procedure known gonzo
	const USHORT FUN_being_scanned		= 4;	// New procedure needs dependencies during scan
	const USHORT FUN_being_altered		= 8;	// Procedure is getting altered
	const USHORT FUN_check_existence	= 16;	// Existence lock released
}

#endif // JRD_FUNCTION_H
