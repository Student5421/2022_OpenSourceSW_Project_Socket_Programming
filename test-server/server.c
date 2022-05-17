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

#include <pthread.h>

#define MAXLINE 1024

fd_set readfds;
int maxfd;
int client_fd_list[10];

void *CommunicateWithClient() {
	char buf[MAXLINE];
	while(1) {
		fd_set allfds = readfds;
		memset(buf, 0x00,sizeof(buf));
		int fd_num = select(maxfd + 1, &allfds, (fd_set *)0, (fd_set *)0, NULL);
		printf("fd num : %d\n", fd_num);
		for(int x = 0 ; x < maxfd + 1 ; x++) {
			int sockfd = x;
			if(FD_ISSET(sockfd, &allfds)) {
				if(read(sockfd, buf, MAXLINE) <= 0) {
					close(sockfd);
					FD_CLR(sockfd, &readfds);
					for(int y = 0 ; y < 10 ; y++)
						if(client_fd_list[y] == sockfd) {
							client_fd_list[y] = -1;
							break;
						}
				}

				else {
					if(!strncmp(buf, "quit\n", 5)) {
						close(sockfd);
						FD_CLR(sockfd, &readfds);
						for(int y = 0 ; y < 10 ; y++)
							if(client_fd_list[y] == sockfd) {
								client_fd_list[y] = -1;
								break;
							}
					}

					else {
						printf("Read from %dfd : %s\n", sockfd, buf);
						for(int y = 0 ; y < 10 ; y++)
							if(client_fd_list[y] != -1)
								write(client_fd_list[y], buf, strlen(buf));
					}

					if(--fd_num <= 0) break;
				} //end else
			} //end if
		} //end for
	} //end while
}

int main(int argc, char **argv) {
	int server_sockfd; int client_sockfd;
	struct sockaddr_in client_addr; struct sockaddr_in server_addr;
	int client_len = sizeof(client_addr);
	pthread_t thread_id;

	//create socket and initialize
	if((server_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		perror("create socket error : ");
		exit(0);
	}

	memset(&server_addr, 0x00, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[1]));

	if(bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
		perror("bind error : ");
		exit(0);
	}

	if(listen(server_sockfd, 5) == -1) {
		perror("listen error : ");
		exit(0);
	}

	FD_ZERO(&readfds);
	memset(client_fd_list, -1, sizeof(client_fd_list));

	int max_fd = 0;
	int is_first_client = 1;
	while(1) {
		client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_addr, &client_len);
		if(client_sockfd == -1) {
			printf("accept error\n");
			continue;
		}

		FD_SET(client_sockfd, &readfds);
		for(int x = 0 ; x < 10 ; x++)
			if(client_fd_list[x] == -1) {
				client_fd_list[x] = client_sockfd;
				break;
			}

		if(client_sockfd > maxfd) {
			printf("Accept new client fd : %d\n", client_sockfd);
			maxfd = client_sockfd;
		}

		if(is_first_client) {
			is_first_client = 0;
			pthread_create(&thread_id, NULL, CommunicateWithClient, NULL);
			pthread_detach(thread_id);
		}
	}

	close(server_sockfd);
	return 0;
}
