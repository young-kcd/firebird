/*
 *	PROGRAM:		JRD Sort
 *	MODULE:			sort_mem.cpp
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
 *							   as the server configured for).
 */

#include "../fbutil/FirebirdConfig.h"
#include "../common/memory/allocators.h"
#include "../jrd/sort_proto.h"
#include "../jrd/gdsassert.h"

#include "../jrd/sort_mem.h"


#define BLOCK_SIZE_KEY "SORT_MEM_BLOCK_SIZE"
#define UPPER_LIMIT_KEY "SORT_MEM_UPPER_LIMIT"


bool SortMem::is_initialized = false;

unsigned long SortMem::mem_block_size = 1048576;				// 1MB
unsigned long SortMem::mem_upper_limit = (unsigned long) -1;	// ~4GB

unsigned long SortMem::mem_total_size = 0;


/******************************************************************************
 *
 *	Generic storage block implementation
 */

SortMem::Block::Block(Block *tail, long length)
	: next(0), size(length)
{
	// Link block with the chain
	if (tail)
	{
		tail->next = this;
	}
	prev = tail;
}

/******************************************************************************
 *
 *	Virtual memory block implementation
 */

SortMem::MemoryBlock::MemoryBlock(Block* tail, long length)
	: Block(tail, length)
{
	// Allocate virtual memory block
	address = reinterpret_cast<char*>(MemoryPool::virtual_alloc_from_system(size));
//	address = reinterpret_cast<char*>(MemoryPool::malloc_from_system(size));
}

SortMem::MemoryBlock::~MemoryBlock()
{
	// Free virtual memory block
	MemoryPool::virtual_free_from_system(address);
//	MemoryPool::free_from_system(address);
}

long SortMem::MemoryBlock::read(STATUS *status, long position, char *buffer, long length)
{
	// Read block from memory
	if (position + length > size)
	{
		length = size - position;
	}
	memcpy(buffer, address + position, length);
	return length;
}

long SortMem::MemoryBlock::write(STATUS *status, long position, char *buffer, long length)
{
	// Write block to memory
	if (position + length > size)
	{
		length = size - position;
	}
	memcpy(address + position, buffer, length);
	return length;
}
		
/******************************************************************************
 *
 *	File block implementation
 */

SortMem::FileBlock::FileBlock(Block *tail, long length, struct sfb *blk, long position)
	: Block(tail, length), file(blk), offset(position)
{
}

SortMem::FileBlock::~FileBlock()
{
}

long SortMem::FileBlock::read(STATUS *status, long position, char *buffer, long length)
{
	// Read block from file
	if (position + length > size)
	{
		length = size - position;
	}
	return SORT_read_block(status, file, offset + position,
		reinterpret_cast<unsigned char*>(buffer), length) - offset - position;
//	_lseek(file->sfb_file, offset + position, SEEK_SET);
//	return _read(file->sfb_file, buffer, length);
}

long SortMem::FileBlock::write(STATUS *status, long position, char *buffer, long length)
{
	// Write block to file
	if (position + length > size)
	{
		length = size - position;
	}
	return SORT_write_block(status, file, offset + position, 
		reinterpret_cast<unsigned char*>(buffer), length) - offset - position;
//	_lseek(file->sfb_file, offset + position, SEEK_SET);
//	return _write(file->sfb_file, buffer, length);
}

/******************************************************************************
 *
 *	Virtual scratch file implementation
 */

SortMem::SortMem(struct sfb *blk, long size)
	: internal(blk), logical_size(0), physical_size(0), file_size(0), head(0), tail(0)
{
	// Override defaults with the configuration values, if they exist
	if (!is_initialized)
	{
		if (FirebirdConfig::doesSysKeyExist(BLOCK_SIZE_KEY))
		{
			mem_block_size = FirebirdConfig::getSysInt(BLOCK_SIZE_KEY);
		}
		if (FirebirdConfig::doesSysKeyExist(UPPER_LIMIT_KEY))
		{
			mem_upper_limit = FirebirdConfig::getSysInt(UPPER_LIMIT_KEY);
		}
		is_initialized = true;
	}

	// Allocate one block
	allocate(size);
}

SortMem::~SortMem()
{
	// Free all allocated blocks
	while (tail)
	{
		Block *block = tail->prev;
		delete tail;
		tail = block;
	}
	// We've just freed some memory
	mem_total_size -= physical_size - file_size;
}

void SortMem::allocate(long size)
{
	if (size > 0)
	{
		logical_size += size;
		bool mem_allocated = false;

		// We've already got enough space to write data of the given size
		if (logical_size > physical_size) 
		{
			Block *block;

			// Calculate how much virtual memory we should allocate
			long smart_size = (mem_block_size > size) ? mem_block_size : size;

			// Check whether virtual memory should be allocated or file should be used instead
			if (mem_total_size + smart_size <= mem_upper_limit)
			{
				// Allocate block in virtual memory
				try
				{
					block = new (*getDefaultMemoryPool())
						MemoryBlock(tail, smart_size);
					mem_allocated = true;
				}
				// Not enough memory to allocate block
				catch (std::bad_alloc)
				{
					// Check whether we can try to allocate less memory
					if (smart_size > size)
					{
						// Allocate minimal possible amount of memory once more
						smart_size = size;
						try
						{
							block = new (*getDefaultMemoryPool())
								MemoryBlock(tail, smart_size);
							mem_allocated = true;
						}
						catch (std::bad_alloc) {}
					}
				}
				if (mem_allocated)
				{
					physical_size += smart_size;
					// We've just allocated some virtual memory
					mem_total_size += smart_size;
				}
			}

			if (!mem_allocated)
			{
				// Allocate block on disk
				block = new (*getDefaultMemoryPool())
					FileBlock(tail, size, internal, file_size);
				physical_size += size;
				// We've just allocated some file storage
				file_size += size;
			}

			// Append new block to the chain
			if (!head)
			{
				head = block;
			}
			tail = block;
		}
	}
}

SortMem::Block* SortMem::seek(long &position)
{
	Block *block = 0;

	// Check whether the given offset is valid
	if (position < physical_size)
	{
		if (position < physical_size / 2)
		{
			// Let's walk forward
			block = head;
			while (block && position >= block->size)
			{
				position -= block->size;
				block = block->next;
			}
		}
		else
		{
			// Let's walk backward
			block = tail;
			while (block && physical_size - position > block->size)
			{
				position += block->size;
				block = block->prev;
			}
			position += block->size - physical_size;
		}
	}

	return block;
}

long SortMem::read(STATUS *status, long position, char *address, long length)
{
	long copied = 0;

	if (length > 0)
	{
		// Search for the first needed block
		long pos = position;
		Block *block = seek(pos);
		assert(block);

		// Read data from as many blocks as necessary
		for (Block *itr = block; itr, length > 0; itr = itr->next, pos = 0)
		{
			long n = itr->read(status, pos, address, length);
			address += n;
			copied += n;
			length -= n;
		}
		assert(!length);
	}

	// New seek value
	return position + copied;
}

long SortMem::write(STATUS *status, long position, char *address, long length)
{
	// There's probably not enough space, try to allocate one more block
	if (position + length >= logical_size)
	{
		allocate(position + length - logical_size);
	}

	long copied = 0;

	if (length > 0)
	{
		// Search for the first needed block
		long pos = position;
		Block *block = seek(pos);
		assert(block);

		// Write data to as many blocks as necessary
		for (Block *itr = block; itr, length > 0; itr = itr->next, pos = 0)
		{
			long n = itr->write(status, pos, address, length);
			address += n;
			copied += n;
			length -= n;
		}
		assert(!length);
	}

	// New seek value
	return position + copied;
}
