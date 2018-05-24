
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#include "server.h"

int main()
{
    int server_sockfd, client_sockfd;
    int server_len, client_len;
    struct sockaddr_un server_addr;
    struct sockaddr_un client_addr;
    int ret;
    fd_set readfds, testfsd;


    //创建
    unlink(SERVER_ADDR);
    server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

    //命名
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SERVER_ADDR);
    server_len = sizeof(server_addr);
    bind(server_sockfd, (struct sockaddr *)&server_addr, server_len);

    //连接
    listen(server_sockfd, 5);

    //关联readfds到server_sockfd
    FD_ZERO(&readfds);
    FD_SET(server_sockfd, &readfds);

    while(1) {
        char buf[BUF_SIZE] = {0};
        int fd;
        int nread;

        testfsd = readfds;

        printf("\nserver waiting ...\n");
        ret = select(FD_SETSIZE, &testfsd, (fd_set *)0, (fd_set *)0,
            (struct timeval *)0);
        if(ret < 1) {
            perror("server select");
            exit(1);
        }

        for(fd = 0; fd < FD_SETSIZE; fd++) {
            if( FD_ISSET(fd, &testfsd) ) {
                if (fd == server_sockfd) { //新的连接请求
                    client_len    = sizeof(client_addr);
                    client_sockfd = accept(server_sockfd,
                                  (struct sockaddr *)&client_addr, &client_len);
                    FD_SET(client_sockfd, &readfds);
                    printf("adding client on fd %d\n", client_sockfd);
                }
                else { //client端处理
                    ioctl(fd, FIONREAD, &nread);
                    if(nread == 0) {
                        close(fd);
                        FD_CLR(fd, &readfds);
                        printf("removing client on fd %d\n", fd);
                    }
                    else {
                        nread = read(fd, buf, sizeof(buf));
                        sleep(1);
                        upper_process(fd, buf, nread);
                        write(fd, buf, sizeof(buf));
                    }
                }
            }
        }
    }
}




int upper_process(int fd, char data[], int len)
{
    int i = 0;

    for( i = 0; i < len; i++) {
        data[i] = toupper(data[i]);
    }
    printf("upper server to client on fd %d\n", fd);

    return i;
}











