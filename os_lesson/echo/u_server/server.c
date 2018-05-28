
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
#include "hash.h"

void *server_process(void *arg);
pthread_mutex_t info_mutex;
server_info cfg;
int char_cnt;         //字符计数
int client_cnt;       //客户端计数
char gMode[10];       //默认全局配置
Hash *hash_table;

int main()
{
    int server_sockfd;
    int server_len;
    struct sockaddr_un server_addr;
    int ret = -1;
    pthread_t thread_s;
    void *thread_result;

    memset(&cfg, 0, sizeof(cfg));
    memcpy(cfg.mode, UPPER, sizeof(UPPER));
    memcpy(gMode, UPPER, sizeof(UPPER));
    char_cnt = 0;
    client_cnt = 0;

	//创建哈希表
	hash_table = hash_new();
    if (hash_table == NULL) {
        perror("server hash_new");

        goto err;
    }

    //创建
    unlink(SERVER_ADDR);
    server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_sockfd < 0) {
        perror("server socket");

        goto err_hash;
    }

    //命名
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SERVER_ADDR);
    server_len = sizeof(server_addr);
    ret = bind(server_sockfd, (struct sockaddr *)&server_addr, server_len);
    if (ret != 0 ) {
        perror("server bind");
        goto err_hash;
    }

    //连接
    ret = listen(server_sockfd, 5);
    if (ret != 0) {
        perror("server listen");
        goto err_hash;
    }

    //设置线程相关功能
    ret = pthread_mutex_init(&info_mutex, NULL);
    if(ret != 0) {
        perror("server pthread_mutex_init");
        goto err_sock;
    }
    ret = pthread_create(&thread_s, NULL, server_process, &server_sockfd);
    if(ret != 0) {
        perror("server pthread_create");
        goto err_mutex;
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
            printf("Global mode: %s\n", gMode);
            printf("Characters: %d\n", char_cnt);
            printf("Clients: %d\n", client_cnt);
            pthread_mutex_unlock(&info_mutex);
        }
        else if(strncmp(buf, QUIT, sizeof(QUIT)-1) == 0) {
            break;
        }
        else if(strncmp(buf, MODE, sizeof(MODE)-1) == 0) {
            sscanf (buf, "mode %[0-9] %[a-z]", client, mode);
            if (client[0] == '0') {
                pthread_mutex_lock(&info_mutex);
                memcpy(gMode, mode, strlen(mode));
                pthread_mutex_unlock(&info_mutex);
            }
            else {
                pthread_mutex_lock(&info_mutex);
                ret = hash_get(hash_table, client, &cfg);
                pthread_mutex_unlock(&info_mutex);
                if(ret == 0) {  // 在哈希表中没有
                    printf("not found this client %s", client);
                    continue;
                }
                else {//在哈希表中找到
                    pthread_mutex_lock(&info_mutex);
                    memcpy(cfg.mode, mode, strlen(mode));
                    hash_set(&hash_table, client, cfg);    //更新到哈希表中
                    pthread_mutex_unlock(&info_mutex);
                }
            }
        }
    }

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

err_mutex:
    pthread_mutex_destroy(&info_mutex);
err_sock:
    close(server_sockfd);
err_hash:
    hash_delete(hash_table);
err:
    return ret;
}




void upper_process(int fd, char data[], int len)
{
    int i = 0;

    for( i = 0; i < len; i++) {
        data[i] = toupper(data[i]);
        usleep(200*1000);
    }
    //printf("upper server to client on fd %d\n", fd);

    return;
}

void lower_process(int fd, char data[], int len)
{
    int i = 0;

    for( i = 0; i < len; i++) {
        data[i] = tolower(data[i]);
        usleep(200*1000);
    }
    //printf("tolower server to client on fd %d\n", fd);

    return;
}

void normal_process(int fd, char data[], int len)
{
    int i = 0;

    for( i = 0; i < len; i++) {
        usleep(200*1000);
    }
    //printf("normal server to client on fd %d\n", fd);

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
	char key_tag[20]  = {0};

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
            FD_CLR(fd, &readfds);
            exit(1);
        }

        for(fd = 0; fd < FD_SETSIZE; fd++) {
            if( FD_ISSET(fd, &testfsd) ) {
                if (fd == server_sockfd) { //新的连接请求
                    client_len    = sizeof(client_addr);
                    client_sockfd = accept(server_sockfd,
                                  (struct sockaddr *)&client_addr, &client_len);
                    FD_SET(client_sockfd, &readfds);
                    sprintf(buf, "%d", client_sockfd);
                    write(client_sockfd, buf, sizeof(buf));
                    //printf("adding client on fd %d\n", client_sockfd);

                    pthread_mutex_lock(&info_mutex);
                    client_cnt++;
                    cfg.client_id = client_sockfd;
                    memcpy(cfg.mode, gMode, strlen(gMode));
                    hash_set(&hash_table, buf, cfg);
                    pthread_mutex_unlock(&info_mutex);
                }
                else { //client端处理
                    ioctl(fd, FIONREAD, &nread);
                    if(nread == 0) {
                        sprintf(key_tag, "%d", fd);
                        close(fd);
                        FD_CLR(fd, &readfds);
                        hash_remove(&hash_table, key_tag);
                        pthread_mutex_lock(&info_mutex);
                        client_cnt--;
                        pthread_mutex_unlock(&info_mutex);
                        //printf("removing client on fd %d\n", fd);
                    }
                    else {
                        sprintf(key_tag, "%d", fd);
                        pthread_mutex_lock(&info_mutex);
                        ret = hash_get(hash_table, key_tag, &cfg);
                        pthread_mutex_unlock(&info_mutex);
                        if(ret == 0) {// 在哈希表中没有
                            continue;
                        }
                        else {//在哈希表中找到
                            nread = read(fd, buf, sizeof(buf));
                            //printf("cfg.mode %s\n", cfg.mode);
                            //printf("cfg.client_id %d\n", cfg.client_id);

                            pthread_mutex_lock(&info_mutex);
                            char_cnt += (strlen(buf) - 1);
                            if (strncmp(cfg.mode, UPPER, sizeof(UPPER)-1) == 0){
                                upper_process(fd, buf, strlen(buf));
                            }
                            else if(strncmp(cfg.mode, LOWER, sizeof(LOWER)-1) == 0){
                                lower_process(fd, buf, strlen(buf));
                            } else {
                                normal_process(fd, buf, strlen(buf));
                            }
                            pthread_mutex_unlock(&info_mutex);

                            write(fd, buf, sizeof(buf));
                        }
                    }
                }
            }
        }
    }
    pthread_exit(0);
}





