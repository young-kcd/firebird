/*
 *	PROGRAM:	Language Preprocessor
 *	MODULE:		pat.h
 *	DESCRIPTION:	Code generator pattern generator
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

#ifndef _GPRE_PAT_H_
#define _GPRE_PAT_H_

/* 
Substitution codes: 

    RH	Request handle
    RL	Request level

    DH	Database handle
    DF	Database filename

    TH	Transaction handle

    BH	Blob handle
    BI	Blob ident

    FH	Form handle

    V1	Start vector (1)
    V2	Start vector (2)
    I1	Identifier 1
    I2	Identifier 2
    RF	Pass by ref modifier
    S1	Arbitrary string
    S2	Arbitrary string
    S3  Arbitrary string
    S4  Arbitrary string
    S5  Arbitrary string
    S6  Arbitrary string
    S7  Arbitrary string
    N1	Arbitrary number (signed SSHORT)
    N2  Arbitrary number (signed SSHORT)
    N3	Arbitrary number (signed SSHORT)
    N4  Arbitrary number (signed SSHORT)
    L1  Arbitrary number (signed SLONG)
    L2  Arbitrary number (signed SLONG)
    PN	Port message number
    PL	Port length
    PI	Port ident
    QN	Second port message number
    QL	Second port length
    QI	Second port ident

*/

struct pat {
	DBB pat_database;
	GPRE_REQ pat_request;
	GPRE_TRA pat_transaction;
	BLB pat_blob;
	POR pat_port;
	POR pat_port2;
	TEXT *pat_string1;
	TEXT *pat_string2;
	TEXT *pat_string3;
	TEXT *pat_string4;
	TEXT *pat_string5;
	TEXT *pat_string6;
	TEXT *pat_string7;
	USHORT pat_ident1;
	USHORT pat_ident2;
	TEXT *pat_vector1;
	TEXT *pat_vector2;
	int pat_value1;
	int pat_value2;
	int pat_value3;
	int pat_value4;
	SLONG pat_long1;
	SLONG pat_long2;
	bool pat_condition;
	REF pat_reference;
};

typedef pat PAT;

#endif /* _GPRE_PAT_H_ */
