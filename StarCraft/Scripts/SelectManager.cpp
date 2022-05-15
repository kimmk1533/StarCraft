#include "..\..\CoreEngine\Scripts\Singleton.cpp"

#include "SelectManager.h"
#include "Cursor.h"
#include "..\..\CoreEngine\Scripts\Texture.h"

namespace Game
{
	using namespace CoreEngine;

	C_SelectManager::C_SelectManager()
	{
		m_pCursorTexture = nullptr;
		m_pCursorTextrueRect = nullptr;
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

		m_pCursorTextrueRect = new std::unordered_map<E_CursorState, std::pair<WORD, RECT>>();
		{
			E_CursorState state = E_CursorState::Idle;
			RECT rect = RECT{ 0, 0, 41 * 5, 41 };
			this->SetTextureRect(state, rect);

			state = E_CursorState::Drag;
			rect.left = 41 * 5; rect.right = 41 * 6;
			this->SetTextureRect(state, rect);

			state = E_CursorState::Selectable_Ally;
			rect.top = 41 * 1; rect.bottom = 41 * 2;
			rect.left = 41 * 2; rect.right = 41 * 16;
			this->SetTextureRect(state, rect);

			state = E_CursorState::Selectable_Neutral;
			rect.top = 41 * 2; rect.bottom = 41 * 3;
			rect.left = 41 * 2; rect.right = 41 * 16;
			this->SetTextureRect(state, rect);

			state = E_CursorState::Selectable_Enemy;
			rect.top = 41 * 3; rect.bottom = 41 * 4;
			rect.left = 41 * 2; rect.right = 41 * 16;
			this->SetTextureRect(state, rect);

			state = E_CursorState::Move;
			rect.top = 41 * 4; rect.bottom = 41 * 5;
			rect.left = 41 * 0; rect.right = 41 * 16;
			this->SetTextureRect(state, rect);

		}

		m_pCursor = new C_Cursor();
		if (FAILED(m_pCursor->Create()))
			return E_FAIL;

		return S_OK;
	}
	void C_SelectManager::Destroy()
	{
		SAFE_DELETE(m_pCursorTextrueRect);
		m_pCursorTexture.reset();
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
		(*m_pCursorTextrueRect)[_state] = _condition;
	}

	std::shared_ptr<C_Texture> C_SelectManager::GetTexture()
	{
		return m_pCursorTexture;
	}
	RECT C_SelectManager::GetTextureRect(const E_CursorState& _state, WORD& _index)
	{
		std::pair<WORD, RECT> index_rect = (*m_pCursorTextrueRect)[_state];
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
		std::pair<WORD, RECT> index_rect = (*m_pCursorTextrueRect)[E_CursorState::Move];
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