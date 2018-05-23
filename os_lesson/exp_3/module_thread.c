#include <linux/module.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/wait.h>
#include <linux/completion.h>
#include <linux/slab.h> 
#include <linux/init.h> 
#include <linux/kernel.h> 
#include <linux/random.h>
#include <linux/kthread.h>
MODULE_LICENSE("GPL");

#define MEM_KMALLOC_SIZE 1024static struct completion comple_a;
static struct completion comple_b;
struct mutex mutex_mem;
struct timer_list my_timer1;
static wait_queue_t data_a;
static wait_queue_t data_b;
static int thread_a;
static int thread_b;
int flag_exit=0;
char * mem_spvm;
char rand_num=0;
char rand_num2=0;
char rand_num3=0;
char rand_num4=0;
unsigned int message_num_a=0;
unsigned int message_num_b=0;
unsigned int read_p=0;

int thread_A(void * argc)
{
  while(1){
   init_completion(&comple_a); 
   init_waitqueue_entry(&data_a,current);
   //printk("<0>the current A pid is:%d wait for complete\n",current->pid);
   add_wait_queue(&(comple_a.wait),&data_a);
   wait_for_completion(&comple_a);
   //printk("<0>the current A pid is:%d out of queue\n",current->pid);
   mutex_lock(&mutex_mem);
   if(mem_spvm != NULL) {
     if(*(char*)mem_spvm=='A') {
        message_num_a++;
        kfree(mem_spvm);
        mem_spvm=NULL; 
     } else if(*(char*)mem_spvm=='P') {
        printk("<0>message for A thread num is %d\n",message_num_a);
        message_num_a=0;
        if(read_p == 1) {
          kfree(mem_spvm);
          mem_spvm=NULL;
        }
        else read_p = 1;
     }
  
   }
   mutex_unlock(&mutex_mem);
   if(flag_exit == 1) break;
}
   do_exit(0);
   return 0;
}

int thread_B(void * argc)
{
  while(1)
  {
    init_completion(&comple_b); 
    init_waitqueue_entry(&data_b,current);
    //printk("<0>the current B pid is:%d wait for complete\n",current->pid);
    add_wait_queue(&(comple_a.wait),&data_b);
    wait_for_completion(&comple_b);
    //printk("<0>the current B pid is:%d out of queue\n",current->pid);
    mutex_lock(&mutex_mem);
    if(mem_spvm != NULL) {
      if(*(char*)mem_spvm=='B') {
       message_num_b++;
       kfree(mem_spvm);
       mem_spvm=NULL; 
    } else if(*(char*)mem_spvm=='P') {
       printk("<0>message for B thread num is %d\n",message_num_b);
       message_num_b=0;
      if(read_p == 1){
       kfree(mem_spvm);
       mem_spvm=NULL;
      }
      else read_p = 1;
      }
  
    } 
   mutex_unlock(&mutex_mem);
   if(flag_exit==1)break;
  }
   do_exit(0);
   return 0;
}

int thread_K(void * argc)
{
  if(flag_exit !=1) {
   mem_spvm = (char *)kmalloc( MEM_KMALLOC_SIZE, GFP_KERNEL);
   read_p=0;
   get_random_bytes(&rand_num,sizeof(char));
   get_random_bytes(&rand_num2,sizeof(char));
   get_random_bytes(&rand_num3,sizeof(char));
   get_random_bytes(&rand_num4,sizeof(char));
   if((rand_num % 2==0) && (rand_num2 % 2==0) && (rand_num3 % 2==0) && (rand_num4 % 2==0)) {
      *(char*)mem_spvm='P';
    } 
   else if (rand_num % 2 ==0 ){
      *(char*)mem_spvm='A';
    }
   else {
     *(char*)mem_spvm='B';
    } 
  complete(&comple_a);
  complete(&comple_b);
  mod_timer(&my_timer1,jiffies + 1*HZ);
}
  return 0;
}

static int __init module_thread_init(void)
{	   printk("<0>into complete_init.\n");
   mutex_init(&mutex_mem);
   init_timer(&my_timer1);   my_timer1.expires = jiffies + 1*HZ;   my_timer1.data = (unsigned long)&my_timer1;
   my_timer1.function = (void *)thread_K;
   add_timer(&my_timer1);
   thread_a=kernel_thread(thread_A,NULL,0);
   thread_b=kernel_thread(thread_B,NULL,0);
   return 0;
}


static void __exit module_thread_exit(void)
{	
  printk("<0>Goodbye complete\n");
  flag_exit=1;	
  del_timer(&my_timer1);
  complete(&comple_a);
  complete(&comple_b);    
}


module_init(module_thread_init);
module_exit(module_thread_exit);

