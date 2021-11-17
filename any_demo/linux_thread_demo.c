
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


















