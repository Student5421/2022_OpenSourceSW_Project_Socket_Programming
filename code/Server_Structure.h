#pragma once

#include <stdlib.h>
#include <string.h>

#define MAX_ROOM 3
#define MAX_ROOM_CLIENT 6
#define MAX_SERVER_CLIENT 10
#define MAX_BUF 1024

typedef enum BOOL_ {
	false, true
}BOOL;

typedef struct CLIENT_ {
	int fd; //client's fd
	char id[MAX_BUF]; //client id
}CLIENT, *LPCLIENT;

typedef struct ROOM_ {
	int room_num; //room id
	int maxfd;
	fd_set readfds; //for check message from clients which are in room
	LPCLIENT client_array[MAX_ROOM_CLIENT]; //client list about clients connected to room
}ROOM, *LPROOM;

typedef struct SERVER_DATA_ {
	LPROOM room_array[MAX_ROOM]; //room list which server has
	LPCLIENT client_array[MAX_SERVER_CLIENT]; //client list about clients connected to server
}SERVER_DATA, *LPSERVER_DATA;

SERVER_DATA server_data;

pthread_mutex_t mutex[MAX_ROOM];
