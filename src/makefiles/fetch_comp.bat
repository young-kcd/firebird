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
@echo off
set HEADER=
if "%2" == "examples" set HEADER=-x
if "%2" == "EXAMPLES" set HEADER=-x
if "%2" == "example4" set HEADER=-x
if "%2" == "EXAMPLE4" set HEADER=-x
set SUBDIR=%2
set CWD=..
if "%2" == "builds" set SUBDIR=WIN_NT\original
if "%2" == "builds" set CWD=..\..
if "%2" == "BUILDS" set SUBDIR=WIN_NT\original
if "%2" == "BUILDS" set CWD=..\..
if "%2" == "isc_lock" set SUBDIR=lock
if "%2" == "ISC_LOCK" set SUBDIR=lock
cd %SUBDIR%
marion -c %2 -a -s %1 -z -v %HEADER%
cd %CWD%
