#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

//线程池头文件

#include "condition.h"

//封装线程池中的对象需要执行的任务对象
typedef struct task
{
    void *(*run)(void *args);  //函数指针，需要执行的任务
    void *arg;              //参数
    struct task *next;      //任务队列中下一个任务
} task_t;


//下面是线程池结构体
class threadpool
{

public:

    threadpool(int _threads);
    virtual ~threadpool();
    
    //线程池初始化
    void threadpool_init(int threads);

    //往线程池中加入任务
    void threadpool_add_task(void *(*run)(void *arg), void *arg);

    // 创建线程执行
    static void* thread_routine(void *arg);

    //摧毁线程池
    void threadpool_destroy();

private:

    condition_t *ready;    //状态量
    task_t *first;       //任务队列中第一个任务
    task_t *last;        //任务队列中最后一个任务
    int counter;         //线程池中已有线程数
    int idle;            //线程池中空闲线程数
    int max_threads;     //线程池最大线程数
    int quit;            //是否退出标志/
};

#endif