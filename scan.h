#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include<pthread.h>

#define SEGMENT_LENGTH     100
#define MAX_PORT         65535 
#define THREAD_NUM          24  