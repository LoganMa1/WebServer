#ifndef _REQUEST_H
#define _REQUEST_H
#include<sys/socket.h>
#include<string>
#include"printerror.h"


class Request
{
private:
    char *      METHOD;
    char *       _path;
    char *      buffer;
public:
    Request();
    ~Request();
    void get_line(int);
    void parse_method();
    void parse_path();
    char *  get_method() const;
    char * get_path() const;
};


Request::Request()
{
    METHOD = new char[10];
    _path = new char[30];
    buffer = new char[1024];
}


Request::~Request()
{
    delete[] METHOD;
    delete[] _path;
    delete[] buffer;
}

void Request::get_line(int _fd)
{
    int i=0;
    char temp = '\0';
    int n=0;
    buffer[0] = '\0';
    while(temp!='\n')
    {
        if(n = recv(_fd,&temp,1,0)>0)
        {
            if(temp=='\r')
            {
                n = recv(_fd,&temp,1,MSG_PEEK);
                if((n>0)&&(temp=='\n'))
                    recv(_fd,&temp,1,0);
                else
                    temp = '\n';
            }
            buffer[i] = temp;
            i++;
        }else printerror("recv!\n");
    }
    buffer[i] = '\0';
}


void Request::parse_method()
{
    METHOD[0] = '\0';
    int i=0;
    while(buffer[i]!=' ')
    {
        METHOD[i] = buffer[i];
        i++;
    }
    METHOD[i] = '\0';
}


void Request::parse_path()
{
    int i =0,j=1;
    while(buffer[i]!=' ') i++;
    _path[0] = '.';
    i++;
    while(buffer[i]!=' ')
    {
        _path[j] = buffer[i];
        i++;j++;
    }
    _path[j] = '\0';
}


char * Request::get_method() const
{
    return METHOD;
}


char * Request::get_path() const
{
    return _path;
}
#endif