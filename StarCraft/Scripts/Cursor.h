#pragma once

namespace CoreEngine
{
	class C_Animator;
	class C_Timer;
}

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
		virtual ~C_Cursor();

	public:
		virtual HRESULT	Create() override;
		virtual void	Destroy() override;

		virtual HRESULT Update(const FLOAT& _deltaTime) override;
		virtual HRESULT	Render() override;

	private: // Cursor Info
		D3DXVECTOR3* m_pHotspot;
		D3DXVECTOR2* m_pPosition;

	private: // Animation
		C_Animator* m_pAnimator;

		WORD m_AnimIndex;
		C_Timer* m_pGameFrameTimer;
		E_CursorState m_CursorState;
		E_CursorDir m_CursorDir;

	public:
		void SetCursorState(E_CursorState _state);

	};
}