Firebird Database Server 1.5.2 
==================================================

This document is a guide to installing this package of 
Firebird 1.5 on the Win32 platform. These notes refer 
to the installation package itself, rather than 
Firebird 1.5 in general.


Contents
--------

o Before installation
o Installation on systems with Firebird 2.0
o New features of the installer
o Deprecated Features related to installation
o Uninstallation
o Other Notes
o Installation from a batch file


Before installation
-------------------

It is recommended that you UNINSTALL all previous 
versions of Firebird 1.0, Firebird 1.5 or InterBase 
before installing this package.


Installation on systems with Firebird 2.0 Unstable
--------------------------------------------------

The Firebird 1.5.2 installation process cannot detect 
a running version of Firebird 2.0 Unstable. If you have 
been running an early unstable version of Firebird 2.0 
it is essential that you stop the server before 
attempting to install Firebird 1.5.2.


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

The setup program can be run from a batch file. The 
following parameters may be passed:


/SP- 
  Disables the 'This will install... Do you wish to 
  continue?' prompt at the beginning of Setup. 

/SILENT, /VERYSILENT 
  Instructs Setup to be silent or very silent. When 
  Setup is silent the wizard and the background window
  are not displayed but the installation progress 
  window is. When a setup is very silent this 
  installation progress window is not displayed. 
  Everything else is normal so for example error 
  messages during installation are displayed and the 
  startup prompt is (if you haven't disabled it with 
  the '/SP-' command line option explained above) 

  If a restart is necessary and the '/NORESTART' 
  command isn't used (see below) and Setup is silent, 
  it will display a Reboot now? messagebox. If it's 
  very silent it will reboot without asking. 

/NORESTART 
  Instructs Setup not to reboot even if it's necessary.
  
/DIR="x:\dirname"  
  Overrides the default directory name displayed on 
  the Select Destination Directory wizard page. A 
  fully qualified pathname must be specified. If the 
  [Setup] section directive DisableDirPage was set to 
  yes, this command line parameter is ignored. 

/GROUP="folder name" 
  Overrides the default folder name displayed on the 
  Select Start Menu Folder wizard page. If the [Setup]
  section directive DisableProgramGroupPage was set to
  yes, this command line parameter is ignored. 

/NOICONS 
  Instructs Setup to initially disable the Don't create
  any icons check box on the Select Start Menu Folder 
  wizard page. 

/COMPONENTS="comma separated list of component names" 

  Choose from - SuperServerComponent, 
                ClassicServerComponent, 
                ServerComponent, 
                DevAdminComponent and 
                ClientComponent

  Overrides the default components settings. Using 
  this command line parameter causes Setup to 
  automatically select a custom type. A full install 
  requires combining components. For example:
  
  /COMPONENTS="SuperServerComponent, ServerComponent, 
               DevAdminComponent, ClientComponent"
  
  would be required for a full install.


/FORCE

  Tells the installer to ignore its analysis of the 
  existing environment. It will attempt to install
  and configure Firebird 1.5 as if no previous
  version of Firebird or InterBase was installed.
  
  This can be useful if you have a seriously broken 
  installation that you cannot uninstall. Or it
  could be another way to aggravate your users by 
  breaking a perfectly good working install of 
  InterBase. Its your choice.


/NOCPL

  Don't install the Control Panel Applet. This is 
  useful for two reasons:
  
  o Installing/Uninstalling the CPL applet will 
    often require a system restart.

  o You may wish to use an alternative cpl applet.

  
/NOGDS32

  Don't install a copy of the client library into
  the system directory, even if installation 
  analysis concludes it is OK to do so.


/COPYFBCLIENT

  Copy the fbclient.dll to the system directory. This 
  is recommended for client installs if you are sure 
  that you will only ever be accessing a single 
  server version. If your client applications are 
  likely to take advantage of accessing different 
  server versions this is not recommended. See

    doc/README.Win32LibraryInstallation.txt

  for more information.


