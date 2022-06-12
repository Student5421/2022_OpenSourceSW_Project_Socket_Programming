#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define MAXLINE 1024

void *WriteToServer(void *fd) {
	int server_sockfd = *((int *)fd);
	char buf[MAXLINE];
	while(1) {
		memset(buf, 0x00, sizeof(buf));
		fgets(buf, sizeof(buf), stdin);

		if(write(server_sockfd, buf, MAXLINE) <= 0) {
			printf("write error\n");
			exit(1);
		}
	}
}

void *ReadFromServer(void *fd) {
	int server_sockfd = *((int *)fd);
	char buf[MAXLINE];
	while(1) {
		memset(buf, 0x00, sizeof(buf));
		if(read(server_sockfd, buf, MAXLINE) <= 0) {
			printf("read error\n");
			exit(1);
		}

		printf("from server : %s\n", buf);
	}
}

void PrintLoginService() {
    printf("
    +---------------------------------------------+\n
    |                                              \n
    |                 Login Service                \n
    |                                              \n
    +---------------------------------------------+\n
    Input your id : 
    ");
}

int main(int argc, char **argv) {
    struct sockaddr_in server_addr;
	int server_sockfd;
	int client_len;
	int num;
    int mode=0;
	pthread_t thread_id[2];

	if((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("error : ");
		exit(1);
	}

	memset(&server_addr, 0x00, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(3600);
	client_len = sizeof(server_addr);

	if(connect(server_sockfd, (struct sockaddr *)&server_addr, client_len) == -1) {
		perror("error : ");
		exit(1);
	}
    
    char check_buffer[10];

    while(1) {
        system("clear");
        char user_id[100];

        PrintLoginService();
        scanf("%s", user_id);

        if(write(server_sockfd, user_id, sizeof(user_id)) <= 0) {
		    printf("write error\nexit program");
		    exit(1);
        }

        memset(check_buffer, 0x00, sizeof(check_buffer));
        if(read(server_sockfd, check_buffer, sizeof(check_buffer)) <= 0) {
		    printf("read error\nexit program");
		    exit(1);
		}

        if(!strcmp(check_buffer, "CanUseId")) break;

        printf("already used id, restart login service\n");
        sleep(2);
    }
    
    memset(check_buffer, 0x00, sizeof(check_buffer));
    if(read(server_sockfd, check_buffer, sizeof(check_buffer)) <= 0) {
		printf("read error\nexit program");
		exit(1);
	}

    if(strcmp(check_buffer, "StartCheating")) {
        printf("something wrong in communicate with server\n
        exit program\n");
        exit(1);
    }

	//read and write at the same time
	pthread_create(&thread_id[0], NULL, WriteToServer, (void *)&server_sockfd);
	pthread_detach(thread_id[0]);

	ReadFromServer((void *)&server_sockfd);
	return 0;
}