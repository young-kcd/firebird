/*
 *	MODULE:		TraceCmdLine.cpp
 *	DESCRIPTION:	
 *
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Khorsun Vladyslav
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2008 Khorsun Vladyslav <hvlad@users.sourceforge.net>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 */

#include "firebird.h"
#include "consts_pub.h"
#include "fb_exception.h"
#include "iberror.h"
#include "../../common/classes/fb_string.h"
#include "../../common/StatusArg.h"
#include "../../common/utilsvc.h"
#include "../../jrd/threaddata.h"
#include "../../jrd/trace/TraceService.h"


namespace Firebird {

static void usage(UtilSvc* uSvc, const char* message, ...)
{
	string msg;
	va_list params;
	va_start(params, message);
	msg.vprintf(message, params);
	va_end(params);

	if (uSvc->isService())
		(Arg::Gds(isc_random) << msg).raise();

	fprintf(stderr, "ERROR: %s.\n\n", msg.c_str());
	fprintf(stderr,
		"Firebird Trace utility.\n"
		"Usage: fbtrace <action> [<parameters>]\n"
		"Actions: \n"
		"  -STA[RT]                              Start trace session\n"
		"  -STO[P]                               Stop trace session\n"
		"  -S[USPEND]                            Susupend trace session\n"
		"  -R[ESUME]                             Resume trace session\n"
		"  -L[IST]                               List existing trace sessions\n\n"
		"Parameters: \n"
		"  -N[AME] <string>                      Session name\n"
		"  -I[D]   <number>                      Session ID\n"
		"  -C[ONFIG]   <string>                  Trace configuration file name\n"
		"  -O[UTPUT]   <string>                  Output file name\n"
		"  -U[SER]     <string>                  User name\n"
		"  -P[ASSWORD] <string>                  Password\n"
		"  -FE[TCH]    <string>                  Fetch password from file\n"
		"  -SE[RVICE]  <string>                  Service name\n\n"
		"Examples: \n"
		"  fbtrace -START -NAME my_trace -CONFIG my_cfg.txt\n"
		"  fbtrace -START -CONFIG my_cfg.txt -OUTPUT log.txt\n"
		"  fbtrace -STOP -ID 12"
		"  fbtrace -LIST -OUTPUT stdout"
		"\n"
		"\n"
		"Notes:\n"
		"  Press CTRL+C to stop interactive trace session"
	);
	exit(FINI_ERROR);
}

const char switch_char = '-';

static bool switchMatch(const string& sw, const char* target)
{
/**************************************
 *
 *	s w i t c h M a t c h
 *
 **************************************
 *
 * Functional description
 *	Returns true if switch matches target
 *
 **************************************/
	size_t n = strlen(target);
	if (n < sw.length())
	{
		return false;
	}
	n = sw.length();
	return memcmp(sw.c_str(), target, n) == 0;
}


static const in_sw_tab_t* findSwitch(const in_sw_tab_t* table, string sw)
{
/**************************************
 *
 *	f i n d S w i t c h
 *
 **************************************
 *
 * Functional description
 *	Returns pointer to in_sw_tab entry for current switch
 *	If not a switch, returns 0.
 *	If no match, throws if throwErrors or returns 0.
 *
 **************************************/
	if (sw.isEmpty())
	{
		return 0;
	}
	if (sw[0] != switch_char)
	{
		return 0;
	}
	sw.erase(0, 1);
	sw.upper();

	for (const in_sw_tab_t* in_sw_tab = table; in_sw_tab->in_sw_name; in_sw_tab++)
	{
		if (switchMatch(sw, in_sw_tab->in_sw_name))
		{
			return in_sw_tab;
		}
	}

	return 0;
}

const char* TRACE_ERR_CONFLICT_ACTS		= "conflicting actions \"%s\" and \"%s\" found";
const char* TRACE_ERR_ACT_NOTFOUND		= "action switch not found";
const char* TRACE_ERR_SWITCH_ONCE		= "switch \"%s\" must be set only once";
const char* TRACE_ERR_PARAM_VAL_MISS	= "value for switch \"%s\" is missed";
const char* TRACE_ERR_PARAM_INVALID		= "invalid value (\"%s\") for switch \"%s\"";
const char* TRACE_ERR_SWITCH_UNKNOWN	= "unknow switch \"%s\" encountered";
const char* TRACE_ERR_SWITCH_SVC_ONLY	= "switch \"%s\" can be used by service only";
const char* TRACE_ERR_SWITCH_PARAM_MISS	= "mandatory parameter \"%s\" for switch \"%s\" is missed";
const char* TRACE_ERR_PARAM_ACT_NOTCOMPAT	= "parameter \"%s\" is incompatible with action \"%s\"";


void trace(UtilSvc* uSvc, TraceSvcIntf* traceSvc)
{
	const char* const* end = uSvc->argv.end();

	// search for "action" switch, set NULL into recognized argv 
	const in_sw_tab_t* action_sw = NULL;
	const char** argv = uSvc->argv.begin();
	for (++argv; argv < end; argv++)
	{
		const in_sw_tab_t* sw = findSwitch(&trace_action_in_sw_table[0], *argv);
		if (sw)
		{
			if (action_sw)
				usage(uSvc, TRACE_ERR_CONFLICT_ACTS, action_sw->in_sw_name, sw->in_sw_name);
			else
				action_sw = sw;

			*argv = NULL;
		}
	}

	if (!action_sw) {
		usage(uSvc, TRACE_ERR_ACT_NOTFOUND);
	}

	// search for action's parameters, set NULL into recognized argv 
	TraceSession session(*getDefaultMemoryPool());
	argv = uSvc->argv.begin();
	for (++argv; argv < end; argv++)
	{
		if (!*argv)
			continue;

		const in_sw_tab_t* sw = findSwitch(&trace_option_in_sw_table[0], *argv);
		if (!sw)
			continue;

		*argv = NULL;

		switch (sw->in_sw)
		{
		case IN_SW_TRACE_CONFIG:
			switch (action_sw->in_sw)
			{
				case IN_SW_TRACE_STOP:
				case IN_SW_TRACE_SUSPEND:
				case IN_SW_TRACE_RESUME:
				case IN_SW_TRACE_LIST:
					usage(uSvc, TRACE_ERR_PARAM_ACT_NOTCOMPAT, sw->in_sw, action_sw->in_sw_name);
					break;
			}

			if (!session.ses_config.empty())
				usage(uSvc, TRACE_ERR_SWITCH_ONCE, sw->in_sw_name);

			argv++;
			if (argv < end)
				session.ses_config = *argv;
			else
				usage(uSvc, TRACE_ERR_PARAM_VAL_MISS, sw->in_sw_name);
			break;

		case IN_SW_TRACE_NAME:
			switch (action_sw->in_sw)
			{
				case IN_SW_TRACE_STOP:
				case IN_SW_TRACE_SUSPEND:
				case IN_SW_TRACE_RESUME:
				case IN_SW_TRACE_LIST:
					usage(uSvc, TRACE_ERR_PARAM_ACT_NOTCOMPAT, sw->in_sw_name, action_sw->in_sw_name);
					break;
			}

			if (!session.ses_name.empty())
				usage(uSvc, TRACE_ERR_SWITCH_ONCE, sw->in_sw_name);

			argv++;
			if (argv < end)
				session.ses_name = *argv;
			else
				usage(uSvc, TRACE_ERR_PARAM_VAL_MISS, sw->in_sw_name);
			break;

		case IN_SW_TRACE_ID:
			switch (action_sw->in_sw)
			{
				case IN_SW_TRACE_START:
				case IN_SW_TRACE_LIST:
					usage(uSvc, TRACE_ERR_PARAM_ACT_NOTCOMPAT, sw->in_sw_name, action_sw->in_sw_name);
					break;
			}

			if (session.ses_id)
				usage(uSvc, TRACE_ERR_SWITCH_ONCE, sw->in_sw_name);

			argv++;
			if (argv < end)
			{
				session.ses_id = atol(*argv);
				if (!session.ses_id)
					usage(uSvc, TRACE_ERR_PARAM_INVALID, *argv, sw->in_sw_name);
			}
			else
				usage(uSvc, TRACE_ERR_PARAM_VAL_MISS, sw->in_sw_name);
			break;

		default:
			fb_assert(false);
		}
		*argv = NULL;
	}

	// search for authentication parameters
	string svc_name, user, pwd;
	bool adminRole = false;
	argv = uSvc->argv.begin();
	for (++argv; argv < end; argv++)
	{
		if (!*argv)
			continue;

		const in_sw_tab_t* sw = findSwitch(&trace_auth_in_sw_table[0], *argv);
		if (!sw) {
			usage(uSvc, TRACE_ERR_SWITCH_UNKNOWN, *argv);
		}

		switch (sw->in_sw)
		{
		case IN_SW_TRACE_USERNAME:
			if (!user.empty())
				usage(uSvc, TRACE_ERR_SWITCH_ONCE, sw->in_sw_name);

			argv++;
			if (argv < end)
				user = *argv;
			else
				usage(uSvc, TRACE_ERR_PARAM_VAL_MISS, sw->in_sw_name);
			break;

		case IN_SW_TRACE_PASSWORD:
			if (!pwd.empty())
				usage(uSvc, TRACE_ERR_SWITCH_ONCE, sw->in_sw_name);

			argv++;
			if (argv < end)
				pwd = *argv;
			else
				usage(uSvc, TRACE_ERR_PARAM_VAL_MISS, sw->in_sw_name);
			break;

		case IN_SW_TRACE_TRUSTED_USER:
			if (!uSvc->isService())
				usage(uSvc, TRACE_ERR_SWITCH_SVC_ONLY, sw->in_sw_name);

			if (!user.empty())
				usage(uSvc, TRACE_ERR_SWITCH_ONCE, sw->in_sw_name);

			argv++;
			if (argv < end)
				user = *argv;
			else
				usage(uSvc, TRACE_ERR_PARAM_VAL_MISS, sw->in_sw_name);
			break;

		case IN_SW_TRACE_TRUSTED_ROLE:
			if (!uSvc->isService())
				usage(uSvc, TRACE_ERR_SWITCH_SVC_ONLY, sw->in_sw_name);
			
			adminRole = true;
			break;

		case IN_SW_TRACE_SERVICE_NAME:
			if (uSvc->isService())
				continue;

			if (!svc_name.empty())
				usage(uSvc, TRACE_ERR_SWITCH_ONCE, sw->in_sw_name);

			argv++;
			if (argv < end)
				svc_name = *argv;
			else
				usage(uSvc, TRACE_ERR_PARAM_VAL_MISS, sw->in_sw_name);

		default:
			fb_assert(false);
		}
	}

	// validate missed action's parameters and perform action
	if (!session.ses_id) 
	{
		switch (action_sw->in_sw)
		{
			case IN_SW_TRACE_STOP:
			case IN_SW_TRACE_SUSPEND:
			case IN_SW_TRACE_RESUME:
				usage(uSvc, TRACE_ERR_SWITCH_PARAM_MISS, "ID", action_sw->in_sw_name);
				break;
		}
	}

	if (session.ses_config.empty())
	{
		if (action_sw->in_sw == IN_SW_TRACE_START) {
			usage(uSvc, TRACE_ERR_SWITCH_PARAM_MISS, "CONFIG", action_sw->in_sw_name);
		}
	}


	traceSvc->setAttachInfo(svc_name, user, pwd, adminRole);

	switch (action_sw->in_sw)
	{
	case IN_SW_TRACE_START:
		traceSvc->startSession(session, true);
		break;

	case IN_SW_TRACE_STOP:
		traceSvc->stopSession(session.ses_id);
		break;

	case IN_SW_TRACE_SUSPEND:
		traceSvc->setActive(session.ses_id, false);
		break;

	case IN_SW_TRACE_RESUME:
		traceSvc->setActive(session.ses_id, true);
		break;

	case IN_SW_TRACE_LIST:
		traceSvc->listSessions();
		break;

	default:
		fb_assert(false);
	}
}

} // namespace Firebird
