#ifndef JRD_SMP_IMPL_H
#define JRD_SMP_IMPL_H

#include "../jrd/smp.h"
#include "../jrd/thd.h"

class V4Mutex : public SmpLock
{
    public:
        V4Mutex()
		{ 
#ifdef V4_THREADING
			V4_MUTEX_INIT(&mutex);
#endif
		}
        ~V4Mutex()
		{ 
#ifdef V4_THREADING
			V4_MUTEX_DESTROY(&mutex);
#endif
		}
        virtual void acquire(void)
		{
#ifdef V4_THREADING
			V4_MUTEX_LOCK(&mutex);
#endif
		}
        virtual void release(void)
		{ 
#ifdef V4_THREADING
			V4_MUTEX_UNLOCK(&mutex);
#endif
		}

    private:
        MUTX mutex;
};

#endif // JRD_SMP_IMPL_H

