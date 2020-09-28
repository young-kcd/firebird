# `RDB$PROFILER` package (FB 5.0)

`RDB$PROFILER` package allows to profile execution of PSQL code gathering statistics of how many times each line was executed along with its minimum, maximum and accumulated execution time (with nanoseconds precision).

To gather profile data, an user must first start a profile session with `RDB$PROFILER.START_SESSION`. This function returns an profile session ID which is later stored in the profile snapshot system tables to be read and analyzed by the user.

After a session is started, PSQL statements execution statistics starts to be collected in engine memory. Note that a profile session gathers data only of statements executed in the same attachment where the session was started.

A session may be paused to temporary disable statistics gathering in a session. It may be resumed later to return gather statistics in the same session.

A new session may be started when a session is already active. In this case it has the same semantics of finishing the current session with `RDB$PROFILER.FINISH_SESSION(FALSE)` so snapshots are not updated in the same moment.

To analyze the gathered data, the user must update the snapshots which may be done finishing or pausing a session (with `UPDATE_SNAPSHOT` parameter set to `TRUE`) or calling `RDB$PROFILER.UPDATE_SNAPSHOT`.

Following is a sample profile session.

```
-- Preparation - create table and routines that will be analyzed

set term !;

create table tab (
    id integer not null,
	val integer not null
)!

create or alter function mult(p1 integer, p2 integer) returns integer
as
begin
    return p1 * p2;
end!

create or alter procedure ins
as
    declare n integer = 1;
begin
    while (n <= 1000)
    do
    begin
        if (mod(n, 2) = 1) then
            insert into tab values (:n, mult(:n, 2));
        n = n + 1;
    end
end!

-- Start profiling

select rdb$profiler.start_session('Profile XPTO') from rdb$database!

execute block
as
begin
    execute procedure ins;
    delete from tab;
end!

execute procedure rdb$profiler.finish_session(true)!

select * from rdb$profile_sessions!
select * from rdb$profile_requests!
select * from rdb$profile_stats order by rdb$profile_session_id, rdb$profile_request_id, rdb$line, rdb$column!
```

Result data for `RDB$PROFILE_SESSIONS`:

| RDB$PROFILE_SESSION_ID | RDB$TIMESTAMP          |
|-----------------------:|------------------------|
|  1 | 2020-09-27 15:45:58.5930 America/Sao_Paulo |

Result data for `RDB$PROFILE_REQUESTS`:

| RDB$PROFILE_SESSION_ID | RDB$PROFILE_REQUEST_ID | RDB$TIMESTAMP                              | RDB$REQUEST_TYPE | RDB$PACKAGE_NAME | RDB$ROUTINE_NAME | RDB$SQL_TEXT            |
|-----------------------:|-----------------------:|--------------------------------------------|------------------|------------------|------------------|-------------------------|
|                      1 |                    118 | 2020-09-27 15:46:02.5230 America/Sao_Paulo | BLOCK            |                  |                  | * Text of EXECUTE BLOCK |
|                      1 |                    119 | 2020-09-27 15:46:02.4820 America/Sao_Paulo | PROCEDURE        |                  | INS              |                         |
|                      1 |                    120 | 2020-09-27 15:46:02.4820 America/Sao_Paulo | FUNCTION         |                  | MULT             |                         |

Result data for `RDB$PROFILE_STATS`:


| RDB$PROFILE_SESSION_ID | RDB$PROFILE_REQUEST_ID | RDB$LINE | RDB$COLUMN | RDB$COUNTER | RDB$MIN_TIME | RDB$MAX_TIME | RDB$ACCUMULATED_TIME |
|-----------------------:|-----------------------:|---------:|-----------:|------------:|-------------:|-------------:|---------------------:|
|                      1 |                    118 |        4 |          5 |           1 |  41567976    |     41567976 |             41567976 |
|                      1 |                    118 |        5 |          1 |           1 |     83132    |        83132 |                83132 |
|                      1 |                    119 |        3 |          5 |           1 |      1780    |         1780 |                 1780 |
|                      1 |                    119 |        5 |          5 |        1001 |       577    |        30277 |              1621196 |
|                      1 |                    119 |        8 |          9 |        1000 |       883    |        46605 |              2049447 |
|                      1 |                    119 |        9 |         13 |         500 |       750    |        25497 |               806646 |
|                      1 |                    119 |       10 |          9 |        1000 |      1058    |      5888615 |             37007949 |
|                      1 |                    120 |        4 |          5 |         500 |      1934    |        53578 |              1939773 |

## Function `START_SESSION`

`RDB$PROFILER.START_SESSION` starts a new profiler session, turns it the current session and return its identifier.

Input parameters:
 - `DESCRIPTION` type `VARCHAR(255) CHARACTER SET UTF8`

Return type: `BIGINT NOT NULL`.

## Procedure `PAUSE_SESSION`

`RDB$PROFILER.PAUSE_SESSION` pauses the current profiler session so the following PSQL executed statements are not accounted.

If `UPDATE_SNAPSHOT` is `TRUE` the snapshot tables are updated with data up to the current moment. Otherwise data remains only in the engine memory for later update.

Calling `RDB$PROFILER.PAUSE_SESSION(TRUE)` has the same semantics of calling `RDB$PROFILER.PAUSE_SESSION(FALSE)` followed by `RDB$PROFILER.UPDATE_SNAPSHOT`.

Input parameters:
 - `UPDATE_SNAPSHOT` type `BOOLEAN NOT NULL`

## Procedure `RESUME_SESSION`

`RDB$PROFILER.RESUME_SESSION` resumes the current profiler session if it was paused so the following PSQL statements are accounted again.

## Procedure `FINISH_SESSION`

`RDB$PROFILER.FINISH_SESSION` finishes the current profiler session.

If `UPDATE_SNAPSHOT` is `TRUE` the snapshot tables are updated with data of the finished session (and old finished sessions not yet present in the snapshot). Otherwise data remains only in the engine memory for later update.

Calling `RDB$PROFILER.FINISH_SESSION(TRUE)` has the same semantics of calling `RDB$PROFILER.FINISH_SESSION(FALSE)` followed by `RDB$PROFILER.UPDATE_SNAPSHOT`.

Input parameters:
 - `UPDATE_SNAPSHOT` type `BOOLEAN NOT NULL`

## Procedure `UPDATE_SNAPSHOT`

`RDB$PROFILER.UPDATE_SNAPSHOT` updates the system tables snapshots with data from the profile sessions in memory.

After update data is stored in tables `RDB$PROFILE_SESSIONS`, `RDB$PROFILE_REQUESTS` and `RDB$PROFILE_STATS` and may be read and analyzed by the user.

It also removes finished sessions from engine memory, so if `RDB$PROFILER.PURGE_SNAPSHOTS` is later called these data are not recovered.

## Procedure `PURGE_SNAPSHOTS`

`RDB$PROFILER.PURGE_SNAPSHOTS` removes all profile snapshots from the system tables and remove finished profile sessions from engine memory.

# Snapshot system tables

The profile snaphsot tables are like user's global temporary table with `ON COMMIT PRESERVE ROWS`. Data are per attachment and remains there until disconnected or explicitely purged with `RDB$PROFILER.PURGE_SNAPSHOTS`.

## Table `RDB$PROFILE_SESSIONS`

 - `RDB$PROFILE_SESSION_ID` type `BIGINT` - Profile session ID
 - `DESCRIPTION` type `VARCHAR(255) CHARACTER SET UTF8` - Description passed in `RDB$PROFILER.START_SESSION`
 - `RDB$START_TIMESTAMP` type `TIMESTAMP WITH TIME ZONE` - Moment the profile session was started
 - `RDB$FINISH_TIMESTAMP` type `TIMESTAMP WITH TIME ZONE` - Moment the profile session was finished (NULL when not finished)

## Table `RDB$PROFILE_REQUESTS`

 - `RDB$PROFILE_SESSION_ID` type `BIGINT` - Profile session ID
 - `RDB$PROFILE_REQUEST_ID` type `BIGINT` - Request ID
 - `RDB$TIMESTAMP` type `TIMESTAMP WITH TIME ZONE` - Moment this request was first gathered profile data
 - `RDB$REQUEST_TYPE` type `VARCHAR(20) CHARACTER SET UTF8` - BLOCK, FUNCTION, PROCEDURE or TRIGGER
 - `RDB$PACKAGE_NAME` type `CHAR(63) CHARACTER SET UTF8` - Package of FUNCTION or PROCEDURE
 - `RDB$ROUTINE_NAME` type `CHAR(63) CHARACTER SET UTF8` - Routine name of FUNCTION, PROCEDURE or TRIGGER
 - `RDB$SQL_TEXT` type `BLOB subtype TEXT CHARACTER SET UTF8` - SQL text for BLOCK

## Table `RDB$PROFILE_STATS`

 - `RDB$PROFILE_SESSION_ID` type `BIGINT` - Profile session ID
 - `RDB$PROFILE_REQUEST_ID` type `BIGINT` - Request ID
 - `RDB$LINE` type `INTEGER` - Line number of the statement
 - `RDB$COLUMN` type `INTEGER` - Column number of the statement
 - `RDB$COUNTER` type `BIGINT` - Number of executed times of the statement
 - `RDB$MIN_TIME` type `BIGINT` - Minimal time (in nanoseconds) of a statement execution
 - `RDB$MAX_TIME` type `BIGINT` - Maximum time (in nanoseconds) of a statement execution
 - `RDB$TOTAL_TIME` type `BIGINT` - Accumulated execution time (in nanoseconds) of the statement
