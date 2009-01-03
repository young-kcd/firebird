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
 *  Copyright (c) 2006 Dmitry Yemanov <dimitr@users.sf.net>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"

#include "../common/config/config.h"
#include "../common/config/dir_list.h"
#include "../jrd/gdsassert.h"
#include "../jrd/gds_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/os/path_utils.h"

#include "../jrd/TempSpace.h"

// Static definitions/initializations

Firebird::GlobalPtr<Firebird::Mutex> TempSpace::initMutex;
Firebird::TempDirectoryList* TempSpace::tempDirs = NULL;
size_t TempSpace::minBlockSize = 0;
offset_t TempSpace::globalCacheUsage = 0;

//
// Generic space block class
//

TempSpace::Block::Block(Block* tail, size_t length)
	: next(NULL), size(length)
{
	if (tail)
	{
		tail->next = this;
	}
	prev = tail;
}

//
// In-memory block class
//

TempSpace::MemoryBlock::MemoryBlock(MemoryPool& pool, Block* tail, size_t length)
	: Block(tail, length)
{
	ptr = FB_NEW(pool) char[length];
}

TempSpace::MemoryBlock::~MemoryBlock()
{
	delete[] ptr;
}

size_t TempSpace::MemoryBlock::read(offset_t offset, void* buffer, size_t length)
{
	if (offset + length > size)
	{
		length = size - offset;
	}
	memcpy(buffer, ptr + offset, length);
	return length;
}

size_t TempSpace::MemoryBlock::write(offset_t offset, const void* buffer, size_t length)
{
	if (offset + length > size)
	{
		length = size - offset;
	}
	memcpy(ptr + offset, buffer, length);
	return length;
}

//
// On-disk block class
//

TempSpace::FileBlock::FileBlock(TempFile* f, Block* tail, size_t length)
	: Block(tail, length), file(f)
{
	fb_assert(file);

	// FileBlock is created after file was extended by length (look at
	// TempSpace::extend) so this FileBlock is already inside the file
	seek = file->getSize() - length;
}

TempSpace::FileBlock::~FileBlock()
{
}

size_t TempSpace::FileBlock::read(offset_t offset, void* buffer, size_t length)
{
	if (offset + length > size)
	{
		length = size - offset;
	}
	offset += seek;
	return file->read(offset, buffer, length);
}

size_t TempSpace::FileBlock::write(offset_t offset, const void* buffer, size_t length)
{
	if (offset + length > size)
	{
		length = size - offset;
	}
	offset += seek;
	return file->write(offset, buffer, length);
}

//
// TempSpace::TempSpace
//
// Constructor
//

TempSpace::TempSpace(MemoryPool& p, const Firebird::PathName& prefix)
		: pool(p), filePrefix(p, prefix),
		  logicalSize(0), physicalSize(0), localCacheUsage(0),
		  head(NULL), tail(NULL), tempFiles(p),
		  freeSegments(NULL), notUsedSegments(NULL)
{
	if (!tempDirs)
	{
		Firebird::MutexLockGuard guard(initMutex);
		if (!tempDirs)
		{
			MemoryPool& def_pool = *getDefaultMemoryPool();
			tempDirs = FB_NEW(def_pool) Firebird::TempDirectoryList(def_pool);
			minBlockSize = Config::getTempBlockSize();
		}
	}
}

//
// TempSpace::~TempSpace
//
// Destructor
//

TempSpace::~TempSpace()
{
	while (head)
	{
		Block* temp = head->next;
		delete head;
		head = temp;
	}

	globalCacheUsage -= localCacheUsage;

	while (tempFiles.getCount())
	{
		delete tempFiles.pop();
	}

	while (freeSegments)
	{
		Segment* temp = freeSegments->next;
		delete freeSegments;
		freeSegments = temp;
	}

	while (notUsedSegments)
	{
		Segment* temp = notUsedSegments->next;
		delete notUsedSegments;
		notUsedSegments = temp;
	}
}

//
// TempSpace::read
//
// Reads bytes from the temporary space
//

size_t TempSpace::read(offset_t offset, void* buffer, size_t length)
{
	fb_assert(offset + length <= logicalSize);

	if (length)
	{
		// search for the first needed block
		Block* block = findBlock(offset);

		char* p = static_cast<char*>(buffer);
		size_t l = length;

		// read data from the block chain
		for (Block* itr = block; itr && l; itr = itr->next, offset = 0)
		{
			const size_t n = itr->read(offset, p, l);
			p += n;
			l -= n;
		}

		fb_assert(!l);
	}

	return length;
}

//
// TempSpace::write
//
// Writes bytes to the temporary space
//

size_t TempSpace::write(offset_t offset, const void* buffer, size_t length)
{
	fb_assert(offset <= logicalSize);

	if (offset + length > logicalSize)
	{
		// not enough space, allocate one more block
		extend(offset + length - logicalSize);
	}

	if (length)
	{
		// search for the first needed block
		Block* const block = findBlock(offset);

		const char* p = static_cast<const char*>(buffer);
		size_t l = length;

		// write data to as many blocks as necessary
		for (Block* itr = block; itr && l; itr = itr->next, offset = 0)
		{
			const size_t n = itr->write(offset, p, l);
			p += n;
			l -= n;
		}

		fb_assert(!l);
	}

	return length;
}

//
// TempSpace::extend
//
// Increases size of the temporary space
//

void TempSpace::extend(size_t size)
{
	logicalSize += size;

	if (logicalSize > physicalSize)
	{
		size = FB_ALIGN(logicalSize - physicalSize, minBlockSize);
		physicalSize += size;

		Block* block = NULL;

		if (globalCacheUsage + size <= size_t(Config::getTempCacheLimit()))
		{
			try
			{
				// allocate block in virtual memory
				block = FB_NEW(pool) MemoryBlock(pool, tail, size);
				localCacheUsage += size;
				globalCacheUsage += size;
			}
			catch (const Firebird::BadAlloc&)
			{
				// not enough memory
			}
		}

		if (!block)
		{
			// allocate block in the temp file
			TempFile* file = setupFile(size);
			if (tail && tail->sameFile(file))
			{
				tail->size += size;
				return;
			}
			block = FB_NEW(pool) FileBlock(file, tail, size);
		}

		// append new block to the chain
		if (!head)
		{
			head = block;
		}
		tail = block;
	}
}

//
// TempSpace::findBlock
//
// Locates the space block corresponding to the given global offset
//

TempSpace::Block* TempSpace::findBlock(offset_t& offset) const
{
	fb_assert(offset <= logicalSize);

	Block* block = NULL;

	if (offset < physicalSize / 2)
	{
		// walk forward
		block = head;
		while (block && offset >= block->size)
		{
			offset -= block->size;
			block = block->next;
		}
		fb_assert(block);
	}
	else
	{
		// walk backward
		block = tail;
		while (block && physicalSize - offset > block->size)
		{
			offset += block->size;
			block = block->prev;
		}
		fb_assert(block);
		offset -= physicalSize - block->size;
	}

	fb_assert(offset <= block->size);
	return block;
}

//
// TempSpace::setupFile
//
// Allocates the required space in some temporary file
//

TempFile* TempSpace::setupFile(size_t size)
{
	TempFile* file = NULL;

	for (size_t i = 0; i < tempDirs->getCount(); i++)
	{
		Firebird::PathName directory = (*tempDirs)[i];
		PathUtils::ensureSeparator(directory);

		for (size_t j = 0; j < tempFiles.getCount(); j++)
		{
			Firebird::PathName dirname, filename;
			PathUtils::splitLastComponent(dirname, filename, tempFiles[j]->getName());
			PathUtils::ensureSeparator(dirname);
			if (!directory.compare(dirname))
			{
				file = tempFiles[j];
				break;
			}
		}

		try
		{
			if (!file)
			{
				file = FB_NEW(pool) TempFile(pool, filePrefix, directory);
				tempFiles.add(file);
			}

			file->extend(size);
		}
		catch (const Firebird::system_call_failed&)
		{
			if (i < tempDirs->getCount() - 1)
			{
				// no room, let's try another directory
				file = NULL;
			}
			else
			{
				// no room in all directories
				gds__log("No free space found in temporary directories");
				throw;
			}
		}

		if (file)
		{
			break;
		}
	}

	fb_assert(file);
	return file;
}

//
// TempSpace::allocateSpace
//
// Allocate available space in free segments. Extend file if necessary
//

offset_t TempSpace::allocateSpace(size_t size)
{
	// Find the best available space. This is defined as the smallest free space
	// that is big enough. This preserves large blocks.
	Segment** best = NULL, *space;

	// Search through the available space in the not used segments list
	for (Segment** ptr = &freeSegments; (space = *ptr); ptr = &(*ptr)->next)
	{
		// If this is smaller than our previous best, use it
		if (space->size >= size && (!best || (space->size < (*best)->size))) {
			best = ptr;
		}
	}

	// If we didn't find any space, allocate it at the end of the file
	if (!best)
	{
		extend(size);
		return getSize() - size;
	}

	// Set up the return parameters
	space = *best;

	// If the hunk was an exact fit, remove the segment from the
	// list and splice it into the not used segments list
	if (space->size == size)
	{
		*best = space->next;
		space->next = notUsedSegments;
		notUsedSegments = space;
		return space->position;
	}

	// The best segment is too big - chop the needed space off the begin
	space->size -= size;
	space->position += size;
	return (space->position - size);
}

//
// TempSpace::releaseSpace
//
// Return previously allocated segment back into not used segments list and
// join it with adjacent segments if found
//

void TempSpace::releaseSpace(offset_t position, size_t size)
{
	fb_assert(size > 0);
	fb_assert(position < getSize());	// Block starts in file
	const offset_t end = position + size;
	fb_assert(end <= getSize());		// Block ends in file

	Segment* new_seg = NULL;
	Segment* space = freeSegments;
	if (!space || end < space->position)
	{
		new_seg = getSegment(position, size);
		freeSegments = new_seg;
		new_seg->next = space;
		return;
	}

	if (end == space->position || position == space->position + space->size)
	{
		joinSegment(space, position, size);
		return;
	}

	while (true)
	{
		Segment* next = space->next;
		if (!next || end < next->position)
		{
			new_seg = getSegment(position, size);
			space->next = new_seg;
			new_seg->next = next;
			return;
		}

		if (end == next->position || position == next->position + next->size)
		{
			joinSegment(next, position, size);
			return;
		}

		space = next;
	}
}

//
// TempSpace::inMemory
//
// Return contiguous chunk of memory if present at given location
//

char* TempSpace::inMemory(offset_t begin, size_t size) const
{
	const Block* block = findBlock(begin);
	return block ? block->inMemory(begin, size) : NULL;
}

//
// TempSpace::findMemory
//
// Return contiguous chunk of memory and adjust starting offset
// of search range if found
//

char* TempSpace::findMemory(offset_t& begin, offset_t end, size_t size) const
{
	offset_t local_offset = begin;
	const offset_t save_begin = begin;
	const Block* block = findBlock(local_offset);

	while (block && (begin + size <= end))
	{
		char* mem = block->inMemory(local_offset, size);
		if (mem)
		{
			return mem;
		}

		begin += block->size - local_offset;
		local_offset = 0;
		block = block->next;
	}

	begin = save_begin;
	return NULL;
}

//
// TempSpace::validate
//
// Validate internal lists for consistency and return back to caller
// amount of available free space
//

bool TempSpace::validate(offset_t& free) const
{
	free = 0;
	for (const Segment* space = freeSegments; space; space = space->next)
	{
		free += space->size;
		fb_assert(!(space->next) || (space->next->position > space->position));
	}

	offset_t disk = 0;
	for (size_t i = 0; i < tempFiles.getCount(); i++)
		disk += tempFiles[i]->getSize();

	return ((localCacheUsage + disk) == physicalSize);
}


//
// TempSpace::allocateBatch
//
// Allocate up to 'count' contiguous chunks of memory available in free
// segments if any. Adjust size of chunks between minSize and maxSize
// accordingly to available free space (assuming all of the free space
// is in memory blocks). Algorithm is very simple and can be improved in future
//

size_t TempSpace::allocateBatch(size_t count, size_t minSize, size_t maxSize, Segments& segments)
{
	// adjust passed chunk size to amount of free memory we have and number
	// of runs still not allocated.
	offset_t freeMem = 0;
	Segment* freeSpace = freeSegments;
	for (; freeSpace; freeSpace = freeSpace->next)
		freeMem += freeSpace->size;

	freeMem = MIN(freeMem / count, maxSize);
	freeMem = MAX(freeMem, minSize);
	freeMem = MIN(freeMem, minBlockSize);

	Segment** prevSpace = &freeSegments;
	freeSpace = freeSegments;
	offset_t freeSeek = freeSpace ? freeSpace->position : 0;
	offset_t freeEnd = freeSpace ? freeSpace->position + freeSpace->size : 0;
	while (segments.getCount() < count && freeSpace)
	{
		char* mem = findMemory(freeSeek, freeEnd, freeMem);

		if (mem)
		{
			fb_assert(freeSeek + freeMem <= freeEnd);
#ifdef DEV_BUILD
			offset_t seek1 = freeSeek;
			char* p = findMemory(seek1, freeEnd, freeMem);
			fb_assert(p == mem);
			fb_assert(seek1 == freeSeek);
#endif
			if (freeSeek != freeSpace->position)
			{
				const ULONG skip_size = freeSeek - freeSpace->position;
				Segment* skip_space = getSegment(freeSpace->position, skip_size);

				(*prevSpace) = skip_space;
				skip_space->next = freeSpace;
				prevSpace = &skip_space->next;

				freeSpace->position += skip_size;
				freeSpace->size -= skip_size;
			}

			SegmentInMemory seg;
			seg.memory = mem;
			seg.position = freeSeek;
			seg.size = freeMem;
			segments.add(seg);

			freeSeek += freeMem;
			freeSpace->position += freeMem;
			freeSpace->size -= freeMem;
			if (!freeSpace->size)
			{
				(*prevSpace) = freeSpace->next;
				freeSpace->next = notUsedSegments;
				notUsedSegments = freeSpace;

				freeSpace = (*prevSpace);
				freeSeek = freeSpace ? freeSpace->position : 0;
				freeEnd = freeSpace ? freeSpace->position + freeSpace->size : 0;
			}
		}
		else
		{
			prevSpace = &freeSpace->next;
			freeSpace = freeSpace->next;
			freeSeek = freeSpace ? freeSpace->position : 0;
			freeEnd = freeSpace ? freeSpace->position + freeSpace->size : 0;
		}
	}

	return segments.getCount();
}


//
// TempSpace::getSegment
//
// Return not used Segment instance or allocate new one
//

TempSpace::Segment* TempSpace::getSegment(offset_t position, size_t size)
{
	Segment* result = notUsedSegments;

	if (result)
	{
		notUsedSegments = result->next;

		result->next = NULL;
		result->position = position;
		result->size = size;
	}
	else
	{
		result = (Segment*) FB_NEW(pool) Segment(NULL, position, size);
	}
	return result;
}


//
// TempSpace::joinSegment
//
// Extend existing segment and join it with adjacent segment
//

void TempSpace::joinSegment(Segment* seg, offset_t position, size_t size)
{
	if (position + size == seg->position)
	{
		seg->position -= size;
		seg->size += size;
	}
	else
	{
		seg->size += size;
		Segment* next = seg->next;
		if (next && next->position == seg->position + seg->size)
		{
			seg->next = next->next;
			seg->size += next->size;

			next->next = notUsedSegments;
			notUsedSegments = next;
		}
	}
}
