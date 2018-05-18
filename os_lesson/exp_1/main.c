#include <stdio.h>  
#include <mqueue.h>  
#include <sys/stat.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <time.h>  
#include <string.h>  
  
#define MAXSIZE     10   //����buf��С  
#define BUFFER      8192 //����Msg��С  
  
struct MsgType{  
    int len;  
    char buf[MAXSIZE];  
    int index;  
    short op;  
};

struct LocalDataBase{
    int index;
    int count;
};

struct ShareDataBase{
    int index;
    int count;
};
  
int main()   
{   
    /*��Ϣ����*/  
    mqd_t msgq_id;  
    struct MsgType msg;

    struct LocalDataBase ldb[10];
    struct ShareDataBase sdb[10];
  
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
  
    printf("Queue \"%s\":\n\t- stores at most %ld messages \
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
    int i = 0; 
    if(pid == 0) {  
    //�ӽ���
        while(i < 10) //����һ��consumenr��Ϊ 10 ��ͬʱ��������consumer���̣�Ϊ 5    
        {  
            msg.len = -1;  
            memset(msg.buf, 0, MAXSIZE);  
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
            printf("child  >> msg.len = %d, msg.buf = %s, msg.index = %d, msg.op = %d\n", msg.len, msg.buf, msg.index, msg.op);

            if ( msg.op == 1 )
                msg.index = (++msg.index) % 10;
            else
                msg.index = (--msg.index) % 10;

            printf("childP >> msg.len = %d, msg.buf = %s, msg.index = %d, msg.op = %d\n", msg.len, msg.buf, msg.index, msg.op);
        
            i++;  
            sleep(2);   
        }  
    }  
    else if(pid > 0) {
    //������
        while(i < 10) 
        {  
            msg.len = i;
            memset(msg.buf, 0, MAXSIZE);
            sprintf(msg.buf, "0x%x", i);  
            msg.index = rand() % 10;  
            msg.op = rand() % 2;  

            printf("parten >> msg.len = %d, msg.buf = %s, msg.index = %d, msg.op = %d\n", msg.len, msg.buf, msg.index, msg.op);  

            /*sending the message      --  mq_send() */  
            /*mq_send() �� msg_ptr ָ�����Ϣ������ mqdes ���õ���Ϣ�����*/  
            /*���� msg_len ָ����Ϣ msg_ptr �ĳ��ȣ�������ȱ���С�ڻ���ڶ��� mq_msgsize ���Ե�ֵ���㳤�ȵ���Ϣ������*/  
            if(mq_send(msgq_id, (char*)&msg, sizeof(struct MsgType), prio) == -1)  
            {  
                perror("mq_send");  
                exit(1);  
            }  

            i++;  
            sleep(1);     
        }  
        
        sleep(30); //�ȴ������߽����˳�          
    }  
    else {  
        fprintf(stderr, "fork error!\n");  
        mq_close(msgq_id);
        mq_unlink(file);
        exit(5);  
    }

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





























