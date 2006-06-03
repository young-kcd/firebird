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

// Const definitions

static const size_t MAX_FILE_SIZE = 1073741824; // 1GB

// Static definitions/initializations

Firebird::Mutex TempSpace::initMutex;
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

size_t TempSpace::MemoryBlock::read(size_t offset, void* buffer, size_t length)
{
	if (offset + length > size)
	{
		length = size - offset;
	}
	memcpy(buffer, (char*) ptr + offset, length);
	return length;
}

size_t TempSpace::MemoryBlock::write(size_t offset, void* buffer, size_t length)
{
	if (offset + length > size)
	{
		length = size - offset;
	}
	memcpy((char*) ptr + offset, buffer, length);
	return length;
}

//
// On-disk block class
//

TempSpace::FileBlock::FileBlock(TempFile* f, Block* tail, size_t length)
	: Block(tail, length), file(f)
{
	fb_assert(file);
	seek = file->getSize();
}

TempSpace::FileBlock::~FileBlock()
{
}

size_t TempSpace::FileBlock::read(size_t offset, void* buffer, size_t length)
{
	if (offset + length > size)
	{
		length = size - offset;
	}
	offset += seek;
	return file->read(offset, buffer, length);
}

size_t TempSpace::FileBlock::write(size_t offset, void* buffer, size_t length)
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
		: pool(p), tempFiles(p),
		  head(NULL), tail(NULL), filePrefix(p, prefix),
		  logicalSize(0), physicalSize(0), localCacheUsage(0)
{
	if (!tempDirs)
	{
		Firebird::MutexLockGuard guard(initMutex);
		if (!tempDirs)
		{
			MemoryPool& p = *getDefaultMemoryPool();
			tempDirs = FB_NEW(p) Firebird::TempDirectoryList(p);
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
		for (Block* itr = block; itr && l;
			itr = itr->next, offset = 0)
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

size_t TempSpace::write(offset_t offset, void* buffer, size_t length)
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
		Block* block = findBlock(offset);

		char* p = static_cast<char*>(buffer);
		size_t l = length;

		// write data to as many blocks as necessary
		for (Block* itr = block; itr && l;
			itr = itr->next, offset = 0)
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
		size = FB_ALIGN(size, minBlockSize);
		physicalSize += size;

		Block* block = NULL;

		if (globalCacheUsage + size <= Config::getTempCacheLimit())
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

TempSpace::Block* TempSpace::findBlock(offset_t& offset)
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
			PathUtils::splitLastComponent(dirname, filename,
										  tempFiles[j]->getName());
			PathUtils::ensureSeparator(dirname);
			if (!directory.compare(dirname) &&
				tempFiles[j]->getSize() + size <= MAX_FILE_SIZE)
			{
				file = tempFiles[i];
				break;
			}
		}

		if (!file)
		{
			fb_assert(size <= MAX_FILE_SIZE);
			file = FB_NEW(pool) TempFile(pool, filePrefix, directory);
			tempFiles.add(file);
		}

		fb_assert(file);

		try
		{
			file->extend(size);
		}
		catch (const Firebird::system_call_failed&)
		{
			if (i < tempDirs->getCount() - 1)
			{
				// no room, let's try another directory
				Firebird::string text;
				text.printf("Temporary directory %s has no free space, "
							"attempting to switch to the next configured one",
							directory.c_str());
				gds__log(text.c_str());
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
