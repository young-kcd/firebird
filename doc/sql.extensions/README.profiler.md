# Profiler (FB 5.0)

The profiler allows users to measure performance cost of SQL and PSQL code.

It's implemented with a system package in the engine passing data to a profiler plugin.

This documentation treats the engine and plugin parts as a single thing, in the way the default profiler is going to be used.

The `RDB$PROFILER` package allows to profile execution of PSQL code collecting statistics of how many times each line was executed along with its minimum, maximum and accumulated execution times (with nanoseconds precision), as well open and fetch statistics of implicit and explicit SQL cursors.

To collect profile data, an user must first start a profile session with `RDB$PROFILER.START_SESSION`. This function returns an profile session ID which is later stored in the profiler snapshot tables to be queried and analyzed by the user.

After a session is started, PSQL and SQL statements statistics starts to be collected in memory. Note that a profile session collects data only of statements executed in the same attachment where the session was started.

Data is aggregated and stored per requests (i.e. a statement execution). When querying snapshot tables, user may do extra aggregation per statements or use the auxiliary views that do that automatically.

A session may be paused to temporary disable statistics collecting. It may be resumed later to return statistics collection in the same session.

A new session may be started when a session is already active. In this case it has the same semantics of finishing the current session with `RDB$PROFILER.FINISH_SESSION(FALSE)` so snapshots tables are not updated in the same moment.

To analyze the collected data, the user must flush the data to the snapshot tables, which may be done finishing or pausing a session (with `FLUSH` parameter set to `TRUE`) or calling `RDB$PROFILER.FLUSH`.

Following is a sample profile session and queries for data analysis.

```
-- Preparation - create table and routines that will be analyzed

create table tab (
    id integer not null,
    val integer not null
);

set term !;

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

set term ;!

-- Start profiling

select rdb$profiler.start_session('Default_Profiler', 'Profile Session 1') from rdb$database;

set term !;

execute block
as
begin
    execute procedure ins;
    delete from tab;
end!

set term ;!

execute procedure rdb$profiler.finish_session(true);

execute procedure ins;

select rdb$profiler.start_session('Default_Profiler', 'Profile Session 2') from rdb$database;

select mod(id, 5),
       sum(val)
  from tab
  where id <= 50
  group by mod(id, 5)
  order by sum(val);

execute procedure rdb$profiler.finish_session(true);

-- Data analysis

select * from fbprof$sessions;

select * from fbprof$psql_stats_view;

select * from fbprof$record_source_stats_view;

select preq.*
  from fbprof$requests preq
  join fbprof$sessions pses
    on pses.session_id = preq.session_id and
       pses.description = 'Profile Session 1';

select pstat.*
  from fbprof$psql_stats pstat
  join fbprof$sessions pses
    on pses.session_id = pstat.session_id and
       pses.description = 'Profile Session 1'
  order by pstat.session_id,
           pstat.request_id,
           pstat.line_num,
           pstat.column_num;

select pstat.*
  from fbprof$record_source_stats pstat
  join fbprof$sessions pses
    on pses.session_id = pstat.session_id and
       pses.description = 'Profile Session 2'
  order by pstat.session_id,
           pstat.request_id,
           pstat.cursor_id,
           pstat.record_source_id;
```

## Function `START_SESSION`

`RDB$PROFILER.START_SESSION` starts a new profiler session, turns it the current session and return its identifier.

Input parameters:
 - `PLUGIN_NAME` type `VARCHAR(255) CHARACTER SET UTF8`
 - `DESCRIPTION` type `VARCHAR(255) CHARACTER SET UTF8`

Return type: `BIGINT NOT NULL`.

## Procedure `PAUSE_SESSION`

`RDB$PROFILER.PAUSE_SESSION` pauses the current profiler session so the next executed statements statistics are not collected.

If `FLUSH` is `TRUE` the snapshot tables are updated with data up to the current moment. Otherwise data remains only in memory for later update.

Calling `RDB$PROFILER.PAUSE_SESSION(TRUE)` has the same semantics of calling `RDB$PROFILER.PAUSE_SESSION(FALSE)` followed by `RDB$PROFILER.FLUSH`.

Input parameters:
 - `FLUSH` type `BOOLEAN NOT NULL`

## Procedure `RESUME_SESSION`

`RDB$PROFILER.RESUME_SESSION` resumes the current profiler session if it was paused so the next executed statements statistics are collected again.

## Procedure `FINISH_SESSION`

`RDB$PROFILER.FINISH_SESSION` finishes the current profiler session.

If `FLUSH` is `TRUE` the snapshot tables are updated with data of the finished session (and old finished sessions not yet present in the snapshot). Otherwise data remains only in memory for later update.

Calling `RDB$PROFILER.FINISH_SESSION(TRUE)` has the same semantics of calling `RDB$PROFILER.FINISH_SESSION(FALSE)` followed by `RDB$PROFILER.FLUSH`.

Input parameters:
 - `FLUSH` type `BOOLEAN NOT NULL`

## Procedure `FLUSH`

`RDB$PROFILER.FLUSH` updates the snapshot tables with data from the profile sessions in memory.

After update data is stored in tables `FBPROF$SESSIONS`, `FBPROF$STATEMENTS`, `FBPROF$RECORD_SOURCES`, `FBPROF$REQUESTS`, `FBPROF$PSQL_STATS` and `FBPROF$RECORD_SOURCE_STATS` and may be read and analyzed by the user.

It also removes finished sessions from memory.

# Snapshot tables

Snapshot tables (as well views and sequence) are automatically created in the first usage of the profiler. They are owned by the current user with read/write permissions for `PUBLIC`.

When a session is deleted the related data in others profiler snapshot tables are automatically deleted too through foregin keys with `DELETE CASCADE` option.

Below is the list of tables that stores profile data.

## Table `FBPROF$SESSIONS`

 - `SESSION_ID` type `BIGINT` - Profile session ID
 - `ATTACHMENT_ID` type `BIGINT` - Attachment ID
 - `USER_NAME` type `CHAR(63) CHARACTER SET UTF8` - User name
 - `DESCRIPTION` type `VARCHAR(255) CHARACTER SET UTF8` - Description passed in `RDB$PROFILER.START_SESSION`
 - `START_TIMESTAMP` type `TIMESTAMP WITH TIME ZONE` - Moment the profile session was started
 - `FINISH_TIMESTAMP` type `TIMESTAMP WITH TIME ZONE` - Moment the profile session was finished (NULL when not finished)
 - Primary key: `SESSION_ID`

## Table `FBPROF$STATEMENTS`

 - `SESSION_ID` type `BIGINT` - Profile session ID
 - `STATEMENT_ID` type `BIGINT` - Statement ID
 - `PARENT_STATEMENT_ID` type `BIGINT` - Parent statement ID - related to sub routines
 - `STATEMENT_TYPE` type `VARCHAR(20) CHARACTER SET UTF8` - BLOCK, FUNCTION, PROCEDURE or TRIGGER
 - `PACKAGE_NAME` type `CHAR(63) CHARACTER SET UTF8` - Package of FUNCTION or PROCEDURE
 - `ROUTINE_NAME` type `CHAR(63) CHARACTER SET UTF8` - Routine name of FUNCTION, PROCEDURE or TRIGGER
 - `SQL_TEXT` type `BLOB subtype TEXT CHARACTER SET UTF8` - SQL text for BLOCK
 - Primary key: `SESSION_ID, STATEMENT_ID`

## Table `FBPROF$RECORD_SOURCES`

 - `SESSION_ID` type `BIGINT` - Profile session ID
 - `STATEMENT_ID` type `BIGINT` - Statement ID
 - `CURSOR_ID` type `BIGINT` - Cursor ID
 - `RECORD_SOURCE_ID` type `BIGINT` - Record source ID
 - `PARENT_RECORD_SOURCE_ID` type `BIGINT` - Parent record source ID
 - `ACCESS_PATH` type `VARCHAR(255) CHARACTER SET UTF8` - Access path for the record source
 - Primary key: `SESSION_ID, STATEMENT_ID, CURSOR_ID, RECORD_SOURCE_ID`

## Table `FBPROF$REQUESTS`

 - `SESSION_ID` type `BIGINT` - Profile session ID
 - `REQUEST_ID` type `BIGINT` - Request ID
 - `STATEMENT_ID` type `BIGINT` - Statement ID
 - `CALLER_REQUEST_ID` type `BIGINT` - Caller request ID
 - `START_TIMESTAMP` type `TIMESTAMP WITH TIME ZONE` - Moment this request was first gathered profile data
 - `FINISH_TIMESTAMP` type `TIMESTAMP WITH TIME ZONE` - Moment this request was finished
 - Primary key: `SESSION_ID, REQUEST_ID`

## Table `FBPROF$PSQL_STATS`

 - `SESSION_ID` type `BIGINT` - Profile session ID
 - `REQUEST_ID` type `BIGINT` - Request ID
 - `LINE_NUM` type `INTEGER` - Line number of the statement
 - `COLUMN_NUM` type `INTEGER` - Column number of the statement
 - `STATEMENT_ID` type `BIGINT` - Statement ID
 - `COUNTER` type `BIGINT` - Number of executed times of the statement
 - `MIN_TIME` type `BIGINT` - Minimal time (in nanoseconds) of a statement execution
 - `MAX_TIME` type `BIGINT` - Maximum time (in nanoseconds) of a statement execution
 - `TOTAL_TIME` type `BIGINT` - Accumulated execution time (in nanoseconds) of the statement
 - Primary key: `SESSION_ID, REQUEST_ID, LINE_NUM, COLUMN_NUM`

## Table `FBPROF$RECORD_SOURCE_STATS`

 - `SESSION_ID` type `BIGINT` - Profile session ID
 - `REQUEST_ID` type `BIGINT` - Request ID
 - `CURSOR_ID` type `BIGINT` - Cursor ID
 - `RECORD_SOURCE_ID` type `BIGINT` - Record source ID
 - `STATEMENT_ID` type `BIGINT` - Statement ID
 - `OPEN_COUNTER` type `BIGINT` - Number of open times of the record source
 - `OPEN_MIN_TIME` type `BIGINT` - Minimal time (in nanoseconds) of a record source open
 - `OPEN_MAX_TIME` type `BIGINT` - Maximum time (in nanoseconds) of a record source open
 - `OPEN_TOTAL_TIME` type `BIGINT` - Accumulated open time (in nanoseconds) of the record source
 - `FETCH_COUNTER` type `BIGINT` - Number of fetch times of the record source
 - `FETCH_MIN_TIME` type `BIGINT` - Minimal time (in nanoseconds) of a record source fetch
 - `FETCH_MAX_TIME` type `BIGINT` - Maximum time (in nanoseconds) of a record source fetch
 - `FETCH_TOTAL_TIME` type `BIGINT` - Accumulated fetch time (in nanoseconds) of the record source
 - Primary key: `SESSION_ID, REQUEST_ID, CURSOR_ID, RECORD_SOURCE_ID`

# Auxiliary views

These views help profile data extraction aggregated at statement level.

They should be the preferred way to analyze the collected data. They can also be used together with the tables to get additional data not present on the views.

After hot spots are found, one can drill down in the data at the request level through the tables.

## View `FBPROF$PSQL_STATS_VIEW`
```
select pstat.session_id,
       pstat.statement_id,
       sta.statement_type,
       sta.package_name,
       sta.routine_name,
       sta.parent_statement_id,
       sta_parent.statement_type parent_statement_type,
       sta_parent.routine_name parent_routine_name,
       (select sql_text
          from fbprof$statements
          where session_id = pstat.session_id and
                statement_id = coalesce(sta.parent_statement_id, pstat.statement_id)
       ) sql_text,
       pstat.line_num,
       pstat.column_num,
       sum(pstat.counter) counter,
       min(pstat.min_time) min_time,
       max(pstat.max_time) max_time,
       sum(pstat.total_time) total_time,
       sum(pstat.total_time) / nullif(sum(pstat.counter), 0) avg_time
  from fbprof$psql_stats pstat
  join fbprof$statements sta
    on sta.session_id = pstat.session_id and
       sta.statement_id = pstat.statement_id
  left join fbprof$statements sta_parent
    on sta_parent.session_id = sta.session_id and
       sta_parent.statement_id = sta.parent_statement_id
  group by pstat.session_id,
           pstat.statement_id,
           sta.statement_type,
           sta.package_name,
           sta.routine_name,
           sta.parent_statement_id,
           sta_parent.statement_type,
           sta_parent.routine_name,
           pstat.line_num,
           pstat.column_num
  order by sum(pstat.total_time) desc
```

## View `FBPROF$RECORD_SOURCE_STATS_VIEW`
```
select rstat.session_id,
       rstat.statement_id,
       sta.statement_type,
       sta.package_name,
       sta.routine_name,
       sta.parent_statement_id,
       sta_parent.statement_type parent_statement_type,
       sta_parent.routine_name parent_routine_name,
       (select sql_text
          from fbprof$statements
          where session_id = rstat.session_id and
                statement_id = coalesce(sta.parent_statement_id, rstat.statement_id)
       ) sql_text,
       rstat.cursor_id,
       rstat.record_source_id,
       recsrc.parent_record_source_id,
       recsrc.access_path,
       sum(rstat.open_counter) open_counter,
       min(rstat.open_min_time) open_min_time,
       max(rstat.open_max_time) open_max_time,
       sum(rstat.open_total_time) open_total_time,
       sum(rstat.open_total_time) / nullif(sum(rstat.open_counter), 0) open_avg_time,
       sum(rstat.fetch_counter) fetch_counter,
       min(rstat.fetch_min_time) fetch_min_time,
       max(rstat.fetch_max_time) fetch_max_time,
       sum(rstat.fetch_total_time) fetch_total_time,
       sum(rstat.fetch_total_time) / nullif(sum(rstat.fetch_counter), 0) fetch_avg_time,
       coalesce(sum(rstat.open_total_time), 0) + coalesce(sum(rstat.fetch_total_time), 0) open_fetch_total_time
  from fbprof$record_source_stats rstat
  join fbprof$record_sources recsrc
    on recsrc.session_id = rstat.session_id and
       recsrc.statement_id = rstat.statement_id and
       recsrc.cursor_id = rstat.cursor_id and
       recsrc.record_source_id = rstat.record_source_id
  join fbprof$statements sta
    on sta.session_id = rstat.session_id and
       sta.statement_id = rstat.statement_id
  left join fbprof$statements sta_parent
    on sta_parent.session_id = sta.session_id and
       sta_parent.statement_id = sta.parent_statement_id
  group by rstat.session_id,
           rstat.statement_id,
           sta.statement_type,
           sta.package_name,
           sta.routine_name,
           sta.parent_statement_id,
           sta_parent.statement_type,
           sta_parent.routine_name,
           rstat.cursor_id,
           rstat.record_source_id,
           recsrc.parent_record_source_id,
           recsrc.access_path
  order by coalesce(sum(rstat.open_total_time), 0) + coalesce(sum(rstat.fetch_total_time), 0) desc
```
