#ifndef _LRUCACHE_H
#define _LRUCAHCE_H
#include"lock.h"
#include<list>
#include<unordered_map>
#include<iterator>
#include<string>
#include<vector>
#include<stdio.h>
#define DEFAULTSIZE 5


struct Node
{
    char * path;
    FILE * file;
    Node(char * s = "./index.html", FILE * p = nullptr):path(s),file(p){}
};

class LRUcache
{
private:
    read_write_lock rwlock;
    
    typedef std::list<struct Node> NodeList;

    std::list<struct Node> lnode;
    std::unordered_map<char* ,NodeList::iterator> hashmap;
    int size;
    
    //not thread safe !. use lock when call !
    void remove_close(NodeList::iterator & _iter);
    void remove(NodeList::iterator &);
    void add(char *,FILE *);
public:
    LRUcache(int _size = DEFAULTSIZE):size(_size){}
    int put(char *,FILE*);
    FILE * get(char* );
};


void LRUcache::remove_close(NodeList::iterator & _iter)
{
    char * _key = _iter->path;
    FILE * _file  = _iter->file;
    fclose(_file);
    hashmap.erase(_key);
    lnode.erase(_iter);
}


void LRUcache::remove(NodeList::iterator & _iter)
{
    char * _key = _iter->path;
    FILE * _file  = _iter->file;
    hashmap.erase(_key);
    lnode.erase(_iter);
}


void LRUcache::add(char * _path,FILE * _file)
{
    fseek(_file,0,SEEK_SET);
    lnode.push_front(Node(_path,_file));
    hashmap[_path] = lnode.begin();
    if(lnode.size()>size)
    {
        auto end = lnode.end();
        remove_close(--end);
    }
}


int LRUcache::put(char * _path,FILE * _file)
{
    rwlock.write_lock();
    auto iter = hashmap.find(_path);
    if(iter!=hashmap.end())
    {
        remove(iter->second);//if find, why not can we just delete the node and add it at the buttom of list. 
    }//and modify the map.
    add(_path,_file);
    printf("thanks you for putting  me here!\n");
    rwlock.write_unlock();
}


FILE * LRUcache::get(char * _path)
{
    rwlock.read_lock();
    for(auto iter = hashmap.begin();iter!=hashmap.end();iter++)
    {
        if(strncmp(_path,iter->first,strlen(_path))==0)
        {
            auto temp = iter->second->file;
            remove(iter->second);
            rwlock.read_unlock();
            rwlock.write_lock();
            add(_path,temp);
            rwlock.write_unlock();
            printf("rigth on, bingo!\n");
            return temp;
        }
    }
    rwlock.read_unlock();
    printf("there is nothing what you want!\n");
    return NULL;
}

#endif