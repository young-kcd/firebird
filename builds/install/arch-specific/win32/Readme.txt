
====================================
Firebird $MAJOR.$MINOR.$RELEASE    (Windows Build)
====================================


o Introduction
o Intended Users
o Known Problems
o Reporting Bugs


Introduction
============

Welcome to Firebird $MAJOR.$MINOR.


Intended Users
==============

This is an alpha version of Firebird.

It is not intended for production use. It is for
test purposes only.


Known Problems
==============

To run firebird.exe as a multi-client superserver
it is necessary to start it with the -m parameter.
Unfortunately for Alpha 2 neither the guardian nor 
the cpl applet know about this. 

If the guardian is used to execute firebird.exe as 
an application it will be single user only so it is
not recommended to use the guardian when running as
an application in this alpha release.

The cpl applet cannot configure firebird as a service
correctly and the installer will not install it for
this alpha release.

Neither of these problems are relevant to running 
firebird as a service via instsvc.exe which works 
correctly - with and without the guardian.


Reporting Bugs
==============

This is an experimental version.

DO NOT REPORT A BUG IN THIS RELEASE
unless you really know what you are doing.

Check first on the firebird-devel list.
If you don't know where the firebird-devel
list is then you shouldn't be using this
version of Firebird!

Please don't use the Firebird-devel list for technical
support unless the question specifically relates to a
new feature in Firebird $MAJOR.$MINOR


Happy Testing!

From the Firebird team.

