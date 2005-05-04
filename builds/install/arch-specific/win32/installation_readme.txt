Firebird Database Server 2.0 ALPHA 2
==================================================

  ** IMPORTANT **

  See note below regarding installation over
  older versions of Firebird

  ** ********* **

This document is a guide to installing this package of
Firebird 2.0 Alpha 2 on the Win32 platform. These notes
refer to the installation package itself, rather than
Firebird 2.0 in general.


Contents
--------

o Before installation
o Installing on systems with older versions of Firebird
o Installing on systems with Firebird 2.0
o New features of the installer
o Known installation problems
o Deprecated features related to installation
o Uninstallation
o Other Notes
o Installation from a batch file


Before installation
-------------------

It is recommended that you UNINSTALL all previous
versions of Firebird or InterBase before installing
this package.


Installation on systems with older version of Firebird
------------------------------------------------------

The security database has been changed. The new
database is called security2.fdb and uses a
strengthened password encryption algorithm. You can
find out more info about the new security features in
doc/README.sha1.txt.

The new database structure and the changed encryption
presents several migration problems for sites that have
many user ids. The installer makes no attempt to carry
out this migration.  For Alpha 1 it must be done
manually. A possible migration technique is available
in misc/upgrade/security/. A future test release will
address the issue of user migration more fully.


Installing on systems with Firebird 2.0 
---------------------------------------

The installer cannot automatically detect if a
Firebird 2.0 Alpha 1 or Alpha 2 server is running. 
Therefore you will receive no warning until the 
installer attempts to overwrite an active file.


New features of the installer
-----------------------------

o If an existing firebird.conf is detected the
  installer will install the default conf file
  as:

  firebird.conf.default.

  The existing firebird.conf file will be untouched.
  This is to ensure that access restrictions are
  maintained after an upgrade.


o Support for several languages has been added to the
  installer. This support extends ONLY to the
  installation process.


Known installation problems
---------------------------

A client-only install is in fact installing the
server executable. This will be fixed in a future
beta release. In the meantime the work-around is
to delete the .exe. This can be automated by using
a scripted install.


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


