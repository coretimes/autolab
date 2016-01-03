#include "sys/socket.h"
#include <stdio.h>

int main(void)
{
    printf("Hello World!\n");
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("%d", sockfd);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("%d", sockfd);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sockfd = socket(AF_INET, SOCK_STREAM);

    printf("%d", sockfd);
    return 0;
}

