
call setenvvar.bat

@echo.
@echo cleaning
:: CLEANING
del %ROOT_PATH%\examples\dyn\*.obj 2>nul
del %ROOT_PATH%\examples\dyn\*.exe 2>nul
del %ROOT_PATH%\examples\dyn\*.c 2>nul

