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
 *  The Original Code was created by Adriano dos Santos Fernandes
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2009 Adriano dos Santos Fernandes <adrianosf@uol.com.br>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#ifndef JRD_AGGREGATE_RSB_H
#define JRD_AGGREGATE_RSB_H

#include "../jrd/VirtualTable.h"

namespace Jrd {


class AggregateRsb : public RecordStream
{
private:
	AggregateRsb(RecordSource* aRsb, jrd_nod* aAggNode);

public:
	static RecordSource* create(thread_db* tdbb, OptimizerBlk* opt, jrd_nod* node,
		NodeStack& deliverStack, RecordSelExpr* rse);

public:
	virtual void findRsbs(StreamStack* streamList, RsbStack* rsbList);
	virtual void invalidate(thread_db* tdbb, record_param* rpb);

	virtual unsigned dump(UCHAR* buffer, unsigned bufferLen);
	virtual void open(thread_db* tdbb, jrd_req* request);
	virtual void close(thread_db* tdbb);
	virtual bool get(thread_db* tdbb, jrd_req* request);
	virtual void markRecursive();

private:
	USHORT evlGroup(thread_db* tdbb, jrd_req* request, USHORT state);

	void initDistinct(jrd_req* request, const jrd_nod* node);
	void computeDistinct(thread_db* tdbb, jrd_req* request, jrd_nod* node);
	void finiDistinct(jrd_req* request, const jrd_nod* const node);

private:
	jrd_nod* aggNode;
	RecordSource* next;
};


} // namespace Jrd

#endif // JRD_AGGREGATE_RSB_H
