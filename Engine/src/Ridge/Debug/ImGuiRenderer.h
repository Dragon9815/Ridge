#pragma once

#include "Ridge/Common.h"

struct ImDrawData;

namespace Ridge {
	class RIDGE_API ImGuiRenderer 
	{
	public:
		ImGuiRenderer() {}
		virtual ~ImGuiRenderer() {}

		virtual bool Init() = 0;
		virtual void Shutdown() = 0;
		virtual void NewFrame() = 0;
		virtual void RenderDrawData(ImDrawData* drawData) = 0;

		//static ImGuiRenderer* Create();
	};
}