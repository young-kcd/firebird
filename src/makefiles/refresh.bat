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
rem set LAST_BUILD=`cat last_fetch`
set LAST_BUILD=%1

set STATUS=OK

rem if %SYS_TYPE = 'APOLLO' -o %SYSTEM = 'NXT' 
rem     DATESTAMP=`date | awk '{print %3 "-" %2 "-" %6}'`
rem else
rem     DATESTAMP=`date +"%d-%h-19%y"`
rem fi

cmd /c fetch_comp %LAST_BUILD% alice
if errorlevel 1 set STATUS=FAIL
cmd /c fetch_comp %LAST_BUILD% burp
if errorlevel 1 set STATUS=FAIL
cmd /c fetch_comp %LAST_BUILD% csv
if errorlevel 1 set STATUS=FAIL
cmd /c fetch_comp %LAST_BUILD% dsql
if errorlevel 1 set STATUS=FAIL
cmd /c fetch_comp %LAST_BUILD% dudley
if errorlevel 1 set STATUS=FAIL
cmd /c fetch_comp %LAST_BUILD% gpre
if errorlevel 1 set STATUS=FAIL
cmd /c fetch_comp %LAST_BUILD% intl
if errorlevel 1 set STATUS=FAIL
cmd /c fetch_comp %LAST_BUILD% isql
if errorlevel 1 set STATUS=FAIL
cmd /c fetch_comp %LAST_BUILD% journal
if errorlevel 1 set STATUS=FAIL
cmd /c fetch_comp %LAST_BUILD% jrd
if errorlevel 1 set STATUS=FAIL
cmd /c fetch_comp %LAST_BUILD% msgs
if errorlevel 1 set STATUS=FAIL
cmd /c fetch_comp %LAST_BUILD% pipe
if errorlevel 1 set STATUS=FAIL
cmd /c fetch_comp %LAST_BUILD% pyxis
if errorlevel 1 set STATUS=FAIL
cmd /c fetch_comp %LAST_BUILD% qli
if errorlevel 1 set STATUS=FAIL
cmd /c fetch_comp %LAST_BUILD% register
if errorlevel 1 set STATUS=FAIL
cmd /c fetch_comp %LAST_BUILD% remote
if errorlevel 1 set STATUS=FAIL
cmd /c fetch_comp %LAST_BUILD% isc_lock
if errorlevel 1 set STATUS=FAIL
cmd /c fetch_comp %LAST_BUILD% utilities
if errorlevel 1 set STATUS=FAIL
cmd /c fetch_comp %LAST_BUILD% wal
if errorlevel 1 set STATUS=FAIL
cmd /c fetch_comp %LAST_BUILD% examples
if errorlevel 1 set STATUS=FAIL

cmd /c fetch_comp %LAST_BUILD% builds
if errorlevel 1 set STATUS=FAIL
cd WIN_NT\original
rem platform WIN_NT
cd ..\..

if "%STATUS%" == "FAIL"  goto err
rem copy last_fetch prior_fetch
rem echo %DATESTAMP > last_fetch

:err
rem build_kit WIN_NT
