yacc -l -d -b dsql ..\..\src\dsql\parse.y
sed -f ..\..\src\dsql\parse.sed <dsql.tab.c >..\..\src\dsql\parse.cpp
copy dsql.tab.h ..\..\src\dsql\dsql.tab.h
del dsql.tab.c
del dsql.tab.h
