/*
 *	PROGRAM:	Intl
 *	MODULE:		ld.h
 *	DESCRIPTION:	Language Driver data structures
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
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete ports:
 *                          - MAC ("MAC", "MAC_AUX" and "MAC_CP" defines)
 *                          - XENIX and OS/2
 *
 * 2002.10.27 Sean Leyne - Code Cleanup, removed obsolete "Ultrix" port
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "MS_DOS" define
 *
 */

/* ------------NOTE NOTE NOTE -----------
 * This file is shared with the Language Driver kit used by PC/Windows
 * products.  This master is in the PC side kit, currently (931018) stored
 * as DBENGINE:/area6/langdrv/h/ld.h
 * with Martin Levy as module owner
 * The file has been ported from ANSI C++ to K&R C, with minimal changes
 * to support re-porting in the future.
 *   C++ comments are surrounded by C comments.
 *   Prototypes are #ifdef'ed out.
 *   Following is a bunch of remappings of InterBase data types to
 *	the type names as used on the PC
 */

#ifndef _INTL_LD_H_
#define _INTL_LD_H_

#ifdef CHAR
#undef CHAR
#endif
#define CHAR	SCHAR
/* #define BYTE	UCHAR */
#define INT8	SCHAR
#define UINT8	UCHAR
#define INT16	SSHORT
#define UINT16	USHORT
#define INT32	SLONG
#define UINT32	ULONG

#define BOOL	UINT16

#ifdef WIN_NT
#define DLL_EXPORT	__declspec(dllexport)
#else
#define DLL_EXPORT
#endif


/* Following this line is LD.H from Borland Language Driver Kit */


/*
//-----------------------------------------------------------------
//
// LD.H
//
// Language Driver API header file
//
// 08-17-92 MLevy LdLibraryName[ LD_LIBNAME_LEN ] changed to pLdLibraryName.
// 08-05-92 MLevy Exposed LdExit() again.
// 06-09-92 MLevy ICELAND and BorlPORTUGUESE added.
// 05-20-92 MLevy File made.
//
//-----------------------------------------------------------------
*/

#ifndef __LD_H
#define __LD_H

/*
//-----------------------------------------------------------------
// DATA TYPE DEFINITIONS
//-----------------------------------------------------------------
*/

#ifndef TRUE
#define           TRUE 1
#endif
#ifndef FALSE
#define           FALSE 0
#endif

/*
//For pointers
*/
#ifndef NULL
#define           NULL 0
#endif

#ifndef NULLP
#define           NULLP (0)
#endif

#ifndef VOID
#define           VOID void
#endif

#ifndef CHAR
#define           CHAR char
#endif

#ifndef BYTE
#define           BYTE unsigned char
#endif

#ifndef INT8
#define           INT8 char
#endif

#ifndef UINT8
#define           UINT8 unsigned char
#endif

#ifndef INT16
#define           INT16 int
#endif

#ifndef UINT16
#define           UINT16 unsigned int
#endif

#ifndef INT32
#define           INT32 long
#endif

#ifndef UINT32
#define           UINT32 unsigned long
#endif

#ifndef BOOL
#define           BOOL int
#endif

#define far

typedef VOID far *pVOID;
typedef pVOID far *ppVOID;
typedef CHAR far *pCHAR;
typedef BYTE far *pBYTE;
typedef INT8 far *pINT8;
typedef UINT8 far *pUINT8;
typedef INT32 far *pINT32;
typedef UINT32 far *pUINT32;
typedef BOOL far *pBOOL;
typedef INT16 far *pINT16;
typedef UINT16 far *pUINT16;
typedef UINT16 PDXResult;

#define hFILE  INT16
#define phFILE pINT16

/*
//-----------------------------------------------------------------
// API CONSTANT VALUES
//-----------------------------------------------------------------
*/

#define YES                       TRUE
#define NO                        FALSE


/*
// LdObject Windows resource type
*/
#define LDRESOBJECT            1001
#define LDOBJ_NAME1             101

#define LDID                   1002

#define LDIDRESNAME               "LDID"

/*
// Language type (file or resource)
*/
#define LDOBJRES                  1
#define LDOBJFILE                 2


#define SECONDARY_DIFF          256
#define TERTIARY_DIFF           512
#define LENGTH_DIFF            1024

/*
//
// SORT/COLLATION TYPE PROVIDED BY LANGUAGE DRIVER
//
*/
#define ASCII_SORT                1
#define CODEPAGE_SORT             ASCII_SORT
#define DICT_SORT                 2
#define PHONETIC_SORT             3
#define NAME_SORT                 4

/*
//
// CODE PAGE/CHARACTER SET PLATFORM TYPES
//
*/
#define DOS_CP                    1
#define OEM_CP                    DOS_CP
#define WIN_CP                    2
#define ANSI_CP                   WIN_CP

/*
// UNIX etc.
*/
#define SUNOS_CP                  4
#define VMS_CP                    5
#define HPUX_CP                   6
#define AIX_CP                    8
#define AUX_CP                    9

/*
// SHIFT-JIS (JAPANESE DBCS)
*/
#define SJIS_CP                 255

/*
// length of descriptive language driver name (incl zero)
*/
#define LD_SYMNAME_LEN           20

/*
// max length of language driver signature & file name (incl zero)
*/
#define LD_SFNAME_LEN            20

/*
// max length of language driver resource file name (incl zero)
*/
#define LD_LIBNAME_LEN           13

#define LDSUCCESS                 0
#define LDFAILURE                -1

#define REVERSE					  0x01
#define EXPAND                    0x02
#define SECONDARY                 0x04
#define LOCAL_EXPAND              0x08
#define LOCAL_SECONDARY           0x10

/*
//
// TABLE INDEX CONSTANTS
//
*/

#define CHARDEF_TBL               0
#define UPPERCASE_TBL             1
#define LOWERCASE_TBL             2
#define EXPAND_TBL                3
#define COMPRESS_TBL              4
#define CASESORT_TBL              5
#define NOCASESORT_TBL            6
#define SOUNDEX_TBL               7
#define LICSCP_TBL                8
#define CPLICS_TBL                9
#define PRIMALT_TBL              10
#define ALTPRIM_TBL              11
#define BASE_TBL                 12
#define EOF_VAL                  13

#define NUMBER_TBLS              EOF_VAL + 1

/*
//-----------------------------------------------------------------
*/

struct SortOrderTblEntry {

	UINT16 Primary:8;
	UINT16 Secondary:4;
	UINT16 Tertiary:2;
	UINT16 IsExpand:1;
	UINT16 IsCompress:1;

};

/*
//
// CHARACTER EXPANSION STRUCTURE
//
*/

struct ExpandChar {

	BYTE Ch;
	BYTE ExpCh1;
	BYTE ExpCh2;
};

/*
//
// CHARACTER PAIR TO SINGLE SORT WEIGHT STRUCTURE
//
*/

struct CompressPair {

	BYTE CharPair[2];
	struct SortOrderTblEntry CaseWeight;
	struct SortOrderTblEntry NoCaseWeight;
};


/*
//-----------------------------------------------------------------
*/

/*
//-----------------------------------------------------------------
// CHARACTER/SYMBOL DEFINITION VALUES
//-----------------------------------------------------------------
*/

/*
// NO SPECIAL QUALITIES
*/
#define CH_UNDEFINED           0x00

/*
// A NUMERIC CHARACTER
*/
#define CH_DIGIT               0x01

/*
// AN UPPERCASE CHARACTER
*/
#define CH_UPPER               0x02

/*
// A LOWERCASE CHARACTER
*/
#define CH_LOWER               0x04

/*
// ALPHA CHARACTER
*/
#define CH_ALPHA               CH_LOWER + CH_UPPER

/*
// ALPHANUMERIC CHARACTER
*/
#define CH_ALPHANUM            CH_LOWER + CH_UPPER + CH_DIGIT

/*
//-----------------------------------------------------------------
// ID CODES
//-----------------------------------------------------------------
//
// PRODUCT ID
//
*/
#define PDOX                    0x1
#define DBASE                   0x2
#define IBASE                   0x4
#define OTHER                   0x8
#define BORLAND                0x10

/*
//
// COUNTRY/LANGUAGE ID
//
*/

/*
#define US                        1
#define CANADA                    2
#define LATINAMER                 3
#define NEDERLANDS               31
#define BELGIUM                  32
#define FRANCE                   33
#define SPAIN                    34
#define HUNGARY                  36
#define YUGOSLAVIA               38
#define ITALY                    39
#define SWITZ                    41
#define CZECH                    42
#define UK                       44
#define DENMARK                  45
#define SWEDEN                   46
#define NORWAY                   47
#define POLAND                   48
#define GERMANY                  49
#define BRAZIL                   55
#define INTL                     61
#define PORTUGAL                351
#define FINLAND                 358
#define JAPAN                    81
#define ICELAND                 354

#define NORDAN                    DENMARK + NORWAY
#define SWEDFIN                   SWEDEN  + FINLAND
*/

/*
//
// UNIQUE LANGUAGE DRIVER ID
//
*/

/*
// Paradox
*/
/*
#define pxUS                      1
#define pxINTL                    2
#define pxJAPANESE                3
#define pxNORDAN                  4
#define pxNORDAN4                 5
#define pxSWEDFIN                 6
*/
/*
// dBASE
*/
/*
#define dbARABIC                  7
#define dbDANISH                  8
#define dbDUTCH                   9
#define dbDUTCH2                 10
#define dbFINNISH                11
#define dbFINNISH2               12
#define dbFRENCH                 13
#define dbFRENCH2                14
#define dbGERMAN                 15
#define dbGERMAN2                16
#define dbITALIAN                17
#define dbITALIAN2               18
#define dbJAPANESE               19
#define dbSPANISH2               20
#define dbSWEDISH                21
#define dbSWEDISH2               22
#define dbNORWEGIAN              23
#define dbSPANISH                24
#define dbUK                     25
#define dbUK2                    26
#define dbUS                     27
#define dbFRENCHCAN              28
#define dbFRENCHCAN2             29
#define dbFRENCHCAN3             30
#define dbCZECH                  31
#define dbCZECH2                 32
#define dbGREEK                  33
#define dbHUNGARIAN              34
#define dbPOLISH                 35
#define dbPORTUGUESE             36
#define dbPORTUGUESE2            37
#define dbRUSSIAN                38
*/
/*
// Borland
*/
/*
#define BorlDANISH               39
#define BorlDUTCH                40
#define BorlFINNISH              41
#define BorlFRENCH               42
#define BorlCANADIAN             43
#define BorlGERMAN               44
#define BorlICELANDIC            45
#define BorlITALIAN              46
#define BorlJAPANESE             47
#define BorlNORWEGIAN            48
#define BorlSPANISH              49
#define BorlSPANISH2             50
#define BorlSWEDISH              51
#define BorlUK                   52
#define BorlUS                   53
#define BorlPORTUGUESE           54
#define dbUS2                    55
*/
/*
// User Defined 201 - 254
*/
/*
#define USERDEFINED_MIN         201
#define USERDEFINED_MAX         255
*/

/*
//-----------------------------------------------------------------
// ERROR CODES
//-----------------------------------------------------------------
*/

#define ERRBASE_LDAPI             0x7000

/*
// no errors at all
*/
#define LDERR_NONE                LDSUCCESS

/*
// LD memory could be allocated
*/
#define LDERR_NOMEMORY            ERRBASE_LDAPI + 0xA1

/*
// LD file corrupt/could not be read
*/
#define LDERR_FILECORRUPT         ERRBASE_LDAPI + 0xA2

/*
// LD version mismatch
*/
#define LDERR_OLDVERSION          ERRBASE_LDAPI + 0xA3

/*
// LD table not supported
*/
#define LDERR_NOTSUPPORTED        ERRBASE_LDAPI + 0xA4

/*
// LD file not accessible
*/
#define LDERR_FILEOPEN            ERRBASE_LDAPI + 0xA5

/*
// LD resource not accessible
*/
#define LDERR_RESOPEN             ERRBASE_LDAPI + 0xA6

/*
// unknown
*/
#define LDERR_UNKNOWN             ERRBASE_LDAPI + 0xA7

#endif
/*
//-----------------------------------------------------------------


//-----------------------------------------------------------------
// VCS AREA
//-----------------------------------------------------------------

//	$Header: /home/job/firebird/cvs-backup/firebird2/src/intl/ld.h,v 1.19 2003-02-28 12:45:35 brodsom Exp $

//	$Log: not supported by cvs2svn $
//	Revision 1.18  2003/02/14 13:47:27  eku
//	Fixed nested comment.
//	
//	Revision 1.17  2003/02/13 09:52:17  dimitr
//	CONST -> const
//	
//	Revision 1.16  2003/02/12 02:53:47  brodsom
//	Remove commit non used defines
//	
//	Revision 1.15  2003/02/10 19:25:14  brodsom
//	Remove QLD and PIRANHA defines
//	
//	Revision 1.14  2003/02/07 15:07:17  brodsom
//	Remove MS_DOS and __LDSYS_H
//	
//	Revision 1.13  2003/01/18 19:50:16  dimitr
//	Cleanup.
//	
//	Revision 1.12  2002/11/06 07:19:00  eku
//	Do not redefine MAXPATHLEN. Always use the value supplied by the operating
//	system (header files).
//	
//	Revision 1.11  2002/10/31 05:09:27  seanleyne
//	Removed support for obsolete "MS_DOS" define
//	
//	Revision 1.10  2002/10/30 06:40:46  seanleyne
//	Removed obsolete "Netware" port
//	
//	Revision 1.9  2002/10/28 05:19:47  seanleyne
//	Code Cleanup, removed obsolete "Ultrix" and "Ultrix/MIPS" ports
//	
//	Revision 1.8  2002/10/02 09:40:21  eku
//	Fixed compiler warning 'No newline at end of file'.
//	
//	Revision 1.7  2002/06/05 11:02:46  dimitr
//	Fixed compile problems on Win32.
//	
//	Revision 1.6  2002/04/09 23:25:13  bellardo
//	changes necessary to get the intl lib compiling after adding the new
//	memory management, an additional intl c++ interface, and a slight
//	refactoring of the intl header files.
//	
//	Revision 1.5  2002/02/16 05:06:17  seanleyne
//	Clean up of change notes in headers
//	
//	Revision 1.4  2002/02/16 04:16:08  seanleyne
//	Code cleanup of obsolete OS/2 port
//	
//	Revision 1.3  2002/02/16 02:49:56  seanleyne
//	Code cleanup of obsolete XENIX port and missed MAC (MAC_AUX) port items
//	
//	Revision 1.2  2002/02/16 01:58:24  seanleyne
//	Code cleanup of obsolete MAC AUX port defines "MAC" and "MAC_CP"
//	
//	Revision 1.1.1.1  2001/05/23 13:25:50  tamlin
//	First steps towards a C++ conversion.
//
//	Revision 1.1.1.1  2000/08/03 20:49:08  skywalker
//	Save the whale, feed the poor, and set the sources free.
//
//	Revision 1.1.1.1  2000/07/29 04:25:02  root
//	interbase initial import
//
//	Revision 1.2.2.2  2000/07/24 20:33:16  chrisj
//	Changing the wording of the license boilerplate in all the source files
//	to match new changes required by Inprise legal dept on 24 July 2000.
//	        --Changed by Chris Jewell, not reviewed.
//
//	Revision 1.2.2.1  2000/07/21 23:02:08  chrisj
//	Globally changing the wording of the copyright and license text
//	in the headers of all the source files.  2000-07-21 by chrisj
//
//	Revision 1.2  2000/06/06 22:50:18  stsikin
//	Removing #ifdef _ANSI_PROTOTYPES stuff.
//	Adding new license/Copyright text.
//
//	Stanislav Tsikin, 06-JUN-2000.
//
//	Revision 1.1.1.1  2000/03/05 00:23:55  rkumar
//	InterBase 6.0 sources
//
//
//    Rev 1.8   23 Nov 1992 16:46:24   MLEVY
// LD.DLL => LD01.DLL
//
//    Rev 1.7   10 Nov 1992 17:26:48   MLEVY
// version info modified
//
//    Rev 1.6   10 Nov 1992 15:24:48   MLEVY
// new defines for CANADA
//
//    Rev 1.5   10 Nov 1992 11:38:46   MLEVY
// LD_MINOR bumped to 1
//
//    Rev 1.4   20 Oct 1992 16:40:54   MLEVY
// PIRANHA_CREATE_RESTRUCTURE ifndef added around 3 typedefs
//
//    Rev 1.3   20 Oct 1992 14:05:58   MLEVY
// LOCAL_EXPAND and LOCAL_SECONDARY added. EXPAND and SECONDARY values changed
// revision bumped. REVERSE replaces REVERSE_DIRECTION
//
//    Rev 1.2   20 Aug 1992 12:04:34   MLEVY
// LdLibraryName is now a pointer
//
//    Rev 1.1   08 Aug 1992 14:33:56   MLEVY
// expose LdExit()
//
//    Rev 1.0   16 Jun 1992 16:37:50   MLEVY
// Initial revision.
//
//    Rev 1.5   16 Jun 1992 16:01:02   MLEVY

//-----------------------------------------------------------------
*/

#endif /* _INTL_LD_H_ */
