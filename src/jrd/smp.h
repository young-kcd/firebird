#ifndef _JRD_SMP_H_
#define _JRD_SMP_H_

class SmpLock
{
    public:
        SmpLock() {}
        virtual ~SmpLock() {}

        virtual void aquire(void) = 0;
        virtual void release(void) = 0;
};

#endif
