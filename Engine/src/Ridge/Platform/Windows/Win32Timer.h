#pragma once

#include "Ridge/Util/Timer.h"

namespace Ridge {
	class WindowsTimer : public Timer {
	public:
		WindowsTimer();

		void Reset() override;
		double Elapsed() override;
		double ElapsedMillis() override;

	private:
		LARGE_INTEGER m_start;
		double m_frequency;
	};
}