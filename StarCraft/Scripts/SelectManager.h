#pragma once

namespace Game
{
	using namespace CoreEngine;

	class C_Cursor;
	class CoreEngine::C_Texture;
	enum class E_CursorState : unsigned char;
	enum class E_CursorDir : unsigned char;

	class C_SelectManager : public C_Singleton<C_SelectManager>, public IFrameWork
	{
		friend class C_Singleton<C_SelectManager>;

	private:
		C_SelectManager();
		~C_SelectManager();

		C_SelectManager(const C_SelectManager& _other) = delete;
		void operator=(const C_SelectManager& _other) = delete;

	private:
		std::shared_ptr<C_Texture> m_pCursorTexture;
		C_Cursor* m_pCursor;
		RECT m_rcCursorSize;
		std::unordered_map<E_CursorState, std::pair<unsigned short, RECT>>* m_pCursorTextrueRect;

	private:
		void SetTextureRect(const E_CursorState& _state, const RECT& _rect);
		void SetTextureRect(const E_CursorState& _state, const unsigned short& _maxIndex, const RECT& _rect);
		void SetTextureRect(const E_CursorState& _state, const std::pair<unsigned short, RECT>& _condition);

	public:
		virtual HRESULT Create() override;
		virtual void	Destroy() override;

	public:
		std::shared_ptr<C_Texture> GetTexture();
		RECT GetTextureRect(const E_CursorState& _state, unsigned short& _index);
		RECT GetTextureRect(const E_CursorDir& _state, unsigned short& _index);
	};
}