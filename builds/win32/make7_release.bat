@echo off

if "%1"=="CLEAN" (call :CLEAN) else (
        call :NORMAL
        (if "%1"=="MOVE" (call :MOVE))
)

goto :END

::===========
:CLEAN
cd msvc7
devenv Firebird2.sln /clean release 
cd ..
goto :EOF

::===========
:NORMAL
cd msvc7
devenv Firebird2.sln /build release 
del release\firebird\bin\*.exp
del release\firebird\bin\*.lib
cd ..
goto :EOF

::===========
:MOVE
rmdir /q /s output
mkdir output
mkdir output\bin
mkdir output\intl
mkdir output\udf
mkdir output\help
mkdir output\doc
mkdir output\include
mkdir output\lib
copy msvc7\release\firebird\bin\* output\bin
copy msvc7\release\firebird\intl\* output\intl
copy msvc7\release\firebird\udf\* output\udf
copy gpre.exe output\bin
copy dbs\jrd\SECURITY.FDB output\security.fdb
copy dbs\qli\HELP.FDB output\help\help.fdb
copy ..\..\ChangeLog output\doc\ChangeLog.txt
copy ..\..\doc\WhatsNew output\doc\WhatsNew.txt
copy ..\..\src\jrd\ibase.h output\include
copy ..\..\src\include\gen\iberror.h output\include
copy install_super.bat output\bin
copy install_classic.bat output\bin
copy uninstall.bat output\bin

goto :EOF

:END