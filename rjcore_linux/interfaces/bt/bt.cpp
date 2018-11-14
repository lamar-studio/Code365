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

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

#define BT_SERVER_PATH "/tmp/server.socket"

#define MAX_BUF_LENGTH   1024
#define BT_PACKET_TYPE_NAME 	 "packet_type"
#define BT_MSG_TYPE_NAME         "msg_type"

enum BT_MSG_TYPE{
	BT_MAKE_SEED_START,
	BT_GET_SEED_STATUS,
	BT_MAKE_SEED_CANCLE,
	BT_FILE_DOWNLOAD_START,
	BT_GET_FILE_PROGRESS,
	BT_FILE_DOWNLOAD_CANCLE,
	BT_SHARE_FILE_START,
	BT_SHARE_FILE_STOP,
};


int create_client_socket(char* buf_msg,int len,char* recv_buf)
{
	int sockfd=0;
	struct  sockaddr_un cliun;
	int length;

	if (buf_msg == NULL || len==0) {
		return -2;
	}

	if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		return -2;
    }

    memset(&cliun, 0, sizeof(cliun));
    cliun.sun_family = AF_UNIX;
    strcpy(cliun.sun_path, BT_SERVER_PATH);
    length = offsetof(struct sockaddr_un, sun_path) + strlen(cliun.sun_path);
    if (connect(sockfd, (struct sockaddr *)&cliun, length) < 0) {
		close(sockfd);
		return -2;
    }

	send(sockfd, buf_msg, len, 0);

	recv(sockfd, recv_buf, MAX_BUF_LENGTH,0);

	close(sockfd);

	return 0;

}

void bt_send_request(char* src,int msg_type,char* respone)
{
	cJSON * pJsonRoot = cJSON_Parse(src);
    if(NULL == pJsonRoot) {
        return;
    }

	cJSON_AddNumberToObject(pJsonRoot, BT_PACKET_TYPE_NAME, 1);
    cJSON_AddNumberToObject(pJsonRoot, BT_MSG_TYPE_NAME, msg_type);

	char* msg = cJSON_Print(pJsonRoot);
    if(NULL == msg) {
        cJSON_Delete(pJsonRoot);
        return;
    }

	create_client_socket(msg,strlen(msg),respone);

    cJSON_Delete(pJsonRoot);
}


void BT_MakeSeed_block(char* request, char* respone)
{
	bt_send_request(request,BT_MAKE_SEED_START,respone);
}

void BT_MakeSeed_cancel(char* request, char* respone)
{
	bt_send_request(request,BT_MAKE_SEED_CANCLE,respone);
}


void BT_Share_Start (char* request, char* respone)
{
	bt_send_request(request,BT_SHARE_FILE_START,respone);
}

void BT_Share_Stop (char* request, char* respone)
{
	bt_send_request(request,BT_SHARE_FILE_STOP,respone);
}

void BT_Download_block(char* request, char* respone)
{
	bt_send_request(request,BT_FILE_DOWNLOAD_START,respone);
}

void BT_Download_cancle(char* request, char* respone)
{
	bt_send_request(request,BT_FILE_DOWNLOAD_CANCLE,respone);
}

void BT_Get_Status(char* request, char* respone)
{
	bt_send_request(request,BT_GET_FILE_PROGRESS,respone);
}

#ifdef __cplusplus
}
#endif
