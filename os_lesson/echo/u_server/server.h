
#ifndef __SERVER_H__
#define __SERVER_H__

#define SERVER_ADDR  "server_socket"
#define BUF_SIZE     1024
#define SHOW_INFO    "show"
#define QUIT         "quit"
#define MODE         "mode"
#define UPPER        "upper"
#define LOWER        "lower"

typedef struct server_info {
    char mode[10];
    int char_cnt;
    int client_id;
}server_info;

int upper_process(int fd, char data[], int len);








#endif







