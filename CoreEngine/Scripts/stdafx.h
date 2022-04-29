#pragma once
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <Windows.h>
// d3d 다음 x는 Extended의 의미로 Direct3D의 기본에 대한 확장을 의미
#include <d3d9.h>
#include <d3dx9.h>

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

#include <list>
#include <vector>
#include <map>
#include <unordered_map>

// 참고 https://www.techiedelight.com/use-std-pair-key-std-unordered_map-cpp/
struct Pair_Hash
{
	template<class T1, class T2>
	std::size_t operator() (const std::pair<T1, T2>& pair) const
	{
		return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
	}
};

//#define GMAIN			g_pApp
//#define GHINST		g_pApp->m_hInst
//#define GHWND			g_pApp->m_hWnd

#include "LcInput.h"
#include "LcTexture.h"
#include "LcSprite.h"

#include "McUtil.h"
#include "Time.h"
#include "FrameWork.h"
#include "Engine.h"


struct Point
{
	int x;
	int y;

	Point()
	{
		x = 0;
		y = 0;
	}
	Point(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
};
