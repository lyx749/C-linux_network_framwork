#ifndef HTTP_C_MEMORY_H
#define HTTP_C_MEMORT_H
#include <stddef.h>
#include <stdlib.h>
#include <string.h>


//memory class
class CMemory
{
private:
    CMemory() {}

public:
    ~CMemory() {}

private:
    static inline CMemory *memoryInstance;

public:
    static CMemory *GetInstance()
    {
        if (!memoryInstance)
        {
            memoryInstance = new CMemory();
            static FreeClassCMemory cl;
        }
        return memoryInstance;
    }

    class FreeClassCMemory
    {
    public:
        ~FreeClassCMemory()
        {
            if (CMemory::memoryInstance)
            {
                delete CMemory::memoryInstance;
                CMemory::memoryInstance = NULL;
            }
        }
    };


public:
    void *AllocMemory(int needMemoryCount, bool ifMemset);
    void FreeMemory(void *point);
};

#endif