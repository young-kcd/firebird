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
#ifndef _JRD_PRV_M_BYPASS_H_
#define _JRD_PRV_M_BYPASS_H_

/*  PRV$V_BYPASS incompatibly defined between VMS V4 and V5,
 *  so use PRV$M_BYPASS.
 *
 *  '$' causes preprocessor problems in WATCOM, even if ifdef is false.
 *  Putting it in an include file that only VMS looks at solves the
 *  problem. */

#ifndef PRV$M_BYPASS
#define PRV$M_BYPASS (1 << PRV$V_BYPASS)
#endif

#endif /* _JRD_PRV_M_BYPASS_H_ */
