
@echo off

::===========
:MAIN
@if "%1"=="" (@goto :ERROR_PARAM_EXPECTED)
@md dbs
@cd dbs
@gbak -r %1\src\misc\metadata.gbak localhost:%1\builds\win32\dbs\metadata.fdb
@md jrd
@gbak -r %1\src\misc\security.gbak localhost:%1\builds\win32\dbs\jrd\security.fdb
@md msgs
@gbak -r %1\src\msgs\msg.gbak localhost:%1\builds\win32\dbs\msgs\msg.fdb
@md qli
@gbak -r %1\src\misc\help.gbak localhost:%1\builds\win32\dbs\qli\help.fdb
@cd ..
@del expand.sed
@call :SED yachts.lnk %1 metadata.fdb
@call :SED ODS.RDB %1 jrd/security.fdb
@call :SED msg.gdb %1 msgs/msg.fdb
@call :SED master_msg_db %1 msgs/msg.fdb
@call :SED yachts.link %1 jrd/security.fdb
@call :SED master_help_db %1 qli/help.fdb
@call :SED help.fdb %1 qli/help.fdb
@call :SED yachts.gdb %1 jrd/security.fdb
@call :SED isc.gdb %1 jrd/security.fdb
@call :SED isc4.gdb %1 jrd/security.fdb
@call :SED security.fdb %1 jrd/security.fdb
@call :SED empbuild.gdb %1 example5/employee.fdb
@call :SED source/msgs/msg.gdb %1 msgs/msg.fdb
@goto :END

::===========
:SED
@echo s?\"%1\"?\"localhost:%2/builds/win32/dbs/%3\"?g >> expand.sed
@goto :EOF

::===========
:ERROR_PARAM_EXPECTED
@echo You must specify the root directory of the Firebird source tree.
@echo Only forward (POSIX-style) slashes should be used in the path.
@echo Example:
@echo     prepare.bat d:/work/firebird
@goto :END

:END
