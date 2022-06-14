#pragma once

#include "Server_Structure.h"
#include "Server_Structure_Function.h"
#include <string.h>

#define MAX_LINE 1024

extern SERVER_DATA server_data;

void *CommunicateWithClient(void *shared_data) {
    int room_num = *(int *)shared_data;

    LPROOM my_room = GetRoomByRoomNum(room_num);
    char buf[MAX_LINE];

    while(true) {
        fd_set allfds = my_room->readfds;
        int maxfd = my_room->maxfd;
        memset(buf, 0x00, sizeof(buf));
        
        struct timeval timeout;
        timeout.tv_sec = 2;
        timeout.tv_usec = 0;

        int fd_num = select(maxfd + 1, &allfds, (fd_set *)0, (fd_set *)0, &timeout);
        if(fd_num == 0) continue;

        for(int cur_sockfd = 0 ; cur_sockfd < maxfd + 1 ; cur_sockfd++) {
            //if message get
            if(FD_ISSET(cur_sockfd, &allfds)) {
                //read error
                if(read(cur_sockfd, buf, MAX_LINE) <= 0) {
                    printf("sockfd : %d, error\n", cur_sockfd);
                    DeleteFromServerData(cur_sockfd);
                    DeleteClientFromRoom(cur_sockfd, room_num);
                }

                //not read error
                else {
                    //if quit room ("quit\n" can change)
                    if(!strcmp(buf, "quit\n")) {
                        DeleteFromServerData(cur_sockfd);
                        DeleteClientFromRoom(cur_sockfd, room_num);
                    }

                    //send message to all client except send
                    else {
                        if(strcmp(buf, "\n")) {
                            LPCLIENT send_client = NULL;
                            for(int x = 0 ; x < MAX_ROOM_CLIENT ; x++)
                                if(my_room->client_array[x] != NULL)
                                    if(my_room->client_array[x]->fd == cur_sockfd)
                                        send_client = my_room->client_array[x];
                        
                            char string[MAX_BUF]; memset(string, 0x00, MAX_BUF);
                            strcat(string, "["); 
                            strcat(string, send_client->id); 
                            strcat(string, "] : ");
                            strcat(string, buf);
                        
                            printf("room[%d] %s", room_num, string);
                        
                            for(int x = 0 ; x < MAX_ROOM_CLIENT ; x++)
                                if(my_room->client_array[x] != NULL) 
                                    if(my_room->client_array[x]->fd != cur_sockfd)
                                        write(my_room->client_array[x]->fd, string, MAX_BUF);
                        }
                    }

                    if(--fd_num <= 0) break;
                }
            }
        }
    }
}