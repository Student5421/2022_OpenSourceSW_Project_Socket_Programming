#pragma once

#include <string.h>
#include <stdlib.h>

#include "Server_Structure.h"

extern SERVER_DATA server_data;
extern pthread_mutex_t mutex[MAX_ROOM];

void InitServerData() {
	memset(&server_data, 0x00, sizeof(SERVER_DATA));
	for(int x = 0 ; x < MAX_ROOM ; x++) pthread_mutex_init(&(mutex[x]),NULL);
}

/* Function of Room & room_list */

int CreateRoom() {
	//create new room
	LPROOM new_room = (LPROOM)malloc(sizeof(ROOM));
	memset(new_room, 0x00, sizeof(ROOM));
	
	//add new room to room_list
	int x = 0;
	for(x ; x < MAX_ROOM ; x++) 
		if(server_data.room_array[x] == NULL) {
			new_room->room_num = x;
			server_data.room_array[x] = new_room;
			//init fd set
			FD_ZERO(&(new_room->readfds));
			break;
		}

	if(x == MAX_ROOM) return false;	
	return x + 1;
}

LPROOM GetRoomByRoomNum(int room_num) {
	LPROOM cur_room = server_data.room_array[room_num];
	
	for(int x = 0 ; x < MAX_ROOM ; x++) {
		if(server_data.room_array[x] == NULL) continue;
		if(server_data.room_array[x]->room_num == room_num) return server_data.room_array[x];
	}

	return false;
}

/* Function of client & client_list */

//add client to room
BOOL AddClientToRoom(LPCLIENT client, int room_num) {
	LPROOM cur_room = NULL;
	if((cur_room = GetRoomByRoomNum(room_num)) == false) return false;

	int x = 0;
	for(x; x < MAX_ROOM_CLINET ; x++)
		if(cur_room->client_array[x] == NULL) {
			//critical section
			pthread_mutex_lock(&(mutex[x]));
			cur_room->client_array[x] = client;

			//update fd set
			FD_SET(client->fd, &(cur_room->readfds));
			if(client->fd > cur_room->maxfd) cur_room->maxfd = client->fd;

			//end of critical section
			pthread_mutex_unlock(&(mutex[x]));
			break;
		}

	if(x == MAX_ROOM_CLINET) return false;
	return true;
}

//delete client from room
BOOL DeleteClientFromRoom(int client_fd, int room_num) {
	LPROOM cur_room = NULL;
	if((cur_room = GetRoomByRoomNum(room_num)) == false) return false;

	int x = 0;
	for(x; x < MAX_ROOM_CLINET ; x++)
		if(cur_room->client_array[x] != NULL)
			if(cur_room->client_array[x]->fd == client_fd) {
				//critical section
				pthread_mutex_lock(&(mutex[x]));
				cur_room->client_array[x] = NULL;

				//update fdset
				FD_CLR(client_fd, &(cur_room->readfds));
				//end of critical section
				pthread_mutex_unlock(&(mutex[x]));
				break;
			}

	if(x == MAX_ROOM_CLINET) return false;
	return true;
}

BOOL AddToServerData(int client_fd, char *client_id) {
	//make client struct
	LPCLIENT new_client = (LPCLIENT)malloc(sizeof(CLIENT));
	memset(new_client, 0x00, sizeof(CLIENT));

	new_client->fd = client_fd;
	strcpy(new_client->id, client_id);

	//add to server_data
	int x = 0;
	for(x ; x < MAX_SERVER_CLIENT ; x++)
		
		if(server_data.client_array[x] == NULL) {
			//critical section
			pthread_mutex_lock(&(mutex[x]));
			server_data.client_array[x] = new_client;
			//end of critical section
			pthread_mutex_unlock(&(mutex[x]));
			break;
		}

	if(x == MAX_SERVER_CLIENT) return false;
	return true;
}

BOOL DeleteFromServerData(int client_fd) {
	//delete from server_data
	int x = 0;
	for(x; x < MAX_SERVER_CLIENT ; x++)
		if(server_data.client_array[x] != NULL)
			if(server_data.client_array[x]->fd == client_fd) {
				//critical section
				pthread_mutex_lock(&(mutex[x]));
				server_data.client_array[x] = NULL;
				//end of critical section
				pthread_mutex_unlock(&(mutex[x]));
				break;
			}

	if(x == MAX_ROOM_CLINET) return false;
	return true;
}

LPCLIENT FindClientFromServerData(int client_fd) {
	LPCLIENT fclient = NULL;

	int x = 0;
	for(x; x < MAX_SERVER_CLIENT ; x++)
		if(server_data.client_array[x] != NULL)
			if(server_data.client_array[x]->fd == client_fd) {
				fclient = server_data.client_array[x];
				break;
			}

	if(fclient == NULL) return false;
	return fclient;
}
