#include <stdlib.h>
#include <string.h>

#define MAX_ROOM 3
#define MAX_ROOM_CLINET 6
#define MAX_SERVER_CLIENT 10
#define MAX_DATABASE_CLIENT 20

typedef enum BOOL_ {
	false, true
}BOOL;

typedef struct CLIENT_ {
	int fd; //client's fd
	int id[20]; //client id
}CLIENT, *LPCLIENT;

typedef structure ROOM_ {
	int room_num; //room id
	int maxfd;
	fd_set readfds; //for check message from clients which are in room
	LPCLIENT client_array[MAX_ROOM_CLINET]; //client list about clients connected to room
}ROOM, *LPROOM;

typedef struct SERVER_DATA_ {
	LPROOM room_array[MAX_ROOM]; //room list which server has
	LPCLIENT client_array[MAX_SERVER_CLIENT] //client list about clients connected to server
}SERVER_DATA, *LPSERVER_DATA;

typedef struct CLIENT_DATA_ {
	char id[20];
	char passwd[20];
}CLIENT_DATA, *LPCLIENT_DATA;

CLIENT_DATA client_database[MAX_DATABASE_CLIENT];
SERVER_DATA server_data;
