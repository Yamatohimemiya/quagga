//MemoryNew.c: New memory allocation implementation
//License: GPL v2
//(C)2025 Hikaru Yamatohimemiya

#include <zebra.h>
#include <MemoryNew.h>

#include "log.h"

#if !defined(HAVE_STDLIB_H) || (defined(GNU_LINUX) && defined(HAVE_MALLINFO))
	#include <malloc.h>
#endif /* !HAVE_STDLIB_H || HAVE_MALLINFO */


static struct MemoryPoolKey *DefaultPool;


struct MemoryPoolKey* MemoryPoolCreate(){
	struct MemoryPoolKey *MPK = malloc(sizeof(struct MemoryPoolKey));
	if(!MPK){
		zlog_err("Failed to allocate memory: %s", strerror(errno));
		exit(0);
	}

	MPK->AllocatedList = list_new();
	return MPK;
}

void* MemoryPoolAlloc(struct MemoryPoolKey *MPK, uintptr_t Size){
	void *R = malloc(Size);

	if(!R){
		zlog_err("Failed to allocate memory: %s", strerror(errno));
		exit(0);
	}

	listnode_add(MPK->AllocatedList, R);
	return R;
}

void* MemoryPoolClearAlloc(struct MemoryPoolKey *MPK, uintptr_t Size){
	void *R = MemoryPoolAlloc(MPK, Size);
	memset(R, 0, Size);
	return R;
}

void* MemoryPoolRealloc(struct MemoryPoolKey *MPK, void* OldMemory, uintptr_t Size){
	listnode_delete(MPK->AllocatedList, OldMemory);
	void *R = realloc(OldMemory, Size);
	if(!R){
		zlog_err("Failed to allocate memory: %s", strerror(errno));
		exit(0);
	}

	listnode_add(MPK->AllocatedList, R);
	return R;
}

void MemoryPoolFree(struct MemoryPoolKey *MPK, void* Memory){
	void *m = 0;
	struct listnode *node, *nnode;

	if(MPK && Memory){
		for(ALL_LIST_ELEMENTS(MPK->AllocatedList, node, nnode, m)) {
			if(m && m == Memory){
				free(m);
				listnode_delete(MPK->AllocatedList, Memory);
				return;
			}
		}

		zlog_err("MemoryPoolFree: Memory '%08X' is not in specified pool '%08X'", Memory, MPK);
		zlog_err("MemoryPoolFree: Already released or wrong pool", Memory, MPK);
		exit(0);
	}

	zlog_err("MemoryPoolFree: NULL detected (MPK = %08X, Memory = %08X)", MPK, Memory);
	exit(0);
}

void MemoryPoolClear(struct MemoryPoolKey *MPK){
	void *m = 0;
	struct listnode *node, *nnode;

	for(ALL_LIST_ELEMENTS(MPK->AllocatedList, node, nnode, m)) {
		if(m){
			free(m);
		}
	}

	list_free(MPK->AllocatedList);
	MPK->AllocatedList = list_new();

	return;
}

void MemoryPoolDestroy(struct MemoryPoolKey *MPK){
	MemoryPoolClear(MPK);
	//If MPK == NULL, exit and do not reach here
	free(MPK);
	return;
}

char* MemoryPoolDuplicateString(struct MemoryPoolKey *MPK, const char* String){
	sleep(1);
	if(String){
		size_t l = strlen(String);
		char* R = MemoryPoolAlloc(MPK, l);
		memcpy(R, String, l);
		return R;
	}
	zlog_err("MemoryPoolDuplicateString: NULL detected (MPK = %08X, String = %08X)", MPK, String);
	exit(0);
}

void* MemoryAlloc(uintptr_t Size){
	if(!DefaultPool){ DefaultPool = MemoryPoolCreate(); }
	return MemoryPoolAlloc(DefaultPool, Size);
}

void* MemoryClearAlloc(uintptr_t Size){
	if(!DefaultPool){ DefaultPool = MemoryPoolCreate(); }
	return MemoryPoolClearAlloc(DefaultPool, Size);
}

void* MemoryRealloc(void* OldMemory, uintptr_t Size){
	if(!DefaultPool){ DefaultPool = MemoryPoolCreate(); }
	return MemoryPoolRealloc(DefaultPool, OldMemory, Size);
}

void MemoryFree(void* Memory){
	if(!DefaultPool){ DefaultPool = MemoryPoolCreate(); }
	return MemoryPoolFree(DefaultPool, Memory);
}
void MemoryClear(){
	if(!DefaultPool){ DefaultPool = MemoryPoolCreate(); }
	return MemoryPoolClear(DefaultPool);
}
char* MemoryDuplicateString(const char* String){
	if(!DefaultPool){ DefaultPool = MemoryPoolCreate(); }
	return MemoryPoolDuplicateString(DefaultPool, String);
}
