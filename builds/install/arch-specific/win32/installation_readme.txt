Firebird Database Server 1.5 Release Candidate 2
================================================

This document is a guide to installing this package of 
Firebird 1.5 on the Win32 platform. These notes refer to the 
installation package itself, rather than Firebird 1.5 in general.


Contents
--------

o Before installation
o Two Packages
o Uninstallation
o Other Notes
o Installation from a batch file


Before installation
-------------------

One of the  design goals of Firebird 1.5 is to allow different 
versions of Firebird (or InterBase) to run simultaneously. 

Much work has been done on this installation script to support this 
aim, but that work is NOT yet complete. 

There are still some known installation problems surrounding the issue 
of co-existence:

o Uninstalling Firebird 1.0 or 1.0.2 after installing this package
  will break this installation by removing some registry keys. The 
  next maintenance release of Firebird 1.0 will correct this.
  
o Neither the Firebird 1.0 nor 1.0.2 installers handle shared 
  library counts correctly. This will have unpredictable results 
  when uninstalling one of these versions. For instance, gds32.dll 
  may or may not exist if one of these earlier versions is uninstalled
  after installing this package.
  
o The server will not start if an existing Firebird or InterBase 
  server is listening on the default port 3050. A future version of
  this installation script will likely detect this and prompt for a
  different port during the install. Until then it is necessary to 
  edit the firebird.conf file manually.

To save yourself confusion while testing this release candidate it is 
recommended that you UNINSTALL all previous versions of Firebird 1.0, 
Firebird 1.5 or InterBase before installing this package.


Two Packages
------------

A subsidiary design goal is to allow developers to easily switch 
between the Super Server and Classic Architectures. Each architecture 
is encapsulated in a single binary. All that is required at a technical 
level is to choose which version of the server to execute. 

Combining the two server architectures in a single installation package 
is doable, at the expense of bringing complexity to the installation 
process.

  o A custom dialogue screen is required to ask the user to choose the
    desired architecture. This is supported by the installation builder 
    but is not an 'out of the box' option. 
  
  o Uninstallation needs to check whether the running architecture has 
    changed from the one originally installed.
  
These issues are solvable but that takes time. While waiting for the 
SuperInstaller to come along the existing installation script has been 
enhanced to ensure that repeated installation and uninstallation of 
each package works flawlessly. Switching between packages should 
require no more effort than uninstalling one and installing the other.


Uninstallation
--------------

This package should be uninstalled via the Control Panel. Alternatively
it can be uninstalled by running unins000.exe from the installation 
directory.


Other Notes
-----------

  Firebird requires WinSock2. All Win32 platforms should have this, 
  except for Win95. A test for the Winsock2 library is made during 
  install. If it is not found the install will fail. You can visit 
  this link:

    http://support.microsoft.com/default.aspx?scid=kb;EN-US;q177719

  to find out how to go about upgrading.


Installation from a batch file
------------------------------

The setup program can be run from a batch file. This is useful if you wish
to install the client across a network. The following parameters may be passed:


/SP- 
  Disables the 'This will install... Do you wish to continue?' prompt at 
  the beginning of Setup. 

/SILENT, /VERYSILENT 
  Instructs Setup to be silent or very silent. When Setup is silent the 
  wizard and the background window are not displayed but the installation 
  progress window is. When a setup is very silent this installation 
  progress window is not displayed. Everything else is normal so for 
  example error messages during installation are displayed and the startup 
  prompt is (if you haven't disabled it with  the '/SP-' command line option 
  explained above) 

  If a restart is necessary and the '/NORESTART' command isn't used 
  (see below) and Setup is silent, it will display a Reboot now? 
  messagebox. If it's very silent it will reboot without asking. 

/NORESTART 
  Instructs Setup not to reboot even if it's necessary. 

/DIR="x:\dirname"  
  Overrides the default directory name displayed on the Select Destination 
  Directory wizard page. A fully qualified pathname must be specified. If 
  the [Setup] section directive DisableDirPage was set to yes, this command 
  line parameter is ignored. 

/GROUP="folder name" 
  Overrides the default folder name displayed on the Select Start Menu Folder 
  wizard page. If the [Setup] section directive DisableProgramGroupPage was 
  set to yes, this command line parameter is ignored. 

/NOICONS 
  Instructs Setup to initially disable the Don't create any icons check box 
  on the Select Start Menu Folder wizard page. 

/COMPONENTS="comma separated list of component names" 

  Choose from - ServerComponent, DevAdminComponent and ClientComponent

  Overrides the default components settings. Using this command line parameter 
  causes Setup to automatically select a custom type. A full install requires 
  combining components.
