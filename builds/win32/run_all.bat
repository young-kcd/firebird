call clean_all
call prepare c:\opt\firebird_15
call make_boot
call make_all
call make_examples
cd ..\install\arch-specific\win32
call BuildExecutableInstall
cd ..\..\..\win32