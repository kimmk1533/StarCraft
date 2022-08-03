#include "stdafx.h"
#include "Cursor.h"

#include <Animator.h>
#include <Timer.h>

#include "SelectManager.h"

namespace Game
{
	using namespace CoreEngine;

	void C_Cursor::SetCursorState(E_CursorState _state)
	{
		m_CursorState = _state;
	}

	C_Cursor::C_Cursor()
	{
		m_pHotspot = nullptr;
		m_pPosition = nullptr;

		m_pAnimator = nullptr;

		m_AnimIndex = 0;
		m_pGameFrameTimer = nullptr;
		m_CursorState = E_CursorState::Idle;
		m_CursorDir = E_CursorDir::Down;
	}
	C_Cursor::~C_Cursor()
	{
		Destroy();
	}

	HRESULT C_Cursor::Create()
	{
		m_pHotspot = new D3DXVECTOR3(19.0f, 17.0f, 1.0f);
		m_pPosition = new D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		m_pAnimator = new C_Animator();
		SAFE_CREATE(m_pAnimator);
		m_pAnimator->SetSampleRate(g_FPS * 2);
		m_pAnimator->AddFunc(0, [this]() -> void
			{
				++m_AnimIndex;
			});

		m_pGameFrameTimer = new C_Timer();
		m_pGameFrameTimer->SetTime(g_FPS * 2);
		m_pGameFrameTimer->Play();

		return S_OK;
	}
	void C_Cursor::Destroy()
	{
		SAFE_DELETE(m_pGameFrameTimer);

		SAFE_DELETE(m_pAnimator);

		SAFE_DELETE(m_pPosition);
		SAFE_DELETE(m_pHotspot);
	}

	HRESULT C_Cursor::Update(const FLOAT& _deltaTime)
	{
		m_pAnimator->Update(_deltaTime);

#pragma region Mouse Position
		(*m_pPosition) = Input->GetMousePos();
#pragma endregion
		
#pragma region Mouse Click
		if (Input->GetMouseDown(E_MouseCode::Left))
		{
			this->SetCursorState(E_CursorState::Drag);
		}
		else if (Input->GetMouseUp(E_MouseCode::Left))
		{
			this->SetCursorState(E_CursorState::Idle);
		}
#pragma endregion

		return S_OK;
	}
	HRESULT C_Cursor::Render()
	{
		std::shared_ptr<C_Texture> texture = C_SelectManager::GetI()->GetTexture();

		RECT rect;
		if (m_CursorState == E_CursorState::Move)
			rect = C_SelectManager::GetI()->GetTextureRect(m_CursorDir, m_AnimIndex);
		else
			rect = C_SelectManager::GetI()->GetTextureRect(m_CursorState, m_AnimIndex);

		D3DXVECTOR2 temp = (D3DXVECTOR2)(*m_pPosition + (D3DXVECTOR3)(Camera->position));

		Sprite->Draw(
			texture->GetTexture(),
			&rect,
			nullptr,
			nullptr,
			&temp,
			m_pHotspot,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
		);

		return S_OK;
	}
}