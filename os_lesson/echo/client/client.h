
#ifndef __CLIENT_H__
#define __CLIENT_H__


int user_connect();

int kernel_connect();

int open_netlink();
int send_event(int socket_fd);
int read_event(int socket_fd);










#endif



