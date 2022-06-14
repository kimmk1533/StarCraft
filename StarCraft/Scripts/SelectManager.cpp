#include "stdafx.h"
#include "SelectManager.h"

#include "Cursor.h"

namespace Game
{
	C_SelectManager::C_SelectManager()
	{
		m_pCursorTexture = nullptr;
		m_pCursorTextureRect = nullptr;
		m_pCursor = nullptr;
	}
	C_SelectManager::~C_SelectManager()
	{
		Destroy();
	}

	HRESULT C_SelectManager::Create()
	{
		m_pCursorTexture = std::make_shared<C_Texture>();
		if (FAILED(m_pCursorTexture->Init(TEXT("Cursor.png"))))
			return E_FAIL;
		if (FAILED(m_pCursorTexture->Create()))
		{
			m_pCursorTexture = nullptr;

			MessageBox(NULL, TEXT("Error: Cursor Texture Create Failed"), TEXT("Error!"), MB_OK);

			return E_FAIL;
		}

		m_rcCursorSize = RECT{ 0, 0, 41, 41 };

		m_pCursorTextureRect = new std::unordered_map<E_CursorState, std::pair<WORD, RECT>>();
		{
			RECT rect = RECT{ 0, 0, m_rcCursorSize.right * 5, m_rcCursorSize.bottom };
			this->SetTextureRect(E_CursorState::Idle, rect);

			rect.left = m_rcCursorSize.right * 5; rect.right = m_rcCursorSize.right * 6;
			this->SetTextureRect(E_CursorState::Drag, rect);

			rect.top = m_rcCursorSize.bottom * 1; rect.bottom = m_rcCursorSize.bottom * 2;
			rect.left = m_rcCursorSize.right * 2; rect.right = m_rcCursorSize.right * 16;
			this->SetTextureRect(E_CursorState::Selectable_Ally, rect);

			rect.top = m_rcCursorSize.bottom * 2; rect.bottom = m_rcCursorSize.bottom * 3;
			rect.left = m_rcCursorSize.right * 2; rect.right = m_rcCursorSize.right * 16;
			this->SetTextureRect(E_CursorState::Selectable_Neutral, rect);

			rect.top = m_rcCursorSize.bottom * 3; rect.bottom = m_rcCursorSize.bottom * 4;
			rect.left = m_rcCursorSize.right * 2; rect.right = m_rcCursorSize.right * 16;
			this->SetTextureRect(E_CursorState::Selectable_Enemy, rect);

			rect.top = m_rcCursorSize.bottom * 4; rect.bottom = m_rcCursorSize.bottom * 5;
			rect.left = m_rcCursorSize.right * 0; rect.right = m_rcCursorSize.right * 16;
			this->SetTextureRect(E_CursorState::Move, rect);
		}

		m_pCursor = new C_Cursor();
		SAFE_CREATE(m_pCursor);
		m_pCursor->SetCursorState(E_CursorState::Idle);

		return S_OK;
	}
	void C_SelectManager::Destroy()
	{
		SAFE_DELETE(m_pCursor);

		SAFE_DELETE(m_pCursorTextureRect);
		m_pCursorTexture.reset();
	}

	HRESULT C_SelectManager::Update(const FLOAT& _deltaTime)
	{
		if (Input->BtnDown(E_KeyCode::MouseLeftButton))
		{
			m_pCursor->SetCursorState(E_CursorState::Drag);

			// ┌
			m_arrDragPos[0] = m_arrDragPos[4] = Input->GetMousePos2();
		}
		else if (Input->BtnPress(E_KeyCode::MouseLeftButton))
		{
			// ┘
			m_arrDragPos[2] = Input->GetMousePos2();

			// ┐
			m_arrDragPos[1].x = m_arrDragPos[2].x;
			m_arrDragPos[1].y = m_arrDragPos[0].y;

			// └
			m_arrDragPos[3].x = m_arrDragPos[0].x;
			m_arrDragPos[3].y = m_arrDragPos[2].y;
		}
		else if (Input->BtnUp(E_KeyCode::MouseLeftButton))
		{
			m_pCursor->SetCursorState(E_CursorState::Idle);

			// 유닛 선택
		}

		SAFE_UPDATE(m_pCursor);

		return S_OK;
	}
	HRESULT C_SelectManager::Render()
	{
		if (Input->BtnPress(E_KeyCode::MouseLeftButton))
		{
			Sprite->DrawLine(m_arrDragPos, 5, D3DCOLOR_XRGB(0, 255, 0), 1.0f, false);
		}

		SAFE_RENDER(m_pCursor);

		return S_OK;
	}

	void C_SelectManager::SetTextureRect(const E_CursorState& _state, const RECT& _rect)
	{
		int width = _rect.right - _rect.left;
		int size = m_rcCursorSize.right - m_rcCursorSize.left;

		if (width <= 0)
		{
			MessageBox(nullptr, TEXT("Error: CursorTexture Rect Width"), TEXT("Error!"), MB_OK);
			return;
		}

		this->SetTextureRect(_state, width / size, _rect);
	}
	void C_SelectManager::SetTextureRect(const E_CursorState& _state, const WORD& _maxIndex, const RECT& _rect)
	{
		this->SetTextureRect(_state, { _maxIndex, _rect });
	}
	void C_SelectManager::SetTextureRect(const E_CursorState& _state, const std::pair<WORD, RECT>& _condition)
	{
		(*m_pCursorTextureRect)[_state] = _condition;
	}

	std::shared_ptr<C_Texture> C_SelectManager::GetTexture()
	{
		return m_pCursorTexture;
	}
	RECT C_SelectManager::GetTextureRect(const E_CursorState& _state, WORD& _index)
	{
		std::pair<WORD, RECT> index_rect = (*m_pCursorTextureRect)[_state];
		RECT rect = index_rect.second;

		if (index_rect.first <= _index)
		{
			_index = 0;
		}

		if (index_rect.first > 1)
		{
			int size = m_rcCursorSize.right - m_rcCursorSize.left;

			rect.left += _index * size;
			rect.right = rect.left + size;
		}

		return rect;
	}
	RECT C_SelectManager::GetTextureRect(const E_CursorDir& _state, WORD& _index)
	{
		std::pair<WORD, RECT> index_rect = (*m_pCursorTextureRect)[E_CursorState::Move];
		RECT rect = index_rect.second;

		if (2 <= _index)
		{
			_index = 0;
		}

		int size = m_rcCursorSize.right - m_rcCursorSize.left;
		WORD dir = static_cast<WORD>(_state) * 2;

		rect.left += (_index + dir) * size;
		rect.right = rect.left + size;

		return rect;
	}
}