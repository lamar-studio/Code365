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
    /*消息队列*/  
    mqd_t msgq_id;  
    struct MsgType msg;
  
    unsigned int prio = 1;  
    unsigned int send_size = BUFFER; 

    unsigned int sender; 
    unsigned int recv_size = BUFFER;

  
    struct mq_attr msgq_attr;  
    const char *file = "/posix";
    
    /*mq_open() for creating a new queue (using default attributes) */  
    /*mq_open() 创建一个新的 POSIX 消息队列或打开一个存在的队列*/  
    msgq_id = mq_open(file, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG, NULL);  
    if(msgq_id == (mqd_t)-1)  
    {  
        perror("mq_open");  
        exit(1);  
    }  
  
    /* getting the attributes from the queue        --  mq_getattr() */  
    if(mq_getattr(msgq_id, &msgq_attr) == -1)  
    {  
        perror("mq_getattr");  
        exit(1);  
    }  
  
    printf("\nQueue \"%s\":\n\t- stores at most %ld messages \
            \n\t- large at most %ld bytes each\n\t- currently holds %ld messages\n",   
        file, msgq_attr.mq_maxmsg, msgq_attr.mq_msgsize, msgq_attr.mq_curmsgs);
     
    /*setting the attributes of the queue        --  mq_setattr() */  
    /*mq_setattr() 设置消息队列的属性，设置时使用由 newattr 指针指向的 mq_attr 结构的信息。*/  
    /*属性中只有标志 mq_flasgs 里的 O_NONBLOCK 标志可以更改，其它在 newattr 内的域都被忽略 */  
    if(mq_setattr(msgq_id, &msgq_attr, NULL) == -1)
    {  
        perror("mq_setattr");
        exit(1);  
    }     

    int pid = fork();
    int child_pid = 0;
    int i = 0;
    if(pid == 0) {  
    //子进程
        child_pid = getpid();
        sigset( SIGTERM, child_process );
        while(1) //运行一个consumenr，为 10 ，同时运行两个consumer进程，为 5    
        { 
            
            if(iExtFlag_c == 1)
            {
                
                printf("the ShareDatabase:%d\t\n", msgCnt);
                for( i = 0; i < MAXSIZE; i++)
                {
                    printf("child  >> shareDatabase[%d] = %d\n", i, shareDatabase[i]);
                }
                printf("SIGTERM exit the child process.\n");

                return 0;
            }

            msg.index = 0;  
            msg.op    = 0;  
        
            /* getting a message */  
        
            /*mq_receive() 从由描述符 mqdes 引用的队列时删除优先级最高的最老的消息，并把放置到 msg_ptr 的缓存区内。*/  
            /*参数 msg_len 指定缓冲区 msg_ptr 的大小：它必须大于队列的 mq_msgsize 属性(参数 mq_getattr)。*/   
            /*如果 prio 不是 NULL，那么它指向的内存用于返回收到消息相关的优先级。*/  
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
    sigset( SIGTERM, parten_process );
    printf("parten id=%d \n", getpid());
    while(1) 
    {  
        msg.index = rand() % 10;  
        msg.op = rand() % 2;

        //printf("parten >> msg.index = %d, msg.op = %d\n", msg.index, msg.op);  

        /*sending the message      --  mq_send() */  
        /*mq_send() 把 msg_ptr 指向的消息加入由 mqdes 引用的消息队列里。*/  
        /*参数 msg_len 指定消息 msg_ptr 的长度：这个长度必须小于或等于队列 mq_msgsize 属性的值。零长度的消息是允许。*/  
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
            kill( 0, SIGTERM );
            printf("the LocalDataBase:\t\n");
            for( i = 0; i < MAXSIZE; i++)
            {
                printf("child  >> localDatebase[%d] = %d\n", i, localDatebase[i]);
            }
    
            printf("SIGTERM exit the parten process.\n");
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

    printf("\n === end the main function ===\n");

    /*closing the queue        -- mq_close() */ 
    /*mq_close() 关闭消息队列描述符 mqdes。如果调用进程在消息队列 mqdes 绑定了通知请求，*/  
    /*那么这个请求被删除，此后其它进程就可以绑定通知请求到此消息队列。*/  
    if(mq_close(msgq_id) == -1)  
    {  
        perror("mq_close");  
        exit(1);  
    }  
  
    /*mq_unlink() 删除名为 name 的消息队列。消息队列名将被直接删除。*/  
    /*消息队列本身在所有引用这个队列的描述符被关闭时销毁。*/  
    if(mq_unlink(file) == -1)  
    {  
        perror("mq_unlink");  
        exit(1);  
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




















