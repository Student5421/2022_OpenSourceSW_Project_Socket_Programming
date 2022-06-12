#include <string.h>

#include "Server_Structure.h"
#include "Server_Structure_Function.h"

#define MAX_BUF

BOOL SendMessageToClient(int client_fd, char *message) {
	if(write(client_fd, message, MAX_BUF) <= 0) return false;
	return true;
}

BOOL ReceiveMessageFromClient(int client_fd, char *message) {
	if(read(sockfd, message, MAX_BUF) <= 0) return false;
	return true;
}

void LoginThread(void *shared_data) {
	int client_fd = shared_data->sockfd;
	char message[MAX_BUF]; memset(message, 0x00, sizeof(message));

	while(1) {
		if(!ReceiveMessageFromClient(client_fd, message)); //if false, quit login thread
		
		int id_exist = 0;
		//check id exist server_data array
		for (int x = 0; x < MAX_SERVER_CLINET; x++) {
			if (!strcmp(message, server_data.client_array[x]->id)) {
				id_exist = 1;
			}
		}
		else id_exist = 0;

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
	memset(new_client->id, 0x00, sizeof(new_client->id);
	strcpy(new_client->id, message);

	AddToServerData(client_fd, message);

	//get clinet service about room
	int room_service = 0;
	int success = 0;
	
	while(1) {
		memset(message, 0x00, sizeof(message));
		if(!ReceiveMessageFromClient(client_fd, message));
		room_service = atoi(room_service);

		switch(room_service) {
			case 1:
				int room_num
				if(room_num = CreateRoom()) success = 1;
				room_num--;
				if(AddClientToRoom(new_client, room_num)) success = 1;
				break;

			case 2:
				for (int x = 0; x < MAX_ROOM; x++) {
					
				}


				if(AddClientToRoom(new_client, room_id)) success = 1;
				break;
		}

		if(success) break;
	}

	memset(message, 0x00, sizeof(message));
	if(!SendMessageToClient(client_fd, "ReadyToRoomService")); //if false, quit thread
}
