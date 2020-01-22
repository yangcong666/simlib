#ifndef  COROUTINE_H

#define  COROUTINE_H
#define  DEFAULT_STACK_SIZE 1024*1024  //缓存栈大小
#define  _XOPEN_SOURCE

#include  <stdio.h>
#include  <string.h>
#include  <ucontext.h>
#include  <memory>

// 状态码 空闲、运行、暂停
enum Costate { FREE = 0, RUNNING = 1, SUSPEND = 2 };

// 协程类
class Coroutine
{
public:
	Coroutine();
	virtual ~Coroutine() {
		delete buffer;
	};

	// 用户协程入口函数
	virtual void CoProcess();

	// 用户协程恢复函数
	void Resume();

	// 获取协程ID
	int GetId() const {
		return _id;
	}

	// 设置协程Id
	void SetId(int id)
	{
		_id = id;
	}

	// 获取协程状态
	int GetState() const
	{
		return _state;
	}

	// 设置协程状态
	void SetState(Costate state)
	{
		_state = state;
	}

protected:

	// 用户协程挂起
	void Yield();


	// 堆栈缓存
	void SaveStack();

	// 堆栈恢复
	void ReloadStack();

public:
	char * buffer;        // 缓存
	ucontext_t ctx;
private:
	int _stack_size;
	int _cap;
	int _id;
	Costate _state;
};



#endif

