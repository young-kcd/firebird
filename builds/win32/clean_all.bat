@echo off

@echo Cleaning directories
@rmdir /S /Q dbs 2>nul
@rmdir /S /Q temp 2>nul
@rmdir /S /Q output 2>nul
@rmdir /S /Q msvc6\debug 2>nul
@rmdir /S /Q msvc6\release 2>nul
@rmdir /S /Q msvc7\debug 2>nul
@rmdir /S /Q msvc7\release 2>nul
@rmdir /S /Q ..\..\gen 2>nul

@echo Cleaning files
@del *.exe 2>nul
@del *.msg 2>nul
@del *.dll 2>nul
@del ..\..\src\include\gen\autoconfig.h 2>nul
@del ..\..\src\include\gen\blrtable.h 2>nul
@del msvc6\*.log 2>nul
@del msvc6\*.plg 2>nul
@del msvc7\*.log 2>nul
@del msvc7\*.plg 2>nul
