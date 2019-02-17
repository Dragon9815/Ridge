#include "RidgePCH.h"
#include "Win32Timer.h"

#include <ctime>

namespace Ridge{
	/*Time* Time::s_instance = new WindowsTime();

	WindowsTime::WindowsTime()
	{
		uint64_t freq;
		if (QueryPerformanceFrequency((LARGE_INTEGER*)&freq)) {
			m_freq = freq;
			m_PCAvailable = true;
		} 
		else {
			m_freq = 1000;
			m_PCAvailable = false;
		}
	}

	WindowsTime::~WindowsTime()
	{

	}

	float WindowsTime::GetSecondsImpl() 
	{
		uint64_t value;

		if (m_PCAvailable) {
			QueryPerformanceCounter((LARGE_INTEGER*)&value);
		}
		else {
			time_t time0;
			time(&time0);
			value = static_cast<uint64_t>(time0);
		}

		return static_cast<float>(value) / m_freq;
	}*/

	Timer* Timer::Create() {
		return new WindowsTimer();
	}

	WindowsTimer::WindowsTimer()
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		m_frequency = 1.0 / frequency.QuadPart;

		Reset();
	}

	void WindowsTimer::Reset()
	{
		QueryPerformanceCounter(&m_start);
	}

	double WindowsTimer::Elapsed()
	{
		LARGE_INTEGER current;
		QueryPerformanceCounter(&current);
		UINT64 cycles = current.QuadPart - m_start.QuadPart;
		return (double)(cycles * m_frequency);
	}

	double WindowsTimer::ElapsedMillis()
	{
		return Elapsed() * 1000.0f;
	}

}