#pragma once

namespace CoreEngine
{
	class C_Timer final : public IUpdatable
	{
	public:
		C_Timer();
		virtual ~C_Timer();

	public:
		HRESULT Update(const FLOAT& _deltaTime) override;

	private:
		FLOAT m_Time;
		FLOAT m_Timer;
		bool m_isPlaying;

	public:
		void SetTime(const FLOAT& _time);
		void Play();
		void Pause();

	};
}