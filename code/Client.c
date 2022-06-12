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
  
  while(1){
    printf("1. 회원가입\n2. 로그인\n원하는 서비스에 맞는 숫자를 입력하세요. :");
    scanf("%d",&mode);//모드를 서버에 보내서 회원가입인지 로그인인지 판별할 수 있게 해야되는지 모르겠습니다.
    if(mode==1){
      printf("아이디를 입력하세요:");
      pthread_create(&thread_id[0], NULL, WriteToServer, (void *)&server_sockfd);
    	pthread_detach(thread_id[0]);//제가 이 함수에 대해 정확히 이해하지 못했습니다. stdin으로 입력받은 문자열이 바로 서버로 전송되는것이 맞는지,
      printf("비밀번호를 입력하세요:");//그리고 그래도 되는지 잘 모르겠지만 일단 밑에 있는 쓰레드를 이용하여 아이디와 비밀번호를 서버로 전송하는 방법을 사용했습니다.
      pthread_create(&thread_id[0], NULL, WriteToServer, (void *)&server_sockfd);
    	pthread_detach(thread_id[0]);
      /*if(!(id&&password)){                 원래는 문자열 변수를 사용하여 아이디나 패스워드가 공백인 경우 다시 입력받게 만들었는데 변수를 사용하여 서버로 문자열을 보내려면
        printf("잘못 입력하였습니다.");        함수를 많이 만들게 되어 일단 무결성 체크는 생략했습니다.
        }
       else printf("회원가입에 성공하였습니다.");
      }*/
    }
    else if(mode==2){
      printf("아이디를 입력하세요:");
      pthread_create(&thread_id[0], NULL, WriteToServer, (void *)&server_sockfd);
    	pthread_detach(thread_id[0]);
      printf("비밀번호를 입력하세요:");
      pthread_create(&thread_id[0], NULL, WriteToServer, (void *)&server_sockfd);
    	pthread_detach(thread_id[0]);
      if(){
        printf("로그인에 성공하였습니다.");
        break;
      }
      else{
        printf("로그인에 실패하였습니다.");
      }
    }
    else printf("잘못 입력하였습니다.");
  }
  

	//read and write at the same time
	pthread_create(&thread_id[0], NULL, WriteToServer, (void *)&server_sockfd);
	pthread_detach(thread_id[0]);

	ReadFromServer((void *)&server_sockfd);

	return 0;
}
