#include "stdafx.h"
#include "Cursor.h"

#include <Animator.h>
#include <Timer.h>
#include <TextureManager.h>

#include "SelectManager.h"

namespace Game
{
	using namespace CoreEngine;

	const WORD C_Cursor::m_wCursorMoveOffset = 10;

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
		m_CursorState = E_CursorState::Idle;
		m_CursorDir = E_CursorDir::Up;
	}
	C_Cursor::~C_Cursor()
	{
		Destroy();
	}

	HRESULT C_Cursor::Create()
	{
		m_pHotspot = new D3DXVECTOR3(19.0f, 17.0f, 1.0f);
		m_pPosition = new D3DXVECTOR3();

		m_pAnimator = new C_Animator();
		SAFE_CREATE(m_pAnimator);
		m_pAnimator->SetSampleRate(g_FPS * 2);
		m_pAnimator->AddFunc(0, [this]() -> void
			{
				++m_AnimIndex;
			});

		return S_OK;
	}
	void C_Cursor::Destroy()
	{

		SAFE_DELETE(m_pAnimator);

		SAFE_DELETE(m_pPosition);
		SAFE_DELETE(m_pHotspot);
	}

	HRESULT C_Cursor::Update(const FLOAT& _deltaTime)
	{
		m_pAnimator->Update(_deltaTime);

		D3DXVECTOR3 mousePos = Input->GetMousePos();

#pragma region Mouse Position
		(*m_pPosition) = Camera->WorldToScreenPoint(mousePos);
#pragma endregion

#pragma region Mouse State

#pragma region Move

		if (m_CursorState != E_CursorState::Drag)
		{
			RECT rc = C_Engine::GetScreenRect();

			if (rc.left + m_wCursorMoveOffset <= mousePos.x &&
				rc.right - m_wCursorMoveOffset >= mousePos.x &&
				-rc.bottom + m_wCursorMoveOffset <= mousePos.y &&
				rc.top - m_wCursorMoveOffset >= mousePos.y)
			{
				this->SetCursorState(E_CursorState::Idle);
			}
			else
			{
				this->SetCursorState(E_CursorState::Move);

				static const E_CursorDir Dir[3][3] =
				{
					{ E_CursorDir::Up_Left,		E_CursorDir::Up,	E_CursorDir::Up_Right	},
					{ E_CursorDir::Left,		E_CursorDir::Max,	E_CursorDir::Right		},
					{ E_CursorDir::Down_Left,	E_CursorDir::Down,	E_CursorDir::Down_Right }
				};

				int x, y;

				if (rc.left + m_wCursorMoveOffset > mousePos.x)
					x = -1;
				else if (rc.right - m_wCursorMoveOffset < mousePos.x)
					x = 1;
				else
					x = 0;

				if (-rc.bottom + m_wCursorMoveOffset > mousePos.y)
					y = 1;
				else if (rc.top - m_wCursorMoveOffset < mousePos.y)
					y = -1;
				else
					y = 0;

				m_CursorDir = Dir[y + 1][x + 1];

				FLOAT speed = Camera->speed * _deltaTime;

				Camera->MoveCamera(D3DXVECTOR2(x * speed, -y * speed));
			}
		}

#pragma endregion

#pragma region Drag

		if (Input->GetMouseDown(E_MouseCode::Left))
		{
			this->SetCursorState(E_CursorState::Drag);
		}
		else if (Input->GetMouseUp(E_MouseCode::Left))
		{
			this->SetCursorState(E_CursorState::Idle);
		}

#pragma endregion // Drag

#pragma endregion // Mouse State


		return S_OK;
	}
	HRESULT C_Cursor::Render()
	{
		std::shared_ptr<C_Texture> texture = C_TextureManager::GetI()->GetTexture("Cursor");

		RECT rect = C_SelectManager::GetI()->GetTextureRect(m_CursorState, m_CursorDir, m_AnimIndex);

		Sprite->Begin();

		Sprite->SetTranslation(*m_pPosition);
		Sprite->SetRotation(nullptr);
		Sprite->SetScale(nullptr);

		Sprite->Draw(
			texture->GetTexture(),
			&rect,
			m_pHotspot,
			D3DCOLOR_XRGB(255, 255, 255)
		);

		Sprite->End();

		return S_OK;
	}
}