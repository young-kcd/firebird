DDL enhancements in Firebird v2.
--------------------------------

Author: Claudio Valderrama C. <cvalde at usa.net>

Other DDL enhancements may have their own README file.

1) Ability to signal SQL NULL via a NULL pointer.
(Claudio Valderrama C.)

Previous to Firebird v2, UDF authors only could guess they got a null value,
but they couldn't be sure, so this let to several problems with UDFs. People
ended up assuming that a null string would be passed as an empty string, a null
numeric would be the same than zero and a null date would mean the base date used
by the engine. Of course, for a numeric value, the author only could assume null
if the UDF was done for an environments where it's known that value is not possible
normally. But several UDFs (including ib_udf supplied with FB) assumed an empty
string most likely would mean a null parameter than a string of length zero. The
trick may work with CHAR type (the minimal declared CHAR length is one and would
contain a blank character normally, so a binary zero in the first position would
signal effectively NULL) but it doesn't work with VARCHAR or CSTRING, where length
zero is valid. The other solution was to rely on raw descriptors, but this draws
people to an area with a lot of things to check, more than they would want to tackle.
The biggest problem is that the engine won't obey the declared type for a parameter;
it will simply send whatever data it has for that parameter, so the UDF is left to
decide whether to reject or to try to convert the parameter to the expected data
type. Since UDFs don't have a formal mechanism to signal errors, the returned value
will have to be used as an indicator.
But the basic problem was how to keep the simplicity of the typical declarations
(no descriptors) while at the same time being able to signal null. The engine
normally passed UDF parameters by reference and it means in practical terms to
pass a pointer to the data. By simply passing a null pointer we can tell the UDF
we have SQL NULL, but since we can't afford to crash an unknown number of different
public and private UDFs in use that don't expect NULL, we had to enhance the syntax
to be able to request explicitly NULL handling. Therefore, only UDFs that are able
to deal with the new scenario can request SQL NULL signaling.
To avoid adding more keywords, the NULL keyword is appended to the UDF parameter
type and this is all the required change. Example:
declare external function sample int null returns int by value...;

If you are already using functions from ib_udf and want to take advantage of
null signaling (and null recognition) in some functions, you should connect to
your desired database and run
upgrade/v2/ib_udf_upgrade.sql
and commit afterwards, preferable when no more users are connected to the database.
The code in the listed functions in that script has been modified to recognize
null only when NULL is signaled by the engine. Therefore, starting with FB v2,
rtrim and ltrim no longer assume that an empty string means a NULL string. If you
don't upgrade, the functions won't crash. They simply won't be able to detect NULL.
If you never have used ib_udf and want to do that, you should connect to your desired
database, run
udf/ib_udf2.sql
and commit afterwards, preferable when no more users are connected to the database.
Note the "2" at the end of the name. The original script for FB v1.5 is still
available in the same directory.
The directories upgrade" and "udf" are inside the home directory of your FB v2
installation.


2) Implemented REVOKE ADMIN OPTION FROM user
(Dmitry Yemanov.)

SYSDBA, the database creator or the owner of an object can grant rights on that
object to other users. However, those rights can be made inheritable, too. By using
WITH GRANT OPTION, the grantor gives the grantee the right to become a grantor of
the same rights in turn. This ability can be removed by the original grantor with
REVOKE GRANT OPTION FROM user.

However, there's a second form that involves roles. Instead of specifying the
same rights for many users (soon it becomes a maintenance nightmare) you can
create a role, assign rights to that role and then grant it to a group of users.
By simply changing the role's rights you affect all those users. By using
WITH ADMIN OPTION, the grantor (typically the role creator) gives the grantee the
right to become a grantor of the same role in turn. Until FB v2, this ability
couldn't be removed unless the original grantor fiddles with system tables directly.
Now, the ability to grant the role can be removed by the original grantor with
REVOKE ADMIN OPTION FROM user.


