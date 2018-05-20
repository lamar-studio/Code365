#include <stdio.h>  
#include <mqueue.h>  
#include <sys/stat.h>  
#include <sys/prctl.h>
#include <stdlib.h>  
#include <unistd.h>  
#include <time.h>  
#include <string.h>
#include <signal.h>

  
#define MAXSIZE     10   //����buf��С  
#define BUFFER      8192 //����Msg��С  
  
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
    /*��Ϣ����*/  
    mqd_t msgq_id;  
    struct MsgType msg;
  
    unsigned int prio = 1;  
    unsigned int send_size = BUFFER; 

    unsigned int sender; 
    unsigned int recv_size = BUFFER;

  
    struct mq_attr msgq_attr;  
    const char *file = "/posix";
    
    /*mq_open() for creating a new queue (using default attributes) */  
    /*mq_open() ����һ���µ� POSIX ��Ϣ���л��һ�����ڵĶ���*/  
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
    /*mq_setattr() ������Ϣ���е����ԣ�����ʱʹ���� newattr ָ��ָ��� mq_attr �ṹ����Ϣ��*/  
    /*������ֻ�б�־ mq_flasgs ��� O_NONBLOCK ��־���Ը��ģ������� newattr �ڵ��򶼱����� */  
    if(mq_setattr(msgq_id, &msgq_attr, NULL) == -1)
    {  
        perror("mq_setattr");
        exit(1);  
    }     

    int pid = fork();
    int child_pid = 0;
    int i = 0;
    if(pid == 0) {  
    //�ӽ���
        child_pid = getpid();
        sigset( SIGTERM, child_process );
        while(1) //����һ��consumenr��Ϊ 10 ��ͬʱ��������consumer���̣�Ϊ 5    
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
        
            /*mq_receive() ���������� mqdes ���õĶ���ʱɾ�����ȼ���ߵ����ϵ���Ϣ�����ѷ��õ� msg_ptr �Ļ������ڡ�*/  
            /*���� msg_len ָ�������� msg_ptr �Ĵ�С����������ڶ��е� mq_msgsize ����(���� mq_getattr)��*/   
            /*��� prio ���� NULL����ô��ָ����ڴ����ڷ����յ���Ϣ��ص����ȼ���*/  
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
    //������
    sigset( SIGTERM, parten_process );
    printf("parten id=%d \n", getpid());
    while(1) 
    {  
        msg.index = rand() % 10;  
        msg.op = rand() % 2;

        //printf("parten >> msg.index = %d, msg.op = %d\n", msg.index, msg.op);  

        /*sending the message      --  mq_send() */  
        /*mq_send() �� msg_ptr ָ�����Ϣ������ mqdes ���õ���Ϣ�����*/  
        /*���� msg_len ָ����Ϣ msg_ptr �ĳ��ȣ�������ȱ���С�ڻ���ڶ��� mq_msgsize ���Ե�ֵ���㳤�ȵ���Ϣ������*/  
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
    /*mq_close() �ر���Ϣ���������� mqdes��������ý�������Ϣ���� mqdes ����֪ͨ����*/  
    /*��ô�������ɾ�����˺��������̾Ϳ��԰�֪ͨ���󵽴���Ϣ���С�*/  
    if(mq_close(msgq_id) == -1)  
    {  
        perror("mq_close");  
        exit(1);  
    }  
  
    /*mq_unlink() ɾ����Ϊ name ����Ϣ���С���Ϣ����������ֱ��ɾ����*/  
    /*��Ϣ���б�������������������е����������ر�ʱ���١�*/  
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




















