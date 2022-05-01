#pragma comment(linker, "/subsystem:windows")

#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")   
#endif

//#include <stdafx.h>
#include "..\..\CoreEngine\Scripts\stdafx.h"

#include "CMain.h"

#ifdef UNICODE
INT WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT)
#else
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
#endif
{
	// utf-8 설정
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);

	C_Main d3dApp;

	g_pApp = dynamic_cast<C_Engine*>(&d3dApp);
	if (nullptr == g_pApp)
		return E_FAIL;

	if (FAILED(d3dApp.Create(hInst)))
		return E_FAIL;

	return d3dApp.Run();
}