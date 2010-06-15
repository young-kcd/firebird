/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		gen_proto.h
 *	DESCRIPTION:	Prototype Header file for gen.cpp
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

#ifndef DSQL_GEN_PROTO_H
#define DSQL_GEN_PROTO_H

void	GEN_descriptor(Jrd::DsqlCompilerScratch* dsqlScratch, const dsc* desc, bool texttype);
void	GEN_expr(Jrd::DsqlCompilerScratch*, Jrd::dsql_nod*);
void	GEN_hidden_variables(Jrd::DsqlCompilerScratch* dsqlScratch, bool inExpression);
void	GEN_port(Jrd::DsqlCompilerScratch*, Jrd::dsql_msg*);
void	GEN_request(Jrd::DsqlCompilerScratch*, Jrd::dsql_nod*);
void	GEN_rse(Jrd::DsqlCompilerScratch*, const Jrd::dsql_nod*);
void	GEN_return(Jrd::DsqlCompilerScratch*, const Firebird::Array<Jrd::dsql_nod*>& variables, bool, bool);
void	GEN_start_transaction(Jrd::DsqlCompilerScratch*, const Jrd::dsql_nod*);
void	GEN_statement(Jrd::DsqlCompilerScratch*, Jrd::dsql_nod*);

// CVC: I think this can be replaced by statement->append_uchar(byte) in the calling code.
inline void stuff(Jrd::DsqlCompiledStatement* statement, const UCHAR byte)
{
	statement->getBlrData().add(byte);
}

// Cram a word into the blr buffer.
inline void stuff_word(Jrd::DsqlCompiledStatement* dsqlScratch, USHORT word)
{
	stuff(dsqlScratch, word);
	stuff(dsqlScratch, word >> 8);
}

// Write out a string with one byte of length.
inline void stuff_string(Jrd::DsqlCompiledStatement* statement, const char* string, int len)
{
	fb_assert(len >= 0 && len <= 255);
	// CVC: Maybe the Release version should truncate "len" to 255?

	stuff(statement, len);
	statement->append_raw_string(string, len);
}

// Write out a string with one byte of length.
inline void stuff_cstring(Jrd::DsqlCompiledStatement* statement, const char* string)
{
	stuff_string(statement, string, strlen(string));
}

#endif //  DSQL_GEN_PROTO_H
