#pragma once

namespace CoreEngine
{
	template <class T>
	class C_Singleton;

	class C_Texture;
	class C_BoxCollider;
}
namespace Game
{
	class C_Cursor;
	class C_Unit;

	enum class E_CursorState : unsigned char;
	enum class E_CursorDir : unsigned char;
}

namespace Game
{	
	using namespace CoreEngine;

	class C_SelectManager : public C_Singleton<C_SelectManager>, public IFrameWork, public IUpdatable, public IRenderable
	{
		friend class C_Singleton<C_SelectManager>;

	private:
		C_SelectManager();
		virtual ~C_SelectManager();

		C_SelectManager(const C_SelectManager& _other) = delete;
		void operator=(const C_SelectManager& _other) = delete;

	public:
		virtual HRESULT Create() override;
		virtual void	Destroy() override;

		virtual HRESULT Update(const FLOAT& _deltaTime) override;
		virtual HRESULT Render() override;

	private: // Cursor
		std::shared_ptr<C_Texture> m_pCursorTexture;
		C_Cursor* m_pCursor;
		RECT m_rcCursorSize;
		std::unordered_map<E_CursorState, std::pair<WORD, RECT>>* m_pCursorTextureRect;

	private: // Drag
		RECT m_DragRect;
		C_BoxCollider* m_pDragCollider;

	private: // Selected Unit
		std::priority_queue<C_Unit>* m_pSelectedUnitList;

	private:
		void SetTextureRect(const E_CursorState& _state, const RECT& _rect);
		void SetTextureRect(const E_CursorState& _state, const WORD& _maxIndex, const RECT& _rect);
		void SetTextureRect(const E_CursorState& _state, const std::pair<WORD, RECT>& _condition);

	public:
		std::shared_ptr<C_Texture> GetTexture();
		RECT GetTextureRect(const E_CursorState& _state, WORD& _index);
		RECT GetTextureRect(const E_CursorDir& _state, WORD& _index);
	};
}