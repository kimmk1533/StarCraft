#include "stdafx.h"

#include "Singleton.h"

namespace CoreEngine
{
	template <class T>
	T* C_Singleton<T>::m_Instance = nullptr;

	template <class T>
	C_Singleton<T>::C_Singleton()
	{
		
	}
	template <class T>
	C_Singleton<T>::~C_Singleton()
	{

	}

	template <class T>
	T& C_Singleton<T>::GetI()
	{
		if (m_Instance == nullptr)
			m_Instance = new T();

		return *m_Instance;
	};
}