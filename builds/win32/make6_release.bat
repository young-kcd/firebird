@echo off

@cd msvc6
@if "%1"=="CLEAN" (call :CLEAN) else (call :NORMAL)
@call :MOVE
@goto :END

::===========
:CLEAN
@msdev Firebird2.dsw /MAKE "fbserver - Win32 Release" "fbguard - Win32 Release" "fb_lock_print - Win32 Release" "fb_inet_server - Win32 Release" "gbak - Win32 Release" "gpre - Win32 Release" "gsplit - Win32 Release" "gdef - Win32 Release" "gfix - Win32 Release" "gsec - Win32 Release" "gstat - Win32 Release" "instreg - Win32 Release" "instsvc - Win32 Release" "isql - Win32 Release" "qli - Win32 Release" "gds32 - Win32 Release" "fbclient - Win32 Release" "ib_udf - Win32 Release" "ib_util - Win32 Release" "intl - Win32 Release" "intlcpp - Win32 Release" /REBUILD /OUT Firebird2.log
@goto :EOF

::===========
:NORMAL
@msdev Firebird2.dsw /MAKE "fbserver - Win32 Release" "fbguard - Win32 Release" "fb_lock_print - Win32 Release" "fb_inet_server - Win32 Release" "gbak - Win32 Release" "gpre - Win32 Release" "gsplit - Win32 Release" "gdef - Win32 Release" "gfix - Win32 Release" "gsec - Win32 Release" "gstat - Win32 Release" "instreg - Win32 Release" "instsvc - Win32 Release" "isql - Win32 Release" "qli - Win32 Release" "gds32 - Win32 Release" "fbclient - Win32 Release" "ib_udf - Win32 Release" "ib_util - Win32 Release" "intl - Win32 Release" "intlcpp - Win32 Release" /OUT Firebird2.log
@goto :EOF

::===========
:MOVE
@del release\firebird\bin\*.exp
@del release\firebird\bin\*.lib
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
@copy msvc6\release\firebird\bin\* output\bin
@copy msvc6\release\firebird\intl\* output\intl
@copy msvc6\release\firebird\udf\* output\udf
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