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

void LoginThread(void *client_fd) {
	int client_fd = shared_data->sockfd;
	char message[MAX_BUF]; memset(message, 0x00, sizeof(message));

	while(1) {
		if(!ReceiveMessageFromClient(client_fd, message)); //if false, quit login thread
		
		//check id exist server_data array
		int id_exist = 0; 

		//if exist
		if(id_exist) if(!SendMessageToClient(client_fd, "AlreadyExist"));

		//if not
		else {
			if(!SendMessageToClient(client_Fd, "CanUseId"));
			break;
		}
	}

	//get clinet service about room
	int room_service = 0;
	int success = 0;
	
	while(1) {
		memset(message, 0x00, sizeof(message));
		if(!ReceiveMessageFromClient(client_fd, message));
		room_service = atoi(room_service);

		switch(room_service) {
			case 1:
				//create room
				success = 1;
				break;

			case 2:
				//enter room
				success = 1;
				break;

			default:
				//nothing
		}

		if(success) break;
	}

	memset(message, 0x00, sizeof(message));
	if(!SendMessageToClient(client_fd, "ReadyToRoomService")); //if false, quit thread
}
