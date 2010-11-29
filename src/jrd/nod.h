/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		nod.h
 *	DESCRIPTION:	Runtime node types
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
 * 2002.10.29 Nickolay Samofatov: Added support for savepoints
 * Adriano dos Santos Fernandes
 */

NODE(nod_asn_list, asn_list, "")
NODE(nod_validate, validate, "")

NODE(nod_assignment, assignment, "")
NODE(nod_dcl_variable, declare, "")
NODE(nod_list, list, "")
NODE(nod_message, message, "")
NODE(nod_init_variable, init_variable, "")

NODE(nod_class_exprnode_jrd, class_exprnode_jrd, "class_exprnode_jrd")
NODE(nod_class_stmtnode_jrd, class_stmtnode_jrd, "class_stmtnode_jrd")
NODE(nod_class_recsrcnode_jrd, class_recsrcnode_jrd, "class_recsrcnode_jrd")
