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
 *  The Original Code was created by Dmitry Yemanov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2002 Dmitry Yemanov <dimitr@users.sf.net>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#ifndef SORT_MEM_H
#define SORT_MEM_H

#include "../jrd/sort.h"

/*
 *	Virtual scratch file
 */

class SortMem {
private:

	// Generic storage block
	class Block {

		friend class SortMem;

	protected:
		Block *prev;
		Block *next;
		size_t size;

	public:
		// Read bytes from the block into the given buffer
		virtual size_t read(ISC_STATUS*, size_t, char*, size_t) = 0;
		// Write bytes from the given buffer into the block
		virtual size_t write(ISC_STATUS*, size_t, char*, size_t) = 0;

		Block(Block*, size_t);
		virtual ~Block() {}
	};

	// Virtual memory block
	class MemoryBlock : public Block {
	private:
		// Base address of the allocated virtual memory
		char* address;

	public:
		size_t read(ISC_STATUS*, size_t, char*, size_t);
		size_t write(ISC_STATUS*, size_t, char*, size_t);
		
		MemoryBlock(Block*, size_t);
		~MemoryBlock();
	};

	// File block
	class FileBlock : public Block {
	private:
		// Sort file block
		struct sfb *file;
		// File offset
		size_t offset;

	public:
		size_t read(ISC_STATUS*, size_t, char*, size_t);
		size_t write(ISC_STATUS*, size_t, char*, size_t);

		FileBlock(Block*, size_t, struct sfb*, size_t);
		~FileBlock();
	};

	// Initialization flag
	static bool is_initialized;

	// Virtual memory allocation values
	static size_t mem_block_size;
	static size_t mem_upper_limit;

	// Total amount of allocated virtual memory
	static size_t mem_total_size;

	struct sfb *internal;

	// Virtual scratch file size
	size_t logical_size;
	// Amount of storage space allocated for this scratch file
	size_t physical_size;
	// File size on disk
	size_t file_size;

	// First block in chain
	Block *head;
	// Last block in chain
	Block *tail;

	// Allocate one more block, if necessary
	void allocate(size_t);
	// Convert logical position to the physical one - pair [block, offset]
	Block* seek(size_t&);

public:
	// Read bytes from the scratch file
	size_t read(ISC_STATUS*, size_t, char*, size_t);
	// Write bytes into the scratch file
	size_t write(ISC_STATUS*, size_t, char*, size_t);

	SortMem(struct sfb*, size_t);
	~SortMem();
};

#endif	// SORT_MEM_H
