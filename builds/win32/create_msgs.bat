@echo off

@call setenvvar.bat
@if errorlevel 1 (goto :END)

@if "%1"=="msg" goto MSG

@if exist "%FB_GEN_DIR%\dbs\msg.fdb" del "%FB_GEN_DIR%\dbs\msg.fdb"

@echo create database '%FB_GEN_DB_DIR%/dbs/msg.fdb'; | "%FB_GEN_DIR%\isql_embed" -q
@set FB_MSG_ISQL=@"%FB_GEN_DIR%\isql_embed" -q %FB_GEN_DB_DIR%/dbs/msg.fdb -i %FB_ROOT_PATH%\src\msgs\
@%FB_MSG_ISQL%msg.sql
@%FB_MSG_ISQL%facilities2.sql
@echo.
@echo loading locales
@%FB_MSG_ISQL%locales.sql
@echo loading history
@%FB_MSG_ISQL%history2.sql
@echo loading messages
@%FB_MSG_ISQL%messages2.sql
@echo loading symbols
@%FB_MSG_ISQL%symbols2.sql
@echo loading system errors
@%FB_MSG_ISQL%system_errors2.sql
@echo loading French translation
@%FB_MSG_ISQL%transmsgs.fr_FR.sql
@echo loading German translation
@%FB_MSG_ISQL%transmsgs.de_DE.sql

@if "%1"=="db" goto END

:MSG
@echo Building message file...
@%FB_GEN_DIR%\build_msg -f %FB_GEN_DB_DIR%/firebird.msg -D %FB_GEN_DB_DIR%\dbs\msg.fdb
@echo Building codes header...
@%FB_GEN_DIR%\codes %FB_ROOT_PATH%\src\include\gen %FB_ROOT_PATH%\lang_helpers

:END
