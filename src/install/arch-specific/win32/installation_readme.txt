Firebird Database Server 1.5
============================

****** NOTE ******

This document is just a placeholder.

**** END NOTE ****


This document is a guide to installation of Firebird 1.5 on 
the Win32 platform.

Contents
--------

o Before installation					TODO
o Installing on a system with InterBase	TODO
o Installation assumptions				TODO
o Uninstallation						TODO
o Other Notes
o Installation from a batch file




Other Notes
-----------

  Firebird requires WinSock2. All Win32 platforms should have this, except
  for Win95. A test for the Winsock2 library is made during install. If
  it is not found the install will fail. You can visit this link:

    http://support.microsoft.com/default.aspx?scid=kb;EN-US;q177719

  to find out how to go about upgrading.


Installation from a batch file
------------------------------

The setup program can be run from a batch file. This is useful if you wish
to install the client across a network. The following parameters may be passed:


/SP- 
  Disables the This will install... Do you wish to continue? prompt at 
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

/COMPONENTS="component name" 

  Choose from - ServerComponent, DevToolsComponent and ClientComponent

  Overrides the default components settings. Using this command line parameter 
  causes Setup to automatically select a custom type. 
