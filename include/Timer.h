#pragma once
#ifndef _TIMER_H_
#define _TIMER_H_

#include <Windows.h>

class Timer {
public:
	Timer();
	~Timer();

	double Tick();
	long long GetFrequency();
	SYSTEMTIME GetStartUTC();
private:
	long long m_frequency;
	SYSTEMTIME m_startUTC;
	long long m_startCounts;
	double m_lastElapsedTime;
	LARGE_INTEGER m_lastTickCounts;
};
#endif // !_TIMER_H_
