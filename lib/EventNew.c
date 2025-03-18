#include <zebra.h>
#include <EventNew.h>

struct thread *thread_fetch_ex(struct thread_master *, int);
void thread_call(struct thread *);

struct EventHandler* EventInitialize(){
	struct EventHandler *EH = malloc(sizeof(struct EventHandler));
	EH->MPK = MemoryPoolCreate();
	EH->EventList = list_new();
	return EH;
}

void EventRunLoop(struct EventHandler *EH, struct thread_master *m){
	struct EventKey *EK;
	struct listnode *node, *nnode;
	struct timeval tv;
	time_t LastTV = 0;
	time_t TimeElapsed = 0;;

	while(1){
		gettimeofday(&tv, NULL);
		if(LastTV){
			TimeElapsed = tv.tv_sec - LastTV;
			LastTV = tv.tv_sec;
		} else {
			TimeElapsed = 0;
			LastTV = tv.tv_sec;
		}
		if(TimeElapsed < 0){
			TimeElapsed = 0;
		}

		for(ALL_LIST_ELEMENTS(EH->EventList, node, nnode, EK)) {
			if(EK){
				switch(EK->Type){
					case EVENT_TYPE_NEXT:
						EK->Func(EK);
						listnode_delete(EH->EventList, EK);
					break;

					case EVENT_TYPE_TIMER:
						if(EK->Timer){
							EK->Timer -= TimeElapsed;
						}
						if(EK->Timer <= 0){
							EK->Func(EK);
							listnode_delete(EH->EventList, EK);
						}
					break;

					default:
						printf("Unknown event type!");
						exit(0);
					break;
				}


				continue;
			}
		}

		struct thread *legacy_thread;
		while((legacy_thread = thread_fetch_ex(m, 1))) {
			thread_call(legacy_thread);
		}

		usleep(1000 * 100);
	}
}

struct EventKey* EventAdd(struct EventHandler *EH, int(*Func)(struct EventKey *t), void* Arg){
	struct EventKey *EK = MPALLOC(EH->MPK, sizeof(struct EventKey));
	EK->Type = EVENT_TYPE_NEXT;
	EK->Func = Func;
	EK->arg = Arg;
	listnode_add(EH->EventList, EK);
	return EK;
}

struct EventKey* EventAddTimer(struct EventHandler *EH, int(*Func)(struct EventKey *t), void* Arg, time_t Timer){
	struct EventKey *EK = MPALLOC(EH->MPK, sizeof(struct EventKey));
	EK->Type = EVENT_TYPE_TIMER;
	EK->Func = Func;
	EK->arg = Arg;
	EK->Timer = Timer;
	listnode_add(EH->EventList, EK);
	return EK;
}

void EventCancel(struct EventHandler *EH, struct EventKey *DeleteEvent){
	struct EventKey *EK;
	struct listnode *node, *nnode;

	for(ALL_LIST_ELEMENTS(EH->EventList, node, nnode, EK)) {
		if(EK && EK == DeleteEvent){
			listnode_delete(EH->EventList, EK);
			return;
		}
	}
	return;
}
