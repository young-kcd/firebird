# SQL Language Extension: SET BIND

##	Implements capability to setup columns coercion rules in current session.


### Author:

	Alex Peshkoff <peshkoff@mail.ru>


### Syntax is:

```sql
SET BIND OF type1 TO type2;
SET BIND OF type NATIVE;
```

### Description:

Makes it possible to define rules of describing types of returned to the client columns in non-standard way - `type1` is replaced with `type2`, automatic data coercion takes place.

When incomplete type definition is used (i.e. `CHAR` instead `CHAR(n)`) in left part of `SET BIND` coercion will take place for all `CHAR` columns, not only default `CHAR(1)`. When incomplete type definiton is used in right side of the statement firebird engine will define missing parts automatically based on source column.

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
