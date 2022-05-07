#include "stdafx.h"

#include "ObjectPool.h"

namespace CoreEngine
{
	template<class T>
	C_ObjectPool<T>::C_ObjectPool(int _capacity)
	{
		m_Capacity = _capacity;
		m_Queue = nullptr;
	}
	template<class T>
	C_ObjectPool<T>::~C_ObjectPool<T>()
	{
		Destroy();
	}

	template<class T>
	HRESULT C_ObjectPool<T>::Create()
	{
		m_Queue = new std::queue<T>(m_Capacity);

		return S_OK;
	}
	template<class T>
	void C_ObjectPool<T>::Destroy()
	{
		SAFE_DELETE(m_Queue);
	}

	template<class T>
	void C_ObjectPool<T>::ExpandPool()
	{
		for (int i = 0; i < m_Capacity; ++i)
		{
			T* item = new T();
			m_Queue->push(item);
		}
	}

	template<class T>
	T C_ObjectPool<T>::Spawn()
	{
		return T();
	}
	template<class T>
	void C_ObjectPool<T>::Despawn(T _item)
	{
	}
}