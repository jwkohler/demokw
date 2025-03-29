
#if ENABLE_ALLOCATION_OVERRIDES

#include <cstdlib>
#include "pico.h"

void *operator new(std::size_t n) { return std::malloc(n); }
void *operator new[](std::size_t n) { return std::malloc(n); }
void operator delete(void *p) { std::free(p); }
void operator delete[](void *p) noexcept { std::free(p); }

#if __cpp_sized_allocation
void operator delete(void *p, __unused std::size_t n) noexcept { std::free(p); }
void operator delete[](void *p, __unused std::size_t n) noexcept { std::free(p); }
#endif
#endif
