# The contents of this file are subject to the Interbase Public
# License Version 1.0 (the "License"); you may not use this file
# except in compliance with the License. You may obtain a copy
# of the License at http://www.Inprise.com/IPL.html
#
# Software distributed under the License is distributed on an
# "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
# or implied. See the License for the specific language governing
# rights and limitations under the License.
#
# The Original Code was created by Inprise Corporation
# and its predecessors. Portions created by Inprise Corporation are
# Copyright (C) Inprise Corporation.
#
# All Rights Reserved.
# Contributor(s): ______________________________________.
# $Id: bind_gds_s.sco,v 1.1 2001-07-12 06:32:03 bellardo Exp $
#ident "InterBase V4.0 shared library"
#address .text 0xa4800000
#address .data 0xa4c00000
#target /shlib/libgds_s
#branch
    gds__alloc			1
    gds__attach_database	2
    gds__blob_info		3
    gds__blob_size		4
    gds__cancel_blob		5
    gds__cancel_events		6
    gds__close			7
    gds__close_blob		8
    gds__commit_retaining	9
    gds__commit_transaction	10
    gds__compile_request	11
    gds__compile_request2	12
    gds__create_blob		13
    gds__create_blob2		14
    gds__create_database	15
    gds__database_info		16
    gds__ddl			17
    gds__declare		18
    gds__decode			19       
    gds__decode_date		20
    gds__describe		21
    gds__describe_bind		22
    gds__detach_database	23
    gds__dsql_finish		24
    gds__edit			25
    gds__enable_subsystem	26
    gds__encode			27
    gds__encode_date		28
    gds__event_block		29
    gds__event_counts		30
    gds__event_wait		31
    gds__execute		32
    gds__execute_immediate	33
    gds__fetch			34
    gds__free			35
    gds__ftof			36
    gds__get_segment		37
    gds__get_slice		38
    gds__interprete		39
    gds__log			40
    gds__msg_format		41
    gds__msg_lookup		42
    gds__msg_open		43
    gds__msg_put		44
    gds__open			45
    gds__open_blob		46
    gds__open_blob2		47
    gds__parse_bpb		48
    gds__prefix			49
    gds__prepare		50
    gds__prepare_transaction	51
    gds__prepare_transaction2	52
    gds__print_blr		53
    gds__print_status		54
    gds__put_error		55
    gds__put_segment		56
    gds__put_slice		57
    gds__qtoq			58
    gds__que_events		59
    gds__receive		60
    gds__reconnect_transaction	61
    gds__register_cleanup	62
    gds__release_request	63
    gds__request_info		64
    gds__rollback_transaction	65
    gds__send			66
    gds__set_debug		67
    gds__sqlcode		68
    gds__start_and_send		69
    gds__start_multiple		70
    gds__start_request		71
    gds__start_transaction	72
    gds__temp_file		73
    gds__thread_enable		74
    gds__thread_enter		75
    gds__thread_exit		76
    gds__thread_start		77
    gds__to_sqlda		78
    gds__transaction_info	79
    gds__unwind_request		80
    gds__vax_integer		81
    gds__version		82
    gds__vtof			83
    gds__vtov			84
    blob__display		85
    blob__dump			86
    blob__edit			87
    blob__load			88
    BLOB_close			89
    BLOB_display		90
    BLOB_dump			91
    BLOB_edit			92
    BLOB_get			93
    BLOB_load			94
    BLOB_open			95
    BLOB_put			96
    Bopen			97
    perf_format			98
    perf_get_info		99
    perf_report			100
    isc_attach_database		101
    isc_blob_info		102
    isc_cancel_blob		103
    isc_cancel_events		104
    isc_close			105
    isc_close_blob		106
    isc_commit_retaining	107
    isc_commit_transaction	108
    isc_compile_request		109
    isc_compile_request2	110
    isc_create_blob		111
    isc_create_blob2		112
    isc_create_database		113
    isc_database_info		114
    isc_ddl			115
    isc_declare			116
    isc_decode_date		117
    isc_describe		118
    isc_describe_bind		119
    isc_detach_database		120
    isc_dsql_finish		121
    isc_event_counts		122
    isc_wait_for_event		123
    isc_execute			124
    isc_execute_immediate	125
    isc_fetch			126
    isc_fetch_a			127
    isc_ftof			128
    isc_get_segment		129
    isc_get_slice		130
    isc_interprete		131
    isc_open			132
    isc_open_blob		133
    isc_open_blob2		134
    isc_prepare			135
    isc_prepare_transaction	136
    isc_prepare_transaction2	137
    isc_print_status		138
    isc_put_segment		139
    isc_put_slice		140
    isc_qtoq			141
    isc_que_events		142
    isc_receive			143
    isc_reconnect_transaction	144
    isc_release_request		145
    isc_request_info		146
    isc_rollback_transaction	147
    isc_send			148
    isc_sqlcode			149
    isc_start_and_send		150
    isc_start_multiple		151
    isc_start_request		152
    isc_to_sqlda		153
    isc_transaction_info	154
    isc_unwind_request		155
    isc_version			156
    isc_vtof			157
    isc_vtov			158
    isc_array_lookup_desc	159
    isc_array_lookup_bounds	160
    isc_array_set_desc		161
    isc_array_gen_sdl		162
    isc_array_get_slice		163
    isc_array_put_slice		164
    isc_event_block		165
    gds__seek_blob		166
    isc_seek_blob		167
    isc_encode_date		168
    isc_print_blr		169
    gds__database_cleanup	170
    isc_database_cleanup	171
    isc_set_debug		172
    isc_print_sqlerror		173
    isc_sql_interprete		174
    isc_dsql_release		175
    isc_start_transaction	176
    isc_vax_integer		177
    isc_dsql_execute		178
    isc_dsql_execute_m		179
    isc_dsql_execute_immediate	180
    isc_dsql_execute_immediate_m 181
    isc_dsql_fetch		182
    isc_dsql_fetch_a		183
    isc_dsql_fetch_m		184
    isc_dsql_free_statement	185
    isc_dsql_prepare		186
    isc_dsql_prepare_m		187
    isc_dsql_set_cursor_name	188
    isc_dsql_sql_info		189
    isc_dsql_allocate_statement	190
    isc_dsql_describe		191
    isc_dsql_describe_bind	192
    isc_embed_dsql_close	193
    isc_embed_dsql_declare	194
    isc_embed_dsql_execute	195
    isc_embed_dsql_execute_immed 196
    isc_embed_dsql_fetch	197
    isc_embed_dsql_fetch_a	198
    isc_embed_dsql_open		199
    isc_embed_dsql_prepare	200
    isc_embed_dsql_release	201
    isc_embed_dsql_describe	202
    isc_embed_dsql_describe_bind 203
    isc_baddress		204
    isc_blob_display		205
    isc_blob_dump		206
    isc_blob_edit		207
    isc_blob_load		208
    isc_expand_dpb		209
    gds__log_status		210
    isc_dsql_insert		211
    isc_dsql_insert_m		212
    isc_embed_dsql_insert	213
    isc_transact_request	214
    gds__disable_subsystem	215
    gds__msg_close		216
    isc_dsql_alloc_statement2	217
    isc_dsql_execute2		218
    isc_dsql_execute2_m		219
    isc_dsql_exec_immed2	220
    isc_dsql_exec_immed2_m	221
    isc_embed_dsql_execute2	222
    isc_embed_dsql_exec_immed2	223
    isc_embed_dsql_open2	224
    gds__unregister_cleanup	225
    isc_attach_service		226
    isc_detach_service		227
    isc_query_service		228
    isc_drop_database		229
    isc_dsql_exec_immed3_m	230
    gds__transaction_cleanup	231
    isc_blob_gen_bpb		232
    isc_blob_lookup_desc	233
    isc_blob_set_desc		234
    isc_blob_default_desc	235
    gds__map_blobs		236
    ISC_signal			237
    ISC_signal_cancel		238
    isc_free			239
    BLOB_text_dump		240
    BLOB_text_load		241
    isc_modify_dpb		242
    isc_dsql_fetch2             243
    isc_reset_fpe               244
#objects
    source/jrd/shrinit.bin
    source/jrd/all.bin
    source/jrd/alt.bin
    source/jrd/blb.bin
    source/jrd/blf.bin
    source/jrd/btr.bin
    source/jrd/cch.bin
    source/jrd/cmp.bin
    source/jrd/cvt.bin
    source/jrd/dpm.bin
    source/jrd/dyn.bin
    source/jrd/dyn_def.bin
    source/jrd/dyn_del.bin
    source/jrd/dyn_mod.bin
    source/jrd/enc.bin
    source/jrd/err.bin
    source/jrd/event.bin
    source/jrd/evl.bin
    source/jrd/exe.bin
    source/jrd/ext.bin
    source/jrd/filters.bin
    source/jrd/flu.bin
    source/jrd/fun.bin
    source/jrd/gds.bin
    source/jrd/iberr.bin
    source/jrd/idx.bin
    source/jrd/inf.bin
    source/jrd/ini.bin
    source/jrd/intl.bin
    source/jrd/isc.bin
    source/jrd/isc_file.bin
    source/jrd/jrd.bin
    source/jrd/jrn.bin
    source/jrd/lck.bin
    source/jrd/log.bin
    source/jrd/met.bin
    source/jrd/mov.bin
    source/jrd/nav.bin
    source/jrd/node.bin
    source/jrd/nodebug.bin
    source/jrd/opt.bin
    source/jrd/pag.bin
    source/jrd/par.bin
    source/jrd/perf.bin
    source/jrd/pwd.bin
    source/jrd/rse.bin
    source/jrd/sbm.bin
    source/jrd/sch.bin
    source/jrd/scl.bin
    source/jrd/sdl.bin
    source/jrd/sdw.bin
    source/jrd/sort.bin
    source/jrd/sqz.bin
    source/jrd/sym.bin
    source/jrd/tra.bin
    source/jrd/utl.bin
    source/jrd/val.bin
    source/jrd/vio.bin
    source/jrd/why.bin
    source/jrd/unix.bin
    source/dsql/alld.bin
    source/dsql/errd.bin
    source/dsql/metd.bin
    source/dsql/movd.bin
    source/dsql/parse.bin
    source/dsql/ddl.bin
    source/dsql/dsql.bin
    source/dsql/gen.bin
    source/dsql/hsh.bin
    source/dsql/make.bin
    source/dsql/pass1.bin
    source/dsql/array.bin
    source/dsql/user_dsql.bin
    source/dsql/utld.bin
    source/lock/lock.bin
    source/remote/interface.bin
    source/remote/allr.bin
    source/remote/inet.bin
    source/remote/merge.bin
    source/remote/parser.bin
    source/remote/protocol.bin
    source/remote/remote.bin
    source/jrd/grant.bin
    source/jrd/head5.bin
    source/jrd/allp.bin
    source/jrd/misc.bin
    source/jrd/builtin.bin
    source/intl/ld.bin
    source/intl/cv_narrow.bin
    source/intl/cs_narrow.bin
    source/intl/lc_ascii.bin
    source/intl/lc_latin1.bin
    source/intl/lc_dos.bin
    source/intl/cs_unicode.bin
    source/intl/lc_unicode.bin
    source/intl/cs_jis.bin
    source/intl/cv_jis.bin
    source/intl/lc_jis.bin
    source/intl/ld2.bin
    source/wal/wal.bin
    source/wal/walc.bin
    source/wal/walf.bin
    source/wal/walr.bin
    source/jrd/ail.bin
    source/jrd/old.bin
    source/jrd/rec.bin
    source/jrd/thd.bin
    source/jrd/dfw.bin
    source/jrd/stubs.bin
    source/jrd/rlck.bin
    source/jrd/llio.bin
    source/jrd/cvt2.bin
    source/jrd/dsc.bin
    source/jrd/pcmet.bin
    source/jrd/rng.bin
    source/jrd/tpc.bin
    source/intl/cs_utffss.bin
    source/dsql/preparse.bin
    source/jrd/shut.bin
    source/jrd/svc.bin
    source/jrd/bkm.bin
    source/jrd/inuse.bin
    source/dsql/blob.bin
    source/jrd/isc_ipc.bin
    source/jrd/isc_sync.bin
#    source/jrd/qatest.bin
    source/intl/lc_narrow.bin
## kludge
    source/jrd/87ftol.o
    source/jrd/_fltused.o
#hide linker
    *
#export linker
    gethostid
#init source/jrd/shrinit.bin
    _libgds_setjmp	setjmp
    _libgds_sprintf	sprintf
    _libgds_strlen	strlen
    _libgds_strcmp	strcmp
    _libgds_strcpy	strcpy
    _libgds_exit	exit
    _libgds_longjmp	longjmp
    _libgds__iob	_iob
    _libgds_abort	abort
    _libgds_memset	memset
    _libgds_getpid	getpid
    _libgds_memcpy	memcpy
    _libgds_shmdt	shmdt
    _libgds_memcmp	memcmp
    _libgds_fprintf	fprintf
    _libgds_printf	printf
    _libgds_errno	errno
    _libgds_fopen	fopen
    _libgds_fclose	fclose
    _libgds__filbuf	_filbuf
    _libgds_rewind	rewind
    _libgds_fseek	fseek
    _libgds_ftell	ftell
    _libgds__flsbuf	_flsbuf
    _libgds_sys_errlist	sys_errlist
    _libgds_sys_nerr	sys_nerr
    _libgds_malloc	malloc
    _libgds_gettimeofday	gettimeofday
    _libgds_ctime	ctime
    _libgds_getenv	getenv
    _libgds_lseek	lseek
    _libgds_read	read
    _libgds_open	open
    _libgds_strcat	strcat
    _libgds_fputs	fputs
    _libgds_fputc	fputc
    _libgds_mktemp	mktemp
    _libgds_unlink	unlink
    _libgds_semctl	semctl
    _libgds_semop	semop
    _libgds_select	select
    _libgds_strncpy	strncpy
    _libgds_strchr	strchr
    _libgds_getpwnam	getpwnam
    _libgds_geteuid	geteuid
    _libgds_getpwuid	getpwuid
    _libgds_readlink	readlink
    _libgds_gethostname	gethostname
    _libgds_endpwent	endpwent
    _libgds_getegid	getegid
    _libgds_kill	kill
    _libgds_pipe	pipe
    _libgds_fork	fork
    _libgds_execl	execl
    _libgds_write	write
    _libgds_umask	umask
    _libgds_lockf	lockf
    _libgds_shmget	shmget
    _libgds_shmat	shmat
    _libgds_shmctl	shmctl
    _libgds_ftok	ftok
    _libgds_close	close
    _libgds_fscanf	fscanf
    _libgds_semget	semget
    _libgds_strncmp	strncmp
    _libgds_send	send
    _libgds_socket	socket
    _libgds_connect	connect
    _libgds_recv	recv
    _libgds_fflush	fflush
    _libgds_time	time
    _libgds_localtime	localtime
    _libgds_sleep	sleep
    _libgds_times	times
    _libgds_stat	stat
    _libgds_system	system
    _libgds_fgetc	fgetc
    _libgds_fstat	fstat
    _libgds_getuid	getuid
    _libgds_fgets	fgets
    _libgds_atoi	atoi
    _libgds_alarm	alarm
    _libgds_signal	signal
    _libgds_getservbyname getservbyname
    _libgds_gethostbyname gethostbyname
    _libgds_setsockopt	setsockopt
    _libgds_bind	bind
    _libgds_listen	listen
    _libgds_accept	accept
    _libgds_setreuid	setreuid
    _libgds_setregid	setregid
    _libgds_chdir	chdir
    _libgds_ioctl	ioctl
    _libgds_getsockname	getsockname
    _libgds_getpeername	getpeername
    _libgds_gethostbyaddr gethostbyaddr
    _libgds_sscanf	sscanf
    _libgds_xdrmem_create xdrmem_create
    _libgds_innetgr	innetgr
    _libgds_xdr_enum	xdr_enum
    _libgds_xdr_short	xdr_short
    _libgds_xdr_u_short	xdr_u_short
    _libgds_xdr_long	xdr_long
    _libgds_xdr_opaque	xdr_opaque
    _libgds_xdr_string	xdr_string
    _libgds_FUNCTIONS_entrypoint FUNCTIONS_entrypoint
    _libgds_fcntl	fcntl
    _libgds_fwrite	fwrite
    _libgds_getppid	getppid
    _libgds_setitimer	setitimer
    _libgds_sigset	sigset
    _libgds_getgroups	getgroups
    _libgds_bzero	bzero
    _libgds_shutdown	shutdown
    _libgds_crypt	crypt
    _libgds_pause	pause
    _libgds_xdr_float	xdr_float
    _libgds_xdr_double	xdr_double
    _libgds_sbrk	sbrk
    _libgds_environ	environ
    _libgds_fdopen	fdopen
    _libgds_dup		dup
    _libgds_execle	execle
    _libgds_strrchr	strrchr
    _libgds_waitpid	waitpid
    _libgds__exit	_exit
    _libgds_popen	popen
    _libgds_htonl	htonl
    _libgds_ntohl	ntohl
    _libgds_getcwd	getcwd
    _libgds_pclose	pclose
    _libgds_atexit	atexit
    _libgds_atol	atol
    _libgds_execvp	execvp
    _libgds_nice	nice
    _libgds_putenv	putenv
    _libgds_xdr_free	xdr_free
    _libgds_xdr_wrapstring	xdr_wrapstring
    _libgds_vsprintf	vsprintf
    _libgds_h_errno	h_errno
    _libgds_chmod	chmod
    _libgds_ungetc	ungetc
    _libgds_vfprintf	vfprintf
    _libgds__assert	_assert
    _libgds_access	access
    _libgds_fsync	fsync
    _libgds_sigsuspend	sigsuspend
    _libgds_sigprocmask	sigprocmask
    _libgds_sigaddset	sigaddset
