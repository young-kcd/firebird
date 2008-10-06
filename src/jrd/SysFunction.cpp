/*
 *	PROGRAM:	JRD System Functions
 *	MODULE:		SysFunctions.h
 *	DESCRIPTION:	System Functions
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
 *  for the Firebird Open Source RDBMS project, based on work done
 *  in Yaffil by Oleg Loa <loa@mail.ru> and Alexey Karyakin <aleksey.karyakin@mail.ru>
 *
 *  Copyright (c) 2007 Adriano dos Santos Fernandes <adrianosf@uol.com.br>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *    Oleg Loa <loa@mail.ru>
 *    Alexey Karyakin <aleksey.karyakin@mail.ru>
 *
 */

#include "firebird.h"
#include "../jrd/SysFunction.h"
#include "../jrd/DataTypeUtil.h"
#include "../include/fb_blk.h"
#include "../jrd/exe.h"
#include "../jrd/intl.h"
#include "../jrd/req.h"
#include "../jrd/blb_proto.h"
#include "../jrd/cvt_proto.h"
#include "../common/cvt.h"
#include "../jrd/evl_proto.h"
#include "../jrd/intl_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/os/guid.h"
#include <math.h>

using namespace Firebird;
using namespace Jrd;


typedef double (*StdMathFunc)(double);	// std math function type

// function types handled in generic functions
enum Function
{
	funBinAnd,
	funBinOr,
	funBinShl,
	funBinShr,
	funBinXor,
	funBinNot,
	funMaxValue,
	funMinValue,
	funLPad,
	funRPad
};

// constants
const static int oneDay = 86400;

// auxiliary functions
static void add10msec(ISC_TIMESTAMP* v, int msec, SINT64 multiplier);
static double cot(double value);

// generic setParams functions
static void setParamsDouble(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, int argsCount, dsc** args);
static void setParamsFromList(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, int argsCount, dsc** args);
static void setParamsInteger(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, int argsCount, dsc** args);
static void setParamsSecondInteger(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, int argsCount, dsc** args);

// specific setParams functions
static void setParamsAsciiVal(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, int argsCount, dsc** args);
static void setParamsCharToUuid(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, int argsCount, dsc** args);
static void setParamsDateAdd(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, int argsCount, dsc** args);
static void setParamsDateDiff(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, int argsCount, dsc** args);
static void setParamsOverlay(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, int argsCount, dsc** args);
static void setParamsPosition(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, int argsCount, dsc** args);
static void setParamsRoundTrunc(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, int argsCount, dsc** args);
static void setParamsUuidToChar(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, int argsCount, dsc** args);

// generic make functions
static void makeDoubleResult(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args);
static void makeFromListResult(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args);
static void makeInt64Result(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args);
static void makeLongResult(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args);
///static void makeLongStringOrBlobResult(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args);
static void makeShortResult(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args);

// specific make functions
static void makeAbs(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args);
static void makeAsciiChar(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args);
static void makeBin(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args);
static void makeBinShift(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args);
static void makeCeilFloor(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args);
static void makeDateAdd(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args);
static void makeLeftRight(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args);
static void makeMod(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args);
static void makeOverlay(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args);
static void makePad(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args);
static void makeReplace(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args);
static void makeReverse(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args);
static void makeRound(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args);
static void makeTrunc(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args);
static void makeUuid(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args);
static void makeUuidToChar(DataTypeUtilBase* dataTypeUtil,const  SysFunction* function, dsc* result, int argsCount, const dsc** args);

// generic stdmath function
static dsc* evlStdMath(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);

// specific evl functions
static dsc* evlAbs(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);
static dsc* evlAsciiChar(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);
static dsc* evlAsciiVal(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);
static dsc* evlAtan2(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);
static dsc* evlBin(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);
static dsc* evlBinShift(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);
static dsc* evlCeil(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);
static dsc* evlCharToUuid(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);
static dsc* evlDateAdd(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);
static dsc* evlDateDiff(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);
static dsc* evlExp(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);
static dsc* evlFloor(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);
static dsc* evlGenUuid(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);
static dsc* evlHash(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);
static dsc* evlLeft(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);
static dsc* evlLn(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);
static dsc* evlLog(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);
static dsc* evlMaxMinValue(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);
static dsc* evlMod(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);
static dsc* evlOverlay(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);
static dsc* evlPad(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);
static dsc* evlPi(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);
static dsc* evlPosition(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);
static dsc* evlPower(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);
static dsc* evlRand(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);
static dsc* evlReplace(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);
static dsc* evlReverse(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);
static dsc* evlRight(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);
static dsc* evlRound(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);
static dsc* evlSign(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);
static dsc* evlSqrt(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);
static dsc* evlTrunc(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);
static dsc* evlUuidToChar(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure);


static void add10msec(ISC_TIMESTAMP* v, int msec, SINT64 multiplier)
{
	const SINT64 full = msec * multiplier;
	const int days = full / (oneDay * ISC_TIME_SECONDS_PRECISION);
	const int secs = full % (oneDay * ISC_TIME_SECONDS_PRECISION);

	v->timestamp_date += days;

	// Time portion is unsigned, so we avoid unsigned rolling over negative values
	// that only produce a new unsigned number with the wrong result.
	if (secs < 0 && ISC_TIME(-secs) > v->timestamp_time)
	{
		v->timestamp_date--;
		v->timestamp_time += (oneDay * ISC_TIME_SECONDS_PRECISION) + secs;
	}
	else if ((v->timestamp_time += secs) >= (oneDay * ISC_TIME_SECONDS_PRECISION))
	{
		v->timestamp_date++;
		v->timestamp_time -= (oneDay * ISC_TIME_SECONDS_PRECISION);
	}
}


static double cot(double value)
{
	return 1.0 / tan(value);
}


static bool initResult(dsc* result, int argsCount, const dsc** args, bool* isNullable)
{
	*isNullable = false;

	for (int i = 0; i < argsCount; ++i)
	{
		if (args[i]->isNull())
		{
			result->setNull();
			return true;
		}

		if (args[i]->isNullable())
			*isNullable = true;
	}

	return false;
}


static void setParamsDouble(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, int argsCount, dsc** args)
{
	for (int i = 0; i < argsCount; ++i)
	{
		if (args[i]->isUnknown())
			args[i]->makeDouble();
	}
}


static void setParamsFromList(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, int argsCount, dsc** args)
{
	dsc desc;
	dataTypeUtil->makeFromList(&desc, function->name.c_str(), argsCount, const_cast<const dsc**>(args));

	for (int i = 0; i < argsCount; ++i)
	{
		if (args[i]->isUnknown())
			*args[i] = desc;
	}
}


static void setParamsInteger(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, int argsCount, dsc** args)
{
	for (int i = 0; i < argsCount; ++i)
	{
		if (args[i]->isUnknown())
			args[i]->makeLong(0);
	}
}


static void setParamsSecondInteger(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, int argsCount, dsc** args)
{
	if (argsCount >= 2)
	{
		if (args[1]->isUnknown())
			args[1]->makeLong(0);
	}
}


static void setParamsAsciiVal(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, int argsCount, dsc** args)
{
	if (argsCount >= 1 && args[0]->isUnknown())
		args[0]->makeText(1, CS_ASCII);
}


static void setParamsCharToUuid(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, int argsCount, dsc** args)
{
	if (argsCount >= 1 && args[0]->isUnknown())
		args[0]->makeText(GUID_BODY_SIZE, ttype_ascii);
}


static void setParamsDateAdd(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, int argsCount, dsc** args)
{
	if (argsCount >= 1 && args[0]->isUnknown())
		args[0]->makeLong(0);

	if (argsCount >= 3 && args[2]->isUnknown())
		args[2]->makeTimestamp();
}


static void setParamsDateDiff(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, int argsCount, dsc** args)
{
	if (argsCount >= 3)
	{
		if (args[1]->isUnknown() && args[2]->isUnknown())
		{
			args[1]->makeTimestamp();
			args[2]->makeTimestamp();
		}
		else if (args[1]->isUnknown())
			*args[1] = *args[2];
		else if (args[2]->isUnknown())
			*args[2] = *args[1];
	}
}


static void setParamsOverlay(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, int argsCount, dsc** args)
{
	if (argsCount >= 3)
	{
		if (!(args[0]->isUnknown() && args[1]->isUnknown()))
		{
			if (args[1]->isUnknown())
				*args[1] = *args[0];
			else if (args[0]->isUnknown())
				*args[0] = *args[1];
		}

		if (argsCount >= 4)
		{
			if (args[2]->isUnknown() && args[3]->isUnknown())
			{
				args[2]->makeLong(0);
				args[3]->makeLong(0);
			}
			else if (args[2]->isUnknown())
				*args[2] = *args[3];
			else if (args[3]->isUnknown())
				*args[3] = *args[2];
		}

		if (args[2]->isUnknown())
			args[2]->makeLong(0);
	}
}


static void setParamsPosition(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, int argsCount, dsc** args)
{
	if (argsCount >= 2)
	{
		if (args[0]->isUnknown())
			*args[0] = *args[1];

		if (args[1]->isUnknown())
			*args[1] = *args[0];
	}
}


static void setParamsRoundTrunc(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, int argsCount, dsc** args)
{
	if (argsCount >= 1)
	{
		if (args[0]->isUnknown())
			args[0]->makeDouble();

		if (argsCount >= 2)
		{
			if (args[1]->isUnknown())
				args[1]->makeLong(0);
		}
	}
}


static void setParamsUuidToChar(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, int argsCount, dsc** args)
{
	if (argsCount >= 1 && args[0]->isUnknown())
		args[0]->makeText(16, ttype_binary);
}


static void makeDoubleResult(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args)
{
	result->makeDouble();

	bool isNullable;
	if (initResult(result, argsCount, args, &isNullable))
		return;

	result->setNullable(isNullable);
}


static void makeFromListResult(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args)
{
	result->clear();
	dataTypeUtil->makeFromList(result, function->name.c_str(), argsCount, args);
}


static void makeInt64Result(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args)
{
	if (dataTypeUtil->getDialect() == 1)
		result->makeDouble();
	else
		result->makeInt64(0);

	bool isNullable;
	if (initResult(result, argsCount, args, &isNullable))
		return;

	result->setNullable(isNullable);
}


static void makeLongResult(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args)
{
	result->makeLong(0);

	bool isNullable;
	if (initResult(result, argsCount, args, &isNullable))
		return;

	result->setNullable(isNullable);
}


/***
 * This function doesn't work yet, because makeFromListResult isn't totally prepared for blobs vs strings.
 *
static void makeLongStringOrBlobResult(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args)
{
	makeFromListResult(dataTypeUtil, function, result, argsCount, args);

	if (result->isText())
		result->makeVarying(dataTypeUtil->fixLength(result, MAX_COLUMN_SIZE), result->getTextType());
}
***/


static void makeShortResult(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args)
{
	result->makeShort(0);

	bool isNullable;
	if (initResult(result, argsCount, args, &isNullable))
		return;

	result->setNullable(isNullable);
}


static void makeAbs(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args)
{
	fb_assert(argsCount == function->minArgCount);

	const dsc* value = args[0];

	if (value->isNull())
	{
		result->makeLong(0);
		result->setNull();
		return;
	}

	switch (value->dsc_dtype)
	{
		case dtype_short:
			result->makeLong(value->dsc_scale);
			break;

		case dtype_long:
			if (dataTypeUtil->getDialect() == 1)
				result->makeDouble();
			else
				result->makeInt64(value->dsc_scale);
			break;

		case dtype_real:
		case dtype_double:
		case dtype_int64:
			*result = *value;
			break;

		default:
			result->makeDouble();
			break;
	}

	result->setNullable(value->isNullable());
}


static void makeAsciiChar(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args)
{
	fb_assert(argsCount == function->minArgCount);

	const dsc* value = args[0];

	if (value->isNull())
	{
		result->makeNullString();
		return;
	}

	result->makeText(1, ttype_none);
	result->setNullable(value->isNullable());
}


static void makeBin(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args)
{
	fb_assert(argsCount >= function->minArgCount);

	bool isNullable = false;
	bool isNull = false;
	bool first = true;

	for (int i = 0; i < argsCount; ++i)
	{
		if (args[i]->isNullable())
			isNullable = true;

		if (args[i]->isNull())
		{
			isNull = true;
			continue;
		}

		if (!args[i]->isExact() || args[i]->dsc_scale != 0)
			status_exception::raise(Arg::Gds(isc_expression_eval_err) <<
										Arg::Gds(isc_sysf_argmustbe_exact) << Arg::Str(function->name));

		if (first)
		{
			first = false;

			result->clear();
			result->dsc_dtype = args[i]->dsc_dtype;
			result->dsc_length = args[i]->dsc_length;
		}
		else
		{
			if (args[i]->dsc_dtype == dtype_int64)
				result->makeInt64(0);
			else if (args[i]->dsc_dtype == dtype_long && result->dsc_dtype != dtype_int64)
				result->makeLong(0);
		}
	}

	if (isNull)
	{
		if (first)
			result->makeLong(0);
		result->setNull();
	}

	result->setNullable(isNullable);
}


static void makeBinShift(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args)
{
	fb_assert(argsCount >= function->minArgCount);

	result->makeInt64(0);

	bool isNullable = false;

	for (int i = 0; i < argsCount; ++i)
	{
		if (args[i]->isNull())
		{
			result->setNull();
			return;
		}

		if (args[i]->isNullable())
			isNullable = true;

		if (!args[i]->isExact() || args[i]->dsc_scale != 0)
			status_exception::raise(Arg::Gds(isc_expression_eval_err) <<
										Arg::Gds(isc_sysf_argmustbe_exact) << Arg::Str(function->name));
	}

	result->setNullable(isNullable);
}


static void makeCeilFloor(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args)
{
	fb_assert(argsCount == function->minArgCount);

	const dsc* value = args[0];

	if (value->isNull())
	{
		result->makeLong(0);
		result->setNull();
		return;
	}

	switch (value->dsc_dtype)
	{
		case dtype_short:
			result->makeLong(0);
			break;

		case dtype_long:
		case dtype_int64:
			result->makeInt64(0);
			break;

		default:
			result->makeDouble();
			break;
	}

	result->setNullable(value->isNullable());
}


static void makeDateAdd(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args)
{
	fb_assert(argsCount >= 3);

	*result = *args[2];

	bool isNullable;
	if (initResult(result, argsCount, args, &isNullable))
		return;

	*result = *args[2];
	result->setNullable(isNullable);
}


static void makeLeftRight(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args)
{
	fb_assert(argsCount == function->minArgCount);

	const dsc* value = args[0];
	const dsc* length = args[1];

	if (value->isNull() || length->isNull())
	{
		result->makeNullString();
		return;
	}

	if (value->isBlob())
		result->makeBlob(value->getBlobSubType(), value->getTextType());
	else
	{
		result->clear();
		result->dsc_dtype = dtype_varying;
		result->setTextType(value->getTextType());
		result->setNullable(value->isNullable() || length->isNullable());

		result->dsc_length = dataTypeUtil->fixLength(
			result, dataTypeUtil->convertLength(value, result)) + sizeof(USHORT);
	}
}


static void makeMod(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args)
{
	fb_assert(argsCount == function->minArgCount);

	const dsc* value1 = args[0];
	const dsc* value2 = args[1];

	if (value1->isNull() || value2->isNull())
	{
		result->makeLong(0);
		result->setNull();
		return;
	}

	switch (value1->dsc_dtype)
	{
		case dtype_short:
		case dtype_long:
		case dtype_int64:
			*result = *value1;
			result->dsc_scale = 0;
			break;

		default:
			result->makeInt64(0);
			break;
	}

	result->setNullable(value1->isNullable() || value2->isNullable());
}


static void makeOverlay(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args)
{
	fb_assert(argsCount >= function->minArgCount);

	result->makeNullString();

	bool isNullable;
	if (initResult(result, argsCount, args, &isNullable))
		return;

	const dsc* value = args[0];
	const dsc* placing = args[1];

	if (value->isBlob())
		*result = *value;
	else if (placing->isBlob())
		*result = *placing;
	else
	{
		result->clear();
		result->dsc_dtype = dtype_varying;
	}

	result->setBlobSubType(dataTypeUtil->getResultBlobSubType(value, placing));
	result->setTextType(dataTypeUtil->getResultTextType(value, placing));

	if (!value->isBlob() && !placing->isBlob())
	{
		result->dsc_length = sizeof(USHORT) +
			dataTypeUtil->convertLength(value, result) +
			dataTypeUtil->convertLength(placing, result);
	}

	result->setNullable(isNullable);
}


static void makePad(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args)
{
	fb_assert(argsCount >= function->minArgCount);

	result->makeNullString();

	bool isNullable;
	if (initResult(result, argsCount, args, &isNullable))
		return;

	const dsc* value1 = args[0];
	const dsc* value2 = (argsCount >= 3 ? args[2] : NULL);

	if (value1->isBlob())
		*result = *value1;
	else if (value2 && value2->isBlob())
		*result = *value2;
	else
	{
		result->clear();
		result->dsc_dtype = dtype_varying;
	}

	result->setBlobSubType(value1->getBlobSubType());
	result->setTextType(value1->getTextType());

	if (!result->isBlob())
		result->dsc_length = sizeof(USHORT) + dataTypeUtil->fixLength(result, MAX_COLUMN_SIZE);

	result->setNullable(isNullable);
}


static void makeReplace(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args)
{
	fb_assert(argsCount >= function->minArgCount);

	bool isNullable = false;
	const dsc* firstBlob = NULL;

	for (int i = 0; i < argsCount; ++i)
	{
		if (args[i]->isNull())
		{
			result->makeNullString();
			return;
		}

		if (args[i]->isNullable())
			isNullable = true;

		if (!firstBlob && args[i]->isBlob())
			firstBlob = args[i];
	}

	const dsc* searched = args[0];
	const dsc* find = args[1];
	const dsc* replacement = args[2];

	if (firstBlob)
		*result = *firstBlob;
	else
	{
		result->clear();
		result->dsc_dtype = dtype_varying;
	}

	result->setBlobSubType(dataTypeUtil->getResultBlobSubType(searched, find));
	result->setBlobSubType(dataTypeUtil->getResultBlobSubType(result, replacement));

	result->setTextType(dataTypeUtil->getResultTextType(searched, find));
	result->setTextType(dataTypeUtil->getResultTextType(result, replacement));

	if (!firstBlob)
	{
		const int searchedLen = dataTypeUtil->convertLength(searched, result);
		const int findLen = dataTypeUtil->convertLength(find, result);
		const int replacementLen = dataTypeUtil->convertLength(replacement, result);

		if (findLen == 0)
			result->dsc_length = dataTypeUtil->fixLength(result, searchedLen) + sizeof(USHORT);
		else
		{
			result->dsc_length = dataTypeUtil->fixLength(result, MAX(searchedLen,
				searchedLen + (searchedLen / findLen) * (replacementLen - findLen))) + sizeof(USHORT);
		}
	}

	result->setNullable(isNullable);
}


static void makeReverse(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args)
{
	fb_assert(argsCount == function->minArgCount);

	const dsc* value = args[0];

	if (value->isNull())
	{
		result->makeNullString();
		return;
	}

	if (value->isBlob())
		*result = *value;
	else
		result->makeVarying(value->getStringLength(), value->getTextType());

	result->setNullable(value->isNullable());
}


static void makeRound(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args)
{
	fb_assert(argsCount >= function->minArgCount);

	const dsc* value1 = args[0];

	if (value1->isNull() || (argsCount > 1 && args[1]->isNull()))
	{
		result->makeLong(0);
		result->setNull();
		return;
	}

	if (value1->isExact() || value1->dsc_dtype == dtype_real || value1->dsc_dtype == dtype_double)
	{
		*result = *value1;
		if (argsCount == 1)
			result->dsc_scale = 0;
	}
	else
		status_exception::raise(Arg::Gds(isc_expression_eval_err) <<
									Arg::Gds(isc_sysf_argmustbe_exact_or_fp) << Arg::Str(function->name));

	result->setNullable(value1->isNullable() || (argsCount > 1 && args[1]->isNullable()));
}


static void makeTrunc(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args)
{
	fb_assert(argsCount >= function->minArgCount);

	const dsc* value = args[0];

	if (value->isNull() || (argsCount == 2 && args[1]->isNull()))
	{
		result->makeLong(0);
		result->setNull();
		return;
	}

	switch (value->dsc_dtype)
	{
		case dtype_short:
		case dtype_long:
		case dtype_int64:
			*result = *value;
			if (argsCount == 1)
				result->dsc_scale = 0;
			break;

		default:
			result->makeDouble();
			break;
	}

	result->setNullable(value->isNullable() || (argsCount > 1 && args[1]->isNullable()));
}


static void makeUuid(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args)
{
	fb_assert(argsCount == function->minArgCount);

	if (argsCount > 0 && args[0]->isNull())
		result->makeNullString();
	else
		result->makeText(16, ttype_binary);

	if (argsCount > 0 && args[0]->isNullable())
		result->setNullable(true);
}


static void makeUuidToChar(DataTypeUtilBase* dataTypeUtil, const SysFunction* function, dsc* result, int argsCount, const dsc** args)
{
	fb_assert(argsCount == function->minArgCount);

	const dsc* value = args[0];

	if (value->isNull())
	{
		result->makeNullString();
		return;
	}

	result->makeText(GUID_BODY_SIZE, ttype_ascii);
	result->setNullable(value->isNullable());
}


static dsc* evlStdMath(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count == 1);
	fb_assert(function->misc != NULL);

	jrd_req* request = tdbb->getRequest();

	request->req_flags &= ~req_null;
	const dsc* value = EVL_expr(tdbb, args->nod_arg[0]);
	if (request->req_flags & req_null)	// return NULL if value is NULL
		return NULL;

	const double v = MOV_get_double(value);

	impure->vlu_misc.vlu_double = ((StdMathFunc) function->misc)(v);
	impure->vlu_desc.makeDouble(&impure->vlu_misc.vlu_double);

	return &impure->vlu_desc;
}


static dsc* evlAbs(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count == 1);

	jrd_req* request = tdbb->getRequest();

	request->req_flags &= ~req_null;
	const dsc* value = EVL_expr(tdbb, args->nod_arg[0]);
	if (request->req_flags & req_null)	// return NULL if value is NULL
		return NULL;

	EVL_make_value(tdbb, value, impure);

	switch (impure->vlu_desc.dsc_dtype)
	{
		case dtype_real:
			impure->vlu_misc.vlu_float = fabs(impure->vlu_misc.vlu_float);
			break;

		case dtype_double:
			impure->vlu_misc.vlu_double = fabs(impure->vlu_misc.vlu_double);
			break;

		case dtype_short:
		case dtype_long:
		case dtype_int64:
			impure->vlu_misc.vlu_int64 = MOV_get_int64(value, value->dsc_scale);

			if (impure->vlu_misc.vlu_int64 == MIN_SINT64)
				status_exception::raise(Arg::Gds(isc_arith_except) << Arg::Gds(isc_numeric_out_of_range));
			else if (impure->vlu_misc.vlu_int64 < 0)
				impure->vlu_misc.vlu_int64 = -impure->vlu_misc.vlu_int64;

			impure->vlu_desc.makeInt64(value->dsc_scale, &impure->vlu_misc.vlu_int64);
			break;

		default:
			impure->vlu_misc.vlu_double = fabs(MOV_get_double(&impure->vlu_desc));
			impure->vlu_desc.makeDouble(&impure->vlu_misc.vlu_double);
			break;
	}

	return &impure->vlu_desc;
}


static dsc* evlAsciiChar(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count == 1);

	jrd_req* request = tdbb->getRequest();

	request->req_flags &= ~req_null;
	const dsc* value = EVL_expr(tdbb, args->nod_arg[0]);
	if (request->req_flags & req_null)	// return NULL if value is NULL
		return NULL;

	const SLONG code = MOV_get_long(value, 0);
	if (!(code >= 0 && code <= 255))
		status_exception::raise(Arg::Gds(isc_arith_except) << Arg::Gds(isc_numeric_out_of_range));

	impure->vlu_misc.vlu_uchar = (UCHAR) code;
	impure->vlu_desc.makeText(1, ttype_none, &impure->vlu_misc.vlu_uchar);

	return &impure->vlu_desc;
}


static dsc* evlAsciiVal(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count == 1);

	jrd_req* request = tdbb->getRequest();

	request->req_flags &= ~req_null;
	const dsc* value = EVL_expr(tdbb, args->nod_arg[0]);
	if (request->req_flags & req_null)	// return NULL if value is NULL
		return NULL;

	const CharSet* cs = INTL_charset_lookup(tdbb, value->getCharSet());

	UCHAR* p;
	MoveBuffer temp;
	const int length = MOV_make_string2(tdbb, value,
		(cs->isMultiByte() || cs->minBytesPerChar() > 1 ? ttype_ascii : value->getCharSet()),
		&p, temp);

	impure->vlu_misc.vlu_short = (length > 0 ? p[0] : 0);
	impure->vlu_desc.makeShort(0, &impure->vlu_misc.vlu_short);

	return &impure->vlu_desc;
}


static dsc* evlAtan2(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count == 2);

	jrd_req* request = tdbb->getRequest();

	request->req_flags &= ~req_null;
	const dsc* value1 = EVL_expr(tdbb, args->nod_arg[0]);
	if (request->req_flags & req_null)	// return NULL if value1 is NULL
		return NULL;

	const dsc* value2 = EVL_expr(tdbb, args->nod_arg[1]);
	if (request->req_flags & req_null)	// return NULL if value2 is NULL
		return NULL;

	impure->vlu_misc.vlu_double = atan2(MOV_get_double(value1), MOV_get_double(value2));
	impure->vlu_desc.makeDouble(&impure->vlu_misc.vlu_double);

	return &impure->vlu_desc;
}


static dsc* evlBin(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count >= 1);

	jrd_req* request = tdbb->getRequest();

	for (int i = 0; i < args->nod_count; ++i)
	{
		request->req_flags &= ~req_null;
		const dsc* value = EVL_expr(tdbb, args->nod_arg[i]);
		if (request->req_flags & req_null)	// return NULL if value is NULL
			return NULL;

		if (i == 0)
		{
			if ((Function)(IPTR) function->misc == funBinNot)
				impure->vlu_misc.vlu_int64 = ~MOV_get_int64(value, 0);
			else
				impure->vlu_misc.vlu_int64 = MOV_get_int64(value, 0);
		}
		else
		{
			switch ((Function)(IPTR) function->misc)
			{
				case funBinAnd:
					impure->vlu_misc.vlu_int64 &= MOV_get_int64(value, 0);
					break;

				case funBinOr:
					impure->vlu_misc.vlu_int64 |= MOV_get_int64(value, 0);
					break;

				case funBinXor:
					impure->vlu_misc.vlu_int64 ^= MOV_get_int64(value, 0);
					break;

				default:
					fb_assert(false);
			}
		}
	}

	impure->vlu_desc.makeInt64(0, &impure->vlu_misc.vlu_int64);

	return &impure->vlu_desc;
}


static dsc* evlBinShift(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count == 2);

	jrd_req* request = tdbb->getRequest();

	request->req_flags &= ~req_null;
	const dsc* value1 = EVL_expr(tdbb, args->nod_arg[0]);
	if (request->req_flags & req_null)	// return NULL if value1 is NULL
		return NULL;

	request->req_flags &= ~req_null;
	const dsc* value2 = EVL_expr(tdbb, args->nod_arg[1]);
	if (request->req_flags & req_null)	// return NULL if value2 is NULL
		return NULL;

	switch ((Function)(IPTR) function->misc)
	{
		case funBinShl:
			impure->vlu_misc.vlu_int64 = MOV_get_int64(value1, 0) << MOV_get_int64(value2, 0);
			break;

		case funBinShr:
			impure->vlu_misc.vlu_int64 = MOV_get_int64(value1, 0) >> MOV_get_int64(value2, 0);
			break;

		default:
			fb_assert(false);
	}

	impure->vlu_desc.makeInt64(0, &impure->vlu_misc.vlu_int64);

	return &impure->vlu_desc;
}


static dsc* evlCeil(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count == 1);

	jrd_req* request = tdbb->getRequest();

	request->req_flags &= ~req_null;
	const dsc* value = EVL_expr(tdbb, args->nod_arg[0]);
	if (request->req_flags & req_null)	// return NULL if value is NULL
		return NULL;

	EVL_make_value(tdbb, value, impure);

	switch (impure->vlu_desc.dsc_dtype)
	{
		case dtype_short:
		case dtype_long:
		case dtype_int64:
			{
				SINT64 scale = 1;

				fb_assert(impure->vlu_desc.dsc_scale <= 0);
				for (int i = -impure->vlu_desc.dsc_scale; i > 0; --i)
					scale *= 10;

				const SINT64 v1 = MOV_get_int64(&impure->vlu_desc, impure->vlu_desc.dsc_scale);
				const SINT64 v2 = MOV_get_int64(&impure->vlu_desc, 0) * scale;

				impure->vlu_misc.vlu_int64 = v1 / scale;

				if (v1 > 0 && v1 != v2)
					++impure->vlu_misc.vlu_int64;

				impure->vlu_desc.makeInt64(0, &impure->vlu_misc.vlu_int64);
			}
			break;

		case dtype_real:
			impure->vlu_misc.vlu_float = ceil(impure->vlu_misc.vlu_float);
			break;

		default:
			impure->vlu_misc.vlu_double = MOV_get_double(&impure->vlu_desc);
			// fall through

		case dtype_double:
			impure->vlu_misc.vlu_double = ceil(impure->vlu_misc.vlu_double);
			impure->vlu_desc.makeDouble(&impure->vlu_misc.vlu_double);
			break;
	}

	return &impure->vlu_desc;
}


static string showInvalidChar(const UCHAR c)
{
	string str;
	str.printf("%c (%X)", c, c);
	return str;
}


static dsc* evlCharToUuid(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count == 1);

	jrd_req* request = tdbb->getRequest();

	request->req_flags &= ~req_null;
	const dsc* value = EVL_expr(tdbb, args->nod_arg[0]);
	if (request->req_flags & req_null)	// return NULL if value is NULL
		return NULL;

	if (!value->isText())
		status_exception::raise(Arg::Gds(isc_expression_eval_err) <<
									Arg::Gds(isc_sysf_argviolates_uuidtype) << Arg::Str(function->name));

	USHORT ttype;
	UCHAR* data_temp;
	const USHORT len = CVT_get_string_ptr(value, &ttype, &data_temp, NULL, 0, ERR_post);
	const UCHAR* data = data_temp;

	// validate the UUID
	if (len != GUID_BODY_SIZE) // 36
	{
		status_exception::raise(Arg::Gds(isc_expression_eval_err) <<
									Arg::Gds(isc_sysf_argviolates_uuidlen) <<
										Arg::Num(GUID_BODY_SIZE) <<
										Arg::Str(function->name));
	}

	for (int i = 0; i < GUID_BODY_SIZE; ++i)
	{
		if (i == 8 || i == 13 || i == 18 || i == 23)
		{
			if (data[i] != '-')
			{
				status_exception::raise(Arg::Gds(isc_expression_eval_err) <<
											Arg::Gds(isc_sysf_argviolates_uuidfmt) <<
												Arg::Str(showInvalidChar(data[i])) <<
												Arg::Num(i) <<
												Arg::Str(function->name));
			}
		}
		else
		{
			const UCHAR c = data[i];
			const UCHAR hex = UPPER7(c);

			if (!((hex >= 'A' && hex <= 'F') || (c >= '0' && c <= '9')))
			{
				status_exception::raise(Arg::Gds(isc_expression_eval_err) <<
											Arg::Gds(isc_sysf_argviolates_guidigits) <<
												Arg::Str(showInvalidChar(c)) <<
												Arg::Num(i) <<
												Arg::Str(function->name));
			}
		}
	}

	// convert to binary representation
	char buffer[GUID_BUFF_SIZE];
	buffer[0] = '{';
	buffer[37] = '}';
	buffer[38] = '\0';
	memcpy(buffer + 1, data, GUID_BODY_SIZE);

	FB_GUID guid;
	StringToGuid(&guid, buffer);

	dsc result;
	result.makeText(16, ttype_binary, reinterpret_cast<UCHAR*>(guid.data));
	EVL_make_value(tdbb, &result, impure);

	return &impure->vlu_desc;
}


/* As seen in blr.h; keep this array "extractParts" in sync.
#define blr_extract_year		(unsigned char)0
#define blr_extract_month		(unsigned char)1
#define blr_extract_day			(unsigned char)2
#define blr_extract_hour		(unsigned char)3
#define blr_extract_minute		(unsigned char)4
#define blr_extract_second		(unsigned char)5
#define blr_extract_weekday		(unsigned char)6
#define blr_extract_yearday		(unsigned char)7
#define blr_extract_millisecond	(unsigned char)8
#define blr_extract_week		(unsigned char)9
*/

const char* extractParts[10] =
{
	"YEAR", "MONTH", "DAY", "HOUR", "MINUTE", "SECOND", "WEEKDAY", "YEARDAY", "MILLISECOND", "WEEK"
};

static const char* getPartName(int n)
{
	if (n < 0 || n > FB_NELEM(extractParts))
		return "Unknown";

	return extractParts[n];
}


static dsc* evlDateAdd(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count == 3);

	jrd_req* request = tdbb->getRequest();

	request->req_flags &= ~req_null;
	const dsc* quantityDsc = EVL_expr(tdbb, args->nod_arg[0]);
	if (request->req_flags & req_null)	// return NULL if quantityDsc is NULL
		return NULL;

	const dsc* partDsc = EVL_expr(tdbb, args->nod_arg[1]);
	if (request->req_flags & req_null)	// return NULL if partDsc is NULL
		return NULL;

	const dsc* valueDsc = EVL_expr(tdbb, args->nod_arg[2]);
	if (request->req_flags & req_null)	// return NULL if valueDsc is NULL
		return NULL;

	const SLONG part = MOV_get_long(partDsc, 0);

	TimeStamp timestamp;

	switch (valueDsc->dsc_dtype)
	{
		case dtype_sql_time:
			timestamp.value().timestamp_time = *(GDS_TIME*) valueDsc->dsc_address;

			if (part != blr_extract_hour &&
				part != blr_extract_minute &&
				part != blr_extract_second &&
				part != blr_extract_millisecond)
			{
				status_exception::raise(Arg::Gds(isc_expression_eval_err) <<
											Arg::Gds(isc_sysf_invalid_addpart_time) <<
												Arg::Str(function->name));
			}
			break;

		case dtype_sql_date:
			timestamp.value().timestamp_date = *(GDS_DATE*) valueDsc->dsc_address;
			/*
			if (part == blr_extract_hour ||
				part == blr_extract_minute ||
				part == blr_extract_second ||
				part == blr_extract_millisecond)
			{
				status_exception::raise(Arg::Gds(isc_expression_eval_err));
			}
			*/
			break;

		case dtype_timestamp:
			timestamp.value() = *(GDS_TIMESTAMP*) valueDsc->dsc_address;
			break;

		default:
			status_exception::raise(Arg::Gds(isc_expression_eval_err) <<
										Arg::Gds(isc_sysf_invalid_add_datetime) <<
											Arg::Str(function->name));
			break;
	}

	tm times;
	timestamp.decode(&times);

	const SLONG quantity = MOV_get_long(quantityDsc, 0);

	switch (part)
	{
		// TO DO: detect overflow in the following cases.

		case blr_extract_year:
			{
				times.tm_year += quantity;
				timestamp.encode(&times);

				int day = times.tm_mday;
				timestamp.decode(&times);

				if (times.tm_mday != day)
					--timestamp.value().timestamp_date;
			}
			break;

		case blr_extract_month:
			{
				int md[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

				const int y = quantity / 12;
				const int m = quantity % 12;

				const int ld = md[times.tm_mon] - times.tm_mday;
				const int lm = times.tm_mon;
				times.tm_year += y;

				if ((times.tm_mon += m) > 11)
				{
					times.tm_year++;
					times.tm_mon -= 12;
				}
				else if (times.tm_mon < 0)
				{
					times.tm_year--;
					times.tm_mon += 12;
				}

				const int ly = times.tm_year + 1900;

				if (ly % 4 == 0 && ly % 100 != 0 || ly % 400 == 0)
					md[1]++;

				if (y >= 0 && m >= 0 && times.tm_mday > md[lm])
					times.tm_mday = md[times.tm_mon] - ld;

				if (times.tm_mday > md[times.tm_mon])
					times.tm_mday = md[times.tm_mon];
				else if (times.tm_mday < 1)
					times.tm_mday = 1;

				timestamp.encode(&times);
			}
			break;

		case blr_extract_day:
			timestamp.value().timestamp_date += quantity;
			break;

		case blr_extract_week:
			timestamp.value().timestamp_date += quantity * 7;
			break;

		case blr_extract_hour:
			if (valueDsc->dsc_dtype == dtype_sql_date)
				timestamp.value().timestamp_date += quantity / 24;
			else
				add10msec(&timestamp.value(), quantity, 3600 * ISC_TIME_SECONDS_PRECISION);
			break;

		case blr_extract_minute:
			if (valueDsc->dsc_dtype == dtype_sql_date)
				timestamp.value().timestamp_date += quantity / 1440; // 1440 == 24 * 60
			else
				add10msec(&timestamp.value(), quantity, 60 * ISC_TIME_SECONDS_PRECISION);
			break;

		case blr_extract_second:
			if (valueDsc->dsc_dtype == dtype_sql_date)
				timestamp.value().timestamp_date += quantity / oneDay;
			else
				add10msec(&timestamp.value(), quantity, ISC_TIME_SECONDS_PRECISION);
			break;

		case blr_extract_millisecond:
			if (valueDsc->dsc_dtype == dtype_sql_date)
				timestamp.value().timestamp_date += quantity / (oneDay * 1000);
			else
				add10msec(&timestamp.value(), quantity, ISC_TIME_SECONDS_PRECISION / 1000);
			break;

		default:
			status_exception::raise(Arg::Gds(isc_expression_eval_err) <<
										Arg::Gds(isc_sysf_invalid_addpart_dtime) <<
											Arg::Str(getPartName(part)) <<
											Arg::Str(function->name));
			break;
	}

	EVL_make_value(tdbb, valueDsc, impure);

	switch (impure->vlu_desc.dsc_dtype)
	{
		case dtype_sql_time:
			impure->vlu_misc.vlu_sql_time = timestamp.value().timestamp_time;
			break;

		case dtype_sql_date:
			impure->vlu_misc.vlu_sql_date = timestamp.value().timestamp_date;
			break;

		case dtype_timestamp:
			impure->vlu_misc.vlu_timestamp = timestamp.value();
			break;

		default:
			status_exception::raise(Arg::Gds(isc_expression_eval_err) <<
										Arg::Gds(isc_sysf_invalid_add_dtime_rc));
			break;
	}

	return &impure->vlu_desc;
}


static dsc* evlDateDiff(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count == 3);

	jrd_req* request = tdbb->getRequest();

	const dsc* partDsc = EVL_expr(tdbb, args->nod_arg[0]);
	if (request->req_flags & req_null)	// return NULL if partDsc is NULL
		return NULL;

	request->req_flags &= ~req_null;
	const dsc* value1Dsc = EVL_expr(tdbb, args->nod_arg[1]);
	if (request->req_flags & req_null)	// return NULL if value1Dsc is NULL
		return NULL;

	const dsc* value2Dsc = EVL_expr(tdbb, args->nod_arg[2]);
	if (request->req_flags & req_null)	// return NULL if value2Dsc is NULL
		return NULL;

	TimeStamp timestamp1;

	switch (value1Dsc->dsc_dtype)
	{
		case dtype_sql_time:
			timestamp1.value().timestamp_time = *(GDS_TIME *) value1Dsc->dsc_address;
			break;

		case dtype_sql_date:
			timestamp1.value().timestamp_date = *(GDS_DATE *) value1Dsc->dsc_address;
			break;

		case dtype_timestamp:
			timestamp1.value() = *(GDS_TIMESTAMP *) value1Dsc->dsc_address;
			break;

		default:
			status_exception::raise(Arg::Gds(isc_expression_eval_err) <<
										Arg::Gds(isc_sysf_invalid_diff_dtime) <<
											Arg::Str(function->name));
			break;
	}

	TimeStamp timestamp2;

	switch (value2Dsc->dsc_dtype)
	{
		case dtype_sql_time:
			timestamp2.value().timestamp_time = *(GDS_TIME *) value2Dsc->dsc_address;
			break;

		case dtype_sql_date:
			timestamp2.value().timestamp_date = *(GDS_DATE *) value2Dsc->dsc_address;
			break;

		case dtype_timestamp:
			timestamp2.value() = *(GDS_TIMESTAMP *) value2Dsc->dsc_address;
			break;

		default:
			status_exception::raise(Arg::Gds(isc_expression_eval_err) <<
										Arg::Gds(isc_sysf_invalid_diff_dtime) <<
											Arg::Str(function->name));
			break;
	}

	tm times1, times2;
	timestamp1.decode(&times1);
	timestamp2.decode(&times2);

	const SLONG part = MOV_get_long(partDsc, 0);

	switch (part)
	{
		case blr_extract_hour:
			times1.tm_min = 0;
			times2.tm_min = 0;
			// fall through

		case blr_extract_minute:
			times1.tm_sec = 0;
			times2.tm_sec = 0;
			// fall through

		case blr_extract_second:
			timestamp1.encode(&times1);
			timestamp2.encode(&times2);
			break;
	}

	// ASF: throw error if at least one value is "incomplete" from the EXTRACT POV
	switch (part)
	{
		case blr_extract_year:
		case blr_extract_month:
		case blr_extract_day:
		case blr_extract_week:
			if (value1Dsc->dsc_dtype == dtype_sql_time || value2Dsc->dsc_dtype == dtype_sql_time)
				status_exception::raise(Arg::Gds(isc_expression_eval_err) <<
											Arg::Gds(isc_sysf_invalid_timediff) <<
												Arg::Str(function->name));
			break;

		case blr_extract_hour:
		case blr_extract_minute:
		case blr_extract_second:
		case blr_extract_millisecond:
			{
				//if (value1Dsc->dsc_dtype == dtype_sql_date || value2Dsc->dsc_dtype == dtype_sql_date)
				//	status_exception::raise(Arg::Gds(isc_expression_eval_err));

				// ASF: also throw error if one value is TIMESTAMP and the other is TIME
				// CVC: Or if one value is DATE and the other is TIME.
				const int type1 = value1Dsc->dsc_dtype;
				const int type2 = value2Dsc->dsc_dtype;
				if (type1 == dtype_timestamp && type2 == dtype_sql_time ||
					type1 == dtype_sql_time && type2 == dtype_timestamp)
				{
					status_exception::raise(Arg::Gds(isc_expression_eval_err) <<
												Arg::Gds(isc_sysf_invalid_tstamptimediff) <<
													Arg::Str(function->name));
				}
				if (type1 == dtype_sql_date && type2 == dtype_sql_time ||
					type1 == dtype_sql_time && type2 == dtype_sql_date)
				{
					status_exception::raise(Arg::Gds(isc_expression_eval_err) <<
												Arg::Gds(isc_sysf_invalid_datetimediff) <<
													Arg::Str(function->name));
				}
			}
			break;

		default:
			status_exception::raise(Arg::Gds(isc_expression_eval_err) <<
										Arg::Gds(isc_sysf_invalid_diffpart) <<
											Arg::Str(getPartName(part)) <<
											Arg::Str(function->name));
			break;
	}

	SINT64 result;

	switch (part)
	{
		case blr_extract_year:
			result = times2.tm_year - times1.tm_year;
			break;

		case blr_extract_month:
			result = 12 * (times2.tm_year - times1.tm_year);
			result += times2.tm_mon - times1.tm_mon;
			break;

		case blr_extract_day:
			result = timestamp2.value().timestamp_date - timestamp1.value().timestamp_date;
			break;

		case blr_extract_week:
			result = (timestamp2.value().timestamp_date - timestamp1.value().timestamp_date) / 7;
			break;

		// TO DO: detect overflow in the following cases.

		case blr_extract_hour:
			result = SINT64(24) * (timestamp2.value().timestamp_date - timestamp1.value().timestamp_date);
			result += ((SINT64) timestamp2.value().timestamp_time -
				(SINT64) timestamp1.value().timestamp_time) /
				ISC_TIME_SECONDS_PRECISION / 3600;
			break;

		case blr_extract_minute:
			result = SINT64(24) * 60 * (timestamp2.value().timestamp_date - timestamp1.value().timestamp_date);
			result += ((SINT64) timestamp2.value().timestamp_time -
				(SINT64) timestamp1.value().timestamp_time) /
				ISC_TIME_SECONDS_PRECISION / 60;
			break;

		case blr_extract_second:
			result = (SINT64) oneDay *
				(timestamp2.value().timestamp_date - timestamp1.value().timestamp_date);
			result += ((SINT64) timestamp2.value().timestamp_time -
				(SINT64) timestamp1.value().timestamp_time) /
				ISC_TIME_SECONDS_PRECISION;
			break;

		case blr_extract_millisecond:
			result = (SINT64) oneDay *
				(timestamp2.value().timestamp_date - timestamp1.value().timestamp_date) * 1000;
			result += ((SINT64) timestamp2.value().timestamp_time -
				(SINT64) timestamp1.value().timestamp_time) /
				(ISC_TIME_SECONDS_PRECISION / 1000);
			break;

		default:
			status_exception::raise(Arg::Gds(isc_expression_eval_err) <<
										Arg::Gds(isc_sysf_invalid_diffpart) <<
											Arg::Str(getPartName(part)) <<
											Arg::Str(function->name));
			break;
	}

	impure->vlu_misc.vlu_int64 = result;
	impure->vlu_desc.makeInt64(0, &impure->vlu_misc.vlu_int64);

	return &impure->vlu_desc;
}


static dsc* evlExp(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count == 1);

	jrd_req* request = tdbb->getRequest();

	request->req_flags &= ~req_null;
	const dsc* value = EVL_expr(tdbb, args->nod_arg[0]);
	if (request->req_flags & req_null)	// return NULL if value is NULL
		return NULL;

	impure->vlu_misc.vlu_double = exp(MOV_get_double(value));
	impure->vlu_desc.makeDouble(&impure->vlu_misc.vlu_double);

	if (impure->vlu_misc.vlu_double == HUGE_VAL)
		status_exception::raise(Arg::Gds(isc_arith_except) << Arg::Gds(isc_numeric_out_of_range));

	return &impure->vlu_desc;
}


static dsc* evlFloor(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count == 1);

	jrd_req* request = tdbb->getRequest();

	request->req_flags &= ~req_null;
	const dsc* value = EVL_expr(tdbb, args->nod_arg[0]);
	if (request->req_flags & req_null)	// return NULL if value is NULL
		return NULL;

	EVL_make_value(tdbb, value, impure);

	switch (impure->vlu_desc.dsc_dtype)
	{
		case dtype_short:
		case dtype_long:
		case dtype_int64:
			{
				SINT64 scale = 1;

				fb_assert(impure->vlu_desc.dsc_scale <= 0);
				for (int i = -impure->vlu_desc.dsc_scale; i > 0; --i)
					scale *= 10;

				const SINT64 v1 = MOV_get_int64(&impure->vlu_desc, impure->vlu_desc.dsc_scale);
				const SINT64 v2 = MOV_get_int64(&impure->vlu_desc, 0) * scale;

				impure->vlu_misc.vlu_int64 = v1 / scale;

				if (v1 < 0 && v1 != v2)
					--impure->vlu_misc.vlu_int64;

				impure->vlu_desc.makeInt64(0, &impure->vlu_misc.vlu_int64);
			}
			break;

		case dtype_real:
			impure->vlu_misc.vlu_float = floor(impure->vlu_misc.vlu_float);
			break;

		default:
			impure->vlu_misc.vlu_double = MOV_get_double(&impure->vlu_desc);
			// fall through

		case dtype_double:
			impure->vlu_misc.vlu_double = floor(impure->vlu_misc.vlu_double);
			impure->vlu_desc.makeDouble(&impure->vlu_misc.vlu_double);
			break;
	}

	return &impure->vlu_desc;
}


static dsc* evlGenUuid(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count == 0);

	FB_GUID guid;
	fb_assert(sizeof(guid.data) == 16);

	GenerateGuid(&guid);

	dsc result;
	result.makeText(16, ttype_binary, reinterpret_cast<UCHAR*>(guid.data));
	EVL_make_value(tdbb, &result, impure);

	return &impure->vlu_desc;
}


static dsc* evlHash(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count == 1);

	jrd_req* request = tdbb->getRequest();

	request->req_flags &= ~req_null;
	const dsc* value = EVL_expr(tdbb, args->nod_arg[0]);
	if (request->req_flags & req_null)	// return NULL if value is NULL
		return NULL;

	impure->vlu_misc.vlu_int64 = 0;

	UCHAR* address;

	if (value->isBlob())
	{
		UCHAR buffer[BUFFER_LARGE];
		blb* blob = BLB_open(tdbb, tdbb->getRequest()->req_transaction,
			reinterpret_cast<bid*>(value->dsc_address));

		while (!(blob->blb_flags & BLB_eof))
		{
			address = buffer;
			const ULONG length = BLB_get_data(tdbb, blob, address, sizeof(buffer), false);

			for (const UCHAR* end = address + length; address < end; ++address)
			{
				impure->vlu_misc.vlu_int64 = (impure->vlu_misc.vlu_int64 << 4) + *address;

				const SINT64 n = impure->vlu_misc.vlu_int64 & CONST64(0xF000000000000000);
				if (n)
					impure->vlu_misc.vlu_int64 ^= n >> 56;
				impure->vlu_misc.vlu_int64 &= ~n;
			}
		}

		BLB_close(tdbb, blob);
	}
	else
	{
		MoveBuffer buffer;
		const ULONG length = MOV_make_string2(tdbb, value, value->getTextType(), &address, buffer, false);

		for (const UCHAR* end = address + length; address < end; ++address)
		{
			impure->vlu_misc.vlu_int64 = (impure->vlu_misc.vlu_int64 << 4) + *address;

			const SINT64 n = impure->vlu_misc.vlu_int64 & CONST64(0xF000000000000000);
			if (n)
				impure->vlu_misc.vlu_int64 ^= n >> 56;
			impure->vlu_misc.vlu_int64 &= ~n;
		}
	}

	// make descriptor for return value
	impure->vlu_desc.makeInt64(0, &impure->vlu_misc.vlu_int64);

	return &impure->vlu_desc;
}


static dsc* evlLeft(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count == 2);

	jrd_req* request = tdbb->getRequest();

	request->req_flags &= ~req_null;
	dsc* str = EVL_expr(tdbb, args->nod_arg[0]);
	if (request->req_flags & req_null)	// return NULL if str is NULL
		return NULL;

	const dsc* len = EVL_expr(tdbb, args->nod_arg[1]);
	if (request->req_flags & req_null)	// return NULL if len is NULL
		return NULL;

	SLONG start = 0;
	dsc startDsc;
	startDsc.makeLong(0, &start);

	return SysFunction::substring(tdbb, impure, str, &startDsc, len);
}


static dsc* evlLn(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count == 1);

	jrd_req* request = tdbb->getRequest();

	request->req_flags &= ~req_null;
	const dsc* value = EVL_expr(tdbb, args->nod_arg[0]);
	if (request->req_flags & req_null)	// return NULL if value is NULL
		return NULL;

	const double v = MOV_get_double(value);

	if (v <= 0)
	{
		status_exception::raise(Arg::Gds(isc_expression_eval_err) <<
									Arg::Gds(isc_sysf_argmustbe_positive) <<
										Arg::Str(function->name));
	}

	impure->vlu_misc.vlu_double = log(v);
	impure->vlu_desc.makeDouble(&impure->vlu_misc.vlu_double);

	return &impure->vlu_desc;
}


static dsc* evlLog(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count == 2);

	jrd_req* request = tdbb->getRequest();

	request->req_flags &= ~req_null;
	const dsc* value1 = EVL_expr(tdbb, args->nod_arg[0]);
	if (request->req_flags & req_null)	// return NULL if value1 is NULL
		return NULL;

	const dsc* value2 = EVL_expr(tdbb, args->nod_arg[1]);
	if (request->req_flags & req_null)	// return NULL if value2 is NULL
		return NULL;

	const double v1 = MOV_get_double(value1);
	const double v2 = MOV_get_double(value2);

	if (v1 <= 0)
	{
		status_exception::raise(Arg::Gds(isc_expression_eval_err) <<
									Arg::Gds(isc_sysf_basemustbe_positive) <<
										Arg::Str(function->name));
	}

	if (v2 <= 0)
	{
		status_exception::raise(Arg::Gds(isc_expression_eval_err) <<
									Arg::Gds(isc_sysf_argmustbe_positive) <<
										Arg::Str(function->name));
	}

	impure->vlu_misc.vlu_double = log(v2) / log(v1);
	impure->vlu_desc.makeDouble(&impure->vlu_misc.vlu_double);

	return &impure->vlu_desc;
}


static dsc* evlMaxMinValue(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count >= 1);

	jrd_req* request = tdbb->getRequest();
	dsc* result = NULL;

	for (int i = 0; i < args->nod_count; ++i)
	{
		request->req_flags &= ~req_null;
		dsc* value = EVL_expr(tdbb, args->nod_arg[i]);
		if (request->req_flags & req_null)	// return NULL if value is NULL
			return NULL;

		if (i == 0)
			result = value;
		else
		{
			switch ((Function)(IPTR) function->misc)
			{
				case funMaxValue:
					if (MOV_compare(value, result) > 0)
						result = value;
					break;

				case funMinValue:
					if (MOV_compare(value, result) < 0)
						result = value;
					break;

				default:
					fb_assert(false);
			}
		}
	}

	return result;
}


static dsc* evlMod(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count == 2);

	jrd_req* request = tdbb->getRequest();

	request->req_flags &= ~req_null;
	const dsc* value1 = EVL_expr(tdbb, args->nod_arg[0]);
	if (request->req_flags & req_null)	// return NULL if value1 is NULL
		return NULL;

	const dsc* value2 = EVL_expr(tdbb, args->nod_arg[1]);
	if (request->req_flags & req_null)	// return NULL if value2 is NULL
		return NULL;

	EVL_make_value(tdbb, value1, impure);
	impure->vlu_desc.dsc_scale = 0;

	const SINT64 divisor = MOV_get_int64(value2, 0);

	if (divisor == 0)
		status_exception::raise(Arg::Gds(isc_arith_except) << Arg::Gds(isc_exception_integer_divide_by_zero));

	const SINT64 result = MOV_get_int64(value1, 0) % divisor;

	switch (impure->vlu_desc.dsc_dtype)
	{
		case dtype_short:
			impure->vlu_misc.vlu_short = (SSHORT) result;
			break;

		case dtype_long:
			impure->vlu_misc.vlu_long = (SLONG) result;
			break;

		case dtype_int64:
			impure->vlu_misc.vlu_int64 = result;
			break;

		default:
			impure->vlu_misc.vlu_int64 = result;
			impure->vlu_desc.makeInt64(0, &impure->vlu_misc.vlu_int64);
			break;
	}

	return &impure->vlu_desc;
}


static dsc* evlOverlay(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count >= 3);

	jrd_req* request = tdbb->getRequest();

	request->req_flags &= ~req_null;
	const dsc* value = EVL_expr(tdbb, args->nod_arg[0]);
	if (request->req_flags & req_null)	// return NULL if value is NULL
		return NULL;

	const dsc* placing = EVL_expr(tdbb, args->nod_arg[1]);
	if (request->req_flags & req_null)	// return NULL if placing is NULL
		return NULL;

	const dsc* fromDsc = EVL_expr(tdbb, args->nod_arg[2]);
	if (request->req_flags & req_null)	// return NULL if fromDsc is NULL
		return NULL;

	const dsc* lengthDsc = NULL;
	ULONG length;

	if (args->nod_count >= 4)
	{
		lengthDsc = EVL_expr(tdbb, args->nod_arg[3]);
		if (request->req_flags & req_null)	// return NULL if lengthDsc is NULL
			return NULL;

		const SLONG auxlen = MOV_get_long(lengthDsc, 0);

		if (auxlen < 0)
		{
			status_exception::raise(Arg::Gds(isc_expression_eval_err) <<
										Arg::Gds(isc_sysf_argnmustbe_nonneg) <<
											Arg::Num(4) <<
											Arg::Str(function->name));
		}

		length = auxlen;
	}

	SLONG from = MOV_get_long(fromDsc, 0);

	if (from <= 0)
	{
		status_exception::raise(Arg::Gds(isc_expression_eval_err) <<
									Arg::Gds(isc_sysf_argnmustbe_positive) <<
										Arg::Num(3) <<
										Arg::Str(function->name));
	}

	const USHORT resultTextType = DataTypeUtil::getResultTextType(value, placing);
	CharSet* cs = INTL_charset_lookup(tdbb, resultTextType);

	MoveBuffer temp1;
	UCHAR* str1;
	ULONG len1;

	if (value->isBlob())
	{
		Firebird::UCharBuffer bpb;
		BLB_gen_bpb_from_descs(value, &impure->vlu_desc, bpb);

		blb* blob = BLB_open2(tdbb, tdbb->getRequest()->req_transaction,
			reinterpret_cast<bid*>(value->dsc_address), bpb.getCount(), bpb.begin());
		len1 =
			(blob->blb_length / INTL_charset_lookup(tdbb, value->getCharSet())->minBytesPerChar()) *
			cs->maxBytesPerChar();

		str1 = temp1.getBuffer(len1);
		len1 = BLB_get_data(tdbb, blob, str1, len1, true);
	}
	else
		len1 = MOV_make_string2(tdbb, value, resultTextType, &str1, temp1);

	MoveBuffer temp2;
	UCHAR* str2;
	ULONG len2;

	if (placing->isBlob())
	{
		Firebird::UCharBuffer bpb;
		BLB_gen_bpb_from_descs(placing, &impure->vlu_desc, bpb);

		blb* blob = BLB_open2(tdbb, tdbb->getRequest()->req_transaction,
			reinterpret_cast<bid*>(placing->dsc_address), bpb.getCount(), bpb.begin());
		len2 =
			(blob->blb_length / INTL_charset_lookup(tdbb, placing->getCharSet())->minBytesPerChar()) *
			cs->maxBytesPerChar();

		str2 = temp2.getBuffer(len2);
		len2 = BLB_get_data(tdbb, blob, str2, len2, true);
	}
	else
		len2 = MOV_make_string2(tdbb, placing, resultTextType, &str2, temp2);

	from = MIN((ULONG) from, len1 + 1);

	if (lengthDsc == NULL)	// not specified
	{
		if (cs->isMultiByte())
			length = cs->length(len2, str2, true);
		else
			length = len2 / cs->maxBytesPerChar();
	}

	length = MIN(length, len1 - from + 1);

	blb* newBlob = NULL;

	if (!value->isBlob() && !placing->isBlob())
	{
		const SINT64 newlen = (SINT64) len1 - length + len2;
		if (newlen > static_cast<SINT64>(MAX_COLUMN_SIZE - sizeof(USHORT)))
			status_exception::raise(Arg::Gds(isc_arith_except) << Arg::Gds(isc_imp_exc));

		dsc desc;
		desc.makeText(newlen, resultTextType);
		EVL_make_value(tdbb, &desc, impure);
	}
	else
	{
		EVL_make_value(tdbb, (value->isBlob() ? value : placing), impure);
		impure->vlu_desc.setBlobSubType(DataTypeUtil::getResultBlobSubType(value, placing));
		impure->vlu_desc.setTextType(resultTextType);
		newBlob = BLB_create(tdbb, tdbb->getRequest()->req_transaction, &impure->vlu_misc.vlu_bid);
	}

	HalfStaticArray<UCHAR, BUFFER_LARGE> blobBuffer;
	int l1;

	if (newBlob)
	{
		l1 = (from - 1) * cs->maxBytesPerChar();

		if (!cs->isMultiByte())
			BLB_put_data(tdbb, newBlob, str1, l1);
		else
		{
			l1 = cs->substring(len1, str1, l1, blobBuffer.getBuffer(l1), 0, from - 1);

			BLB_put_data(tdbb, newBlob, blobBuffer.begin(), l1);
		}
	}
	else
	{
		l1 = cs->substring(len1, str1, impure->vlu_desc.dsc_length,
			impure->vlu_desc.dsc_address, 0, from - 1);
	}

	int l2;

	if (newBlob)
	{
		BLB_put_data(tdbb, newBlob, str2, len2);

		const ULONG auxlen = len1 - l1;
		if (!cs->isMultiByte())
		{
			BLB_put_data(tdbb, newBlob, str1 + l1 + length * cs->maxBytesPerChar(),
				auxlen - length * cs->maxBytesPerChar());
		}
		else
		{
			l2 = cs->substring(auxlen, str1 + l1, auxlen,
				blobBuffer.getBuffer(auxlen), length, auxlen);
			BLB_put_data(tdbb, newBlob, blobBuffer.begin(), l2);
		}

		BLB_close(tdbb, newBlob);
	}
	else
	{
		memcpy(impure->vlu_desc.dsc_address + l1, str2, len2);
		l2 = cs->substring(len1 - l1, str1 + l1, impure->vlu_desc.dsc_length - len2,
			impure->vlu_desc.dsc_address + l1 + len2, length, len1 - l1);

		impure->vlu_desc.dsc_length = (USHORT) (l1 + len2 + l2);
	}

	return &impure->vlu_desc;
}


static dsc* evlPad(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count >= 2);

	jrd_req* request = tdbb->getRequest();

	request->req_flags &= ~req_null;
	const dsc* value1 = EVL_expr(tdbb, args->nod_arg[0]);
	if (request->req_flags & req_null)	// return NULL if value1 is NULL
		return NULL;

	const dsc* padLenDsc = EVL_expr(tdbb, args->nod_arg[1]);
	if (request->req_flags & req_null)	// return NULL if padLenDsc is NULL
		return NULL;

	const SLONG padLenArg = MOV_get_long(padLenDsc, 0);
	if (padLenArg < 0)
		status_exception::raise(Arg::Gds(isc_expression_eval_err) <<
									Arg::Gds(isc_sysf_argnmustbe_nonneg) <<
										Arg::Num(2) <<
										Arg::Str(function->name));


	ULONG padLen = static_cast<ULONG>(padLenArg);

	const dsc* value2 = NULL;
	if (args->nod_count >= 3)
	{
		value2 = EVL_expr(tdbb, args->nod_arg[2]);
		if (request->req_flags & req_null)	// return NULL if value2 is NULL
			return NULL;
	}

	const USHORT ttype = value1->getTextType();
	CharSet* cs = INTL_charset_lookup(tdbb, ttype);

	MoveBuffer buffer1;
	UCHAR* address1;
	ULONG length1 = MOV_make_string2(tdbb, value1, ttype, &address1, buffer1, false);
	ULONG charLength1 = cs->length(length1, address1, true);

	MoveBuffer buffer2;
	const UCHAR* address2;
	ULONG length2;

	if (value2 == NULL)
	{
		address2 = const_cast<UCHAR*>(cs->getSpace());
		length2 = cs->getSpaceLength();
	}
	else
	{
		UCHAR* address2_temp = NULL;
		length2 = MOV_make_string2(tdbb, value2, ttype, &address2_temp, buffer2, false);
		address2 = address2_temp;
	}

	ULONG charLength2 = cs->length(length2, address2, true);

	blb* newBlob = NULL;

	if (value1->isBlob() || (value2 && value2->isBlob()))
	{
		EVL_make_value(tdbb, (value1->isBlob() ? value1 : value2), impure);
		impure->vlu_desc.setBlobSubType(value1->getBlobSubType());
		impure->vlu_desc.setTextType(ttype);
		newBlob = BLB_create(tdbb, tdbb->getRequest()->req_transaction, &impure->vlu_misc.vlu_bid);
	}
	else
	{
		if (padLen * cs->maxBytesPerChar() > MAX_COLUMN_SIZE - sizeof(USHORT))
			status_exception::raise(Arg::Gds(isc_arith_except) << Arg::Gds(isc_imp_exc));

		dsc desc;
		desc.makeText(padLen * cs->maxBytesPerChar(), ttype);
		EVL_make_value(tdbb, &desc, impure);
	}

	MoveBuffer buffer;

	if (charLength1 > padLen)
	{
		if (newBlob)
		{
			buffer.getBuffer(padLen * cs->maxBytesPerChar());
			length1 = cs->substring(length1, address1, buffer.getCapacity(),
				buffer.begin(), 0, padLen);
		}
		else
		{
			length1 = cs->substring(length1, address1, impure->vlu_desc.dsc_length,
				impure->vlu_desc.dsc_address, 0, padLen);
		}
		charLength1 = padLen;
	}

	padLen -= charLength1;

	UCHAR* p = impure->vlu_desc.dsc_address;

	if ((Function)(IPTR) function->misc == funRPad)
	{
		if (newBlob)
			BLB_put_data(tdbb, newBlob, address1, length1);
		else
		{
			memcpy(p, address1, length1);
			p += length1;
		}
	}

	for (; charLength2 > 0 && padLen > 0; padLen -= charLength2)
	{
		if (charLength2 <= padLen)
		{
			if (newBlob)
				BLB_put_data(tdbb, newBlob, address2, length2);
			else
			{
				memcpy(p, address2, length2);
				p += length2;
			}
		}
		else
		{
			if (newBlob)
			{
				buffer.getBuffer(padLen * cs->maxBytesPerChar());
				SLONG len = cs->substring(length2, address2, buffer.getCapacity(),
					buffer.begin(), 0, padLen);
				BLB_put_data(tdbb, newBlob, address2, len);
			}
			else
			{
				p += cs->substring(length2, address2,
					impure->vlu_desc.dsc_length - (p - impure->vlu_desc.dsc_address), p,
					0, padLen);
			}

			charLength2 = padLen;
		}
	}

	if ((Function)(IPTR) function->misc == funLPad)
	{
		if (newBlob)
			BLB_put_data(tdbb, newBlob, address1, length1);
		else
		{
			memcpy(p, address1, length1);
			p += length1;
		}
	}

	if (newBlob)
		BLB_close(tdbb, newBlob);
	else
		impure->vlu_desc.dsc_length = p - impure->vlu_desc.dsc_address;

	return &impure->vlu_desc;
}


static dsc* evlPi(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count == 0);

	impure->vlu_misc.vlu_double = 3.14159265358979323846;
	impure->vlu_desc.makeDouble(&impure->vlu_misc.vlu_double);

	return &impure->vlu_desc;
}


static dsc* evlPosition(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count >= 2);

	jrd_req* request = tdbb->getRequest();

	request->req_flags &= ~req_null;
	const dsc* value1 = EVL_expr(tdbb, args->nod_arg[0]);
	if (request->req_flags & req_null)	// return NULL if value1 is NULL
		return NULL;

	const dsc* value2 = EVL_expr(tdbb, args->nod_arg[1]);
	if (request->req_flags & req_null)	// return NULL if value2 is NULL
		return NULL;

	SLONG start = 1;

	if (args->nod_count >= 3)
	{
		const dsc* value3 = EVL_expr(tdbb, args->nod_arg[2]);
		if (request->req_flags & req_null)	// return NULL if value3 is NULL
			return NULL;

		start = MOV_get_long(value3, 0);
		if (start <= 0)
			status_exception::raise(Arg::Gds(isc_expression_eval_err) <<
										Arg::Gds(isc_sysf_argnmustbe_positive) <<
											Arg::Num(3) <<
											Arg::Str(function->name));

	}

	// make descriptor for return value
	impure->vlu_desc.makeLong(0, &impure->vlu_misc.vlu_long);

	// we'll use the collation from the second string
	const USHORT ttype = value2->getTextType();
	TextType* tt = INTL_texttype_lookup(tdbb, ttype);
	CharSet* cs = tt->getCharSet();
	const UCHAR canonicalWidth = tt->getCanonicalWidth();

	MoveBuffer value1Buffer;
	UCHAR* value1Address;
	ULONG value1Length;

	if (value1->isBlob())
	{
		// value1 is a blob
		blb* blob = BLB_open(tdbb, tdbb->getRequest()->req_transaction,
			reinterpret_cast<bid*>(value1->dsc_address));

		value1Address = value1Buffer.getBuffer(blob->blb_length);
		value1Length = BLB_get_data(tdbb, blob, value1Address, blob->blb_length, true);
	}
	else
		value1Length = MOV_make_string2(tdbb, value1, ttype, &value1Address, value1Buffer);

	Firebird::HalfStaticArray<UCHAR, BUFFER_SMALL> value1Canonical;
	value1Canonical.getBuffer(value1Length / cs->minBytesPerChar() * canonicalWidth);
	const SLONG value1CanonicalLen = tt->canonical(value1Length, value1Address,
		value1Canonical.getCount(), value1Canonical.begin()) * canonicalWidth;

	// if the first string is empty, we should return 1 accordingly to SQL2003 standard
	if (value1CanonicalLen == 0)
	{
		impure->vlu_misc.vlu_long = 1;
		return &impure->vlu_desc;
	}

	MoveBuffer value2Buffer;
	UCHAR* value2Address;
	ULONG value2Length;

	if (value2->isBlob())
	{
		// value2 is a blob
		blb* blob = BLB_open(tdbb, tdbb->getRequest()->req_transaction,
			reinterpret_cast<bid*>(value2->dsc_address));

		value2Address = value2Buffer.getBuffer(blob->blb_length);
		value2Length = BLB_get_data(tdbb, blob, value2Address, blob->blb_length, true);
	}
	else
		value2Length = MOV_make_string2(tdbb, value2, ttype, &value2Address, value2Buffer);

	Firebird::HalfStaticArray<UCHAR, BUFFER_SMALL> value2Canonical;
	value2Canonical.getBuffer(value2Length / cs->minBytesPerChar() * canonicalWidth);
	const SLONG value2CanonicalLen = tt->canonical(value2Length, value2Address,
		value2Canonical.getCount(), value2Canonical.begin()) * canonicalWidth;

	// if the second string is empty, first one is not inside it
	if (value2CanonicalLen == 0)
	{
		impure->vlu_misc.vlu_long = 0;
		return &impure->vlu_desc;
	}

	// search if value1 is inside value2
	const UCHAR* const end = value2Canonical.begin() + value2CanonicalLen;

	for (const UCHAR* p = value2Canonical.begin() + (start - 1) * canonicalWidth;
		 p + value1CanonicalLen <= end;
		 p += canonicalWidth)
	{
		if (memcmp(p, value1Canonical.begin(), value1CanonicalLen) == 0)
		{
			impure->vlu_misc.vlu_long = ((p - value2Canonical.begin()) / canonicalWidth) + 1;
			return &impure->vlu_desc;
		}
	}

	// value1 isn't inside value2
	impure->vlu_misc.vlu_long = 0;
	return &impure->vlu_desc;
}


static dsc* evlPower(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count == 2);

	jrd_req* request = tdbb->getRequest();

	request->req_flags &= ~req_null;
	const dsc* value1 = EVL_expr(tdbb, args->nod_arg[0]);
	if (request->req_flags & req_null)	// return NULL if value1 is NULL
		return NULL;

	const dsc* value2 = EVL_expr(tdbb, args->nod_arg[1]);
	if (request->req_flags & req_null)	// return NULL if value2 is NULL
		return NULL;

	impure->vlu_desc.makeDouble(&impure->vlu_misc.vlu_double);

	const double v1 = MOV_get_double(value1);
	const double v2 = MOV_get_double(value2);

	if (v1 == 0 && v2 < 0)
		status_exception::raise(Arg::Gds(isc_expression_eval_err) <<
									Arg::Gds(isc_sysf_invalid_zeropowneg) <<
										Arg::Str(function->name));

	if (v1 < 0 && !(value2->isExact() && value2->dsc_scale == 0))
		status_exception::raise(Arg::Gds(isc_expression_eval_err) <<
									Arg::Gds(isc_sysf_invalid_negpowfp) <<
										Arg::Str(function->name));

	impure->vlu_misc.vlu_double = pow(v1, v2);

	return &impure->vlu_desc;
}


static dsc* evlRand(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count == 0);

	SINT64 n;
	tdbb->getAttachment()->att_random_generator.getBytes(&n, sizeof(n));
	n &= QUADCONST(0x7FFFFFFFFFFFFFFF);	// remove the sign

	impure->vlu_misc.vlu_double = (double) n / MAX_SINT64;
	impure->vlu_desc.makeDouble(&impure->vlu_misc.vlu_double);

	return &impure->vlu_desc;
}


static dsc* evlReplace(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count == 3);

	jrd_req* request = tdbb->getRequest();
	dsc* values[3];	// 0 = searched, 1 = find, 2 = replacement
	const dsc* firstBlob = NULL;
	int i;

	for (i = 0; i < 3; ++i)
	{
		request->req_flags &= ~req_null;
		values[i] = EVL_expr(tdbb, args->nod_arg[i]);
		if (request->req_flags & req_null)	// return NULL if values[i] is NULL
			return NULL;

		if (!firstBlob && values[i]->isBlob())
			firstBlob = values[i];
	}

	const USHORT ttype = values[0]->getTextType();
	TextType* tt = INTL_texttype_lookup(tdbb, ttype);
	CharSet* cs = tt->getCharSet();
	const UCHAR canonicalWidth = tt->getCanonicalWidth();

	MoveBuffer buffers[3];
	UCHAR* addresses[3];
	ULONG lengths[3];
	Firebird::HalfStaticArray<UCHAR, BUFFER_SMALL> canonicals[2];	// searched, find

	for (i = 0; i < 3; ++i)
	{
		if (values[i]->isBlob())
		{
			// values[i] is a blob
			blb* blob = BLB_open(tdbb, tdbb->getRequest()->req_transaction,
				reinterpret_cast<bid*>(values[i]->dsc_address));

			addresses[i] = buffers[i].getBuffer(blob->blb_length);
			lengths[i] = BLB_get_data(tdbb, blob, addresses[i], blob->blb_length, true);
		}
		else
			lengths[i] = MOV_make_string2(tdbb, values[i], ttype, &addresses[i], buffers[i]);
	}

	if (lengths[1] == 0)
		return values[0];

	for (i = 0; i < 2; ++i)
	{
		canonicals[i].getBuffer(lengths[i] / cs->minBytesPerChar() * canonicalWidth);
		canonicals[i].resize(tt->canonical(lengths[i], addresses[i],
			canonicals[i].getCount(), canonicals[i].begin()) * canonicalWidth);
	}

	blb* newBlob = NULL;

	// make descriptor for return value
	if (!firstBlob)
	{
		const unsigned int searchedLen = canonicals[0].getCount() / canonicalWidth;
		const unsigned int findLen = canonicals[1].getCount() / canonicalWidth;
		const unsigned int replacementLen = lengths[2] / cs->minBytesPerChar();

		const USHORT len = MIN(MAX_COLUMN_SIZE, cs->maxBytesPerChar() *
			MAX(searchedLen, searchedLen +
					(searchedLen / findLen) * (replacementLen - findLen)));

		dsc desc;
		desc.makeText(len, ttype);
		EVL_make_value(tdbb, &desc, impure);
	}
	else
	{
		EVL_make_value(tdbb, firstBlob, impure);
		impure->vlu_desc.setBlobSubType(values[0]->getBlobSubType());
		impure->vlu_desc.setTextType(ttype);
		newBlob = BLB_create(tdbb, tdbb->getRequest()->req_transaction, &impure->vlu_misc.vlu_bid);
	}

	// search 'find' in 'searched'
	bool finished = false;
	const UCHAR* const end = canonicals[0].begin() + canonicals[0].getCount();
	const UCHAR* srcPos = addresses[0];
	UCHAR* dstPos = (newBlob ? NULL : impure->vlu_desc.dsc_address);
	MoveBuffer buffer;
	const UCHAR* last;

	for (const UCHAR* p = last = canonicals[0].begin();
		 !finished || (p + canonicals[1].getCount() <= end);
		 p += canonicalWidth)
	{
		if (p + canonicals[1].getCount() > end)
		{
			finished = true;
			p = canonicals[0].end();
		}

		if (finished || memcmp(p, canonicals[1].begin(), canonicals[1].getCount()) == 0)
		{
			int len;

			if (newBlob)
			{
				len = ((p - last) / canonicalWidth) * cs->maxBytesPerChar();

				if (cs->isMultiByte())
				{
					buffer.getBuffer(len);
					len = cs->substring(addresses[0] + lengths[0] - srcPos, srcPos,
						buffer.getCapacity(), buffer.begin(), 0, (p - last) / canonicalWidth);

					BLB_put_data(tdbb, newBlob, buffer.begin(), len);
				}
				else
					BLB_put_data(tdbb, newBlob, srcPos, len);

				if (!finished)
					BLB_put_data(tdbb, newBlob, addresses[2], lengths[2]);
			}
			else
			{
				len = cs->substring(addresses[0] + lengths[0] - srcPos, srcPos,
					(impure->vlu_desc.dsc_address + impure->vlu_desc.dsc_length) - dstPos, dstPos,
					0, (p - last) / canonicalWidth);

				dstPos += len;

				if (!finished)
				{
					memcpy(dstPos, addresses[2], lengths[2]);
					dstPos += lengths[2];
				}
			}

			srcPos += len + lengths[1];

			last = p + canonicals[1].getCount();
			p += canonicals[1].getCount() - canonicalWidth;
		}
	}

	if (newBlob)
		BLB_close(tdbb, newBlob);
	else
		impure->vlu_desc.dsc_length = dstPos - impure->vlu_desc.dsc_address;

	return &impure->vlu_desc;
}


static dsc* evlReverse(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count == 1);

	jrd_req* request = tdbb->getRequest();

	request->req_flags &= ~req_null;
	dsc* value = EVL_expr(tdbb, args->nod_arg[0]);
	if (request->req_flags & req_null)	// return NULL if value is NULL
		return NULL;

	CharSet* cs = INTL_charset_lookup(tdbb, value->getCharSet());

	if (value->isBlob())
	{
		blb* blob = BLB_open(tdbb, tdbb->getRequest()->req_transaction,
			reinterpret_cast<bid*>(value->dsc_address));

		Firebird::HalfStaticArray<UCHAR, BUFFER_LARGE> buffer;
		Firebird::HalfStaticArray<UCHAR, BUFFER_LARGE> buffer2;

		UCHAR* p = buffer.getBuffer(blob->blb_length);
		const SLONG len = BLB_get_data(tdbb, blob, p, blob->blb_length, true);

		if (cs->isMultiByte() || cs->minBytesPerChar() > 1)
		{
			const UCHAR* p1 = p;
			UCHAR* p2 = buffer2.getBuffer(len) + len;
			const UCHAR* const end = p1 + len;
			ULONG size = 0;

			while (p2 > buffer2.begin())
			{
				const bool read = IntlUtil::readOneChar(cs, &p1, end, &size);
				fb_assert(read == true);
				memcpy(p2 -= size, p1, size);
			}

			p = p2;
		}
		else
		{
			for (UCHAR* p2 = p + len - 1; p2 >= p; p++, p2--)
			{
				const UCHAR c = *p;
				*p = *p2;
				*p2 = c;
			}

			p = buffer.begin();
		}

		EVL_make_value(tdbb, value, impure);

		blb* newBlob = BLB_create(tdbb, tdbb->getRequest()->req_transaction,
			&impure->vlu_misc.vlu_bid);
		BLB_put_data(tdbb, newBlob, p, len);
		BLB_close(tdbb, newBlob);
	}
	else
	{
		MoveBuffer temp;
		UCHAR* p;
		const int len = MOV_make_string2(tdbb, value, value->getTextType(), &p, temp);

		dsc desc;
		desc.makeText(len, value->getTextType());
		EVL_make_value(tdbb, &desc, impure);

		UCHAR* p2 = impure->vlu_desc.dsc_address + impure->vlu_desc.dsc_length;

		if (cs->isMultiByte() || cs->minBytesPerChar() > 1)
		{
			const UCHAR* p1 = p;
			const UCHAR* const end = p1 + len;
			ULONG size = 0;

			while (p2 > impure->vlu_desc.dsc_address)
			{
				const bool read = IntlUtil::readOneChar(cs, &p1, end, &size);
				fb_assert(read == true);
				memcpy(p2 -= size, p1, size);
			}
		}
		else
		{
			while (p2 > impure->vlu_desc.dsc_address)
				*--p2 = *p++;
		}
	}

	return &impure->vlu_desc;
}


static dsc* evlRight(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count == 2);

	jrd_req* request = tdbb->getRequest();

	request->req_flags &= ~req_null;
	dsc* value = EVL_expr(tdbb, args->nod_arg[0]);
	if (request->req_flags & req_null)	// return NULL if value is NULL
		return NULL;

	dsc* len = EVL_expr(tdbb, args->nod_arg[1]);
	if (request->req_flags & req_null)	// return NULL if len is NULL
		return NULL;

	SLONG start;

	if (value->isBlob())
	{
		CharSet* charSet = INTL_charset_lookup(tdbb, value->getCharSet());
		blb* blob = BLB_open(tdbb, tdbb->getRequest()->req_transaction,
			reinterpret_cast<bid*>(value->dsc_address));

		if (charSet->isMultiByte())
		{
			Firebird::HalfStaticArray<UCHAR, BUFFER_LARGE> buffer;

			start = charSet->length(
				BLB_get_data(tdbb, blob, buffer.getBuffer(blob->blb_length), blob->blb_length, false),
				buffer.begin(), true);
		}
		else
			start = blob->blb_length / charSet->maxBytesPerChar();

		BLB_close(tdbb, blob);
	}
	else
	{
		MoveBuffer temp;
		UCHAR* p;
		start = MOV_make_string2(tdbb, value, value->getTextType(), &p, temp);
	}

	start -= MOV_get_long(len, 0);
	start = MAX(0, start);

	dsc startDsc;
	startDsc.makeLong(0, &start);

	return SysFunction::substring(tdbb, impure, value, &startDsc, len);
}


static dsc* evlRound(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count >= 1);

	jrd_req* request = tdbb->getRequest();

	request->req_flags &= ~req_null;
	const dsc* value = EVL_expr(tdbb, args->nod_arg[0]);
	if (request->req_flags & req_null)	// return NULL if value is NULL
		return NULL;

	SLONG scale = 0;

	if (args->nod_count > 1)
	{
		const dsc* scaleDsc = EVL_expr(tdbb, args->nod_arg[1]);
		if (request->req_flags & req_null)	// return NULL if scaleDsc is NULL
			return NULL;

		scale = -MOV_get_long(scaleDsc, 0);
		if (!(scale >= MIN_SCHAR && scale <= MAX_SCHAR))
			status_exception::raise(Arg::Gds(isc_expression_eval_err) <<
										Arg::Gds(isc_sysf_invalid_scale) <<
											Arg::Str(function->name));
	}

	impure->vlu_misc.vlu_int64 = MOV_get_int64(value, scale);
	impure->vlu_desc.makeInt64(scale, &impure->vlu_misc.vlu_int64);

	return &impure->vlu_desc;
}


static dsc* evlSign(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count == 1);

	jrd_req* request = tdbb->getRequest();

	request->req_flags &= ~req_null;
	const dsc* value = EVL_expr(tdbb, args->nod_arg[0]);
	if (request->req_flags & req_null)	// return NULL if value is NULL
		return NULL;

	const double val = MOV_get_double(value);

	if (val > 0)
		impure->vlu_misc.vlu_short = 1;
	else if (val < 0)
		impure->vlu_misc.vlu_short = -1;
	else	// val == 0
		impure->vlu_misc.vlu_short = 0;

	impure->vlu_desc.makeShort(0, &impure->vlu_misc.vlu_short);

	return &impure->vlu_desc;
}


static dsc* evlSqrt(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count == 1);

	jrd_req* request = tdbb->getRequest();

	request->req_flags &= ~req_null;
	const dsc* value = EVL_expr(tdbb, args->nod_arg[0]);
	if (request->req_flags & req_null)	// return NULL if value is NULL
		return NULL;

	impure->vlu_misc.vlu_double = MOV_get_double(value);

	if (impure->vlu_misc.vlu_double < 0)
		status_exception::raise(Arg::Gds(isc_expression_eval_err) <<
									Arg::Gds(isc_sysf_argmustbe_nonneg) <<
										Arg::Str(function->name));

	impure->vlu_misc.vlu_double = sqrt(impure->vlu_misc.vlu_double);
	impure->vlu_desc.makeDouble(&impure->vlu_misc.vlu_double);

	return &impure->vlu_desc;
}


static dsc* evlTrunc(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count >= 1);

	jrd_req* request = tdbb->getRequest();

	request->req_flags &= ~req_null;
	const dsc* value = EVL_expr(tdbb, args->nod_arg[0]);
	if (request->req_flags & req_null)	// return NULL if value is NULL
		return NULL;

	SLONG resultScale = 0;
	if (args->nod_count > 1)
	{
		const dsc* scaleDsc = EVL_expr(tdbb, args->nod_arg[1]);
		if (request->req_flags & req_null)	// return NULL if scaleDsc is NULL
			return NULL;

		resultScale = -MOV_get_long(scaleDsc, 0);
		if (!(resultScale >= MIN_SCHAR && resultScale <= MAX_SCHAR))
			status_exception::raise(Arg::Gds(isc_expression_eval_err) <<
										Arg::Gds(isc_sysf_invalid_scale) <<
											Arg::Str(function->name));
	}

	if (value->isExact())
	{
		SSHORT scale = value->dsc_scale;
		impure->vlu_misc.vlu_int64 = MOV_get_int64(value, scale);

		if (resultScale < scale)
			resultScale = scale;

		scale -= resultScale;

		if (scale < 0)
		{
			while (scale)
			{
				impure->vlu_misc.vlu_int64 /= 10;
				++scale;
			}
		}

		impure->vlu_desc.makeInt64(resultScale, &impure->vlu_misc.vlu_int64);
	}
	else
	{
		impure->vlu_misc.vlu_double = MOV_get_double(value);

		SINT64 v = 1;

		if (resultScale > 0)
		{
			while (resultScale > 0)
			{
				v *= 10;
				--resultScale;
			}

			impure->vlu_misc.vlu_double /= v;
			modf(impure->vlu_misc.vlu_double, &impure->vlu_misc.vlu_double);
			impure->vlu_misc.vlu_double *= v;
		}
		else
		{
			double r = modf(impure->vlu_misc.vlu_double, &impure->vlu_misc.vlu_double);

			if (resultScale != 0)
			{
				for (SLONG i = 0; i > resultScale; --i)
					v *= 10;

				modf(r * v, &r);
				impure->vlu_misc.vlu_double += r / v;
			}
		}

		impure->vlu_desc.makeDouble(&impure->vlu_misc.vlu_double);
	}

	return &impure->vlu_desc;
}


static dsc* evlUuidToChar(Jrd::thread_db* tdbb, const SysFunction* function, Jrd::jrd_nod* args, Jrd::impure_value* impure)
{
	fb_assert(args->nod_count == 1);

	jrd_req* request = tdbb->getRequest();

	request->req_flags &= ~req_null;
	const dsc* value = EVL_expr(tdbb, args->nod_arg[0]);
	if (request->req_flags & req_null)	// return NULL if value is NULL
		return NULL;

	if (!value->isText())
		status_exception::raise(Arg::Gds(isc_expression_eval_err) <<
									Arg::Gds(isc_sysf_binuuid_mustbe_str) <<
										Arg::Str(function->name));

	USHORT ttype;
	UCHAR* data;
	const USHORT len = CVT_get_string_ptr(value, &ttype, &data, NULL, 0, ERR_post);

	if (len != sizeof(FB_GUID))
		status_exception::raise(Arg::Gds(isc_expression_eval_err) <<
									Arg::Gds(isc_sysf_binuuid_wrongsize) <<
										Arg::Num(sizeof(FB_GUID)) <<
										Arg::Str(function->name));

	char buffer[GUID_BUFF_SIZE];
	GuidToString(buffer, reinterpret_cast<const FB_GUID*>(data));

	dsc result;
	result.makeText(GUID_BODY_SIZE, ttype_ascii, reinterpret_cast<UCHAR*>(buffer) + 1);
	EVL_make_value(tdbb, &result, impure);

	return &impure->vlu_desc;
}


#define SF(a, b, c, d, e, f, g) {a, b, c, d, e, f, g}

#ifdef _MSC_VER
typedef StdMathFunc VoidPtrStdMathFunc;
#else
typedef void* VoidPtrStdMathFunc;
#endif

const SysFunction SysFunction::functions[] =
	{
		SF("ABS", 1, 1, setParamsDouble, makeAbs, evlAbs, NULL),
		SF("ACOS", 1, 1, setParamsDouble, makeDoubleResult, evlStdMath, (VoidPtrStdMathFunc) acos),
		SF("ASCII_CHAR", 1, 1, setParamsInteger, makeAsciiChar, evlAsciiChar, NULL),
		SF("ASCII_VAL", 1, 1, setParamsAsciiVal, makeShortResult, evlAsciiVal, NULL),
		SF("ASIN", 1, 1, setParamsDouble, makeDoubleResult, evlStdMath, (VoidPtrStdMathFunc) asin),
		SF("ATAN", 1, 1, setParamsDouble, makeDoubleResult, evlStdMath, (VoidPtrStdMathFunc) atan),
		SF("ATAN2", 2, 2, setParamsDouble, makeDoubleResult, evlAtan2, NULL),
		SF("BIN_AND", 2, -1, setParamsInteger, makeBin, evlBin, (void*) funBinAnd),
		SF("BIN_NOT", 1, 1, setParamsInteger, makeBin, evlBin, (void*) funBinNot),
		SF("BIN_OR", 2, -1, setParamsInteger, makeBin, evlBin, (void*) funBinOr),
		SF("BIN_SHL", 2, 2, setParamsInteger, makeBinShift, evlBinShift, (void*) funBinShl),
		SF("BIN_SHR", 2, 2, setParamsInteger, makeBinShift, evlBinShift, (void*) funBinShr),
		SF("BIN_XOR", 2, -1, setParamsInteger, makeBin, evlBin, (void*) funBinXor),
		SF("CEIL", 1, 1, setParamsDouble, makeCeilFloor, evlCeil, NULL),
		SF("CEILING", 1, 1, setParamsDouble, makeCeilFloor, evlCeil, NULL),
		SF("CHAR_TO_UUID", 1, 1, setParamsCharToUuid, makeUuid, evlCharToUuid, NULL),
		SF("COS", 1, 1, setParamsDouble, makeDoubleResult, evlStdMath, (VoidPtrStdMathFunc) cos),
		SF("COSH", 1, 1, setParamsDouble, makeDoubleResult, evlStdMath, (VoidPtrStdMathFunc) cosh),
		SF("COT", 1, 1, setParamsDouble, makeDoubleResult, evlStdMath, (VoidPtrStdMathFunc) cot),
		SF("DATEADD", 3, 3, setParamsDateAdd, makeDateAdd, evlDateAdd, NULL),
		SF("DATEDIFF", 3, 3, setParamsDateDiff, makeInt64Result, evlDateDiff, NULL),
		SF("EXP", 1, 1, setParamsDouble, makeDoubleResult, evlExp, NULL),
		SF("FLOOR", 1, 1, setParamsDouble, makeCeilFloor, evlFloor, NULL),
		SF("GEN_UUID", 0, 0, NULL, makeUuid, evlGenUuid, NULL),
		SF("HASH", 1, 1, NULL, makeInt64Result, evlHash, NULL),
		SF("LEFT", 2, 2, setParamsSecondInteger, makeLeftRight, evlLeft, NULL),
		SF("LN", 1, 1, setParamsDouble, makeDoubleResult, evlLn, NULL),
		SF("LOG", 2, 2, setParamsDouble, makeDoubleResult, evlLog, NULL),
		SF("LOG10", 1, 1, setParamsDouble, makeDoubleResult, evlStdMath, (VoidPtrStdMathFunc) log10),
		SF("LPAD", 2, 3, setParamsSecondInteger, makePad, evlPad, (void*) funLPad),
		SF("MAXVALUE", 1, -1, setParamsFromList, makeFromListResult, evlMaxMinValue, (void*) funMaxValue),
		SF("MINVALUE", 1, -1, setParamsFromList, makeFromListResult, evlMaxMinValue, (void*) funMinValue),
		SF("MOD", 2, 2, setParamsFromList, makeMod, evlMod, NULL),
		SF("OVERLAY", 3, 4, setParamsOverlay, makeOverlay, evlOverlay, NULL),
		SF("PI", 0, 0, NULL, makeDoubleResult, evlPi, NULL),
		SF("POSITION", 2, 3, setParamsPosition, makeLongResult, evlPosition, NULL),
		SF("POWER", 2, 2, setParamsDouble, makeDoubleResult, evlPower, NULL),
		SF("RAND", 0, 0, NULL, makeDoubleResult, evlRand, NULL),
		SF("REPLACE", 3, 3, setParamsFromList, makeReplace, evlReplace, NULL),
		SF("REVERSE", 1, 1, NULL, makeReverse, evlReverse, NULL),
		SF("RIGHT", 2, 2, setParamsSecondInteger, makeLeftRight, evlRight, NULL),
		SF("ROUND", 1, 2, setParamsRoundTrunc, makeRound, evlRound, NULL),
		SF("RPAD", 2, 3, setParamsSecondInteger, makePad, evlPad, (void*) funRPad),
		SF("SIGN", 1, 1, setParamsDouble, makeShortResult, evlSign, NULL),
		SF("SIN", 1, 1, setParamsDouble, makeDoubleResult, evlStdMath, (VoidPtrStdMathFunc) sin),
		SF("SINH", 1, 1, setParamsDouble, makeDoubleResult, evlStdMath, (VoidPtrStdMathFunc) sinh),
		SF("SQRT", 1, 1, setParamsDouble, makeDoubleResult, evlSqrt, NULL),
		SF("TAN", 1, 1, setParamsDouble, makeDoubleResult, evlStdMath, (VoidPtrStdMathFunc) tan),
		SF("TANH", 1, 1, setParamsDouble, makeDoubleResult, evlStdMath, (VoidPtrStdMathFunc) tanh),
		SF("TRUNC", 1, 2, setParamsRoundTrunc, makeTrunc, evlTrunc, NULL),
		SF("UUID_TO_CHAR", 1, 1, setParamsUuidToChar, makeUuidToChar, evlUuidToChar, NULL),
		SF("", 0, 0, NULL, NULL, NULL, NULL)
	};


const SysFunction* SysFunction::lookup(const Firebird::MetaName& name)
{
	for (const SysFunction* f = functions; f->name.length() > 0; ++f)
	{
		if (f->name == name)
			return f;
	}

	return NULL;
}


dsc* SysFunction::substring(thread_db* tdbb, impure_value* impure,
	dsc* value, const dsc* offset_value, const dsc* length_value)
{
/**************************************
 *
 *      s u b s t r i n g
 *
 **************************************
 *
 * Functional description
 *      Perform substring function.
 *
 **************************************/
	SET_TDBB(tdbb);

	const SLONG offset_arg = MOV_get_long(offset_value, 0);
	const SLONG length_arg = MOV_get_long(length_value, 0);

	if (offset_arg < 0)
		status_exception::raise(Arg::Gds(isc_bad_substring_offset) << Arg::Num(offset_arg + 1));
	else if (length_arg < 0)
		status_exception::raise(Arg::Gds(isc_bad_substring_length) << Arg::Num(length_arg));

	dsc desc;
	DataTypeUtil(tdbb).makeSubstr(&desc, value, offset_value, length_value);

	ULONG offset = (ULONG) offset_arg;
	ULONG length = (ULONG) length_arg;

	if (desc.isText() && length > MAX_COLUMN_SIZE)
		length = MAX_COLUMN_SIZE;

	ULONG dataLen;

	if (value->isBlob())
	{
		// Source string is a blob, things get interesting.

		fb_assert(desc.dsc_dtype == dtype_blob);

		desc.dsc_address = (UCHAR*) &impure->vlu_misc.vlu_bid;

		blb* newBlob = BLB_create(tdbb, tdbb->getRequest()->req_transaction,
			&impure->vlu_misc.vlu_bid);

		blb* blob = BLB_open(tdbb, tdbb->getRequest()->req_transaction,
							reinterpret_cast<bid*>(value->dsc_address));

		Firebird::HalfStaticArray<UCHAR, BUFFER_LARGE> buffer;
		CharSet* charSet = INTL_charset_lookup(tdbb, value->getCharSet());
		const ULONG totLen = length * charSet->maxBytesPerChar();

		if (charSet->isMultiByte())
		{
			buffer.getBuffer(MIN(blob->blb_length, (offset + length) * charSet->maxBytesPerChar()));
			dataLen = BLB_get_data(tdbb, blob, buffer.begin(), buffer.getCount(), false);

			Firebird::HalfStaticArray<UCHAR, BUFFER_LARGE> buffer2;
			buffer2.getBuffer(dataLen);

			dataLen = charSet->substring(dataLen, buffer.begin(),
				buffer2.getCapacity(), buffer2.begin(), offset, length);
			BLB_put_data(tdbb, newBlob, buffer2.begin(), dataLen);
		}
		else
		{
			offset *= charSet->maxBytesPerChar();
			length *= charSet->maxBytesPerChar();

			while (!(blob->blb_flags & BLB_eof) && offset)
			{
				// Both cases are the same for now. Let's see if we can optimize in the future.
				ULONG l1 = BLB_get_data(tdbb, blob, buffer.begin(),
					MIN(buffer.getCapacity(), offset), false);
				offset -= l1;
			}

			while (!(blob->blb_flags & BLB_eof) && length)
			{
				dataLen = BLB_get_data(tdbb, blob, buffer.begin(),
					MIN(length, buffer.getCapacity()), false);
				length -= dataLen;

				BLB_put_data(tdbb, newBlob, buffer.begin(), dataLen);
			}
		}

		BLB_close(tdbb, blob);
		BLB_close(tdbb, newBlob);

		EVL_make_value(tdbb, &desc, impure);
	}
	else
	{
		fb_assert(desc.isText());

		desc.dsc_dtype = dtype_text;

		// CVC: I didn't bother to define a larger buffer because:
		//		- Native types when converted to string don't reach 31 bytes plus terminator.
		//		- String types do not need and do not use the buffer ("temp") to be pulled.
		//		- The types that can cause an error() issued inside the low level MOV/CVT
		//		routines because the "temp" is not enough are blob and array but at this time
		//		they aren't accepted, so they will cause error() to be called anyway.
		UCHAR temp[32];
		USHORT ttype;
		desc.dsc_length =
			MOV_get_string_ptr(value, &ttype, &desc.dsc_address,
							   reinterpret_cast<vary*>(temp), sizeof(temp));
		desc.setTextType(ttype);

		// CVC: Why bother? If the offset is greater or equal than the length in bytes,
		// it's impossible that the offset be less than the length in an international charset.
		if (offset >= desc.dsc_length || !length)
		{
			desc.dsc_length = 0;
			EVL_make_value(tdbb, &desc, impure);
		}
		// CVC: God save the king if the engine doesn't protect itself against buffer overruns,
		//		because intl.h defines UNICODE as the type of most system relations' string fields.
		//		Also, the field charset can come as 127 (dynamic) when it comes from system triggers,
		//		but it's resolved by INTL_obj_lookup() to UNICODE_FSS in the cases I observed. Here I cannot
		//		distinguish between user calls and system calls. Unlike the original ASCII substring(),
		//		this one will get correctly the amount of UNICODE characters requested.
		else if (ttype == ttype_ascii || ttype == ttype_none || ttype == ttype_binary)
		{
			/* Redundant.
			if (offset >= desc.dsc_length)
				desc.dsc_length = 0;
			else */
			desc.dsc_address += offset;
			desc.dsc_length -= offset;
			if (length < desc.dsc_length)
				desc.dsc_length = length;
			EVL_make_value(tdbb, &desc, impure);
		}
		else
		{
			// CVC: ATTENTION:
			// I couldn't find an appropriate message for this failure among current registered
			// messages, so I will return empty.
			// Finally I decided to use arithmetic exception or numeric overflow.
			const UCHAR* p = desc.dsc_address;
			const USHORT pcount = desc.dsc_length;

			CharSet* charSet = INTL_charset_lookup(tdbb, desc.getCharSet());

			desc.dsc_address = NULL;
			const ULONG totLen = MIN(MAX_COLUMN_SIZE, length * charSet->maxBytesPerChar());
			desc.dsc_length = totLen;
			EVL_make_value(tdbb, &desc, impure);

			dataLen = charSet->substring(pcount, p, totLen,
				impure->vlu_desc.dsc_address, offset, length);
			impure->vlu_desc.dsc_length = static_cast<USHORT>(dataLen);
		}
	}

	return &impure->vlu_desc;
}


void SysFunction::checkArgsMismatch(int count) const
{
	if (count < minArgCount || (maxArgCount != -1 && count > maxArgCount))
	{
		status_exception::raise(Arg::Gds(isc_funmismat) << Arg::Str(name.c_str()));
	}
}
