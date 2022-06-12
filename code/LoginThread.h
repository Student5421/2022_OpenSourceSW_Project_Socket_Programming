#include <string.h>

#include "Server_Structure.h"
#include "Server_Structure_Function.h"

#define MAX_BUF

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

BOOL SendMessageToClient(int client_fd, char *message) {
	if(write(client_fd, message, MAX_BUF) <= 0) exit(1);
	return true;
}

BOOL ReceiveMessageFromClient(int client_fd, char *message) {
	if(read(sockfd, message, MAX_BUF) <= 0) exit(1);
	return true;
}

void LoginThread(void *shared_data) {
	int client_fd = shared_data->sockfd;
	char message[MAX_BUF]; memset(message, 0x00, sizeof(message));

	while(1) {
		if(!ReceiveMessageFromClient(client_fd, message)); //get user id from client
		
		int id_exist = 0;
		//check id exist server_data array
		for (int x = 0; x < MAX_SERVER_CLINET; x++)
			if (!strcmp(message, server_data.client_array[x]->id)) id_exist = 1;

		//if exist
		if(id_exist) if(!SendMessageToClient(client_fd, "AlreadyExist"));

		//if not
		else {
			if(!SendMessageToClient(client_Fd, "CanUseId"));
			break;
		}
	}

	//make client struct and add to server data
	LPCLIENT new_client = (LPCLIENT)malloc(sizeof(CLIENT));
	new_client->fd = client_fd;
	memset(new_client->id, 0x00, sizeof(new_client->id));
	strcpy(new_client->id, message);

	AddToServerData(client_fd, message);
	
	SendMessageToClient(client_fd, "ReadyToRoomService")

	//get clinet service about room
	int room_service = 0;
	int success = 0;
	
	while(1) {
		memset(message, 0x00, sizeof(message));
		ReceiveMessageFromClient(client_fd, message)
		if(!strcmp(message, "CreateRoom")) {
			int room_num
			if(room_num = CreateRoom()) {
				room_num--;
				if(AddClientToRoom(new_client, room_num)) success = 1;
			}
		}

		else if(!strcmp(message, "EnterRoom")) {
			char temp[100]; 
			for (int x = 0; x < MAX_ROOM; x++) {
				if(server_data.room_array[x] != 0) {
					memset(temp, 0x00, sizeof(temp));
					itoa(x, temp);
					SendMessageToClient(client_fd, temp, sizeof(temp));
				}
			}
			memset(temp, 0x00, sizeof(temp));
			strcpy(temp, "Finish");
			SendMessageToClient(client_fd, temp, sizeof(temp));

			memset(temp, 0x00, sizeof(temp));
			ReceiveMessageFromClient(client_fd, temp);
			if(!strcmp(temp, "Quit")) return 0;
			int room_num = atoi(temp);

			if(AddClientToRoom(new_client, room_id)) success = 1;
			break;
		}

		if(success) break;
	}

	memset(message, 0x00, sizeof(message));
	if(!SendMessageToClient(client_fd, "ReadyToRoomService")); //if false, quit thread
}
