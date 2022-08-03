#include "stdafx.h"
#include "SelectManager.h"

#include <BoxCollider.h>
#include <Physics.h>

#include "Cursor.h"

#include "UnitManager.h"
#include "MarineManager.h"

namespace Game
{
	C_SelectManager::C_SelectManager()
	{
		m_pCursorTexture = nullptr;
		m_pCursorTextureRect = nullptr;
		m_pCursor = nullptr;

		m_pDragCollider = nullptr;
		m_pSelectedUnitList = nullptr;
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

		m_pDragCollider = new C_BoxCollider();
		SAFE_CREATE(m_pDragCollider);

		return S_OK;
	}
	void C_SelectManager::Destroy()
	{
		SAFE_DELETE(m_pDragCollider);

		SAFE_DELETE(m_pCursor);

		SAFE_DELETE(m_pCursorTextureRect);

		m_pCursorTexture.reset();
	}

	HRESULT C_SelectManager::Update(const FLOAT& _deltaTime)
	{
		if (Input->GetMouseDown(E_MouseCode::Left))
		{
			// ┌
			m_DragRect.left = Input->GetMousePos().x + Camera->position.x;
			m_DragRect.top = Input->GetMousePos().y + Camera->position.y;
		}
		else if (Input->GetMouse(E_MouseCode::Left))
		{
			// ┘
			m_DragRect.right = Input->GetMousePos().x + Camera->position.x;
			m_DragRect.bottom = Input->GetMousePos().y + Camera->position.y;
		}
		else if (Input->GetMouseUp(E_MouseCode::Left))
		{
			if (m_DragRect.left > m_DragRect.right)
				swap(m_DragRect.left, m_DragRect.right);
			if (m_DragRect.top > m_DragRect.bottom)
				swap(m_DragRect.top, m_DragRect.bottom);

			// 유닛 선택
			m_pDragCollider->bounds->size = D3DXVECTOR3(
				m_DragRect.right - m_DragRect.left,
				m_DragRect.bottom - m_DragRect.top,
				0.0f
			);
			m_pDragCollider->bounds->center = D3DXVECTOR3(
				m_DragRect.left + m_DragRect.right,
				m_DragRect.top + m_DragRect.bottom,
				0.0f
			) * 0.5f;

			auto marineList = C_MarineManager::GetI()->GetSpawnedObjList();
			for (size_t i = 0; i < marineList->size(); ++i)
			{
				if (Physics::CollisionCheck(*m_pDragCollider->bounds, (*(*marineList)[i]->collider->bounds)))
					std::cout << "충돌\n";

#ifdef TEST
				if (i == 0)
				{
					S_Bounds* bounds = (*marineList)[i]->collider->bounds;
					std::cout << "unit_min (" << bounds->min.x << ", " << bounds->min.y << ")\n";
					std::cout << "unit_max (" << bounds->max.x << ", " << bounds->max.y << ")\n";
					std::cout << "m_arrDragPos[0] (" << m_arrDragPos[0].x << ", " << m_arrDragPos[0].y << ")\n";
					std::cout << "m_arrDragPos[1] (" << m_arrDragPos[1].x << ", " << m_arrDragPos[1].y << ")\n\n";
				}
#endif // TEST

			} // for (size_t i = 0; i < marineList->size(); ++i)
		} // else if (Input->GetMouseUp(E_MouseCode::Left))

		SAFE_UPDATE(m_pCursor);

		return S_OK;
	}
	HRESULT C_SelectManager::Render()
	{
		if (Input->GetMouse(E_MouseCode::Left))
		{
			FAILED_CHECK_RETURN(Sprite->DrawRect(
				m_DragRect,
				1.0f,
				false,
				D3DCOLOR_XRGB(0, 255, 0)
			));
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