@echo off

@cd msvc6
@if "%2"=="CLEAN" (call :CLEAN) else (call :NORMAL)
@call :MOVE
@goto :END

::===========
:CLEAN
@msdev Firebird2.dsw /MAKE "fbserver - Win32 Debug" "fbguard - Win32 Debug" "fb_lock_print - Win32 Debug" "fb_inet_server - Win32 Debug" "gbak - Win32 Debug" "gpre - Win32 Debug" "gsplit - Win32 Debug" "gdef - Win32 Debug" "gfix - Win32 Debug" "gsec - Win32 Debug" "gstat - Win32 Debug" "instreg - Win32 Debug" "instsvc - Win32 Debug" "isql - Win32 Debug" "qli - Win32 Debug" "gds32 - Win32 Debug" "fbclient - Win32 Debug" "ib_udf - Win32 Debug" "ib_util - Win32 Debug" "intl - Win32 Debug" "intlcpp - Win32 Debug" /REBUILD /OUT Firebird2.log
@goto :EOF

::===========
:NORMAL
@msdev Firebird2.dsw /MAKE "fbserver - Win32 Debug" "fbguard - Win32 Debug" "fb_lock_print - Win32 Debug" "fb_inet_server - Win32 Debug" "gbak - Win32 Debug" "gpre - Win32 Debug" "gsplit - Win32 Debug" "gdef - Win32 Debug" "gfix - Win32 Debug" "gsec - Win32 Debug" "gstat - Win32 Debug" "instreg - Win32 Debug" "instsvc - Win32 Debug" "isql - Win32 Debug" "qli - Win32 Debug" "gds32 - Win32 Debug" "fbclient - Win32 Debug" "ib_udf - Win32 Debug" "ib_util - Win32 Debug" "intl - Win32 Debug" "intlcpp - Win32 Debug" /OUT Firebird2.log
@goto :EOF

::===========
:MOVE
@del debug\firebird\bin\*.exp
@del debug\firebird\bin\*.lib
@cd ..
@rmdir /q /s output
@mkdir output
@mkdir output\bin
@mkdir output\intl
@mkdir output\udf
@mkdir output\help
@mkdir output\doc
@mkdir output\include
@mkdir output\lib
@copy msvc6\debug\firebird\bin\* output\bin
@copy msvc6\debug\firebird\intl\* output\intl
@copy msvc6\debug\firebird\udf\* output\udf
@copy gpre.exe output\bin
@copy dbs\jrd\SECURITY.FDB output\security.fdb
@copy dbs\qli\HELP.fdb output\help\help.fdb
@copy ..\..\ChangeLog output\doc\ChangeLog.txt
@copy ..\..\doc\WhatsNew output\doc\WhatsNew.txt
@copy ..\..\src\jrd\ibase.h output\include
@copy ..\..\src\include\gen\iberror.h output\include
@copy install_super.bat output\bin
@copy install_classic.bat output\bin
@copy uninstall.bat output\bin

@goto :EOF

:END