
@echo off

@echo.

@call setenvvar.bat
@if errorlevel 1 (goto :END)

@echo Generating parse.cpp and dsql.tab.h

%FB_ROOT_PATH%\temp\%FB_OBJ_DIR%\btyacc\btyacc -l -d -S %FB_ROOT_PATH%\src\dsql\btyacc_fb.ske %FB_ROOT_PATH%\src\dsql\parse.y
@if errorlevel 1 (exit /B 1)
@copy y_tab.h %FB_ROOT_PATH%\src\include\gen\parse.h > nul
@copy y_tab.c %FB_ROOT_PATH%\src\dsql\parse.cpp > nul
@del y_tab.h
@del y_tab.c

:END
