#include "..\..\CoreEngine\Scripts\stdafx.h"

#include "CMain.h"
#include "Marine.h"

HRESULT C_Main::Create(HINSTANCE hInst)
{
	if (FAILED(C_Engine::Create(hInst)))
		return E_FAIL;

	return S_OK;
}

HRESULT C_Main::Create()
{
	if (FAILED(C_Engine::Create()))
		return E_FAIL;

	m_Marine = new C_Marine();
	if (FAILED(m_Marine->Create()))
		return E_FAIL;

	return S_OK;
}
void C_Main::Destroy()
{
	SAFE_DELETE(m_Marine);
}

HRESULT C_Main::Update(const float deltaTime)
{
	if (FAILED(C_Engine::Update(deltaTime)))
		return E_FAIL;

	if (FAILED(m_Marine->Update(deltaTime)))
		return E_FAIL;

	return S_OK;
}

HRESULT C_Main::Render()
{
	if (FAILED(C_Engine::Render()))
		return E_FAIL;

	if (FAILED(m_pd3dDevice->BeginScene()))
		return E_FAIL;

	if (FAILED(m_Marine->Render()))
		return E_FAIL;

	// EndScene
	m_pd3dDevice->EndScene();

	return S_OK;
}

