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
if "%1" == "" goto err
if not "%2" == "" goto err

echo Compressing DB in... %1
sed -f ..\$(SYSTEM)\compress.sed %1 > .\sed.TMP
copy .\sed.TMP %1
del .\sed.TMP
goto end

:err
echo "Usage: compress_dbs filename"

:end
