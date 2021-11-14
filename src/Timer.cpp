#include "Timer.h"

Timer::Timer()
{
	LARGE_INTEGER li = { 0 };
	QueryPerformanceFrequency(&li);
	m_frequency = li.QuadPart;

	li = { 0 };
	QueryPerformanceCounter(&li);
	m_startCounts = li.QuadPart;

	m_lastTickCounts = static_cast<LARGE_INTEGER>(li);
	
	GetSystemTime(&m_startUTC);
}

double Timer::Tick()
{
	LARGE_INTEGER li = { 0 };
	QueryPerformanceCounter(&li);
	m_lastElapsedTime = static_cast<double>(li.QuadPart - m_lastTickCounts.QuadPart) / m_frequency * 1000.0f;
	m_lastTickCounts = static_cast<LARGE_INTEGER>(li);
	
	return m_lastElapsedTime;
}

SYSTEMTIME Timer::GetStartUTC()
{
	return m_startUTC;
}