#include <assert.h>
#include "schedule.h"

Schedule::Schedule()
{

}

Schedule::~Schedule()
{

}

void Schedule::coroutineEntry(Coroutine* crt)
{
	crt->SetState(RUNNING);
	crt->CoProcess();
	crt->SetState(FREE);
}

void Schedule::coroutineNew(Coroutine* crt)
{
	int id = crt->GetId();
	Costate state = Costate(crt->GetState());
	assert(id!= 0);
	assert(state == FREE);

	if (crtPool[id] != nullptr)
	{
		CrtMap::iterator it = crtPool.find(id);
		crtPool.erase(it);
	}

	// 构建用户协程上下文
	getcontext(&(crt->ctx));
	crt->ctx.uc_stack.ss_sp = stack;
	crt->ctx.uc_stack.ss_size = DEFAULT_STACK_SIZE;
	crt->ctx.uc_stack.ss_flags = 0;
	crt->ctx.uc_link = &mainCtx;
	crtPool[id] = crt;

	makecontext(&crt->ctx, (void(*)(void))coroutineEntry, 1, (void*)crt);
	swapcontext(&mainCtx, &crt->ctx);
}

void Schedule::Resume(int id)
{
	if (crtPool[id] != nullptr)
	{
		crtPool[id]->Resume();
	}
}

int Schedule::HasCoroutine()
{
	int count = 0;
	CrtMap::iterator it;
	for (it = crtPool.begin(); it != crtPool.end(); ++it)
	{
		if (it->second->GetState() != FREE)
		{
			count++;
		}
		else {
			it = crtPool.erase(it);
			--it;
		}
	}
	return count;
}

void Schedule::Remove(int id)
{
	if (crtPool[id] != nullptr)
	{
		crtPool.erase(crtPool.find(id));
	}
}