#include "stdafx_Core.h"
#include "Animator.h"

namespace CoreEngine
{
	void C_Animator::SetSampleRate(const FLOAT& _sampleRate)
	{
		m_pAnimTimer->SetTime(_sampleRate);
	}
	void C_Animator::AddFunc(const DWORD& _index, function_update _func)
	{
		m_dwFuncCount = max(_index + 1, m_dwFuncCount);

		m_pFuncDictionary->insert({ _index, _func });
	}
	void C_Animator::SetRenderFunc(function_render _func)
	{
		*m_pRenderFunc = _func;
	}

	C_Animator::C_Animator()
	{
		m_pAnimTimer = nullptr;

		m_dwFuncIndex = 1;
		m_dwFuncCount = 1;

		m_pFuncDictionary = nullptr;
		m_pRenderFunc = nullptr;
	}
	C_Animator::~C_Animator()
	{
		Destroy();
	}

	HRESULT C_Animator::Create()
	{
		m_pAnimTimer = std::make_unique<C_Timer>();
		m_pAnimTimer->Play();

		if (nullptr == m_pFuncDictionary)
			m_pFuncDictionary = std::make_unique<std::unordered_multimap<DWORD, function_update>>();
		if (nullptr == m_pRenderFunc)
			m_pRenderFunc = std::make_unique<function_render>();

		return S_OK;
	}
	void C_Animator::Destroy()
	{
		m_pRenderFunc = nullptr;

		m_pFuncDictionary->clear();
		m_pFuncDictionary = nullptr;

		m_pAnimTimer = nullptr;
	}

	HRESULT C_Animator::Update(const FLOAT& _deltaTime)
	{
		if (m_dwFuncCount <= 0)
			return S_OK;

		if (FAILED(m_pAnimTimer->Update(_deltaTime)))
			return S_OK;

		for (auto iter = m_pFuncDictionary->lower_bound(0); iter != m_pFuncDictionary->upper_bound(0); ++iter)
		{
			(*iter).second();
		}
		for (auto iter = m_pFuncDictionary->lower_bound(m_dwFuncIndex); iter != m_pFuncDictionary->upper_bound(m_dwFuncIndex); ++iter)
		{
			(*iter).second();
		}

		++m_dwFuncIndex;

		if (m_dwFuncIndex >= m_dwFuncCount)
			m_dwFuncIndex = 1;

		return S_OK;
	}
	HRESULT C_Animator::Render()
	{
		if (m_pRenderFunc != nullptr)
			FAILED_CHECK_RETURN((*m_pRenderFunc)());

		return S_OK;
	}
}