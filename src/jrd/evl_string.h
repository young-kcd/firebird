/*
 *	PROGRAM:		JRD Access Method
 *	MODULE:			evl_string.h
 *	DESCRIPTION:	Algorithms for streamed string functions
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
 *  $Id: evl_string.h,v 1.7 2004-02-20 06:42:59 robocop Exp $
 *
 */

#ifndef EVL_STRING_H
#define EVL_STRING_H

#include "../common/classes/alloc.h"
#include "../common/classes/array.h"

// Number of pattern items statically allocated
#define STATIC_PATTERN_ITEMS 16

// Number of pattern items that are matched in parallel statically allocated
#define STATIC_STATUS_ITEMS 16

// Size of internal static buffer used for allocation
// This buffer is used for KMP masks and string unescaping
#ifdef TESTING_ONLY
#define STATIC_PATTERN_BUFFER 16
#else
#define STATIC_PATTERN_BUFFER 256
#endif

namespace Firebird {

template <typename CharType>
class StartsEvaluator {
public:
	StartsEvaluator(const CharType* _pattern_str, SSHORT _pattern_len) : 
		// No need to copy string because this class is used briefly
		pattern_str(_pattern_str), pattern_len(_pattern_len)
	{
		reset();
	}
	void reset() {
		result = true;
		offset = 0;
	}
	bool getResult() {
		return offset >= pattern_len && result;
	}
	bool processNextChunk(const CharType* data, SSHORT data_len) {
		fb_assert(data_len);
		if (!result || offset >= pattern_len) return false;
		SSHORT comp_length = data_len < pattern_len - offset ? data_len : pattern_len-offset;
		if (memcmp(data, pattern_str + offset, sizeof(CharType)*comp_length)!=0) {
			result = false;
			return false;
		}
		offset += comp_length;
		return offset < pattern_len;
	}
private:
	SSHORT offset;
	const CharType* pattern_str;
	SSHORT pattern_len;
	bool result;
};

template <typename CharType>
static void preKmp(const CharType *x, int m, SSHORT kmpNext[]) {
   SSHORT i, j;

   i = 0;
   j = kmpNext[0] = -1;
   while (i < m) {
      while (j > -1 && x[i] != x[j])
         j = kmpNext[j];
      i++;
      j++;
      if (x[i] == x[j])
         kmpNext[i] = kmpNext[j];
      else
         kmpNext[i] = j;
   }
}

class StaticAllocator {
public:
	StaticAllocator(MemoryPool* _pool) : chunksToFree(_pool), pool(_pool), allocated(0) {};

	~StaticAllocator() {
		for (int i=0; i < chunksToFree.getCount(); i++)
			pool->deallocate(chunksToFree[i]);
	}

	void* alloc(SSHORT count) {
		void* result;
		if (allocated + count <= STATIC_PATTERN_BUFFER) {
			result = allocBuffer + allocated;
			allocated += count;		
			return result;
		}
		else {
			result = pool->allocate(count);
			chunksToFree.add(result);
			return result;
		}
	}
private:
	Array<void*> chunksToFree;
	MemoryPool* pool;
	char allocBuffer[STATIC_PATTERN_BUFFER];
	int allocated;
};

template <typename CharType>
class ContainsEvaluator : private StaticAllocator {
public:
	ContainsEvaluator(MemoryPool* _pool, const CharType* _pattern_str, SSHORT _pattern_len) : 
		StaticAllocator(_pool),	pattern_len(_pattern_len)
	{
		CharType* temp = reinterpret_cast<CharType*>(alloc(_pattern_len*sizeof(CharType)));
		memcpy(temp, _pattern_str, _pattern_len);
		pattern_str = temp;
		kmpNext = reinterpret_cast<SSHORT*>(alloc((_pattern_len+1)*sizeof(SSHORT)));
		preKmp<CharType>(_pattern_str, _pattern_len, kmpNext);
		reset();
	}

	void reset() {
		offset = 0;
		result = false;
	}

	bool getResult() {
		return result;
	}

	bool processNextChunk(const CharType* data, SSHORT data_len) {		
		fb_assert(data_len);
		if (result) return false;
		SSHORT data_pos = 0;
		while (data_pos < data_len) {
			while (offset > -1 && pattern_str[offset] != data[data_pos])
				offset = kmpNext[offset];
			offset++;
			data_pos++;
			if (offset >= pattern_len) {
				result = true;
				return false;
			}
		}
		return true;
	}

private:
	const CharType* pattern_str;
	SSHORT pattern_len;
	SSHORT offset;
	bool result;
	SSHORT *kmpNext;
};

enum PatternItemType {
	piNone = 0,
	piSearch,
	piSkipFixed,
	piDirectMatch,
	// Used only during compilation phase to indicate that string is pending cleanup
	piEscapedString,
	// Used to optimize subpatterns like "%_%____%_", not used during matching phase
	piSkipMore
};

enum MatchType {
	MATCH_NONE = 0,
	MATCH_FIXED,
	MATCH_ANY
};

template <typename CharType>
class LikeEvaluator : private StaticAllocator {
public:
	LikeEvaluator(MemoryPool* _pool, const CharType* _pattern_str, 
		SSHORT pattern_len, CharType escape_char, CharType sql_match_any, 
		CharType sql_match_one);

	void reset() {
		fb_assert(patternItems.getCount());
		branches.shrink(0);
		if (patternItems[0].type == piNone) {
			match_type = (patternItems[0].match_any ? MATCH_ANY : MATCH_FIXED);
		}
		else {
			BranchItem temp = {&patternItems[0], 0};
			branches.add(temp);
			match_type = MATCH_NONE;
		}
	}

	bool getResult() {
		return match_type != MATCH_NONE;
	}

	// Returns true if more data can change the result of evaluation
	bool processNextChunk(const CharType* data, SSHORT data_len);

private:
	struct PatternItem {
		PatternItemType type;
		union {
			struct {
				SSHORT length;
				CharType* data;
				SSHORT* kmpNext; // Jump table for Knuth-Morris-Pratt algorithm
			} str;
			SSHORT skipCount;
		};
		bool match_any;
	};

	struct BranchItem {
		PatternItem* pattern;
		SSHORT offset; // Match offset inside this pattern
	};

	HalfStaticArray<PatternItem, STATIC_PATTERN_ITEMS> patternItems;
	HalfStaticArray<BranchItem, STATIC_STATUS_ITEMS> branches;

	MatchType match_type;
};

template <typename CharType>
LikeEvaluator<CharType>::LikeEvaluator(
	MemoryPool* _pool, const CharType* _pattern_str, SSHORT pattern_len, 
	CharType escape_char, CharType sql_match_any, CharType sql_match_one)
: StaticAllocator(_pool), patternItems(_pool), branches(_pool), match_type(MATCH_NONE)
{
	// Create local copy of the string.
	CharType* pattern_str = reinterpret_cast<CharType*>(alloc(pattern_len*sizeof(CharType)));
	memcpy(pattern_str, _pattern_str, pattern_len);

	patternItems.grow(1);
	// PASS1. Parse pattern.
	SSHORT pattern_pos = 0;
	PatternItem *item = patternItems.begin();
	while (pattern_pos < pattern_len) {
		CharType c = pattern_str[pattern_pos++];
		// Escaped symbol
		if (escape_char && c == escape_char) {
			if (pattern_pos < pattern_len) {
				c = pattern_str[pattern_pos++];
				/* Note: SQL II says <escape_char><escape_char> is error condition */
				if (c == escape_char ||	c == sql_match_any || c == sql_match_one)
				{
					switch (item->type) {
					case piSkipFixed:
					case piSkipMore:
						patternItems.grow(patternItems.getCount()+1);
						item = patternItems.end()-1;
						// Note: fall into
					case piNone:
						item->type = piEscapedString;
						item->str.data = const_cast<CharType*>(pattern_str + pattern_pos - 2);
						item->str.length = 1;
						break;
					case piSearch:
						item->type = piEscapedString;
						// Note: fall into
					case piEscapedString:
						item->str.length++;
						break;
					}
					continue;
				}
			}
			ERR_post(isc_like_escape_invalid, 0);
		}
		// percent sign
		if (c == sql_match_any) {
			switch (item->type) {
			case piSearch:
			case piEscapedString:
				patternItems.grow(patternItems.getCount()+1);
				item = patternItems.end()-1;
				// Note: fall into
			case piSkipFixed:
			case piNone:
				item->type = piSkipMore;
				break;
			}
			continue;
		}
		// underscore
		if (c == sql_match_one) {
			switch (item->type) {
			case piSearch:
			case piEscapedString:
				patternItems.grow(patternItems.getCount()+1);
				item = patternItems.end()-1;
				// Note: fall into
			case piNone:
				item->type = piSkipFixed;
				item->skipCount = 1;
				break;
			case piSkipFixed:
			case piSkipMore:
				item->skipCount++;
				break;
			}
			continue;
		}
		// anything else			
		switch (item->type) {
		case piSkipFixed:
		case piSkipMore:
			patternItems.grow(patternItems.getCount()+1);
			item = patternItems.end()-1;
			// Note: fall into
		case piNone:
			item->type = piSearch;
			item->str.data = const_cast<CharType*>(pattern_str + pattern_pos - 1);
			item->str.length = 1;
			break;
		case piSearch:
		case piEscapedString:
			item->str.length++;
			break;
		}
	}

	// PASS2. Compilation/Optimization
	// Unescape strings, mark direct match items, pre-compile KMP tables and
	// optimize out piSkipMore nodes
	bool directMatch = true;
	for (int i=0; i < patternItems.getCount();) {
		PatternItem *item = &patternItems[i];
		switch (item->type) {
		case piEscapedString: {
			const CharType *curPos = item->str.data;
			item->str.data = 
				reinterpret_cast<CharType*>(alloc(item->str.length*sizeof(CharType)));
			for (SSHORT j=0; j < item->str.length; j++) {
				if (*curPos == escape_char) 
					curPos++;
				item->str.data[j] = *curPos++;
			}
			item->type = piSearch;			
			// Note: fall into
			}
		case piSearch:
			if (directMatch)
				item->type = piDirectMatch;
			else {
				item->str.kmpNext = 
					reinterpret_cast<SSHORT*>(alloc((item->str.length+1)*sizeof(SSHORT)));
				preKmp<CharType>(item->str.data, item->str.length, item->str.kmpNext);
				directMatch = true;
			}
			break;
		case piSkipMore:
			// Optimize out piSkipMore
			directMatch = false;
			if (item->skipCount != 0) {
				// Convert this node to SkipFixed if possible
				item->type = piSkipFixed;
				item->match_any = true;
			}
			else {
				if (i > 0) {
					// Mark previous node if it exists
					patternItems[i-1].match_any = true;
					patternItems.remove(i);
					continue;
				}
				// Remove node if we have other nodes
				if (patternItems.getCount() != 1) {
					patternItems.remove(i);
					continue;
				}
				// Our pattern is single %
				item->type = piNone;
				item->match_any = true;
			}
			break;
		}
		i++;
	}

	// Get ready for parsing
	reset();
}

template <typename CharType>
bool LikeEvaluator<CharType>::processNextChunk(const CharType* data, SSHORT data_len) {
	fb_assert(data_len);
	fb_assert(patternItems.getCount());

	if (match_type == MATCH_FIXED)
		match_type = MATCH_NONE;

	if (branches.getCount() == 0)
		return false;

	SSHORT data_pos = 0;	
	SSHORT finishCandidate = -1;
	while (data_pos < data_len) {

		int branch_number = 0;
		while (branch_number < branches.getCount()) 
		{
			BranchItem *current_branch = &branches[branch_number];
			PatternItem *current_pattern = current_branch->pattern;
			switch (current_pattern->type) {
			case piDirectMatch:
				if (data[data_pos] != current_pattern->str.data[current_branch->offset]) {
					// Terminate matching branch
					branches.remove(branch_number);
					if (branches.getCount() == 0)
						return false;
					continue;
				}
				// Note: fall into
			case piSkipFixed:
				current_branch->offset++;				
				if (current_branch->offset >= current_pattern->str.length) {
					// Switch to next subpattern or finish matching
					if (current_pattern->match_any) {
						current_pattern++;
						if (current_pattern >= patternItems.end()) {
							branches.shrink(0);
							match_type = MATCH_ANY;
							return false;
						}
						branches.shrink(1);
						branches[0].pattern = current_pattern;
						branches[0].offset = 0;
						branch_number = 0;
						break;
					}
					current_pattern++;
					if (current_pattern >= patternItems.end()) {
						finishCandidate = data_pos;
						branches.remove(branch_number);
						if (branches.getCount() == 0) {
							if (data_pos == data_len-1) {
								match_type = MATCH_FIXED;
								return true;
							}
							return false;
						}
						continue;
					}
					current_branch->pattern = current_pattern;
					current_branch->offset = 0;
				}
				break;
			case piSearch:
				// Knuth-Morris-Pratt search algorithm
				while (current_branch->offset >= 0 && 
					   current_pattern->str.data[current_branch->offset] != data[data_pos])
				{
			    	current_branch->offset = current_pattern->str.kmpNext[current_branch->offset];
				}
				current_branch->offset++;
				if (current_branch->offset >= current_pattern->str.length) {
					PatternItem *next_pattern = current_pattern + 1;
					if (next_pattern >= patternItems.end()) {
						if (current_pattern->match_any) {
							branches.shrink(0);
							match_type = MATCH_ANY;
							return false;
						}
						// We are looking for the pattern at the end of string
						current_branch->offset = current_pattern->str.kmpNext[current_branch->offset];
						finishCandidate = data_pos;
					}
					else {
						if (next_pattern->type == piSearch) {
							// Search for the next pattern
							current_branch->pattern = next_pattern;
							current_branch->offset = 0;
						}
						else {
							// Try to apply further non-search patterns and continue searching
							current_branch->offset = current_pattern->str.kmpNext[current_branch->offset];
							BranchItem temp = {next_pattern, 0};
							branches.insert(branch_number+1, temp); // +1 is to reduce movement effort :)
							branch_number++; // Skip newly inserted branch in this cycle
						}
					}
				}
				break;
			}
			branch_number++;
		}
		data_pos++;
	}
	if (finishCandidate == data_len - 1)
		match_type = MATCH_FIXED;
	return true;
}

}

#endif
