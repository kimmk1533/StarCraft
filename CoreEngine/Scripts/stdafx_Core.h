#pragma once
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#pragma region Define DEBUG

//#define DEBUG_QuadTree_PRINT
//#define DEBUG_QuadTree_RENDER

#pragma endregion

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
#include <fstream>
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
namespace filesystem = std::experimental::filesystem;
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

#pragma region D3D

using LPD3DXTEXTURE9 = LPDIRECT3DTEXTURE9;

#pragma region VECTOR

D3DXVECTOR2 operator+(const D3DXVECTOR2& _lhs, const D3DXVECTOR3& _rhs);
D3DXVECTOR3 operator+(const D3DXVECTOR3& _lhs, const D3DXVECTOR2& _rhs);

D3DXVECTOR2 operator-(const D3DXVECTOR2& _lhs, const D3DXVECTOR3& _rhs);
D3DXVECTOR3 operator-(const D3DXVECTOR3& _lhs, const D3DXVECTOR2& _rhs);

D3DXVECTOR2& operator+=(D3DXVECTOR2& _lhs, const D3DXVECTOR3& _rhs);
D3DXVECTOR2& operator+=(D3DXVECTOR2& _lhs, const FLOAT& _rhs);
D3DXVECTOR3& operator+=(D3DXVECTOR3& _lhs, const D3DXVECTOR2& _rhs);
D3DXVECTOR3& operator+=(D3DXVECTOR3& _lhs, const FLOAT& _rhs);

D3DXVECTOR2& operator-=(D3DXVECTOR2& _lhs, const D3DXVECTOR3& _rhs);
D3DXVECTOR2& operator-=(D3DXVECTOR2& _lhs, const FLOAT& _rhs);
D3DXVECTOR3& operator-=(D3DXVECTOR3& _lhs, const D3DXVECTOR2& _rhs);
D3DXVECTOR3& operator-=(D3DXVECTOR3& _lhs, const FLOAT& _rhs);

std::ostream& operator<<(std::ostream& _out, const D3DXVECTOR2& _vector);
std::ostream& operator<<(std::ostream& _out, const D3DXVECTOR3& _vector);

#pragma endregion

#pragma endregion

#include "CoreUtil.h"
#include "Texture.h"

#include "Camera.h"
#include "Sprite.h"
#include "Input.h"
#include "CoreTime.h"

#include "FrameWork.h"
#include "Engine.h"

#include "Singleton.h"
#include "ObjectPool.h"

#define Camera CoreEngine::C_Engine::GetCamera()
#define Sprite CoreEngine::C_Engine::GetSprite()
#define Input CoreEngine::C_Engine::GetInput()
