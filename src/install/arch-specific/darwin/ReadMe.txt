The Firebird install process will create a new user: firebird. This is for added security. Please don't delete this user unless you know what you are doing. The installer installs the Firebird framework in /Library/Frameworks. The default installs one super-user database user: "sysdba", password "masterkey". You should change that password using gsec according to the documentation.

All the standard command line executables are installed in /Library/Frameworks/Firebird.framework/Resources/bin. Currently there is no GUI console application for Firebird. There is one in the works somewhere. In addition there is an Objective-C interface in the works. If you are interested in helping with these efforts, or with others, please contact us (contact inormation available on the firebird site).

Please note that every MacOS X user you want to have access to your database MUST have read/write permissions on the .gdb file.

The complete release notes can be found on the firebird web site (www.firebirdsql.org).  The documentation for Firebird is the same as the Interbase documentation, available at www.interbase.com. There is a yahoo group named "ib-support" if you have any problems with firebird.  The IBPhoenix web site (www.ibphoenix.com) is also a great reference.
