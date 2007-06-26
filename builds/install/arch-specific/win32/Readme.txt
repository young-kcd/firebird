
====================================
Firebird 2.1.0       (Windows Build)
====================================

  *** Beta 1 ***

o Introduction
o Intended Users
o Features in this release (all platforms)
o Bugs fixed in this release
o Installation
o Reporting Bugs


Introduction
============

Welcome to the first beta release of Firebird 2.1


Intended Users
==============

This beta release is intended for experienced Firebird
users. It has not been thoroughly tested and new
features are only sparsely documented. Do not use this
release in a production environment.


Features in this release
========================

Apart from numerous bug fixes there are several
major new features in Firebird 2.1. In no particular
order they are:

 o Text blob compatibility with varchars
 o Database Triggers (connect, disconnect)
 o Transaction Triggers (start/commit/rollback)
 o Use of domains for Trigger/SP variable definition
 o Numerous monitoring facilities available via
     SQL statements
 o Global temporary tables
 o Query termination
 o Unicode collations
 o MERGE and UPDATE or INSERT statements
 o Support for chinese characters via GBK
 o Common table expressions
 o Aggregate concatenation function
 o Character set TIS620
 o 64-bit builds for Windows

There are also many more enhancements. You can browse
a summary of changes via the bug tracker here:

http://tracker.firebirdsql.org/secure/ReleaseNote.jspa?projectId=10000&styleName=Html&version=10041

Improvements and New features are listed after the
bug fixes.

We are in the process of documenting the new features
and you can read more about them in the release notes.


Installation
============

Binary installation package are available, but they do
not yet support multiple service instance installation.
Neither are they fully 64-bit ready. The known
limitations are discussed in installation_readme.txt.

You can also use the zip packages to install Firebird
manually. There is more info on manual installation
in doc\install_windows_manually.txt.


Reporting Bugs
==============

Before you report a bug:

 o Check you know how Firebird works.
   Maybe it is not a bug at all.

 o Perhaps some has already reported this? Browse
   existing bug reports here:

    http://tracker.firebirdsql.org/secure/BrowseProject.jspa

 o If in doubt why not discuss the problem on the
   Firebird-devel list? You can subscribe here:

    http://lists.sourceforge.net/lists/listinfo/firebird-devel

   and the list is viewable via a newsgroup interface
   here:

    news://news.atkin.com/


Please don't use the Firebird-devel list for technical
support unless the question specifically relates to a
new feature in Firebird 2.1


Happy Testing!

From the Firebird team.
