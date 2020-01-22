#ifndef SCHEDULE_H

#define SCHEDULE_H
#define _XOPEN_SOURCE

#include <stdio.h>
#include  <map>
#include  <ucontext.h>
#include  "coroutine.h"
#include  "../singlenton.h"

/*
协程调度类实现
*/

typedef std::map<int, Coroutine*> CrtMap;
class Schedule
{
public:
	Schedule();
	virtual ~Schedule();

	// 用户协程入口函数
	static void coroutineEntry(Coroutine* crt);

	// 将协程加入协程池，并开启协程
	void coroutineNew(Coroutine* crt);

	// 恢复用户协程
	void Resume(int id);

	// 判断协程池里是否还有未完成的协程，
	// 并将已经终止的协程删除
	int HasCoroutine();

	// 通过协程id删除协程
	void Remove(int id);

	// 获取栈底
	char* GetStackBottom()
	{
		return stack + DEFAULT_STACK_SIZE;
	}

public:
	ucontext_t mainCtx;//主线程上下文信息
    char stack[DEFAULT_STACK_SIZE];  //协程堆栈

private:
	CrtMap crtPool;

};


typedef Singleton<Schedule> SingleSchedule;



#endif