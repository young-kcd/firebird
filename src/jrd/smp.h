#ifndef JRD_SMP_H
#define JRD_SMP_H

class SmpLock
{
    public:
        SmpLock() {}
        virtual ~SmpLock() {}

        virtual void acquire(void) = 0;
        virtual void release(void) = 0;
};

#endif // JRD_SMP_H

