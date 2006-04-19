
@echo off

@echo.

@call setenvvar.bat
@if errorlevel 1 (goto :END)

@echo Generating parse.cpp and dsql.tab.h

%ROOT_PATH%\gen\btyacc -l -d -S %ROOT_PATH%\src\dsql\btyacc_fb.ske %ROOT_PATH%\src\dsql\parse.y
@if errorlevel 1 (exit /B 1)
@copy y_tab.h %ROOT_PATH%\src\dsql\dsql.tab.h > nul
@copy y_tab.c %ROOT_PATH%\src\dsql\parse.cpp > nul

:END
