#include "../API/queue.c"

#include <stdio.h>

#include <string.h>

int main(int argc, char ** argv)
{
	queue_t queue = QUEUE_INITIALIZER;
	while(*argv != NULL)
	{
		queue_enqueue(&queue,*argv);
		argv++;
	}

	queue_tour(
		queue,
		{
			puts((const char *)cur_node->elem);
		}
	);
	while(1)
	{
		int pos = 0;
		queue_tour(
			queue,
			{
				if(strcmp((const char*)cur_node->elem,"nya") == 0)
				{
					pos *= -1;
					break;
				}
				pos++;
			}
		);
		if(pos < 0)
		{
			queue_deletenode(&queue,pos*-1);
		}
		else
		{
			break;
		}
	}
	queue_tour(
		queue,
		{
			puts((const char *)cur_node->elem);
		}
	);
	void*elem;
	while((elem = queue_dequeue(&queue)) != NULL)
	{
		puts((const char*)elem);
	}
	queue_destroy(&queue);
	puts("chau mijos");
	return 0;
}
