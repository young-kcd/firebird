/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		idx
 *	DESCRIPTION:	Declarations for index initialization
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
 * 2002.08.26 Dmitry Yemanov: new indices on system tables
 * 2002.09.16 Nickolay Samofatov: one more system index
 */

#ifndef _JRD_IDX_H_
#define _JRD_IDX_H_

/* Indices to be created */

/* Maxinum number of segments in any existing system index */
#define INI_IDX_MAX_SEGMENTS 2

struct ini_idx_t {
	UCHAR ini_idx_index_id;
	UCHAR ini_idx_version_flag;
	UCHAR ini_idx_relid;
	UCHAR ini_idx_unique_flag;
	UCHAR ini_idx_segment_count;
	struct ini_idx_segment_t {
		UCHAR ini_idx_rfld_id;
		UCHAR ini_idx_type;
	} ini_idx_segment[INI_IDX_MAX_SEGMENTS];
};

/* Encoded descriptions of system indices */

#define INDEX(id,flag,rel,unique,count) {(id),(flag),(UCHAR) (rel),(unique),(count),{
#define SEGMENT(fld,type) {(fld),(type)}

static const struct ini_idx_t indices[] = {

/*	define index RDB$INDEX_0 for RDB$RELATIONS unique RDB$RELATION_NAME; */
	INDEX(0, ODS_8_0, rel_RELATIONS, idx_unique, 1)
		SEGMENT(f_RELATIONS__RELATION_NAME, idx_metadata)	/* relation name */
	}},
/*	define index RDB$INDEX_1 for RDB$RELATIONS RDB$RELATION_ID; */
	INDEX(1, ODS_8_0, rel_RELATIONS, 0, 1)
		SEGMENT(f_RELATIONS__RELATION_ID, idx_numeric)	/* relation id */
	}},
/*	define index RDB$INDEX_2 for RDB$FIELDS unique RDB$FIELD_NAME; */
	INDEX(2, ODS_8_0, rel_FIELDS, idx_unique, 1)
		SEGMENT(f_FIELDS__FIELD_NAME, idx_metadata)	/* field name */
	}},
/*	define index RDB$INDEX_3 for RDB$RELATION_FIELDS RDB$FIELD_SOURCE; */
	INDEX(3, ODS_8_0, rel_RELATION_FIELDS, 0, 1)
		SEGMENT(f_RELATION_FIELDS__FIELD_SOURCE, idx_metadata)	/* field source name */
	}},
/*	define index RDB$INDEX_4 for RDB$RELATION_FIELDS RDB$RELATION_NAME; */
	INDEX(4, ODS_8_0, rel_RELATION_FIELDS, 0, 1)
		SEGMENT(f_RELATION_FIELDS__RELATION_NAME, idx_metadata)	/* relation name in RFR */
	}},
/*	define index RDB$INDEX_5 for RDB$INDICES unique RDB$INDEX_NAME; */
	INDEX(5, ODS_8_0, rel_INDICES, idx_unique, 1)
		SEGMENT(f_INDICES__INDEX_NAME, idx_metadata)	/* index name */
	}},
/*	define index RDB$INDEX_6 for RDB$INDEX_SEGMENTS RDB$INDEX_NAME; */
	INDEX(6, ODS_8_0, rel_INDEX_SEGMENTS, 0, 1)
		SEGMENT(f_INDEX_SEGMENTS__INDEX_NAME, idx_metadata)	/* index name in seg */
	}},
/*	define index RDB$INDEX_7 for RDB$SECURITY_CLASSES unique RDB$SECURITY_CLASS; */
	INDEX(7, ODS_8_0, rel_SECURITY_CLASSES, idx_unique, 1)
		SEGMENT(f_SECURITY_CLASSES__SECURITY_CLASS, idx_metadata)	/* security class */
	}},
/*	define index RDB$INDEX_8 for RDB$TRIGGERS unique RDB$TRIGGER_NAME; */
	INDEX(8, ODS_8_0, rel_TRIGGERS, idx_unique, 1)
		SEGMENT(f_TRIGGERS__TRIGGER_NAME, idx_metadata)	/* trigger name */
	}},
/*	define index RDB$INDEX_9 for RDB$FUNCTIONS unique RDB$FUNCTION_NAME; */
	INDEX(9, ODS_8_0, rel_FUNCTIONS, idx_unique, 1)
		SEGMENT(f_FUNCTIONS__FUNCTION_NAME, idx_metadata)	/* function name */
	}},
/*	define index RDB$INDEX_10 for RDB$FUNCTION_ARGUMENTS RDB$FUNCTION_NAME; */
	INDEX(10, ODS_8_0, rel_FUNCTION_ARGUMENTS, 0, 1)
		SEGMENT(f_FUNCTION_ARGUMENTS__FUNCTION_NAME, idx_metadata)	/* function name */
	}},
/*	define index RDB$INDEX_11 for RDB$GENERATORS unique RDB$GENERATOR_NAME; */
	INDEX(11, ODS_8_0, rel_GENERATORS, idx_unique, 1)
		SEGMENT(f_GENERATORS__GENERATOR_NAME, idx_metadata)	/* Generator name */
	}},
/*	define index RDB$INDEX_12 for RDB$RELATION_CONSTRAINTS unique RDB$CONSTRAINT_NAME; */
	INDEX(12, ODS_8_0, rel_RELATION_CONSTRAINTS, idx_unique, 1)
		SEGMENT(f_RELATION_CONSTRAINTS__CONSTRAINT_NAME, idx_metadata)	/* constraint name */
	}},
/*	define index RDB$INDEX_13 for RDB$REF_CONSTRAINTS unique RDB$CONSTRAINT_NAME; */
	INDEX(13, ODS_8_0, rel_REF_CONSTRAINTS, idx_unique, 1)
		SEGMENT(f_REF_CONSTRAINTS__CONSTRAINT_NAME, idx_metadata)	/* constraint name */
	}},
/*	define index RDB$INDEX_14 for RDB$CHECK_CONSTRAINTS RDB$CONSTRAINT_NAME; */
	INDEX(14, ODS_8_0, rel_CHECK_CONSTRAINTS, 0, 1)
		SEGMENT(f_CHECK_CONSTRAINTS__CONSTRAINT_NAME, idx_metadata)	/* constraint name */
	}},
/*	define index RDB$INDEX_15 for RDB$RELATION_FIELDS unique RDB$FIELD_NAME, RDB$RELATION_NAME; */
	INDEX(15, ODS_8_0, rel_RELATION_FIELDS, idx_unique, 2)
		SEGMENT(f_RELATION_FIELDS__FIELD_NAME, idx_metadata),	/* field name */
		SEGMENT(f_RELATION_FIELDS__RELATION_NAME, idx_metadata)	/* relation name */
	}},
/*	define index RDB$INDEX_16 for RDB$FORMATS RDB$RELATION_ID, RDB$FORMAT; */
	INDEX(16, ODS_8_0, rel_FORMATS, 0, 2)
		SEGMENT(f_FORMATS__RELATION_ID, idx_numeric),	/* relation id */
		SEGMENT(f_FORMATS__FORMAT, idx_numeric)	/* format id */
	}},
/*	define index RDB$INDEX_17 for RDB$FILTERS RDB$INPUT_SUB_TYPE, RDB$OUTPUT_SUB_TYPE; */
	INDEX(17, ODS_8_0, rel_FILTERS, 0, 2)
		SEGMENT(f_FILTERS__INPUT_SUB_TYPE, idx_numeric),	/* input subtype */
		SEGMENT(f_FILTERS__OUTPUT_SUB_TYPE, idx_numeric)	/* output subtype */
	}},
/*	define index RDB$INDEX_18 for RDB$PROCEDURE_PARAMETERS unique RDB$PROCEDURE_NAME, RDB$PARAMETER_NAME; */
	INDEX(18, ODS_8_0, rel_PROCEDURE_PARAMETERS, idx_unique, 2)
		SEGMENT(f_PROCEDURE_PARAMETERS__PROCEDURE_NAME, idx_metadata),	/* procedure name */
		SEGMENT(f_PROCEDURE_PARAMETERS__PARAMETER_NAME, idx_metadata)	/* parameter name */
	}},

	/* Index on rel_files removed 93-Feb-27 by DaveS.
	 * f_file_name is now 255 bytes, which with roundup creates
	 * an index key which is too large.
	 * Per Deej, this is only necessary to prevent someone creating
	 * a shadow file of the same name as the database file.
	 * Once large indices are implemented this should be reactivated.
	 * 
	 *	INDEX (XX, no_ODS, rel_files, idx_unique, 1)
	 *		SEGMENT (f_file_name, idx_string)
	 *  }},
	 */

/*	define index RDB$INDEX_19 for RDB$CHARACTER_SETS unique RDB$CHARACTER_SET_NAME; */
	INDEX(19, ODS_8_0, rel_CHARACTER_SETS, idx_unique, 1)
		SEGMENT(f_CHARACTER_SETS__CHARACTER_SET_NAME, idx_metadata)	/* character set name */
	}},
/*	define index RDB$INDEX_20 for RDB$COLLATIONS unique RDB$COLLATION_NAME; */
	INDEX(20, ODS_8_0, rel_COLLATIONS, idx_unique, 1)
		SEGMENT(f_COLLATIONS__COLLATION_NAME, idx_metadata)	/* collation name */
	}},
/*	define index RDB$INDEX_21 for RDB$PROCEDURES unique RDB$PROCEDURE_NAME; */
	INDEX(21, ODS_8_0, rel_PROCEDURES, idx_unique, 1)
		SEGMENT(f_PROCEDURES__PROCEDURE_NAME, idx_metadata)	/* procedure name */
	}},
/*	define index RDB$INDEX_22 for RDB$PROCEDURES RDB$PROCEDURE_ID; */
	INDEX(22, ODS_8_0, rel_PROCEDURES, 0, 1)
		SEGMENT(f_PROCEDURES__PROCEDURE_ID, idx_numeric)	/* procedure id */
	}},
/*	define index RDB$INDEX_23 for RDB$EXCEPTIONS unique RDB$EXCEPTION_NAME; */
	INDEX(23, ODS_8_0, rel_EXCEPTIONS, idx_unique, 1)
		SEGMENT(f_EXCEPTIONS__EXCEPTION_NAME, idx_metadata)	/* exception name */
	}},
/*	define index RDB$INDEX_24 for RDB$EXCEPTIONS RDB$EXCEPTION_NUMBER; */
	INDEX(24, ODS_8_0, rel_EXCEPTIONS, 0, 1)
		SEGMENT(f_EXCEPTIONS__EXCEPTION_NUMBER, idx_numeric)	/* exception number */
	}},
/*	define index RDB$INDEX_25 for RDB$CHARACTER_SETS unique	RDB$CHARACTER_SET_ID; */
	INDEX(25, ODS_8_0, rel_CHARACTER_SETS, idx_unique, 1)
		SEGMENT(f_CHARACTER_SETS__CHARACTER_SET_ID, idx_numeric)	/* character set id */
	}},
/*	define index RDB$INDEX_26 for RDB$COLLATIONS unique	RDB$COLLATION_ID, RDB$CHARACTER_SET_ID; */
	INDEX(26, ODS_8_0, rel_COLLATIONS, idx_unique, 2)
		SEGMENT(f_COLLATIONS__COLLATION_ID, idx_numeric),
		SEGMENT(f_COLLATIONS__CHARACTER_SET_ID, idx_numeric)
	}},

	/* Last index in ODS 8.0 is RDB$INDEX_26 */
	
	/* New indices added as part of ODS 8.1
	 * These are now indices on all system tables that previously 
	 * did not have one, except for RDB$PAGES & RDB$LOG_FILES
	 * An index on rel_pages would help DROP TABLE, but not any other
	 * operation.
	 */

/*	define index RDB$INDEX_27 for RDB$DEPENDENCIES RDB$DEPENDENT_NAME; */
	INDEX(27, ODS_8_1, rel_DEPENDENCIES, 0, 1)
		SEGMENT(f_DEPENDENCIES__DEPENDENT_NAME, idx_metadata)	/* dependent name */
	}},
/*	define index RDB$INDEX_28 for RDB$DEPENDENCIES RDB$DEPENDED_ON_NAME; */
	INDEX(28, ODS_8_1, rel_DEPENDENCIES, 0, 1)
		SEGMENT(f_DEPENDENCIES__DEPENDED_ON_NAME, idx_metadata)	/* dependent on name */
	}},
/*	define index RDB$INDEX_29 for RDB$USER_PRIVILEGES RDB$RELATION_NAME; */
	INDEX(29, ODS_8_1, rel_USER_PRIVILEGES, 0, 1)
		SEGMENT(f_USER_PRIVILEGES__RELATION_NAME, idx_metadata)	/* relation name */
	}},
/*	define index RDB$INDEX_30 for RDB$USER_PRIVILEGES RDB$USER; */
	INDEX(30, ODS_8_1, rel_USER_PRIVILEGES, 0, 1)
		SEGMENT(f_USER_PRIVILEGES__USER, idx_metadata)	/* granted user */
	}},
/*	define index RDB$INDEX_31 for RDB$INDICES RDB$RELATION_NAME; */
	INDEX(31, ODS_8_1, rel_INDICES, 0, 1)
		SEGMENT(f_INDICES__RELATION_NAME, idx_metadata)	/* indexed relation */
	}},
/*	define index RDB$INDEX_32 for RDB$TRANSACTIONS unique RDB$TRANSACTION_ID; */
	INDEX(32, ODS_8_1, rel_TRANSACTIONS, idx_unique, 1)
		SEGMENT(f_TRANSACTIONS__TRANSACTION_ID, idx_numeric)	/* transaction id */
	}},
/*	define index RDB$INDEX_33 for RDB$VIEW_RELATIONS RDB$VIEW_NAME; */
	INDEX(33, ODS_8_1, rel_VIEW_RELATIONS, 0, 1)
		SEGMENT(f_VIEW_RELATIONS__VIEW_NAME, idx_metadata)	/* View name */
	}},
/*	define index RDB$INDEX_34 for RDB$VIEW_RELATIONS RDB$RELATION_NAME; */
	INDEX(34, ODS_8_1, rel_VIEW_RELATIONS, 0, 1)
		SEGMENT(f_VIEW_RELATIONS__RELATION_NAME, idx_metadata)	/* base relation name */
	}},
/*	define index RDB$INDEX_35 for RDB$TRIGGER_MESSAGES RDB$TRIGGER_NAME; */
	INDEX(35, ODS_8_1, rel_TRIGGER_MESSAGES, 0, 1)
		SEGMENT(f_TRIGGER_MESSAGES__TRIGGER_NAME, idx_metadata)	/* trigger name */
	}},
/*	define index RDB$INDEX_36 for RDB$FIELD_DIMENSIONS RDB$FIELD_NAME; */
	INDEX(36, ODS_8_1, rel_FIELD_DIMENSIONS, 0, 1)
		SEGMENT(f_FIELD_DIMENSIONS__FIELD_NAME, idx_metadata)	/* array name */
	}},
/*	define index RDB$INDEX_37 for RDB$TYPES RDB$TYPE_NAME; */
	INDEX(37, ODS_8_1, rel_TYPES, 0, 1)
		SEGMENT(f_TYPES__TYPE_NAME, idx_metadata)	/* type name */
	}},
/*	define index RDB$INDEX_38 for RDB$TRIGGERS RDB$RELATION_NAME; */
	INDEX(38, ODS_8_1, rel_TRIGGERS, 0, 1)
		SEGMENT(f_TRIGGERS__RELATION_NAME, idx_metadata)	/* triggered relation */
	}},
/*	define index RDB$INDEX_39 for RDB$ROLES unique RDB$ROLE_NAME; */
	INDEX(39, ODS_9_0, rel_ROLES, idx_unique, 1)
		SEGMENT(f_ROLES__ROLE_NAME, idx_metadata)	/* role name */
	}},

	/* Last index in ODS 9.0 is RDB$INDEX_39 */
	
	/* New indices added as part of ODS 9.1
	 * These are now indices on all system tables that previously 
	 * did not have one.
	 */

/*	define index RDB$INDEX_40 for RDB$CHECK_CONSTRAINTS RDB$TRIGGER_NAME; */
	INDEX(40, ODS_9_1, rel_CHECK_CONSTRAINTS, 0, 1)
		SEGMENT(f_CHECK_CONSTRAINTS__TRIGGER_NAME, idx_metadata)	/* trigger name */
	}},

	/* Last index in both ODS 9.1 and ODS 10.0 is RDB$INDEX_40 */

	/* New indices added as part of ODS 10.1
	 * These are now indices on all system tables that previously 
	 * did not have one.
	 */

/*	define index RDB$INDEX_41 for RDB$INDICES RDB$FOREIGN_KEY; */
	INDEX(41, ODS_10_1, rel_INDICES, 0, 1)
		SEGMENT(f_INDICES__FOREIGN_KEY, idx_metadata)	/* foreign key name */
	}},
/*	define index RDB$INDEX_42 for RDB$RELATION_CONSTRAINTS RDB$RELATION_NAME, RDB$CONSTRAINT_TYPE; */
	INDEX(42, ODS_10_1, rel_RELATION_CONSTRAINTS, 0, 2)
		SEGMENT(f_RELATION_CONSTRAINTS__RELATION_NAME, idx_metadata),	/* relation name */
		SEGMENT(f_RELATION_CONSTRAINTS__CONSTRAINT_TYPE, idx_metadata)     /* constraint type */
	}},
/*	define index RDB$INDEX_43 for RDB$RELATION_CONSTRAINTS RDB$INDEX_NAME; */
	INDEX(43, ODS_10_1, rel_RELATION_CONSTRAINTS, 0, 1)
		SEGMENT(f_RELATION_CONSTRAINTS__INDEX_NAME, idx_metadata),	/* index name */
	}}
	/* Last index in ODS 10.1 is RDB$INDEX_43 */

};

#define SYSTEM_INDEX_COUNT FB_NELEM(indices)

#endif /* _JRD_IDX_H_ */
