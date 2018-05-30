
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "client.h"

int main(int argc, char *argv[])
{
    int opt;

    if (argc != 2)
    {
        printf("Usage: ./client -k/u\n");
        exit(0);
    }

    while( (opt = getopt(argc, argv, ":uk")) != -1) {
        switch(opt) {
        case 'u':
            printf("\nconnect the user server.\n");
            user_connect();
            break;
        case 'k':
            printf("\nconnect the kernel server.\n");
            kernel_connect();
            break;
        case ':':
            printf("\nplease config the server.\n");
            break;
        case '?':
            printf("\nunknow option: %c\n", opt);
            break;
        }
    }

    return 0;
}


























