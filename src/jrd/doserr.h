/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		neterr.h
 *	DESCRIPTION:	Common network error message handling
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

#ifndef _REMOTE_DOSERR_H_
#define _REMOTE_DOSERR_H_

#define TCP_ERROR_BEG   WSABASEERR
#define TCP_ERROR_END   (TCP_ERROR_BEG + 10000L)
#define SPX_ERROR_BEG   20000L
#define SPX_ERROR_END   (SPX_ERROR_BEG + 10000L)
#define WNP_ERROR_BEG   30000L
#define WNP_ERROR_END   (WNP_ERROR_BEG + 10000L)
#define LOC_ERROR_BEG   40000L
#define LOC_ERROR_END   (LOC_ERROR_BEG + 10000L)

#ifndef REMOTE_REMOTE_H

#include <winsock2.h>
#include <nwerror.h>
#undef SOCKET_NOT_OPEN			/* Defined in nwerror.h, redefined by nxtw.h */
#undef SOCKET_ALREADY_OPEN		/* Defined in nwerror.h, redefined by nxtw.h */
#include <nxtw.h>
#define INCL_ERRORS
#include <bseerr.h>

#define dos_tcp_wsaeconnrefused                   0
#define dos_tcp_wsaeintr                          1
#define dos_tcp_wsaebadf                         -1
#define dos_tcp_wsaeacces                        -1
#define dos_tcp_wsaefault                        -1
#define dos_tcp_wsaeinval                        -1
#define dos_tcp_wsaemfile                         2
#define dos_tcp_wsaenametoolong                  -1
#define dos_tcp_wsaenotsock                      -1
#define dos_tcp_wsaedestaddrreq                  -1
#define dos_tcp_wsaemsgsize                       3
#define dos_tcp_wsaeprototype                    -1
#define dos_tcp_wsaenoprotoopt                   -1
#define dos_tcp_wsaeprotonosupport                4
#define dos_tcp_wsaesocktnosupport                5
#define dos_tcp_wsaeopnotsupp                     6
#define dos_tcp_wsaepfnosupport                   7
#define dos_tcp_wsaeafnosupport                   8
#define dos_tcp_wsaeaddrinuse                     9
#define dos_tcp_wsaeaddrnotavail                 10
#define dos_tcp_wsaenetdown                      11
#define dos_tcp_wsaenetunreach                   12
#define dos_tcp_wsaenetreset                     13
#define dos_tcp_wsaeconnaborted                  14
#define dos_tcp_wsaeconnreset                    15
#define dos_tcp_wsaenobufs                       16
#define dos_tcp_wsaeisconn                       17
#define dos_tcp_wsaenotconn                      18
#define dos_tcp_wsaeshutdown                     19
#define dos_tcp_wsaetoomanyrefs                  -1
#define dos_tcp_wsaetimedout                     20
#define dos_tcp_wsaehostdown                     21
#define dos_tcp_wsaehostunreach                  22
#define dos_tcp_wsaealready                      23
#define dos_tcp_wsaeinprogress                   24
#define dos_tcp_wsaewouldblock                   25
#define dos_tcp_wsaeloop                         -1
#define dos_tcp_wsasysnotready                   26
#define dos_tcp_wsavernotsupported               27
#define dos_tcp_wsanotinitialised                28

#define dos_spx_name_not_found                   29
#define dos_spx_not_initialized                  30
#define dos_spx_connection_terminated            31
#define dos_spx_no_answer_from_target            32
#define dos_spx_invalid_connection               33
#define dos_spx_connection_table_full            34
#define dos_spx_socket_closed                    35
#define dos_spx_malformed_packet                 36
#define dos_spx_socket_not_opened                37
#define dos_spx_no_dos_memory                    49
#define dos_spx_no_free_ecb                      50
#define dos_spx_lock_failed                      51
#define dos_spx_over_max_limit                   52
#define dos_spx_prev_init                        53

#define dos_wnp_bad_pipe                         38
#define dos_wnp_invalid_handle                   -1
#define dos_wnp_not_enough_memory                39
#define dos_wnp_bad_length                       40
#define dos_wnp_broken_pipe                      41
#define dos_wnp_insufficient_buffer              42
#define dos_wnp_network_busy                     43
#define dos_wnp_no_proc_slots                    44
#define dos_wnp_file_not_found                   45
#define dos_wnp_path_not_found                   46
#define dos_wnp_pipe_busy                        47
#define dos_wnp_invalid_parameter                -1
#define dos_wnp_dev_not_exist                    48
#define dos_wnp_rem_not_list		                54
#define dos_wnp_dup_name			                55
#define dos_wnp_bad_netpath		                56
#define dos_wnp_too_many_cmds		                57
#define dos_wnp_adap_hdw_err		                58
#define dos_wnp_bad_net_resp		                59
#define dos_wnp_unexp_net_err		                60
#define dos_wnp_bad_rem_adap		                61
#define dos_wnp_netname_deleted		             62
#define dos_wnp_network_access_denied            63
#define dos_wnp_bad_dev_type		                64
#define dos_wnp_bad_net_name		                65
#define dos_wnp_too_many_names		             66
#define dos_wnp_too_many_sess		                67
#define dos_wnp_req_not_accep		                68
#define dos_wnp_net_write_fault		             69
#define dos_wnp_pipe_not_connected	             70

#define dos_loc_not_enough_disk_space    	       73


#define MAP_DOS_ERROR(code)                                                  \
   (((code) >= TCP_ERROR_BEG && (code) <= TCP_ERROR_END) ?                   \
                                 MAP_TCP_ERROR((code))                     : \
    ((code) >= SPX_ERROR_BEG && (code) <= SPX_ERROR_END) ?                   \
                                 MAP_SPX_ERROR((code) - SPX_ERROR_BEG)     : \
    ((code) >= WNP_ERROR_BEG && (code) <= WNP_ERROR_END) ?                   \
                                 MAP_WNP_ERROR((code) - WNP_ERROR_BEG)     : \
    ((code) >= LOC_ERROR_BEG && (code) <= LOC_ERROR_END) ?                   \
                                 MAP_LOC_ERROR((code) - LOC_ERROR_BEG)     : \
                                 -2)

#define SUB_DOS_BASE(code)                                                   \
   (((code) >= TCP_ERROR_BEG && (code) <= TCP_ERROR_END) ?                   \
                                 ((code))                                  : \
    ((code) >= SPX_ERROR_BEG && (code) <= SPX_ERROR_END) ?                   \
                                 ((code) - SPX_ERROR_BEG)                  : \
    ((code) >= WNP_ERROR_BEG && (code) <= WNP_ERROR_END) ?                   \
                                 ((code) - WNP_ERROR_BEG)                  : \
    ((code) >= LOC_ERROR_BEG && (code) <= LOC_ERROR_END) ?                   \
                                 ((code) - LOC_ERROR_BEG)                  : \
                                 0)

#define MAP_TCP_ERROR(code)                                                  \
   (code == WSAECONNREFUSED                ? dos_tcp_wsaeconnrefused       : \
    code == WSAEINTR                       ? dos_tcp_wsaeintr              : \
    code == WSAEBADF                       ? dos_tcp_wsaebadf              : \
    code == WSAEACCES                      ? dos_tcp_wsaeacces             : \
    code == WSAEFAULT                      ? dos_tcp_wsaefault             : \
    code == WSAEINVAL                      ? dos_tcp_wsaeinval             : \
    code == WSAEMFILE                      ? dos_tcp_wsaemfile             : \
    code == WSAENAMETOOLONG                ? dos_tcp_wsaenametoolong       : \
    code == WSAENOTSOCK                    ? dos_tcp_wsaenotsock           : \
    code == WSAEDESTADDRREQ                ? dos_tcp_wsaedestaddrreq       : \
    code == WSAEMSGSIZE                    ? dos_tcp_wsaemsgsize           : \
    code == WSAEPROTOTYPE                  ? dos_tcp_wsaeprototype         : \
    code == WSAENOPROTOOPT                 ? dos_tcp_wsaenoprotoopt        : \
    code == WSAEPROTONOSUPPORT             ? dos_tcp_wsaeprotonosupport    : \
    code == WSAESOCKTNOSUPPORT             ? dos_tcp_wsaesocktnosupport    : \
    code == WSAEOPNOTSUPP                  ? dos_tcp_wsaeopnotsupp         : \
    code == WSAEPFNOSUPPORT                ? dos_tcp_wsaepfnosupport       : \
    code == WSAEAFNOSUPPORT                ? dos_tcp_wsaeafnosupport       : \
    code == WSAEADDRINUSE                  ? dos_tcp_wsaeaddrinuse         : \
    code == WSAEADDRNOTAVAIL               ? dos_tcp_wsaeaddrnotavail      : \
    code == WSAENETDOWN                    ? dos_tcp_wsaenetdown           : \
    code == WSAENETUNREACH                 ? dos_tcp_wsaenetunreach        : \
    code == WSAENETRESET                   ? dos_tcp_wsaenetreset          : \
    code == WSAECONNABORTED                ? dos_tcp_wsaeconnaborted       : \
    code == WSAECONNRESET                  ? dos_tcp_wsaeconnreset         : \
    code == WSAENOBUFS                     ? dos_tcp_wsaenobufs            : \
    code == WSAEISCONN                     ? dos_tcp_wsaeisconn            : \
    code == WSAENOTCONN                    ? dos_tcp_wsaenotconn           : \
    code == WSAESHUTDOWN                   ? dos_tcp_wsaeshutdown          : \
    code == WSAETOOMANYREFS                ? dos_tcp_wsaetoomanyrefs       : \
    code == WSAETIMEDOUT                   ? dos_tcp_wsaetimedout          : \
    code == WSAEHOSTDOWN                   ? dos_tcp_wsaehostdown          : \
    code == WSAEHOSTUNREACH                ? dos_tcp_wsaehostunreach       : \
    code == WSAEALREADY                    ? dos_tcp_wsaealready           : \
    code == WSAEINPROGRESS                 ? dos_tcp_wsaeinprogress        : \
    code == WSAEWOULDBLOCK                 ? dos_tcp_wsaewouldblock        : \
    code == WSAELOOP                       ? dos_tcp_wsaeloop              : \
    code == WSASYSNOTREADY                 ? dos_tcp_wsasysnotready        : \
    code == WSAVERNOTSUPPORTED             ? dos_tcp_wsavernotsupported    : \
    code == WSANOTINITIALISED              ? dos_tcp_wsanotinitialised     : \
                                             -1)

#define MAP_SPX_ERROR(code)                                                  \
   (code == (INVALID_CONNECTION - 30000)   ? dos_spx_name_not_found        : \
    code == (NO_SUCH_SEGMENT    - 30000)   ? dos_spx_name_not_found        : \
    code == (NO_SUCH_PROPERTY   - 30000)   ? dos_spx_name_not_found        : \
    code == (NO_SUCH_OBJECT     - 30000)   ? dos_spx_name_not_found        : \
    code == IPXSPX_NOT_INIT                ? dos_spx_not_initialized       : \
    code == SPX_CONNECTION_TERMINATED      ? dos_spx_connection_terminated : \
    code == SPX_NO_ANSWER_FROM_TARGET      ? dos_spx_no_answer_from_target : \
    code == SPX_INVALID_CONNECTION         ? dos_spx_invalid_connection    : \
    code == SPX_CONNECTION_TABLE_FULL      ? dos_spx_connection_table_full : \
    code == 0xFC /* SPX_SOCKET_CLOSED */   ? dos_spx_socket_closed         : \
    code == SPX_MALFORMED_PACKET           ? dos_spx_malformed_packet      : \
    code == SPX_SOCKET_NOT_OPENED          ? dos_spx_socket_not_opened     : \
    code == NO_DOS_MEMORY                  ? dos_spx_no_dos_memory         : \
    code == NO_FREE_ECB                    ? dos_spx_no_free_ecb           : \
    code == WINLOCK_FAILED                 ? dos_spx_lock_failed           : \
    code == OVER_MAX_LIMIT                 ? dos_spx_over_max_limit        : \
    code == IPXSPX_PREV_INIT               ? dos_spx_prev_init             : \
                                             -1)

/* Note:  The constant for SPX_SOCKET_CLOSED was not found in the headers,
   but the value 0xFC is described in the documentation */

#define MAP_WNP_ERROR(code)                                                  \
   (code == ERROR_BAD_PIPE                 ? dos_wnp_bad_pipe              : \
    code == ERROR_INVALID_HANDLE           ? dos_wnp_invalid_handle        : \
    code == ERROR_NOT_ENOUGH_MEMORY        ? dos_wnp_not_enough_memory     : \
    code == ERROR_BAD_LENGTH               ? dos_wnp_bad_length            : \
    code == ERROR_BROKEN_PIPE              ? dos_wnp_broken_pipe           : \
    code == ERROR_INSUFFICIENT_BUFFER      ? dos_wnp_insufficient_buffer   : \
    code == ERROR_NETWORK_BUSY             ? dos_wnp_network_busy          : \
    code == ERROR_NO_PROC_SLOTS            ? dos_wnp_no_proc_slots         : \
    code == ERROR_FILE_NOT_FOUND           ? dos_wnp_file_not_found        : \
    code == ERROR_PATH_NOT_FOUND           ? dos_wnp_path_not_found        : \
    code == ERROR_PIPE_BUSY                ? dos_wnp_pipe_busy             : \
    code == ERROR_UNEXP_NET_ERR            ? dos_wnp_unexp_net_err         : \
    code == ERROR_INVALID_PARAMETER        ? dos_wnp_invalid_parameter     : \
    code == ERROR_DEV_NOT_EXIST            ? dos_wnp_dev_not_exist         : \
    code == ERROR_REM_NOT_LIST		       ? dos_wnp_rem_not_list		      : \
    code == ERROR_DUP_NAME			          ? dos_wnp_dup_name			      : \
    code == ERROR_BAD_NETPATH		          ? dos_wnp_bad_netpath		      : \
    code == ERROR_TOO_MANY_CMDS		       ? dos_wnp_too_many_cmds		   : \
    code == ERROR_ADAP_HDW_ERR		       ? dos_wnp_adap_hdw_err		      : \
    code == ERROR_BAD_NET_RESP		       ? dos_wnp_bad_net_resp		      : \
    code == ERROR_UNEXP_NET_ERR		       ? dos_wnp_unexp_net_err		   : \
    code == ERROR_BAD_REM_ADAP		       ? dos_wnp_bad_rem_adap		      : \
    code == ERROR_NETNAME_DELETED		    ? dos_wnp_netname_deleted		   : \
    code == ERROR_NETWORK_ACCESS_DENIED    ? dos_wnp_network_access_denied : \
    code == ERROR_BAD_DEV_TYPE		       ? dos_wnp_bad_dev_type		      : \
    code == ERROR_BAD_NET_NAME		       ? dos_wnp_bad_net_name		      : \
    code == ERROR_TOO_MANY_NAMES		       ? dos_wnp_too_many_names		   : \
    code == ERROR_TOO_MANY_SESS		       ? dos_wnp_too_many_sess		   : \
    code == ERROR_REQ_NOT_ACCEP		       ? dos_wnp_req_not_accep		   : \
    code == ERROR_NET_WRITE_FAULT		    ? dos_wnp_net_write_fault		   : \
    code == ERROR_PIPE_NOT_CONNECTED	    ? dos_wnp_pipe_not_connected	   : \
                                             -1)

#define MAP_LOC_ERROR(code)                                                  \
   (code == ENOSPC                         ? dos_loc_not_enough_disk_space : \
                                             -1)

#endif /* REMOTE_REMOTE_H */

#endif /* _REMOTE_DOSERR_H_ */
