Firebird Database Server 2.0 ALPHA 1
==================================================

  ** IMPORTANT **

  See note below regarding installation over
  older versions of Firebird

  ** ********* **

This document is a guide to installing this package of
Firebird 2.0 Alpha 1 on the Win32 platform. These notes
refer to the installation package itself, rather than
Firebird 2.0 in general.


Contents
--------

o Before installation
o Installing on systems with older versions of Firebird
o New features of the installer
o Deprecated features related to installation
o Uninstallation
o Other Notes
o Installation from a batch file


Before installation
-------------------

It is recommended that you UNINSTALL all previous
versions of Firebird or InterBase
before installing this package.


Installation on systems with older version of Firebird
------------------------------------------------------

  **** IMPORTANT ****

The format of the security database has changed. Do
not install Firebird 2.0 Alpha 1 into a directory that
contains a previous version of Firebird.

The installer always retains an existing security
database if it finds one. It never overwrites it. If you
attempt to use an older version of the security database
with Firebird 2.0 Alpha 1 or later you will receive this
message:

  cannot attach to password database

For this reason it is recommended that you install into
a new directory and run the migration script to upgrade
the old database.

Here are the steps in detail:
  1. Install to a new directory.
  2. Make a file copy of the old security database.
     (Make sure your old Firebird is not running.)
  3. Start firebird using it's new, native security.fdb.
  4. Connect to your old security database as SYSDBA and
     run the script.
  5. Stop firebird.
  6. Copy upgraded database to firebird's home directory.
  7. Set LegacyHash parameter in firebird.conf to 1.
  8. Start firebird.

Now you should be able to connect to firebird 2.0 server
using your old logins and passwords. But be careful - as
long as you have LegacyHash = 1, firebird's security
doesn't work completely. You must change SYSDBA
password, let users change their passwords (yes, since
2.0 each user can change his password himself) and set
LegacyHash back to default value of 0.



New features of the installer in Firebird 2.0.0 Alpha 1
-------------------------------------------------------

There are no new features in Alpha 1.
Internationalization has been temporarily disabled.
It will be re-enabled for Alpha 2.


New features of the installer in Firebird 1.5.2
-----------------------------------------------

Support for several languages has been added to the
installer. This support extends ONLY to the
installation process.


New features of the installer in Firebird 1.5
---------------------------------------------

o This installer now combines the super server and
  classic server binaries into a single installation
  package. You can choose to install one or the other,
  but not both. To switch server type you need to
  uninstall and re-install.

o The rules for library installation have changed
  considerably. They are explained in detail in
    .\docs\README.Win32LibraryInstallation.txt
  which will be available to you after installation.

  As a result of these new rules the installer
  checks for an existing install of Firebird or
  InterBase.

  - If Firebird 1.5 is already installed
    it will attempt to install over it. If
    the server is running it will halt the install.

  - If another version of Firebird or InterBase is
    already installed it will warn the user. If
    the user continues the installer will install
    Firebird and set up registry entries but it
    will not configure Firebird to run, either as
    a service or as an application. This must
    be done manually.

  - The installer has a new commandline option
      /force
    which allows those with a 'devil may care'
    attitude to override the above.


o If an amended firebird.conf exists in the
  installation directory it is saved as:
    firebird.conf.saved.n
  where n is a number. The installer always installs
  the default firebird.conf file. This is to guarantee
  consistency to the installation process. Otherwise
  the installer would have to parse the existing (and
  possibly broken) configuration file.


Deprecated Features related to installation
-------------------------------------------

o Firebird 1.0 reserved a new registry key for
  Firebird use. It was:

    HKLM\SOFTWARE\FirebirdSQL
  This is now deprecated and will be deleted by the
  installer. If you have applications which rely
  on this key you should add it back manually.
  However, it is preferable if you rebuild your
  application to read the new key.

o Earlier Firebird 1.5 release candidates installed
  fbclient.dll in the <system> directory. This
  practice is now deprecated. An option to install
  it into the <system> directory is available at
  install time. However, it is preferable if you
  rebuild your applications to conform  to the new
  usage of fbclient.


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


