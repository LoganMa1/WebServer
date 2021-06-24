#ifndef _HTTPCON_H
#define _HTTPCON_H
#include<sys/socket.h>
#include<stdlib.h>
#include<errno.h>
#include<stdio.h>
#include<netinet/ip.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<memory.h>

#include"Threadpool.h"
#include"Response.h"

#define PORT 7878
#define BACKLOG 10


class httpcon
{
private:
    int sockt_t;
    int port_t;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;

    Threadpool threadpool;
public:
    httpcon(int _port = PORT):port_t(_port){
        my_addr.sin_family = AF_INET;
        my_addr.sin_port = htons(port_t);
        my_addr.sin_addr.s_addr = INADDR_ANY;
        memset(&(my_addr.sin_zero),'\0',8);
        init();}
    void init();

};


void httpcon::init()
{
    if((sockt_t = socket(PF_INET,SOCK_STREAM,0))==-1)
        printerror("sock!");
    int yes =1;
    if(setsockopt(sockt_t,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1)
        printerror("setsockopt!");
    if(bind(sockt_t,(struct sockaddr *)&my_addr,sizeof(my_addr))==-1)
        printerror("bind!");
    if(listen(sockt_t,BACKLOG)==-1)
        printerror("listen!");
    int new_fd = 0;
    socklen_t sin_size = sizeof(struct sockaddr_in);
    while(true)
    {
        if((new_fd = accept(sockt_t,(struct sockaddr * )&their_addr,&sin_size))==-1) printerror("accept!");
        printf("connected from %s\n",inet_ntoa(their_addr.sin_addr));
        threadpool.push(new_fd);
    }
}

#endif