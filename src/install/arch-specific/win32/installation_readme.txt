Firebird Database Server 1.5 Final Release (Draft)
==================================================

This document is a guide to installing this package of 
Firebird 1.5 on the Win32 platform. These notes refer 
to the installation package itself, rather than 
Firebird 1.5 in general.

** IMPORTANT NOTE **

If you used one of the installable binaries from 
Sourceforge to install RC2 it is recommended that you 
uninstall directly from the installation directory 
with unins000.exe.

Do not use the Control Panel as the path to the 
uninstaller. There appears to be a bug that prevents 
the server service from being shut down if the 
uninstaller is run from there. 

This only applies to the uninstaller with RC2 from 
Firebird on Sourceforge. The RC4 installable binaries 
can be uninstalled via any available means.

**       END      **

Contents
--------

o Before installation
o New features of the installer
o Uninstallation
o Other Notes
o Installation from a batch file


Before installation
-------------------

It is recommended that you UNINSTALL all previous 
versions of Firebird 1.0, Firebird 1.5 or InterBase 
before installing this package.


New features of the installer in Firebird 1.5
---------------------------------------------

o This installer now combines the super server and 
  classic server binaries into a single installation 
  package. You can choose to install one or the other,
  but not both. To switch server type you need to 
  uninstall and re-install.
  
o Libraries are no longer installed into the Windows 
  system directory. This applies to both Firebird's 
  own libraries and the Microsoft C run-time 
  libraries. They are installed into the Firebird bin 
  directory. This should resolve all installation 
  conflicts, expecially on Windows 2000 and Windows 
  XP.

o If firebird.conf exists in the installation 
  directory it is saved as:
    firebird.conf.saved.n
  where n is a number. The installer always installs 
  the default firebird.conf file. This is to guarantee
  consistency to the installation process. Otherwise 
  the installer would have to parse the existing (and 
  possibly broken) configuration file.


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

