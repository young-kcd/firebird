
@echo off

::===========
:MAIN
@if "%1"=="" (@goto :ERROR_PARAM_EXPECTED)
@md dbs
@cd dbs
@gbak -r %1\src\misc\metadata.gbak localhost:%1\builds\win32\dbs\metadata.gdb
@md jrd
@gbak -r %1\src\misc\security.gbak localhost:%1\builds\win32\dbs\jrd\isc.gdb
@md msgs
@gbak -r %1\src\msgs\msg.gbak localhost:%1\builds\win32\dbs\msgs\msg.gdb
@md qli
@gbak -r %1\src\misc\help.gbak localhost:%1\builds\win32\dbs\qli\help.gdb
@cd ..
@del expand.sed
@call :SED yachts.lnk %1 metadata.gdb
@call :SED ODS.RDB %1 jrd/isc.gdb
@call :SED msg.gdb %1 msgs/msg.gdb
@call :SED master_msg_db %1 msgs/msg.gdb
@call :SED yachts.link %1 jrd/isc.gdb
@call :SED master_help_db %1 qli/help.gdb
@call :SED help.gdb %1 qli/help.gdb
@call :SED yachts.gdb %1 jrd/isc.gdb
@call :SED isc.gdb %1 jrd/isc.gdb
@call :SED isc4.gdb %1 jrd/isc.gdb
@call :SED empbuild.gdb %1 example5/employee.gdb
@call :SED source/msgs/msg.gdb %1 msgs/msg.gdb
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
