#ifndef _LOCK_H
#define _LOCK_H
#include<pthread.h>
#include <condition_variable>
#include<shared_mutex>
#include<mutex>

class mutex_lock
{
private:
    pthread_mutex_t mutex_t;
public:
    mutex_lock(){pthread_mutex_init(&mutex_t,NULL);}
    ~mutex_lock(){pthread_mutex_destroy(&mutex_t);}
    void lock(){pthread_mutex_lock(&mutex_t);}
    void unlock(){pthread_mutex_unlock(&mutex_t);}
    pthread_mutex_t * getmutex(){return &mutex_t;}
};

class read_write_lock
{
private:
    std::shared_timed_mutex mutex ; 
public:
    //read
    void read_lock(){mutex.lock_shared();}
    void read_unlock(){mutex.unlock_shared();}

    //write
    void write_lock(){mutex.lock();}
    void write_unlock(){mutex.unlock();}
};
#endif