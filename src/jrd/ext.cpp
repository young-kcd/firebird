/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		ext.cpp
 *	DESCRIPTION:	External file access
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
 *
 * 26-Sept-2001 Paul Beach - Windows External File Directory Config. Parameter 
 *
 * 2001.07.06 Sean Leyne - Code Cleanup, removed "#ifdef READONLY_DATABASE"
 *                         conditionals, as the engine now fully supports
 *                         readonly databases.
 *
 * 2001.08.07 Sean Leyne - Code Cleanup, removed "#ifdef READONLY_DATABASE"
 *                         conditionals, second attempt
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 */

#include "firebird.h"
#include "../jrd/common.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "../jrd/jrd.h"
#include "../jrd/req.h"
#include "../jrd/val.h"
#include "../jrd/exe.h"
#include "../jrd/rse.h"
#include "../jrd/ext.h"
#include "../jrd/tra.h"
#include "gen/iberror.h"
#include "../jrd/all_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/ext_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/met_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/thd.h"
#include "../jrd/vio_proto.h"
#include "../jrd/dls_proto.h"
#include "../common/config/config.h"
#include "../common/config/dir_list.h"
#include "../jrd/os/path_utils.h"
#include "../common/classes/init.h"

using namespace Jrd;

namespace {
	FILE *ext_fopen(const char *filename, const char *mode);

	class ExternalFileDirectoryList : public Firebird::DirectoryList
	{
	private:
		const Firebird::PathName getConfigString(void) const {
			return Firebird::PathName(Config::getExternalFileAccess());
		}
	public:
		ExternalFileDirectoryList(MemoryPool& p) : DirectoryList(p) 
		{
			initialize();
		}
	};
	Firebird::InitInstance<ExternalFileDirectoryList> iExternalFileDirectoryList;

	FILE *ext_fopen(const char *filename, const char *mode) {
		if (!iExternalFileDirectoryList().isPathInList(filename))
			ERR_post(isc_conf_access_denied,
				isc_arg_string, "external file",
				isc_arg_string, ERR_cstring(filename),
				isc_arg_end);

		return fopen(filename, mode);
	}
} // namespace

#ifdef WIN_NT
static const char* FOPEN_TYPE		= "a+b";
static const char* FOPEN_READ_ONLY	= "rb";
#else
static const char* FOPEN_TYPE		= "a+";
static const char* FOPEN_READ_ONLY	= "rb";
#endif

void EXT_close(RecordSource* rsb)
{
/**************************************
 *
 *	E X T _ c l o s e
 *
 **************************************
 *
 * Functional description
 *	Close a record stream for an external file.
 *
 **************************************/
}


void EXT_erase(record_param* rpb, int* transaction)
{
/**************************************
 *
 *	E X T _ e r a s e
 *
 **************************************
 *
 * Functional description
 *	Update an external file.
 *
 **************************************/

	ERR_post(isc_ext_file_delete, 0);
}


// Third param is unused.
ExternalFile* EXT_file(jrd_rel* relation, const TEXT* file_name, bid* description)
{
/**************************************
 *
 *	E X T _ f i l e
 *
 **************************************
 *
 * Functional description
 *	Create a file block for external file access.
 *
 **************************************/
	Database* dbb = GET_DBB();
	CHECK_DBB(dbb);

/* if we already have a external file associated with this relation just
 * return the file structure */
	if (relation->rel_file) {
		EXT_fini(relation);
	}

#ifdef WIN_NT
	/* Default number of file handles stdio.h on Windows is 512, use this 
	call to increase and set to the maximum */
	_setmaxstdio(2048);
#endif

	// If file_name has no path part, expand it in ExternalTablesDirs.
	Firebird::PathName Path, Name;
	PathUtils::splitLastComponent(Path, Name, file_name);
	if (Path.length() == 0)	{	// path component not present in file_name
		iExternalFileDirectoryList().expandFileName(Path, Name);
		file_name = Path.c_str();
	}

	ExternalFile* file =
		FB_NEW_RPT(*dbb->dbb_permanent, (strlen(file_name) + 1)) ExternalFile();
	relation->rel_file = file;
	strcpy(reinterpret_cast<char*>(file->ext_filename), file_name);
	file->ext_flags = 0;
	file->ext_ifi = NULL;

/* If the database is updateable, then try opening the external files in
 * RW mode. If the DB is ReadOnly, then open the external files only in
 * ReadOnly mode, thus being consistent.
 */
	if (!(dbb->dbb_flags & DBB_read_only))
		file->ext_ifi = (int *) ext_fopen(file_name, FOPEN_TYPE);
	if (!(file->ext_ifi))
	{
		/* could not open the file as read write attempt as read only */
		if (!(file->ext_ifi = (int *) ext_fopen(file_name, FOPEN_READ_ONLY)))
		{
			ERR_post(isc_io_error,
					 isc_arg_string, "fopen",
					 isc_arg_string,
					 ERR_cstring(reinterpret_cast<const char*>(file->ext_filename)),
					isc_arg_gds, isc_io_open_err, SYS_ERR, errno, 0);
		}
		else {
			file->ext_flags |= EXT_readonly;
		}
	}

	return file;
}


void EXT_fini(jrd_rel* relation)
{
/**************************************
 *
 *	E X T _ f i n i
 *
 **************************************
 *
 * Functional description
 *	Close the file associated with a relation.
 *
 **************************************/
	if (relation->rel_file) {
		ExternalFile* file = relation->rel_file;
		if (file->ext_ifi)
			fclose((FILE *) file->ext_ifi);
		/* before zeroing out the rel_file we need to deallocate the memory */
		delete relation->rel_file;
		relation->rel_file = 0;
	}
}


bool EXT_get(RecordSource* rsb)
{
/**************************************
 *
 *	E X T _ g e t
 *
 **************************************
 *
 * Functional description
 *	Get a record from an external file.
 *
 **************************************/
 	thread_db* tdbb = JRD_get_thread_data();

	jrd_rel* relation = rsb->rsb_relation;
	ExternalFile* file = relation->rel_file;
	jrd_req* request = tdbb->tdbb_request;

	if (request->req_flags & req_abort)
		return false;

	record_param* rpb = &request->req_rpb[rsb->rsb_stream];
	Record* record = rpb->rpb_record;
	const Format* format = record->rec_format;

	const SSHORT offset = (SSHORT) (IPTR) format->fmt_desc[0].dsc_address;
	UCHAR* p = record->rec_data + offset;
	SSHORT l = record->rec_length - offset;

	if (file->ext_ifi == 0 ||
		(fseek((FILE *) file->ext_ifi, rpb->rpb_ext_pos, 0) != 0))
	{
		ERR_post(isc_io_error,
				 isc_arg_string, "fseek",
				 isc_arg_string,
				 ERR_cstring(reinterpret_cast<const char*>(file->ext_filename)),
				 isc_arg_gds, isc_io_open_err, SYS_ERR, errno, 0);
	}

	while (l--) {
		const SSHORT c = getc((FILE *) file->ext_ifi);
		if (c == EOF)
			return false;
		*p++ = c;
	}
	rpb->rpb_ext_pos = ftell((FILE *) file->ext_ifi);

/* Loop thru fields setting missing fields to either blanks/zeros
   or the missing value */

	dsc desc;
	Format::fmt_desc_const_iterator desc_ptr = format->fmt_desc.begin();

    SSHORT i = 0;
	for (vec::iterator itr = relation->rel_fields->begin();
			i < format->fmt_count; ++i, ++itr, ++desc_ptr)
	{
	    const jrd_fld* field = (jrd_fld*) (*itr);
		SET_NULL(record, i);
		if (!desc_ptr->dsc_length || !field)
			continue;
		const Literal* literal = (Literal*) field->fld_missing_value;
		if (literal) {
			desc = *desc_ptr;
			desc.dsc_address = record->rec_data + (IPTR) desc.dsc_address;
			if (!MOV_compare(&literal->lit_desc, &desc))
				continue;
		}
		CLEAR_NULL(record, i);
	}

	return true;
}


void EXT_modify(record_param* old_rpb, record_param* new_rpb, int* transaction)
{
/**************************************
 *
 *	E X T _ m o d i f y
 *
 **************************************
 *
 * Functional description
 *	Update an external file.
 *
 **************************************/

/* ERR_post (isc_wish_list, isc_arg_interpreted, "EXT_modify: not yet implemented", 0); */
	ERR_post(isc_ext_file_modify, 0);
}


void EXT_open(RecordSource* rsb)
{
/**************************************
 *
 *	E X T _ o p e n
 *
 **************************************
 *
 * Functional description
 *	Open a record stream for an external file.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();

	jrd_rel* relation = rsb->rsb_relation;
	jrd_req* request = tdbb->tdbb_request;
	record_param* rpb = &request->req_rpb[rsb->rsb_stream];

	const Format* format;
	Record* record = rpb->rpb_record;
	if (!record || !(format = record->rec_format)) {
		format = MET_current(tdbb, relation);
		VIO_record(tdbb, rpb, format, request->req_pool);
	}

	rpb->rpb_ext_pos = 0;
}


RecordSource* EXT_optimize(OptimizerBlk* opt, SSHORT stream, jrd_nod** sort_ptr)
{
/**************************************
 *
 *	E X T _ o p t i m i z e
 *
 **************************************
 *
 * Functional description
 *	Compile and optimize a record selection expression into a
 *	set of record source blocks (rsb's).
 *
 **************************************/
/* all these are un refrenced due to the code commented below
jrd_nod*		node, inversion;
OptimizerBlk::opt_repeat	*tail, *opt_end;
SSHORT		i, size;
*/

	thread_db* tdbb = JRD_get_thread_data();

	CompilerScratch* csb = opt->opt_csb;
	CompilerScratch::csb_repeat* csb_tail = &csb->csb_rpt[stream];
	jrd_rel* relation = csb_tail->csb_relation;

/* Time to find inversions.  For each index on the relation
   match all unused booleans against the index looking for upper
   and lower bounds that can be computed by the index.  When
   all unused conjunctions are exhausted, see if there is enough
   information for an index retrieval.  If so, build up and
   inversion component of the boolean. */

/*
inversion = NULL;
opt_end = opt->opt_rpt + opt->opt_count;

if (opt->opt_count)
    for (i = 0; i < csb_tail->csb_indices; i++)
	{
	clear_bounds (opt, idx);
	for (tail = opt->opt_rpt; tail < opt_end; tail++)
	    {
	    node = tail->opt_conjunct;
	    if (!(tail->opt_flags & opt_used) &&
		computable (csb, node, -1))
		match (opt, stream, node, idx);
	    if (node->nod_type == nod_starts)
		compose (&inversion,
			 make_starts (opt, node, stream, idx), nod_bit_and);
	    }
	compose (&inversion, make_index (opt, relation, idx),
		nod_bit_and);
	idx = idx->idx_rpt + idx->idx_count;
	}
*/


	RecordSource* rsb = FB_NEW_RPT(*tdbb->getDefaultPool(), 0) RecordSource;
	rsb->rsb_type = rsb_ext_sequential;
	const SSHORT size = sizeof(irsb);

	rsb->rsb_stream = stream;
	rsb->rsb_relation = relation;
	rsb->rsb_impure = csb->csb_impure;
	csb->csb_impure += size;

	return rsb;
}


void EXT_ready(jrd_rel* relation)
{
/**************************************
 *
 *	E X T _ r e a d y
 *
 **************************************
 *
 * Functional description
 *	Open an external file.
 *
 **************************************/
}


void EXT_store(record_param* rpb, int* transaction)
{
/**************************************
 *
 *	E X T _ s t o r e
 *
 **************************************
 *
 * Functional description
 *	Update an external file.
 *
 **************************************/
	jrd_rel* relation = rpb->rpb_relation;
	ExternalFile* file = relation->rel_file;
	Record* record = rpb->rpb_record;
	const Format* format = record->rec_format;

/* Loop thru fields setting missing fields to either blanks/zeros
   or the missing value */

/* check if file is read only if read only then
   post error we cannot write to this file */
	if (file->ext_flags & EXT_readonly) {
		Database* dbb = GET_DBB();
		CHECK_DBB(dbb);
		/* Distinguish error message for a ReadOnly database */
		if (dbb->dbb_flags & DBB_read_only)
			ERR_post(isc_read_only_database, 0);
		else {
			ERR_post(isc_io_error,
					 isc_arg_string, "insert",
					 isc_arg_string, file->ext_filename,
					 isc_arg_gds, isc_io_write_err,
					 isc_arg_gds, isc_ext_readonly_err, 0);
		}
	}

	dsc desc;
	vec::iterator field_ptr = relation->rel_fields->begin();
	Format::fmt_desc_const_iterator desc_ptr = format->fmt_desc.begin();

	for (USHORT i = 0; i < format->fmt_count; i++, field_ptr++, desc_ptr++)
	{
		const jrd_fld* field = (jrd_fld*) *field_ptr;
		if (field &&
			!field->fld_computation &&
			desc_ptr->dsc_length &&
			TEST_NULL(record, i))
		{
			UCHAR* p = record->rec_data + (IPTR) desc_ptr->dsc_address;
			const Literal* literal = (Literal*) field->fld_missing_value;
			if (literal) {
				desc = *desc_ptr;
				desc.dsc_address = p;
				MOV_move(&literal->lit_desc, &desc);
			}
			else {
				USHORT l = desc_ptr->dsc_length;
				const UCHAR pad = (desc_ptr->dsc_dtype == dtype_text) ? ' ' : 0;
				do {
					*p++ = pad;
				} while (--l);
			}
		}
	}

	const USHORT offset = (USHORT) (IPTR) format->fmt_desc[0].dsc_address;
	const UCHAR* p = record->rec_data + offset;
	USHORT l = record->rec_length - offset;

	if (file->ext_ifi == 0
		|| (fseek((FILE *) file->ext_ifi, (SLONG) 0, 2) != 0))
	{
		ERR_post(isc_io_error, isc_arg_string, "fseek", isc_arg_string,
				 ERR_cstring(reinterpret_cast<const char*>(file->ext_filename)),
				 isc_arg_gds, isc_io_open_err, SYS_ERR, errno, 0);
	}
	for (; l--; ++p)
		putc(*p, (FILE *) file->ext_ifi);
	fflush((FILE *) file->ext_ifi);
}


void EXT_trans_commit(jrd_tra* transaction)
{
/**************************************
 *
 *	E X T _ t r a n s _ c o m m i t
 *
 **************************************
 *
 * Functional description
 *	Checkin at transaction commit time.
 *
 **************************************/
}


void EXT_trans_prepare(jrd_tra* transaction)
{
/**************************************
 *
 *	E X T _ t r a n s _ p r e p a r e
 *
 **************************************
 *
 * Functional description
 *	Checkin at transaction prepare time.
 *
 **************************************/
}


void EXT_trans_rollback(jrd_tra* transaction)
{
/**************************************
 *
 *	E X T _ t r a n s _ r o l l b a c k
 *
 **************************************
 *
 * Functional description
 *	Checkin at transaction rollback time.
 *
 **************************************/
}


void EXT_trans_start(jrd_tra* transaction)
{
/**************************************
 *
 *	E X T _ t r a n s _ s t a r t
 *
 **************************************
 *
 * Functional description
 *	Checkin at start transaction time.
 *
 **************************************/
}

