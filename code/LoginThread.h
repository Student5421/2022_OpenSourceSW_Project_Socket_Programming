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

	if(!SendMessageToClient(client_fd, "ReadyToLogin")); //if false, quit login thread
	if(!ReceiveMessageFromClient(client_fd, message)); //if false, quit login thread

	//if service message is 1, 2, ...
	int select_service = atoi(message);

	switch(select_service) {
		case 2: //sign up
			//get ip and passwd

			//check there are not same to another client

			//if same, ask again

			//if does not same, go to login service

		case 1: //login
			//ask id and passwd

			//check id and passwd is in client_database

			//if exist, success to login -> add to server_data

			//if does not exist, fail to login

		case default: //not declare
			//go to ask login or sign up
	}

	memset(message, 0x00, sizeof(message));
	if(!SendMessageToClient(client_fd, "ReadyToRoomService")); //if false, quit thread
	if(!ReceiveMessageFromClient(client_fd, message)); //if false, quit thread

	select_service = atoi(message);
	switch(select_service) {
		case 1: //create room
			//if finish create room, client enter created room

		case 2: //enter room
			//ask room number or id and get it

			//check room is exist

			//if exist, enter the room

			//if does not exist, ask room number or id again

		case default: //not declare
			//go to ask room service
	}
}
