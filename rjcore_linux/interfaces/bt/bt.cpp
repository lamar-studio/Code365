#define MODULE_TAG    "bt"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include "rj_commom.h"
#include "json.h"
#include "rj_log.h"

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

#define BT_SERVER_PATH          "/tmp/server.socket"

#define MAX_BUF_LENGTH          1024
#define BT_PACKET_TYPE_NAME     "packet_type"
#define BT_MSG_TYPE_NAME        "msg_type"

enum BT_MSG_TYPE{
    BT_MAKE_SEED_START=10,
    BT_GET_SEED_STATUS,
    BT_MAKE_SEED_CANCLE,
    BT_FILE_DOWNLOAD_START=30,
    BT_GET_FILE_PROGRESS,
    BT_FILE_DOWNLOAD_CANCLE,
    BT_SHARE_FILE_START=50,
    BT_SHARE_FILE_STOP,
};

int cmd_send(int fd, const char* buf, size_t len)
{
    int n = 0;
    int total_length = len;
    int pos = 0;

    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    fd_set fset;

    while (pos < total_length) {

        FD_ZERO(&fset);
        FD_SET(fd, &fset);

        if (select(fd + 1, NULL, &fset, NULL, &tv) > 0) {

            n = send(fd, buf + pos, len - pos, 0);

            if ( n < 0 ) {
                switch(errno) {
                case EPIPE :
                    goto error;
                case EINTR:
                case EAGAIN:
                    continue;
                default:
                    goto error;
                }
            }
            pos += n;
        } else {
            goto error;
        }
    }

    return 0;

error:
    return -1;
}

int recv_respone(int fd,char* respone,int res_len)
{
    int n = 0,total_len=0;

    while ((n = recv(fd, respone+total_len, res_len-total_len, 0)) > 0) {
	    total_len += n;
    }


    if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)) {

        if (total_len <= 0) {
            rjlog_error("recv result 0, connect closed, fd : %d", fd);
        } else {
	        rjlog_info("recv success,fd:%d, buf:%s",fd,respone);
	        close(fd);	    
	        return 0;
	    }

    }

    if (n == 0) {
        rjlog_error("recv result 0, connect closed, fd : %d", fd);
    }

    if (n < 0) {
        rjlog_error("read %d error: %d %s", fd, errno, strerror(errno));
    }

    close(fd);

    return -1;
}

int create_client_socket(char* request,int len,char* respone,int res_len)
{
    int sockfd=0;
    struct  sockaddr_un cliun;
    int length;

    if ((request == NULL) || (len==0) || (respone == NULL)) {
	    rjlog_error("the param is error,return!!!");
	    return -2;
	}

    if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
	    rjlog_error("create socket id is error,return!!!");
	    return -2;
    }

    memset(&cliun, 0, sizeof(cliun));
    cliun.sun_family = AF_UNIX;
    strcpy(cliun.sun_path, BT_SERVER_PATH);
    length = offsetof(struct sockaddr_un, sun_path) + strlen(cliun.sun_path);
    if (connect(sockfd, (struct sockaddr *)&cliun, length) < 0) {
	    rjlog_error("connect function failed,close socket");
	    close(sockfd);
	    return -2;
    }

	if (cmd_send(sockfd, request, len) < 0) {
	    rjlog_error("send failed,buf:%s",request);
	    return -1;
	}

    return recv_respone(sockfd, respone, res_len);

}

void bt_send_request(const char* request,int msg_type,char* respone,int res_len)
{
    cJSON * pJsonRoot = NULL;
    int ret = -2;
    char* msg = NULL;

    CHECK_INIT_STR_EX(respone,res_len);
    CHECK_FUNCTION_IN();
		
    rjlog_info("bt task will start, msg type is %d",msg_type);
    pJsonRoot = cJSON_Parse(request);
    if(pJsonRoot == NULL) {
        rjlog_error("malloc json object is failed");
        goto ERROR;
    }

    cJSON_AddNumberToObject(pJsonRoot, BT_PACKET_TYPE_NAME, 1);
    cJSON_AddNumberToObject(pJsonRoot, BT_MSG_TYPE_NAME, msg_type);

    msg = cJSON_Print(pJsonRoot);
    if(msg == NULL) {
        rjlog_error("msg is null,return!!!");
        cJSON_Delete(pJsonRoot);
        goto ERROR;
    }
    
    ret = create_client_socket(msg,strlen(msg),respone,res_len);

    if (ret < 0) {
	     rjlog_error("socket operation resule is failed!!!");
	     goto ERROR;
    }

    cJSON_Delete(pJsonRoot);
    rjlog_info("bt task the end, msg type is %d",msg_type);
    
    return;
    
ERROR:
    sprintf(respone,"RJFAIL-%d",15000-ret);
}


void BtMakeSeed_block(const char* request, char* respone,int res_len)
{

    bt_send_request(request,BT_MAKE_SEED_START,respone,res_len);
}

void BtMakeSeedCancel(const char* request, char* respone,int res_len)
{

    bt_send_request(request,BT_MAKE_SEED_CANCLE,respone,res_len);
}


void BtShareStart (const char* request, char* respone,int res_len)
{

    bt_send_request(request,BT_SHARE_FILE_START,respone,res_len);
}

void BtShareStop (const char* request, char* respone,int res_len)
{

    bt_send_request(request,BT_SHARE_FILE_STOP,respone,res_len);
}

void BtDownload_block(const char* request, char* respone,int res_len)
{

    bt_send_request(request,BT_FILE_DOWNLOAD_START,respone,res_len);
}

void BtDownloadCancle(const char* request, char* respone,int res_len)
{

    bt_send_request(request,BT_FILE_DOWNLOAD_CANCLE,respone,res_len);
}

void BtGetStatus(const char* request, char* respone,int res_len)
{
	
    bt_send_request(request,BT_GET_FILE_PROGRESS,respone,res_len);
}

#ifdef __cplusplus
}
#endif
