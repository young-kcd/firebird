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
cmd /c "cd jrd && make gds.h"
if errorlevel 1 goto end
cmd /c "cd dsql && make dsql_objects"
if errorlevel 1 goto end
cmd /c "cd lock && make lock.obj"
if errorlevel 1 goto end
cmd /c "cd remote && make rem_objects"
if errorlevel 1 goto end
cmd /c "cd intl && make intl_objects"
if errorlevel 1 goto end
cmd /c "cd wal && make wal_objects"
if errorlevel 1 goto end
cmd /c "cd jrd && make gdsshr.dll"
if errorlevel 1 goto end
rem cmd /c "cd jrd && make gds_b.lib"
rem if errorlevel 1 goto end
cmd /c "cd gpre && make gpre.exe"
if errorlevel 1 goto end
cmd /c "cd msgs && make check_messages.exe"
if errorlevel 1 goto end
cmd /c "cd msgs && check_messages -d yoda:/usr/gds/dev/msgs/master_msg_db"
if errorlevel 1 goto end
cmd /c "cd msgs && make interbase.msg"
if errorlevel 1 goto end
cmd /c "cd jrd && make includes.msg"
if errorlevel 1 goto end
cmd /c "cd dudley && make dudley.exe"
if errorlevel 1 goto end
cmd /c "cd intl && make intl"
if errorlevel 1 goto end
cmd /c "cd isql && make isql.exe"
if errorlevel 1 goto end
cmd /c "cd burp && make burp.exe"
if errorlevel 1 goto end
cmd /c "cd qli && make qli.exe"
if errorlevel 1 goto end
cmd /c "cd lock && make lock_print.exe"
if errorlevel 1 goto end
copy lock\config_params interbase\isc_config
cmd /c "cd remote && make inet_server.exe"
if errorlevel 1 goto end
cmd /c "cd alice && make alice.exe"
if errorlevel 1 goto end
cmd /c "cd journal && make gjrn.exe"
if errorlevel 1 goto end
cmd /c "cd journal && make gds_archive.exe"
if errorlevel 1 goto end
cmd /c "cd journal && make journal.db"
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
cmd /c "cd wal && make wal_writer.exe"
if errorlevel 1 goto end
cmd /c "cd wal && make wal_print.exe"
if errorlevel 1 goto end
cmd /c "cd wal && make driver.exe"
if errorlevel 1 goto end
cmd /c "cd utilities && make dba.exe"
if errorlevel 1 goto end
cmd /c "cd utilities && make gsec.exe"
if errorlevel 1 goto end
rem cmd /c "cd utilities && make install_reg.exe"
rem if errorlevel 1 goto end
rem cmd /c "cd utilities && make install_svc.exe"
rem if errorlevel 1 goto end
del isc4.gdb
set SAVE_USER=%ISC_USER%
set SAVE_PASSWORD=%ISC_PASSWORD%
set ISC_USER=sysdba
set ISC_PASSWORD=masterkey
interbase\bin\isql -z -i utilities\isc4.sql
interbase\bin\gdef -z utilities\isc4.gdl
interbase\bin\gsec -da isc4.gdb -add sysdba -pw masterkey
interbase\bin\gbak -z isc4.gdb interbase\bin\isc.gbk
set ISC_USER=%SAVE_USER%
set ISC_PASSWORD=%SAVE_PASSWORD%
ren isc4.gdb interbase\isc4.gdb
echo sm ibase.h interbase\include\ibase.h > sm_include
echo sm perf.h interbase\include\perf.h >> sm_include
echo sm ibase.h >> sm_include
echo sm gdsold.h >> sm_include
marion -c jrd -x -v -z -d yoda:/usr/gds/dev/marion.gdb < sm_include
copy ibase.h+gdsold.h interbase\include\gds.h
del sm_include ibase.h gdsold.h
cmd /c "cd examples && make examples"
if errorlevel 1 goto end
set SAVE_USER=%ISC_USER%
set SAVE_PASSWORD=%ISC_PASSWORD%
set ISC_USER=sysdba
set ISC_PASSWORD=masterkey
cmd /c "cd example4 && make v4_examples"
if errorlevel 1 goto end
interbase\bin\gbak yoda:/usr/gds/dev/qli/master_help_db interbase\help\help.gbk
del interbase\help\help.gdb
interbase\bin\gbak -r interbase\help\help.gbak interbase\help\help.gdb
set ISC_USER=%SAVE_USER%
set ISC_PASSWORD=%SAVE_PASSWORD%
cmd /c "cd msgs && make locales.msg"
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
rem copy interbase\bin\gds_install.exe interbase\bin\instreg.exe
rem del  interbase\bin\gds_install.exe
rem copy interbase\bin\gds_install_service.exe interbase\bin\instsvc.exe
rem del  interbase\bin\gds_install_service.exe
copy interbase\bin\gds_lock_print.exe interbase\bin\iblockpr.exe
del  interbase\bin\gds_lock_print.exe
copy interbase\bin\gds_inet_server.exe interbase\bin\ibremote.exe
del  interbase\bin\gds_inet_server.exe
copy interbase\bin\gds_wal_print.exe interbase\bin\ibwalpr.exe
del  interbase\bin\gds_wal_print.exe
copy interbase\bin\gds_wal_writer.exe interbase\bin\ibwalwrt.exe
del  interbase\bin\gds_wal_writer.exe
copy interbase\bin\iscinstall.exe interbase\bin\ibinstal.exe
del  interbase\bin\iscinstall.exe
copy interbase\examples\v3\*.gbak interbase\examples\v3\*.gbk
del  interbase\examples\v3\*.gbak

:end
