@echo off

@echo Cleaning build
@rmdir /S /Q dbs 2>nul
@rmdir /S /Q temp 2>nul
@rmdir /S /Q output 2>nul
@rmdir /S /Q msvc6\debug 2>nul
@rmdir /S /Q msvc6\release 2>nul
@rmdir /S /Q msvc7\debug 2>nul
@rmdir /S /Q msvc7\release 2>nul
@rmdir /S /Q ..\..\gen 2>nul
@del *.exe 2>nul
@del *.msg 2>nul
@del ..\..\src\include\gen\autoconfig.h 2>nul
@del ..\..\src\include\gen\blrtable.h 2>nul
@del msvc6\*.log