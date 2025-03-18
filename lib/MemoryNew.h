//MemoryNew.h: New memory allocation implementation
//License: GPL v2
//(C)2025 Hikaru Yamatohimemiya

#if !defined(MEMORYNEW_H)
#define MEMORYNEW_H

#include <stdint.h>

#include "linklist.h"

struct MemoryPoolKey {
	struct list *AllocatedList;
};

#define MPALLOC(MPK, SIZE) MemoryPoolAlloc(MPK, SIZE);
#define MPCALLOC(MPK, SIZE) MemoryPoolClearAlloc(MPK, SIZE);
#define MPREALLOC(MPK, OLD, SIZE) MemoryPoolRealloc(MPK, OLD, SIZE);
#define MPSTRDUP(MPK, STR) MemoryPoolDuplicateString(MPK, STR);
#define MPFREE(MPK, MEM) MemoryPoolFree(MPK, MEM); MEM = NULL;

#define MemoryGetArraySize(ar) (sizeof(ar) / sizeof(ar[0]))

struct MemoryPoolKey* MemoryPoolCreate();
void* MemoryPoolAlloc(struct MemoryPoolKey *MPK, uintptr_t Size);
void* MemoryPoolClearAlloc(struct MemoryPoolKey *MPK, uintptr_t Size);
void* MemoryPoolRealloc(struct MemoryPoolKey *MPK, void* OldMemory, uintptr_t Size);
void MemoryPoolFree(struct MemoryPoolKey *MPK, void* Memory);
void MemoryPoolClear(struct MemoryPoolKey *MPK);
void MemoryPoolDestroy(struct MemoryPoolKey *MPK);
char* MemoryPoolDuplicateString(struct MemoryPoolKey *MPK, const char* String);

void* MemoryAlloc(uintptr_t Size);
void* MemoryClearAlloc(uintptr_t Size);
void* MemoryRealloc(void* OldMemory, uintptr_t Size);
void MemoryFree(void* Memory);
void MemoryClear();
char* MemoryDuplicateString(const char* String);

#endif //MEMORYNEW_H
