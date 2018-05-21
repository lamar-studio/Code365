#include "queue.h"
#include "hash.h"
#include "queue.h"

typedef struct {
	QUEUE 			*que;
	Hash  			*hash_table;
	pthread_mutex_t mutex;
	pthread_cond_t cond_empty;
	pthread_cond_t cond_full;
}parameter_t;

static bool thread_kill_flag = false;

//整型数转字符串
void itochar(int num,char *pstr)
{
        int val1 = num,val0 = 0;
        int i,j;
        char sastr[10];
        memset(sastr,0,sizeof(sastr));
        for(i = 0; i < 100; i++) {
                val0 = val1 % 10;
                val1 = val1 / 10;
                sastr[i] = val0 + 48;
                if(val1 < 10) {
                        i++;
                        sastr[i] = val1 + 48;
                        break;
                }
        }
        for(j = 0; j <= i;j++)
                pstr[j] = sastr[i - j];
        pstr[j] = '\0';
}

int mySleep(int sec, int msec)
{
	struct timeval tv;
	tv.tv_sec = sec;
	tv.tv_usec = msec * 1000;

	select(0, NULL, NULL, NULL, &tv);

	return 0;
}

void sig_handle(int signum)
{
	// printf("come to the signal*************************************************************\n");
	thread_kill_flag = true;
	 // _exit(0);
}	

void *thread_customer_fun(void *param)
{
	parameter_t *parameter = (parameter_t *)param;
	QUEUE_ITEM *item;
	struct work_record workrecord;
	int ret;
	// printf("customer pid = %lu\n",pthread_self());
	while(1) {
		// if(!is_Queue_empty(parameter->que)){
			// printf("the queue is empty\n");
			// pthread_cond_wait(&parameter->cond_empty,&parameter->mutex);
		// }
		while(!is_Queue_empty(parameter->que))
			;
		//出队
		item = Get_Queue_Item(parameter->que);
		//修改哈希表
		pthread_mutex_lock(&(parameter->mutex));
		ret = hash_get(parameter->hash_table,item->action,&workrecord);
		if(ret == 0) {
			printf("Not find in the hash table\n");
		} else {
			workrecord.c_count++;
			hash_set(&parameter->hash_table,item->action,workrecord);
		}
		pthread_cond_signal(&parameter->cond_full);
		pthread_mutex_unlock(&(parameter->mutex));
		mySleep(0,200);
		// printf("thread_customer_fun item->action = %s workrecord.p_count = %d workrecord.c_count = %d\n",item->action,workrecord.p_count,workrecord.c_count);
	}
	
	return NULL;
}

void *thread_producer_fun(void *param)
{
	int tag;
	parameter_t *parameter = (parameter_t *)param;
	char key_tag[20];
	int ret;
	struct work_record workrecord;
	memset(key_tag,0,sizeof(key_tag));
	
	//生产流程
	while(1) {
		tag = rand() % 100 + 1;//产生100以内的随机数
		itochar(tag,key_tag);
		pthread_mutex_lock(&(parameter->mutex));
		ret = hash_get(parameter->hash_table,key_tag,&workrecord);
		if(ret == 0) {// 在哈希表中没有
			workrecord.tag = tag;
			workrecord.p_count = 1;
			workrecord.c_count = 0;
			hash_set(&parameter->hash_table,key_tag,workrecord);//添加到哈希表中
		} else {//在哈希表中找到 
			workrecord.p_count++;
			hash_set(&parameter->hash_table,key_tag,workrecord);
		}
		Add_Queue_Item(parameter->que, key_tag, tag, sizeof(int));//入队
		// pthread_cond_signal(&parameter->cond_empty);
		if(parameter->que->numitems == 50) {
			 // printf("to the max 50 items\n");
			 pthread_cond_wait(&parameter->cond_full,&parameter->mutex);/*等待*/
		}
		pthread_mutex_unlock(&(parameter->mutex));
		mySleep(0,2);//生产间隔200ms
		if(thread_kill_flag)
			 break;
		
			
		// printf("thread_producer_fun--------tag = %d----------\n",tag);
	}
	// printf("end thread_producer_fun \n");
	
	return NULL;
}

void *thread_control_fun(void *num)
{
	pthread_t producer;
	int i,thread_num;
	pthread_t *custromer_pid_ptr;
	parameter_t parameter;
	thread_num = *(int *)num;
	custromer_pid_ptr = (pthread_t *)malloc(thread_num * sizeof(pthread_t));
	if(custromer_pid_ptr == NULL) {
		printf("malloc failed thread_control_fun exit\n");
		return NULL;
	}
	
	//创建哈希表
	parameter.hash_table = hash_new();
	
	//创建队列
	parameter.que = Initialize_Queue();
	
	//初始化锁
	pthread_mutex_init(&(parameter.mutex), NULL);
	// parameter.cond_empty = PTHREAD_COND_INITIALIZER;
	// parameter.cond_full = PTHREAD_COND_INITIALIZER
	pthread_cond_init(&parameter.cond_empty,NULL);
	pthread_cond_init(&parameter.cond_full,NULL);
	//创建生产者线程
	if(pthread_create(&producer,NULL,thread_producer_fun,&parameter)) {  
        printf("can't create thread: LINE = %d\n",__LINE__);  
		free(custromer_pid_ptr);
        return NULL;  
    } 
	// 创建N个消费者线程
	for(i = 0; i < thread_num;i++) {
		if(pthread_create(custromer_pid_ptr + i,NULL,thread_customer_fun,&parameter)) {
			printf("can't create thread: LINE = %d\n",__LINE__);  
			free(custromer_pid_ptr);
			return NULL;  
		}
	}
		
	
	// 注册信号SIGUSR1处理函数，接收到信号后结束生产者线程和消费者线程，等待所有子线程退出，然后自己退出
	 if(signal(SIGINT, sig_handle) == SIG_ERR) {
        printf("error catching SIGINT\n");
        return NULL;
    }
	
	// pthread_join回收所有的子线程
	pthread_join(producer,NULL);
	 for(i = 0; i < thread_num; i++)
		 pthread_cancel(*(custromer_pid_ptr + i));
	 free(custromer_pid_ptr);
	
	printf("get the signal ,producer and customer is over , the result is :\n");
	hash_foreach(parameter.hash_table);
	 
	// printf("end control thread \n");
	
	return NULL;
}

int main(int argc,char *argv[])	
{
	
	pthread_t control;
	int thread_num = 0;
	if(argc < 2) {
		printf("please the thread num \n");
		return 1;
	}
	thread_num = atoi(argv[1]);
	
	// 创建主线程
	if(pthread_create(&control,NULL,thread_control_fun,&thread_num)) {  
        printf("can't create thread: LINE = %d\n",__LINE__);  
        return 1;  
    }  
	pthread_join(control,NULL);//等待主线程退出
	
	return 0;
}
