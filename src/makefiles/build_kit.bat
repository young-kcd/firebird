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
set ISC_USER=builder
set ISC_PASSWORD=builder
touch dsql\parse.c
rem The above is to prevent the make from trying to yacc and sed parse.c,
rem which destroys the file on nt builds. This should be moved into the
rem make file at some point -reed

cmd /c "cd jrd && nmake gds.h"
if errorlevel 1 goto end
cmd /c "cd dsql && nmake dsql_objects"
if errorlevel 1 goto end
cmd /c "cd lock && nmake lock.obj"
if errorlevel 1 goto end
cmd /c "cd remote && nmake rem_objects"
if errorlevel 1 goto end
cmd /c "cd intl && nmake intl_objects"
if errorlevel 1 goto end
cmd /c "cd wal && nmake wal_objects"
if errorlevel 1 goto end
cmd /c "cd jrd && nmake gdsshr.dll"
if errorlevel 1 goto end
rem cmd /c "cd jrd && nmake gdsshr_ss.dll"
rem if errorlevel 1 goto end
rem cmd /c "cd jrd && nmake gds_b.lib"
rem if errorlevel 1 goto end
rem cmd /c "cd client && nmake gdsshr.dll"
rem if errorlevel 1 goto end
cmd /c "cd gpre && nmake gpre.exe"
if errorlevel 1 goto end
cmd /c "cd msgs && nmake check_messages.exe"
if errorlevel 1 goto end
cmd /c "cd msgs && check_messages -d yoda:/usr/gds.yoda/dev/msgs/master_msg_db"
if errorlevel 1 goto end
cmd /c "cd msgs && nmake interbase.msg"
if errorlevel 1 goto end
cmd /c "cd jrd && nmake includes.msg"
if errorlevel 1 goto end
cmd /c "cd dudley && nmake dudley.exe"
if errorlevel 1 goto end
cmd /c "cd intl && nmake intl"
if errorlevel 1 goto end
cmd /c "cd isql && nmake isql.exe"
if errorlevel 1 goto end
cmd /c "cd burp && nmake burp.exe"
if errorlevel 1 goto end
cmd /c "cd qli && nmake qli.exe"
if errorlevel 1 goto end
cmd /c "cd lock && nmake lock_print.exe"
if errorlevel 1 goto end
copy lock\config_params interbase\isc_config
cmd /c "cd remote && nmake nt_server.exe"
if errorlevel 1 goto end
cmd /c "cd alice && nmake alice.exe"
if errorlevel 1 goto end
cmd /c "cd journal && nmake gjrn.exe"
if errorlevel 1 goto end
cmd /c "cd journal && nmake gds_archive.exe"
if errorlevel 1 goto end
cmd /c "cd journal && nmake journal.db"
if errorlevel 1 goto end
cmd /c "cd journal && ..\compress_dbs ..\journal.gdl"
set SAVE_USER=%ISC_USER%
set SAVE_PASSWORD=%ISC_PASSWORD%
set ISC_USER=sysdba
set ISC_PASSWORD=masterkey
interbase\bin\gdef -z -r journal.gdl
interbase\bin\gbak -z journal.gdb interbase/bin/journal.gbak
set ISC_USER=%SAVE_USER%
set ISC_PASSWORD=%SAVE_PASSWORD%
del journal.gdl journal.gdb
cmd /c "cd wal && nmake wal_writer.exe"
if errorlevel 1 goto end
cmd /c "cd wal && nmake wal_print.exe"
if errorlevel 1 goto end
cmd /c "cd wal && nmake driver.exe"
if errorlevel 1 goto end
cmd /c "cd utilities && nmake dba.exe"
if errorlevel 1 goto end
cmd /c "cd utilities && nmake gsec.exe"
if errorlevel 1 goto end
cmd /c "cd utilities && nmake install_reg.exe"
if errorlevel 1 goto end
cmd /c "cd utilities && nmake install_svc.exe"
if errorlevel 1 goto end
del isc4.gdb
set SAVE_USER=%ISC_USER%
set SAVE_PASSWORD=%ISC_PASSWORD%
set ISC_USER=sysdba
set ISC_PASSWORD=masterkey
interbase\bin\isql -z -i utilities\isc4.sql
interbase\bin\gdef -z utilities\isc4.gdl
interbase\bin\gsec -da isc4.gdb -add sysdba -pw masterkey
interbase\bin\gbak -z isc4.gdb interbase\bin\isc4.gbk
set ISC_USER=%SAVE_USER%
set ISC_PASSWORD=%SAVE_PASSWORD%
copy isc4.gdb interbase\isc4.gdb
del isc4.gdb
echo sm ibase.h interbase\include\ibase.h > sm_include
echo sm perf.h interbase\include\perf.h >> sm_include
echo sm ibase.h >> sm_include
echo sm gdsold.h >> sm_include
marion -c jrd -x -v -z -d yoda:/usr/gds.yoda/dev/marion.gdb < sm_include
copy ibase.h+gdsold.h interbase\include\gds.h
del sm_include ibase.h gdsold.h
set SAVE_USER=%ISC_USER%
set SAVE_PASSWORD=%ISC_PASSWORD%
set ISC_USER=sysdba
set ISC_PASSWORD=masterkey
cmd /c "cd examples && nmake examples"
if errorlevel 1 goto end
cmd /c "cd example4 && nmake v4_examples"
if errorlevel 1 goto end
interbase\bin\gbak yoda:/usr/gds.yoda/dev/qli/master_help_db interbase\help\help.gbk
del interbase\help\help.gdb
interbase\bin\gbak -r interbase\help\help.gbk interbase\help\help.gdb
set ISC_USER=%SAVE_USER%
set ISC_PASSWORD=%SAVE_PASSWORD%
cmd /c "cd msgs && nmake locales.msg"
if errorlevel 1 goto end

copy interbase\isc_config interbase\ibconfig
del  interbase\isc_config
copy interbase\isc_ins.hlp interbase\ib_instl.hlp
del  interbase\isc_ins.hlp
copy interbase\interbase.msg interbase\interbas.msg
del  interbase\interbase.msg
copy interbase\bin\journal.gbak interbase\bin\journal.gbk
del  interbase\bin\journal.gbak
copy interbase\bin\gds_archive.exe interbase\bin\ibarchiv.exe
del  interbase\bin\gds_archive.exe
copy interbase\bin\gds_install.exe interbase\bin\instreg.exe
del  interbase\bin\gds_install.exe
copy interbase\bin\gds_install_service.exe interbase\bin\instsvc.exe
del  interbase\bin\gds_install_service.exe
copy interbase\bin\gds_lock_print.exe interbase\bin\iblockpr.exe
del  interbase\bin\gds_lock_print.exe
copy interbase\bin\gds_remote_server.exe interbase\bin\ibremote.exe
del  interbase\bin\gds_remote_server.exe
copy interbase\bin\gds_wal_print.exe interbase\bin\ibwalpr.exe
del  interbase\bin\gds_wal_print.exe
copy interbase\bin\gds_wal_writer.exe interbase\bin\ibwalwrt.exe
del  interbase\bin\gds_wal_writer.exe
copy interbase\bin\iscinstall.exe interbase\bin\ibinstal.exe
del  interbase\bin\iscinstall.exe
copy interbase\examples\v3\*.gbak interbase\examples\v3\*.gbk
del  interbase\examples\v3\*.gbak

:end
