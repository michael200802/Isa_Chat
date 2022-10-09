#include "packet.h"

#include "socket_wrapper.c"

#include <stdlib.h>

inline bool packet_send(int sockfd, const packet_t * restrict const _packet)
{
	packet_t packet = *_packet;
	portable_integer_t len = packet.str_len;

	packet.intention = htons(packet.intention);
	packet.str_len = htonl(packet.str_len);

	if(fun_send(sockfd,(byte_t*)(void*)&packet.intention,sizeof(packet_intention_t)) == false)
	{
		return false;
	}
	if(fun_send(sockfd,(byte_t*)(void*)&packet.str_len,sizeof(portable_integer_t)) == false)
	{
		return false;
	}
	if(fun_send(sockfd,(byte_t*)(void*)packet.str,len) == false)
	{
		return false;
	}

	return true;
}

inline bool packet_recv(int sockfd, packet_t * restrict const packet)
{
        if(fun_recv(sockfd,(byte_t*)(void*)&packet->intention,sizeof(packet_intention_t)) == false)
        {
                return false;
        }
        if(fun_recv(sockfd,(byte_t*)(void*)&packet->str_len,sizeof(portable_integer_t)) == false)
        {
                return false;
        }
        packet->intention = ntohs(packet->intention);
        packet->str_len = ntohl(packet->str_len);

	portable_integer_t len = packet->str_len;
	packet->str = malloc(len);
        if(fun_recv(sockfd,(byte_t*)(void*)packet->str,len) == false)
        {
                return false;
        }

	return true;
}

