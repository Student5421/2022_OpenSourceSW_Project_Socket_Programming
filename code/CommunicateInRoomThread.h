#include "Server_Structure.h"
#include "Server_Structure_Function"

#define MAX_LINE 1024

extern SERVER_DATA server_data;

void *CommunicateWithClient(void *room_num) {
    int room_num = *(int *)room_num;
    int my_room = GetRoomByRoomNum(room_num);
    char message[MAX_LINE];
    while(true) {
        fd_set allfds = my_room->readfs;
        int maxfd = my_room->maxfd;
        memset(buf, 0x00, sizeof(buf));
        int fd_num = select(maxfd + 1, &allfds, (fd_set *)0, (fd_set *)0, NULL);
        
        for(int cur_sockfd = 0 ; cur_sockfd < maxfd + 1 ; cur_sockfd++) {
            //if message get
            if(FD_ISSET(cur_sockfd, &allfds)) {
                //read error
                if(read(cur_sockfd, buf, MAX_LINE) <= 0) {
                    //delete from room and server
                }

                //not read error
                else {
                    //if quit room ("quit\n" can change)
                    if(!strncmp(buf, "quit\n", 5)) {
                        //delte from room
                    }

                    //send message to all client except send
                    else {
                        for(int x = 0 ; x < MAX_ROOM_CLINET ; x++)
                            if(my_room->client_array[x] != NULL && my_room->client_array[x] != cur_sockfd)
                                write(my_room->client_array[x], buf, strlen(buf));
                    }

                    if(--fd_num <= 0) break;
                }
            }
        }
    }
}