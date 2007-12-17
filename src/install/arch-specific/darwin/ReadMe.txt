The Firebird install process will create a new user: firebird. This is for added security. Please don't delete this user unless you know what you are doing. The installer installs the Firebird framework in /Library/Frameworks. The default installs one super-user database user: "sysdba", password "masterkey". You should change that password using gsec according to the documentation.

All the standard command line executables are installed in /Library/Frameworks/Firebird.framework/Resources/bin. If you are interested in helping with these efforts, or with others, please contact us via the Firebird website at www.firebirdsql.org. 

Please note that every MacOS X user you want to have access to your database MUST have read/write permissions on the .gdb file.

The complete release notes can be found in the doc directory.  Generic documentation for Firebird can be found on the IBPhoenix web site at www.ibphoenix.com, as well as at the Firebird website. There is also a yahoo group named "ib-support" if you have any problems with firebird. 
