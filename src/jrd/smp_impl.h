#ifndef _JRD_SMP_IMPL_H_
#define _JRD_SMP_IMPL_H_

#include "../jrd/smp.h"
#include "../jrd/thd.h"

class V4Mutex : public SmpLock
{
    public:
        V4Mutex() { V4_MUTEX_INIT(&mutex); }
        ~V4Mutex() { V4_MUTEX_DESTROY(&mutex); }

        virtual void aquire(void) { V4_MUTEX_LOCK(&mutex); }
        virtual void release(void) { V4_MUTEX_UNLOCK(&mutex); }

    private:
        MUTX mutex;
};

#endif
