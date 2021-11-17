

#include <windows.h>


// printf process id and thread id
int main()
{
    printf("pid is %d", GetCurrentProcessId());
    printf("tid is %d", GetCurrentThreadId());

    return 0;
}

