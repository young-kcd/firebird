@nmake /f build_msg.mak
@build_msg -D %DB_PATH%/generated/msg.fdb
@nmake /f codes.mak
@codes ..\..\src\include\gen
