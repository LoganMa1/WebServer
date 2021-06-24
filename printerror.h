#ifndef _PRINT_ERROR_H
#define _PRINT_ERROR_H
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>


void printerror(const char * errorinfo)
{
    char * buffer;
    sprintf(buffer,"error when %s",(*errorinfo));
    perror(buffer);
    exit(1);
}

#endif