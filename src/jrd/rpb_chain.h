/*
 *	PROGRAM:	Server Code
 *	MODULE:		rpb_chain.h
 *	DESCRIPTION:	Keeps track of rpb's, updated_in_place by
 *	        		single transcation
 *
 * The contents of this file are subject to the Interbase Public
 * License Version 1.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy
 * of the License at http://www.Inprise.com/IPL.html
 *
 * Software distributed under the License is distributed on an
 * "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
 * or implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * Created by: Alex Peshkov <peshkoff@mail.ru>
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 */
 
#ifndef RPB_CHAIN_H
#define RPB_CHAIN_H

#include <assert.h>
#include <string.h>

#include "../common/classes/array.h"
#include "../jrd/jrd.h"
#include "../jrd/req.h"

class traRpbListElement {
public:
	struct rpb *lr_rpb;
	int level;
	traRpbListElement(struct rpb *r, USHORT l) : 
			lr_rpb(r), level(l) {}
	traRpbListElement() {}
	static const inline bool compare(const traRpbListElement& i1, const traRpbListElement& i2) {
		return i1.lr_rpb->rpb_relation->rel_id != i2.lr_rpb->rpb_relation->rel_id ? 
			   i1.lr_rpb->rpb_relation->rel_id > i2.lr_rpb->rpb_relation->rel_id :
			   i1.lr_rpb->rpb_number != i2.lr_rpb->rpb_number ?
			   i1.lr_rpb->rpb_number > i2.lr_rpb->rpb_number : 
			   i1.level > i2.level;
	}
	static const inline traRpbListElement& generate(void *sender, const traRpbListElement& Item) {
		return Item;
	}
};

typedef Firebird::SortedArray<traRpbListElement, traRpbListElement, 
			traRpbListElement, traRpbListElement> traRpbArray;
class traRpbList : public traRpbArray {
public:
	traRpbList(Firebird::MemoryPool *p) : 
		traRpbArray(p, 16) {}
	int PushRpb(struct rpb* value);
	bool PopRpb(struct rpb* value, int Level);
};
#endif	//RPB_CHAIN_H
