/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		map.c
 *	DESCRIPTION:	Map GALAXY blr into RDB blr
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


#include "firebird.h"
#include ssdef
#include descrip

#include "../jrd/gds.h"
#include "../jrd/common.h"
#include "../jrd/gdsassert.h"
#include "../jrd/gds_proto.h"

#define BUGCHECK		bugcheck
#define RDB$_SEGSTR_EOF		20480378
#define RDB$_SEGMENT		20480369
#define GDS$_MISC_INTERPRETED   1

/* Struct used to describe records that request mapping */

typedef struct map_msg {
	struct msg *msg_next;		/* Next message in request */
	USHORT msg_number;			/* Message number */
	USHORT msg_gds_length;		/* Length of GDS form */
	USHORT msg_rdb_length;		/* Length of RDB form */
	USHORT msg_count;			/* Number of fields */
	struct msg_repeat {
		USHORT msg_dtype;		/* Parameter dtype */
		USHORT msg_length;		/* Parameter length */
	} msg_rpt[];
} *MAP_MSG;

#include "../jrd/map_proto.h"

double MTH$CVT_D_G(), MTH$CVT_G_D();

static void bugcheck(int);
static bool check_message(UCHAR **);
static MAP_MSG rebuild_message(UCHAR **, UCHAR **);
static int translate_status(ISC_STATUS *, ISC_STATUS *, SCHAR **);

#define WRKBUF_SIZ		256
#define DEFAULT_STATUS_FLAGS	0xF0000L

//static SLONG user_codes[] = {
//	1,
#include "../include/gen/rdb_codes.h"
//};

static SCHAR workbuf[1024];


void MAP_date_to_gds(SLONG * vms_date, SLONG * gds_date)
{
/**************************************
 *
 *	M A P _ d a t e _ t o _ g d s
 *
 **************************************
 *
 * Functional description
 *	Map a date to GDS date format from VMS format.
 *
 **************************************/
	SLONG time;

	lib$day(gds_date, vms_date, &time);
	gds_date[1] = time * 100;
}


void MAP_date_to_rdb(SLONG * gds_date, SLONG * vms_date)
{
/**************************************
 *
 *	M A P _ d a t e _ t o _ r d b
 *
 **************************************
 *
 * Functional description
 *	Map a date to VMS date format from GDS format.
 *
 **************************************/
	SLONG time[2], zero[2], temp, temp2;

/* Compute time portion */

	zero[0] = zero[1] = 0;
	temp = 1000;
	lib$emul(gds_date + 1, &temp, zero, time);

/* Compute date portion being careful of overflow */

	temp = 60 * 10000000;
	temp2 = gds_date[0] * 24 * 60;
	lib$emul(&temp2, &temp, time, vms_date);
}


int MAP_gds_to_rdb(USHORT number, MAP_MSG msg, UCHAR * from, UCHAR * to)
{
/**************************************
 *
 *	M A P _ g d s _ t o _ r d b 
 *
 **************************************
 *
 * Functional description
 *	Given a message number, a GDS format record, and a buffer
 *	to hold an Rdb format record, check to see if a map record
 *	exists for the message, and if so do the mapping.  If there
 *	is not a map record, return 0.  If there is, return the length
 *	of the Rdb record.
 *
 **************************************/
	VARY vary;
	UCHAR *p, *q;
	USHORT l;
	struct msg_repeat *desc, *end;

/* Find the message.  If none, return 0 and get out */

	for (; msg; msg = msg->msg_next)
		if (msg->msg_number == number)
			break;

	if (!msg)
		return 0;

/* Map data.  Whoppee */

	for (desc = msg->msg_rpt, end = desc + msg->msg_count; desc < end; desc++)
		switch (desc->msg_dtype) {
		case blr_cstring:
			q = from;
			from += desc->msg_length;
			vary = to;
			to += desc->msg_length + 1;
			l = strlen(q);
			if (l > desc->msg_length - 1)
				l = desc->msg_length - 1;
			if (vary->vary_length = l) {
				p = vary->vary_data;
				do
					*p++ = *q++;
				while (--l);
			}
			break;

		case blr_timestamp:
			MAP_date_to_rdb(from, to);
			from += desc->msg_length;
			to += desc->msg_length;
			break;

		case blr_d_float:
			*((double *) to) = MTH$CVT_D_G((double *) from);
			from += desc->msg_length;
			to += desc->msg_length;
			break;

		default:
			if (l = desc->msg_length)
				do
					*to++ = *from++;
				while (--l);
		}

	return msg->msg_rdb_length;
}


MAP_MSG MAP_parse_blr(UCHAR * org_blr,
				  USHORT org_blr_length,
				  UCHAR * new_blr,
				  USHORT * new_blr_length, SLONG * max_length)
{
/**************************************
 *
 *	M A P _ p a r s e _ b l r
 *
 **************************************
 *
 * Functional description
 *	The price of progress is incompatibility.  GALAXY supports null
 *	terminated strings, but the Rdb world does not.  To bridge the
 *	incompatibility, search out messages that contain verboten data
 *	types and build message blocks for the offending messages.
 *
 **************************************/
	MAP_MSG msg, next;
	UCHAR *end_org_blr, *new, *org, *last_copied;
	SLONG max;

	end_org_blr = org_blr + org_blr_length;
	org = last_copied = org_blr;
	new = new_blr;
	msg = NULL;

/* Check version.  It must be version 4 or 5 or else we can't handle this */

	if (*org != blr_version4 && *org != blr_version5)
		return NULL;
	++org;
	if (*org++ != blr_begin)
		return NULL;

/* Process messages until something else is entered.  If the message
   requires translation, build both a msg mapping block and a new
   blr message */

	while (*org++ == blr_message)
		if (check_message(&org)) {
			while (last_copied < org)
				*new++ = *last_copied++;
			if (!(next = rebuild_message(&org, &new)))
				return NULL;
			next->msg_next = msg;
			msg = next;
			last_copied = org;
		}

/* If we've process all messages and none require work, indicate that
   nothing remains to be done and get out */

	for (max = 0, next = msg; next; next = next->msg_next) {
		max = MAX(max, next->msg_gds_length);
		max = MAX(max, next->msg_rdb_length);
	}

	*max_length = max;

	if (!msg)
		return NULL;

/* The request needs to be rebuilt completely.  We've already copied the
   messages, now finish the rest of the request. */

	while (last_copied < end_org_blr)
		*new++ = *last_copied++;

	*new_blr_length = new - new_blr;

	return msg;
}


int MAP_rdb_length(USHORT number, MAP_MSG msg)
{
/**************************************
 *
 *	M A P _ r d b _ l e n g t h
 *
 **************************************
 *
 * Functional description
 *	Determine where a particular message type requires mapping,
 *	and if so, the Rdb length.
 *
 **************************************/

	for (; msg; msg = msg->msg_next)
		if (msg->msg_number == number)
			return msg->msg_rdb_length;

	return 0;
}


int MAP_rdb_to_gds(USHORT number, MAP_MSG msg, UCHAR * from, UCHAR * to)
{
/**************************************
 *
 *	M A P _ r d b _ t o _ g d s
 *
 **************************************
 *
 * Functional description
 *	Given a message number, an Rdb format record, and a buffer
 *	to hold an GDS format record, check to see if a map record
 *	exists for the message, and if so do the mapping.  If there
 *	is not a map record, return 0.  If there is, return the length
 *	of the GDS record.
 *
 **************************************/
	VARY vary;
	UCHAR *p, *q;
	USHORT l;
	struct msg_repeat *desc, *end;

/* Find the message.  If none, return 0 and get out */

	for (; msg; msg = msg->msg_next)
		if (msg->msg_number == number)
			break;

	if (!msg)
		return 0;

/* Map data.  Whoppee */

	for (desc = msg->msg_rpt, end = desc + msg->msg_count; desc < end; desc++)
		switch (desc->msg_dtype) {
		case blr_cstring:
			vary = from;
			from += desc->msg_length + 1;
			p = to;
			to += desc->msg_length;
			l = MIN(vary->vary_length, desc->msg_length - 1);
			if (l) {
				q = vary->vary_data;
				do
					*p++ = *q++;
				while (--l);
			}
			*p = 0;
			break;

		case blr_timestamp:
			MAP_date_to_gds(from, to);
			from += desc->msg_length;
			to += desc->msg_length;
			break;

		case blr_d_float:
			*((double *) to) = MTH$CVT_G_D((double *) from);
			from += desc->msg_length;
			to += desc->msg_length;
			break;

		default:
			if (l = desc->msg_length)
				do
					*to++ = *from++;
				while (--l);
		}

	return msg->msg_gds_length;
}


void MAP_release(MAP_MSG msg)
{
/**************************************
 *
 *	M A P _ r e l e a s e
 *
 **************************************
 *
 * Functional description
 *	Release any mapping blocks.
 *
 **************************************/
	MAP_MSG next;

	while (next = msg) {
		msg = msg->msg_next;
		gds__free(next);
	}
}


int MAP_status_to_gds(ISC_STATUS * vms_status, ISC_STATUS * gds_status)
{
/**************************************
 *
 *	M A P _ s t a t u s _ t o _ g d s
 *
 **************************************
 *
 * Functional description
 *	Translate an RDB status vector to a GDS error status vector.
 *
 **************************************/
	ISC_STATUS code, number, length, gds_index, *tmp;
	ISC_STATUS_ARRAY temp;
	USHORT shift;
	bool flag;
	SCHAR msgbuff[WRKBUF_SIZ], *p, *q, *pw1, *pw2, flags[4], part;
	struct dsc$descriptor_s desc, *dsc_ptr;
//	static const SCHAR * const messages[] = {
#include "gen/msgs.h"
//		0
//	};

	tmp = temp;
	pw1 = pw2 = workbuf;

	gds_status[0] = gds_arg_gds;
	code = vms_status[1];

	if (code == 1)
		return (gds_status[1] = FB_SUCCESS);

	p = msgbuff;
	length = 0;
	desc.dsc$b_class = DSC$K_CLASS_S;
	desc.dsc$b_dtype = DSC$K_DTYPE_T;
	desc.dsc$w_length = WRKBUF_SIZ;
	desc.dsc$a_pointer = p;
	sys$getmsg(code, &length, &desc, 15, flags);
	p[length] = 0;

	gds_index = (code >> 3) - 2560000;

	if (gds_index < 0 || gds_index > gds_err_max) {
		gds_status[1] = gds__random;
		gds_status[2] = gds_arg_string;
		gds_status[3] = pw1;
		gds_status[4] = gds_arg_end;
		while (*p)
			*pw2++ = *p++;
		return gds__random;
	}

	flag = false;
	vms_status = vms_status + 3;

	if (*p == '%')
		p++;

	while (*pw2++ = *p) {
		if (*p++ != '!')
			continue;

		pw2--;
		switch (*p++) {
		case 'O':
		case 'Z':
		case 'S':
			BUGCHECK(234);		/* msg 234 Unimplemented conversion, FAO directive O,Z,S */

		case 'X':
		case 'U':
			if (*p++ != 'L')
				BUGCHECK(235);	/* msg 235 Unimplemented conversion, FAO directive X,U */
			*tmp++ = gds_arg_number;
			number = *tmp++ = *vms_status++;
			flag = (number != 1);
			sprintf(pw2, "%%x%x", number);
			while (*pw2)
				++pw2;
			break;

		case 'A':
			if (*p == 'C') {
				q = *vms_status++;
				length = *q++;
			}
			else if (*p == 'D' || *p == 'F') {
				length = *vms_status++;
				q = *vms_status++;
			}
			else if (*p == 'S') {
				dsc_ptr = *vms_status++;
				length = dsc_ptr->dsc$w_length;
				q = dsc_ptr->dsc$a_pointer;
			}
			else
				BUGCHECK(236);	/* msg 236 Error parsing RDB FAO msg string */

			++p;
			*tmp++ = gds_arg_cstring;
			*tmp++ = length;
			*tmp++ = pw2;
			while (length--)
				*pw2++ = *q++;
			break;

		case '%':
			if (*p != 'S')
				*pw2++ = *p;
			else if (flag)
				*pw2++ = 's';
			p++;
			break;

		case '!':
			*pw2++ = '!';
			break;

#ifdef DEBUG
		case '-':
		case '+':
		case '*':
		case '>':
		case '<':
		case '/':
		case '_':
		case '^':
			break;

		default:
			BUGCHECK(237);		/* msg 237 Error parsing RDB FAO msg str */
#endif

		}
	}

/* So now we've parsed the message & made it into a string.
   Further, we've neatly saved the args in order in tmp...!
   Get gds_message into buffer, parse & fudge parms*/

	gds_status[1] = code = gds__encode(gds_index, 0);
	p = messages[gds_index];
	tmp = temp;
	gds_status = gds_status + 2;
	flag = false;

	while (*p) {
		if (*p++ != '%')
			continue;
		switch (*p) {
		case 's':
			*gds_status++ = gds_arg_cstring;
			if (*tmp++ != gds_arg_cstring || flag) {
				flag = true;
				*gds_status++ = 3;
				*gds_status++ = pw2;
				sprintf(pw2, "<?>");
				break;
			}
			*gds_status++ = *tmp++;
			*gds_status++ = *tmp++;
			break;

		case 'l':
			++p;
		case 'd':
			*gds_status++ = gds_arg_number;
			if (*tmp++ != gds_arg_number || flag) {
				flag = true;
				*gds_status++ = -1;
				break;
			}
			*gds_status++ = *tmp++;
			break;

		default:
			BUGCHECK(238);		/* msg 238 unknown parameter in RdB status vector */
		}
	}
	*gds_status++ = gds_arg_interpreted;
	*gds_status++ = pw1;
	*gds_status++ = gds_arg_end;

	return code;
}


int MAP_status_to_rdb(ISC_STATUS * status_vector, ISC_STATUS * user_status)
{
/**************************************
 *
 *	M A P _ s t a t u s _ t o _ r d b
 *
 **************************************
 *
 * Functional description
 *	Translate a status vector from the Groton world to the RDB world.
 *	Continue until the end of the GDS vector is reached or until
 *	we hit a code that we don't know.
 *
 **************************************/
	ISC_STATUS *gds, *rdb;
	SLONG total_longwds, ret_value;
	SCHAR *buf_ptr;

	total_longwds = 0;
	gds = status_vector;
	rdb = user_status + 1;
	buf_ptr = workbuf;

	while (*gds)
		if (ret_value = translate_status(&gds, &rdb, &buf_ptr))
			total_longwds += ret_value += 2;
		else {
			total_longwds += 2;
			break;
		}

	user_status[0] = DEFAULT_STATUS_FLAGS + total_longwds;	/* Hack, hack, hack... */
	return user_status[1];
}


static void bugcheck(int number)
{
/**************************************
 *
 *	b u g c h e c k
 *
 **************************************
 *
 * Functional description
 * output an error message
 *
 **************************************/
	int n;
	SCHAR buffer[128], number_text[20];
	USHORT flags;

	n =
		gds__msg_lookup(0, JRD_BUGCHK, number, sizeof(buffer), buffer,
						&flags);

	if (n < 1)
		strcpy(buffer, "error code");
	sprintf(number_text, " (%d)", number);
	strcat(buffer, number_text);

	gds__log(buffer);
}


static bool check_message(UCHAR ** org_ptr)
{
/**************************************
 *
 *	c h e c k _ m e s s a g e
 *
 **************************************
 *
 * Functional description
 *	Parse a blr message.  If the message contains a non-Rdb data type,
 *	return true.  Otherwise update the blr pointer and return false.
 *
 **************************************/
	UCHAR *org, dtype;
	SSHORT count;

/* Throw away the message number then pick up the number of fields */

	org = *org_ptr + 1;
	count = *org++;
	count |= *org++ << 16;

	while (count--)
		switch (dtype = *org++) {
		case blr_timestamp:
		case blr_cstring:
		case blr_d_float:
			return true;

		case blr_text:
		case blr_varying:
			*org++;

		case blr_short:
		case blr_long:
		case blr_quad:
			*org++;
		}

	*org_ptr = org;

	return false;
}


static MAP_MSG rebuild_message(UCHAR ** org_ptr, UCHAR ** new_ptr)
{
/**************************************
 *
 *	r e b u i l d _ m e s s a g e
 *
 **************************************
 *
 * Functional description
 *	A message contains an offensive gds datatype.  Rebuild it as a
 *	valid Rdb message while building a msg block to drive the data
 *	mapping.  If we succeed, return the MAP_MSG block.  If we fail for
 *	any reason, return NULL;
 *
 **************************************/
	MAP_MSG msg;
	UCHAR *org, *new, *last_copied;
	USHORT number, l, length, gds_length, rdb_length;
	struct msg_repeat *desc, *end;

/* Do some housekeeping then pick up message number and count of parameters */

	length = 0;
	org = last_copied = *org_ptr;
	new = *new_ptr;
	number = *org++;
	l = *org++;
	l |= *org++ << 16;
	msg = gds__alloc((SLONG)
					 (sizeof(struct msg) + l * sizeof(struct msg_repeat)));
/* FREE: by MAP_release() */
	if (!msg)
		return NULL;			/* NO MEM */


	msg->msg_number = number;
	msg->msg_count = l;
	msg->msg_gds_length = 0;
	msg->msg_rdb_length = 0;

	for (desc = msg->msg_rpt, end = desc + l; desc < end; desc++) {
		switch (desc->msg_dtype = *org++) {
		case blr_text:
			gds_length = *org++;
			gds_length += *org++ << 16;
			rdb_length = gds_length;
			break;

		case blr_varying:
			gds_length = 2 + *org++;
			gds_length += *org++ << 16;
			rdb_length = gds_length;
			break;

		case blr_short:
			*org++;
			rdb_length = gds_length = 2;
			break;

		case blr_long:
			*org++;

		case blr_float:
			rdb_length = gds_length = 4;
			break;

		case blr_quad:
			*org++;

		case blr_timestamp:
		case blr_double:
			rdb_length = gds_length = 8;
			break;

		case blr_d_float:
			while (last_copied < org)
				*new++ = *last_copied++;
			new[-1] = blr_double;
			rdb_length = gds_length = 8;
			break;

		case blr_cstring:
			while (last_copied < org)
				*new++ = *last_copied++;
			new[-1] = blr_varying;
			gds_length = *org++;
			gds_length |= *org++ << 16;
			rdb_length = gds_length + 1;
			l = rdb_length - 2;
			*new++ = l;
			*new++ = l >> 16;
			last_copied = org;
			break;

		default:
			return NULL;
		}
		desc->msg_length = gds_length;
		msg->msg_gds_length += gds_length;
		msg->msg_rdb_length += rdb_length;
	}

/* We're done processing parameters.  Copy anything passed over, update
   pointers, and return */

	while (last_copied < org)
		*new++ = *last_copied++;

	*org_ptr = org;
	*new_ptr = new;

	return msg;
}


static int translate_status(
							ISC_STATUS * status_vector,
							ISC_STATUS * user_status, SCHAR ** buf_ptr)
{
/**************************************
 *
 *	t r a n s l a t e _ s t a t u s
 *
 **************************************
 *
 * Functional description
 *	Translate a single status message.
 *	Return the number of FAO arguments.
 *	If we don't know the status code, return it to caller.
 *
 **************************************/
	ISC_STATUS *gds, *rdb, code, *count_addr;
	USHORT fac = 0, class_ = 0;
	SSHORT count, length;
	SCHAR msgbuff[WRKBUF_SIZ], *p, *q, *pw1, *pw2, flags[4];
	struct dsc$descriptor_s desc, *desc_ptr;

	p = msgbuff;
	pw1 = pw2 = *buf_ptr;
	gds = *status_vector;
	rdb = *user_status;
	count = 0;

	if (*gds == gds_arg_interpreted) {
		q = gds[1];
		rdb[0] = GDS$_MISC_INTERPRETED;
		rdb[1] = DEFAULT_STATUS_FLAGS + 1;
		rdb[2] = strlen(q);
		rdb[3] = gds[1];
		return 2;
	}

	if (*gds++ != gds_arg_gds)
		BUGCHECK(239);			/* msg 239 Interbase status vector inconsistent */

	code = gds__decode(*gds, &fac, &class_);

	if ((code < 0) || (code > gds_err_max)) {
		rdb[0] = *gds;
		rdb[1] = DEFAULT_STATUS_FLAGS;
		return 0;
	}

	code = *rdb++ = user_codes[code];

	length = 0;
	desc.dsc$b_class = DSC$K_CLASS_S;
	desc.dsc$b_dtype = DSC$K_DTYPE_T;
	desc.dsc$w_length = WRKBUF_SIZ;
	desc.dsc$a_pointer = p;
	sys$getmsg(code, &length, &desc, 15, flags);

	p[length] = 0;
	count_addr = rdb++;
	++gds;

/* GDS message parameters should be the same in number, order & datatype
   as those RDB expects.  If not, be real unpleasant about it. */

	while (*p) {
		if (*p++ != '!')
			continue;

		switch (*p) {
		case 'O':
		case 'X':
		case 'Z':
		case 'U':
		case 'S':
			if ((*gds++) != gds_arg_number)
				BUGCHECK(240);	/* msg 240 Interbase/RdB message parameter inconsistency */
			*rdb++ = *gds++;
			++count;
			break;

		case 'A':
			++p;
			switch (*gds++) {
			case gds_arg_cstring:
				length = *pw2++ = *gds++;
				q = *gds++;
				do
					(*pw2++ = *q++);
				while (--length);
				break;

			case gds_arg_string:
				++pw2;
				length = 0;
				q = *gds++;
				while (*pw2++ = *q++)
					++length;
				*pw1 = length;
				break;

			default:
				BUGCHECK(240);	/* msg 240 Interbase/RdB message parameter inconsistency */
			}

			if (*p == 'C')
				*rdb++ = pw1;
			else if (*p == 'D' || *p == 'F') {
				*rdb++ = *pw1++;
				*rdb++ = pw1;
				++count;
			}
			else if (*p == 'S') {
				*rdb++ = pw2;
				desc_ptr = (struct dsc$descriptor *) pw2;
				desc_ptr->dsc$b_class = DSC$K_CLASS_S;
				desc_ptr->dsc$b_dtype = DSC$K_DTYPE_T;
				desc_ptr->dsc$w_length = *pw1++;
				desc_ptr->dsc$a_pointer = pw1;
				pw2 += sizeof(struct dsc$descriptor);
			}
			else
				BUGCHECK(241);	/* msg 241 error parsing RDB FAO message string */

			pw1 = pw2;
			++count;
			break;

		case '%':
			++p;
			if (*p == 'S')
				break;
			BUGCHECK(242);		/* msg 242 unimplemented FAO directive */

		case '!':
			++p;
			break;

#ifdef DEBUG
		case '-':
		case '+':
			break;

		case '*':
		case '>':
		case '<':
		case '/':
		case '_':
		case '^':
			++p;
			break;

		default:
			BUGCHECK(241);		/* msg 241 error parsing RDB FAO message string */
#endif

		}
	}

	*buf_ptr = pw2;
	count_addr[0] = DEFAULT_STATUS_FLAGS + count;
	*status_vector = gds;
	*user_status = rdb;

	return count;
}
