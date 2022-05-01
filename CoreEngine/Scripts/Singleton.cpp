#include "stdafx.h"

#include "Singleton.h"

template <class T>
T& C_Singleton<T>::GetI()
{
	static T Instance;
	return Instance;
}