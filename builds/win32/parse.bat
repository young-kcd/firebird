
@echo off

@echo.
@set PARSER=
@set SED=
@call :CHECKTOOLS
@if "%PARSER%" == "" ((goto :HELP_TOOLS) & (goto :EOF))
@if "%SED%" == "" ((goto :HELP_TOOLS) & (goto :EOF))

@if "%PARSER%" == "YACC" (call :YACC_PROC) else (call :BISON_PROC)
@call :SED_PROC
@goto :END

::=====================================
:CHECKTOOLS
:: verify our unix tool set is available

@bison --help > nul 2>nul
@if not errorlevel 9009 ((set PARSER=BISON) & (goto :sed_pro))

@byacc --help > nul 2> nul
@if not errorlevel 9009 ((set PARSER=YACC) & (goto :sed_pro))

:sed_pro
@sed --help > nul 2> nul
@if not errorlevel 9009 (set SED=TRUE)

@goto :EOF

::=====================================
:BISON_PROC
@set PARSER=BISON
@echo .
@echo Processing with bison
@bison -y -l -d -b dsql %ROOT_PATH%\src\dsql\parse.y
@goto :EOF

::=====================================
:YACC_PROC
@set PARSER=YACC
@echo .
@echo Processing with yacc
@byacc -l -d -b dsql %ROOT_PATH%\src\dsql\parse.y
@goto :EOF

::=====================================
:SED_PROC
@echo .
@echo Processing with sed
@sed -f %ROOT_PATH%\src\dsql\parse.sed <dsql_tab.c >%ROOT_PATH%\src\dsql\parse.cpp
@copy dsql.tab.h %ROOT_PATH%\src\dsql\dsql_tab.h > nul
@del dsql_tab.c
@del dsql_tab.h
@goto :EOF

::=================
:HELP_TOOLS
@echo:
@echo   Please check that these utilities programs are
@echo   all on your path:
@echo:
@echo     sed.exe
@echo     bison.exe or yacc.exe (only one needed)
@echo:
@echo   If you do not have these utilities they may be downloaded
@echo   from the following sites
@echo:
@echo     sed.exe and bison 1.35 from http://gnuwin32.sourceforge.net/
@echo:
@echo     byacc 1.9 from ftp://garbo.uwasa.fi/pc/unix/
@echo     
@echo:
@goto :END


:END