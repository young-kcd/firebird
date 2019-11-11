# SQL Language Extension: SET BIND

##	Implements capability to setup columns coercion rules in current session.


### Author:

	Alex Peshkoff <peshkoff@mail.ru>


### Syntax is:

```sql
SET BIND OF type-from TO { type-to | LEGACY };
SET BIND OF type NATIVE;
```

### Description:

Makes it possible to define rules of describing types of returned to the client columns in non-standard way -
`type-from` is replaced with `type-to`, automatic data coercion takes place.

Except SQL-statement there are two more ways to specify data coercion - tag `isc_dpb_bind` in DPB
and `SetBind` parameter in firebird.conf & databases.conf. The later the rule is introduced (.conf->DPB->SQL)
the higher priotiy it has. I.e. one can override any preconfigured settings from SQL statement.

When incomplete type definition is used (i.e. `CHAR` instead `CHAR(n)`) in left part of `SET BIND` coercion
will take place for all `CHAR` columns, not only default `CHAR(1)`.
When incomplete type definiton is used in right side of the statement (TO part) firebird engine will define missing
details about that type automatically based on source column.

Special `TO` part format `LEGACY` is used when datatype, missing in previous FB version, should be represented in
a way, understandable by old client software (may be with some data losses). For example, `NUMERIC(38)` in legacy
form will be represented as `NUMERIC(18)`.

Setting `NATIVE` means `type` will be used as if there were no previous rules for it.


### Samples:

```sql
SELECT CAST('123.45' AS DECFLOAT(16)) FROM RDB$DATABASE;	--native
```
```
                   CAST
=======================
                 123.45
```

```sql
SET BIND OF DECFLOAT TO DOUBLE PRECISION;
SELECT CAST('123.45' AS DECFLOAT(16)) FROM RDB$DATABASE;	--double
```
```
                   CAST
=======================
      123.4500000000000
```

```sql
SET BIND OF DECFLOAT(34) TO CHAR;
SELECT CAST('123.45' AS DECFLOAT(16)) FROM RDB$DATABASE;	--still double
```
```
                   CAST
=======================
      123.4500000000000
```

```sql
SELECT CAST('123.45' AS DECFLOAT(34)) FROM RDB$DATABASE;	--text
```
```
CAST
==========================================
123.45
```
