rmdir ..\..\..\..\output 2>nul
move ..\..\..\..\gen\firebird ..\..\..\..\output
xcopy ..\..\..\..\doc ..\..\..\..\output\doc
copy ..\..\..\..\output\misc\firebird.conf ..\..\..\..\output\firebird.conf
gbak ..\..\..\..\output\security.fdb ..\..\..\..\output\security.fbk
ren ..\..\..\..\output\bin\ibguard.exe fbguard.exe
move ..\..\..\..\output\lib\fbclient.dll ..\..\..\..\output\bin\fbclient.dll
move ..\..\..\..\output\lib\libib_util.dll ..\..\..\..\output\bin\ib_util.dll