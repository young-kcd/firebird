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

#ifndef JRD_TEMP_SPACE_H
#define JRD_TEMP_SPACE_H

#include "firebird.h"
#include "../common/classes/fb_string.h"
#include "../common/classes/array.h"
#include "../common/classes/TempFile.h"
#include "../common/config/dir_list.h"
#include "../common/classes/init.h"

class TempSpace : public Firebird::File
{
public:
	TempSpace(MemoryPool& pool, const Firebird::PathName& prefix);
	virtual ~TempSpace();

	size_t read(offset_t offset, void* buffer, size_t length);
	size_t write(offset_t offset, const void* buffer, size_t length);

	void unlink() {}

	offset_t getSize() const
	{
		return logicalSize;
	}

	void extend(size_t size);

	offset_t allocateSpace(size_t size);
	void releaseSpace(offset_t offset, size_t size);

	char* inMemory(offset_t offset, size_t size) const;

	struct SegmentInMemory
	{
		char* memory;
		offset_t position;
		size_t size;
	};

	typedef Firebird::Array<SegmentInMemory> Segments;

	size_t allocateBatch(size_t count, size_t minSize, size_t maxSize,
		Segments& segments);

	bool validate(offset_t& freeSize) const;
private:

	// Generic space block
	class Block
	{
	public:
		Block(Block* tail, size_t length);
		virtual ~Block() {}

		virtual size_t read(offset_t offset, void* buffer, size_t length) = 0;
		virtual size_t write(offset_t offset, const void* buffer, size_t length) = 0;

		virtual char* inMemory(offset_t offset, size_t size) const = 0;
		virtual bool sameFile(const TempFile* file) const = 0;

		Block *prev;
		Block *next;
		offset_t size;
	};

	class MemoryBlock : public Block
	{
	public:
		MemoryBlock(MemoryPool& pool, Block* tail, size_t length);
		~MemoryBlock();

		size_t read(offset_t offset, void* buffer, size_t length);
		size_t write(offset_t offset, const void* buffer, size_t length);

		char* inMemory(offset_t offset, size_t _size) const
		{
			if ((offset < this->size) && (offset + _size <= this->size))
				return ptr + offset;

			return NULL;
		}

		bool sameFile(const TempFile* file) const
		{
			return false;
		}

	private:
		char* ptr;
	};

	class FileBlock : public Block
	{
	public:
		FileBlock(TempFile* file, Block* tail, size_t length);
		~FileBlock();

		size_t read(offset_t offset, void* buffer, size_t length);
		size_t write(offset_t offset, const void* buffer, size_t length);

		char* inMemory(offset_t offset, size_t a_size) const
		{
			return NULL;
		}

		bool sameFile(const TempFile* aFile) const
		{
			return (aFile == this->file);
		}

	private:
		TempFile* file;
		offset_t seek;
	};

	Block* findBlock(offset_t& offset) const;
	TempFile* setupFile(size_t size);

	virtual bool adjustCacheSize(long) const
	{
		return false;
	}

	char* findMemory(offset_t& begin, offset_t end, size_t size) const;

	//  free/used segments management
	class Segment
	{
	public:
		Segment(Segment* _next, offset_t _position, offset_t _size) :
			next(_next), position(_position), size(_size)
		{}

		Segment* next;
		offset_t position;
		offset_t size;
	};

	Segment* getSegment(offset_t position, size_t size);
	void joinSegment(Segment* seg, offset_t position, size_t size);

	MemoryPool& pool;
	Firebird::PathName filePrefix;
	offset_t logicalSize;
	offset_t physicalSize;
	offset_t localCacheUsage;
	Block* head;
	Block* tail;
	Firebird::Array<TempFile*> tempFiles;

	Segment* freeSegments;
	Segment* notUsedSegments;

	static Firebird::GlobalPtr<Firebird::Mutex> initMutex;
	static Firebird::TempDirectoryList* tempDirs;
	static size_t minBlockSize;
	static offset_t globalCacheUsage;
};

#endif // JRD_TEMP_SPACE_H
