
.FACILITY GDS,941
.BASE 1

.SEVERITY ERROR
BADBLK/FAO_COUNT=1	<Bad !AC block encountered>
INVPOOLCL		<Invalid pool class given>
NOPOOLIDS		<All available dynamic memory pools have been exhausted>
RELBADBLK		<RLBLK: Attempted to release bad block>
BLKTOOBIG		<Attempted to get a block that is too big>
BUFEXH			<Buffers exhausted>
SYNTAXERR/FAO_COUNT=3	<Syntax error in BLR: !AC, offset !UL, operator !UL>
BUFINUSE		<Buffer was in use>
BDBINCON		<BDB inconsistent>
REQINUSE		<Request already in use>
BADODSVER/FAO_COUNT=2	<Bad version (!UL) of on-disk structure (expected !UL)>
RELNOTDEF/FAO_COUNT=1	<Relation !AC is not defined>
FLDNOTDEF/FAO_COUNT=2	<Field !AC is not defined in relation !AC>

.SEVERITY INFORMATIONAL
DIRTYPAGE/FAO_COUNT=1	<Page !UL accessed for write must be written>
WAIFORTRA/FAO_COUNT=1	<Waiting for transaction !UL>       

.SEVERITY ERROR
DOUBLELOC		<Double lock during lock conversion>

.SEVERITY INFORMATIONAL
NODNOTFND		<Index not found during delete>
DUPNODFND		<Duplicate index entry found>
LOCNOTMAR/FAO_COUNT=1	<Page !XL lock for write but not written>

.SEVERITY FATAL
BADPAGTYP/FAO_COUNT=2	<Bad page type, page=!XL, type expected=!UL>
CORRUPT/FAO_COUNT=2	<Database is corrupt, page=!XL, !AC>

.SEVERITY INFORMATIONAL
BADPAGE/FAO_COUNT=1	<Page checksum error on page=!XL>
BADINDEX/FAO_COUNT=1	<Can't create index !AC. Improper or non-existent field(s)>

.SEVERITY ERROR
DBBNOTZER		<Database handle not zero for attach/create database>
TRANOTZER/FAO_COUNT=1	<Transaction handle not zero for !AC>
TRAREQMIS		<Request handle and transaction handle conflict>
BADHNDCNT		<Only one database handle is supported on RDB$START_TRANSACTION>
WROTPBVER/FAO_COUNT=2	<Bad TPB version number.  Expected version !UL, found version !UL>
WROBLRVER/FAO_COUNT=2	<Bad BLR version number.  Expected version !UL, found version !UL>
WRODPBVER/FAO_COUNT=2	<Bad DPB version number.  Expected version !UL, found version !UL>
SEGSTRNOTSUP/FAO_COUNT=1	<!AC operation is not supported on segmented strings>
BADRELATION/FAO_COUNT=1	<Relation (ID = !UL) is ill-formed (bad view?)>

.SEVERITY INFORMATIONAL
NODETACH/FAO_COUNT=2	<Process exit with database attached; !UL transaction!%S>

.SEVERITY ERROR
NOTREMOTE		<Filename did not contain a remote node name>
TRAINLIM/FAO_COUNT=1	<Transaction !UL is stuck in limbo>
NOTINLIM/FAO_COUNT=1	<Can't reconnect to transaction !UL (not in limbo)>
TRAOUTSTA		<Can't detach, database has transactions outstanding>
CONNECT_REJECT		<Host and server can't agree on a protocol>
DBFILE/FAO_COUNT=1	<Database is !AC>

.SEVERITY INFORMATIONAL
ORPHAN_PAGES/FAO_COUNT=3	<!UL Orphan Page!%S found !AC>

.SEVERITY ERROR
MISC_INTERPRETED/FAO_COUNT=1	<!AC>
.END
