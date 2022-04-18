#pragma once
#pragma comment(linker, "/subsystem:windows")

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
#include <time.h>
#include <mmsystem.h>

#include <list>
#include <vector>
#include <map>

#define GMAIN			g_pApp
//#define GHINST			g_pApp->m_hInst
//#define GHWND			g_pApp->m_hWnd
#define GDEVICE			g_pApp->GetDevice()

#include "LcInput.h"
#include "LcTexture.h"
#include "LcSprite.h"

#include "FrameWork.h"
#include "Time.h"
#include "McUtil.h"

#include "Main.h"

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
