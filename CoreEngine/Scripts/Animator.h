#pragma once
#include "Timer.h"

namespace CoreEngine
{
	class C_Animator : public C_FrameWork, public IUpdatable, public IRenderable
	{
	protected:
		using function_update = std::function<void()>;
		using function_render = std::function<HRESULT()>;

		std::unique_ptr<C_Timer> m_pAnimTimer;

		DWORD m_dwFuncIndex;
		DWORD m_dwFuncCount;

		std::unique_ptr<std::unordered_multimap<DWORD, function_update>> m_pFuncDictionary;
		std::unique_ptr<function_render> m_pRenderFunc;

	public:
		void SetSampleRate(const FLOAT& _sampleRate);
		/// <summary>
		/// 애니메이션 함수 추가
		/// </summary>
		/// <param name="_index">인덱스(0 == 매 프래임마다)</param>
		/// <param name="_func">함수</param>
		void AddFunc(const DWORD& _index, function_update _func);
		void SetRenderFunc(function_render _func);

	public:
		C_Animator();
		virtual ~C_Animator();

	public:
		virtual HRESULT Create() override;
		virtual void	Destroy() override;

		virtual HRESULT Update(const FLOAT& _deltaTime) override;
		virtual HRESULT Render() override;

	};
}