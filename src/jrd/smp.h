#ifndef JRD_SMP_H
#define JRD_SMP_H

class SmpLock
{
    public:
        SmpLock() {}
        virtual ~SmpLock() {}

        virtual void aquire(void) = 0;
        virtual void release(void) = 0;
};

#endif
