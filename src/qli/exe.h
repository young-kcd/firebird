/*
 *	PROGRAM:	JRD Command Oriented Query Language
 *	MODULE:		exe.h
 *	DESCRIPTION:	Execution struct definitions
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

#ifndef _QLI_EXE_H_
#define _QLI_EXE_H_

/* Request Language Block  -- used for BLR, DYN, SDL, etc. */

typedef struct rlb {
    struct blk	rlb_header;
    UCHAR	*rlb_data;		/* Pointer to end of BLR/DYN/SDL */
    UCHAR	*rlb_base;		/* Pointer to start of buffer */
    UCHAR	*rlb_limit;		/* Upper limit of string */
    USHORT	rlb_length;		/* Length of string */
} *RLB;

/* RLB manipulation macros */

#define RLB_BUFFER_SIZE	2048
#define RLB_SAFETY_MARGIN	48

#define CHECK_RLB(in)	(!in || (in->rlb_data > in->rlb_limit)) ? (in = GEN_rlb_extend (in)) : in;
#define RELEASE_RLB	GEN_rlb_release (rlb);
#define STUFF(blr)	*rlb->rlb_data++ = blr
#define STUFF_WORD(blr)	{STUFF (blr); STUFF (blr >> 8);}

/* Request block */

typedef struct qli_req {
    struct blk	req_header;
    struct qli_req	*req_next;		/* Next request in statement */
    struct dbb	*req_database;		/* Database for request */
    FRBRD		*req_handle;		/* Database request handle */
    struct rlb	*req_blr;
    struct qli_msg	*req_messages;		/* Messages associated with request */
    struct qli_msg	*req_receive;		/* Current receive message, if any  */
    struct qli_msg	*req_send;		/* Current send message, if any  */
    struct qli_msg	*req_continue;		/* Message to continue FOR loop after optional actions */
    USHORT	req_flags;		/* Flags for state of request compilation, etc. */
    USHORT	req_context;		/* Next available context */
    USHORT	req_msg_number;		/* Next available message number */
    USHORT	req_label;		/* Next available label */
} *QLI_REQ;

#define REQ_rse_compiled	1
#define REQ_project		2	/* Set during generation of project clause */
#define REQ_group_by		4	/* Set during generation of group by clause */

/* Context node */

typedef enum {
    CTX_RELATION,
    CTX_VARIABLE,
    CTX_AGGREGATE,
    CTX_UNION,
    CTX_STREAM
} CTX_T;

typedef struct qli_ctx {
    struct blk	ctx_header;
    CTX_T	ctx_type;		/* Type of context */
    struct qli_ctx	*ctx_source;		/* Source context for MODIFY */
    struct qli_ctx	*ctx_primary;		/* Primary context */
    struct sym	*ctx_symbol;		/* Context symbol, if any */
    struct qli_rel	*ctx_relation;		/* Relation of context */
    struct qli_nod	*ctx_stream;		/* Stream of context */
    struct frm	*ctx_form;		/* Form block, if any */
    struct qli_fld	*ctx_variable;		/* Variable reference */
    struct qli_req	*ctx_request;		/* Request block */
    struct qli_msg	*ctx_message;		/* Message for data */
    struct qli_nod	*ctx_rse;		/* RSE node for root context */
    struct qli_nod	*ctx_sub_rse;		/* RSE node aggregate */
    struct qli_ctx	*ctx_parent;		/* Parent context for map */
    struct map	*ctx_map;		/* Map items, if any */
    USHORT	ctx_context;		/* Context in request */
} *QLI_CTX;

/* Aggregate/union map block */

typedef struct map {
    struct blk	map_header;
    struct map	*map_next;		/* Next map in item */
    struct qli_nod	*map_node;		/* Value for map item */
    USHORT	map_position;		/* Position in map */
} *MAP;

/* Message block */                                       

typedef struct qli_msg {
    struct blk	msg_header;
    struct qli_req	*msg_request;		/* Parent request */
    struct qli_ctx	*msg_context;		/* Contexts in message */
    struct qli_msg	*msg_next;		/* Next message in request */
    struct par	*msg_parameters;	/* Field instances */
    USHORT	msg_number;		/* Message number */
    USHORT	msg_length;		/* Message length */
    USHORT	msg_parameter;		/* Next parameter number */
    UCHAR	*msg_buffer;		/* Message buffer */
} *QLI_MSG;

/* Field Instance */

typedef struct par {
    struct blk	par_header;
    DSC		par_desc;		/* Value descriptor */
    struct par	*par_next;		/* Next par block in context */
    struct qli_msg	*par_message;		/* Parent message */
    struct qli_nod	*par_value;		/* Value */
    USHORT	par_parameter;		/* Parameter number */
    USHORT	par_offset;		/* Offset of parameter in message */
    struct par	*par_missing;		/* Parameter block for missing value */
} *PAR;

/* Print item block */

typedef enum itm_t
    {
    item_value,
    item_skip,
    item_column,
    item_tab,
    item_space,
    item_new_page,
    item_column_header,
    item_report_header
} ITM_T;

typedef struct itm {
    struct blk	itm_header;
    struct qli_nod	*itm_value;
    TEXT	*itm_edit_string;
    struct pics	*itm_picture;			/* picture string block */
    TEXT	*itm_query_header;
    ITM_T	itm_type;
    USHORT	itm_flags;			/* Misc flags and crud */
    UCHAR	itm_dtype;
    UCHAR	itm_sub_type;
    USHORT	itm_print_offset;
    USHORT	itm_print_length;
    USHORT	itm_header_offset;
    USHORT	itm_header_length;
    USHORT	itm_header_segments;
    USHORT	itm_count;			/* Number of lines to skip */
    USHORT	itm_column;			/* Logical column number */
    FRBRD	*itm_stream;
    USHORT	itm_kanji_fragment;		/* JPN: last kanji on line border */
    ISC_STATUS	itm_blob_status;		/* JPN: status of the last blob fetch */
} *ITM;

#define ITM_overlapped	1			/* Overlapped by another item */

/* Print Control Block */

typedef struct prt {
    struct blk	prt_header;
    struct file	*prt_file;		/* IB_FILE pointer */
    struct rpt	*prt_report;		/* Report block (if report) */
    int		(*prt_new_page)();	/* New page routine, if any */
    USHORT	prt_lines_per_page;
    SSHORT	prt_lines_remaining;
    USHORT	prt_page_number;
} *PRT;

/* General node blocks */

typedef struct qli_nod {
    struct blk	nod_header;
    NOD_T	nod_type;		/* Type of node */
    DSC		nod_desc;		/* Descriptor */
    PAR		nod_import;		/* To pass random value */
    PAR		nod_export;		/* To pass random value */
    SSHORT	nod_count;		/* Number of arguments */
    UCHAR	nod_flags;
    struct qli_nod	*nod_arg[1];
} *QLI_NOD;

#define NOD_local	1		/* locally computed expression */
#define NOD_remote	2
#define NOD_parameter2	4		/* generate a parameter2 if field */
#define nod_partial	8
#define nod_comparison 	16
#define nod_date	32		/* node is a date operation, regardless */

/* Execution node positions */

#define e_fld_field	0		/* field block */
#define e_fld_context	1		/* context for field */
#define e_fld_reference	2		/* points to parameter */
#define e_fld_subs	3		/* subscripts */
#define e_fld_count	4

#define e_for_request	0		/* Request to be started */
#define e_for_send	1		/* Message to be sent */
#define e_for_receive	2		/* Message to be received */
#define e_for_eof	3		/* End of file parameter */
#define e_for_rse	4
#define e_for_statement	5
#define e_for_count	6

#define e_itm_value	0		/* Value of print item */
#define e_itm_edit_string 1		/* Edit string, if any */
#define e_itm_header	2		/* Query header, if any */
#define e_itm_count	3

#define e_rse_first	0		/* FIRST clause, if any */
#define e_rse_boolean	1		/* Boolean clause, if any */
#define e_rse_sort	2		/* Sort clause, if any */
#define e_rse_reduced	3		/* Reduced clause, if any */
#define e_rse_context	4		/* Context block */
#define e_rse_group_by	5
#define e_rse_having	6
#define e_rse_join_type	7
#ifdef PC_ENGINE
#define e_rse_index	8
#endif
#define e_rse_count	9

#define e_prt_list	0		/* List of print items */
#define e_prt_file_name	1		/* Output file name */
#define e_prt_output	2		/* Output file */
#define e_prt_header	3		/* Header to be printed, if any */
#define e_prt_count	4

#define e_prm_prompt	0		/* Prompt string, if any */
#define e_prm_string	1		/* String node for data */
#define e_prm_next	2		/* Next prompt in statement */
#define e_prm_field	3		/* Prototype field, if known */
#define e_prm_count	4

#define e_sto_context	0
#define e_sto_statement	1
#define e_sto_request	2
#define e_sto_send	3
#define e_sto_count	4

#define e_asn_to	0
#define e_asn_from	1
#define e_asn_initial	2
#define e_asn_valid	3		/* Always second-to-last */
#define e_asn_count	4

#define e_mod_send	0
#define e_mod_statement	1		/* Sub-statement */
#define e_mod_request	2		/* Parent request for statement */
#define e_mod_count	3

#define e_era_context	0
#define e_era_request	1		/* Parent request for erase */
#define e_era_message	2		/* Message to be sent, if any */
#define e_era_count	3

#define e_any_request	0		/* Request to be started */
#define e_any_send	1		/* Message to be sent */
#define e_any_receive	2		/* Message to be received */
#define e_any_rse	3
#define e_any_count	4

#define e_rpt_value	0
#define e_rpt_statement	1
#define e_rpt_count	2

#define e_if_boolean	0
#define e_if_true	1
#define e_if_false	2
#define e_if_count	3

#define e_edt_input	0
#define e_edt_dbb	1
#define e_edt_id1	2
#define e_edt_id2	3
#define e_edt_name	4
#define e_edt_count	5

#define e_out_statement	0
#define e_out_file	1
#define e_out_pipe	2
#define e_out_print	3
#define e_out_count	4

#define e_fmt_value	0
#define e_fmt_edit	1
#define e_fmt_picture	2
#define e_fmt_count	3

/* Statistical expression */

#define e_stt_rse	0
#define e_stt_value	1
#define e_stt_default	2
#define e_stt_request	3			/* Request to be started */
#define e_stt_send	4			/* Message to be sent */
#define e_stt_receive	5			/* Message to be received */
#define e_stt_count	6

#define e_map_context	0
#define e_map_map	1
#define e_map_count	2

#define e_ffr_form	0
#define e_ffr_statement	1
#define e_ffr_count	2

#define e_fup_form	0
#define e_fup_fields	1
#define e_fup_tag	2
#define e_fup_count	3

#define e_ffl_form	0
#define e_ffl_field	1
#define e_ffl_string	2
#define e_ffl_count	3

#define e_men_statements	0
#define e_men_labels		1
#define e_men_string		2
#define e_men_menu		4
#define e_men_count		5

#define e_fun_args		0
#define e_fun_function		1
#define e_fun_request		2			/* Request to be started */
#define e_fun_send		3			/* Message to be sent */
#define e_fun_receive		4			/* Message to be received */
#define e_fun_count		5

#define e_syn_statement		0
#define e_syn_send		1
#define e_syn_count		2

/* A Program global or two */

#ifdef REQUESTS_MAIN
#define EXTERN
#else
#define EXTERN	extern
#endif

EXTERN QLI_REQ	QLI_requests;			/* Requests in statement */

#undef EXTERN

#endif /* _QLI_EXE_H_ */
