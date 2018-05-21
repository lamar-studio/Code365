#include <stdio.h>  
#include <mqueue.h>  
#include <sys/stat.h>  
#include <sys/prctl.h>
#include <stdlib.h>  
#include <unistd.h>  
#include <time.h>  
#include <string.h>
#include <signal.h>

  
#define MAXSIZE     10   //定义buf大小  
#define BUFFER      8192 //定义Msg大小  
  
struct MsgType{  
    int index;  
    short op;
};


static int iExtFlag = 0;
static int iExtFlag_c = 0;
static int  msgCnt  = 0;
static int localDatebase[MAXSIZE] = {0};
static int shareDatabase[MAXSIZE] = {0};

void parten_process();
void child_process();


int main(int argc, char ** argv)   
{   
    struct MsgType msg;
  
    unsigned int prio = 1;
    unsigned int send_size = BUFFER; 

    unsigned int sender;
    unsigned int recv_size = BUFFER;

  
    struct mq_attr msgq_attr; 
    mqd_t msgq_id;
    const char *file = "/posix";

    //设置信号处理函数
	if(signal(SIGTERM, parten_process) == SIG_ERR) {
	    return 1;
	}

    //创建消息队列
    msgq_id = mq_open(file, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG, NULL);  
    if(msgq_id == (mqd_t)-1)  
    {  
        perror("mq_open");  
        exit(1);  
    }  
    //获取默认属性
    if(mq_getattr(msgq_id, &msgq_attr) == -1)  
    {  
        perror("mq_getattr");  
        exit(1);  
    } 
    printf("\nParten id=%d \n", getpid());
    printf("Queue \"%s\":\n\t- stores at most %ld messages \
            \n\t- large at most %ld bytes each\n\t- currently holds %ld messages\n",   
        file, msgq_attr.mq_maxmsg, msgq_attr.mq_msgsize, msgq_attr.mq_curmsgs);
     
    //设置消息队列的属性
    if(mq_setattr(msgq_id, &msgq_attr, NULL) == -1)
    {  
        perror("mq_setattr");
        exit(1);  
    }     

    int pid = fork();
    int i = 0;
    if(pid == 0) {  
    //子进程
        if(signal(SIGTERM, child_process) == SIG_ERR) {
            return 1;
        }
        while(1)   
        { 
            
            if(iExtFlag_c == 1)
            {
                printf("\nthe ShareDatabase:%d\t\n", msgCnt);
                printf("index  count\n");
                for( i = 0; i < MAXSIZE; i++ )
                {
                    printf("  %d      %d\n", i, shareDatabase[i]);
                }
                break;
            }

            msg.index = 0;  
            msg.op    = 0;  
        
            //接收消息        
            if (mq_receive(msgq_id, (char*)&msg, recv_size, &sender) == -1)   
            {  
                perror("mq_receive");
                exit(1);  
            }
            //printf("child  >> msg.index = %d, msg.op = %d\n", msg.index, msg.op);

            //upadte the localDataBase
            if ( msg.op == 0)
            {
                shareDatabase[msg.index] -=  1;
            } else if( msg.op == 1)
            {
                shareDatabase[msg.index] +=  1;
            }
            else
            {
                printf("the opration format err.");  
            }
            msgCnt++;
            usleep(500000);
        }
    }  
    else if(pid > 0) {
    //父进程
    while(1) 
    {  
        msg.index = rand() % 10;  
        msg.op = rand() % 2;

        //printf("parten >> msg.index = %d, msg.op = %d\n", msg.index, msg.op);  
        //发送消息
        if(mq_send(msgq_id, (char*)&msg, sizeof(struct MsgType), prio) == -1)  
        {  
            perror("mq_send");
            exit(1);  
        }  

        //upadte the localDataBase
        if ( msg.op == 0)
        {
            localDatebase[msg.index] -=  1;
        } else if( msg.op == 1)
        {
            localDatebase[msg.index] +=  1;
        }
        else
        {
            printf("the opration format err.");  
        }
        sleep(1);
        if( iExtFlag == 1)
        {
            printf("\nthe LocalDataBase:\t\n");
            printf("index  count\n");
            for( i = 0; i < MAXSIZE; i++)
            {
                printf("  %d     %d\n", i, localDatebase[i]);
            }
            mq_close(msgq_id);
            mq_unlink(file);

            kill( 0, SIGTERM );
            wait();
            break;
        }
    }
    }
    else {  
        fprintf(stderr, "fork error!\n");  
        mq_close(msgq_id);
        mq_unlink(file);
        exit(5);  
    }

    return 0;   
}  


void parten_process()
{    
    iExtFlag = 1;

    return ;
}

void child_process()
{
    iExtFlag_c = 1;
    
    return ;
}




















