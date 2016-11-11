/*
 *	PROGRAM:		Decimal 64 & 128 type.
 *	MODULE:			DecFloat.cpp
 *	DESCRIPTION:	Floating point with decimal exponent.
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
 *  Copyright (c) 2016 Alex Peshkov <peshkoff at mail dot ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 */

#include "firebird.h"
#include "DecFloat.h"

#include "StatusArg.h"
#include "gen/iberror.h"

extern "C"
{
#include "../../extern/decNumber/decimal128.h"
#include "../../extern/decNumber/decimal64.h"
#include "../../extern/decNumber/decNumber.h"
}

#include <stdlib.h>
#include <string.h>
#include <float.h>

using namespace Firebird;

namespace {

class DecimalContext : public decContext
{
public:
	DecimalContext(const Decimal64*, DecimalStatus ds)
		: decSt(ds)
	{
		init(DEC_INIT_DECIMAL64);
	}

	DecimalContext(const Decimal128*, DecimalStatus ds)
		: decSt(ds)
	{
		init(DEC_INIT_DECIMAL128);
	}

	~DecimalContext()
	{
		// Typically excptions should better be not thrown from destructors.
		// But in our case there should never be any exception raised inside
		// Decimal64/128 functions - C library never throw, i.e. dtor will
		// be never called due to exception processing.
		// Therefore checking status in destructor is safe.
		checkForExceptions();
	}

	void checkForExceptions()
	{
		USHORT unmaskedExceptions = decSt.decExtFlag & decContextGetStatus(this);
		if (!unmaskedExceptions)
			return;

		for (USHORT mask = 1; mask; mask <<= 1)
		{
			if (mask & unmaskedExceptions)
			{
				decContextRestoreStatus(this, mask, ~0);
				const char* statusString = decContextStatusToString(this);
				(Arg::Gds(isc_random) << "DecFloat error" <<
				 Arg::Gds(isc_random) << statusString).raise();
			}
		}
	}

private:
	DecimalStatus decSt;

	void init(int kind)
	{
		decContextDefault(this, kind);
		fb_assert(decSt.roundingMode < USHORT(DEC_ROUND_MAX));
		enum rounding rMode = rounding(decSt.roundingMode);
		decContextSetRounding(this, rMode);
		traps = 0;		// do not raise SIGFPE
	}
};

CDecimal128 dmax(DBL_MAX, DecimalStatus(0)), dmin(-DBL_MAX, DecimalStatus(0));

} // anonymous namespace



namespace Firebird {

void Decimal64::setScale(DecimalStatus decSt, int scale)
{
	if (scale)
	{
		DecimalContext context(this, decSt);
		scale += decDoubleGetExponent(&dec);
		decDoubleSetExponent(&dec, &context, scale);
	}
}

Decimal64 Decimal64::set(int value, DecimalStatus decSt, int scale)
{
	decDoubleFromInt32(&dec, value);
	setScale(decSt, -scale);

	return *this;
}

Decimal64 Decimal64::set(SINT64 value, DecimalStatus decSt, int scale)
{
	{
		char s[30];		// for sure enough for int64
		sprintf(s, "%" SQUADFORMAT, value);
		DecimalContext context(this, decSt);
		decDoubleFromString(&dec, s, &context);
	}
	setScale(decSt, -scale);

	return *this;
}

Decimal64 Decimal64::set(const char* value, DecimalStatus decSt)
{
	DecimalContext context(this, decSt);
	decDoubleFromString(&dec, value, &context);

	return *this;
}

Decimal64 Decimal64::set(double value, DecimalStatus decSt)
{
	char s[50];
	sprintf(s, "%.016e", value);
	DecimalContext context(this, decSt);
	decDoubleFromString(&dec, s, &context);

	return *this;
}

void Decimal64::toString(DecimalStatus decSt, unsigned length, char* to) const
{
	DecimalContext context(this, decSt);
	if (length)
	{
		--length;
		char s[DECDOUBLE_String];
		memset(s, 0, sizeof(s));
		decDoubleToString(&dec, s);
		if (strlen(s) > length)
			decContextSetStatus(&context, DEC_Invalid_operation);
		else
			length = strlen(s);
		memcpy(to, s, length + 1);
	}
	else
		decContextSetStatus(&context, DEC_Invalid_operation);
}

void Decimal64::toString(string& to) const
{
	to.grow(DECDOUBLE_String);
	toString(DecimalStatus(0), to.length(), to.begin());		// provide long enough string, i.e. no traps
	to.recalculate_length();
}

SCHAR* Decimal64::getBytes()
{
	return reinterpret_cast<SCHAR*>(dec.bytes);
}

Decimal64 Decimal64::abs() const
{
	Decimal64 rc;
	decDoubleCopyAbs(&rc.dec, &dec);
	return rc;
}

Decimal64 Decimal64::ceil(DecimalStatus decSt) const
{
	DecimalContext context(this, decSt);
	Decimal64 rc;
	decDoubleToIntegralValue(&rc.dec, &dec, &context, DEC_ROUND_CEILING);
	return rc;
}

Decimal64 Decimal64::floor(DecimalStatus decSt) const
{
	DecimalContext context(this, decSt);
	Decimal64 rc;
	decDoubleToIntegralValue(&rc.dec, &dec, &context, DEC_ROUND_FLOOR);
	return rc;
}

int Decimal64::compare(DecimalStatus decSt, Decimal64 tgt) const
{
	DecimalContext context(this, decSt);
	decDouble r;
	decDoubleCompare(&r, &dec, &tgt.dec, &context);
	return decDoubleToInt32(&r, &context, DEC_ROUND_UP);
}

bool Decimal64::isInf() const
{
	switch(decDoubleClass(&dec))
	{
	case DEC_CLASS_NEG_INF:
	case DEC_CLASS_POS_INF:
		return true;
	}

	return false;
}

bool Decimal64::isNan() const
{
	switch(decDoubleClass(&dec))
	{
    case DEC_CLASS_SNAN:
    case DEC_CLASS_QNAN:
		return true;
	}

	return false;
}

#ifdef DEV_BUILD
int Decimal64::show()
{
	decDoubleShow(&dec, "");
	return 0;
}
#endif

Decimal64 Decimal64::neg() const
{
	Decimal64 rc;
	decDoubleCopyNegate(&rc.dec, &dec);
	return rc;
}



Decimal128 Decimal128::set(Decimal64 d64)
{
	decDoubleToWider(&d64.dec, &dec);

	return *this;
}

Decimal128 Decimal128::set(int value, DecimalStatus decSt, int scale)
{
	decQuadFromInt32(&dec, value);
	setScale(decSt, -scale);

	return *this;
}

Decimal128 Decimal128::set(SINT64 value, DecimalStatus decSt, int scale)
{
	{
		int high = value >> 32;
		unsigned low = value & 0xFFFFFFFF;

		DecimalContext context(this, decSt);
		decQuad pow2_32;
		decQuadFromString(&pow2_32, "4294967296", &context);

		decQuad up, down;
		decQuadFromInt32(&up, high);
		decQuadFromUInt32(&down, low);
		decQuadFMA(&dec, &up, &pow2_32, &down, &context);
	}
	setScale(decSt, -scale);

	return *this;
}

Decimal128 Decimal128::set(const char* value, DecimalStatus decSt)
{
	DecimalContext context(this, decSt);
	decQuadFromString(&dec, value, &context);

	return *this;
}

Decimal128 Decimal128::set(double value, DecimalStatus decSt)
{
	char s[50];
	sprintf(s, "%.016e", value);
	DecimalContext context(this, decSt);
	decQuadFromString(&dec, s, &context);

	return *this;
}

Decimal128 Decimal128::operator=(Decimal64 d64)
{
	decDoubleToWider(&d64.dec, &dec);
	return *this;
}

int Decimal128::toInteger(DecimalStatus decSt, int scale) const
{
	Decimal128 tmp(*this);
	tmp.setScale(decSt, -scale);
	DecimalContext context(this, decSt);
	enum rounding rMode = decContextGetRounding(&context);
	return decQuadToInt32(&tmp.dec, &context, rMode);
}

void Decimal128::toString(DecimalStatus decSt, unsigned length, char* to) const
{
	DecimalContext context(this, decSt);
	if (length)
	{
		--length;
		char s[DECQUAD_String];
		memset(s, 0, sizeof(s));
		decQuadToString(&dec, s);
		if (strlen(s) > length)
			decContextSetStatus(&context, DEC_Invalid_operation);
		else
			length = strlen(s);
		memcpy(to, s, length + 1);
	}
	else
		decContextSetStatus(&context, DEC_Invalid_operation);
}

void Decimal128::toString(string& to) const
{
	to.grow(DECQUAD_String);
	toString(DecimalStatus(0), to.length(), to.begin());		// provide long enough string, i.e. no traps
	to.recalculate_length();
}

double Decimal128::toDouble(DecimalStatus decSt) const
{
	DecimalContext context(this, decSt);
	if (compare(decSt, dmin) < 0 || compare(decSt, dmax) > 0)
		decContextSetStatus(&context, DEC_Overflow);
	else
	{
		char s[DECQUAD_String];
		memset(s, 0, sizeof(s));
		decQuadToString(&dec, s);
		return atof(s);
	}
	return 0.0;
}

SINT64 Decimal128::toInt64(DecimalStatus decSt, int scale) const
{
	Decimal128 wrk(*this);
	wrk.setScale(decSt, -scale);

	DecimalContext context(this, decSt);
	decQuad pow2_32, div, rem;
	decQuadFromString(&pow2_32, "4294967296", &context);
	decQuadDivideInteger(&div, &wrk.dec, &pow2_32, &context);
	decQuadRemainder(&rem, &wrk.dec, &pow2_32, &context);

	SINT64 high = decQuadToInt32(&div, &context, DEC_ROUND_DOWN);
	high <<= 32;
	return high + decQuadToInt32(&rem, &context, DEC_ROUND_DOWN);
}

SCHAR* Decimal128::getBytes()
{
	return reinterpret_cast<SCHAR*>(dec.bytes);
}

Decimal64 Decimal128::toDecimal64(DecimalStatus decSt) const
{
	Decimal64 rc;
	DecimalContext context(this, decSt);
	decDoubleFromWider(&rc.dec, &dec, &context);
	return rc;
}

void Decimal128::setScale(DecimalStatus decSt, int scale)
{
	if (scale)
	{
		DecimalContext context(this, decSt);
		scale += decQuadGetExponent(&dec);
		decQuadSetExponent(&dec, &context, scale);
	}
}

int Decimal128::compare(DecimalStatus decSt, Decimal128 tgt) const
{
	DecimalContext context(this, decSt);
	decQuad r;
	decQuadCompare(&r, &dec, &tgt.dec, &context);
	return decQuadToInt32(&r, &context, DEC_ROUND_UP);
}

bool Decimal128::isInf() const
{
	switch(decQuadClass(&dec))
	{
	case DEC_CLASS_NEG_INF:
	case DEC_CLASS_POS_INF:
		return true;
	}

	return false;
}

bool Decimal128::isNan() const
{
	switch(decQuadClass(&dec))
	{
    case DEC_CLASS_SNAN:
    case DEC_CLASS_QNAN:
		return true;
	}

	return false;
}

Decimal128 Decimal128::abs() const
{
	Decimal128 rc;
	decQuadCopyAbs(&rc.dec, &dec);
	return rc;
}

Decimal128 Decimal128::ceil(DecimalStatus decSt) const
{
	DecimalContext context(this, decSt);
	Decimal128 rc;
	decQuadToIntegralValue(&rc.dec, &dec, &context, DEC_ROUND_CEILING);
	return rc;
}

Decimal128 Decimal128::floor(DecimalStatus decSt) const
{
	DecimalContext context(this, decSt);
	Decimal128 rc;
	decQuadToIntegralValue(&rc.dec, &dec, &context, DEC_ROUND_FLOOR);
	return rc;
}

#ifdef DEV_BUILD
int Decimal128::show()
{
	decQuadShow(&dec, "");
	return 0;
}
#endif

Decimal128 Decimal128::add(DecimalStatus decSt, Decimal128 op2) const
{
	DecimalContext context(this, decSt);
	Decimal128 rc;
	decQuadAdd(&rc.dec, &dec, &op2.dec, &context);
	return rc;
}

Decimal128 Decimal128::sub(DecimalStatus decSt, Decimal128 op2) const
{
	DecimalContext context(this, decSt);
	Decimal128 rc;
	decQuadSubtract(&rc.dec, &dec, &op2.dec, &context);
	return rc;
}

Decimal128 Decimal128::mul(DecimalStatus decSt, Decimal128 op2) const
{
	DecimalContext context(this, decSt);
	Decimal128 rc;
	decQuadMultiply(&rc.dec, &dec, &op2.dec, &context);
	return rc;
}

Decimal128 Decimal128::div(DecimalStatus decSt, Decimal128 op2) const
{
	DecimalContext context(this, decSt);
	Decimal128 rc;
	decQuadDivide(&rc.dec, &dec, &op2.dec, &context);
	return rc;
}

Decimal128 Decimal128::neg() const
{
	Decimal128 rc;
	decQuadCopyNegate(&rc.dec, &dec);
	return rc;
}

Decimal128 Decimal128::fma(DecimalStatus decSt, Decimal128 op2, Decimal128 op3) const
{
	DecimalContext context(this, decSt);
	Decimal128 rc;
	decQuadFMA(&rc.dec, &op2.dec, &op3.dec, &dec, &context);
	return rc;
}

Decimal128 Decimal128::sqrt(DecimalStatus decSt) const
{
	decNumber dn;
	decQuadToNumber(&dec, &dn);

	DecimalContext context(this, decSt);
	decNumberSquareRoot(&dn, &dn, &context);

	Decimal128 rc;
	decQuadFromNumber(&rc.dec, &dn, &context);
	return rc;
}

Decimal128 Decimal128::pow(DecimalStatus decSt, Decimal128 op2) const
{
	decNumber dn, dn2;
	decQuadToNumber(&dec, &dn);
	decQuadToNumber(&op2.dec, &dn2);

	DecimalContext context(this, decSt);
	decNumberPower(&dn, &dn, &dn2, &context);

	Decimal128 rc;
	decQuadFromNumber(&rc.dec, &dn, &context);
	return rc;
}

Decimal128 Decimal128::ln(DecimalStatus decSt) const
{
	decNumber dn;
	decQuadToNumber(&dec, &dn);

	DecimalContext context(this, decSt);
	decNumberLn(&dn, &dn, &context);

	Decimal128 rc;
	decQuadFromNumber(&rc.dec, &dn, &context);
	return rc;
}

Decimal128 Decimal128::log10(DecimalStatus decSt) const
{
	decNumber dn;
	decQuadToNumber(&dec, &dn);

	DecimalContext context(this, decSt);
	decNumberLog10(&dn, &dn, &context);

	Decimal128 rc;
	decQuadFromNumber(&rc.dec, &dn, &context);
	return rc;
}

} // namespace Firebird
