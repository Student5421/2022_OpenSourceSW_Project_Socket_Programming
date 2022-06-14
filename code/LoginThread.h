#pragma once

#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "Server_Structure.h"
#include "Server_Structure_Function.h"
#include "CommunicateInRoomThread.h"

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
	if(read(client_fd, message, MAX_BUF) <= 0) exit(1);
	return true;
}

void LoginThread(void *shared_data) {
	int client_fd = *(int *)shared_data;
	char message[MAX_BUF];
	memset(message, 0x00, MAX_BUF);

	//make client struct and add to server data
	LPCLIENT new_client = (LPCLIENT)malloc(sizeof(CLIENT));
	new_client->fd = client_fd;
	while(1) {
		if(!ReceiveMessageFromClient(client_fd, message)); //get user id from client
		int id_exist = 0;
		//check id exist server_data array maybe error in here
		for (int x = 0; x < MAX_SERVER_CLIENT; x++){
			if(server_data.client_array[x] == NULL) continue;
			if (!strcmp(message, server_data.client_array[x]->id)) id_exist = 1;
		}

		//if exist
		if(id_exist) {
			memset(message, 0x00, MAX_BUF);
			strcpy(message, "AlreadyExist");
			if(!SendMessageToClient(client_fd, message));
		}

		//if not
		else {
			memset(new_client->id, 0x00, sizeof(new_client->id));
			strcpy(new_client->id, message);

			memset(message, 0x00, MAX_BUF);
			strcpy(message, "CanUseId");
			if(!SendMessageToClient(client_fd, message));
			break;
		}
	}

	AddToServerData(client_fd, new_client->id);

	memset(message, 0x00, MAX_BUF);
	strcpy(message, "ReadyToRoomService");
	SendMessageToClient(client_fd, message);

	//get clinet service about room
	int room_service = 0;
	int success = 0;

	while(1) {
		memset(message, 0x00, sizeof(message));
		ReceiveMessageFromClient(client_fd, message);
		if(!strcmp(message, "CreateRoom")) {
			int room_num = -1;
			if(room_num = CreateRoom()) {
				pthread_t thread_id;
				pthread_create(&thread_id, NULL, CommunicateWithClient, (void *)&room_num);
				pthread_detach(thread_id);
				if(AddClientToRoom(new_client, room_num)) success = 1;
				sleep(1);
				memset(message, 0x00, MAX_BUF);
				itoa(room_num, message);
				SendMessageToClient(client_fd, message);	
			}

			else {
				memset(message, 0x00, MAX_BUF);
				strcpy(message, "TooManyRoom");
				SendMessageToClient(client_fd, message);
			}
		}

		else if(!strcmp(message, "EnterRoom")) {
			char temp[MAX_BUF]; 
			//error
			for (int x = 0; x < MAX_ROOM; x++) {
				if(server_data.room_array[x] != NULL) {
					memset(temp, 0x00, MAX_BUF);
					itoa(x + 1, temp);
					SendMessageToClient(client_fd, temp);
				}
			}
			memset(temp, 0x00, sizeof(temp));	
			strcpy(temp, "Finish");
			SendMessageToClient(client_fd, temp);

			memset(temp, 0x00, sizeof(temp));
			ReceiveMessageFromClient(client_fd, temp);
			if(!strcmp(temp, "Quit")) exit(0);
			int room_num = atoi(temp);

			if(AddClientToRoom(new_client, room_num))
				success = 1;

			memset(message, 0x00, MAX_BUF);
			strcpy(message, "EnterRoom");
			SendMessageToClient(client_fd, message);
			break;
		}

		if(success) break;
	}

	memset(message, 0x00, sizeof(message)); strcpy(message, "ReadyToStartChat");
	if(!SendMessageToClient(client_fd, message)); //if false, quit thread
}
