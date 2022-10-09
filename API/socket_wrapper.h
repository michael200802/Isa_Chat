#pragma once
#include "types.h"

#include <stdbool.h>

bool fun_send(int sockfd, const byte_t * restrict buf, size_t size);

bool fun_recv(int sockfd, byte_t * restrict buf, size_t size);

bool send_number(int sockfd, portable_integer_t num);

bool recv_number(int sockfd, portable_integer_t * restrict const num);
