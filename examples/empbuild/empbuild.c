/*********** Preprocessed module -- do not edit ***************/
/*********** Preprocessed module -- do not edit ***************/
/*********** Preprocessed module -- do not edit ***************/
/*********** Preprocessed module -- do not edit ***************/
/*********** Preprocessed module -- do not edit ***************/
/***************** gpre version WI-T1.5.0.3656 Firebird 1.5 Release Candidate 4 **********************/
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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "../jrd/common.h"

#include "gds.h"

/* Some #defines that are used in the program - they actually come from
   jrd/common.h but should not be exposed externally with those name so 
   are reproduced here MOD 15-07-2001
*/
/* typedef char TEXT; */
#define FINI_OK 0
#define FINI_ERROR 44

/* 
**  Empbuild.e   GPRE with manual switch, since it creates the database 
**		This program then calls isql with various input files
**		It installs the blobs and arrays.
**		Usage:  empbuild <db name>
*/

static int	addlang (void);
static int	addjob (void);
static int	addproj (void);
static int	addqtr (void);

static TEXT	Db_name[128];
static FILE	*Fp;

/*EXEC SQL INCLUDE SQLCA;*/
/**** GDS Preprocessor Definitions ****/
#ifndef JRD_IBASE_H
#include <ibase.h>
#endif

static const ISC_QUAD
   isc_blob_null = {0,0};	/* initializer for blobs */
static const long *gds__null = 0;	/* dummy status vector */
isc_db_handle
   DB = 0;		/* database handle */

isc_tr_handle
   gds__trans = 0;		/* default transaction handle */
long
   isc_status [20],	/* status vector */
   isc_status2 [20];	/* status vector */
long
   isc_array_length, 	/* array return size */
   SQLCODE;		/* SQL status code */
static const char
   isc_tpb_6 [4] = {1,9,2,6};

static const char
   isc_tpb_5 [4] = {1,9,2,6};

static const char
   isc_tpb_4 [4] = {1,9,2,6};

static const char
   isc_tpb_1 [4] = {1,9,2,6};

static const char
   isc_tpb_0 [4] = {1,9,2,6};

static isc_req_handle
   isc_9 = 0;		/* request handle */

static const short
   isc_10l = 163;
static const char
   isc_10 [] = {
   4,2,4,1,1,0,8,0,4,0,4,0,9,0,40,4,0,8,0,40,6,0,12,0,14,1,2,1,
   21,8,0,0,0,0,0,25,1,0,0,7,'C',1,'J',16,'P','R','O','J','_','D',
   'E','P','T','_','B','U','D','G','E','T',0,'G',58,47,23,0,11,
   'F','I','S','C','A','L','_','Y','E','A','R',25,0,2,0,58,47,23,
   0,7,'P','R','O','J','_','I','D',25,0,3,0,47,23,0,7,'D','E','P',
   'T','_','N','O',25,0,1,0,-1,2,10,0,1,2,1,25,0,0,0,23,1,14,'Q',
   'U','A','R','T','_','H','E','A','D','_','C','N','T',-1,1,34,
   21,8,0,1,0,0,0,25,1,0,0,25,1,0,0,-1,-1,-1,'L'
   };	/* end of blr string for request isc_10 */

static const short
   isc_18l = 51;
static const char
   isc_18 [] = {
   1,6,1,8,0,2,16,'P','R','O','J','_','D','E','P','T','_','B','U',
   'D','G','E','T',4,14,'Q','U','A','R','T','_','H','E','A','D',
   '_','C','N','T',35,0,9,4,'$',1,8,0,1,7,0,-1
   };	/* end of sdl string for request isc_18 */

static isc_req_handle
   isc_20 = 0;		/* request handle */

static const short
   isc_21l = 108;
static const char
   isc_21 [] = {
   4,2,4,1,1,0,8,0,4,0,2,0,9,0,40,6,0,12,0,14,1,2,1,21,8,0,0,0,
   0,0,25,1,0,0,7,'C',1,'J',7,'P','R','O','J','E','C','T',0,'G',
   47,23,0,7,'P','R','O','J','_','I','D',25,0,1,0,-1,2,10,0,1,2,
   1,25,0,0,0,23,1,9,'P','R','O','J','_','D','E','S','C',-1,1,34,
   21,8,0,1,0,0,0,25,1,0,0,25,1,0,0,-1,-1,-1,'L'
   };	/* end of blr string for request isc_21 */

static isc_stmt_handle
   isc_28s;		/* sql statement handle */
static isc_req_handle
   isc_34 = 0;		/* request handle */

static const short
   isc_35l = 154;
static const char
   isc_35 [] = {
   4,2,4,1,1,0,8,0,4,0,4,0,40,16,0,9,0,7,0,40,6,0,12,0,14,1,2,1,
   21,8,0,0,0,0,0,25,1,0,0,7,'C',1,'J',3,'J','O','B',0,'G',58,47,
   23,0,8,'J','O','B','_','C','O','D','E',25,0,3,0,58,47,23,0,9,
   'J','O','B','_','G','R','A','D','E',25,0,2,0,47,23,0,11,'J',
   'O','B','_','C','O','U','N','T','R','Y',25,0,0,0,-1,2,10,0,1,
   2,1,25,0,1,0,23,1,15,'J','O','B','_','R','E','Q','U','I','R',
   'E','M','E','N','T',-1,1,34,21,8,0,1,0,0,0,25,1,0,0,25,1,0,0,
   -1,-1,-1,'L'
   };	/* end of blr string for request isc_35 */

static isc_stmt_handle
   isc_44s;		/* sql statement handle */
static isc_req_handle
   isc_50 = 0;		/* request handle */

static const short
   isc_51l = 151;
static const char
   isc_51 [] = {
   4,2,4,1,1,0,8,0,4,0,4,0,40,16,0,9,0,7,0,40,6,0,12,0,14,1,2,1,
   21,8,0,0,0,0,0,25,1,0,0,7,'C',1,'J',3,'J','O','B',0,'G',58,47,
   23,0,8,'J','O','B','_','C','O','D','E',25,0,3,0,58,47,23,0,9,
   'J','O','B','_','G','R','A','D','E',25,0,2,0,47,23,0,11,'J',
   'O','B','_','C','O','U','N','T','R','Y',25,0,0,0,-1,2,10,0,1,
   2,1,25,0,1,0,23,1,12,'L','A','N','G','U','A','G','E','_','R',
   'E','Q',-1,1,34,21,8,0,1,0,0,0,25,1,0,0,25,1,0,0,-1,-1,-1,'L'
   
   };	/* end of blr string for request isc_51 */

static const short
   isc_59l = 37;
static const char
   isc_59 [] = {
   1,6,1,40,16,0,2,3,'J','O','B',4,12,'L','A','N','G','U','A','G',
   'E','_','R','E','Q',35,0,9,5,'$',1,8,0,1,7,0,-1
   };	/* end of sdl string for request isc_59 */


#define gds__blob_null	isc_blob_null	/* compatibility symbols */
#define gds__status	isc_status
#define gds__status2	isc_status2
#define gds__array_length	isc_array_length
#define gds__count	isc_count
#define gds__slack	isc_slack
#define gds__utility	isc_utility	/* end of compatibility symbols */

#ifndef isc_version4
    Generate a compile-time error.
    Picking up a V3 include file after preprocessing with V4 GPRE.
#endif

/**** end of GPRE definitions ****/


/*EXEC SQL SET DATABASE DB = COMPILETIME "empbuild.fdb" RUNTIME :Db_name;*/

int main (
    int		argc,
    char	*argv[])
{
/**************************************
 *
 *	m a i n
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
TEXT	cmd [140];

if (argc > 1)
    strcpy (Db_name, argv[1]);
else
    strcpy (Db_name, "employee.fdb");

/* Create the database */

printf ("creating database %s\n", Db_name);
sprintf (cmd, "CREATE DATABASE \"%s\"", Db_name);
gds__trans = 0;

/*EXEC SQL EXECUTE IMMEDIATE :cmd;*/
{
isc_embed_dsql_execute_immed (isc_status, &DB, &gds__trans, 0, cmd, 1, (XSQLDA*) 0L);
SQLCODE = isc_sqlcode (isc_status);
}
if (SQLCODE)
    {
    isc_print_status (gds__status);
    exit (FINI_ERROR);
    }

gds__trans = 0;

/*EXEC SQL DISCONNECT ALL;*/
{
if (DB)
   isc_detach_database (isc_status, &DB);
SQLCODE = isc_sqlcode (isc_status);
}
if (SQLCODE)
    {
    isc_print_status (gds__status);
    exit (FINI_ERROR);
    }

printf ("Creating tables\n");
sprintf (cmd, "isql %s -q -i empddl.sql", Db_name);
if (system (cmd))
    {
    printf ("Couldn't create tables \n");
    exit (FINI_ERROR);
    }

printf ("Turning  off indices and triggers \n");
sprintf (cmd, "isql %s -i indexoff.sql", Db_name);
system (cmd);
printf ("Loading  column data\n");
sprintf (cmd, "isql %s -i empdml.sql", Db_name);
system (cmd);
printf ("Turning  on indices and triggers \n");
sprintf (cmd, "isql %s -i indexon.sql", Db_name);
system (cmd);

/*EXEC SQL CONNECT DB;*/
{
isc_attach_database (isc_status, 0, Db_name, &DB, 0, (char*) 0);
SQLCODE = isc_sqlcode (isc_status);
}
if (SQLCODE)
    {
    isc_print_status (gds__status);
    exit (FINI_ERROR);
    }

/*EXEC SQL SET TRANSACTION;*/
{
isc_start_transaction (isc_status, (FRBRD**) &gds__trans, (short) 1, &DB, (short) 4, isc_tpb_0);
SQLCODE = isc_sqlcode (isc_status);
}

printf ("Loading Language blobs\n");
addlang();
printf ("Loading Job blobs\n");
addjob();
printf ("Loading project blobs \n");
addproj();
printf ("Loading quarter arrays \n");
addqtr();

exit (FINI_OK);
}

static int addlang (void)
{
   struct {
          long isc_58;	/* isc_count */
   } isc_57;
   struct {
          char  isc_53 [16];	/* JOB_COUNTRY */
          ISC_QUAD isc_54;	/* LANGUAGE_REQ */
          short isc_55;	/* JOB_GRADE */
          char  isc_56 [6];	/* JOB_CODE */
   } isc_52;
/**************************************
 *
 *	a d d l a n g
 *
 **************************************
 *
 * Functional description
 *	Add language array to 'job' table.
 *
 **************************************/
TEXT	job_code[6], job_country[16];
TEXT	line[81];
TEXT	lang_array[5][16];
int	i, job_grade, rec_cnt = 0;

/*EXEC SQL SET TRANSACTION;*/
{
isc_start_transaction (isc_status, (FRBRD**) &gds__trans, (short) 1, &DB, (short) 4, isc_tpb_1);
SQLCODE = isc_sqlcode (isc_status);
}
/*EXEC SQL WHENEVER SQLERROR GO TO Error;*/

Fp = fopen ("lang.inp", "r");
if (Fp == NULL){
	printf ("lang.inp not found\n");
	exit(FINI_ERROR);
	}

while (fgets (line, 100, Fp) != NULL)
    {
    sscanf (line, "%s %d %s", job_code, &job_grade, job_country);

    for (i = 0; i < 5; i++)
	{
	if (fgets (line, 100, Fp) == NULL)
	    break;
	strcpy (lang_array [i], line);
	}
		
    /*EXEC SQL
	UPDATE job
	    SET language_req = :lang_array
	    WHERE job_code = :job_code AND
		job_grade = :job_grade AND
		job_country = :job_country;*/
    {
    if (!isc_50)
       isc_compile_request2 (isc_status, (FRBRD**) &DB, (FRBRD**) &isc_50, (short) sizeof (isc_51), (char *) isc_51);
    isc_vtov ((char*)job_country, (char*)isc_52.isc_53, 16);
    isc_52.isc_54 = isc_blob_null;
    isc_put_slice (isc_status, &DB, &gds__trans, &isc_52.isc_54, (short) 37, (char *) isc_59, 0, (long*) 0, (long) 80, (void *)lang_array);
    SQLCODE = isc_sqlcode (isc_status);
    if (SQLCODE < 0) goto Error;
    isc_52.isc_55 = job_grade;
    isc_vtov ((char*)job_code, (char*)isc_52.isc_56, 6);
    if (isc_50)
       isc_start_and_send (isc_status, (FRBRD**) &isc_50, (FRBRD**) &gds__trans, (short) 0, (short) 32, &isc_52, (short) 0);
    SQLCODE = isc_sqlcode (isc_status);
    if (!SQLCODE) 
       {
       isc_receive (isc_status, (FRBRD**) &isc_50, (short) 1, (short) 4, &isc_57, (short) 0);
       SQLCODE = isc_sqlcode (isc_status);
       if (!SQLCODE && !isc_57.isc_58)
	  SQLCODE = 100;
       }
    if (SQLCODE < 0) goto Error;
    }

    if (SQLCODE == 0)
	rec_cnt++;
    else
	{
	printf ("Input error -- no job record with key: %s %d %s\n",
	    job_code, job_grade, job_country);
	}
    }

/*EXEC SQL COMMIT;*/
{
isc_commit_transaction (isc_status, (FRBRD**) &gds__trans);
SQLCODE = isc_sqlcode (isc_status);
if (SQLCODE < 0) goto Error;
}
printf ("Added %d language arrays.\n", rec_cnt);
fclose (Fp);

return (0);
		
Error:

printf ("SQLCODE=%ld\n", SQLCODE);
isc_print_status (gds__status);
return (1);
}

static int addjob (void)
{
   struct {
          long isc_42;	/* isc_count */
   } isc_41;
   struct {
          char  isc_37 [16];	/* JOB_COUNTRY */
          ISC_QUAD isc_38;	/* JOB_REQUIREMENT */
          short isc_39;	/* JOB_GRADE */
          char  isc_40 [6];	/* JOB_CODE */
   } isc_36;
   struct {
          ISC_QUAD isc_49;	/* JOB_REQUIREMENT */
   } isc_48;
   isc_blob_handle		isc_45;		/* blob handle */
   char			isc_46 [400];	/* blob segment */
   unsigned short	isc_47;		/* segment length */
/**************************************
 *
 *	a d d j o b
 *
 **************************************
 *
 * Functional description
 *	Add job description blobs.
 *
 **************************************/
TEXT		job_code[6];
TEXT		line[82], job_country[16];
int		len;
ISC_QUAD	job_blob;
int		job_grade, rec_cnt = 0;

/*EXEC SQL SET TRANSACTION;*/
{
isc_start_transaction (isc_status, (FRBRD**) &gds__trans, (short) 1, &DB, (short) 4, isc_tpb_4);
SQLCODE = isc_sqlcode (isc_status);
if (SQLCODE < 0) goto Error;
}
/*EXEC SQL WHENEVER SQLERROR GO TO Error;*/

/*EXEC SQL DECLARE be CURSOR FOR
    INSERT BLOB job_requirement INTO job;*/
isc_45 = 0;

Fp = fopen ("job.inp", "r");
if (Fp == NULL){
	printf ("job.inp not found\n");
	exit(FINI_ERROR);
	}

while (fgets (line, 100, Fp) != NULL)
    {
    /*EXEC SQL OPEN be INTO :job_blob;*/
    {
    {
    if (!isc_44s)
       isc_dsql_alloc_statement2 (isc_status, &DB, &isc_44s);
    if (isc_44s)
       {
       if (!isc_dsql_set_cursor_name (isc_status, &isc_44s, "BE", 0) &&
          !isc_dsql_execute_m (isc_status, &gds__trans, &isc_44s, 0, (char *)0, -1, 0, (char *)0))
	  {
          isc_create_blob2 (isc_status, &DB, &gds__trans, &isc_45, &isc_48.isc_49, (short) 0, (char*) 0);
	  }
       }
    }
    SQLCODE = isc_sqlcode (isc_status);
    if (!SQLCODE)
       job_blob = isc_48.isc_49;
    if (SQLCODE < 0) goto Error;
    }

    sscanf (line, "%s %d %s", job_code, &job_grade, job_country);

    while (fgets (line, 100, Fp) != NULL)
	{
	if (*line == '\n')
	    break;

	len = strlen (line);
	/*EXEC SQL INSERT CURSOR be VALUES (:line INDICATOR :len);*/
	{
	{
	isc_47 = len;
	isc_ftof (line, isc_47, isc_46, isc_47);
        isc_put_segment (isc_status, &isc_45, isc_47, isc_46);
	SQLCODE = isc_sqlcode (isc_status);
	}
	if (SQLCODE < 0) goto Error;
	}
	}

    /*EXEC SQL CLOSE be;*/
    {
    if (isc_44s && !isc_dsql_free_statement (isc_status, &isc_44s, 1))
       {
       isc_close_blob (isc_status, &isc_45);
       }
    SQLCODE = isc_sqlcode (isc_status);
    if (SQLCODE < 0) goto Error;
    }

    /*EXEC SQL
	UPDATE job
	    SET job_requirement = :job_blob
	    WHERE job_code = :job_code AND
		job_grade = :job_grade AND
		job_country = :job_country;*/
    {
    if (!isc_34)
       isc_compile_request2 (isc_status, (FRBRD**) &DB, (FRBRD**) &isc_34, (short) sizeof (isc_35), (char *) isc_35);
    isc_vtov ((char*)job_country, (char*)isc_36.isc_37, 16);
    isc_36.isc_38 = job_blob;
    isc_36.isc_39 = job_grade;
    isc_vtov ((char*)job_code, (char*)isc_36.isc_40, 6);
    if (isc_34)
       isc_start_and_send (isc_status, (FRBRD**) &isc_34, (FRBRD**) &gds__trans, (short) 0, (short) 32, &isc_36, (short) 0);
    SQLCODE = isc_sqlcode (isc_status);
    if (!SQLCODE) 
       {
       isc_receive (isc_status, (FRBRD**) &isc_34, (short) 1, (short) 4, &isc_41, (short) 0);
       SQLCODE = isc_sqlcode (isc_status);
       if (!SQLCODE && !isc_41.isc_42)
	  SQLCODE = 100;
       }
    if (SQLCODE < 0) goto Error;
    }

    if (SQLCODE == 0)
	rec_cnt++;
    else
	{
	printf ("Input error -- no job record with key: %s %d %s\n",
	    job_code, job_grade, job_country);
	}
    }

/*EXEC SQL COMMIT;*/
{
isc_commit_transaction (isc_status, (FRBRD**) &gds__trans);
SQLCODE = isc_sqlcode (isc_status);
if (SQLCODE < 0) goto Error;
}
printf ("Added %d job requirement descriptions.\n", rec_cnt);
fclose (Fp);

return (0);
	
Error:

printf ("SQLCODE=%ld\n", SQLCODE);
isc_print_status (gds__status);

return (1);
}

static int addproj (void)
{
   struct {
          long isc_26;	/* isc_count */
   } isc_25;
   struct {
          ISC_QUAD isc_23;	/* PROJ_DESC */
          char  isc_24 [6];	/* PROJ_ID */
   } isc_22;
   struct {
          ISC_QUAD isc_33;	/* PROJ_DESC */
   } isc_32;
   isc_blob_handle		isc_29;		/* blob handle */
   char			isc_30 [800];	/* blob segment */
   unsigned short	isc_31;		/* segment length */
/**************************************
 *
 *	a d d p r o j
 *
 **************************************
 *
 * Functional description
 *	Add project description blobs.
 *
 **************************************/
TEXT		proj_id[6];
TEXT		line[82];
int		len;
ISC_QUAD	proj_blob;
int		rec_cnt = 0;

/*EXEC SQL SET TRANSACTION;*/
{
isc_start_transaction (isc_status, (FRBRD**) &gds__trans, (short) 1, &DB, (short) 4, isc_tpb_5);
SQLCODE = isc_sqlcode (isc_status);
if (SQLCODE < 0) goto Error;
}
/*EXEC SQL WHENEVER SQLERROR GO TO Error;*/

/*EXEC SQL DECLARE bd CURSOR FOR
    INSERT BLOB proj_desc INTO project;*/
isc_29 = 0;

Fp = fopen ("proj.inp", "r");
if (Fp == NULL){
	printf ("proj.inp not found\n");
	exit(FINI_ERROR);
	}

while (fgets (line, 100, Fp) != NULL)
    {
    /*EXEC SQL OPEN bd INTO :proj_blob;*/
    {
    {
    if (!isc_28s)
       isc_dsql_alloc_statement2 (isc_status, &DB, &isc_28s);
    if (isc_28s)
       {
       if (!isc_dsql_set_cursor_name (isc_status, &isc_28s, "BD", 0) &&
          !isc_dsql_execute_m (isc_status, &gds__trans, &isc_28s, 0, (char *)0, -1, 0, (char *)0))
	  {
          isc_create_blob2 (isc_status, &DB, &gds__trans, &isc_29, &isc_32.isc_33, (short) 0, (char*) 0);
	  }
       }
    }
    SQLCODE = isc_sqlcode (isc_status);
    if (!SQLCODE)
       proj_blob = isc_32.isc_33;
    if (SQLCODE < 0) goto Error;
    }

    sscanf (line, "%s", proj_id);

    while (fgets (line, 100, Fp) != NULL)
	{
	if (*line == '\n')
	    break;

	len = strlen (line);
	/*EXEC SQL INSERT CURSOR bd VALUES (:line INDICATOR :len);*/
	{
	{
	isc_31 = len;
	isc_ftof (line, isc_31, isc_30, isc_31);
        isc_put_segment (isc_status, &isc_29, isc_31, isc_30);
	SQLCODE = isc_sqlcode (isc_status);
	}
	if (SQLCODE < 0) goto Error;
	}
	}

    /*EXEC SQL CLOSE bd;*/
    {
    if (isc_28s && !isc_dsql_free_statement (isc_status, &isc_28s, 1))
       {
       isc_close_blob (isc_status, &isc_29);
       }
    SQLCODE = isc_sqlcode (isc_status);
    if (SQLCODE < 0) goto Error;
    }

    /*EXEC SQL
	UPDATE project
	    SET proj_desc = :proj_blob
	    WHERE proj_id = :proj_id;*/
    {
    if (!isc_20)
       isc_compile_request2 (isc_status, (FRBRD**) &DB, (FRBRD**) &isc_20, (short) sizeof (isc_21), (char *) isc_21);
    isc_22.isc_23 = proj_blob;
    isc_vtov ((char*)proj_id, (char*)isc_22.isc_24, 6);
    if (isc_20)
       isc_start_and_send (isc_status, (FRBRD**) &isc_20, (FRBRD**) &gds__trans, (short) 0, (short) 14, &isc_22, (short) 0);
    SQLCODE = isc_sqlcode (isc_status);
    if (!SQLCODE) 
       {
       isc_receive (isc_status, (FRBRD**) &isc_20, (short) 1, (short) 4, &isc_25, (short) 0);
       SQLCODE = isc_sqlcode (isc_status);
       if (!SQLCODE && !isc_25.isc_26)
	  SQLCODE = 100;
       }
    if (SQLCODE < 0) goto Error;
    }

    if (SQLCODE == 0)
	rec_cnt++;
    else
	{
	printf ("Input error -- no project record with key: %s\n", proj_id);
	}
    }

/*EXEC SQL COMMIT;*/
{
isc_commit_transaction (isc_status, (FRBRD**) &gds__trans);
SQLCODE = isc_sqlcode (isc_status);
if (SQLCODE < 0) goto Error;
}
printf ("Added %d project descriptions.\n", rec_cnt);
fclose (Fp);

return (0);
		
Error:

printf ("SQLCODE=%ld\n", SQLCODE);
isc_print_status (gds__status);

return (1);
}

static int addqtr (void)
{
   struct {
          long isc_17;	/* isc_count */
   } isc_16;
   struct {
          ISC_QUAD isc_12;	/* QUART_HEAD_CNT */
          char  isc_13 [4];	/* DEPT_NO */
          long isc_14;	/* FISCAL_YEAR */
          char  isc_15 [6];	/* PROJ_ID */
   } isc_11;
/**************************************
 *
 *	a d d q t r
 *
 **************************************
 *
 * Functional description
 *	Add project quarterly head-count array to 'proj_dept_budget' table.
 *
 **************************************/
TEXT		proj_id[6], dept_no[4];
int		yr;
TEXT		line[81];
int		hcnt[4];
int		rec_cnt = 0;

/*EXEC SQL SET TRANSACTION;*/
{
isc_start_transaction (isc_status, (FRBRD**) &gds__trans, (short) 1, &DB, (short) 4, isc_tpb_6);
SQLCODE = isc_sqlcode (isc_status);
if (SQLCODE < 0) goto Error;
}
/*EXEC SQL WHENEVER SQLERROR GO TO Error;*/

Fp = fopen ("qtr.inp", "r");
if (Fp == NULL){
	printf ("qtr.inp not found\n");
	exit(FINI_ERROR);
	}

while (fgets (line, 100, Fp) != NULL)
    {
    sscanf (line, "%d %s %s %d %d %d %d", &yr, proj_id, dept_no,
	&hcnt[0], &hcnt[1], &hcnt[2], &hcnt[3]);

    /*EXEC SQL
	UPDATE proj_dept_budget
	    SET quart_head_cnt = :hcnt
	    WHERE fiscal_year = :yr AND proj_id = :proj_id AND dept_no = :dept_no;*/
    {
    if (!isc_9)
       isc_compile_request2 (isc_status, (FRBRD**) &DB, (FRBRD**) &isc_9, (short) sizeof (isc_10), (char *) isc_10);
    isc_11.isc_12 = isc_blob_null;
    isc_put_slice (isc_status, &DB, &gds__trans, &isc_11.isc_12, (short) 51, (char *) isc_18, 0, (long*) 0, (long) 16, (void *)hcnt);
    SQLCODE = isc_sqlcode (isc_status);
    if (SQLCODE < 0) goto Error;
    isc_vtov ((char*)dept_no, (char*)isc_11.isc_13, 4);
    isc_11.isc_14 = yr;
    isc_vtov ((char*)proj_id, (char*)isc_11.isc_15, 6);
    if (isc_9)
       isc_start_and_send (isc_status, (FRBRD**) &isc_9, (FRBRD**) &gds__trans, (short) 0, (short) 22, &isc_11, (short) 0);
    SQLCODE = isc_sqlcode (isc_status);
    if (!SQLCODE) 
       {
       isc_receive (isc_status, (FRBRD**) &isc_9, (short) 1, (short) 4, &isc_16, (short) 0);
       SQLCODE = isc_sqlcode (isc_status);
       if (!SQLCODE && !isc_16.isc_17)
	  SQLCODE = 100;
       }
    if (SQLCODE < 0) goto Error;
    }

    if (SQLCODE == 0)
	rec_cnt++;
    else
	{
	printf ("Input error -- no job record with key: %d %s %s\n",
	   yr, proj_id, dept_no);
	}
    }

/*EXEC SQL COMMIT RELEASE;*/
{
if (gds__trans)
    isc_commit_transaction (isc_status, (FRBRD**) &gds__trans);
if (DB)
   isc_detach_database (isc_status, &DB);
SQLCODE = isc_sqlcode (isc_status);
if (SQLCODE < 0) goto Error;
}
printf ("Added %d quarter arrays.\n", rec_cnt);
fclose (Fp);

return (0);
		
Error:

printf ("SQLCODE=%ld\n", SQLCODE);
isc_print_status (gds__status);

return (1);
}
