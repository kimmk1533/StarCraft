#include "stdafx.h"

#ifndef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")   
#endif

CMain* g_pApp;

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	CMain	d3dApp;

	g_pApp = &d3dApp;

	if (FAILED(d3dApp.Create(hInst)))
		return E_FAIL;

	return d3dApp.Run();
}