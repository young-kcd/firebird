/*
 *	PROGRAM:		JRD Access Method
 *	MODULE:			evl_string.h
 *	DESCRIPTION:	Tests for streamed string functions
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * You may obtain a copy of the Licence at
 * http://www.gnu.org/licences/lgpl.html
 * 
 * As a special exception this file can also be included in modules
 * with other source code as long as that source code has been 
 * released under an Open Source Initiative certificed licence.  
 * More information about OSI certification can be found at: 
 * http://www.opensource.org 
 * 
 * This module is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public Licence for more details.
 * 
 * This module was created by members of the firebird development 
 * team.  All individual contributions remain the Copyright (C) of 
 * those individuals and all rights are reserved.  Contributors to 
 * this file are either listed below or can be obtained from a CVS 
 * history command.
 *
 *  Created by: Nickolay Samofatov <skidder@bssys.com>
 *
 *  Contributor(s):
 * 
 *
 *  $Id: evl_string_test.cpp,v 1.1 2003-11-16 22:10:26 skidder Exp $
 *
 */

#include "../common/classes/alloc.h"

const gds_like_escape_invalid = 1;

void ERR_post(...) {
	abort();
}

#include "evl_string.h"

using namespace Firebird;

class StringLikeEvaluator : public LikeEvaluator<char> {
public:
	StringLikeEvaluator(MemoryPool *pool) : LikeEvaluator<char>(pool) {}
	void prepare(const char *pattern, char escape_char) {
		LikeEvaluator<char>::prepare(pattern, (SSHORT)strlen(pattern), escape_char, '%', '_');
	}
	void process(const char *data, bool more, bool result) {
		SSHORT len = (SSHORT)strlen(data);
		if (len) {
			bool needMore = LikeEvaluator<char>::processNextChunk(data, len);
			assert(more == needMore);
		}
		assert(getResult() == result);
	}
};

int main() {
	MemoryPool *testPool = MemoryPool::createPool();
	StringLikeEvaluator t(testPool);

	// The tests below attempt to do full code coverage for the LikeEvaluator
	// Not finished, only 92,89% of evl_string.h is covered

	// '' LIKE ''
	t.prepare("", 0);
	t.process("", true, true);
	t.process("something", false, false);

	// 'test' LIKE 'test'
	t.prepare("test", 0);
	t.process("test", true, true);
	t.process("a", false, false);

	// '_%test' LIKE 'test'
	t.prepare("_%test", 0);
	t.process("test", true, false);
	t.reset();
	t.process("ntest", true, true);
	t.process("yep?", true, false);

	// Tests for escaped patterns
	t.prepare("\\%\\_%some_text_", '\\');
	t.process("%_(is it nice?)some!text?", true, true);
	t.reset();
	t.process("%_some text", true, false);
	t.process(".", true, true);
	
	// More escaped patterns
	t.prepare("%sosome_\\%text%", '\\');
	t.process("sosomso", true, false);
	t.process("sosome.%text", false, true);
	//MemoryPool::deletePool(testPool);
	return 0;
}