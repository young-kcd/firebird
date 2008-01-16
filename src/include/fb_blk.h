#ifndef INCLUDE_FB_BLK
#define INCLUDE_FB_BLK

#include "../common/classes/alloc.h"

struct blk
{
};
typedef blk* BLK;
typedef blk* BlkPtr;

// Traditional way to check handle type
template<SSHORT BlockType>
class PoolType : public blk
{
public:
	bool checkHandle() const
	{
		if (!this)
		{
			return false;
		}
		return MemoryPool::blk_type(this) == BlockType;
	}
};

template<SSHORT BLOCK_TYPE = 0>
class pool_alloc : public PoolType<BLOCK_TYPE>
{
    public:
#ifdef DEBUG_GDS_ALLOC
        void* operator new(size_t s, MemoryPool& p, char* file, int line)
            { return p.calloc(s, BLOCK_TYPE, file, line); }
        void* operator new[](size_t s, MemoryPool& p, char* file, int line)
            { return p.calloc(s, BLOCK_TYPE, file, line); }
#else
        void* operator new(size_t s, MemoryPool& p )
            { return p.calloc(s, BLOCK_TYPE); }
        void* operator new[](size_t s, MemoryPool& p)
            { return p.calloc(s, BLOCK_TYPE); }
#endif

        void operator delete(void* mem, MemoryPool& p)
            { if (mem) p.deallocate(mem); }
        void operator delete[](void* mem, MemoryPool& p)
            { if (mem) p.deallocate(mem); }

        void operator delete(void* mem) { if (mem) MemoryPool::globalFree(mem); }
        void operator delete[](void* mem) { if (mem) MemoryPool::globalFree(mem); }

private:
    /* These operators are off-limits */
	void* operator new(size_t s) { return 0; }
    void* operator new[](size_t s) { return 0; }
};

template<typename RPT, SSHORT BLOCK_TYPE = 0>
class pool_alloc_rpt : public PoolType<BLOCK_TYPE>
{
    public:
		typedef RPT blk_repeat_type;
#ifdef DEBUG_GDS_ALLOC
        void* operator new(size_t s, MemoryPool& p, int rpt, char* file, int line)
            { return p.calloc(s + sizeof(RPT) * rpt, BLOCK_TYPE, file, line); }
#else
        void* operator new(size_t s, MemoryPool& p, int rpt)
            { return p.calloc(s + sizeof(RPT) * rpt, BLOCK_TYPE); }
#endif
        void operator delete(void* mem, MemoryPool& p, int rpt)
            { if (mem) p.deallocate(mem); }
        void operator delete(void* mem) { if (mem) MemoryPool::globalFree(mem); }

    private:
        // These operations are not supported on static repeat-base objects
        void* operator new[](size_t s, MemoryPool& p)
            { return 0; }
        void operator delete[](void* mem, MemoryPool& p)
            { }
        void operator delete[](void* mem) { }

private:
    /* These operators are off-limits */
	void* operator new(size_t s) { return 0; }
    void* operator new[](size_t s) { return 0; }
};

// New way to check handle type
template<SSHORT BlockType>
class LocalType
{
private:
	const SSHORT blockType;

public:
	LocalType() : blockType(BlockType) { }

	bool checkHandle() const
	{
		if (!this)
		{
			return false;
		}
		return blockType == BlockType;
	}
};

#endif	/* INCLUDE_FB_BLK */

