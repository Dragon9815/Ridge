#pragma once

#include "Ridge\Common.h"

namespace Ridge {
	class RIDGE_API Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void Init();

		void Start();

	private:
	};
}