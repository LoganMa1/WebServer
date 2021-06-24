#include"Response.h"

std::vector<std::string> *HTMLpointer = new std::vector<std::string>();

void setHTML(const char * path)
{
    char temp;
    int num =0;
    int filed = open(path,O_RDONLY);
    if(filed==-1){perror("error when open file!");exit(1);}
    std::string buffer; 
    do
    {
        num = read(filed,&temp,1);
        if(temp!='\n')
            buffer.push_back(temp);
        else
        { 
            HTMLpointer->push_back(buffer);
            buffer = "";
        }
    } while (num>0);
}

#include<iostream>


void printhtml(FILE * _file)
{
    char buffer[1024];
    fgets(buffer,sizeof(buffer),_file);
    while(!feof(_file))
    {
        printf("%s\n",buffer);
        buffer[0] = '\0';
        fgets(buffer,sizeof(buffer),_file);
    }
    printf("%s\n",buffer);
    fseek(_file,0,SEEK_SET);
}


int main()
{

    FILE * file = fopen("./index.html","r");
    for(int i=0;i<10;i++)
    {
        printhtml(file);
    }

    return 0;
    
}