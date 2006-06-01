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

class TempSpace : public Firebird::File {
public:
	TempSpace(MemoryPool&, const Firebird::PathName&);
	virtual ~TempSpace();

	size_t read(offset_t, void*, size_t);
	size_t write(offset_t, void*, size_t);

	void unlink() {}

	offset_t getSize() const
	{
		return logicalSize;
	}

	void extend(size_t);

private:

	// Generic space block
	class Block {
	public:
		Block(Block*, size_t);
		virtual ~Block() {}

		virtual size_t read(size_t, void*, size_t) = 0;
		virtual size_t write(size_t, void*, size_t) = 0;

		Block *prev;
		Block *next;
		size_t size;
	};

	class MemoryBlock : public Block {
	public:
		MemoryBlock(MemoryPool&, Block*, size_t);
		~MemoryBlock();

		size_t read(size_t, void*, size_t);
		size_t write(size_t, void*, size_t);
		
	private:
		void* ptr;
	};

	class FileBlock : public Block {
	public:
		FileBlock(TempFile*, Block*, size_t);
		~FileBlock();

		size_t read(size_t, void*, size_t);
		size_t write(size_t, void*, size_t);

	private:
		TempFile* file;
		size_t seek;
	};

	Block* findBlock(offset_t&);
	TempFile* setupFile(size_t);

	virtual bool adjustCacheSize(long) const
	{
		return false;
	}

	MemoryPool& pool;
	Firebird::PathName filePrefix;
	offset_t logicalSize;
	offset_t physicalSize;
	offset_t localCacheUsage;
	Block* head;
	Block* tail;
	Firebird::Array<TempFile*> tempFiles;

	static Firebird::Mutex initMutex;
	static Firebird::TempDirectoryList* tempDirs;
	static size_t minBlockSize;
	static offset_t globalCacheUsage;
};

#endif // JRD_TEMP_SPACE_H
