
#ifndef __SERVER_H__
#define __SERVER_H__

#define SERVER_ADDR  "server_socket"
#define BUF_SIZE     1024


typedef struct server_info {
    char *mode;
    int char_cnt;
    int client_cnt;
}server_info;

int upper_process(int fd, char data[], int len);








#endif







