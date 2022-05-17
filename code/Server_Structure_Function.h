#include <string.h>

#include "Server_Structure.h"

extern CLIENT_DATA client_database[MAX_DATABASE_CLIENT_NUM];
extern SERVER_DATA server_data;

void InitServerData() {
	memset(server_data, 0x00, sizeof(SERVER_DATA));
}

LPROOM_NODE GetRoomByRoomNum(int room_num) {
	LPROOM_NODE room_list = server_data.room_list;
	if(room_list == NULL) return false;

	while(room_list->room_num != room_num || room_list != NULL) room_list = room_list->next;
	if(room_list == NULL) return false;

	return room_list;
}

	/*					*
	 *	About Linked List Function	*
	 *					*/

void AddClientToList(LPCLIENT_NODE head, LPCLIENT_NODE new_client) {
	if(head == NULL) head = new_client;

	else {
		LPCLIENT_NODE cur_client = head;
		while(cur_client->next != NULL) cur_client = cur_client->next;
		cur_client->next = new_client;
	}
}

int AddClient(int client_fd, char *client_id, int room_num) {
	LPCLIENT_NODE new_client = NULL;
	if((new_clinet = (LPCLIENT_NODE)malloc(sizeof(CLIENT_NODE))) == NULL) {
		//error
		return false;
	}

	memset(new_client, 0x00, sizeof(CLIENT_NODE));
	new_client->fd = client_fd;
	strcpy(new_client->id, client_id);

	AddClientToList(server_data.client_list, new_client);

	LPROOM_NODE room_list = NULL;
	if(!(room_list = GetRoomByRoomNum(room_num))) return -1;
	AddClientToList(room_list->client_list, new_client);

	return true;
}

BOOL DeleteClientFromList(LPCLIENT_NODE head, int client_fd) {
	LPCLIENT_NODE cur_client = head;
	LPCLIENT_NODE delete_node = NULL;
	while(cur_client->next->fd != client_fd || cur_client->next != NULL) cur_client = cur_client->next;
	if(cur_client->next == NULL) return false;

	delete_node = cur_client->next;
	cur_client->next = delete_node->next;
	free(delete_node);

	return true;
}

BOOL DeleteClient(int client_fd, int room_num) {
	if(!DeleteClientFromList(server_data.client_list, client_fd)) return false;

	LPROOM_NODE room_list = NULL;
	if(!(room_list = GetRoomByRoomNum(room_num))) return false;
	if(!DeleteClientFromList(room_list->client_list, client_fd)) return false;

	return true;
}
