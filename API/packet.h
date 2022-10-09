#pragma once

#include "types.h"
#include <stddef.h>
#include <stdbool.h>

bool packet_send(int sockfd, const packet_t * restrict const _packet);

bool packet_recv(int sockfd, packet_t * restrict const packet);

