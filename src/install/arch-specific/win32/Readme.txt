
==================================
Firebird 1.5.0       (Win32 Build)
==================================


o Introduction
o Stability
o Installation
o Known Issues
o Language documentation
o Reporting Bugs
o Requesting New Features


Introduction
============

Welcome to Firebird 1.5.0. This represents a feature complete
build of Firebird 1.5. It is the product of many months work by the 
Firebird team. See the Release Notes document for details of all the 
new features and the bug-fixes it contains. 


Stability
=========

This release is the first using the C++ codebase, the latest 
evolution of more than three years' refactoring of the original 
InterBase 6.n C code.  It has been through a long beta and release 
candidate cycle and is considered to be as stable as its predecessor, 
Firebird 1.0.3.  There is no reason to be concerned about using it in 
a production environment. However, before ANY new software into a 
production environment it should be always be tested properly on a 
development system. This is standard practice.


Installing the self-installing executable
=========================================

Please run the executable and read the accompanying installation 
instructions that are contained within the setup wizard. More 
detailed information is in the release notes. 

Many changes have been implemented in this release.  If you have an 
existing installation of Firebird (including a v.1.5 beta or release 
candidate) you must read the latest release notes and understand the 
new names and locations of all binaries and associated files.  This 
applies to both server and client installations. 

Do not install this version "over the top" of an existing Firebird or
InterBase installation.  


Known Issues
============

Client library
--------------
Some legacy Windows tools will not recognise the client library 
fbclient.dll and/or will not find it in its default location, now the
..\bin directory beneath the Firebird root.  
For example, the Borland IBConsole program will not work with 
fbclient.dll, even if it is copied to the system directory and renamed 
to gds32.dll. 
The Windows kits contain a program named instclient.exe that can be
used to build a "legacy-compatible" gds32.dll and optionally install 
it to your system directory.  It is essential to read the release 
notes and the accompanying README_library_install.txt (in the ..\doc
subdirectory) before performing this change.    

UDFs and Blob Filters
---------------------
Security changes in this version may cause your UDFs to be invisible to 
the server until their location is configured in firebird.conf.  For 
more information, read the annotations in firebird.conf and the 
documentation in the release notes.


SQL Language Documentation
========================== 
New language features, keywords, extensions and restrictions introduced 
in this release are documented in detail in the release notes. 
The Firebird 1.5 release notes version for this release is 1.08. 


Reporting Bugs
==============

o Are you sure you understand how Firebird works?

  Perhaps you are seeing the correct behaviour and you really have a 
  support question. In this case contact the firebird-support list server.
 
  You may subscribe here: 

    mailto:firebird-support-subscribe@yahoogroups.com

o Still think it is a bug? 

  Check the list of Open Bugs. This can be found in the Firebird bug tracker:

    http://sourceforge.net/tracker/?group_id=9028&atid=109028

Otherwise, if you are reasonably sure it is a bug then please 
try to develop a reproducible test case. You can then submit it
to the bug tracker.


Requesting New Features
=======================

Before submitting feature requests please review the existing 
feature request list. Chances are that someone has already thought 
of it. Use this link to review a current list and submit new request,
if necessary:

  http://sourceforge.net/tracker/?group_id=9028&atid=359028
