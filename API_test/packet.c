#include "publicip.h"

#include "../API/packet.c"
#include "../API/queue.c"

#include <stdio.h>
#include <unistd.h>

#include <pthread.h>
#include <semaphore.h>

#define PORT 8082

char ** str_arr;

void* server_routine(void* arg)
{
	sem_t * sem = arg;
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in sockaddr = {.sin_family = AF_INET, .sin_port = htons(PORT), .sin_addr = {.s_addr = INADDR_ANY}};
	if(bind(sockfd,(struct sockaddr*)&sockaddr,sizeof(sockaddr)) == -1)
	{
		exit(-1);
	}
	if(listen(sockfd,100) == -1)
	{
		exit(-1);
	}

	sem_post(sem);

	int client_sockfd = accept(sockfd,NULL,NULL);

	portable_integer_t n_msg;
	if(recv_number(client_sockfd, &n_msg) == false)
	{
		exit(-1);
	}

	queue_t queue = QUEUE_INITIALIZER;
	for(size_t i = 0; i < n_msg; i++)
	{
		packet_t * packet = malloc(sizeof(packet_t));
		if(packet_recv(client_sockfd,packet) == false)
		{
			exit(-1);
		}
		queue_enqueue(&queue,packet);
	}

	if(send_number(client_sockfd, n_msg) == false)
	{
		exit(-1);
	}
	packet_t * packet;
	while((packet = queue_dequeue(&queue)) != NULL)
	{
		if(packet_send(client_sockfd,packet) == false)
		{
			exit(-1);
		}
		free(packet);
	}

	sem_wait(sem);

	close(sockfd);
	close(client_sockfd);
	return NULL;
}

void* client_routine(void* arg)
{
	sem_t * sem = arg;
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in sockaddr = {.sin_family = AF_INET, .sin_port = htons(PORT), .sin_addr = inet_addr(PUBLIC_IP)};

	sem_wait(sem);

	if(connect(sockfd,(struct sockaddr*)&sockaddr,sizeof(sockaddr)) == -1)
	{
		exit(-1);
	}

	size_t n_str = 0;
	while(str_arr[n_str] != NULL)
	{
		n_str++;
	}
	if(send_number(sockfd,n_str) == false)
	{
		exit(-1);
	}
	while(*str_arr != NULL)
	{
		char * str = *str_arr;
		size_t len = 0;
		while(str[len++] != '\0');
		packet_t packet = {.intention = PI_MSG, .str_len = len, .str = str};
		if(packet_send(sockfd,&packet) == false)
		{
			exit(-1);
		}
		str_arr++;
	}

	portable_integer_t n_msg;
	if(recv_number(sockfd, &n_msg) == false)
	{
		exit(-1);
	}
        while(n_msg != 0)
        {
                packet_t packet;
                if(packet_recv(sockfd,&packet) == false)
                {
                        exit(-1);
                }
		printf("intention: %d, str_len: %zu, str: \"%s\"\n",packet.intention,packet.str_len,packet.str);
                n_msg--;
        }

	sem_post(sem);

	close(sockfd);
	return NULL;
}

void ending(void)
{
	perror("exit(-1); says nyan");
}

int main(int argc, char ** argv)
{
	atexit(ending);

	str_arr = argv;

	sem_t sem;
	sem_init(&sem,0,0);

	pthread_t thr_server, thr_client;

	pthread_create(&thr_server,NULL,server_routine,&sem);
	pthread_create(&thr_client,NULL,client_routine,&sem);
	pthread_join(thr_server,NULL);
	pthread_join(thr_client,NULL);

	sem_destroy(&sem);
	return 0;
}

