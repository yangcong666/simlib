#ifndef SINGLETON_H
#define SINGLETON_H

template<class T>
class Singleton
{
public:

	// 单例获取
	static T* GetInst()
	{
		if (!flag_instance)
		{
			flag_instance = new Singleton();
		}
		return &(flag_instance->_instance);
	}

protected:

	// 单例构造
	Singleton() {};

private:

	// 对象实例
	T _instance;

	// 单例模板实例
	static Singleton<T>* flag_instance;


};

template<class T>
Singleton<T>* Singleton<T>::flag_instance = NULL;



#endif