#pragma once

#include <stdint.h>

typedef char byte_t;

typedef uint32_t portable_integer_t;

typedef enum packet_intention{PI_MSG,PI_NEWMEMBER,PI_ACCEPTMEMBER}packet_intention_t;

typedef struct
{
	packet_intention_t intention;
	portable_integer_t str_len;
	char * str;
}packet_t;

typedef struct __node
{
	void * elem;
	struct __node * next;
}node_t;

typedef struct
{
	node_t * head, * tail;
}queue_t;
