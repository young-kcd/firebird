#ifndef JRD_BLOCK_CACHE_H
#define JRD_BLOCK_CACHE_H

#include "../common/classes/alloc.h"
#include "../jrd/smp_impl.h"

template <class T>
class BlockCache
{
public:
	BlockCache(MemoryPool& p) : pool(p), head(0) {}
	~BlockCache();

	T* newBlock();
	void returnBlock(T*) ;

private:
	struct Node
	{
		Node*	next;
	};
	MemoryPool&	pool;
	Node*		head;
	V4Mutex		lock;
};

template <class T>
inline T* BlockCache<T>::newBlock()
{
	lock.acquire();
    if (head)
    {
        T* result = reinterpret_cast<T*>(head);
        head = head->next;
		lock.release();
        return result;
    }
	lock.release();
    return FB_NEW(pool) T;
}

template<class T>
inline void BlockCache<T>::returnBlock(T* back)
{
	Node* returned = reinterpret_cast<Node*>(back);
	lock.acquire();
	returned->next = head;
	head = returned;
	lock.release();
}

template <class T>
BlockCache<T>::~BlockCache()
{
	// Notice there is no destructor?  This is because all our memory
	// is allocated from a pool.  When the pool gets freed so will our
	// storage, simple as that.  No need to waste extra processor time
	// freeing memory just to have it freed again!
/*	Node *next;
	while (head)
	{
		next = head->next;
		delete ((T*)head);
		head = next;
	} */
}

#endif // JRD_BLOCK_CACHE_H

