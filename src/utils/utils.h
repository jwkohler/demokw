#pragma once

#include <pico/stdlib.h>
#include <time.h>
#include <stdio.h>
#include <malloc.h>

#ifndef elementsof
#define elementsof(A) (sizeof(A) / sizeof(A[0]))
#endif

clock_t clock();

uint32_t getTotalHeap();
uint32_t getHeapArena();
uint32_t getFreeHeap();
uint32_t getUsedHeap();
intptr_t getFlashImageStart();
intptr_t getFlashImageEnd();
size_t getFlashImageSize();

/// @brief format current time as "HH:MM:SS.fff"
/// @returns number of bytes written into buff
size_t formatTimestamp(char *buff, size_t size);

//
// get address of reserved block of flash
// symbols __persistent_startand __persistent_end are defined in the
// custom linker script:    memmap_custom.ld
// cmake:  pico_set_linker_script(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/src/memmap_custom.ld)
//

/// @brief return starting address of reserved flash storage
/// @return
inline char *getPersistentStoreBase()
{
    extern char __persistent_start[];
    return __persistent_start;
}
/// @brief return end address of reserved flash
inline char *getPersistentStoreEnd()
{
    extern char __persistent_end[];
    return __persistent_end;
}
/// @brief return size of reserved flash storage
inline size_t getPersistentStoreSize()
{
    extern char __persistent_start[];
    extern char __persistent_end[];
    return __persistent_end - __persistent_start;
}
