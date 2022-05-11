#include "..\..\CoreEngine\Scripts\stdafx.h"

#include "Main.h"
#include "Marine.h"

namespace Game
{
	using namespace CoreEngine;

	const int Count = 1;

	HRESULT C_Main::Init(HINSTANCE _hInst)
	{
		if (FAILED(C_Engine::Init(_hInst)))
			return E_FAIL;

		return S_OK;
	}
	HRESULT C_Main::Create()
	{
		if (FAILED(C_Engine::Create()))
			return E_FAIL;

		m_Marine = new C_Marine * [Count];
		for (int i = 0; i < Count; ++i)
		{
			m_Marine[i] = new C_Marine();

			if (FAILED(m_Marine[i]->Create()))
				return E_FAIL;
		}

		return S_OK;
	}
	void C_Main::Destroy()
	{
		for (int i = 0; i < Count; ++i)
		{
			SAFE_DELETE(m_Marine[i]);
		}
		SAFE_DELETE_ARRAY(m_Marine);
	}

	HRESULT C_Main::Update(const float _deltaTime)
	{
		if (FAILED(C_Engine::Update(_deltaTime)))
			return E_FAIL;

		for (int i = 0; i < Count; ++i)
		{
			if (FAILED(m_Marine[i]->Update(_deltaTime)))
				return E_FAIL;
		}

		return S_OK;
	}

	HRESULT C_Main::Render()
	{
		if (FAILED(C_Engine::Render()))
			return E_FAIL;

		if (FAILED(m_pd3dDevice->BeginScene()))
			return E_FAIL;

		for (int i = 0; i < Count; ++i)
		{
			if (FAILED(m_Marine[i]->Render()))
				return E_FAIL;
		}

		// EndScene
		m_pd3dDevice->EndScene();

		return S_OK;
	}
}