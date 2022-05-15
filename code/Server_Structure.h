#include <stdlib.h>
#include <string.h>

#define MAX_ROOM 3
#define MAX_CURRENT_CLIENT 6
#define MAX_DATABASE_CLIENT_NUM 20

typedef enum BOOL_ {
	false, true
}BOOL;

typedef struct CLIENT_NODE_ {
	int fd;
	int id[20];
	struct CLIENT_NODE *next;
}CLIENT_NODE, *LPCLIENT_NODE;

typedef structure ROOM_NODE_ {
	int room_num;
	LPCLIENT_NODE client_list;
	struct ROOM_NODE_ *next;
}ROOM_NODE, *LPROOM_NODE;

typedef struct SERVER_DATA_ {
	LPROOM_NODE room_list;
	LPCLIENT_NODE client_list;
}SERVER_DATA, *LPSERVER_DATA;

typedef struct CLIENT_DATA_ {
	char id[20];
	char passwd[20];
}CLIENT_DATA, *LPCLIENT_DATA;

CLIENT_DATA client_database[MAX_DATABASE_CLIENT_NUM];
SERVER_DATA server_data;
