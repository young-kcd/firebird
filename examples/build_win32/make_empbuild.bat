
call setenvvar.bat

:: CLEANING
call clean_empbuild.bat

cd %ROOT_PATH%\examples\empbuild\
@echo.
@echo building databases
del isql.sed 2>nul
:: @echo s;empbuild.fdb;localhost:%DB_PATH%/examples/empbuild/empbuild.fdb;g > isql.tmp
:: @echo s;employe2.fdb;localhost:%DB_PATH%/examples/empbuild/employe2.fdb;g >> isql.tmp
:: @echo s;intlbuild.fdb;localhost:%DB_PATH%/examples/empbuild/intlbuild.fdb;g >> isql.tmp
::
:: sed "s/\//\\//g" < isql.tmp > isql.tmp2
:: sed "s/;/\//g" < isql.tmp2 > isql.sed

@echo s;empbuild.fdb;localhost:%ROOT_PATH%\examples\empbuild\empbuild.fdb;g > isql.tmp
@echo s;employe2.fdb;localhost:%ROOT_PATH%\examples\empbuild\employe2.fdb;g >> isql.tmp
@echo s;intlbuild.fdb;localhost:%ROOT_PATH%\examples\empbuild\intlbuild.fdb;g >> isql.tmp
@echo s;intlemp.fdb;localhost:%ROOT_PATH%\examples\empbuild\intlemp.fdb;g >> isql.tmp

sed "s/\\/\\\\/g" < isql.tmp > isql.tmp2
sed "s/;/\//g" < isql.tmp2 > isql.sed

sed -f isql.sed < empbld.sql > empbld.tmp
sed -f isql.sed < employe2.sql > employe2.tmp
sed -f isql.sed < intlbld.sql > intlbld.tmp
sed -f isql.sed < intldml.sql > intldml.tmp

echo save intldml.sql
move intldml.sql intldml.ori
move intldml.tmp intldml.sql

echo Executing sql
isql -i empbld.tmp
isql -i employe2.tmp
isql -i intlbld.tmp
cd %ROOT_PATH%\examples\build_win32

@echo.
@echo preprocessing empbuild.e and intlbuild.e
%ROOT_PATH%\bin\gpre -r -m -n -z %ROOT_PATH%\examples\empbuild\empbuild.e %ROOT_PATH%\examples\empbuild\empbuild.c -b localhost:%ROOT_PATH%\examples\empbuild\
%ROOT_PATH%\bin\gpre -r -m -n -z %ROOT_PATH%\examples\empbuild\intlbld.e %ROOT_PATH%\examples\empbuild\intlbld.c -b localhost:%ROOT_PATH%\examples\empbuild\

cd %ROOT_PATH%\examples\empbuild\
cl -I %ROOT_PATH%\include %ROOT_PATH%\lib\gds32_ms.lib %ROOT_PATH%\examples\empbuild\empbuild.c 
cl -I %ROOT_PATH%\include %ROOT_PATH%\lib\gds32_ms.lib %ROOT_PATH%\examples\empbuild\intlbld.c 
cd %ROOT_PATH%\examples\build_win32

@echo.
@echo Building employee.fdb
:: and empbuild.exe uses isql
@del %ROOT_PATH%\examples\empbuild\employee.fdb
@del %ROOT_PATH%\examples\empbuild\intlemp.fdb

:: Este programa utiliza isql y por lo tanto hay que ponerse en el lugar adecuado
:: para que encuentre los scripts
cd %ROOT_PATH%\examples\empbuild\
@empbuild.exe localhost:%ROOT_PATH%\examples\empbuild\employee.fdb
@intlbld.exe localhost:%ROOT_PATH%\examples\empbuild\intlemp.fdb

echo Restore 
move intldml.sql intldml.tmp
move intldml.ori intldml.sql

cd %ROOT_PATH%\examples\build_win32

