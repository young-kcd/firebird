
call setenvvar.bat

@echo.
@echo cleaning
:: CLEANING
del %ROOT_PATH%\examples\stat\*.obj 2>nul
del %ROOT_PATH%\examples\stat\*.exe 2>nul
del %ROOT_PATH%\examples\stat\*.c 2>nul

