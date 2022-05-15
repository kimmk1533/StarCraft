#include "..\..\CoreEngine\Scripts\stdafx.h"

#include "Cursor.h"
#include "..\..\CoreEngine\Scripts\Singleton.cpp"
#include "SelectManager.h"
#include "Utility.h"
#include "Timer.h"

namespace Game
{
	using namespace CoreEngine;

	C_Cursor::C_Cursor()
	{
		m_AnimIndex = 0;
		m_pGameFrameTimer = nullptr;
		m_CursorState = E_CursorState::Idle;
		m_CursorDir = E_CursorDir::Down;
		m_pHotspot = nullptr;
		m_pPosition = nullptr;
	}
	C_Cursor::~C_Cursor()
	{
		Destroy();
	}

	HRESULT C_Cursor::Create()
	{
		m_pGameFrameTimer = new C_Timer();
		m_pGameFrameTimer->SetTime(FPS * 2);
		m_pGameFrameTimer->Play();

		m_pHotspot = new D3DXVECTOR3(20.0f, 20.0f, 0.0f);
		m_pPosition = new D3DXVECTOR2(0.0f, 0.0f);

		return S_OK;
	}
	void C_Cursor::Destroy()
	{
		SAFE_DELETE(m_pHotspot);
		SAFE_DELETE(m_pPosition);

		SAFE_DELETE(m_pGameFrameTimer);
	}

	HRESULT C_Cursor::Update(const FLOAT& _deltaTime)
	{
#pragma region 1 프레임
		(*m_pPosition) = m_pInput->GetMousePos2();
#pragma endregion

#pragma region 1 게임 프레임 (Normal 기준 15프레임)
		// 1 게임 프레임 대기
		if (FAILED(m_pGameFrameTimer->Update(_deltaTime)))
			return S_OK;

		++m_AnimIndex;
#pragma endregion

		return S_OK;
	}
	HRESULT C_Cursor::Render()
	{
		std::shared_ptr<C_Texture> texture = C_SelectManager::GetI().GetTexture();
		RECT rect;

		if (m_CursorState == E_CursorState::Move)
			rect = C_SelectManager::GetI().GetTextureRect(m_CursorDir, m_AnimIndex);
		else
			rect = C_SelectManager::GetI().GetTextureRect(m_CursorState, m_AnimIndex);

		m_pSprite->Draw(texture->GetTexture(), &rect, nullptr, nullptr, m_pPosition, m_pHotspot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		return S_OK;
	}

	void C_Cursor::SetCursorState(E_CursorState _state)
	{
		m_CursorState = _state;
	}

}