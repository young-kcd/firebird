/*
 *	PROGRAM:		JRD Sort
 *	MODULE:			sort_mem.h
 *	DESCRIPTION:	Sort Space Management
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
 *
 * 2002.04.28 Dmitry Yemanov - Created new implementation of temporary
 *							   sort space that allows virtual memory to
 *							   be used as much as possible (or as much
 *							   as server configured for).
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
		long size;

	public:
		// Read bytes from the block into the given buffer
		virtual long read(STATUS*, long, char*, long) = 0;
		// Write bytes from the given buffer into the block
		virtual long write(STATUS*, long, char*, long) = 0;

		Block(Block*, long);
		virtual ~Block() {}
	};

	// Virtual memory block
	class MemoryBlock : public Block {
	private:
		// Base address of the allocated virtual memory
		char* address;

	public:
		long read(STATUS*, long, char*, long);
		long write(STATUS*, long, char*, long);
		
		MemoryBlock(Block*, long);
		~MemoryBlock();
	};

	// File block
	class FileBlock : public Block {
	private:
		// Sort file block
		struct sfb *file;
		// File offset
		long offset;

	public:
		long read(STATUS*, long, char*, long);
		long write(STATUS*, long, char*, long);

		FileBlock(Block*, long, struct sfb*, long);
		~FileBlock();
	};

	// Initialization flag
	static bool is_initialized;

	// Virtual memory allocation values
	static unsigned long mem_block_size;
	static unsigned long mem_upper_limit;

	// Total amount of allocated virtual memory
	static unsigned long mem_total_size;

	struct sfb *internal;

	// Virtual scratch file size
	long logical_size;
	// Amount of storage space allocated for this scratch file
	long physical_size;
	// File size on disk
	long file_size;

	// First block in chain
	Block *head;
	// Last block in chain
	Block *tail;

	// Allocate one more block, if necessary
	void allocate(long);
	// Convert logical position to the physical one - pair [block, offset]
	Block* seek(long&);

public:
	// Read bytes from the scratch file
	long read(STATUS*, long, char*, long);
	// Write bytes into the scratch file
	long write(STATUS*, long, char*, long);

	SortMem(struct sfb*, long);
	~SortMem();
};

#endif	// SORT_MEM_H
