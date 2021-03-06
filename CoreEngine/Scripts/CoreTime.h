#pragma once

// 파일명이 Time.h인 경우 std라이브러리의 time.h와 충돌을 일으킴
// 참고: https://stackoverflow.com/questions/23907008/compilation-error-error-c2039-clock-t-is-not-a-member-of-global-namespace

namespace CoreEngine
{
	// (주의)
	// 이 함수는 CPU 에 의존적이기 때문에 멀티스레드를 활용하는 프로그램에서 여러개의 CPU로 자신의
	// 스레드가 분산되었을때, 각 스레드가 동일한 시점에 QueryPerformanceCounter 함수를 사용해서
	// 값을 읽어도 서로다른 값을 가져올수 있으니, 주의해야 합니다.

	// 참고: http://www.tipssoft.com/bulletin/board.php?bo_table=FAQ&wr_id=735
	class C_Time final
	{
	public:
		C_Time();

	public:
		static void Update();

	private:
		static std::unique_ptr<LARGE_INTEGER>	m_PeriodFrequency;
		static std::unique_ptr<LARGE_INTEGER>	m_CurrentTime;
		static std::unique_ptr<LARGE_INTEGER>	m_LastTime;
		static std::unique_ptr<FLOAT>			m_DeltaTime;
		static std::unique_ptr<FLOAT>			m_TimeScale;

	public:
		static const FLOAT DeltaTime();

	};
}