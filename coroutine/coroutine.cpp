#include <assert.h>
#include "coroutine.h"
#include "schedule.h"

Coroutine::Coroutine()
	:_id(0),_state(FREE),_cap(0),
	_stack_size(0),buffer(nullptr)
{

}


void Coroutine::CoProcess()
{

}

void Coroutine::Resume()
{
	if (_state == SUSPEND)
	{
		ReloadStack();
		_state = RUNNING;
		swapcontext(&(SingleSchedule::GetInst()->mainCtx), &ctx);
	}
}

void Coroutine::Yield()
{
	if (_state == RUNNING)
	{
		SaveStack();
		_state = SUSPEND;
		swapcontext(&ctx, &(SingleSchedule::GetInst()->mainCtx));
	}
}

void Coroutine::SaveStack()
{
	char* stackBottom = SingleSchedule::GetInst()->GetStackBottom();
	char dumy = 0;

	assert(stackBottom - &dumy <= DEFAULT_STACK_SIZE);
	if (_cap < stackBottom - &dumy)
	{
		// if (buffer)  delete buffer;
		_cap = stackBottom - &dumy;
		buffer = new char[_cap];
	}
	_stack_size = stackBottom - &dumy;

	memcpy(buffer, &dumy, _stack_size);
}

void Coroutine::ReloadStack()
{
	memcpy(SingleSchedule::GetInst()->GetStackBottom() - _stack_size, buffer, _stack_size);
}



