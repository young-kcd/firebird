cl -Ob2iytp -Gs -DWIN95 -D__STDC__ -W3 -G4 -Gd -MD -I..\..\src\include -DWIN32_LEAN_AND_MEAN -DSERVER_SHUTDOWN -DSUPERSERVER -DGOVERNOR -DNOMSG -D_X86_=1 -DWIN32 -DI386 -DEXACT_NUMERICS ..\..\src\jrd\blrtable.cpp
if errorlevel 1 exit
blrtable.exe >blrtable.h
copy blrtable.h ..\..\src\include\gen\blrtable.h
del blrtable.exe
del blrtable.obj
del blrtable.h
