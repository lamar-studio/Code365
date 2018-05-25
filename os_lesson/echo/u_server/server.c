
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <semaphore.h>

#include "server.h"

void *server_process(void *arg);
pthread_mutex_t info_mutex;
server_info cfg;

int main()
{
    int server_sockfd;
    int server_len;
    struct sockaddr_un server_addr;
    int ret;
    pthread_t thread_s;
    void *thread_result;

    memset(&cfg, 0, sizeof(cfg));
    memcpy(cfg.mode, UPPER, sizeof(UPPER));

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

    //设置线程相关功能
    ret = pthread_mutex_init(&info_mutex, NULL);
    if(ret != 0) {
        perror("server pthread_mutex_init");
        exit(1);
    }
    ret = pthread_create(&thread_s, NULL, server_process, &server_sockfd);
    if(ret != 0) {
        perror("server pthread_create");
        exit(1);
    }

    while(1) {
        char buf[50]   = {0};
        char client[5] = {0};
        char mode[10]  = {0};

        printf("\nSERVER> ");
        fgets(buf, sizeof(buf), stdin);
        if (strncmp(buf, SHOW_INFO, sizeof(SHOW_INFO)-1) == 0) {
            pthread_mutex_lock(&info_mutex);
            printf("\nServer info:\n");
            printf("Global mode: %s\n", cfg.mode);
            printf("Characters: %d\n", cfg.char_cnt);
            printf("Clients: %d\n", cfg.client_id);
            pthread_mutex_unlock(&info_mutex);
        }
        else if(strncmp(buf, QUIT, sizeof(QUIT)-1) == 0) {
            break;
        }
        else if(strncmp(buf, MODE, sizeof(MODE)-1) == 0) {
            sscanf (buf, "mode %[0-9] %[a-z]", client, mode);
            pthread_mutex_lock(&info_mutex);
            cfg.client_id = atoi(client);
            memcpy(cfg.mode, mode, strlen(mode));
            pthread_mutex_unlock(&info_mutex);
        }
    }

    close(server_sockfd);

    ret = pthread_cancel(thread_s);
    if (ret != 0) {
        perror("server pthread_cancel");
        exit(1);
    }

    ret = pthread_join(thread_s, &thread_result);
    if (ret != 0) {
        perror("server pthread_join");
        exit(1);
    }

    ret = pthread_mutex_destroy(&info_mutex);
    if (ret != 0) {
        perror("server pthread_mutex_destroy");
        exit(1);
    }

    return ret;
}




void upper_process(int fd, char data[], int len)
{
    int i = 0;

    for( i = 0; i < len; i++) {
        data[i] = toupper(data[i]);
    }
    //printf("upper server to client on fd %d\n", fd);

    return;
}

void lower_process(int fd, char data[], int len)
{
    int i = 0;

    for( i = 0; i < len; i++) {
        data[i] = tolower(data[i]);
    }
    //printf("tolower server to client on fd %d\n", fd);

    return;
}


void *server_process(void *arg)
{
    fd_set readfds, testfsd;
    int client_sockfd;
    int ret;
    int client_len;
    int server_sockfd = *(int *)arg;
    struct sockaddr_un client_addr;

    ret = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    if (ret != 0) {
        perror("pthread_setcancelstate");
        exit(1);
    }

    ret = pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    if (ret != 0) {
        perror("pthread_setcanceltype");
        exit(1);
    }

    FD_ZERO(&readfds);
    FD_SET(server_sockfd, &readfds);

    while(1) {
        char buf[BUF_SIZE] = {0};
        int fd;
        int nread;

        testfsd = readfds;

        //printf("\nserver waiting ...\n");
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
                    //printf("adding client on fd %d\n", client_sockfd);
                    pthread_mutex_lock(&info_mutex);
                    cfg.client_id = client_sockfd;
                    pthread_mutex_unlock(&info_mutex);
                }
                else { //client端处理
                    ioctl(fd, FIONREAD, &nread);
                    if(nread == 0) {
                        close(fd);
                        FD_CLR(fd, &readfds);
                        //printf("removing client on fd %d\n", fd);
                    }
                    else {
                        if (fd == cfg.client_id) {
                            nread = read(fd, buf, sizeof(buf));
                            sleep(1);
                            pthread_mutex_lock(&info_mutex);
                            cfg.char_cnt += (strlen(buf) - 1);
                            pthread_mutex_unlock(&info_mutex);
                            if (strncmp(cfg.mode, UPPER, sizeof(UPPER)-1) == 0)
                                upper_process(fd, buf, nread);

                            if (strncmp(cfg.mode, LOWER, sizeof(LOWER)-1) == 0)
                                lower_process(fd, buf, nread);

                            write(fd, buf, sizeof(buf));
                        }
                        else {
                            printf("nonsupport this client %d", fd);
                        }
                    }
                }
            }
        }
    }
    pthread_exit(0);
}





