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
This file contains some basic information about the input files used
to create the 'custom' installable InterBase product on SCO.
For detailed instructions on creating 'custom' installable products 
please refer to chapter 12 "Custom Distribution Mastering Toolkit" in 
the SCO manual "Developer's Topics".

The following files are required to create the SCO custom package
for InterBase

cdmt.config
ccs
ibase.prd
ibase.cmpnt
ibase.pkg
iclient.prd
iclient.cmpnt
iclient.pkg

All are ascii files.
IMPORTANT : Since they are being checked into the common builds 
component of marion, they are being suffixed with a .sco to
uniquely identify them as SCO specific files. However once they
are checked out to use them you SHOULD rename them without the
.sco extension.

How to create the package
1. Login as root since all the subsequent steps will be done
as root.
2. Create an empty directory say /usr/gds.mymachine/custom
3. Copy cdmt.config to that directory.
4. Create a directory under custom called cntl. Copy file ccs 
to that directory.  
5. Create a directory under custom called input. Copy files
ibase.prd
ibase.cmpnt
ibase.pkg
iclient.prd
iclient.cmpnt
iclient.pkg
into that directory
6. Go to the interbase directory in your build area which 
contains the latest built InterBase product which you want
to package. Copy the product to the custom directory using
the following command
tar cvf - . | ( cd /usr/gds.mymachine/custom; tar xf - ) 
6. cd to the custom directory and run the following commands 
to create the InterBase custom package 
a. cdmtParse
b. cdmtCompress
c. cdmtArchive 
7. After the last command you will find that two new directories
would have been created under custom, sso and archives. These
are created by the custom executables. What you are interested
in is the directory archives/TAPE. This directory will contain
InterBase & InterClient packaged in the custom format. You will
see files with names like VOL.000.000, VOL.000.000.files, 
VOL.000.000.sum etc.
8. To install InterBase from this package you need to do the
following:
In custom select option "Install from media image" and then
supply the full path of the directory containing the above 
files, namely /usr/gds.mymachine/custom/archives/TAPE. custom
should show you two installable products, InterBase and 
InterClient. Selecting either and continuing should install 
the product on the system.
9. On the CD, the packaged files should go in the directory
/opt/archives if custom has to automatically recognize them 
off the CD.

NOTE : The above input files to the custom creation utilities
were created for InterBase 5.5. For future ports they need to
be reviewed and modified appropriately to ensure that the 
product is packaged correctly. For instance the file ibase.pkg
contains a block for each file which goes into the package.
So if 6.0 introduces any new files, the port engineer needs to
add a block for each file so that those files will get included
in the package. Also cdmt.config and the .pkg and .cmpnt files 
contain hardcoded references to directories on the machine on 
which they were originally created. These need to be changed
to the appropriate directories on the new build machine.


-- Shailesh Prabhu  1/26/99
