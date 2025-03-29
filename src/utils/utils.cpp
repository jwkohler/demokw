// #include <time.h>
// #include <stdio.h>
// #include <malloc.h>
// #include <pico/stdlib.h>
// #include <FreeRTOS.h>

#include "utils.h"

//
//
//
clock_t clock()
{
    return (clock_t)time_us_64() / 1000;
}

size_t formatTimestamp(char *buff, size_t size)
{
    uint32_t total_ms = us_to_ms(time_us_64());
    uint32_t k = total_ms / 1000u; // temp
    size_t offset = 0;
    return snprintf(buff, size, "%02lu:%02lu:%02lu.%03lu",
                    k / 3600u,
                    (k / 60u) % 60u,
                    k % 60,
                    total_ms % 1000u);
}

//
// total available system heap
uint32_t getTotalHeap()
{
    extern char __StackLimit, __bss_end__;
    return &__StackLimit - &__bss_end__;
}
//
// total space allocated from system - usually less then total heap
uint32_t getHeapArena()
{
    struct mallinfo minfo = mallinfo();
    return minfo.arena;
}
//
// free heap
uint32_t getFreeHeap()
{
    struct mallinfo minfo = mallinfo();
    return getTotalHeap() - minfo.uordblks;
}
//
// used heap
uint32_t getUsedHeap()
{
    return getTotalHeap() - getFreeHeap();
}

#if 0 
intptr_t getFlashImageStart()
{
    extern char __flash_binary_start[];
    return (intptr_t)__flash_binary_start;
}

intptr_t getFlashImageEnd()
{
    extern char __flash_binary_end[];
    return (intptr_t)__flash_binary_end;
}

size_t getFlashImageSize()
{
    extern char __flash_binary_start[];
    extern char __flash_binary_end[];

    intptr_t start = (intptr_t)__flash_binary_start;
    intptr_t end = (intptr_t)__flash_binary_end;
    return end - start;
}
#endif
