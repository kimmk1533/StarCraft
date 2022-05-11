#pragma once

namespace Game
{
	using namespace CoreEngine;

	class C_Marine;

	class C_Main : public C_Engine
	{
	private:
		C_Marine** m_Marine;

	public:
		// Window+Device 관련 함수들
		virtual HRESULT	Init(HINSTANCE _hInst) override;

	public:
		virtual HRESULT	Create() override;
		virtual HRESULT	Update(const float _deltaTime) override;
		virtual HRESULT	Render() override;
		virtual void	Destroy() override;

	};
}