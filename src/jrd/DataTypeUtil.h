/*
 *	PROGRAM:	
 *	MODULE:		DataTypeUtil.h
 *	DESCRIPTION:	Data Type Utility functions
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
 *  The Original Code was created by Adriano dos Santos Fernandes
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2006 Adriano dos Santos Fernandes <adrianosf@uol.com.br>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#ifndef JRD_DATATYPEUTIL_H
#define JRD_DATATYPEUTIL_H

struct dsc;

class DataTypeUtilBase
{
public:
	static SSHORT getResultBlobSubType(const dsc* value1, const dsc* value2);
	static USHORT getResultTextType(const dsc* value1, const dsc* value2);

private:
	static void clearDsc(dsc* desc);

public:
	ULONG convertLength(const dsc* src, const dsc* dst);
	ULONG fixLength(const dsc* desc, ULONG length);

	void makeConcatenate(dsc* result, const dsc* value1, const dsc* value2);
	void makeNullString(dsc* result);
	void makeSubstr(dsc* result, const dsc* value, const dsc* offset, const dsc* length);

public:
	virtual UCHAR maxBytesPerChar(UCHAR charSet) = 0;
};


namespace Jrd {

class thread_db;

class DataTypeUtil : public DataTypeUtilBase
{
public:
	DataTypeUtil(thread_db* original_tdbb)
		: tdbb(original_tdbb)
	{
	}

public:
	virtual UCHAR maxBytesPerChar(UCHAR charSet);

private:
	thread_db* tdbb;
};

}	// namespace Jrd

#endif	// JRD_DATATYPEUTIL_H
