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
 *  $Id: evl_string_test.cpp,v 1.4 2003-12-27 04:37:23 skidder Exp $
 *
 */

#include "../common/classes/alloc.h"
#include <assert.h>

const isc_like_escape_invalid = 1;

void ERR_post(...) {
	throw std::exception();
}

#include "evl_string.h"

using namespace Firebird;

class StringLikeEvaluator : public LikeEvaluator<char> {
public:
	StringLikeEvaluator(MemoryPool *pool, const char *pattern, char escape_char) : 
	  LikeEvaluator<char>(pool, pattern, (SSHORT)strlen(pattern), escape_char, '%', '_') {}

	void process(const char *data, bool more, bool result) {
		SSHORT len = (SSHORT)strlen(data);
		if (len) {
			bool needMore = processNextChunk(data, len);
			assert(more == needMore);
		}
		assert(getResult() == result);
	}
};

class StringStartsEvaluator : public StartsEvaluator<char> {
public:
	StringStartsEvaluator(const char *pattern) : 
	  StartsEvaluator<char>(pattern, (SSHORT)strlen(pattern)) {}

	void process(const char *data, bool more, bool result) {
		SSHORT len = (SSHORT)strlen(data);
		if (len) {
			bool needMore = processNextChunk(data, len);
			assert(more == needMore);
		}
		assert(getResult() == result);
	}
};

class StringContainsEvaluator : public ContainsEvaluator<char> {
public:
	StringContainsEvaluator(MemoryPool *pool, const char *pattern) : 
	  ContainsEvaluator<char>(pool, pattern, (SSHORT)strlen(pattern)) {}

	void process(const char *data, bool more, bool result) {
		SSHORT len = (SSHORT)strlen(data);
		if (len) {
			bool needMore = processNextChunk(data, len);
			assert(more == needMore);
		}
		assert(getResult() == result);
	}
};

int main() {
	MemoryPool *p = MemoryPool::createPool();

	// The tests below attempt to do full code coverage for the LikeEvaluator
	// Every line of evl_string.h code is covered by the tests

	// '' LIKE ''
	StringLikeEvaluator t1(p, "", 0);
	t1.process("", true, true);
	t1.process("something", false, false);

	// 'test' LIKE 'test'
	StringLikeEvaluator t2(p, "test", 0);
	t2.process("test", true, true);
	t2.process("a", false, false);

	// '_%test' LIKE 'test'
	StringLikeEvaluator t3(p, "_%test", 0);
	t3.process("test", true, false);
	t3.reset();
	t3.process("ntest", true, true);
	t3.process("yep?", true, false);

	// Tests for escaped patterns
	StringLikeEvaluator t4(p, "\\%\\_%some_text_", '\\');
	t4.process("%_(is it nice?)some!text?", true, true);
	t4.reset();
	t4.process("%_some text", true, false);
	t4.process(".", true, true);
	
	// More escaped patterns
	StringLikeEvaluator t5(p, "%sosome_\\%text%", '\\');
	t5.process("sosomso", true, false);
	t5.process("sosome.%text", false, true);

	// More escaped patterns
	StringLikeEvaluator t6(p, "%sosome_text\\%%", '\\');
	t6.process("sosomso", true, false);
	t6.process("sosome.text%", false, true);

	// Check for invalid escapes
	try {
		StringLikeEvaluator t7(p, "%sosome_text\\?", '\\');
		assert(false);
	} catch (const std::exception&) {
	}

	// Test single '%' pattern
	StringLikeEvaluator t8(p, "%%%%%", 0);
	t8.process("something", false, true);
	t8.process("something else", false, true);

	// Test optimization of consequitive "_" and "%" subpatterns
	StringLikeEvaluator t9(p, "%__%%_A", 0);
	t9.process("ABAB", true, false);
	t9.process("ABA", true, true);

	// Check multi-branch search
	StringLikeEvaluator t10(p, "%test____test%", 0);
	t10.process("test1234", true, false);
	t10.process("test...", false, true);
	t10.reset();
	t10.process("testestestestetest", false, true);

	// Check simple matching
	StringLikeEvaluator t11(p, "test%", 0);
	t11.process("test11", false, true);
	t11.reset();
	t11.process("nop", false, false);

	// Check skip counting
	StringLikeEvaluator t12(p, "test___", 0);
	t12.process("test1", true, false);
    t12.process("23", true, true);
	t12.process("45", false, false);
	t12.reset();
	t12.process("test1234", false, false);

    // Check simple search
	StringLikeEvaluator t13(p, "%test%", 0);
	t13.process("1234tetes", true, false);
    t13.process("t", false, true);

	// Test STARTS
	StringStartsEvaluator t14("test");
	t14.process("test", false, true);
	t14.reset();
	t14.process("te!", false, false);
	t14.reset();
	t14.process("te", true, false);
	t14.process("st!", false, true);

	// Test CONTAINS
	StringContainsEvaluator t15(p, "test");
	t15.process("123test456", false, true);
	t15.reset();
	t15.process("1234tetes", true, false);
    t15.process("t", false, true);

	// Test pattern that caused endless loop during initial tests
	StringLikeEvaluator t16(p, "%a%a%a%a%a%Toronto%a", 0);
	t16.process("Each day hosts Anthony Regan and Natalie Hunter bring you a lively hour of information and fun specifically for Toronto viewers.\n"
		"1.Author Pierre Burton on his new book \"Cats I Have Known and Loved.\"\n"
		"2.  Culinary expert Rose Reisman makes something scrumptious from her new book \"The Art of Living Well.\"\n"
		"3.  All about the Universal Influenza Vaccination.\n"
		"4. Painting with flare and style...tips, dos, and don'ts from an expert at PARA Paints.\n"
		"5.  The facts on zoo animal diets.", true, false);

	return 0;
}