#ifndef INCLUDE_FB_BLK
#define INCLUDE_FB_BLK

#include "../common/memory/memory_pool.h"
//#include <vector>

struct blk
{
};
typedef blk* BLK;


//typedef PtrWrapper<BLK> BlkPtr;
typedef blk* BlkPtr;

template<SSHORT TYPE = 0>
class pool_alloc : public blk
{
    public:
#ifdef DEBUG_GDS_ALLOC
        void* operator new(size_t s, MemoryPool& p, char* file, int line)
            { return p.allocate(s, TYPE, file, line); }
        void* operator new[](size_t s, MemoryPool& p, char* file, int line)
            { return p.allocate(s, TYPE, file, line); }
#else
        void* operator new(size_t s, MemoryPool& p )
            { return p.allocate(s, TYPE); }
        void* operator new[](size_t s, MemoryPool& p)
            { return p.allocate(s, TYPE); }
#endif

        void operator delete(void* mem, MemoryPool& p)
            { if (mem) p.deallocate(mem); }
        void operator delete[](void* mem, MemoryPool& p)
            { if (mem) p.deallocate(mem); }

        void operator delete(void* mem) { if (mem) MemoryPool::deallocate(mem); }
        void operator delete[](void* mem) { if (mem) MemoryPool::deallocate(mem); }

private:
    /* These operators are off-limits */
	void* operator new(size_t s) { return 0; }
    void* operator new[](size_t s) { return 0; }
};

template<class RPT, SSHORT TYPE = 0>
class pool_alloc_rpt : public blk
{
    public:
#ifdef DEBUG_GDS_ALLOC
        void* operator new(size_t s, MemoryPool& p, int rpt, char *file, int line)
            { return p.allocate(s + sizeof(RPT)*rpt, TYPE, file, line); }
#else
        void* operator new(size_t s, MemoryPool& p, int rpt)
            { return p.allocate(s + sizeof(RPT)*rpt, TYPE); }
#endif
        void operator delete(void* mem, MemoryPool& p,int rpt)
            { if (mem) p.deallocate(mem); }
        void operator delete(void* mem) { if (mem) MemoryPool::deallocate(mem); }

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

/* template<class BASE, class RPT, UCHAR TYPE>
class vector_rpt : public BASE
{
    private:
        MemoryPool::allocator<RPT> rptAllocator;
        vector_rpt(int size, MemoryPool& pool)
          : rptAllocator(pool, TYPE), rpt(size, rptAllocator) {}

    public:
        std::vector<RPT>    rpt;

        static vector_rpt<BASE,RPT,TYPE>* allocate(int size, MemoryPool& p)
            { return new(p) vector_rpt<BASE,RPT,TYPE>(size,p); }
}; */

#endif	/* INCLUDE_FB_BLK */
