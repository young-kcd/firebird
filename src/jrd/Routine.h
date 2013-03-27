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
#include "../common/classes/QualifiedName.h"
#include "../common/classes/NestConst.h"
#include "../common/MsgMetadata.h"

namespace Jrd
{
	class JrdStatement;
	class Format;
	class Parameter;

	class Routine : public Firebird::PermanentStorage
	{
	protected:
		explicit Routine(MemoryPool& p)
			: PermanentStorage(p),
			  id(0),
			  name(p),
			  securityName(p),
			  statement(NULL),
			  subRoutine(false),
			  implemented(true),
			  defined(true),
			  defaultCount(0),
			  inputFormat(NULL),
			  outputFormat(NULL),
			  inputFields(p),
			  outputFields(p)
		{
		}

	public:
		virtual ~Routine()
		{
		}

	public:
		static Firebird::MsgMetadata* createMetadata(
			const Firebird::Array<NestConst<Parameter> >& parameters);
		static Format* createFormat(MemoryPool& pool, Firebird::IMessageMetadata* params, bool addEof);

	public:
		USHORT getId() const
		{
			fb_assert(!subRoutine);
			return id;
		}

		void setId(USHORT value) { id = value; }

		const Firebird::QualifiedName& getName() const { return name; }
		void setName(const Firebird::QualifiedName& value) { name = value; }

		const Firebird::MetaName& getSecurityName() const { return securityName; }
		void setSecurityName(const Firebird::MetaName& value) { securityName = value; }

		/*const*/ JrdStatement* getStatement() const { return statement; }
		void setStatement(JrdStatement* value) { statement = value; }

		bool isSubRoutine() const { return subRoutine; }
		void setSubRoutine(bool value) { subRoutine = value; }

		bool isImplemented() const { return implemented; }
		void setImplemented(bool value) { implemented = value; }

		bool isDefined() const { return defined; }
		void setDefined(bool value) { defined = value; }

		USHORT getDefaultCount() const { return defaultCount; }
		void setDefaultCount(USHORT value) { defaultCount = value; }

		const Format* getInputFormat() const { return inputFormat; }
		void setInputFormat(const Format* value) { inputFormat = value; }

		const Format* getOutputFormat() const { return outputFormat; }
		void setOutputFormat(const Format* value) { outputFormat = value; }

		const Firebird::Array<NestConst<Parameter> >& getInputFields() const { return inputFields; }
		Firebird::Array<NestConst<Parameter> >& getInputFields() { return inputFields; }

		const Firebird::Array<NestConst<Parameter> >& getOutputFields() const { return outputFields; }
		Firebird::Array<NestConst<Parameter> >& getOutputFields() { return outputFields; }

	public:
		virtual int getObjectType() const = 0;
		virtual SLONG getSclType() const = 0;

	private:
		USHORT id;							// routine ID
		Firebird::QualifiedName name;		// routine name
		Firebird::MetaName securityName;	// security class name
		JrdStatement* statement;			// compiled routine statement
		bool subRoutine;					// Is this a subroutine?
		bool implemented;					// Is the packaged routine missing the body/entrypoint?
		bool defined;						// UDF has its implementation module available
		USHORT defaultCount;				// default input arguments
		const Format* inputFormat;			// input format
		const Format* outputFormat;			// output format
		Firebird::Array<NestConst<Parameter> > inputFields;		// array of field blocks
		Firebird::Array<NestConst<Parameter> > outputFields;	// array of field blocks
	};
}

#endif // JRD_ROUTINE_H
