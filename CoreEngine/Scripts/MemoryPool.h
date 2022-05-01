#pragma once
#include <queue>

template<class T>
class C_MemoryPool : C_FrameWork
{
private:
	std::queue<T>* m_Queue;

public:
	C_MemoryPool()
	{
		m_Queue = nullptr;
	}
	virtual ~C_MemoryPool() { }

public:

};