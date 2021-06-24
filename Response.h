#ifndef _RESPONSE_H
#define _RESPONSE_H
#include<sys/socket.h>
#include<iostream>
#include<vector>
#include<errno.h>
#include<stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<unistd.h>
#include<string.h>
#include"printerror.h"
#include"lock.h"


#define SERVER "Server: Logan/0.0.1\r\n"
#define BLANKLINE "\r\n"


class Response
{
private:
    char temp[1024];

    void reset_temp();

    read_write_lock rwlock;
public:
    Response() {}
    void init();
    ~Response();

    void setHTML(const char * path="/index.html");
    int sendStatus(int);
    int sendServername(int);
    int sendContenttype(int);
    int sendBlankline(int);
    int sendHtml(int,FILE * _file);

    void sendall(int,FILE * _file);

};


void Response::reset_temp()
{
    temp[0] = '\0';
}


int Response::sendStatus(int _fd)
{
    int numbytes = 0;
    reset_temp();
    strcpy(temp,"HTTP/1.0 200 OK\r\n");
    numbytes = send(_fd,&temp,strlen(temp),0);
    printf("has send %d bytes status\n",numbytes);
    return numbytes;
}


int Response::sendServername(int _fd)
{
    int numbytes = 0;
    reset_temp();
    strcpy(temp,SERVER);
    numbytes = send(_fd,&temp,strlen(temp),0);
    printf("has send %d bytes servername\n",numbytes);
    return numbytes;
}

int Response::sendContenttype(int _fd)
{
    int numbytes = 0;
    reset_temp();
    strcpy(temp,"Content-Type: text/html\r\n");
    numbytes = send(_fd,&temp,strlen(temp),0);
    printf("has send %d bytes contenttype\n",numbytes);
    return numbytes;
}

int Response::sendBlankline(int _fd)
{
    int numbytes = 0;
    reset_temp();
    strcpy(temp,"\r\n");
    numbytes = send(_fd,&temp,strlen(temp),0);
    printf("has send %d bytes blank line\n",numbytes);
    return numbytes;
}

int Response::sendHtml(int _fd,FILE * _file)
{
    int totalbytes = 0;
    char buffer[1024];
    buffer[0] = '\0';
    if(fgets(buffer,sizeof(buffer),_file)==NULL)printerror("fgets!");
    while(!feof(_file))
    {
        totalbytes += send(_fd,buffer,strlen(buffer),0);
        buffer[0] = '\0';
        if(fgets(buffer,sizeof(buffer),_file)==NULL)printerror("fgets!");
    }
    totalbytes += send(_fd,buffer,strlen(buffer),0);
    if(fseek(_file,0,SEEK_SET)!=0)printerror("fseek!");
    printf("has send %d html file\n",totalbytes);
    return totalbytes;
}


void Response::sendall(int _fd,FILE * _file)
{
    printf("start sending ...\n");
    if(sendStatus(_fd)<=0) printerror("send status!");
    if(sendServername(_fd)<=0) printerror("send status!");
    if (sendContenttype(_fd)<=0) printerror("send status!");
    if(sendBlankline(_fd)<=0) printerror("send status!");
    if(sendHtml(_fd,_file)<=0) printerror("send status!");
    printf("response has been send all!\n");
}
#endif 