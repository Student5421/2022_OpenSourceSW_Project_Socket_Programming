#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_BUF 1024

void itoa(int num, char *str){
    int i=0;
    int radix = 10;
    int deg=1;
    int cnt = 0;

    while(1){
        if( (num/deg) > 0)
            cnt++;
        else
            break;
        deg *= radix;
    }
    deg /=radix; 
    for(i=0; i<cnt; i++)    {
        *(str+i) = num/deg + '0';
        num -= ((num/deg) * deg);
        deg /=radix;
    }
    *(str+i) = '\0';
} 

void *WriteToServer(void *fd) {
	int server_sockfd = *((int *)fd);
	char buf[MAX_BUF];
	while(1) {
		memset(buf, 0x00, sizeof(buf));
		fgets(buf, sizeof(buf), stdin);

		if(write(server_sockfd, buf, MAX_BUF) <= 0) {
			printf("write error\n");
			exit(1);
		}
	}
}

void *ReadFromServer(void *fd) {
	int server_sockfd = *((int *)fd);
	char buf[MAX_BUF];
	while(1) {
		memset(buf, 0x00, sizeof(buf));
		if(read(server_sockfd, buf, MAX_BUF) <= 0) {
			printf("read error\n");
			exit(1);
		}

		printf("%s", buf);
	}
}

void PrintLoginService() {
    printf(
    "+---------------------------------------------+\n"
    "|                                              \n"
    "|                 Login Service                \n"
    "|                                              \n"
    "+---------------------------------------------+\n"
    "Input your id : ");
}

void PrintRoomService() {
    printf(
    "+--------------------------------------------+\n"
    "|                                             \n"
    "|                 Room Service                \n"
    "|         select service what you want        \n"
    "|               1. create room                \n"
    "|               2. enter room                 \n"
    "|                                             \n"
    "+--------------------------------------------+\n"
    "Input service number : ");
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
    
    char message[MAX_BUF];

    while(1) {
        system("clear");
        char user_id[100];

        PrintLoginService();
        scanf("%s", user_id);
		while(getchar() != '\n');

        if(write(server_sockfd, user_id, sizeof(user_id)) <= 0) {
		    printf("write error\nexit program\n");
		    exit(1);
        }

        memset(message, 0x00, MAX_BUF);
        if(read(server_sockfd, message, MAX_BUF) <= 0) {
		    printf("CanUserId read error\nexit program\n");
		    exit(1);
	}

        if(!strcmp(message, "CanUseId")) break;

        printf("already used id, restart login service\n");
        sleep(2);
    }
    
    memset(message, 0x00, MAX_BUF);
    if(read(server_sockfd, message, MAX_BUF) <= 0) {
		printf("ReadyToRoomService read error\nexit program\n");
		exit(1);
    }
    printf("check_buffer : %s\n", message);

    if(strcmp(message, "ReadyToRoomService")) {
        printf("error to enter room\nexit program\n");
	exit(1);
    }

	//enter the room
    int room_service = 0;
    while(1) {
        system("clear");
        PrintRoomService();
        scanf("%d", &room_service);
        while(getchar() != '\n');

        switch(room_service) {
            case 1: //create room
		        memset(message, 0x00, MAX_BUF); strcpy(message, "CreateRoom");
                if(write(server_sockfd, message, MAX_BUF) <= 0) {
		            printf("write error\nexit program\n");
		            exit(1);
                }
                break;

            case 2:
		        memset(message, 0x00, MAX_BUF); strcpy(message, "EnterRoom");
                if(write(server_sockfd, message, MAX_BUF) <= 0) {
		            printf("write error\nexit program\n");
		            exit(1);
                }

                printf("\nRoom List\n");
                while(1) {
                    memset(message, 0x00, MAX_BUF);
                    if(read(server_sockfd, message, MAX_BUF) <= 0) {
                        printf("Room List read error\nexit program\n");
                        exit(1);
                    }

                    printf("message : %s\n", message);

                    int room_num = -1;
                    if(strcmp(message, "Finish")) room_num = atoi(message);
                    if(room_num != -1) printf("[%d] room\n", room_num);
                    else break;
                }

                int room_num = -1;
                printf("\ninput room number, if you wan't to exit, enter -1\n");
                printf("input : ");
                scanf("%d", &room_num);

                if(room_num == -1) {
			        memset(message, 0x00, MAX_BUF); strcpy(message, "QUIT");
			        write(server_sockfd, message, MAX_BUF);
                    printf("exit program\n");
                    exit(0);
                }

                memset(message, 0x00, MAX_BUF);
                itoa(room_num, message);
                if(write(server_sockfd, message, MAX_BUF) <= 0) {
		            printf("write error\nexit program\n");
		            exit(1);
                }

                break;
        }

		memset(message, 0x00, MAX_BUF);
		if(read(server_sockfd, message, MAX_BUF) <= 0) {
			printf("ReadyToRoomService read error\nexit program\n");
			exit(1);
		}

		if(!strcmp(message, "ReadyToStartChat")) break;
    }

	system("clear");
	printf("+---------------------------+\n"
	       "|                           |\n"
	       "|        Chating Room       |\n"
	       "|                           |\n"
               "+---------------------------+\n");

	//read and write at the same time
	pthread_create(&thread_id[0], NULL, WriteToServer, (void *)&server_sockfd);
	pthread_detach(thread_id[0]);

	ReadFromServer((void *)&server_sockfd);
	return 0;
}
