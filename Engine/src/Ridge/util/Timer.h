#pragma once

#include "Ridge/Common.h"

namespace Ridge {
	class RIDGE_API Timer {
	public:
		Timer() {}
		virtual ~Timer() {}

		virtual void Reset() = 0;
		virtual double Elapsed() = 0;
		virtual double ElapsedMillis() = 0;

		static Timer* Create();
	};
}