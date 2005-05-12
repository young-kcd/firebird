/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		sym.h
 *	DESCRIPTION:	Header file for sym.cpp
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
 * The Original Code was created by Inprise Corporation
 * and its predecessors. Portions created by Inprise Corporation are
 * Copyright (C) Inprise Corporation.
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 */

#ifndef JRD_SYM_H
#define JRD_SYM_H

#include "../common/classes/MetaName.h"

namespace Jrd {
/* symbol definitions */

class Symbol : public pool_alloc<type_sym>
{
private:	
	Symbol*	sym_collision;	/* collision pointer */

public:
	enum sym_t {
		rel,					/* relation block */
		fld,					/* field block */
		fun,					/* UDF function block */
		prc,					/* stored procedure block */
		sql,					/* SQL request cache block */
		blr,					/* BLR request cache block */
		label					/* CVC: I need to track labels if LEAVE is implemented. */
	};
	typedef sym_t SYM_T;
	Firebird::MetaName	sym_string;	// symbol value
	SYM_T	sym_type;		/* symbol type */
	BLK		sym_object;		/* general pointer to object */
	Symbol*	sym_homonym;	/* homonym pointer */

public:
	explicit Symbol(MemoryPool& p, const Firebird::MetaName& val,
					SYM_T type, BLK object) 
		: sym_collision(0), sym_string(p, val), sym_type(type), 
		sym_object(object),	sym_homonym(0) { }
	void insert();
	static Symbol* lookup(const Firebird::MetaName&);
	void remove();
};

} //namespace Jrd


#endif // JRD_SYM_H

