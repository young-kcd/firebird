;  The contents of this file are subject to the Interbase Public
;  License Version 1.0 (the "License"); you may not use this file
;  except in compliance with the License. You may obtain a copy
;  of the License at http://www.Inprise.com/IPL.html
;
;  Software distributed under the License is distributed on an
;  "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
;  or implied. See the License for the specific language governing
;  rights and limitations under the License.
;
;  The Original Code was created by Inprise Corporation
;  and its predecessors. Portions created by Inprise Corporation are
;  Copyright (C) Inprise Corporation.
;
;  All Rights Reserved.
;  Contributor(s): ______________________________________.

	.386p
	ifndef	??version
?debug	macro
	endm
publicdll macro	name
	public	name
	endm
$comm	macro	name,dist,size,count
	comm	dist name:BYTE:count*size
	endm
	else
$comm	macro	name,dist,size,count
	comm	dist name[size]:BYTE:count
	endm
	endif

JumpTo macro   FunctionName
		mov	ax,ss
		nop
		jmp	 far ptr <FunctionName>
endm

THIN_TEXT	segment word public use16 'CODE'
THIN_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:THIN_TEXT,ds:DGROUP
_DATA	segment word public use16 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public use16 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
THIN_TEXT	segment word public use16 'CODE'

	assume	cs:THIN_TEXT,ds:DGROUP
_THIN_gds__alloc	proc	far
	mov	ax,ss
	nop
	jmp	 far ptr _gds__alloc
_THIN_gds__alloc	endp

	assume	cs:THIN_TEXT,ds:DGROUP
_THIN_gds__free	proc	far
	mov	ax,ss
	nop
	jmp	 far ptr _gds__free
_THIN_gds__free	endp

	assume	cs:THIN_TEXT,ds:DGROUP
_THIN_gds__msg_format	proc	far
	mov	ax,ss
	nop
	jmp	 far ptr _gds__msg_format
_THIN_gds__msg_format	endp

	assume	cs:THIN_TEXT,ds:DGROUP
_THIN_gds__print_status	proc	far
	mov	ax,ss
	nop
	jmp	 far ptr _gds__print_status
_THIN_gds__print_status	endp

	assume	cs:THIN_TEXT,ds:DGROUP
_THIN_gds__temp_file	proc	far
	mov	ax,ss
	nop
	jmp	 far ptr _gds__temp_file
_THIN_gds__temp_file	endp

	assume	cs:THIN_TEXT,ds:DGROUP
_THIN_gds__vax_integer	proc	far
	mov	ax,ss
	nop
	jmp	 far ptr _gds__vax_integer
_THIN_gds__vax_integer	endp

	assume	cs:THIN_TEXT,ds:DGROUP
_THIN_BLOB_dump	proc	far
	mov	ax,ss
	nop
	jmp	 far ptr _BLOB_dump
_THIN_BLOB_dump	endp

	assume	cs:THIN_TEXT,ds:DGROUP
_THIN_BLOB_edit	proc	far
	mov	ax,ss
	nop
	jmp	 far ptr _BLOB_edit
_THIN_BLOB_edit	endp

	assume	cs:THIN_TEXT,ds:DGROUP
_THIN_isc_blob_default_desc	proc	far
	mov	ax,ss
	nop
	jmp	 far ptr _isc_blob_default_desc
_THIN_isc_blob_default_desc	endp

	assume	cs:THIN_TEXT,ds:DGROUP
_THIN_isc_blob_gen_bpb	proc	far
	mov	ax,ss
	nop
	jmp	 far ptr _isc_blob_gen_bpb
_THIN_isc_blob_gen_bpb	endp

	assume	cs:THIN_TEXT,ds:DGROUP
_THIN_isc_blob_lookup_desc	proc	far
	mov	ax,ss
	nop
	jmp	 far ptr _isc_blob_lookup_desc
_THIN_isc_blob_lookup_desc	endp

	assume	cs:THIN_TEXT,ds:DGROUP
_THIN_perf_format	proc	far
	mov	ax,ss
	nop
	jmp	 far ptr _perf_format
_THIN_perf_format	endp

	assume	cs:THIN_TEXT,ds:DGROUP
_THIN_perf_get_info	proc	far
	mov	ax,ss
	nop
	jmp	 far ptr _perf_get_info
_THIN_perf_get_info	endp

	assume	cs:THIN_TEXT,ds:DGROUP
_THIN_isc_sql_interprete        proc	far
	mov	ax,ss
	nop
	jmp	 far ptr _isc_sql_interprete
_THIN_isc_sql_interprete	endp

THIN_TEXT	ends
_DATA	segment word public use16 'DATA'
s@	label	byte
_DATA	ends
THIN_TEXT	segment word public use16 'CODE'
THIN_TEXT	ends
_s@	equ	s@

	extrn	_BLOB_dump:far
	extrn	_BLOB_edit:far
	extrn	_gds__alloc:far
	extrn	_gds__free:far
	extrn	_gds__msg_format:far
	extrn	_gds__print_status:far
	extrn	_gds__temp_file:far
	extrn	_gds__vax_integer:far
	extrn	_isc_blob_default_desc:far
	extrn	_isc_blob_gen_bpb:far
	extrn	_isc_blob_lookup_desc:far
	extrn	_perf_format:far
	extrn	_perf_get_info:far
	extrn	_isc_sql_interprete:far

	publicdll	_THIN_BLOB_dump
	publicdll	_THIN_BLOB_edit
	publicdll	_THIN_gds__alloc
	publicdll	_THIN_gds__free
	publicdll	_THIN_gds__msg_format
	publicdll	_THIN_gds__print_status
	publicdll	_THIN_gds__temp_file
	publicdll	_THIN_gds__vax_integer
	publicdll	_THIN_isc_blob_default_desc
	publicdll	_THIN_isc_blob_gen_bpb
	publicdll	_THIN_isc_blob_lookup_desc
	publicdll	_THIN_perf_format
	publicdll	_THIN_perf_get_info
	publicdll	_THIN_perf_get_info
	publicdll	_THIN_isc_sql_interprete

	end
