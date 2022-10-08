#include "queue.h"

#include <stdlib.h>

inline bool queue_enqueue(queue_t * restrict queue, void * const elem)
{
	node_t * newnode = malloc(sizeof(node_t));
	if(newnode == NULL)
	{
		return false;
	}
	newnode->elem = elem;
	newnode->next = NULL;

	if(queue->head == NULL)
	{
		queue->head = queue->tail = newnode;
	}
	else
	{
		queue->tail->next = newnode;
		queue->tail = queue->tail->next;
	}

	return true;
}

inline void* queue_dequeue(queue_t * restrict queue)
{
	if(queue->head == NULL)
	{
		return NULL;
	}

	node_t * oldhead = queue->head;
	void * elem = oldhead->elem;
	queue->head = queue->head->next;
	free(oldhead);
	return elem;
}

inline void* queue_deletenode(queue_t * restrict queue, size_t pos)
{
	if(pos == 0)
	{
		return queue_dequeue(queue);
	}
	pos--;

	node_t * curnode = queue->head;
	while(curnode != NULL && pos != 0)
	{
		curnode = curnode->next;
		pos--;
	}

	if(curnode == NULL || curnode->next == NULL)
	{
		return NULL;
	}
	node_t * node = curnode->next;
	void * elem = node->elem;
	curnode->next = node->next;
	free(node);
	return elem;
}

inline void queue_destroy(queue_t * restrict queue)
{
	node_t * curnode = queue->head, * nextnode;
	while(curnode != NULL)
	{
		nextnode = curnode->next;
		free(curnode);
		curnode = nextnode;
	}
}
