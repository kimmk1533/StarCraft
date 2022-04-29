#include "CMain.h"
#include "Marine.h"

HRESULT CMain::Create(HINSTANCE hInst)
{
	if (FAILED(CEngine::Create(hInst)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMain::Create()
{
	if (FAILED(CEngine::Create()))
		return E_FAIL;

	m_Marine = new CMarine();
	if (FAILED(m_Marine->Create()))
		return E_FAIL;

	return S_OK;
}
void CMain::Destroy()
{
	SAFE_DELETE(m_Marine);
}

HRESULT CMain::Update(const float deltaTime)
{
	if (FAILED(CEngine::Update(deltaTime)))
		return E_FAIL;

	if (FAILED(m_Marine->Update(deltaTime)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMain::Render()
{
	if (FAILED(CEngine::Render()))
		return E_FAIL;

	if (FAILED(m_pd3dDevice->BeginScene()))
		return E_FAIL;

	if (FAILED(m_Marine->Render()))
		return E_FAIL;

	// EndScene
	m_pd3dDevice->EndScene();

	return S_OK;
}

