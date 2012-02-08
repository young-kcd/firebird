
==================================================
Firebird Database Server $MAJOR.$MINOR.$RELEASE
==================================================

  ** IMPORTANT **

  The ODS has changed since Firebird 1.5. See the notes
  below regarding the new ODS and installation over
  older versions of Firebird.

  ** ********* **

This document is a guide to installing this package of
Firebird $MAJOR.$MINOR.$RELEASE on the Win32 platform. These notes refer
to the installation package itself, rather than
Firebird $MAJOR.$MINOR.$RELEASE in general.


Contents
--------

o Before installation
o New ODS 11.0
o Installation on systems with Firebird 1.5
o Installing on systems with Firebird $MAJOR.$MINOR
o New features of the installer
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


New ODS 11.0
------------

Firebird $MAJOR.$MINOR features a new ODS 11.0 which is wholly
incompatible with the ODS 11.0 used by the previous
alpha releases.

If you are installing this release of Firebird $MAJOR.$MINOR for
the first time over a previously installed Firebird 2
alpha it is recommended that you install into a fresh
directory. This is because an uninstall will preserve
the existing security2 database. However this older
version uses an incompatible ODS so you will be unable
to connect to the server.


Installation on systems with Firebird 1.5
-----------------------------------------

The security database has been changed. The new
database is called security2.fdb and uses a
strengthened password encryption algorithm. You can
find out more info about the new security features in
the release notes.

The new database structure and the changed encryption
presents several migration problems for sites that have
many user ids. The installer makes no attempt to carry
out this migration.  This must be done manually. A
possible migration technique is available in
misc/upgrade/security/.


Installing on systems with Firebird $MAJOR.$MINOR
---------------------------------------

In addition to the ODS change please note that the
installer cannot automatically detect if a
Firebird $MAJOR.$MINOR Alpha or Beta server is running
as a service. Therefore you will receive no warning
until the installer attempts to overwrite an active
file. This can be avoided if you uninstall first.


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

  NOTE: The available languages are determined by
  the localized version of Windows that you are
  running. This means, for example, that Western
  European users will not normally see the slavic
  languages available.


Known installation problems
---------------------------

None are known at this time.


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


