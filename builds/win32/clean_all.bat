@echo off

@call setenvvar.bat
@if errorlevel 1 (goto :END)

@echo Cleaning temporary directories
@rmdir /S /Q %ROOT_PATH%\output 2>nul
@rmdir /S /Q %ROOT_PATH%\gen 2>nul
@rmdir /S /Q %ROOT_PATH%\temp 2>nul

@echo Cleaning build files
@del *.log 2>nul
@del %ROOT_PATH%\src\include\gen\autoconfig.h 2>nul
@del %ROOT_PATH%\src\include\gen\blrtable.h 2>nul
@del %ROOT_PATH%\builds\win32\msvc6\*.plg 2>nul
@del %ROOT_PATH%\builds\win32\msvc7\*.plg 2>nul

@Echo Cleaning install files
@del %ROOT_PATH%\src\install\arch-specific\win32\*.dll 2>nul