

#include <sys/time.h>

// timeval struct define
//=========================
// timeval
// {
    // time_t tv_sec;       //√Î [long int]
    // suseconds_t tv_usec; //Œ¢√Î [long int]
// };



// Capture time difference
int main()
{
    struct timeval tv_begin, tv_end;

    gettimeofday(&tv_begin, NULL);
    //do something
    sleep(1);
    gettimeofday(&tv_begin, NULL);

    LOG("duration ms:%ld", 1000*(tv_end.tv_sec - tv_begin.tv_sec)+(tv_end.tv_usec - tv_begin.tv_usec)/1000);
}















