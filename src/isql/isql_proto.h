/*
 *	PROGRAM:	Interactive SQL utility
 *	MODULE:		isql_proto.h
 *	DESCRIPTION:	Prototype header file for isql.e
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

#ifndef _ISQL_ISQL_PROTO_H_
#define _ISQL_ISQL_PROTO_H_

extern void ISQL_array_dimensions(TEXT *);
extern SCHAR *ISQL_blankterm(TEXT *);
extern void ISQL_build_table_list(void **, IB_FILE *, IB_FILE *, IB_FILE *);
extern void ISQL_build_view_list(void **, IB_FILE *, IB_FILE *, IB_FILE *);
extern int ISQL_commit_work(int, IB_FILE *, IB_FILE *, IB_FILE *);
extern void ISQL_copy_SQL_id(TEXT *, TEXT *, TEXT);
extern int ISQL_create_database(TEXT *,
								SCHAR **,
								SCHAR *,
								SCHAR *, IB_FILE *, IB_FILE *, IB_FILE *);
extern BOOLEAN ISQL_dbcheck(void);
extern void ISQL_disconnect_database(int);
extern void ISQL_errmsg(ISC_STATUS *);
extern void ISQL_warning(ISC_STATUS *);
extern void ISQL_exit_db(void);
extern int ISQL_extract(TEXT *, int, IB_FILE *, IB_FILE *, IB_FILE *);
extern int ISQL_frontend_command(TEXT *, IB_FILE *, IB_FILE *, IB_FILE *);
extern BOOLEAN ISQL_get_base_column_null_flag(TEXT *, SSHORT, TEXT *);
extern void ISQL_get_character_sets(SSHORT, SSHORT, USHORT, TEXT *);
extern SSHORT ISQL_get_default_char_set_id(void);
extern void ISQL_get_default_source(TEXT *, TEXT *, GDS__QUAD *);
extern SSHORT ISQL_get_field_length(TEXT *);
extern SSHORT ISQL_get_index_segments(TEXT *, TEXT *, BOOLEAN);
extern BOOLEAN ISQL_get_null_flag(TEXT *, TEXT *);
extern void ISQL_get_version(BOOLEAN);
extern SSHORT ISQL_init(IB_FILE *, IB_FILE *);
extern BOOLEAN ISQL_is_domain(TEXT *);
extern int ISQL_main(int, char **);
extern void ISQL_make_upper(UCHAR *);
extern void ISQL_msg_get(USHORT,
						 TEXT *, TEXT *, TEXT *, TEXT *, TEXT *, TEXT *);
extern void ISQL_print_validation(IB_FILE *, GDS__QUAD *, SSHORT, FRBRD *);
extern void ISQL_printf(IB_FILE *, TEXT *);
extern void ISQL_query_database(SSHORT *, IB_FILE *, IB_FILE *, IB_FILE *);
extern void ISQL_reset_settings(void);
extern void ISQL_ri_action_print(TEXT *, TEXT *, BOOLEAN);
extern int ISQL_sql_statement(TEXT *, IB_FILE *, IB_FILE *, IB_FILE *);
extern void ISQL_truncate_term(TEXT *, USHORT);
extern void ISQL_win_err(SCHAR *);

#endif /* _ISQL_ISQL_PROTO_H_ */
