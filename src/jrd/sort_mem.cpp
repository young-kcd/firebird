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

#include "firebird.h"

#include "../common/config/config.h"
#include "../jrd/gds_proto.h"
#include "../jrd/sort_proto.h"
#include "../jrd/gdsassert.h"
#include "../jrd/sort_mem.h"

bool SortMem::is_initialized = false;

size_t SortMem::mem_block_size;
size_t SortMem::mem_upper_limit;

size_t SortMem::mem_total_size = 0;

/******************************************************************************
 *
 *	Generic storage block implementation
 */

SortMem::Block::Block(Block *tail, size_t length)
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

SortMem::MemoryBlock::MemoryBlock(Block* tail, size_t length)
	: Block(tail, length)
{
	// Allocate virtual memory block
	address = reinterpret_cast<char*>(gds__alloc(size));
}

SortMem::MemoryBlock::~MemoryBlock()
{
	// Free virtual memory block
	gds__free(address);
}

size_t SortMem::MemoryBlock::read(STATUS *status, size_t position, char *buffer, size_t length)
{
	// Read block from memory
	if (position + length > size)
	{
		length = size - position;
	}
	memcpy(buffer, address + position, length);
	return length;
}

size_t SortMem::MemoryBlock::write(STATUS *status, size_t position, char *buffer, size_t length)
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

SortMem::FileBlock::FileBlock(Block *tail, size_t length, struct sfb *blk, size_t position)
	: Block(tail, length), file(blk), offset(position)
{
}

SortMem::FileBlock::~FileBlock()
{
}

size_t SortMem::FileBlock::read(STATUS *status, size_t position, char *buffer, size_t length)
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

size_t SortMem::FileBlock::write(STATUS *status, size_t position, char *buffer, size_t length)
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

SortMem::SortMem(struct sfb *blk, size_t size)
	: internal(blk), logical_size(0), physical_size(0), file_size(0), head(0), tail(0)
{
	// Initialize itself
	if (!is_initialized)
	{
		mem_block_size = Config::getSortMemBlockSize();
		mem_upper_limit = Config::getSortMemUpperLimit();
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

void SortMem::allocate(size_t size)
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
			size_t smart_size = (mem_block_size > size) ? mem_block_size : size;

			// Check whether virtual memory should be allocated or file should be used instead
			if (mem_total_size + smart_size <= mem_upper_limit)
			{
				// Allocate block in virtual memory
				try
				{
					block = FB_NEW (*getDefaultMemoryPool())
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
							block = FB_NEW (*getDefaultMemoryPool())
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
				block = FB_NEW (*getDefaultMemoryPool())
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

SortMem::Block* SortMem::seek(size_t &position)
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

size_t SortMem::read(STATUS *status, size_t position, char *address, size_t length)
{
	// If we'are not allowed to use memory, don't waste time
	// playing with all these memory blocks - just use scratch file and return
	if (!mem_upper_limit)
	{
		return SORT_read_block(status, internal, position, 
			reinterpret_cast<unsigned char*>(address), length);
	}

	size_t copied = 0;

	if (length > 0)
	{
		// Search for the first needed block
		size_t pos = position;
		Block *block = seek(pos);
		assert(block);

		// Read data from as many blocks as necessary
		for (Block *itr = block; itr && length > 0; itr = itr->next, pos = 0)
		{
			size_t n = itr->read(status, pos, address, length);
			address += n;
			copied += n;
			length -= n;
		}
		assert(!length);
	}

	// New seek value
	return position + copied;
}

size_t SortMem::write(STATUS *status, size_t position, char *address, size_t length)
{
	// If we'are not allowed to use memory, don't waste time
	// playing with all these memory blocks - just use scratch file and return
	if (!mem_upper_limit)
	{
		return SORT_write_block(status, internal, position, 
			reinterpret_cast<unsigned char*>(address), length);
	}

	// There's probably not enough space, try to allocate one more block
	if (position + length >= logical_size)
	{
		allocate(position + length - logical_size);
	}

	size_t copied = 0;

	if (length > 0)
	{
		// Search for the first needed block
		size_t pos = position;
		Block *block = seek(pos);
		assert(block);

		// Write data to as many blocks as necessary
		for (Block *itr = block; itr && length > 0; itr = itr->next, pos = 0)
		{
			size_t n = itr->write(status, pos, address, length);
			address += n;
			copied += n;
			length -= n;
		}
		assert(!length);
	}

	// New seek value
	return position + copied;
}
