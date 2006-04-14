@echo off

@call setenvvar2.bat
@if errorlevel 1 (goto :END)

@if "%1"=="msg" goto MSG

@if exist "%DB_PATH%\gen\dbs\msg.fdb" del "%DB_PATH%\gen\dbs\msg.fdb"

@echo create database '%DB_PATH%\gen\dbs\msg.fdb'; | "%ROOT_PATH%\gen\isql_embed" -q
@set MSG_ISQL=@"%ROOT_PATH%\gen\isql_embed" -q %DB_PATH%\gen\dbs\msg.fdb -i %ROOT_PATH%\src\msgs\
@%MSG_ISQL%msg.sql
@%MSG_ISQL%facilities.sql
@echo.
@echo loading locales
@%MSG_ISQL%locales.sql
@echo loading history
@%MSG_ISQL%history.sql
@echo loading messages
@%MSG_ISQL%messages.sql
@echo loading symbols
@%MSG_ISQL%symbols.sql
@echo loading system errors
@%MSG_ISQL%system_errors.sql
@echo loading French translation
@%MSG_ISQL%transmsgs.fr_FR.sql
@echo loading German translation
@%MSG_ISQL%transmsgs.de_DE.sql

@if "%1"=="db" goto END

:MSG
@echo Building message file and codes header...
@%ROOT_PATH%\gen\build_msg -f %DB_PATH%/gen/firebird.msg -D %DB_PATH%/gen/dbs/msg.fdb
@%ROOT_PATH%\gen\codes %ROOT_PATH%\src\include\gen %ROOT_PATH%\lang_helpers

:END
