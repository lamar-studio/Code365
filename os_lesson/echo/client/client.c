
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

#define BUF_SIZE  1024
#define QUIT      "quit"
int user_connect()
{
    int sockfd;
    int len;
    struct sockaddr_un addr;
    int ret;
    char buf[BUF_SIZE] = {0};

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "../u_server/server_socket");
    len = sizeof(addr);

    ret = connect(sockfd, &addr, len);
    if(ret == -1)
    {
        perror("client connect");
        exit(1);
    }
    printf("\nClient ID is:%d", sockfd);
    while(1) {
        printf("\nCLIENT> ");
        fgets(buf, sizeof(buf), stdin);
        if (strncmp(buf, QUIT, sizeof(QUIT)-1) == 0) {
            close(sockfd);
            exit(0);
        }
        write(sockfd, buf, sizeof(buf));
        read(sockfd, buf, sizeof(buf));
        printf("\nECHO: %s", buf);

    }

    close(sockfd);

    return 0;
}




int kernel_connect()
{
#if 0
    int sockfd;
    int len;
    struct sockaddr_un addr;
    int ret;
    char ch = 'A';

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "../u_server/server_socket");
    len = sizeof(addr);

    ret = connect(sockfd, &addr, len);
    if(ret == -1)
    {
        perror("socket connect");
        exit(1);
    }

    write(sockfd, &ch, 1);
    read(sockfd, &ch, 1);
    printf("char from server = %c\n", ch);

    close(sockfd);
#endif
    return 0;
}














