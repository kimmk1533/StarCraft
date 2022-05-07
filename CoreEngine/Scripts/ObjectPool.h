#pragma once
#include <queue>

namespace CoreEngine
{
	template<class T>
	class C_ObjectPool final
	{
	private:
		int m_Capacity;
		std::queue<T*>* m_Queue;

	private:
		void ExpandPool();

	public:
		C_ObjectPool(int _capacity = 10);
		virtual ~C_ObjectPool();

	public:
		HRESULT Create();
		void	Destroy();

	public:
		T Spawn();
		void Despawn(T _item);

	};
}