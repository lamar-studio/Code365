
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <linux/netlink.h>
#include <errno.h>
#include <err.h>
#include <string.h>

#define BUF_SIZE  1024
#define QUIT      "quit"

#define USING_RECVMSG
#define NETLINK_USER 31
#define MY_GROUP 1
#define MAX_PAYLOAD 1024 /* maximum payload size*/


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
    read(sockfd, buf, sizeof(buf));
    printf("\nClient ID is:%s", buf);

    while(1) {
        printf("\nCLIENT> ");
        fgets(buf, sizeof(buf), stdin);

        if(buf[0] == '\n')
            continue;

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
    int ret;
    char buf[BUF_SIZE] = {0};
	int socket_fd = open_netlink();
	if (socket_fd < 0) {
		perror("client open_netlink");
        exit(1);
	}

	while (1) {
        printf("\nCLIENT> ");
        fgets(buf, sizeof(buf), stdin);

        if(buf[0] == '\n')
            continue;

        if (strncmp(buf, QUIT, sizeof(QUIT)-1) == 0) {
            close(socket_fd);
            exit(0);
        }
		send_event(socket_fd, buf);
		sleep(1);
		read_event(socket_fd, buf);
        printf("\nECHO: %s", buf);
	}

	return 0;
}


int open_netlink()
{
	int  socket_fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_USER);
	struct sockaddr_nl addr;

	memset((void *)&addr, 0, sizeof(addr));

	if (socket_fd < 0)
		return socket_fd;
	addr.nl_family = AF_NETLINK;
	addr.nl_pid    = getpid();
	addr.nl_groups = MY_GROUP; // for multicast or broadcast
	if (bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		return -1;
	return socket_fd;
}

int send_event(int socket_fd, char data[])
{
	//printf("send_event in Sending message to kernel linzr...");
	struct iovec       iov;
	struct sockaddr_nl dest_addr;
	struct nlmsghdr *  nlh = NULL;
	struct msghdr      msg_hdr;

	memset(&dest_addr, 0, sizeof(dest_addr));
	dest_addr.nl_family = AF_NETLINK;
	dest_addr.nl_pid    = 0; /* For Linux Kernel */
	dest_addr.nl_groups = 0; /* unicast */

	nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
	//printf("11 Sending message to kernel linzr...");
	memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
	nlh->nlmsg_len   = NLMSG_SPACE(MAX_PAYLOAD);
	nlh->nlmsg_pid   = getpid();
	nlh->nlmsg_flags = 0;
	strcpy(NLMSG_DATA(nlh), data);
	memset(&msg_hdr, 0, sizeof(struct msghdr));

	iov.iov_base        = (void *)nlh;
	iov.iov_len         = nlh->nlmsg_len;
	msg_hdr.msg_name    = (void *)&dest_addr;
	msg_hdr.msg_namelen = sizeof(dest_addr);
	msg_hdr.msg_iov     = &iov;
	msg_hdr.msg_iovlen  = 1;

	//printf("Sending message to kernel: %s", data);
	int ret = sendmsg(socket_fd, &msg_hdr, 0);
	//printf("Done: %d %s\n", ret, strerror(errno));
	free(nlh);
}

int read_event(int socket_fd, char data[])
{
	struct sockaddr_nl nladdr;
	struct msghdr      msg_hdr;
	struct iovec       iov;
	struct nlmsghdr *  nlh;

	nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));

	iov.iov_base = (void *)nlh;
	iov.iov_len  = NLMSG_SPACE(MAX_PAYLOAD);

	memset(&nladdr, 0, sizeof(nladdr));

	msg_hdr.msg_name    = (void *)&(nladdr);
	msg_hdr.msg_namelen = sizeof(nladdr);
	msg_hdr.msg_iov     = &iov;
	msg_hdr.msg_iovlen  = 1;


	int ret = 0;

	ret = recvmsg(socket_fd, &msg_hdr, 0);
	if (ret < 0) {
        perror("client recvmsg");
		return ret;
	}
	//printf("Received message from kernel: %s", (char *)NLMSG_DATA(nlh));
    memcpy(data, (char *)NLMSG_DATA(nlh), nlh->nlmsg_len);
    #if 0
	printf("\nret:%d (nlh)->nlmsg_len:%d\n", ret, nlh->nlmsg_len);

	printf("NLMSG_PAYLOAD: %d \n"
	       "NLMSG_SPACE(MAX_PAYLOAD): %d \n"
	       "NLMSG_PAYLOAD(nlh, 0): %d \n",
	       MAX_PAYLOAD,
	       NLMSG_SPACE(MAX_PAYLOAD),
	       NLMSG_PAYLOAD(nlh, 0));
    #endif

	free(nlh);
}










