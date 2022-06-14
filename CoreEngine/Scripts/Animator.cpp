#include "stdafx_Core.h"
#include "Animator.h"

#include "Timer.h"

namespace CoreEngine
{
	C_Animator::C_Animator()
	{
		m_pAnimTimer = nullptr;

		m_dwFuncIndex = 1;
		m_dwFuncCount = 1;
		m_pFuncList = nullptr;
	}
	C_Animator::~C_Animator()
	{
		Destroy();
	}

	HRESULT C_Animator::Create()
	{
		m_pAnimTimer = new C_Timer();
		m_pAnimTimer->Play();

		m_pFuncList = new std::unordered_multimap<DWORD, std::function<void()>>();

		return S_OK;
	}
	void C_Animator::Destroy()
	{
		SAFE_DELETE(m_pFuncList);

		SAFE_DELETE(m_pAnimTimer);
	}

	HRESULT C_Animator::Update(const FLOAT& _deltaTime)
	{
		if (m_dwFuncCount <= 0)
			return S_OK;

		if (FAILED(m_pAnimTimer->Update(_deltaTime)))
			return S_OK;

		for (auto iter = m_pFuncList->lower_bound(0); iter != m_pFuncList->upper_bound(0); ++iter)
		{
			(*iter).second();
		}
		for (auto iter = m_pFuncList->lower_bound(m_dwFuncIndex); iter != m_pFuncList->upper_bound(m_dwFuncIndex); ++iter)
		{
			(*iter).second();
		}

		++m_dwFuncIndex;

		if (m_dwFuncIndex >= m_dwFuncCount)
			m_dwFuncIndex = 1;

		return S_OK;
	}

	void C_Animator::SetSampleRate(const FLOAT& _sampleRate)
	{
		m_pAnimTimer->SetTime(_sampleRate);
	}

	void C_Animator::AddFunc(DWORD _index, std::function<void()> _func)
	{
		if (_index + 1 > m_dwFuncCount)
			m_dwFuncCount = _index + 1;

		m_pFuncList->insert({ _index, _func});
	}
}