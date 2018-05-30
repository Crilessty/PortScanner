#include"scan.h"

typedef struct port_data_segment{
    struct in_addr ip;
    unsigned int start;
}port_data_segment;

int scan_(char *ip,int po)
{
    
    struct sockaddr_in sa;
    int sockfd;


    memset(&sa, 0, sizeof(sa));

    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(ip);
    sa.sin_port = htons(po);

    if( (sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        perror("socket error :");
        return 0;
    }
    
    
    if( connect(sockfd,(struct sockaddr *)&sa,sizeof(sa)) < 0){
        //perror("connect fail: ");
        close(sockfd);
        return 0;
    }
    else
    {
        close(sockfd);
        return 1;
    }
      
}

//扫描函数
void *scan(void *arg)
{
    port_data_segment port;
    int i = 0;

    //pthread_detach(pthread_self());  

    memcpy(&port,arg,sizeof(port_data_segment));
 

    
    while(port.start <= MAX_PORT)
    {
        if(scan_(inet_ntoa(port.ip),port.start) == 1)
            printf("%s : The port %5d is accepted.\n",inet_ntoa(port.ip),port.start);
        port.start += THREAD_NUM;
    }

}

int main(int argc,char **argv)
{
    pthread_t *thread;
    int i,j;
    struct in_addr IP;
    port_data_segment *port;
    thread = (pthread_t*)malloc( THREAD_NUM * sizeof(pthread_t));
    port = (port_data_segment*)malloc(THREAD_NUM * sizeof(port_data_segment));

    if ( argc < 2 ) {
        fprintf( stderr, "please scan the ip address\n" );
        exit (0);
    }
    if(inet_aton(argv[1],&IP) == 0){
            printf("ip address is wrong");
            exit(1);
    }
    //thread 实现
    for(i = 0;i < THREAD_NUM; i++)
    {
        port[i].ip = IP;
        port[i].start = i;
        
        if(pthread_create(&thread[i], NULL, scan, (void *)&port[i]) != 0)
        {
            perror("pthread create failed");
            return 0;
        }
    }
    
    for(i = 0;i<THREAD_NUM;i++)
    {
        pthread_join(*(thread+i),NULL);
    }

    printf("%s : ports (1 - %d) scanning is completed .\n",argv[1],MAX_PORT);
    
    free(thread);
    free(port);

    return 0;
    
}
