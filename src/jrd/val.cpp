/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		val.cpp
 *	DESCRIPTION:	Validation and garbage collection
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


/*

                      Database Validation and Repair
                      ==============================
 
                              Deej Bredenberg
                              March 16, 1994
			Updated: 1996-Dec-11 David Schnepper 


I. TERMINOLOGY 

   The following terminology will be helpful to understand in this discussion:

   record fragment: The smallest recognizable piece of a record; multiple 
                    fragments can be linked together to form a single version.
   record version:  A single version of a record representing an INSERT, UPDATE
                    or DELETE by a particular transaction (note that deletion 
                    of a record causes a new version to be stored as a
                    deleted stub).
   record chain:    A linked list of record versions chained together to
                    represent a single logical "record".
   slot:            The line number of the record on page.  A
                    variable-length array on each data page stores the 
                    offsets to the stored records on 
                    that page, and the slot is an index into that array.

   For more information on data page format, see my paper on the internals
   of the InterBase Engine.

II. COMMAND OPTIONS

   Here are all the options for gfix which have to do with validation, and 
   what they do:

   gfix switch   dpb parameter      
   -----------   -------------      

   -validate    isc_dpb_verify  (gds__dpb_verify prior to 4.0)

    Invoke validation and repair.  All other switches modify this switch.

   -full        isc_dpb_records       

    Visit all records.  Without this switch, only page structures will be
    validated, which does involve some limited checking of records.     

   -mend        isc_dpb_repair     

    Attempts to mend the database where it can to make it viable for reading;
    does not guarantee to retain data.

   -no_update   isc_dpb_no_update  

    Specifies that orphan pages not be released, and allocated pages not 
    be marked in use when found to be free.  Actually a misleading switch
    name since -mend will update the database, but if -mend is not specified 
    and -no_update is specified, then no updates will occur to the database.

   -ignore      isc_dpb_ignore

    Tells the engine to ignore checksums in fetching pages.  Validate will 
    report on the checksums, however.  Should probably not even be a switch,
    it should just always be in effect.  Otherwise checksums will disrupt  
    the validation.  Customers should be advised to always use it.
    NOTE: Unix 4.0 (ODS 8.0) does not have on-page checksums, and all
          platforms under ODS 9.0 (NevaStone & above) does not have
          checksums.

III.  OPERATION

   Validation runs only with exclusive access to the database, to ensure
   that database structures are not modified during validation.  On attach,
   validate attempts to obtain an exclusive lock on the database.

   If other attachments are already made locally or through the same multi-
   client server, validate gives up with the message:

   "Lock timeout during wait transaction
   -- Object "database_filename.fdb" is in use"

   If other processes or servers are attached to the database, validate 
   waits for the exclusive lock on the database (i.e. waits for every 
   other server to get out of the database).

   NOTE: Ordinarily when processes gain exclusive access to the database, 
   all active transactions are marked as dead on the Transaction Inventory 
   Pages.  This feature is turned off for validation.

IV. PHASES OF VALIDATION

   There are two phases to the validation, the first of which is a walk through
   the entire database (described below).  During this phase, all pages visited 
   are stored in a bitmap for later use during the garbage collection phase.  
 
   A. Visiting Pages

      During the walk-through phase, any page that is fetched goes through a
      basic validation:

      1. Page Type Check

         Each page is check against its expected type.  If the wrong type
         page is found in the page header, the message:

         "Page xxx wrong type (expected xxx encountered xxx)"

         is returned.  This could represent a) a problem with the database 
         being overwritten, b) a bug with InterBase page allocation mechanisms 
         in which one page was written over another, or c) a page which was 
         allocated but never written to disk (most likely if the encountered
         page type was 0).

         The error does not tell you what page types are what, so here
         they are for reference:

         #define pag_undefined     0    // purposely undefined
         #define pag_header        1    // Database header page
         #define pag_pages         2    // Page inventory page
         #define pag_transactions  3    // Transaction inventory page
         #define pag_pointer       4    // Pointer page
         #define pag_data          5    // Data page
         #define pag_root          6    // Index root page
         #define pag_index         7    // Index (B-tree) page
         #define pag_blob          8    // Blob data page
         #define pag_ids           9    // Gen-ids
         #define pag_log           10   // Write ahead log page: 4.0 only

      2. Checksum

         If -ignore is specified, the checksum is specifically checked in
         validate instead of in the engine.  If the checksum is found to 
         be wrong, the error:
 
         "Checksum error on page xxx"

         is returned.  This is harmless when found by validate, and the page
         will still continue to be validated--if data structures can be 
         validated on page, they will be.  If -mend is specified, the page 
         will be marked for write, so that when the page is written to disk 
         at the end of validation the checksum will automatically be 
         recalculated.

         Note: For 4.0 only Windows & NLM platforms keep page checksums.

      3. Revisit

         We check each page fetched against the page bitmap to make sure we
         have not visited already.  If we have, the error:
 
          "Page xxx doubly allocated"

         is returned.  This should catch the case when a page of the same type 
         is allocated for two different purposes.

	 Data pages are not checked with the Revisit mechanism - when walking
         record chains and fragments they are frequently revisited.

   B. Garbage Collection

      During this phase, the Page Inventory (PIP) pages are checked against the
      bitmap of pages visited.  Two types of errors can be detected during
      this phase.

      1. Orphan Pages

         If any pages in the page inventory were not visited 
         during validation, the following error will be returned:

         "Page xxx is an orphan"

         If -no_update was not specified, the page will be marked as free
         on the PIP.

      2. Improperly Freed Pages

         If any pages marked free in the page inventory were in fact 
         found to be in use during validation, the following error 
         will be returned:
 
         "Page xxx is use but marked free"  (sic)
     
         If -no_update was not specified, the page will be marked in use
         on the PIP.

      NOTE:  If errors were found during the validation phase, no changes will
      be made to the PIP pages.  This assumes that we did not have a chance to
      visit all the pages because invalid structures were detected.

V. WALK-THROUGH PHASE

   A. Page Fetching

      In order to ensure that all pages are fetched during validation, the
      following pages are fetched just for the most basic validation:

      1. The header page (and for 4.0 any overflow header pages).
      2. Log pages for after-image journalling (4.0 only).
      3. Page Inventory pages.
      4. Transaction Inventory pages

         If the system relation RDB$PAGES could not be read or did not
         contain any TIP pages, the message: 

         "Transaction inventory pages lost"
 
         will be returned.  If a particular page is missing from the 
         sequence as established by RDB$PAGE_SEQUENCE, then the following
         message will be returned:
                                        
         "Transaction inventory page lost, sequence xxx"

         If -mend is specified, then a new TIP will be allocated on disk and 
         stored in RDB$PAGES in the proper sequence.  All transactions which 
         would have been on that page are assumed committed.
 
         If a TIP page does not point to the next one in sequence, the
         following message will be returned:

         "Transaction inventory pages confused, sequence xxx"

      5. Generator pages as identified in RDB$PAGES.
   
   B. Relation Walking

      All the relations in the database are walked.  For each relation, all
      indices defined on the relation are fetched, and all pointer and 
      data pages associated with the relation are fetched (see below).

      But first, the metadata is scanned from RDB$RELATIONS to fetch the
      format of the relation.  If this information is missing or 
      corrupted the relation cannot be walked.  
      If any bugchecks are encountered from the scan, the following 
      message is returned:

      "bugcheck during scan of table xxx (<table_name>)"

      This will prevent any further validation of the relation.

      NOTE: For views, the metadata is scanned but nothing further is done.

   C. Index Walking

      Prior to 4.5 (NevaStone) Indices were walked before data pages.
      In NevaStone Index walking was moved to after data page walking.
      Please refer to the later section entitled "Index Walking".

   D. Pointer Pages

      All the pointer pages for the relation are walked.  As they are walked
      all child data pages are walked (see below).  If a pointer page cannot 
      be found, the following message is returned:

      "Pointer page (sequence xxx) lost"

      If the pointer page is not part of the relation we expected or
      if it is not marked as being in the proper sequence, the following
      message is returned:
                       
      "Pointer page xxx is inconsistent"
 
      If each pointer page does not point to the next pointer page as
      stored in the RDB$PAGE_SEQUENCE field in RDB$PAGES, the following 
      error is returned:

      "Pointer page (sequence xxx) inconsistent"

   E. Data Pages

      Each of the data pages referenced by the pointer page is fetched.
      If any are found to be corrupt at the page level, and -mend is 
      specified, the page is deleted from its pointer page.  This will 
      cause a whole page of data to be lost.

      The data page is corrupt at the page level if it is not marked as
      part of the current relation, or if it is not marked as being in 
      the proper sequence.  If either of these conditions occurs, the 
      following error is returned:

      "Data page xxx (sequence xxx) is confused"

   F. Slot Validation

      Each of the slots on the data page is looked at, up to the count
      of records stored on page.  If the slot is non-zero, the record 
      fragment at the specified offset is retrieved.  If the record
      begins before the end of the slots array, or continues off the
      end of the page, the following error is returned:

      "Data page xxx (sequence xxx), line xxx is bad"

      where "line" means the slot number.  
  
      NOTE: If this condition is encountered, the data page is considered 
      corrupt at the page level (and thus will be removed from its
      pointer page if -mend is specified).
                                          
   G. Record Validation
                       
      The record at each slot is looked at for basic validation, regardless
      of whether -full is specified or not.  The fragment could be any of the 
      following:

      1.  Back Version

          If the fragment is marked as a back version, then it is skipped.  
          It will be fetched as part of its record.

      2.  Corrupt

          If the fragment is determined to be corrupt for any reason, and -mend 
          is specified, then the record header is marked as damaged.

      3.  Damaged

          If the fragment is marked damaged already from a previous visit or
          a previous validation, the following error is returned:
      
          "Record xxx is marked as damaged"

          where xxx is the record number.  

      4.  Bad Transaction 

          If the record is marked with a transaction id greater than the last 
          transaction started in the database, the following error is returned:
                                         
          "Record xxx has bad transaction xxx"

   H. Record Walking

      If -full is specified, and the fragment is the first fragment in a logical
      record, then the record at this slot number is fully retrieved.  This
      involves retrieving all versions, and all fragments of each 
      particular version.  In other 
      words, the entire logical record will be retrieved.

      1. Back Versions

         If there are any back versions, they are visited at this point.  
         If the back version is on another page, the page is fetched but 
         not validated since it will be walked separately.  

         If the slot number of the back version is greater than the max
         records on page, or there is no record stored at that slot number, 
         or it is a blob record, or it is a record fragment, or the 
         fragment itself is invalid, the following error 
         message is returned:

         "Chain for record xxx is broken"

      2. Incomplete

         If the record header is marked as incomplete, it means that there
         are additional fragments to be fetched--the record was too large 
         to be stored in one slot.
         A pointer is stored in the record to the next fragment in the list.

         For fragmented records, all fragments are fetched to form a full
         record version.  If any of the fragments is not in a valid position,
         or is not the correct length, the following error is returned:

         "Fragmented record xxx is corrupt"      
   
      Once the full record has been retrieved, the length of the format is
      checked against the expected format stored in RDB$FORMATS (the 
      format number is stored with the record, representing the exact 
      format of the relation at the time the record was stored.)  
      If the length of the reconstructed record does not match
      the expected format length, the following error is returned:

      "Record xxx is wrong length"

      For delta records (record versions which represent updates to the record)
      this check is not made.

   I. Blob Walking 

      If the slot on the data page points to a blob record, then the blob
      is fetched (even without -full).  This has several cases, corresponding 
      to the various blob levels.  (See the "Engine Internals" document for a 
      discussion of blob levels.)
             
      Level                      Action
      -----   ----------------------------------------------------------------- 
        0     These are just records on page, and no further validation is done.
        1     All the pages pointed to by the blob record are fetched and
              validated in sequence.
        2     All pages pointed to by the blob pointer pages are fetched and 
              validated.
        3     The blob page is itself a blob pointer page; all its children
              are fetched and validated.

      For each blob page found, some further validation is done.  If the
      page does not point back to the lead page, the following error 
      is returned:

      "Warning: blob xxx appears inconsistent"
   
      where xxx corresponds to the blob record number.  If any of the blob pages
      are not marked in the sequence we expect them to be in, the following
      error is returned:
 
      "Blob xxx is corrupt"
 
      Tip: the message for the same error in level 2 or 3 blobs is slightly
           different:

      "Blob xxx corrupt"

      If we have lost any of the blob pages in the sequence, the following error
      is returned:

      "Blob xxx is truncated"

      If the fetched blob is determined to be corrupt for any of the above
      reasons, and -mend is specified, then the blob record is marked as
      damaged.

   J. Index Walking

      In 4.5 (NevaStone) Index walking was moved to after the completion
      of data page walking.

      The indices for the relation are walked.  If the index root page
      is missing, the following message is returned:

      "Missing index root page"

      and the indices are not walked.  Otherwise the index root page
      is fetched and all indices on the page fetched. 

      For each index, the btree pages are fetched from top-down, left to
      right.  
      Basic validation is made on non-leaf pages to ensure that each node
      on page points to another index page.  If -full validation is specified
      then the lower level page is fetched to ensure it is starting index
      entry is consistent with the parent entry. 
      On leaf pages, the records pointed to by the index pages are not 
      fetched, the keys are looked at to ensure they are in correct 
      ascending order.

      If a visited page is not part of the specified relation and index,
      the following error is returned:
      
      "Index xxx is corrupt at page xxx"
        
      If there are orphan child pages, i.e. a child page does not have its entry 
      as yet in the parent page, however the child's left sibling page has it's 
      btr_sibling updated, the following error is returned
	    
      "Index xxx has orphan child page at page xxx"

      If the page does not contain the number of nodes we would have
      expected from its marked length, the following error is returned:

      "Index xxx is corrupt on page xxx"

      While we are walking leaf pages, we keep a bitmap of all record
      numbers seen in the index.  At the conclusion of the index walk
      we compare this bitmap to the bitmap of all records in the 
      relation (calculated during data page/Record Validation phase).
      If the bitmaps are not equal then we have a corrupt index
      and the following error is reported:

      "Index %d is corrupt (missing entries)"

      We do NOT check that each version of each record has a valid
      index entry - nor do we check that the stored key for each item
      in the index corresponds to a version of the specified record.

   K. Relation Checking

      We count the number of backversions seen while walking pointer pages,
      and separately count the number of backversions seen while walking
      record chains.  If these numbers do not match it indicates either
      "orphan" backversion chains or double-linked chains.  If this is
      see the following error is returned:

     "Relation has %ld orphan backversions (%ld in use)"

      Currently we do not try to correct this condition, mearly report
      it.  For "orphan" backversions the space can be reclaimed by
      a backup/restore.  For double-linked chains a SWEEP should
      remove all the backversions.

VI. ADDITIONAL NOTES

   A.  Damaged Records

      If any corruption of a record fragment is seen during validation, the 
      record header is marked as "damaged".  As far as I can see, this has no 
      effect on the engine per se.  Records marked as damaged will still be 
      retrieved by the engine itself.  There is some question in my mind as
      to whether this record should be retrieved at all during a gbak.

      If a damaged record is visited, the following error message will appear:

      "Record xxx is marked as damaged"

      Note that when a damaged record is first detected, this message is not
      actually printed.  The record is simply marked as damaged.  It is only 
      thereafter when the record is visited that this message will appear.
      So I would postulate that unless a full validation is done at some point,
      you would not see this error message; once the full validation is done, 
      the message will be returned even if you do not specify -full.

   B. Damaged Blobs

      Blob records marked as damaged cannot be opened and will not be deleted 
      from disk.  This means that even during backup the blob structures marked 
      as damaged will not be fetched and backed up.  (Why this is done
      differently for blobs than for records I cannot say.  
      Perhaps it was viewed as too difficult to try to retrieve a damaged blob.)

*/

#include "firebird.h"
#include "memory_routines.h"
#include "../jrd/ib_stdio.h"
#include "../jrd/common.h"
#include <stdarg.h>
#include "../jrd/jrd.h"
#include "../jrd/ods.h"
#include "../jrd/pag.h"
#include "../jrd/gds.h"
#include "../jrd/val.h"
#include "../jrd/btr.h"
#include "../jrd/all.h"
#include "../jrd/lck.h"
#include "../jrd/cch.h"
#include "../jrd/rse.h"
#include "../jrd/sbm.h"
#include "../jrd/tra.h"
#include "../jrd/all_proto.h"
#include "../jrd/btr_proto.h"
#include "../jrd/cch_proto.h"
#include "../jrd/dbg_proto.h"
#include "../jrd/dpm_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/jrd_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/met_proto.h"
#include "../jrd/sbm_proto.h"
#include "../jrd/sch_proto.h"
#include "../jrd/thd_proto.h"
#include "../jrd/tra_proto.h"
#include "../jrd/val_proto.h"

#ifdef DEBUG_VAL_VERBOSE
#include "../jrd/dmp_proto.h"
/* Control variable for verbose output during debug of
   validation.  
   0 == logged errors only
   1 == logical output also
   2 == physical page output also */
static USHORT VAL_debug_level = 0;
#endif

/* Validation/garbage collection/repair control block */

typedef struct vdr
{
	SBM vdr_page_bitmap;
	SLONG vdr_max_page;
	USHORT vdr_flags;
	USHORT vdr_errors;
	SLONG vdr_max_transaction;
	ULONG vdr_rel_backversion_counter;	/* Counts slots w/rhd_chain */
	ULONG vdr_rel_chain_counter;	/* Counts chains w/rdr_chain */
	SBM vdr_rel_records;		/* 1 bit per valid record */
	SBM vdr_idx_records;		/* 1 bit per index item */
} *VDR;

#define vdr_update		2		/* fix simple things */
#define vdr_repair		4		/* fix non-simple things (-mend) */
#define vdr_records		8		/* Walk all records */

typedef enum {
	fetch_ok,
	fetch_checksum,
	fetch_type,
	fetch_duplicate
} FETCH_CODE;

typedef enum {
	rtn_ok,
	rtn_corrupt,
	rtn_eof
} RTN;

static const TEXT msg_table[][52] = {
	"Page %ld wrong type (expected %d encountered %d)",	/* 0 */
	"Checksum error on page %ld",
	"Page %ld doubly allocated",
	"Page %ld is use but marked free",
	"Page %ld is an orphan",
	"Warning: blob %ld appears inconsistent",	/* 5 */
	"Blob %ld is corrupt",
	"Blob %ld is truncated",
	"Chain for record %ld is broken",
	"Data page %ld (sequence %ld) is confused",
	"Data page %ld (sequence %ld), line %ld is bad",	/* 10 */
	"Index %d is corrupt on page %ld",
	"Pointer page (sequence %ld) lost",
	"Pointer page (sequence %ld) inconsistent",
	"Record %ld is marked as damaged",
	"Record %ld has bad transaction %ld",	/* 15 */
	"Fragmented record %ld is corrupt",
	"Record %ld is wrong length",
	"Missing index root page",
	"Transaction inventory pages lost",
	"Transaction inventory page lost, sequence %ld",	/* 20 */
	"Transaction inventory pages confused, sequence %ld",
	"Relation has %ld orphan backversions (%ld in use)",
	"Index %d is corrupt (missing entries)",
	"Index %d has orphan child page at page %ld"
};


static RTN corrupt(TDBB, VDR, USHORT, JRD_REL, ...);
static FETCH_CODE fetch_page(TDBB, VDR, SLONG, USHORT, WIN *, void *);
static void garbage_collect(TDBB, VDR);
#ifdef DEBUG_VAL_VERBOSE
static void print_rhd(USHORT, RHD);
#endif
static RTN walk_blob(TDBB, VDR, JRD_REL, BLH, USHORT, SLONG);
static RTN walk_chain(TDBB, VDR, JRD_REL, RHD, SLONG);
static void walk_database(TDBB, VDR);
static RTN walk_data_page(TDBB, VDR, JRD_REL, SLONG, SLONG);
static void walk_generators(TDBB, VDR);
static void walk_header(TDBB, VDR, SLONG);
static RTN walk_index(TDBB, VDR, JRD_REL, SLONG, USHORT);
static void walk_log(TDBB, VDR);
static void walk_pip(TDBB, VDR);
static RTN walk_pointer_page(TDBB, VDR, JRD_REL, int);
static RTN walk_record(TDBB, VDR, JRD_REL, RHD, USHORT, SLONG, USHORT);
static RTN walk_relation(TDBB, VDR, JRD_REL);
static RTN walk_root(TDBB, VDR, JRD_REL);
static RTN walk_tip(TDBB, VDR, SLONG);

static const SLONG end_level = END_LEVEL, end_bucket = END_BUCKET;

BOOLEAN VAL_validate(TDBB tdbb, USHORT switches)
{
/**************************************
 *
 *	V A L _ v a l i d a t e
 *
 **************************************
 *
 * Functional description
 *	Validate a database.
 *
 **************************************/
	struct vdr control;
	JrdMemoryPool *val_pool, *old_pool;
	ATT att;
	USHORT i;
	DBB dbb;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	att = tdbb->tdbb_attachment;

	try {

	old_pool = tdbb->tdbb_default;
	val_pool = 0;
	tdbb->tdbb_default = val_pool = JrdMemoryPool::createPool();

	control.vdr_page_bitmap = NULL;
	control.vdr_flags = 0;
	control.vdr_errors = 0;

	if (switches & gds_dpb_records)
		control.vdr_flags |= vdr_records;

	if (switches & gds_dpb_repair)
		control.vdr_flags |= vdr_repair;

	if (!(switches & gds_dpb_no_update))
		control.vdr_flags |= vdr_update;

	control.vdr_max_page = 0;
	control.vdr_rel_records = NULL;
	control.vdr_idx_records = NULL;

/* initialize validate errors */

	if (!att->att_val_errors) {
		att->att_val_errors = vcl::newVector(*dbb->dbb_permanent, VAL_MAX_ERROR);
	}
	else {
		for (i = 0; i < VAL_MAX_ERROR; i++)
			(*att->att_val_errors)[i] = 0;
	}

	tdbb->tdbb_flags |= TDBB_sweeper;
	walk_database(tdbb, &control);
	if (control.vdr_errors)
		control.vdr_flags &= ~vdr_update;

	garbage_collect(tdbb, &control);
	CCH_flush(tdbb, (USHORT) FLUSH_FINI, 0);

	JrdMemoryPool::deletePool(val_pool);
	tdbb->tdbb_default = old_pool;
	tdbb->tdbb_flags &= ~TDBB_sweeper;
	}	// try
	catch (const std::exception&) {
		JrdMemoryPool::deletePool(val_pool);
		tdbb->tdbb_default = old_pool;
		tdbb->tdbb_flags &= ~TDBB_sweeper;
		return FALSE;
	}

	return TRUE;
}

static RTN corrupt(TDBB tdbb, VDR control, USHORT err_code, JRD_REL relation, ...)
{
/**************************************
 *
 *	c o r r u p t
 *
 **************************************
 *
 * Functional description
 *	Corruption has been detected.
 *
 **************************************/
	TEXT s[256], *p;
	va_list ptr;
	const TEXT *string;
	ATT att;

	VA_START(ptr, relation);

	SET_TDBB(tdbb);
	att = tdbb->tdbb_attachment;
	if (err_code < att->att_val_errors->count())
		(*att->att_val_errors)[err_code]++;

	string = msg_table[err_code];

	sprintf(s, "Database: %s\n\t",
			tdbb->tdbb_attachment->att_filename->str_data);
	for (p = s; *p; p++)
		/* nothing */ ;
	vsprintf(p, string, ptr);

	if (relation) {
		for (; *p; p++)
			/* nothing */ ;
		sprintf(p, " in table %s (%d)\n", relation->rel_name,
				relation->rel_id);
	}

	gds__log(s);

#ifdef DEBUG_VAL_VERBOSE
	if (VAL_debug_level >= 0)
		ib_fprintf(ib_stdout, "LOG:\t%s\n", s);
#endif

	if (control)
		++control->vdr_errors;

	return rtn_corrupt;
}

static FETCH_CODE fetch_page(TDBB tdbb,
							 VDR control,
							 SLONG page_number,
							 USHORT type, WIN * window, void *page_pointer)
{
/**************************************
 *
 *	f e t c h _ p a g e
 *
 **************************************
 *
 * Functional description
 *	Fetch page and return type of illness, if any.  If a control block
 *	is present, check for doubly allocated pages and account for page
 *	use.
 *
 **************************************/
	DBB dbb;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

#ifdef SUPERSERVER
	if (--tdbb->tdbb_quantum < 0 && !tdbb->tdbb_inhibit)
		JRD_reschedule(tdbb, 0, TRUE);
#endif

	window->win_page = page_number;
	window->win_flags = 0;
	*(PAG *) page_pointer = CCH_FETCH_NO_SHADOW(tdbb, window, LCK_write, 0);

	if ((*(PAG *) page_pointer)->pag_type != type) {
		corrupt(tdbb, control, VAL_PAG_WRONG_TYPE,
				0, page_number, type, (*(PAG *) page_pointer)->pag_type);
		return fetch_type;
	}

	if (!control)
		return fetch_ok;

/* If "damaged" flag was set, checksum may be incorrect.  Check. */

	if ((dbb->dbb_flags & DBB_damaged) && !CCH_validate(window)) {
		corrupt(tdbb, control, VAL_PAG_CHECKSUM_ERR, 0, page_number);
		if (control->vdr_flags & vdr_repair)
			CCH_MARK(tdbb, window);
	}

	control->vdr_max_page = MAX(control->vdr_max_page, page_number);

/* For walking back versions & record fragments on data pages we
   sometimes will fetch the same page more than once.  In that
   event we don't report double allocation.  If the page is truely
   double allocated (to more than one relation) we'll find it
   when the on-page relation id doesn't match */

	if ((type != pag_data) && SBM_test(control->vdr_page_bitmap, page_number)) {
		corrupt(tdbb, control, VAL_PAG_DOUBLE_ALLOC, 0, page_number);
		return fetch_duplicate;
	}


	SBM_set(tdbb, &control->vdr_page_bitmap, page_number);

	return fetch_ok;
}

static void garbage_collect(TDBB tdbb, VDR control)
{
/**************************************
 *
 *	g a r b a g e _ c o l l e c t
 *
 **************************************
 *
 * Functional description
 *	The database has been walked; compare the page inventory against
 *	the bitmap of pages visited.
 *
 **************************************/
	DBB dbb;
	WIN window;
	PIP page;
	PGC pgc;
	UCHAR *p, *end, byte;
	USHORT sequence, i;
	SLONG number, page_number;

	SET_TDBB(tdbb);

	dbb = tdbb->tdbb_database;
	pgc = dbb->dbb_pcontrol;

	for (sequence = 0, number = 0; number < control->vdr_max_page; sequence++) {
		page_number = (sequence) ? sequence * pgc->pgc_ppp - 1 : pgc->pgc_pip;
		fetch_page(tdbb, 0, page_number, pag_pages, &window, &page);
		p = page->pip_bits;
		end = p + pgc->pgc_bytes;
		while (p < end && number < control->vdr_max_page) {
			byte = *p++;
			for (i = 8; i; --i, byte >>= 1, number++)
				if (SBM_test(control->vdr_page_bitmap, number)) {
					if (byte & 1) {
						corrupt(tdbb, control, VAL_PAG_IN_USE, 0, number);
						if (control->vdr_flags & vdr_update) {
							CCH_MARK(tdbb, &window);
							p[-1] &= ~(1 << (number & 7));
						}
						DEBUG;
					}
				}

			/* Page is potentially an orphan - but don't declare it as such
			   unless we think we walked all pages */

				else if (!(byte & 1) && (control->vdr_flags & vdr_records)) {
					corrupt(tdbb, control, VAL_PAG_ORPHAN, 0, number);
					if (control->vdr_flags & vdr_update) {
						CCH_MARK(tdbb, &window);
						p[-1] |= 1 << (number & 7);
					}
					DEBUG;
				}
		}
		byte = p[-1];
		CCH_RELEASE(tdbb, &window);
		if (byte & 0x80)
			break;
	}

#ifdef DEBUG_VAL_VERBOSE
/* Dump verbose output of all the pages fetched */
	if (VAL_debug_level >= 2) {
		SLONG dmp_page_number;
		dmp_page_number = -1;
		while (SBM_next
			   (control->vdr_page_bitmap, &dmp_page_number,
				RSE_get_forward)) DMP_page(dmp_page_number,
										   dbb->dbb_page_size);
	};
#endif
}

#ifdef DEBUG_VAL_VERBOSE
static void print_rhd(USHORT length, RHD header)
{
/**************************************
 *
 *	p r i n t _ r h d
 *
 **************************************
 *
 * Functional description
 *	Debugging routine to print a
 *	Record Header Data.
 *
 **************************************/
	if (VAL_debug_level) {
		ib_fprintf(ib_stdout, "rhd: len %d TX %d fmt %d ",
				   length, header->rhd_transaction, header->rhd_format);
		ib_fprintf(ib_stdout, "BP %d/%d flags 0x%x ",
				   header->rhd_b_page, header->rhd_b_line, header->rhd_flags);
		if (header->rhd_flags & rhd_incomplete) {
			RHDF fragment;
			fragment = (RHDF) header;
			ib_fprintf(ib_stdout, "FP %d/%d ",
					   fragment->rhdf_f_page, fragment->rhdf_f_line);
		};
		ib_fprintf(ib_stdout, "%s ",
				   (header->rhd_flags & rhd_deleted) ? "DEL" : "   ");
		ib_fprintf(ib_stdout, "%s ",
				   (header->rhd_flags & rhd_chain) ? "CHN" : "   ");
		ib_fprintf(ib_stdout, "%s ",
				   (header->rhd_flags & rhd_fragment) ? "FRG" : "   ");
		ib_fprintf(ib_stdout, "%s ",
				   (header->rhd_flags & rhd_incomplete) ? "INC" : "   ");
		ib_fprintf(ib_stdout, "%s ",
				   (header->rhd_flags & rhd_blob) ? "BLB" : "   ");
		ib_fprintf(ib_stdout, "%s ",
				   (header->rhd_flags & rhd_delta) ? "DLT" : "   ");
		ib_fprintf(ib_stdout, "%s ",
				   (header->rhd_flags & rhd_large) ? "LRG" : "   ");
		ib_fprintf(ib_stdout, "%s ",
				   (header->rhd_flags & rhd_damaged) ? "DAM" : "   ");
		ib_fprintf(ib_stdout, "\n");
	};
}
#endif

static RTN walk_blob(TDBB tdbb,
					 VDR control,
					 JRD_REL relation, BLH header, USHORT length, SLONG number)
{
/**************************************
 *
 *	w a l k _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Walk a blob.
 *
 **************************************/
	WIN window1, window2;
	BLP page1, page2;
	SLONG sequence, *pages1, *pages2, *end1, *end2;

	SET_TDBB(tdbb);

#ifdef DEBUG_VAL_VERBOSE
	if (VAL_debug_level) {
		ib_fprintf(ib_stdout,
				   "walk_blob: level %d lead page %d max pages %d max segment %d\n",
				   header->blh_level, header->blh_lead_page,
				   header->blh_max_sequence, header->blh_max_segment);
		ib_fprintf(ib_stdout, "           count %d, length %d sub_type %d\n",
				   header->blh_count, header->blh_length,
				   header->blh_sub_type);
	};
#endif

/* Level 0 blobs have no work to do. */

	if (header->blh_level == 0)
		return rtn_ok;

/* Level 1 blobs are a little more complicated */

	pages1 = header->blh_page;
	end1 = pages1 + ((USHORT) (length - BLH_SIZE) >> SHIFTLONG);

	for (sequence = 0; pages1 < end1; pages1++) {
		fetch_page(tdbb, control, *pages1, pag_blob, &window1, &page1);
		if (page1->blp_lead_page != header->blh_lead_page)
			corrupt(tdbb, control, VAL_BLOB_INCONSISTENT, relation, number);
		if ((header->blh_level == 1 && page1->blp_sequence != sequence)) {
			corrupt(tdbb, control, VAL_BLOB_CORRUPT, relation, number);
			CCH_RELEASE(tdbb, &window1);
			return rtn_corrupt;
		}
		if (header->blh_level == 1)
			sequence++;
		else {
			pages2 = page1->blp_page;
			end2 = pages2 + (page1->blp_length >> SHIFTLONG);
			for (; pages2 < end2; pages2++, sequence++) {
				fetch_page(tdbb, control, *pages2, pag_blob, &window2,
						   &page2);
				if (page2->blp_lead_page != header->blh_lead_page
					|| page2->blp_sequence != sequence) {
					corrupt(tdbb, control, VAL_BLOB_CORRUPT, relation,
							number);
					CCH_RELEASE(tdbb, &window1);
					CCH_RELEASE(tdbb, &window2);
					return rtn_corrupt;
				}
				CCH_RELEASE(tdbb, &window2);
			}
		}
		CCH_RELEASE(tdbb, &window1);
	}

	if (sequence - 1 != header->blh_max_sequence)
		return corrupt(tdbb, control, VAL_BLOB_TRUNCATED, relation, number);

	return rtn_ok;
}

static RTN walk_chain(TDBB tdbb,
					  VDR control,
					  JRD_REL relation, RHD header, SLONG head_number)
{
/**************************************
 *
 *	w a l k _ c h a i n
 *
 **************************************
 *
 * Functional description
 *	Make sure chain of record versions is completely intact.
 *
 **************************************/
	DPG page;
	WIN window;
	SLONG page_number;
	USHORT line_number, delta_flag;
	dpg::dpg_repeat * line;
	//USHORT counter = 0;

	SET_TDBB(tdbb);

	page_number = header->rhd_b_page;
	line_number = header->rhd_b_line;

	while (page_number) {
		delta_flag = (header->rhd_flags & rhd_delta) ? TRUE : FALSE;
#ifdef DEBUG_VAL_VERBOSE
		if (VAL_debug_level)
			ib_fprintf(ib_stdout, "  BV %02d: ", ++counter);
#endif
		control->vdr_rel_chain_counter++;
		fetch_page(tdbb, control, page_number, pag_data, &window, &page);
		line = &page->dpg_rpt[line_number];
		header = (RHD) ((UCHAR *) page + line->dpg_offset);
		if (page->dpg_count <= line_number ||
			!line->dpg_length ||
			(header->rhd_flags & (rhd_blob | rhd_fragment)) ||
			walk_record(tdbb, control, relation, header, line->dpg_length,
						head_number, delta_flag) != rtn_ok) {
			CCH_RELEASE(tdbb, &window);
			return corrupt(tdbb, control, VAL_REC_CHAIN_BROKEN,
						   relation, head_number);
		}
		page_number = header->rhd_b_page;
		line_number = header->rhd_b_line;
		CCH_RELEASE(tdbb, &window);
	}

	return rtn_ok;
}

static void walk_database(TDBB tdbb, VDR control)
{
/**************************************
 *
 *	w a l k _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	DBB dbb;
	WIN window;
	HDR page;
	VEC vector;
	JRD_REL relation;
	USHORT i;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;

#ifdef DEBUG_VAL_VERBOSE
	if (VAL_debug_level)
		ib_fprintf(ib_stdout,
				   "walk_database: %s\nODS: %d.%d  (creation ods %d)\nPage size %d\n",
				   dbb->dbb_attachment->att_filename->str_data, dbb->dbb_ods_version,
				   dbb->dbb_minor_version, dbb->dbb_minor_original,
				   dbb->dbb_page_size);
#endif

	DPM_scan_pages(tdbb);
	fetch_page(tdbb, control, (SLONG) HEADER_PAGE, pag_header, &window,
			   &page);
	control->vdr_max_transaction = page->hdr_next_transaction;

	walk_header(tdbb, control, page->hdr_next_page);
	walk_log(tdbb, control);
	walk_pip(tdbb, control);
	walk_tip(tdbb, control, page->hdr_next_transaction);
	walk_generators(tdbb, control);

	for (i = 0; (vector = dbb->dbb_relations) && i < vector->count(); i++) {
#ifdef DEBUG_VAL_VERBOSE
		if (i >= 32 /* rel_MAX */ )
			VAL_debug_level = 2;
#endif
		if ( (relation = (JRD_REL) (*vector)[i]) )
			walk_relation(tdbb, control, relation);
	}

	CCH_RELEASE(tdbb, &window);
}

static RTN walk_data_page(TDBB tdbb,
						  VDR control,
						  JRD_REL relation, SLONG page_number, SLONG sequence)
{
/**************************************
 *
 *	w a l k _ d a t a _ p a g e
 *
 **************************************
 *
 * Functional description
 *	Walk a single data page.
 *
 **************************************/
	DBB dbb;
	WIN window;
	DPG page;
	RHD header;
	RTN result;
	UCHAR *end_page;
	SLONG number;
	int state;
	dpg::dpg_repeat * line, *end;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;

	fetch_page(tdbb, control, page_number, pag_data, &window, &page);

#ifdef DEBUG_VAL_VERBOSE
	if (VAL_debug_level)
		ib_fprintf(ib_stdout,
				   "walk_data_page: page %d rel %d seq %d count %d\n",
				   page_number, page->dpg_relation, page->dpg_sequence,
				   page->dpg_count);
#endif

	if (page->dpg_relation != relation->rel_id
		|| page->dpg_sequence != sequence) {
		++control->vdr_errors;
		CCH_RELEASE(tdbb, &window);
		return corrupt(tdbb, control, VAL_DATA_PAGE_CONFUSED,
					   relation, page_number, sequence);
	}

/* Walk records */

	end_page = (UCHAR *) page + dbb->dbb_page_size;
	end = page->dpg_rpt + page->dpg_count;
	number = sequence * dbb->dbb_max_records;

	for (line = page->dpg_rpt; line < end; line++, number++) {
#ifdef DEBUG_VAL_VERBOSE
		if (VAL_debug_level)
			ib_fprintf(ib_stdout, "Slot %02d (%d,%d): ",
					   line - page->dpg_rpt,
					   line->dpg_offset, line->dpg_length);
#endif
		if (line->dpg_length) {
			header = (RHD) ((UCHAR *) page + line->dpg_offset);
			if ((UCHAR *) header < (UCHAR *) end ||
				(UCHAR *) header + line->dpg_length > end_page)
				return corrupt(tdbb, control, VAL_DATA_PAGE_LINE_ERR,
							   relation, page_number, sequence,
							   (SLONG) (line - page->dpg_rpt));
			if (header->rhd_flags & rhd_chain)
				control->vdr_rel_backversion_counter++;

			/* Record the existance of a primary version of a record */

			if ((control->vdr_flags & vdr_records) &&
				!(header->rhd_flags & (rhd_chain | rhd_fragment | rhd_blob))) {
				/* Only set committed (or limbo) records in the bitmap. If there
				   is a backversion then at least one of the record versions is
				   committed. If there's no backversion then check transaction
				   state of the lone primary record version. */

				if (header->rhd_b_page)
					SBM_set(tdbb, &control->vdr_rel_records, number);
				else {
					if (header->rhd_transaction < dbb->dbb_oldest_transaction)
						state = tra_committed;
					else
						state =
							TRA_fetch_state(tdbb, header->rhd_transaction);
					if (state == tra_committed || state == tra_limbo)
						SBM_set(tdbb, &control->vdr_rel_records, number);
				}
			}

#ifdef DEBUG_VAL_VERBOSE
			if (VAL_debug_level) {
				if (header->rhd_flags & rhd_chain)
					ib_fprintf(ib_stdout, "(backvers)");
				if (header->rhd_flags & rhd_fragment)
					ib_fprintf(ib_stdout, "(fragment)");
				if (header->rhd_flags & (rhd_fragment | rhd_chain))
					print_rhd(line->dpg_length, header);
			}
#endif
			if (!(header->rhd_flags & rhd_chain) &&
				((header->rhd_flags & rhd_large) ||
				 (control->vdr_flags & vdr_records))) {
				result = (header->rhd_flags & rhd_blob) ?
					walk_blob(tdbb, control, relation, (BLH) header,
							  line->dpg_length, number) :
					walk_record(tdbb, control, relation, header,
								line->dpg_length, number, FALSE);
				if ((result == rtn_corrupt)
					&& (control->vdr_flags & vdr_repair)) {
					CCH_MARK(tdbb, &window);
					header->rhd_flags |= rhd_damaged;
				}
			}
		}
#ifdef DEBUG_VAL_VERBOSE
		else if (VAL_debug_level)
			ib_fprintf(ib_stdout, "(empty)\n");
#endif
	}

	CCH_RELEASE(tdbb, &window);

#ifdef DEBUG_VAL_VERBOSE
	if (VAL_debug_level)
		ib_fprintf(ib_stdout, "------------------------------------\n");
#endif

	return rtn_ok;
}

static void walk_generators(TDBB tdbb, VDR control)
{
/**************************************
 *
 *	w a l k _ g e n e r a t o r s
 *
 **************************************
 *
 * Functional description
 *	Walk the page inventory pages.
 *
 **************************************/
	DBB dbb;
	WIN window;
	VCL vector;
	PPG page;
	vcl::iterator ptr, end;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	if ( (vector = dbb->dbb_gen_id_pages) )
		for (ptr = vector->begin(), end = vector->end(); ptr < end;
			 ptr++)
			if (*ptr) {
#ifdef DEBUG_VAL_VERBOSE
				if (VAL_debug_level)
					ib_fprintf(ib_stdout, "walk_generator: page %d\n", *ptr);
#endif
				fetch_page(tdbb, control, *ptr, pag_ids, &window, &page);
				CCH_RELEASE(tdbb, &window);
			}
}

static void walk_header(TDBB tdbb, VDR control, SLONG page_num)
{
/**************************************
 *
 *	w a l k _ h e a d e r
 *
 **************************************
 *
 * Functional description
 *	Walk the overflow header pages
 *
 **************************************/
	WIN window;
	HDR page;

	SET_TDBB(tdbb);

	while (page_num) {
#ifdef DEBUG_VAL_VERBOSE
		if (VAL_debug_level)
			ib_fprintf(ib_stdout, "walk_header: page %d\n", page_num);
#endif
		fetch_page(tdbb, control, page_num, pag_header, &window, &page);
		page_num = page->hdr_next_page;
		CCH_RELEASE(tdbb, &window);
	}
}

static RTN walk_index(TDBB tdbb,
					  VDR control, JRD_REL relation, SLONG page_number, USHORT id)
{
/**************************************
 *
 *	w a l k _ i n d e x
 *
 **************************************
 *
 * Functional description
 *	Walk all btree pages left-to-right and top-down.
 *	Check all the pointers and keys for consistency 
 *	relative to each other, and check sibling pointers. 
 *
 *	NOTE: id is the internal index id, relative for each 
 *	relation.  It is 1 less than the user level index id.
 *	So errors are reported against index id+1
 *
 **************************************/
	DBB dbb;
	WIN window, down_window;
	BTR page, down_page;
	BTN node, end, down_node;
	SLONG next, down, down_number, previous_number, next_number;
	KEY key;
	UCHAR *p, *q, l;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	next = down = page_number;
	key.key_length = 0;
	previous_number = 0;

	if (control)
		SBM_reset(&control->vdr_idx_records);

	while (next) {
		fetch_page(tdbb, control, next, pag_index, &window, &page);

		if (page->btr_relation != relation->rel_id || page->btr_id != (UCHAR) (id % 256)) 
		{
			corrupt(tdbb, control, VAL_INDEX_PAGE_CORRUPT, relation, id + 1,
					next);
			CCH_RELEASE(tdbb, &window);
			return rtn_corrupt;
		}

		/* go through all the nodes on the page and check for validity */

		for (node = (BTN) page->btr_nodes, end =
			 (BTN) ((UCHAR *) page + page->btr_length); node < end;
			 node = NEXT_NODE(node)) {
			/* make sure the current key is not less than the previous key */

			q = BTN_DATA(node);
			p = key.key_data + BTN_PREFIX(node);
			for (l =
				 MIN(BTN_LENGTH(node),
					 (UCHAR) (key.key_length - BTN_PREFIX(node))); l;
				 l--, p++, q++)
				if (*p > *q)
					corrupt(tdbb, control, VAL_INDEX_PAGE_CORRUPT, relation,
							id + 1, next);
				else if (*p < *q)
					break;

			/* save the current key */

			q = BTN_DATA(node);
			p = key.key_data + BTN_PREFIX(node);
			for (l = BTN_LENGTH(node); l; l--)
				*p++ = *q++;
			key.key_length = p - key.key_data;

			if (QUAD_EQUAL(BTN_NUMBER(node), &end_level) ||
				QUAD_EQUAL(BTN_NUMBER(node), &end_bucket)) {
				node = NEXT_NODE(node);
				break;
			}

			/* Record the existance of a primary version of a record */
			if (!page->btr_level && control
				&& (control->vdr_flags & vdr_records)) {
			  SBM_set(tdbb, &control->vdr_idx_records, get_long(BTN_NUMBER(node)));
		    }

			/* fetch the next page down (if full validation was specified) */

			if (page->btr_level && control
				&& (control->vdr_flags & vdr_records)) {
				down_number = get_long(BTN_NUMBER(node));

				/* Note: control == 0 for the fetch_page() call here 
				   as we don't want to mark the page as visited yet - we'll 
				   mark it when we visit it for real later on */

				fetch_page(tdbb, 0, down_number, pag_index, &down_window,
						   &down_page);

				/* make sure the initial key is greater than the pointer key */

				down_node = (BTN) down_page->btr_nodes;
				p = BTN_DATA(down_node);
				q = key.key_data;
				for (l = static_cast<UCHAR>(MIN(key.key_length,
											BTN_LENGTH(down_node)));
					l; l--, p++, q++)
				{
					if (*p < *q)
					{
						corrupt(tdbb, control, VAL_INDEX_PAGE_CORRUPT,
								relation, id + 1, next);
					}
					else if (*p > *q) {
						break;
					}
				}

				/* check the left and right sibling pointers against the parent pointers */

				if (previous_number != down_page->btr_left_sibling)
					corrupt(tdbb, control, VAL_INDEX_PAGE_CORRUPT, relation,
							id + 1, next);
				next_number = get_long(BTN_NUMBER((NEXT_NODE(node))));
				if (next_number >= 0 && next_number != down_page->btr_sibling)
					corrupt(tdbb, control, VAL_INDEX_PAGE_CORRUPT, relation,
							id + 1, next);
				if (next_number == END_LEVEL && down_page->btr_sibling)
					corrupt(tdbb, control, VAL_INDEX_ORPHAN_CHILD, relation,
							id + 1, next);
				previous_number = down_number;

				CCH_RELEASE(tdbb, &down_window);
			}
		}

		if (node != end || page->btr_length > dbb->dbb_page_size)
			corrupt(tdbb, control, VAL_INDEX_PAGE_CORRUPT, relation, id + 1,
					next);

		if (next == down)
			down =
				(page->btr_level) ? get_long(page->btr_nodes[0].btn_number) : 0;

		if (!(next = page->btr_sibling)) {
			next = down;
			key.key_length = 0;
			previous_number = 0;
		}

		CCH_RELEASE(tdbb, &window);
	}

/* If the index & relation contain different sets of records we
   have a corrupt index */

	if (control && (control->vdr_flags & vdr_records)) {
		THREAD_EXIT;
		next_number = -1;
		while (SBM_next(control->vdr_rel_records, &next_number,
						RSE_get_forward))
		{
			if (!SBM_test(control->vdr_idx_records, next_number)) {
				THREAD_ENTER;
				return corrupt(tdbb, control, VAL_INDEX_MISSING_ROWS,
							   relation, id + 1);
			}
		}
		THREAD_ENTER;
	}

	return rtn_ok;
}

static void walk_log(TDBB tdbb, VDR control)
{
/**************************************
 *
 *	w a l k _ l o g
 *
 **************************************
 *
 * Functional description
 *	Walk the log and overflow pages
 *
 **************************************/
	WIN window;
	LIP page;
	SLONG page_num = LOG_PAGE;

	SET_TDBB(tdbb);

	while (page_num) {
		fetch_page(tdbb, control, page_num, pag_log, &window, &page);
		page_num = page->log_next_page;
		CCH_RELEASE(tdbb, &window);
	}
}

static void walk_pip(TDBB tdbb, VDR control)
{
/**************************************
 *
 *	w a l k _ p i p
 *
 **************************************
 *
 * Functional description
 *	Walk the page inventory pages.
 *
 **************************************/
	DBB dbb;
	WIN window;
	PIP page;
	PGC pgc;
	USHORT sequence;
	UCHAR byte;
	SLONG page_number;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	pgc = dbb->dbb_pcontrol;

	for (sequence = 0;; sequence++) {
		page_number = (sequence) ? sequence * pgc->pgc_ppp - 1 : pgc->pgc_pip;
#ifdef DEBUG_VAL_VERBOSE
		if (VAL_debug_level)
			ib_fprintf(ib_stdout, "walk_pip: page %d\n", page_number);
#endif
		fetch_page(tdbb, control, page_number, pag_pages, &window, &page);
		byte = page->pip_bits[pgc->pgc_bytes - 1];
		CCH_RELEASE(tdbb, &window);
		if (byte & 0x80)
			break;
	}
}

static RTN walk_pointer_page(	TDBB	tdbb,
								VDR		control,
								JRD_REL		relation,
								int		sequence)
{
/**************************************
 *
 *	w a l k _ p o i n t e r _ p a g e
 *
 **************************************
 *
 * Functional description
 *	Walk a pointer page for a relation.  Return TRUE if there are more
 *	to go.
 *
 **************************************/
	DBB dbb;
	WIN window;
	PPG page;
	RTN result;
	USHORT slot;
	SLONG *pages, seq;

	SET_TDBB(tdbb);

	dbb = tdbb->tdbb_database;

	VCL vector = relation->rel_pages;

	if (!vector || sequence >= static_cast<int>(vector->count())) {
		return corrupt(tdbb, control, VAL_P_PAGE_LOST, relation, sequence);
	}

	fetch_page(	tdbb,
				control,
				(*vector)[sequence],
				pag_pointer,
				&window,
				&page);

#ifdef DEBUG_VAL_VERBOSE
	if (VAL_debug_level)
		ib_fprintf(ib_stdout,
				   "walk_pointer_page: page %d relation %d sequence %d\n",
				   (*vector)[sequence], relation->rel_id, sequence);
#endif

/* Give the page a quick once over */

	if (page->ppg_relation != relation->rel_id ||
		page->ppg_sequence != sequence)
			return corrupt(tdbb, control, VAL_P_PAGE_INCONSISTENT, relation,
						   sequence);

/* Walk the data pages (someday we may optionally walk pages with "large objects" */

	seq = (SLONG) sequence *dbb->dbb_dp_per_pp;

	for (pages = page->ppg_page, slot = 0; slot < page->ppg_count;
		 slot++, pages++, seq++)
		if (*pages) {
			result = walk_data_page(tdbb, control, relation, *pages, seq);
			if (result != rtn_ok && (control->vdr_flags & vdr_repair)) {
				CCH_MARK(tdbb, &window);
				*pages = 0;
			}
		}

/* If this is the last pointer page in the relation, we're done */

	if (page->ppg_header.pag_flags & ppg_eof) {
		CCH_RELEASE(tdbb, &window);
		return rtn_eof;
	}

/* Make sure the "next" pointer agrees with the pages relation */

	if (++sequence >= static_cast<int>(vector->count()) ||
		(page->ppg_next && page->ppg_next != (*vector)[sequence]))
	{
		CCH_RELEASE(tdbb, &window);
		return corrupt(	tdbb,
						control,
						VAL_P_PAGE_INCONSISTENT,
						relation,
						sequence);
	}

	CCH_RELEASE(tdbb, &window);
	return rtn_ok;
}


static RTN walk_record(TDBB tdbb,
					   VDR control,
					   JRD_REL relation,
					   RHD header,
					   USHORT length, SLONG number, USHORT delta_flag)
{
/**************************************
 *
 *	w a l k _ r e c o r d
 *
 **************************************
 *
 * Functional description
 *	Walk a record.
 *
 **************************************/
	WIN window;
	DPG page;
	RHDF fragment;
	USHORT record_length, line_number;
	SLONG page_number;
	FMT format;
	UCHAR flags;
	SCHAR c, *p, *end;
	RTN result;
	dpg::dpg_repeat * line;

	SET_TDBB(tdbb);

#ifdef DEBUG_VAL_VERBOSE
	if (VAL_debug_level) {
		ib_fprintf(ib_stdout, "record: number %ld (%d/%d) ",
				   number,
				   (USHORT) number / tdbb->tdbb_database->dbb_max_records,
				   (USHORT) number % tdbb->tdbb_database->dbb_max_records);
		print_rhd(length, header);
	};
#endif

	if (header->rhd_flags & rhd_damaged) {
		corrupt(tdbb, control, VAL_REC_DAMAGED, relation, number);
		return rtn_ok;
	}

	if (control && header->rhd_transaction > control->vdr_max_transaction)
		corrupt(tdbb, control, VAL_REC_BAD_TID, relation, number,
				header->rhd_transaction);

/* If there's a back pointer, verify that it's good */

	if (header->rhd_b_page && !(header->rhd_flags & rhd_chain)) {
		result = walk_chain(tdbb, control, relation, header, number);
		if (result != rtn_ok)
			return result;
	}

/* If the record is a fragment, not large, or we're not interested in
   chasing records, skip the record */

	if (header->rhd_flags & (rhd_fragment | rhd_deleted) ||
		!((header->rhd_flags & rhd_large) ||
		  (control && (control->vdr_flags & vdr_records)))) return rtn_ok;

/* Pick up what length there is on the fragment */

	fragment = (RHDF) header;

	if (header->rhd_flags & rhd_incomplete) {
		p = (SCHAR *) fragment->rhdf_data;
		end = p + length - OFFSETA(RHDF, rhdf_data);
	}
	else {
		p = (SCHAR *) header->rhd_data;
		end = p + length - OFFSETA(RHD, rhd_data);
	}

	record_length = 0;

	while (p < end)
		if ((c = *p++) >= 0) {
			record_length += c;
			p += c;
		}
		else {
			record_length -= c;
			p++;
		}

/* Next, chase down fragments, if any */

	page_number = fragment->rhdf_f_page;
	line_number = fragment->rhdf_f_line;
	flags = fragment->rhdf_flags;

	while (flags & rhd_incomplete) {
		fetch_page(tdbb, control, page_number, pag_data, &window, &page);
		line = &page->dpg_rpt[line_number];
		if (page->dpg_relation != relation->rel_id ||
			line_number >= page->dpg_count || !(length = line->dpg_length)) {
			corrupt(tdbb, control, VAL_REC_FRAGMENT_CORRUPT, relation,
					number);
			CCH_RELEASE(tdbb, &window);
			return rtn_corrupt;
		}
		fragment = (RHDF) ((UCHAR *) page + line->dpg_offset);
#ifdef DEBUG_VAL_VERBOSE
		if (VAL_debug_level) {
			ib_fprintf(ib_stdout, "fragment: pg %d/%d ",
					   page_number, line_number);
			print_rhd(line->dpg_length, (RHD) fragment);
		}
#endif
		if (fragment->rhdf_flags & rhd_incomplete) {
			p = (SCHAR *) fragment->rhdf_data;
			end = p + line->dpg_length - OFFSETA(RHDF, rhdf_data);
		}
		else {
			p = (SCHAR *) ((RHD) fragment)->rhd_data;
			end = p + line->dpg_length - OFFSETA(RHD, rhd_data);
		}
		while (p < end)
			if ((c = *p++) >= 0) {
				record_length += c;
				p += c;
			}
			else {
				record_length -= c;
				p++;
			}
		page_number = fragment->rhdf_f_page;
		line_number = fragment->rhdf_f_line;
		flags = fragment->rhdf_flags;
		CCH_RELEASE(tdbb, &window);
	}

/* Check out record length and format */

	format = MET_format(tdbb, relation, header->rhd_format);

	if (!delta_flag && record_length != format->fmt_length)
		return corrupt(tdbb, control, VAL_REC_WRONG_LENGTH, relation, number);

	return rtn_ok;
}


static RTN walk_relation(TDBB tdbb, VDR control, JRD_REL relation)
{
/**************************************
 *
 *	w a l k _ r e l a t i o n
 *
 **************************************
 *
 * Functional description
 *	Walk all pages associated with a given relation.
 *
 **************************************/

	SET_TDBB(tdbb);

	try {

	// If relation hasn't been scanned, do so now

	if (!(relation->rel_flags & REL_scanned) ||
		(relation->rel_flags & REL_being_scanned))
			MET_scan_relation(tdbb, relation);

#ifdef DEBUG_VAL_VERBOSE
	if (VAL_debug_level)
		ib_fprintf(ib_stdout, "walk_relation: id %d fmt %d %s %s\n",
				   relation->rel_id, relation->rel_current_fmt,
				   relation->rel_name, relation->rel_owner_name);
#endif

/* If it's a view or external file, skip this */

	if (relation->rel_view_rse || relation->rel_file) {
		return rtn_ok;
	}


/* Walk pointer and selected data pages associated with relation */

	if (control) {
		control->vdr_rel_backversion_counter = 0;
		control->vdr_rel_chain_counter = 0;
		SBM_reset(&control->vdr_rel_records);
	}
	for (SLONG sequence = 0; TRUE; sequence++) {
		RTN result;
		result = walk_pointer_page(tdbb, control, relation, sequence);
		if (result == rtn_eof) {
			break;
		}
		if (result != rtn_ok) {
			return result;
		}
	}

	// Walk indices for the relation
	walk_root(tdbb, control, relation);

	// See if the counts of backversions match
	if (control && (control->vdr_flags & vdr_records) &&
		(control->vdr_rel_backversion_counter !=
		 control->vdr_rel_chain_counter))
	{
		 return corrupt(tdbb,
						control,
						VAL_REL_CHAIN_ORPHANS,
						relation,
						control->vdr_rel_backversion_counter - control-> vdr_rel_chain_counter,
						control-> vdr_rel_chain_counter);
	}

	}	// try
	catch (const std::exception&) {
		TEXT s[64];
		TEXT* msg = (relation->rel_name) ?
			(TEXT*)"bugcheck during scan of table %d (%s)" :
			(TEXT*)"bugcheck during scan of table %d";
		sprintf(s, msg, relation->rel_id, relation->rel_name);
		gds__log(s);
#ifdef DEBUG_VAL_VERBOSE
		if (VAL_debug_level)
			ib_fprintf(ib_stdout, "LOG:\t%s\n", s);
#endif
		ERR_punt();
	}

	return rtn_ok;
}


static RTN walk_root(TDBB tdbb, VDR control, JRD_REL relation)
{
/**************************************
 *
 *	w a l k _ r o o t
 *
 **************************************
 *
 * Functional description
 *	Walk index root page for a relation as well as any indices.
 *
 **************************************/
	WIN window;
	IRT page;
	SLONG page_number;
	USHORT i;

	SET_TDBB(tdbb);

/* If the relation has an index root, walk it */

	if (!relation->rel_index_root)
		return corrupt(tdbb, control, VAL_INDEX_ROOT_MISSING, relation);

	fetch_page(tdbb, control, relation->rel_index_root, pag_root, &window,
			   &page);

	for (i = 0; i < page->irt_count; i++)
		if ( (page_number = page->irt_rpt[i].irt_root) )
			walk_index(tdbb, control, relation, page_number, i);

	CCH_RELEASE(tdbb, &window);

	return rtn_ok;
}

static RTN walk_tip(TDBB tdbb, VDR control, SLONG transaction)
{
/**************************************
 *
 *	w a l k _ t i p
 *
 **************************************
 *
 * Functional description
 *	Walk transaction inventory pages.
 *
 **************************************/
	DBB dbb;
	WIN window;
	TIP page;
	VCL vector;
	ULONG sequence, pages;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);


	if (!(vector = dbb->dbb_t_pages)) {
		return corrupt(tdbb, control, VAL_TIP_LOST, 0);
	}

	pages = transaction / dbb->dbb_pcontrol->pgc_tpt;

	for (sequence = 0; sequence <= pages; sequence++) {
		if (!(*vector)[sequence] || sequence >= vector->count()) {
			corrupt(tdbb, control, VAL_TIP_LOST_SEQUENCE, 0, sequence);
			if (!(control->vdr_flags & vdr_repair))
				continue;
			TRA_extend_tip(tdbb, sequence, 0);
			vector = dbb->dbb_t_pages;
		}
		fetch_page(	tdbb,
					control,
					(*vector)[sequence],
					pag_transactions,
					&window,
					&page);

#ifdef DEBUG_VAL_VERBOSE
		if (VAL_debug_level)
			ib_fprintf(ib_stdout, "walk_tip: page %d next %d\n",
					   (*vector)[sequence], page->tip_next);
#endif
		if (page->tip_next && page->tip_next != (*vector)[sequence + 1])
		{
			corrupt(tdbb, control, VAL_TIP_CONFUSED, 0, sequence);
		}
		CCH_RELEASE(tdbb, &window);
	}

	return rtn_ok;
}
