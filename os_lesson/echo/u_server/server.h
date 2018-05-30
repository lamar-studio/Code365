
#ifndef __SERVER_H__
#define __SERVER_H__

#define SERVER_ADDR  "server_socket"
#define SHOW_INFO    "show"
#define QUIT         "quit"
#define MODE         "mode"
#define UPPER        "upper"
#define LOWER        "lower"
#define BUF_SIZE     1024

typedef struct server_info {
    int client_id;
    char mode[10];
    int client_cnt;
}server_info;

void upper_process(int fd, char data[], int len);
void lower_process(int fd, char data[], int len);
void *server_process(void *arg);

#endif







