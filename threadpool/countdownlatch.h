#ifndef _COUNTDOWNLATCH_H
#define _COUNTDOWNLATCH_H

#include "condition.h"
#include "mutexlock.h"

// CountDownLatch的主要作用是确保Thread中传进去的func真的启动了以后
// 外层的start才返回
class CountDownLatch 
{
public:
    explicit CountDownLatch(int count);
    void wait();
    void countDown();

private:
    MutexLock mutex_;
    Condition condition_;
    int count_;
};

CountDownLatch::CountDownLatch(int count)
  : mutex_(),
    condition_(mutex_),
    count_(count)
{ }

void CountDownLatch::wait()
{
    MutexLockGuard lock(mutex_);
    while (count_ > 0)
        condition_.wait();
}

void CountDownLatch::countDown()
{
    MutexLockGuard lock(mutex_);
    --count_;
    if (count_ == 0)
        condition_.notifyAll();
}

#endif