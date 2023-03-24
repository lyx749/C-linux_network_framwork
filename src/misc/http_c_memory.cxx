#include "http_c_memory.h"
CMemory *CMemory::memoryInstance = NULL;
void *CMemory::AllocMemory(int needMemoryCount, bool ifMemset)
{
    void *ptr = (void *)new char[needMemoryCount];
    if (ifMemset)
    {
        memset(ptr, 0, needMemoryCount);
    }
    return ptr;
}
void CMemory::FreeMemory(void *point)
{
    delete[] ((char *)point);
}