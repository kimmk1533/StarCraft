#pragma once

namespace Game
{
	using namespace CoreEngine;

	enum class E_CursorState : unsigned char
	{
		Idle,
		Drag,
		Selectable_Ally,
		Selectable_Neutral,
		Selectable_Enemy,
		Move,

		Max
	};
	enum class E_CursorDir : unsigned char
	{
		Down,
		Down_Left,
		Left,
		Up_Left,
		Up,
		Up_Right,
		Right,
		Down_Right,

		Max
	};

	class C_Cursor : public C_FrameWork, public IUpdatable, public IRenderable
	{
	public:
		C_Cursor();
		~C_Cursor();

	private: // Animation
		unsigned short m_AnimIndex;
		float m_AnimIndexTimer;
		E_CursorState m_CursorState;
		E_CursorDir m_CursorDir;
		D3DXVECTOR3* m_Hotspot;
		D3DXVECTOR2* m_Position;

	public:
		virtual HRESULT	Create() override;
		virtual HRESULT Update(float _deltaTime) override;
		virtual HRESULT	Render() override;
		virtual void	Destroy() override;

	public:
		void SetCursorState(E_CursorState _state);

	};
}