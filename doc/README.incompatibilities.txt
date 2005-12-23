********************************************************************************
  LIST OF KNOWN INCOMPATIBILITIES
  between versions 1.5.x and 2.x
********************************************************************************

This document describes all the changes that make v2.0 incompatible in any way
as compared with the previous release and hence could affect your databases and
applications.

Please read the below descriptions carefully before upgrading your software to
the new Firebird version.

INSTALLATION/CONFIGURATION
--------------------------

  * Security database is renamed to security2.fdb. If you upgrade the existing
    installation, please be sure you restore/copy your security database using
    the new name. It's recommended to upgrade the security database using the
    provided script in order to use more advanced accounts protection, but this
    is not strictly required.

  * Password hashes are now generated using the SHA-1 algorithm instead of the
    old DES one. If you want to preserve the already existing security database
    (i.e. just migrate it using backup/restore), you'll need to set the
    LegacyHash config option to 1 (TRUE). However, it's recommended to recreate
    your user accounts from scratch in order to generate stronger hashes for
    the stored passwords.

  * The new client library is not compatible with the older server (and vice
    versa) in regard to the local protocol, as the transport internals has been
    reimplemented (XNET instead of IPServer). If you need to use the local
    protocol, please ensure your server and client binaries are of the same
    version.

SECURITY
--------------------------

  * Direct connections to the security database are not allowed anymore. This is
    done for security reasons and also in order to isolate authentication
    mechanisms from the implementation. You should use GSTAT or the Services API
    to configurate user accounts instead.

  * Non-SYSDBA users no longer can see accounts of other users in the security
    database. A non-privileges user can only retrieve/modify its own account,
    including a password change.

UTILITIES
--------------------------

  * GBAK -R (former shortcut for "-REPLACE_DATABASE") no longer overwrites an
    existing database during restore, it reports an error instead. In order to
    use the previous behaviour, you may specify either the full syntax
    "GBAK -REPLACE_DATABASE" or use the new command
    "-R[ECREATE_DATABASE] OVERWRITE". The "-R" shortcut is now considered to
    represent the "-R[ECREATE_DATABASE]" command, while "-REP" is offered as a
    new shortcut for "-REP[LACE_DATABASE]". This change is targeted to prevent
    accidental database overwrites by the users treating "-R" as "restore".
    Those using the full syntax are expected to know what this restore mode
    actually means.

SQL CHECKING
--------------------------

  * It's now prohibited to reference columns of an aliased table using the table
    name, e.g. "SELECT TAB.A FROM TAB T". You should use the table alias
    instead: "SELECT T.A FROM TAB T". Such behaviour is declared by the SQL
    specification.

SQL EXECUTION RESULTS
--------------------------

  * NULLs are now considered the least possible values in terms of the ordering
    and they are sorted accordingly. This means that NULLs are placed in the
    beginning of the result set for ascending sorts and in the end for
    descending sorts. In the previous versions, NULLs were always on top. If
    you rely on the legacy NULLs placement, please use the NULLS FIRST option
    in your ORDER BY clauses.

  * Views updatable via triggers no longer perform direct table operations. In
    previous versions, a naturally updatable view with triggers passed the DML
    operation to the underlying table as well as executed the triggers. So if
    you followed the official documentation and used triggers to perform a table
    update, this update was actually done twice, causing either performance
    issues or errors (if blobs were affected). Now availability of triggers turn
    direct table updates off and everything depends on your triggers. If you
    rely on the legacy undocumented behaviour (e.g. use triggers for logging
    purposes only), you'll need to adjust your triggers to perform actual
    updates as well.

  * CURRENT_TIMESTAMP now returns milliseconds by default, while it returned
    truncated seconds value in the previous versions. If you don't need second
    fractions here, please specify the required accuracy explicitly, e.g. use
    CURRENT_TIMESTAMP(0).

  * ORDER BY now expands asterisks in the select list when referencing columns
    by ordinal. So now "SELECT T1.*, T2.COL FROM T1, T2 ORDER BY 2" sorts on the
    second column of table T1, while the previous versions sorted on T2.COL.
    However, this change makes queries like "SELECT * FROM TAB ORDER BY 5"
    possible.

PERFORMANCE
--------------------------

  * In SuperServer, garbage collection is now performed in the combined mode
    (cooperative + background) by default, whilst previous versions used only
    background garbage collection. It means that some queries may return data
    slower when there are enough old record versions in the affected tables
    (this is especially true for ODS10 and below databases which has ineffective
    garbage collection in indices). Please note the new behaviour generally
    guarantees better overall performance as the garbage collation is performed
    online and it prevents version chains from growing under high load. But you
    can rollback to the older behaviour using the GCPolicy config option.

  * This version never uses indices for the ALL predicate and it can cause a
    performance degradation for some your queries. This is done to fix known
    bugs causing wrong results returned by this predicate in cases when index
    was involved.
