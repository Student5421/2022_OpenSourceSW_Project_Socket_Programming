//about socket programing
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

//about shared memory
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

//about thread
#include <pthread.h>

#include "Server_Structure.h"
#include "Server_Structure_Function.h"
#include "LoginThread.h"

extern CLIENT_DATA client_database[MAX_DATABASE_CLINET_NUM];
extern SERVER_DATA server_data;

int main(int argc, char **argv) {
	//declare needed value
	int server_sockfd; int clinet_sockfd;
	struct sockaddr_in clinet_addr; struct sockaddr_in server_addr;
	int client_len = sizeof(client_addr);
	pthread_t thread_id;

	//initialize server_data
	InitServerData();

	//create socket and initialize
	if((server_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		perror("create socket error : ");
		exit(0);
	}

	memset(&server_addr, 0x00, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_Addr.sin_port = htons(atoi(argv[1]));

	if(bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
		perror("bind error : ");
		exit(0);
	}

	if(listen(server_sockfd, 5) == -1) {
		perror("listen error : ");
		exit(0);
	}

	//accept client connection
	while(true) {
		client_sockfd = accept(server_sockfd, (struct sockaddr *)client_addr, &client_len);
		if(client_sockfd == -1) {
			printf("accept error\n");
			continue;
		}

		//hand over client_sockfd to login thread
		pthread_create(&thread_id, NULL, LoginThread, (void *)client_fd);
		pthread_detach(thread_id);
	}

	close(server_sockfd);
	return 0;
}
