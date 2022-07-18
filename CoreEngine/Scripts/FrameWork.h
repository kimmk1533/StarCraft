#pragma once

#define Sprite CoreEngine::C_FrameWork::m_pSprite
#define Input CoreEngine::C_FrameWork::m_pInput

namespace CoreEngine
{
	class C_Time;
	class C_Sprite;
	class C_Input;
}

namespace CoreEngine
{
	__interface IFrameWork
	{
		virtual HRESULT Create() = 0;
		virtual void	Destroy() = 0;
	};
	__interface IUpdatable
	{
		virtual HRESULT Update(const FLOAT& _deltaTime) = 0;
	};
	__interface IRenderable
	{
		virtual HRESULT Render() = 0;
	};

	class C_FrameWork : public IFrameWork
	{
	protected:
		// 항상 포인터들은 NULL로 초기화
		static D3DPRESENT_PARAMETERS	m_d3dpp;			// D3D Parameter
		static LPDIRECT3D9				m_pD3D;				// D3D
		static LPDIRECT3DDEVICE9		m_pd3dDevice;		// Device

	public:
		static C_Sprite*				m_pSprite;			// 2D를 출력하기 위한 객체
		static C_Input*					m_pInput;			// 입력을 위한 객체

	public:
		// 게임 관련 함수들
		virtual HRESULT Create() override = 0;
		virtual void	Destroy() override = 0;

	};
}