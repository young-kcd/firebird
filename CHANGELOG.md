# v4.0 Alpha 1 (unreleased)

## Bugfixes

* [CORE-5141](http://tracker.firebirdsql.org/browse/CORE-5141): Field definition allows several NOT NULL clauses  
  Contributor(s): Dimitry Sibiryakov

* [CORE-4985](http://tracker.firebirdsql.org/browse/CORE-4985): Non-privileged user can implicitly count records in a restricted table  
  Contributor(s): Dmitry Yemanov

* [CORE-4701](http://tracker.firebirdsql.org/browse/CORE-4701): Index and blob garbage collection don't take into accout data in undo log  
  Contributor(s): Dimitry Sibiryakov

* [CORE-4483](http://tracker.firebirdsql.org/browse/CORE-4483):Changed data not visible in WHEN-section if exception occured inside SP that has been called from this code
  Contributor(s): Dimitry Sibiryakov

* [CORE-4424](http://tracker.firebirdsql.org/browse/CORE-4424): Rollback to wrong savepoint if several exception handlers on the same level are executed  
  Contributor(s): Dimitry Sibiryakov

## Improvements

* [CORE-5119](http://tracker.firebirdsql.org/browse/CORE-5119): Support autocommit mode in SET TRANSACTION statement  
  Contributor(s): Dmitry Yemanov

* [CORE-2192](http://tracker.firebirdsql.org/browse/CORE-2192): Extend maximum database page size to 32KB
  Contributor(s): Dmitry Yemanov

## New features

* [CORE-1815](http://tracker.firebirdsql.org/browse/CORE-1815): Ability to grant role to another role
  Contributor(s): Roman Simakov

* [CORE-751](http://tracker.firebirdsql.org/browse/CORE-751): Implicitly active roles (and their permissions summarized)
  Contributor(s): Roman Simakov
