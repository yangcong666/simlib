#ifndef EPELLEVENT_H
#define EPELLEVENT_H

#include <sys/epoll.h>

#include "timer.h"


typedef int handle_t;

namespace epollevent {
enum event_t {
    readEvent = 0x01,
    writeEvent = 0x02,
    errorEvent = 0x04,
    eventMask = 0xff
};
}
using namespace epollevent;
class EventHandler {
public:
    virtual handle_t getHandle() const = 0;

    virtual void handleRead() = 0;

    virtual void handleWrite() = 0;

    virtual void handleError() = 0;

public:
    EventHandler(){};

    virtual ~EventHandler(){};
};

class Epollevent {
public:
    class ReactorImpl;

private:
    ReactorImpl* _reactorImpl;

public:
    explicit Epollevent();

    ~Epollevent();

    int registerHandler(EventHandler* handler, event_t event);

    int removeHandler(EventHandler* handler);

    void handlerEvent();

    int registerTimerTask(HeapTimer* timer);

private:
    Epollevent(const Epollevent&) {} // 禁止拷贝

    Epollevent& operator=(const Epollevent&) {} // 禁止赋值
};
#endif