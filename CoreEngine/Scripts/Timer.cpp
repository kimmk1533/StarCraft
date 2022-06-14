#include "stdafx_Core.h"
#include "Timer.h"

namespace CoreEngine
{
	C_Timer::C_Timer()
	{
		m_Time = 0.0f;
		m_Timer = 0.0f;
		m_isPlaying = false;
	}
	C_Timer::~C_Timer()
	{
		
	}

	HRESULT C_Timer::Update(const FLOAT& _deltaTime)
	{
		if (!m_isPlaying)
			return E_FAIL;

		m_Timer += _deltaTime;
		if (m_Timer >= m_Time)
		{
			m_Timer -= m_Time;
			return S_OK;
		}

		return E_FAIL;
	}

	void C_Timer::SetTime(const FLOAT& _time)
	{
		m_Time = _time;
	}
	void C_Timer::Play()
	{
		m_isPlaying = true;
	}
	void C_Timer::Pause()
	{
		m_isPlaying = false;
	}
}