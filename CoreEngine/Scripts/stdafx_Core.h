#pragma once
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

//#define TEST

#include <Windows.h>
// d3d 다음 x는 Extended의 의미로 Direct3D의 기본에 대한 확장을 의미
#include <d3d9.h>
#include <d3dx9.h>

#pragma region STD Library

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <malloc.h>
#include <memory>
#include <tchar.h>
#include <mmsystem.h>
#include <iostream>
#include <utility>
#include <functional>

#pragma endregion

#pragma region Container

#include <vector>
#include <list>
#include <queue>
#include <map>
#include <unordered_map>

struct Pair_Hash
{
	template<class T1, class T2>
	std::size_t operator() (const std::pair<T1, T2>& pair) const
	{
		// 참고 https://www.techiedelight.com/use-std-pair-key-std-unordered_map-cpp/
		return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);

		// 참고 https://www.geeksforgeeks.org/how-to-create-an-unordered_map-of-pairs-in-c/
		/*
		* auto hash1 = hash<T1>{}(p.first);
		* auto hash2 = hash<T2>{}(p.second);
		*
		* if (hash1 != hash2) {
		* return hash1 ^ hash2;
		* }
		*
		* // If hash1 == hash2, their XOR is zero.
		* return hash1;
		*/
	}
};

#pragma endregion

namespace std
{
	/*template <class TContainer, class TValue>
	bool contains(const TContainer& _container, const TValue& _value)
	{
		std::vector
		return std::find(_container)
	}*/
}

#include "CoreUtil.h"

#include "Input.h"
#include "Texture.h"
#include "Sprite.h"
#include "CoreTime.h"

#include "FrameWork.h"
#include "Engine.h"

#include "Singleton.h"
#include "ObjectPool.h"