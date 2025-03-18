//MemoryNew.h: New memory allocation implementation
//License: GPL v2
//(C)2025 Hikaru Yamatohimemiya

#if !defined(EVENTNEW_H)
#define EVENTNEW_H

#include <stdint.h>

#include "thread.h"
#include "linklist.h"
#include "MemoryNew.h"

#define EVENT_TYPE_NEXT 1
#define EVENT_TYPE_TIMER 2

struct EventHandler {
	struct MemoryPoolKey *MPK;
	struct list *EventList;
};

struct EventKey{
	uint32_t Type;
	time_t Timer;
	int(*Func)(struct EventKey *t);
	void* arg; //Compatible for THREAD_ARG
};

struct EventHandler* EventInitialize();
void EventRunLoop(struct EventHandler *, struct thread_master *);
struct EventKey* EventAdd(struct EventHandler *, int(*)(struct EventKey *), void*);
struct EventKey* EventAddTimer(struct EventHandler *, int(*Func)(struct EventKey *), void*, time_t);
void EventCancel(struct EventHandler *, struct EventKey *);

#endif //EVENTNEW_H
