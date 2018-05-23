
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>


int user_connect()
{
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

    return 0;
}



















