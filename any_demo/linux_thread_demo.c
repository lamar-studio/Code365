
#include <unistd.h>


// printf process id and thread id
int main()
{
    printf("pid is %d \n", getpid());
    printf("tid is %d \n", gettid());

    return 0;
}

// creat thread and set name
int main()
{
    pthread_t th_id;

    pthread_create(&th_id, NULL, (void *)t_func, NULL);
    pthread_setname_np(th_id, "my name");

    return 0;
}

void t_func(void *args)
{
    printf("thread function");

    return ;
}


// set thread priority
//
#include <pthread.h>
#include <sched.h>

int main()
{
    pthread_attr_t tattr;
    pthread_t tid;
    int ret;
    int newprio = 20;
    sched_param param;

    /* initialized with default attributes */
    ret = pthread_attr_init (&tattr);

    /* safe to get existing scheduling param */
    ret = pthread_attr_getschedparam (&tattr, &param);

    /* set the priority; others are unchanged */
    param.sched_priority = newprio;

    /* setting the new scheduling param */
    ret = pthread_attr_setschedparam (&tattr, &param);

    /* with new priority specified */
    ret = pthread_create (&tid, &tattr, func, arg);
}
















