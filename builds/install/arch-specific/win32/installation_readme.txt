Firebird Database Server 2.1
==================================================


This document is a guide to installing this package of
Firebird 2.1 on the Win32 platform. These notes refer
to the installation package itself, rather than
Firebird 2.1 in general. In addition, these notes are
primarily aimed at users of the binary installer.

It is assumed that readers of this document are already
familiar Firebird 2.0. If you are evaluating Fb 2.1 as
part of a migration from Fb 1.5 you are advised to
review the Fb 2.0 documentation to understand the
changes made between 1.5 and 2.0.


Contents
--------

o Before installation
o Known installation problems
o Uninstallation
o Other Notes
o Installation from a batch file


Before installation
-------------------

It is recommended that you UNINSTALL all previous
versions of Firebird or InterBase before installing
this package. It is especially important to verify that
fbclient.dll and gds32.dll are removed from <system32>.


Known installation problems
---------------------------

o It is only possible to use the binary installer
  to install the default instance of Firebird 2.1. If
  you wish to install additional, named instances you
  should manually install them with the zipped install
  image.

o Unfortunately, (at the time of Beta 2) the installer
  cannot detect if a previous version of Firebird
  Classic server are running.

o There are known areas of overlap between the
  32-bit and 64-bit installs:

  - The service installer (instsvc) uses the same
    default instance name for 32-bit and 64-bit
    installations. This is by design. Services exist
    in a single name space.

  - If the 32-bit and 64-bit control panel applets are
    installed they will both point to the same default
    instance.

o It is possible to deploy the client library
  (fbclient or gds32) to the <system32> directory.
  However, the necessary microsoft runtime libraries
  will be missing. To resolve this it is necessary to
  manually install vcredist.exe. Different versions are
  available for x86 here:

    http://www.microsoft.com/downloads/details.aspx?familyid=32BC1BEE-A3F9-4C13-9C99-220B62A191EE&displaylang=en

  and x64 here:

    http://www.microsoft.com/downloads/details.aspx?familyid=90548130-4468-4BBC-9673-D6ACABD5D13B&displaylang=en


Uninstallation
--------------

o It is preferred that this package be uninstalled
  correctly using the uninstallation application
  supplied. This can be called from the Control Panel.
  Alternatively it can be uninstalled by running
  unins000.exe directly from the installation
  directory.

o If Firebird is running as an application (instead of
  as a service) it is recommended that you manually
  stop the server before running the uninstaller. This
  is because the uninstaller cannot stop a running
  application. If a server is running during the
  uninstall the uninstall will complete with errors.
  You will have to delete the remnants by hand.

o Uninstallation leaves four files in the install
  directory:

  - aliases.conf
  - firebird.conf
  - firebird.log
  - security2.fdb

  This is intentional. These files are all
  potentially modifiable by users and may be required
  if Firebird is re-installed in the future. They can
  be deleted if no longer required.

o A new feature of the uninstaller is an option to
  run it with the /CLEAN parameter. This will check
  the shared file count of each of the above files. If
  possible it will delete them.


Other Notes
-----------

  Firebird requires WinSock2. All Win32 platforms
  should have this, except for Win95. A test for the
  Winsock2 library is made during install. If it is
  not found the install will fail. You can visit
  this link:

    http://support.microsoft.com/default.aspx?scid=kb;EN-US;q177719

  to find out how to go about upgrading.


Installation from a batch file
------------------------------

The setup program can be run from a batch file.
Please see this document:

    installation_scripted.txt

for full details.


