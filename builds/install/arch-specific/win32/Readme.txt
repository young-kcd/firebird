
==================================
Firebird 2.0.0       (Win32 Build)
==================================

o Introduction
o Intended Users
o Features in this release (all platforms)
o Bugs fixed in this release
o Installation
o Known Issues
o Reporting Bugs
o Requesting New Features


Introduction
============

Welcome to Firebird 2.0 Release Candidate 1.


Intended Users
==============

This release candidate is not intended for production 
use although it is expected to be nearly production
ready. However, it is released for evaluation purposes 
and is expected to be tested by experienced users of 
Firebird. It is hoped that all field testers will read 
the release notes first.

 o DO NOT install this version if you have never used
   Firebird before. Use Firebird v1.5.3 instead.

 o If you have been testing earlier Alpha or Beta 
   releases please make sure you read the installation 
   readme and the release notes.


Features in this release (all platforms)
========================================

See doc/Firebird_v2.0.0.ReleaseNotes.pdf for more
information.


Bugs fixed in this release (all platforms)
==========================================

See doc/Firebird_v2.0.0.ReleaseNotes.pdf for more
information.


Installing the self-installing executable
=========================================

Please run the executable and read the accompanying
installation instructions that are contained within the
setup wizard.


Known Issues
============

 o The installation process fails to detect a running
   Firebird 2.0 server. Ultimately the installation
   will fail, when it attempts to overwrite a file
   that is active.

 o The security database has changed! See the DOC
   directory for more details. Users who wish to
   migrate an old security database to the new format
   can use the upgrade script in misc/upgrade/security


Reporting Bugs
==============

o Are you sure you understand how Firebird works?

  Perhaps you are seeing the correct behaviour and you
  really have a support question. In this case contact
  the firebird-support list server.

  You may subscribe here:

    mailto:firebird-support-subscribe@yahoogroups.com


o Still think it is a bug?

  First check the field test issue tracker here:

    http://sourceforge.net/tracker/?atid=593943&group_id=9028&func=browse

  It is specifically intended for reporting of problems
  found while testing pre-release versions of Firebird.

  You might also want to check the list of Open Bugs in
  stable Firebird releases. This can be found at:

    http://sourceforge.net/tracker/?atid=109028&group_id=9028&func=browse

Please do not use the bug tracker to get technical
support. If in doubt subscribe to the support list 
first at

  mailto:firebird-support-subscribe@yahoogroups.com
  
and ask your question there.

	
Requesting New Features
=======================

Before submitting feature requests please review the
existing feature request list. Chances are that someone
has already thought of it. Existing feature requests
can be browed here:

  http://sourceforge.net/tracker/?atid=359028&group_id=9028&func=browse


