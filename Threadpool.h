#ifndef THREADPOOL_H
#define THREADPOOL_H
#include"lock.h"
#include"printerror.h"
#include"Response.h"
#include"LRUcache.h"
#include"Request.h"
#include<pthread.h>
#include<list>
#include<sys/socket.h>
#define QUEUEDEFAULTSIZE 10
#define MAXQUEUE 20


class Threadpool
{
private:
    int        poolsize;
    int        queuesize;
    pthread_t * threads;
    std::list<int> workqueue;
    static LRUcache  * cache;
    
    mutex_lock   queue_lock;
    pthread_cond_t in;
    pthread_cond_t run_cond;

public:
    Threadpool(int _poolsize = QUEUEDEFAULTSIZE,int _queuesize = MAXQUEUE)
        :poolsize(_poolsize),queuesize(_queuesize){init();printf("threadpool has been start!\n");}
    ~Threadpool();
    void init();
    static void * handle_function(void * input);
    void run();
    int getsize() const{return poolsize;}

    void push(int);

};

LRUcache * Threadpool::cache = new LRUcache();

void Threadpool::push(int fd)
{
    queue_lock.lock();
    if(workqueue.size()>=queuesize) pthread_cond_wait(&in,queue_lock.getmutex());
    workqueue.push_back(fd);
    printf("a fd has been added to pool!\n");
    pthread_cond_signal(&run_cond);
    queue_lock.unlock();
}


Threadpool::~Threadpool()
{
    delete threads;
    delete cache;
}


void Threadpool::init()
{
    if((threads = new pthread_t[poolsize])==nullptr) 
        printerror("create thread pool!");
    for(int i=0;i<poolsize;i++)
    {
        pthread_create(&threads[i],nullptr,handle_function,this);
        pthread_detach(threads[i]);
    }
}


void * Threadpool::handle_function(void * input)
{
    Threadpool * pool = (Threadpool * )input;
    if(pool == nullptr) printerror("convert input to Threadpool object!");
    pool->run();
    return nullptr;
}


void Threadpool::run()
{
    while(true)
    {
        queue_lock.lock();
        if(workqueue.size()<=0) pthread_cond_wait(&run_cond,queue_lock.getmutex());

        int new_fd = this->workqueue.front();
        workqueue.pop_front();


        Request * request = new Request();
        request->get_line(new_fd);
        request->parse_method();
        request->parse_path();
        char *_path = request->get_path();

        FILE * file = cache->get(_path);
        if(file==NULL)
        {
            if(file = fopen(_path,"r")) 
                cache->put(_path,file);
            else 
               printerror("fopen!"); 
        }
        Response * response = new Response;

        response->sendall(new_fd,file);
        close(new_fd);
//        delete request;//if delete, the _path is release. and the memory is collected! so it's null.
        pthread_cond_signal(&in);
        queue_lock.unlock();
    }
}
#endif