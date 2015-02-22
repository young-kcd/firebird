/*
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Vlad Khorsun
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2008 Vlad Khorsun <hvlad@users.sourceforge.net>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "fb_types.h"
#include "../../include/fb_blk.h"

#include "../align.h"
#include "../exe.h"
#include "../jrd.h"
#include "../tra.h"
#include "../common/dsc.h"
#include "../../dsql/dsql.h"
#include "../../dsql/sqlda_pub.h"

#include "../blb_proto.h"
#include "../evl_proto.h"
#include "../exe_proto.h"
#include "../mov_proto.h"
#include "../mov_proto.h"
#include "../PreparedStatement.h"
#include "../Function.h"

#include "InternalDS.h"

using namespace Jrd;
using namespace Firebird;

namespace EDS {

const char* INTERNAL_PROVIDER_NAME = "Internal";

class RegisterInternalProvider
{
public:
	RegisterInternalProvider(MemoryPool&)
	{
		InternalProvider* provider = new InternalProvider(INTERNAL_PROVIDER_NAME);
		Manager::addProvider(provider);
	}
};

static GlobalPtr<RegisterInternalProvider> reg;

// InternalProvider

void InternalProvider::jrdAttachmentEnd(thread_db* tdbb, Jrd::Attachment* att)
{
	if (m_connections.getCount() == 0)
		return;

	Connection** ptr = m_connections.end();
	Connection** begin = m_connections.begin();

	for (ptr--; ptr >= begin; ptr--)
	{
		InternalConnection* conn = (InternalConnection*) *ptr;
		if (conn->getJrdAtt() == att->getInterface())
			releaseConnection(tdbb, *conn, false);
	}
}

void InternalProvider::getRemoteError(const ISC_STATUS* status, string& err) const
{
	err = "";

	char buff[1024];
	const ISC_STATUS* p = status;
	const ISC_STATUS* end = status + ISC_STATUS_LENGTH;

	while (p < end)
	{
		const ISC_STATUS code = *p ? p[1] : 0;
		if (!fb_interpret(buff, sizeof(buff), &p))
			break;

		string rem_err;
		rem_err.printf("%lu : %s\n", code, buff);
		err += rem_err;
	}
}

Connection* InternalProvider::doCreateConnection()
{
	return new InternalConnection(*this);
}


// InternalConnection

InternalConnection::~InternalConnection()
{
}

// Status helper
class IntStatus : public LocalStatus
{
public:
	explicit IntStatus(ISC_STATUS *p)
		: LocalStatus(), v(p)
	{}

	~IntStatus()
	{
		if (v)
			fb_utils::mergeStatus(v, ISC_STATUS_LENGTH, this);
	}

private:
	ISC_STATUS *v;
};

void InternalConnection::attach(thread_db* tdbb, const string& dbName,
		const string& user, const string& pwd,
		const string& role)
{
	fb_assert(!m_attachment);
	Database* dbb = tdbb->getDatabase();
	fb_assert(dbName.isEmpty() || dbName == dbb->dbb_database_name.c_str());

	// Don't wrap raised errors. This is needed for backward compatibility.
	setWrapErrors(false);

	Jrd::Attachment* attachment = tdbb->getAttachment();
	if ((user.isEmpty() || user == attachment->att_user->usr_user_name) &&
		pwd.isEmpty() &&
		(role.isEmpty() || role == attachment->att_user->usr_sql_role_name))
	{
		m_isCurrent = true;
		m_attachment = attachment->getInterface();
	}
	else
	{
		m_isCurrent = false;
		m_dbName = dbb->dbb_database_name.c_str();
		generateDPB(tdbb, m_dpb, user, pwd, role);

		LocalStatus status;
		CheckStatusWrapper statusWrapper(&status);

		{
			EngineCallbackGuard guard(tdbb, *this, FB_FUNCTION);
			RefPtr<JProvider> jInstance(JProvider::getInstance());
			jInstance->setDbCryptCallback(&statusWrapper, tdbb->getAttachment()->att_crypt_callback);
			m_attachment.assignRefNoIncr(jInstance->attachDatabase(&statusWrapper, m_dbName.c_str(),
				m_dpb.getBufferLength(), m_dpb.getBuffer()));
		}

		if (status.getState() & IStatus::STATE_ERRORS)
			raise(status, tdbb, "JProvider::attach");
	}

	m_sqlDialect = (m_attachment->getHandle()->att_database->dbb_flags & DBB_DB_SQL_dialect_3) ?
					SQL_DIALECT_V6 : SQL_DIALECT_V5;
}

void InternalConnection::doDetach(thread_db* tdbb)
{
	fb_assert(m_attachment);

	if (m_isCurrent)
	{
		m_attachment = NULL;
	}
	else
	{
		LocalStatus status;
		CheckStatusWrapper statusWrapper(&status);

		RefPtr<JAttachment> att = m_attachment;
		m_attachment = NULL;

		{	// scope
			EngineCallbackGuard guard(tdbb, *this, FB_FUNCTION);
			att->detach(&statusWrapper);
		}

		if (status.getErrors()[1] == isc_att_shutdown)
		{
			status.init();
		}

		if (status.getState() & IStatus::STATE_ERRORS)
		{
			m_attachment = att;
			raise(status, tdbb, "JAttachment::detach");
		}
	}

	fb_assert(!m_attachment);
}

bool InternalConnection::cancelExecution()
{
	if (m_isCurrent)
		return true;

	LocalStatus status;
	CheckStatusWrapper statusWrapper(&status);

	m_attachment->cancelOperation(&statusWrapper, fb_cancel_raise);
	return !(status.getState() & IStatus::STATE_ERRORS);
}

// this internal connection instance is available for the current execution context if it
// a) is current connection and current thread's attachment is equal to
//	  this attachment, or
// b) is not current connection
bool InternalConnection::isAvailable(thread_db* tdbb, TraScope /*traScope*/) const
{
	return !m_isCurrent ||
		(m_isCurrent && (tdbb->getAttachment() == m_attachment->getHandle()));
}

bool InternalConnection::isSameDatabase(thread_db* tdbb, const string& dbName,
		const string& user, const string& pwd,
		const string& role) const
{
	if (m_isCurrent)
	{
		const UserId* attUser = m_attachment->getHandle()->att_user;
		return ((user.isEmpty() || user == attUser->usr_user_name) &&
				pwd.isEmpty() &&
				(role.isEmpty() || role == attUser->usr_sql_role_name));
	}

	return Connection::isSameDatabase(tdbb, dbName, user, pwd, role);
}

Transaction* InternalConnection::doCreateTransaction()
{
	return new InternalTransaction(*this);
}

Statement* InternalConnection::doCreateStatement()
{
	return new InternalStatement(*this);
}

Blob* InternalConnection::createBlob()
{
	return new InternalBlob(*this);
}


// InternalTransaction()

void InternalTransaction::doStart(ISC_STATUS* status, thread_db* tdbb, ClumpletWriter& tpb)
{
	fb_assert(!m_transaction);

	jrd_tra* localTran = tdbb->getTransaction();
	fb_assert(localTran);

	if (m_scope == traCommon && m_IntConnection.isCurrent())
		m_transaction = localTran->getInterface();
	else
	{
		JAttachment* att = m_IntConnection.getJrdAtt();

		EngineCallbackGuard guard(tdbb, *this, FB_FUNCTION);
		IntStatus s(status);
		CheckStatusWrapper statusWrapper(&s);

		m_transaction.assignRefNoIncr(
			att->startTransaction(&statusWrapper, tpb.getBufferLength(), tpb.getBuffer()));

		if (m_transaction)
			m_transaction->getHandle()->tra_callback_count = localTran->tra_callback_count;
	}
}

void InternalTransaction::doPrepare(ISC_STATUS* /*status*/, thread_db* /*tdbb*/,
		int /*info_len*/, const char* /*info*/)
{
	fb_assert(m_transaction);
	fb_assert(false);
}

void InternalTransaction::doCommit(ISC_STATUS* status, thread_db* tdbb, bool retain)
{
	fb_assert(m_transaction);

	if (m_scope == traCommon && m_IntConnection.isCurrent())
	{
		if (!retain) {
			m_transaction = NULL;
		}
	}
	else
	{
		IntStatus s(status);
		CheckStatusWrapper statusWrapper(&s);

		EngineCallbackGuard guard(tdbb, *this, FB_FUNCTION);
		if (retain)
			m_transaction->commitRetaining(&statusWrapper);
		else
		{
			m_transaction->commit(&statusWrapper);
			m_transaction = NULL;
		}
	}
}

void InternalTransaction::doRollback(ISC_STATUS* status, thread_db* tdbb, bool retain)
{
	fb_assert(m_transaction);

	if (m_scope == traCommon && m_IntConnection.isCurrent())
	{
		if (!retain) {
			m_transaction = NULL;
		}
	}
	else
	{
		IntStatus s(status);
		CheckStatusWrapper statusWrapper(&s);

		EngineCallbackGuard guard(tdbb, *this, FB_FUNCTION);
		if (retain)
			m_transaction->rollbackRetaining(&statusWrapper);
		else
		{
			m_transaction->rollback(&statusWrapper);
			m_transaction = NULL;
		}
	}

	if (status[1] == isc_att_shutdown && !retain)
	{
		m_transaction = NULL;
		fb_utils::init_status(status);
	}
}


// InternalStatement

InternalStatement::InternalStatement(InternalConnection& conn) :
	Statement(conn),
	m_intConnection(conn),
	m_intTransaction(0),
	m_request(0),
	m_cursor(0),
	m_inMetadata(new MsgMetadata),
	m_outMetadata(new MsgMetadata)
{
}

InternalStatement::~InternalStatement()
{
}

void InternalStatement::doPrepare(thread_db* tdbb, const string& sql)
{
	m_inMetadata->reset();
	m_outMetadata->reset();

	JAttachment* att = m_intConnection.getJrdAtt();
	JTransaction* tran = getIntTransaction()->getJrdTran();

	LocalStatus status;
	CheckStatusWrapper statusWrapper(&status);

	if (m_request)
	{
		doClose(tdbb, true);
		fb_assert(!m_allocated);
	}

	{
		EngineCallbackGuard guard(tdbb, *this, FB_FUNCTION);

		CallerName save_caller_name(tran->getHandle()->tra_caller_name);

		if (m_callerPrivileges)
		{
			jrd_req* request = tdbb->getRequest();
			JrdStatement* statement = request ? request->getStatement() : NULL;
			CallerName callerName;
			const Routine* routine;

			if (statement && statement->parentStatement)
				statement = statement->parentStatement;

			if (statement && statement->triggerName.hasData())
				tran->getHandle()->tra_caller_name = CallerName(obj_trigger, statement->triggerName);
			else if (statement && (routine = statement->getRoutine()) &&
				routine->getName().identifier.hasData())
			{
				if (routine->getName().package.isEmpty())
				{
					tran->getHandle()->tra_caller_name = CallerName(routine->getObjectType(),
						routine->getName().identifier);
				}
				else
				{
					tran->getHandle()->tra_caller_name = CallerName(obj_package_header,
						routine->getName().package);
				}
			}
			else
				tran->getHandle()->tra_caller_name = CallerName();
		}

		m_request.assignRefNoIncr(att->prepare(&statusWrapper, tran, sql.length(), sql.c_str(),
			m_connection.getSqlDialect(), 0));
		m_allocated = (m_request != NULL);

		tran->getHandle()->tra_caller_name = save_caller_name;
	}

	if (status.getState() & IStatus::STATE_ERRORS)
		raise(status, tdbb, "JAttachment::prepare", &sql);

	const DsqlCompiledStatement* statement = m_request->getHandle()->getStatement();

	if (statement->getSendMsg())
	{
		try
		{
			PreparedStatement::parseDsqlMessage(statement->getSendMsg(), m_inDescs,
				m_inMetadata, m_in_buffer);
			m_inputs = m_inMetadata->getCount();
		}
		catch (const Exception&)
		{
			raise(tdbb->tdbb_status_vector, tdbb, "parse input message", &sql);
		}
	}
	else
		m_inputs = 0;

	if (statement->getReceiveMsg())
	{
		try
		{
			PreparedStatement::parseDsqlMessage(statement->getReceiveMsg(), m_outDescs,
				m_outMetadata, m_out_buffer);
			m_outputs = m_outMetadata->getCount();
		}
		catch (const Exception&)
		{
			raise(tdbb->tdbb_status_vector, tdbb, "parse output message", &sql);
		}
	}
	else
		m_outputs = 0;

	m_stmt_selectable = false;

	switch (statement->getType())
	{
	case DsqlCompiledStatement::TYPE_SELECT:
	case DsqlCompiledStatement::TYPE_SELECT_UPD:
	case DsqlCompiledStatement::TYPE_SELECT_BLOCK:
		m_stmt_selectable = true;
		break;

	case DsqlCompiledStatement::TYPE_START_TRANS:
	case DsqlCompiledStatement::TYPE_COMMIT:
	case DsqlCompiledStatement::TYPE_ROLLBACK:
	case DsqlCompiledStatement::TYPE_COMMIT_RETAIN:
	case DsqlCompiledStatement::TYPE_ROLLBACK_RETAIN:
	case DsqlCompiledStatement::TYPE_CREATE_DB:
		Arg::Gds(isc_eds_expl_tran_ctrl).copyTo(&statusWrapper);
		raise(status, tdbb, "JAttachment::prepare", &sql);
		break;

	case DsqlCompiledStatement::TYPE_INSERT:
	case DsqlCompiledStatement::TYPE_DELETE:
	case DsqlCompiledStatement::TYPE_UPDATE:
	case DsqlCompiledStatement::TYPE_UPDATE_CURSOR:
	case DsqlCompiledStatement::TYPE_DELETE_CURSOR:
	case DsqlCompiledStatement::TYPE_DDL:
	case DsqlCompiledStatement::TYPE_EXEC_PROCEDURE:
	case DsqlCompiledStatement::TYPE_SET_GENERATOR:
	case DsqlCompiledStatement::TYPE_SAVEPOINT:
	case DsqlCompiledStatement::TYPE_EXEC_BLOCK:
		break;
	}
}


void InternalStatement::doExecute(thread_db* tdbb)
{
	JTransaction* transaction = getIntTransaction()->getJrdTran();

	LocalStatus status;
	CheckStatusWrapper statusWrapper(&status);

	{
		EngineCallbackGuard guard(tdbb, *this, FB_FUNCTION);

		fb_assert(m_inMetadata->getMessageLength() == m_in_buffer.getCount());
		fb_assert(m_outMetadata->getMessageLength() == m_out_buffer.getCount());

		m_request->execute(&statusWrapper, transaction,
			m_inMetadata, m_in_buffer.begin(), m_outMetadata, m_out_buffer.begin());
	}

	if (status.getState() & IStatus::STATE_ERRORS)
		raise(status, tdbb, "JStatement::execute");
}


void InternalStatement::doOpen(thread_db* tdbb)
{
	JTransaction* transaction = getIntTransaction()->getJrdTran();

	LocalStatus status;
	CheckStatusWrapper statusWrapper(&status);

	{
		EngineCallbackGuard guard(tdbb, *this, FB_FUNCTION);

		if (m_cursor)
		{
			m_cursor->close(&statusWrapper);
			m_cursor = NULL;
		}

		fb_assert(m_inMetadata->getMessageLength() == m_in_buffer.getCount());

		m_cursor.assignRefNoIncr(m_request->openCursor(&statusWrapper, transaction,
			m_inMetadata, m_in_buffer.begin(), m_outMetadata, 0));
	}

	if (status.getState() & IStatus::STATE_ERRORS)
		raise(status, tdbb, "JStatement::open");
}


bool InternalStatement::doFetch(thread_db* tdbb)
{
	LocalStatus status;
	CheckStatusWrapper statusWrapper(&status);

	bool res = true;

	{
		EngineCallbackGuard guard(tdbb, *this, FB_FUNCTION);

		fb_assert(m_outMetadata->getMessageLength() == m_out_buffer.getCount());
		fb_assert(m_cursor);
		res = m_cursor->fetchNext(&statusWrapper, m_out_buffer.begin()) == IStatus::RESULT_OK;
	}

	if (status.getState() & IStatus::STATE_ERRORS)
		raise(status, tdbb, "JResultSet::fetchNext");

	return res;
}


void InternalStatement::doClose(thread_db* tdbb, bool drop)
{
	LocalStatus status;
	CheckStatusWrapper statusWrapper(&status);

	{
		EngineCallbackGuard guard(tdbb, *this, FB_FUNCTION);

		if (m_cursor)
			m_cursor->close(&statusWrapper);

		m_cursor = NULL;
		if (status.getState() & IStatus::STATE_ERRORS)
		{
			raise(status, tdbb, "JResultSet::close");
		}

		if (drop)
		{
			if (m_request)
				m_request->free(&statusWrapper);

			m_allocated = false;
			m_request = NULL;

			if (status.getState() & IStatus::STATE_ERRORS)
			{
				raise(status, tdbb, "JStatement::free");
			}
		}
	}
}

void InternalStatement::putExtBlob(thread_db* tdbb, dsc& src, dsc& dst)
{
	if (m_transaction->getScope() == traCommon)
		MOV_move(tdbb, &src, &dst);
	else
		Statement::putExtBlob(tdbb, src, dst);
}

void InternalStatement::getExtBlob(thread_db* tdbb, const dsc& src, dsc& dst)
{
	fb_assert(dst.dsc_length == src.dsc_length);
	fb_assert(dst.dsc_length == sizeof(bid));

	if (m_transaction->getScope() == traCommon)
		memcpy(dst.dsc_address, src.dsc_address, sizeof(bid));
	else
		Statement::getExtBlob(tdbb, src, dst);
}



// InternalBlob

InternalBlob::InternalBlob(InternalConnection& conn) :
	Blob(conn),
	m_connection(conn),
	m_blob(NULL)
{
	memset(&m_blob_id, 0, sizeof(m_blob_id));
}

InternalBlob::~InternalBlob()
{
	fb_assert(!m_blob);
}

void InternalBlob::open(thread_db* tdbb, Transaction& tran, const dsc& desc, const UCharBuffer* bpb)
{
	fb_assert(!m_blob);
	fb_assert(sizeof(m_blob_id) == desc.dsc_length);

	JAttachment* att = m_connection.getJrdAtt();
	JTransaction* transaction = static_cast<InternalTransaction&>(tran).getJrdTran();
	memcpy(&m_blob_id, desc.dsc_address, sizeof(m_blob_id));

	LocalStatus status;
	CheckStatusWrapper statusWrapper(&status);

	{
		EngineCallbackGuard guard(tdbb, m_connection, FB_FUNCTION);

		USHORT bpb_len = bpb ? bpb->getCount() : 0;
		const UCHAR* bpb_buff = bpb ? bpb->begin() : NULL;

		m_blob.assignRefNoIncr(
			att->openBlob(&statusWrapper, transaction, &m_blob_id, bpb_len, bpb_buff));
	}

	if (status.getState() & IStatus::STATE_ERRORS)
		m_connection.raise(status, tdbb, "JAttachment::openBlob");

	fb_assert(m_blob);
}

void InternalBlob::create(thread_db* tdbb, Transaction& tran, dsc& desc, const UCharBuffer* bpb)
{
	fb_assert(!m_blob);
	fb_assert(sizeof(m_blob_id) == desc.dsc_length);

	JAttachment* att = m_connection.getJrdAtt();
	JTransaction* transaction = ((InternalTransaction&) tran).getJrdTran();
	memset(&m_blob_id, 0, sizeof(m_blob_id));

	LocalStatus status;
	CheckStatusWrapper statusWrapper(&status);

	{
		EngineCallbackGuard guard(tdbb, m_connection, FB_FUNCTION);

		const USHORT bpb_len = bpb ? bpb->getCount() : 0;
		const UCHAR* bpb_buff = bpb ? bpb->begin() : NULL;

		m_blob.assignRefNoIncr(
			att->createBlob(&statusWrapper, transaction, &m_blob_id, bpb_len, bpb_buff));
	}

	if (status.getState() & IStatus::STATE_ERRORS)
		m_connection.raise(status, tdbb, "JAttachment::createBlob");

	fb_assert(m_blob);
	memcpy(desc.dsc_address, &m_blob_id, sizeof(m_blob_id));
}

USHORT InternalBlob::read(thread_db* tdbb, UCHAR* buff, USHORT len)
{
	fb_assert(m_blob);

	unsigned result = 0;
	LocalStatus status;
	CheckStatusWrapper statusWrapper(&status);

	{
		EngineCallbackGuard guard(tdbb, m_connection, FB_FUNCTION);
		m_blob->getSegment(&statusWrapper, len, buff, &result);
	}

	if (status.getState() & IStatus::STATE_ERRORS)
		m_connection.raise(status, tdbb, "JBlob::getSegment");

	return result;
}

void InternalBlob::write(thread_db* tdbb, const UCHAR* buff, USHORT len)
{
	fb_assert(m_blob);

	LocalStatus status;
	CheckStatusWrapper statusWrapper(&status);

	{
		EngineCallbackGuard guard(tdbb, m_connection, FB_FUNCTION);
		m_blob->putSegment(&statusWrapper, len, buff);
	}

	if (status.getState() & IStatus::STATE_ERRORS)
		m_connection.raise(status, tdbb, "JBlob::putSegment");
}

void InternalBlob::close(thread_db* tdbb)
{
	fb_assert(m_blob);
	LocalStatus status;
	CheckStatusWrapper statusWrapper(&status);

	{
		EngineCallbackGuard guard(tdbb, m_connection, FB_FUNCTION);
		m_blob->close(&statusWrapper);
		m_blob = NULL;
	}

	if (status.getState() & IStatus::STATE_ERRORS)
		m_connection.raise(status, tdbb, "JBlob::close");

	fb_assert(!m_blob);
}

void InternalBlob::cancel(thread_db* tdbb)
{
	if (!m_blob) {
		return;
	}

	LocalStatus status;
	CheckStatusWrapper statusWrapper(&status);

	{
		EngineCallbackGuard guard(tdbb, m_connection, FB_FUNCTION);
		m_blob->cancel(&statusWrapper);
		m_blob = NULL;
	}

	if (status.getState() & IStatus::STATE_ERRORS)
		m_connection.raise(status, tdbb, "JBlob::cancel");

	fb_assert(!m_blob);
}


} // namespace EDS
