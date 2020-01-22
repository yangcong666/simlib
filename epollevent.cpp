#include "epollevent.h"
#include "demultiplexer.h"
#include "timer.h"

#include <map>

using namespace reactor;
/*************************ReactorImpl**************************************/
class Epollevent::ReactorImpl {
public:
    ReactorImpl();

    ~ReactorImpl();

    int registerHandler(EventHandler* handler, event_t event);

    int removeHandler(EventHandler* handler);

    void handlerEvent();

    std::function<void(void*)> cb;

    int registerTimerTask(HeapTimer* timerEvent);

private:
    EventDemultiplexer* _eventDemultiplexer;

    std::map<handle_t, EventHandler*> _handlers;

    HeapTimerContainer* _eventTimer;
};
/****************************Reactor*************************************/
Epollevent::Epollevent()
{
    _reactorImpl = new ReactorImpl();
}

Epollevent::~Epollevent()
{
    delete _reactorImpl;
}

void Epollevent::handlerEvent()
{
    _reactorImpl->handlerEvent();
}

int Epollevent::registerHandler(EventHandler* handler, event_t event)
{
    return _reactorImpl->registerHandler(handler, event);
}

int Epollevent::removeHandler(EventHandler* handler)
{
    return _reactorImpl->removeHandler(handler);
}

int Epollevent::registerTimerTask(HeapTimer* timer)
{
    return _reactorImpl->registerTimerTask(timer);
}
/*****************************ReactorImpl**********************************/
Epollevent::ReactorImpl::ReactorImpl()
{
    _eventDemultiplexer = new EpollEventDemultiplexer();
    _eventTimer = new HeapTimerContainer();
}

void Epollevent::ReactorImpl::handlerEvent()
{
    int timeout = 0;
    if (_eventTimer) {
        // 如果有计时器
        struct timeval timeTop;
        _eventTimer->top(timeTop);
        struct timeval now;
        gettimeofday(&now, 0);
        timeout = timeTop.tv_sec - now.tv_sec;
    } else {
        timeout = -1;
    }
    _eventDemultiplexer->waitEvent(&_handlers, timeout, _eventTimer);
}

int Epollevent::ReactorImpl::registerHandler(EventHandler* handler, event_t event)
{
    handle_t handle = handler->getHandle();
    std::map<handle_t, EventHandler*>::iterator it = _handlers.find(handle);
    if (it == _handlers.end()) {
        _handlers[handle] = handler;
    }
    return _eventDemultiplexer->addEvent(handle, event);
}

int Epollevent::ReactorImpl::removeHandler(EventHandler* handler)
{
    handle_t handle = handler->getHandle();
    _handlers.erase(handle);
    return _eventDemultiplexer->delEvent(handle);
}

Epollevent::ReactorImpl::~ReactorImpl()
{
    delete _eventDemultiplexer;
}

int Epollevent::ReactorImpl::registerTimerTask(HeapTimer* timerEvent)
{
    if (timerEvent == 0) {
        return fail;
    }

    _eventTimer->add_timer(timerEvent);
    return success;
}