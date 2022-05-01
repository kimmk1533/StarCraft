#include "stdafx.h"

//#include "Time.h"

// 멤버 초기화
std::unique_ptr<LARGE_INTEGER>	C_Time::m_PeriodFrequency = std::make_unique<LARGE_INTEGER>();
std::unique_ptr<LARGE_INTEGER>	C_Time::m_CurrentTime = std::make_unique<LARGE_INTEGER>();
std::unique_ptr<LARGE_INTEGER>	C_Time::m_LastTime = std::make_unique<LARGE_INTEGER>();
std::unique_ptr<float>			C_Time::m_DeltaTime = std::make_unique<float>();
std::unique_ptr<float>			C_Time::m_TimeScale = std::make_unique<float>();

C_Time::C_Time()
{
	// 고해상도 타이머의 주파수를 얻는다.
	QueryPerformanceFrequency(&(*m_PeriodFrequency));

	// 시작 시점의 CPU 클럭수를 얻는다.
	QueryPerformanceCounter(&(*m_LastTime));

	// 미리  '1 / 초당진동수'  의 형태로 만들어 놓는다.
	// 컴퓨터는 나눗셈연산보다 곱셈연산이 더 빠르기때문에 매 프레임마다 연산을 해야할경우 
	// 역수로 만들어놓고 실제 계산때 곱셈하게 만들어놓는게 좋다.
	*m_TimeScale = 1.0f / ((*m_PeriodFrequency).QuadPart);
}
void C_Time::Update()
{
	// 현재 프레임 진동수 저장.
	QueryPerformanceCounter(&(*m_CurrentTime));

	// 끝 시점의 CPU 클럭수에서 시작 시점의 클럭수를 뺀 후 주파수를 나누어 준다.(생성자 주석 참고)
	*m_DeltaTime = ((*m_CurrentTime).QuadPart - (*m_LastTime).QuadPart) * (*m_TimeScale);

	// 현재 프레임을 이전 프레임으로 저장. 이후 계속 반복.
	*m_LastTime = *m_CurrentTime;
}
const float C_Time::DeltaTime()
{
	return *m_DeltaTime;
}