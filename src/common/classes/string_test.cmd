@echo off

: preprocessor defines
: DEV_BUILD makes single iteration, validating all string results.
:   When not defined, makes 100000 iterations (a few seconds runtime),
:   and reports time of that test.
: FIRESTR - use Firebird::string, if not defined - STL basic_string is used.
:   In the latter case some checks are not performed, because not supported by STL
:   or give AV with it (at least MS implementation).
: Without DEV_BUILD this checks are also not performed to give compareable results
:   for both string classes.

del /Q string_test.exe

set cpp_files=string_test.cpp fb_string.cpp alloc.cpp locks.cpp ..\fb_exception.cpp
set inc_dirs=-I ..\..\include
set lib_files=user32.lib

set filesNdirs=%cpp_files% %inc_dirs% %lib_files%

: This line tests our test using std::basic_string
: cl -GR -GX -DDEV_BUILD %filesNdirs%

: This line tests correctness of Firebird::string
cl -GR -GX -DFIRESTR -DDEV_BUILD %filesNdirs%

: This line tests speed of Firebird::string
: cl -GR -GX -Ox -DFIRESTR %filesNdirs%

: This line tests speed of std::basic_string for comparison
: cl -GR -GX -Ox %filesNdirs%

if errorlevel 1 exit

string_test.exe
