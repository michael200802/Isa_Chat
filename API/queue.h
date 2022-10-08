#pragma once

#include "types.h"

#define QUEUE_INITIALIZER {}

#include <stdbool.h>
extern bool queue_enqueue(queue_t * restrict queue, void * const elem);

extern void* queue_dequeue(queue_t * restrict queue);

//use cur_node to access the current node
#define queue_tour(queue,DO)\
	{\
		node_t * curnode = queue.head;\
		while(curnode != NULL)\
		{\
			const node_t * cur_node = curnode;\
			DO;\
			curnode = curnode->next;\
		}\
	}

#include <stddef.h>
extern void* queue_deletenode(queue_t * restrict queue, size_t pos);

extern void queue_destroy(queue_t * restrict queue);


