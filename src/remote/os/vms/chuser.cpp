/*
 *	PROGRAM:	JRD Remote Server
 *	MODULE:		chuser.c
 *	DESCRIPTION:	Change user identity.
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
#include rms
#include iodef
#include descrip
#include uaidef
#include "../jrd/common.h"

#define SYSUAF	"SYS$SYSTEM:SYSUAF.DAT"

#ifdef __ALPHA
zap_process_alpha_15();
#else
zap_process_v4(), zap_process_v5();
#endif

static void define_logical(UCHAR *, UCHAR *, UCHAR *);
static int get_major_vms_version(void);

/*	Taken from $SYIDEF	*/

#define SYI$_VERSION	4096

static int major_vms_version = 0;

typedef struct itm {
	SSHORT itm_buflen;
	SSHORT itm_code;
	SCHAR *itm_bufadr;
	SCHAR *itm_retlenadr;
} ITM;

#define	LNM$M_NO_ALIAS	1
#define	LNM$M_CONFINE	2
#define	LNM$M_CRELOG	4
#define	LNM$M_TABLE	8
#define	LNM$M_CONCEALED	256
#define	LNM$M_TERMINAL	512
#define	LNM$M_EXISTS	1024
#define	LNM$M_SHAREABLE	65536
#define	LNM$M_CREATE_IF	16777216
#define	LNM$M_CASE_BLIND	33554432
#define	LNM$S_LNMDEF	4
#define	LNM$V_NO_ALIAS	0
#define	LNM$V_CONFINE	1
#define	LNM$V_CRELOG	2
#define	LNM$V_TABLE	3
#define	LNM$V_CONCEALED	8
#define	LNM$V_TERMINAL	9
#define	LNM$V_EXISTS	10
#define	LNM$V_SHAREABLE	16
#define	LNM$V_CREATE_IF	24
#define	LNM$V_CASE_BLIND	25
#define	LNM$C_TABNAMLEN	31
#define	LNM$C_NAMLENGTH	255
#define	LNM$C_MAXDEPTH	10
#define	LNM$_INDEX	1
#define	LNM$_STRING	2
#define	LNM$_ATTRIBUTES	3
#define	LNM$_TABLE	4
#define	LNM$_LENGTH	5
#define	LNM$_ACMODE	6
#define	LNM$_MAX_INDEX	7
#define	LNM$_PARENT	8
#define	LNM$_LNMB_ADDR	9
#define	LNM$_CHAIN	-1

#define	PSL$C_KERNEL	0
#define	PSL$C_EXEC	1
#define	PSL$C_SUPER	2
#define	PSL$C_USER	3


#define	UAF$C_USER_ID	1
#define	UAF$C_VERSION1	1
#define	UAF$C_KEYED_PART	52
#define	UAF$C_AD_II	0
#define	UAF$C_PURDY	1
#define	UAF$C_PURDY_V	2
#define	UAF$K_FIXED	644
#define	UAF$C_FIXED	644
#define	UAF$K_LENGTH	1412
#define	UAF$C_LENGTH	1412
#define	UAF$S_UAFDEF	1412
#define	UAF$B_RTYPE	0
#define	UAF$B_VERSION	1
#define	UAF$W_USRDATOFF	2
#define	UAF$S_USERNAME	32
#define	UAF$T_USERNAME	4
#define	UAF$T_USERNAME_TAG	35
#define	UAF$L_UIC	36
#define	UAF$W_MEM	36
#define	UAF$W_GRP	38
#define	UAF$L_SUB_ID	40
#define	UAF$S_PARENT_ID	8
#define	UAF$Q_PARENT_ID	44
#define	UAF$S_ACCOUNT	32
#define	UAF$T_ACCOUNT	52
#define	UAF$S_OWNER	32
#define	UAF$T_OWNER	84
#define	UAF$S_DEFDEV	32
#define	UAF$T_DEFDEV	116
#define	UAF$S_DEFDIR	64
#define	UAF$T_DEFDIR	148
#define	UAF$S_LGICMD	64
#define	UAF$T_LGICMD	212
#define	UAF$S_DEFCLI	32
#define	UAF$T_DEFCLI	276
#define	UAF$S_CLITABLES	32
#define	UAF$T_CLITABLES	308
#define	UAF$S_PWD	8
#define	UAF$Q_PWD	340
#define	UAF$L_PWD	340
#define	UAF$S_PWD2	8
#define	UAF$Q_PWD2	348
#define	UAF$W_LOGFAILS	356
#define	UAF$W_SALT	358
#define	UAF$B_ENCRYPT	360
#define	UAF$B_ENCRYPT2	361
#define	UAF$B_PWD_LENGTH	362
#define	UAF$S_EXPIRATION	8
#define	UAF$Q_EXPIRATION	364
#define	UAF$S_PWD_LIFETIME	8
#define	UAF$Q_PWD_LIFETIME	372
#define	UAF$S_PWD_DATE	8
#define	UAF$Q_PWD_DATE	380
#define	UAF$S_PWD2_DATE	8
#define	UAF$Q_PWD2_DATE	388
#define	UAF$S_LASTLOGIN_I	8
#define	UAF$Q_LASTLOGIN_I	396
#define	UAF$S_LASTLOGIN_N	8
#define	UAF$Q_LASTLOGIN_N	404
#define	UAF$S_PRIV	8
#define	UAF$Q_PRIV	412
#define	UAF$S_DEF_PRIV	8
#define	UAF$Q_DEF_PRIV	420
#define	UAF$S_MIN_CLASS	20
#define	UAF$R_MIN_CLASS	428
#define	UAF$S_MAX_CLASS	20
#define	UAF$R_MAX_CLASS	448
#define	UAF$L_FLAGS	468
#define	UAF$V_DISCTLY	0
#define	UAF$V_DEFCLI	1
#define	UAF$V_LOCKPWD	2
#define	UAF$V_CAPTIVE	3
#define	UAF$V_DISACNT	4
#define	UAF$V_DISWELCOM	5
#define	UAF$V_DISMAIL	6
#define	UAF$V_NOMAIL	7
#define	UAF$V_GENPWD	8
#define	UAF$V_PWD_EXPIRED	9
#define	UAF$V_PWD2_EXPIRED	10
#define	UAF$V_AUDIT	11
#define	UAF$V_DISREPORT	12
#define	UAF$V_DISRECONNECT	13
#define	UAF$V_AUTOLOGIN	14
#define	UAF$S_NETWORK_ACCESS_P	3
#define	UAF$B_NETWORK_ACCESS_P	472
#define	UAF$S_NETWORK_ACCESS_S	3
#define	UAF$B_NETWORK_ACCESS_S	475
#define	UAF$S_BATCH_ACCESS_P	3
#define	UAF$B_BATCH_ACCESS_P	478
#define	UAF$S_BATCH_ACCESS_S	3
#define	UAF$B_BATCH_ACCESS_S	481
#define	UAF$S_LOCAL_ACCESS_P	3
#define	UAF$B_LOCAL_ACCESS_P	484
#define	UAF$S_LOCAL_ACCESS_S	3
#define	UAF$B_LOCAL_ACCESS_S	487
#define	UAF$S_DIALUP_ACCESS_P	3
#define	UAF$B_DIALUP_ACCESS_P	490
#define	UAF$S_DIALUP_ACCESS_S	3
#define	UAF$B_DIALUP_ACCESS_S	493
#define	UAF$S_REMOTE_ACCESS_P	3
#define	UAF$B_REMOTE_ACCESS_P	496
#define	UAF$S_REMOTE_ACCESS_S	3
#define	UAF$B_REMOTE_ACCESS_S	499
#define	UAF$B_PRIMEDAYS	514
#define	UAF$V_MONDAY	0
#define	UAF$V_TUESDAY	1
#define	UAF$V_WEDNESDAY	2
#define	UAF$V_THURSDAY	3
#define	UAF$V_FRIDAY	4
#define	UAF$V_SATURDAY	5
#define	UAF$V_SUNDAY	6
#define	UAF$B_PRI	516
#define	UAF$B_QUEPRI	517
#define	UAF$W_MAXJOBS	518
#define	UAF$W_MAXACCTJOBS	520
#define	UAF$W_MAXDETACH	522
#define	UAF$W_PRCCNT	524
#define	UAF$W_BIOLM	526
#define	UAF$W_DIOLM	528
#define	UAF$W_TQCNT	530
#define	UAF$W_ASTLM	532
#define	UAF$W_ENQLM	534
#define	UAF$W_FILLM	536
#define	UAF$W_SHRFILLM	538
#define	UAF$L_WSQUOTA	540
#define	UAF$L_DFWSCNT	544
#define	UAF$L_WSEXTENT	548
#define	UAF$L_PGFLQUOTA	552
#define	UAF$L_CPUTIM	556
#define	UAF$L_BYTLM	560
#define	UAF$L_PBYTLM	564
#define	UAF$L_JTQUOTA	568
#define	UAF$W_PROXY_LIM	572
#define	UAF$W_PROXIES	574
#define	UAF$W_ACCOUNT_LIM	576
#define	UAF$W_ACCOUNTS	578


int chuser( UCHAR * name)
{
/**************************************
 *
 *	c h u s e r
 *
 **************************************
 *
 * Functional description
 *	Change the process into the context of a given user.  If the
 *	user doesn't seem to exist, do nothing (except return FALSE).
 *
 **************************************/
	UCHAR username[32], *p;
	int status, vector[8];
	ITM items[6];
	SLONG uic, priv[2];
	SSHORT enqlm;
	SCHAR dev[32], dir[64];
	SLONG uiclen, privlen, enqlmlen, devlen, dirlen;
	struct dsc$descriptor_s desc;
	SCHAR s[1024];
	TEXT flags[4];
	ISC_STATUS errstatus;
	SSHORT l;

	for (p = username; *name; name++)
		*p++ = UPPER(*name);
	while (p < username + sizeof(username))
		*p++ = ' ';

	desc.dsc$b_class = DSC$K_CLASS_S;
	desc.dsc$b_dtype = DSC$K_DTYPE_T;
	desc.dsc$w_length = sizeof(username);
	desc.dsc$a_pointer = username;

	items[0].itm_code = UAI$_UIC;
	items[0].itm_buflen = sizeof(uic);
	items[0].itm_bufadr = &uic;
	items[0].itm_retlenadr = &uiclen;
	items[1].itm_code = UAI$_ENQLM;
	items[1].itm_buflen = sizeof(enqlm);
	items[1].itm_bufadr = &enqlm;
	items[1].itm_retlenadr = &enqlmlen;
	items[2].itm_code = UAI$_DEF_PRIV;
	items[2].itm_buflen = sizeof(priv);
	items[2].itm_bufadr = priv;
	items[2].itm_retlenadr = &privlen;
	items[3].itm_code = UAI$_DEFDEV;
	items[3].itm_buflen = sizeof(dev);
	items[3].itm_bufadr = dev;
	items[3].itm_retlenadr = &devlen;
	items[4].itm_code = UAI$_DEFDIR;
	items[4].itm_buflen = sizeof(dir);
	items[4].itm_bufadr = dir;
	items[4].itm_retlenadr = &dirlen;
	items[5].itm_code = 0;
	items[5].itm_buflen = 0;


/* Lookup UAF record for user.  If it doesn't exist, give up */

	status = sys$getuai(NULL, NULL, &desc, items, NULL, NULL, NULL);

	if (!(status & 1)) {
		if (status != RMS$_RNF) {
			gds__log("SYS$GETUAI failed");
			l = 0;
			desc.dsc$b_class = DSC$K_CLASS_S;
			desc.dsc$b_dtype = DSC$K_DTYPE_T;
			desc.dsc$w_length = 128;
			desc.dsc$a_pointer = s;
			errstatus = sys$getmsg(status, &l, &desc, 15, flags);
			if (errstatus & 1)
				s[l] = 0;
			else
				sprintf(s, "uninterpreted VMS code %x", status);
			gds__log(s);
		}
		lib$signal(status);
		return FALSE;
	}

/* Change to process UIC and USERNAME */

	vector[0] = 4;
	vector[1] = &uic;
	vector[2] = username;
	vector[3] = &enqlm;
	vector[4] = priv;

#ifdef	__ALPHA
	sys$cmkrnl(zap_process_alpha_15, vector);
#else

/* Get the major VMS version so we call a compatible zap routine */

	if (major_vms_version == 0)
		major_vms_version = get_major_vms_version();

/* VMS V5.x requires a different routine from VMS V4.x */

	if (major_vms_version <= 4)
		sys$cmkrnl(zap_process_v4, vector);
	else
		sys$cmkrnl(zap_process_v5, vector);
#endif

	sys$setprv(1, priv, 0, 0);

/* Define process local logical names */

	define_logical("SYS$LOGIN", dev, dir);
	define_logical("SYS$SCRATCH", dev, dir);
	define_logical("SYS$DISK", dev, NULL);
	define_logical("SYS$LOGIN_DEVICE", dev, NULL);

/* Set default directory */

	desc.dsc$b_dtype = DSC$K_DTYPE_T;
	desc.dsc$b_class = DSC$K_CLASS_S;
	desc.dsc$w_length = *dir;
	desc.dsc$a_pointer = dir + 1;
	status = sys$setddir(&desc, NULL, NULL);

	return TRUE;
}


static void define_logical( UCHAR * name, UCHAR * string1, UCHAR * string2)
{
/**************************************
 *
 *	d e f i n e _ l o g i c a l
 *
 **************************************
 *
 * Functional description
 *	Define a logical name based on the concatenation of a couple of strings.
 *
 **************************************/
	struct dsc$descriptor_s desc, table;
	ITM item_list[2];
	int status, retvalue;
	USHORT l;
	UCHAR buffer[128], *p, acmode;

	desc.dsc$b_dtype = DSC$K_DTYPE_T;
	desc.dsc$b_class = DSC$K_CLASS_S;
	desc.dsc$w_length = strlen(name);
	desc.dsc$a_pointer = name;

	table.dsc$b_dtype = DSC$K_DTYPE_T;
	table.dsc$b_class = DSC$K_CLASS_S;
	table.dsc$w_length = sizeof("LNM$PROCESS_TABLE") - 1;
	table.dsc$a_pointer = "LNM$PROCESS_TABLE";

	p = buffer;
	if (l = *string1++)
		do
			*p++ = *string1++;
		while (--l);

	if (string2 && (l = *string2++))
		do
			*p++ = *string2++;
		while (--l);

	item_list[0].itm_code = LNM$_STRING;
	item_list[0].itm_buflen = p - buffer;
	item_list[0].itm_bufadr = buffer;
	item_list[0].itm_retlenadr = &retvalue;
	item_list[1].itm_code = 0;
	item_list[1].itm_buflen = 0;

	acmode = PSL$C_SUPER;
	status = sys$crelnm(NULL, &table, &desc, &acmode, item_list);
	if (!(status & 1))
		lib$signal(status);
}


static int get_major_vms_version(void)
{
/**************************************
 *
 *	g e t _ m a j o r _ v m s _ v e r s i o n
 *
 **************************************
 *
 * Functional description
 *	Return the major VMS version that is running
 *
 **************************************/
	ITM item_list[2];
	int status;
	USHORT retvalue;
	UCHAR buffer[8];

	item_list[0].itm_code = SYI$_VERSION;
	item_list[0].itm_buflen = sizeof buffer;
	item_list[0].itm_bufadr = buffer;
	item_list[0].itm_retlenadr = &retvalue;
	item_list[1].itm_code = 0;
	item_list[1].itm_buflen = 0;

	status = sys$getsyiw(NULL, NULL, NULL, item_list, NULL, NULL, NULL);
	if (!(status & 1))
		lib$signal(status);

/* Buffer contents are of the form Vm.n */

	return (int) (buffer[1] - '0');
}
