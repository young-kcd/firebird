BEGIN {Output = 1}
/NOT_FLINTSTONE/ {if(FLINTSTONE) Output = 0 ; next}
/END_FLINTSTONE/ {if(FLINTSTONE) Output = 1 ; next}
/FLINT_CACHE/ {if(CACHE && FLINTSTONE) Output = 1 ; next}
/END_CACHE/ {if(CACHE && FLINTSTONE) Output = 0 ; next}
/FLINT_WAL/ {if(WAL && FLINTSTONE) Output = 1 ; next}
/END_WAL/ {if(WAL && FLINTSTONE) Output = 0 ; next}
