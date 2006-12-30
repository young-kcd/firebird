------------------
Database triggers
------------------

Author:
	Adriano dos Santos Fernandes <adrianosf@uol.com.br>

Syntax:
	<database-trigger> ::=
		{CREATE | RECREATE | CREATE OR ALTER}
			TRIGGER <name>
			[ACTIVE | INACTIVE]
			ON <event>
			[POSITION <n>]
		AS
		BEGIN
			...
		END

	<event> ::=
		  CONNECT
		| DISCONNECT
		| TRANSACTION START
		| TRANSACTION COMMIT
		| TRANSACTION ROLLBACK

Syntax rules:
	1) Database triggers type can't be changed.

Events descriptions:

- CONNECT
	Database connection established
	A transaction is started
	Triggers are fired - uncaught exceptions rollbacks the transaction,
disconnect the attachment and are returned to the client
	The transaction is committed

- DISCONNECT
	A transaction is started
	Triggers are fired - uncaught exceptions rollbacks the transaction,
disconnect the attachment and are swalloed
	The transaction is committed
	The attachment is disconnected

- TRANSACTION START
	Triggers are fired in the newly created transaction - uncaught
exceptions are returned to the client and the transaction is not created

- TRANSACTION COMMIT
	Triggers are fired in the committing transaction - uncaught exceptions
rollbacks the triggers savepoint, the commit command is aborted and the
exception is returned to the client

- TRANSACTION ROLLBACK
	Triggers are fired in the rolling-back transaction - changes done will
be rolled-back and exceptions are swallowed
