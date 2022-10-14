#include "publicip.h"

#include "../API/packet.c"
#include "../API/queue.c"

#include "../config.h"

#include <stdio.h>
#include <unistd.h>

#include <string.h>

int main(int argc, char ** argv)
{
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in sockaddr = {
		.sin_family = AF_INET,
		.sin_port = htons(ISACHAT_PORT),
		.sin_addr = inet_addr(PUBLIC_IP)
	};
	if(connect(sockfd,(struct sockaddr*)&sockaddr,sizeof(sockaddr)) == -1)
	{
		perror("connect failed");
		return -1;
	}
	portable_integer_t n_packs = argc;
	packet_t pack = {
			.intention = PI_MSG,
		};
	send_number(sockfd,n_packs);
	while(*argv != NULL)
	{
		pack.str_len = strlen(*argv)+1;
		pack.str = *argv;

		packet_send(sockfd,&pack);

		argv++;
	}
	close(sockfd);
	return 0;
}
