/*
 *      PROGRAM:        JRD Math Floating Point Module
 *      MODULE:         math.c
 *      DESCRIPTION:    These are floating point operations which 
 * 			are explicitly performed outside the module 
 *			in which they are requested. This allows us to
 * 			place a stack switching layer in order to 
 *			guarantee that the EXE stack is in effect at 
 *			the time the floating point operations are 
 *			performed.  This is necessary for the floating 
 *			point co-processor to handle floating point 
 * 			errors correctly (instead of GPFing).
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
 */

#include "firebird.h"
#include "../jrd/jrd_math.h"

double _export MATH_multiply(double f1, double f2)
{
	return f1 * f2;
}

double _export MATH_divide(double f1, double f2)
{
	return f1 / f2;
}
