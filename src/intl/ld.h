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
#define BYTE	UCHAR
#define INT8	SCHAR
#define UINT8	UCHAR
#define INT16	SSHORT
#define UINT16	USHORT
#define INT32	SLONG
#define UINT32	ULONG

#define BOOL	UINT16


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
*/

#define LD_MAJOR                  1
#define LD_MINOR                  1
#define LD_REV                    1

/*
// default LD resource DLL name
*/


#ifdef QLD
#define LD_LIBRARY                "QLD.DLL"
#else
#define LD_LIBRARY                "LD01.DLL"
#endif

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
#ifndef UNKNOWN
#define           UNKNOWN -1
#endif

/*
//For pointers
*/
#ifndef NULL
#ifdef MS_DOS
#define           NULL (0)
#else
#define           NULL 0
#endif
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

#ifndef NETWARE_386
#ifndef MS_DOS
#define far
#endif
#endif

typedef VOID far *pVOID;
typedef pVOID far *ppVOID;
typedef CHAR far *pCHAR;
typedef BYTE far *pBYTE;
typedef INT8 far *pINT8;
typedef UINT8 far *pUINT8;
typedef INT32 far *pINT32;
typedef UINT32 far *pUINT32;
typedef BOOL far *pBOOL;

#ifndef PIRANHA_CREATE_RESTRUCTURE

typedef INT16 far *pINT16;
typedef UINT16 far *pUINT16;
typedef UINT16 PDXResult;

#endif

#define hFILE  INT16
#define phFILE pINT16


#ifdef MS_DOS
//-----------------------------------------------------------------
// API PROC CALLING CONVENTION
//-----------------------------------------------------------------

#ifndef CALLTYPE
#define CALLTYPE pascal far
#endif

//-----------------------------------------------------------------

#define LdStrColl( LdObj, Str1, Str2 )                             \
                                                                   \
                 LdObj->pColl(   LdObj, Str1, Str2 )

#define LdStrColli( LdObj, Str1, Str2 )                            \
                                                                   \
                 LdObj->pColli(  LdObj, Str1, Str2 )

#define LdStrnColl( LdObj, Str1, Str2, MaxLen )                    \
                                                                   \
                 LdObj->pNColl(  LdObj, Str1, Str2, MaxLen )

#define LdStrnColli( LdObj, Str1, Str2, MaxLen )                   \
                                                                   \
                 LdObj->pNColli( LdObj, Str1, Str2, MaxLen )

#define LdStrCmp( LdObj, Str1, Str2 )                              \
                                                                   \
                 LdObj->pCmp(   LdObj, Str1, Str2 )

#define LdStrCmpi( LdObj, Str1, Str2 )                             \
                                                                   \
                 LdObj->pCmpi(  LdObj, Str1, Str2 )

#define LdStrnCmp( LdObj, Str1, Str2, MaxLen )                     \
                                                                   \
                 LdObj->pNCmp(  LdObj, Str1, Str2, MaxLen )

#define LdStrnCmpi( LdObj, Str1, Str2, MaxLen )                    \
                                                                   \
                 LdObj->pNCmpi( LdObj, Str1, Str2, MaxLen )

#define LdExStrCmp( LdObj, Right, RightLen, Left, LeftLen, Exact ) \
                                                                   \
                 LdObj->pExCmp( LdObj, Right, RightLen, Left, LeftLen, Exact )

//  ------

#define LdCharToOem( LdObj, ch )                                   \
                                                                   \
                 LdCharConvert( LdObj->pOemTable, ch )

#define LdCharToAnsi( LdObj, ch )                                  \
                                                                   \
                 LdCharConvert( LdObj->pAnsiTable, ch )

#define LdStrToOem( LdObj, Str )                                   \
                                                                   \
                 LdStrConvert( LdObj->pOemTable, Str )

#define LdStrToAnsi( LdObj, Str )                                  \
                                                                   \
                 LdStrConvert( LdObj->pAnsiTable, Str )

#define LdStrnToOem( LdObj, Str, Len )                             \
                                                                   \
                 LdStrnConvert( LdObj->pOemTable, Str, Len )

#define LdStrnToAnsi( LdObj, Str, Len )                            \
                                                                   \
                 LdStrnConvert( LdObj->pAnsiTable, Str, Len )

#define LdStrToOemStr( LdObj, Src, Dst )                           \
                                                                   \
                 LdStrConvertStr( LdObj->pOemTable, Src, Dst )

#define LdStrToAnsiStr( LdObj, Src, Dst )                          \
                                                                   \
                 LdStrConvertStr( LdObj->pAnsiTable, Src, Dst )

#define LdStrnToOemStr( LdObj, Src, Dst, Len )                     \
                                                                   \
                 LdStrnConvertStr( LdObj->pOemTable, Src, Dst, Len )

#define LdStrnToAnsiStr( LdObj, Src, Dst, Len )                    \
                                                                   \
                 LdStrnConvertStr( LdObj->pAnsiTable, Src, Dst, Len )

#define LdIsStrInOemCp( LdObj, Str )                               \
                                                                   \
                 LdIsStrConvert( LdObj->pOemTable, Str )

#define LdIsStrInAnsiCp( LdObj, Str )                              \
                                                                   \
                 LdIsStrConvert( LdObj->pAnsiTable, Str )

#define LdIsStrnInOemCp( LdObj, Str, Len )                         \
                                                                   \
                 LdIsStrnConvert( LdObj->pOemTable, Str, Len )

#define LdIsStrnInAnsiCp( LdObj, Str, Len )                        \
                                                                   \
                 LdIsStrnConvert( LdObj->pAnsiTable, Str, Len )

#define LdIsCharInOemCp( LdObj, ch )                               \
                                                                   \
                 LdIsCharConvert( LdObj->pOemTable, ch )

#define LdIsCharInAnsiCp( LdObj, ch )                              \
                                                                   \
                 LdIsCharConvert( LdObj->pAnsiTable, ch )

#endif

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
#define OS2_CP                    3

/*
// UNIX etc.
*/
#define SUNOS_CP                  4
#define VMS_CP                    5
#define HPUX_CP                   6
#define ULTRIX_CP                 7
#define AIX_CP                    8
#define AUX_CP                    9
#define XENIX_CP                 10
#define MAC_CP                   11

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

#ifndef MAXPATHLEN
#define MAXPATHLEN               81
#endif

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
#ifdef MS_DOS
	UINT16 Secondary:5;
	UINT16 Tertiary:1;
#else
	UINT16 Secondary:4;
	UINT16 Tertiary:2;
#endif
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

#ifdef MS_DOS
typedef struct LdResInfo {

	INT16 ID;
	BYTE LdResName[LD_SFNAME_LEN];
	pBYTE pLdLibraryName;
	UINT16 hLdLibrary;
	UINT16 hResource;
	pBYTE pResource;
	UINT16 CurPos;

} LDRESINFO;

typedef LDRESINFO *hLDRESINFO;
#endif

#ifndef __LDSYS_H

#ifdef MS_DOS

//-----------------------------------------------------------------
// LDOBJECT DEFINITIONS
//-----------------------------------------------------------------

typedef struct LDTABLES {

	UINT16 CaseSortTable:1;
	UINT16 NoCaseSortTable:1;
	UINT16 SoundexTable:1;
	UINT16 LicsCPTable:1;
	UINT16 CPLicsTable:1;
	UINT16 PrimAltTable:1;
	UINT16 AltPrimTable:1;
	UINT16 BaseTable:1;
	UINT16 CharDefTable:1;
	UINT16 UpperCaseTable:1;
	UINT16 LowerCaseTable:1;
	UINT16 Reserved:5;

} LDTABLES;

//
// Language Driver header structure
//

typedef struct LdObject {

	// DRIVER ID
	UINT16 DriverID;

	// LANGUAGE/COUNTRY ID
	UINT16 LangCountryID;

	// PRIMARY LANGUAGE DRIVER CODE PAGE ID
	UINT16 PrimaryCodePageID;

	// ALTERNATE CODE PAGE ID (FOR CP TO CP CONVERSION)
	UINT16 AltCodePageID;

	// PRODUCT ID
	UINT16 ProductID;

	// OEM (DOS) / ANSI (WIN) etc.
	BYTE PrimaryCpPlatform;

	// OEM (DOS) / ANSI (WIN) etc.
	BYTE AlternateCpPlatform;

	// TYPE OF TEST FOR BREAKING TIE BREAKS FIRST
	BYTE TieBreakPriority;

	// CHARACTER SET SORT / DICTIONARY SORT
	BYTE LanguageSortType;

	// LANGUAGE DRIVER MAJOR
	BYTE LanguageMajorVersion;

	// LANGUAGE DRIVER MAJOR
	BYTE LanguageMinorVersion;

	// LANGUAGE DRIVER REVISION
	BYTE LanguageRevision;

	// LANGUAGE DRIVER DESCRIPTION
	BYTE LanguageName[LD_SYMNAME_LEN];

	// LANGUAGE DRIVER SIGNATURE & FILENAME
	BYTE LanguageSigFileName[LD_SFNAME_LEN];

	BYTE Reserved[sizeof(LDTABLES) + sizeof(UINT16)];

	// PATHNAME OF ACTUAL LANGUAGE DRIVER LOADED
	BYTE LdFullPathName[MAXPATHLEN + 1];

	// LANGUAGE DRIVER RESOURCE TYPE: WINDOWS RES / DOS FILE
	BYTE LanguageResourceType;

	// CP TO CP CONVERSION
	pUINT16 pOemTable;

	// CP TO CP CONVERSION
	pUINT16 pAnsiTable;

	// STRING COMPARIONS METHODS
	INT16 CALLTYPE(*pColl) (struct LdObject * LdObj, pBYTE Str1, pBYTE Str2);
	INT16 CALLTYPE(*pColli) (struct LdObject * LdObj, pBYTE Str1, pBYTE Str2);
	INT16 CALLTYPE(*pNColl) (struct LdObject * LdObj, pBYTE Str1, pBYTE Str2,
							 UINT16 Maxlen);
	INT16 CALLTYPE(*pNColli) (struct LdObject * LdObj, pBYTE Str1, pBYTE Str2,
							  UINT16 Maxlen);
	INT16 CALLTYPE(*pCmp) (struct LdObject * LdObj, pBYTE Str1, pBYTE Str2);
	INT16 CALLTYPE(*pCmpi) (struct LdObject * LdObj, pBYTE Str1, pBYTE Str2);
	INT16 CALLTYPE(*pNCmp) (struct LdObject * LdObj, pBYTE Str1, pBYTE Str2,
							UINT16 Maxlen);
	INT16 CALLTYPE(*pNCmpi) (struct LdObject * LdObj, pBYTE Str1, pBYTE Str2,
							 UINT16 Maxlen);
	INT16 CALLTYPE(*pExCmp) (struct LdObject * LdObj, pBYTE Right,
							 INT16 RightLen, pBYTE Left, INT16 LeftLen,
							 BOOL Exact);

} LdObject;

#endif

#else

#include "ldstruct.h"

#endif /* //__LDSYS_H */

#ifdef MS_DOS
typedef LdObject LDOBJECT;
typedef LdObject *pLDOBJECT;
typedef LdObject *pLdObject;
typedef LdObject *Ldh;
#endif


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

#ifdef MS_DOS
//-----------------------------------------------------------------
// API PROC PROTOTYPES
//-----------------------------------------------------------------

LDTABLES CALLTYPE LdWhichObjTablesLoaded(LdObject * LdObj);

LDTABLES CALLTYPE LdWhichObjTablesSupported(LdObject * LdObj);

BOOL CALLTYPE LdIsObjTableLoaded(LdObject * LdObj, INT16 table);

BOOL CALLTYPE LdIsObjTableSupported(LdObject * LdObj, INT16 table);

INT16 CALLTYPE LdLoadObj(LdObject ** ReturnLdObj, pBYTE Filename,
						 LDTABLES * Tables);

INT16 CALLTYPE LdLoadObjTable(LdObject * LdObj, INT16 table);

INT16 CALLTYPE LdUnLoadObj(LdObject ** LdObj);

INT16 CALLTYPE LdUnLoadObjTable(LdObject * LdObj, INT16 table);

INT16 CALLTYPE LdResetObjTables(LdObject * LdObj, LDTABLES * Tables);

INT16 CALLTYPE LdReadObjInfo(LdObject * LdObj, pCHAR fileName);

INT16 CALLTYPE LdInit(INT16 MajorVer, INT16 MinorVer);

VOID CALLTYPE LdExit(VOID);

UINT16 CALLTYPE LdCharToBase(LdObject * LdObj, BYTE ch);

UINT16 CALLTYPE LdCharToAlt(LdObject * LdObj, BYTE ch);

UINT16 CALLTYPE LdCharToPrim(LdObject * LdObj, BYTE ch);

BYTE CALLTYPE LdCharToUpper(LdObject * LdObj, BYTE ch);

BYTE CALLTYPE LdCharToLower(LdObject * LdObj, BYTE ch);

BYTE CALLTYPE LdCharToSoundex(LdObject * LdObj, BYTE ch);

BYTE CALLTYPE LdLicsToChar(LdObject * LdObj, BYTE ch);

BYTE CALLTYPE LdCharToLics(LdObject * LdObj, BYTE ch);

BOOL CALLTYPE LdStrToAlt(LdObject * LdObj, pBYTE Str);

BOOL CALLTYPE LdStrToPrim(LdObject * LdObj, pBYTE Str);

BOOL CALLTYPE LdStrnToAlt(LdObject * LdObj, pBYTE Str, UINT16 MaxLen);

BOOL CALLTYPE LdStrnToPrim(LdObject * LdObj, pBYTE Str, UINT16 MaxLen);

BOOL CALLTYPE LdStrToPrimStr(LdObject * LdObj, pBYTE AltStr, pBYTE PrimStr);

BOOL CALLTYPE LdStrToAltStr(LdObject * LdObj, pBYTE PrimStr, pBYTE AltStr);

BOOL CALLTYPE LdStrnToPrimStr(LdObject * LdObj, pBYTE AltStr, pBYTE PrimStr,
							  UINT16 MaxLen);

BOOL CALLTYPE LdStrnToAltStr(LdObject * LdObj, pBYTE PrimStr, pBYTE AltStr,
							 UINT16 MaxLen);

BOOL CALLTYPE LdIsPrimStrInAltCp(LdObject * LdObj, pBYTE Str);

BOOL CALLTYPE LdIsAltStrInPrimCp(LdObject * LdObj, pBYTE Str);

BOOL CALLTYPE LdIsPrimStrnInAltCp(LdObject * LdObj, pBYTE Str, UINT16 MaxLen);

BOOL CALLTYPE LdIsAltStrnInPrimCp(LdObject * LdObj, pBYTE Str, UINT16 MaxLen);

pBYTE CALLTYPE LdStrToUpper(LdObject * LdObj, pBYTE Str);

pBYTE CALLTYPE LdStrToLower(LdObject * LdObj, pBYTE Str);

pBYTE CALLTYPE LdStrnToUpper(LdObject * LdObj, pBYTE Str, UINT16 MaxLen);

pBYTE CALLTYPE LdStrnToLower(LdObject * LdObj, pBYTE Str, UINT16 MaxLen);

BOOL CALLTYPE LdIsAltCharInPrimCp(LdObject * LdObj, BYTE ch);

BOOL CALLTYPE LdIsPrimCharInAltCp(LdObject * LdObj, BYTE ch);

BOOL CALLTYPE LdIsCharAlpha(LdObject * LdObj, BYTE ch);

BOOL CALLTYPE LdIsCharAlphaNumeric(LdObject * LdObj, BYTE ch);

BOOL CALLTYPE LdIsCharDigit(LdObject * LdObj, BYTE ch);

BOOL CALLTYPE LdIsCharLower(LdObject * LdObj, BYTE ch);

BOOL CALLTYPE LdIsCharUpper(LdObject * LdObj, BYTE ch);

INT16 CALLTYPE LdStrColl_(LdObject * LdObj, pBYTE Str1, pBYTE Str2);

INT16 CALLTYPE LdStrnColl_(LdObject * LdObj, pBYTE Str1, pBYTE Str2,
						   UINT16 Maxlen);

INT16 CALLTYPE LdStrColli_(LdObject * LdObj, pBYTE Str1, pBYTE Str2);

INT16 CALLTYPE LdStrnColli_(LdObject * LdObj, pBYTE Str1, pBYTE Str2,
							UINT16 Maxlen);

INT16 CALLTYPE LdStrCmp_(LdObject * LdObj, pBYTE Str1, pBYTE Str2);

INT16 CALLTYPE LdStrnCmp_(LdObject * LdObj, pBYTE Str1, pBYTE Str2,
						  UINT16 Maxlen);

INT16 CALLTYPE LdStrCmpi_(LdObject * LdObj, pBYTE Str1, pBYTE Str2);

INT16 CALLTYPE LdStrnCmpi_(LdObject * LdObj, pBYTE Str1, pBYTE Str2,
						   UINT16 Maxlen);

INT16 CALLTYPE LdExStrCmp_(LdObject * LdObj, pBYTE Right, INT16 RightLen,
						   pBYTE Left, INT16 LeftLen, BOOL Exact);

BYTE CALLTYPE LdGetCharDefinition(LdObject * LdObj, BYTE ch);

//---------------------------------------------------------------

UINT16 CALLTYPE LdCharConvert(pUINT16 Table, BYTE ch);

BOOL CALLTYPE LdStrConvert(pUINT16 Table, pBYTE Str);

BOOL CALLTYPE LdStrnConvert(pUINT16 Table, pBYTE Str, UINT16 MaxLen);

BOOL CALLTYPE LdStrConvertStr(pUINT16 Table, pBYTE SrcStr, pBYTE DstStr);

BOOL CALLTYPE LdStrnConvertStr(pUINT16 Table, pBYTE SrcStr, pBYTE DstStr,
							   UINT16 MaxLen);

BOOL CALLTYPE LdIsStrConvert(pUINT16 Table, pBYTE Str);

BOOL CALLTYPE LdIsStrnConvert(pUINT16 Table, pBYTE Str, UINT16 MaxLen);

BOOL CALLTYPE LdIsCharConvert(pUINT16 Table, BYTE ch);

//---------------------------------------------------------------
// WINDOWS SPECIFIC PROC PROTOTYPES
//---------------------------------------------------------------

INT16 CALLTYPE LdLoadResObj(LDOBJECT ** ReturnLdObj, pBYTE ResName,
							LDTABLES * Tables, pBYTE ResLibrary);

INT16 CALLTYPE LdReadResObjInfo(LDOBJECT * LdObj, pBYTE ResName,
								pBYTE ResLibrary);

INT16 CALLTYPE LdFindFirstResObj(LDRESINFO * Info);

INT16 CALLTYPE LdFindNextResObj(LDRESINFO * Info);

INT16 CALLTYPE LdLoadResObjTable(LDOBJECT * LdObj, INT16 table);

INT16 CALLTYPE LdGetResNamebyID(LDRESINFO * Info);

//---------------------------------------------------------------

pLDOBJECT CALLTYPE LdFindFirstLdObj(VOID);

pLDOBJECT CALLTYPE LdFindNextLdObj(LdObject * LdObj);

pVOID CALLTYPE LdGetObjTableAddr(LdObject * LdObj, LDTABLES * Tables);

UINT16 CALLTYPE LdGetLdObjUsage(LdObject * LdObj);

UINT16 CALLTYPE LdIncLdObjUsage(LdObject * LdObj);

UINT16 CALLTYPE LdGetAPIVersion(VOID);

UINT16 CALLTYPE LdGetAPIRevision(VOID);

pBYTE CALLTYPE LdGetAPIDate(VOID);

VOID CALLTYPE LdFree(pVOID MemPtr);

pVOID CALLTYPE LdCalloc(UINT16 NumberItems, UINT16 SizeItem);
#endif

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

#define pxUS                      1
#define pxINTL                    2
#define pxJAPANESE                3
#define pxNORDAN                  4
#define pxNORDAN4                 5
#define pxSWEDFIN                 6

/*
// dBASE
*/

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

/*
// Borland
*/

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

/*
// User Defined 201 - 254
*/

#define USERDEFINED_MIN         201
#define USERDEFINED_MAX         255

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

//	$Header: /home/job/firebird/cvs-backup/firebird2/src/intl/ld.h,v 1.1.1.1 2001-05-23 13:25:50 tamlin Exp $

//	$Log: not supported by cvs2svn $
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
