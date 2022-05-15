#include "..\..\CoreEngine\Scripts\stdafx.h"

#include "Cursor.h"
#include "..\..\CoreEngine\Scripts\Singleton.cpp"
#include "SelectManager.h"
#include "Utility.h"

namespace Game
{
	using namespace CoreEngine;

	C_Cursor::C_Cursor()
	{
		m_AnimIndex = 0;
		m_AnimIndexTimer = 0.0f;
		m_CursorState = E_CursorState::Idle;
		m_CursorDir = E_CursorDir::Down;
		m_Hotspot = nullptr;
		m_Position = nullptr;
	}
	C_Cursor::~C_Cursor()
	{
		Destroy();
	}

	HRESULT C_Cursor::Create()
	{
		m_Hotspot = new D3DXVECTOR3(20.0f, 20.0f, 0.0f);
		m_Position = new D3DXVECTOR2(0.0f, 0.0f);

		return S_OK;
	}
	HRESULT C_Cursor::Update(float _deltaTime)
	{
		*m_Position = m_pInput->GetMousePos2();

		// 1 게임 프레임 대기
		m_AnimIndexTimer += _deltaTime;

		// 1 게임 프레임 진행
		if (m_AnimIndexTimer >= FPS * 2)
		{
			m_AnimIndexTimer -= FPS * 2;

			++m_AnimIndex;
		}

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

		m_pSprite->Draw(texture->GetTexture(), &rect, nullptr, nullptr, m_Position, m_Hotspot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		return S_OK;
	}
	void C_Cursor::Destroy()
	{
		SAFE_DELETE(m_Hotspot);
		SAFE_DELETE(m_Position);
	}
	void C_Cursor::SetCursorState(E_CursorState _state)
	{
		m_CursorState = _state;
	}

}