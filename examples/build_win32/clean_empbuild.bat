
call setenvvar.bat

@echo.
@echo cleaning empbuild.fdb
:: CLEANING
del %ROOT_PATH%\examples\empbuild\*.fdb 2>nul
del %ROOT_PATH%\examples\empbuild\*.obj 2>nul
del %ROOT_PATH%\examples\empbuild\*.c 2>nul
del %ROOT_PATH%\examples\empbuild\*.exe 2>nul
