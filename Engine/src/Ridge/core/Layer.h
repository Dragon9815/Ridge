#pragma once

#include "RidgePCH.h"

namespace Ridge {
	class RIDGE_API Layer
	{
	public:
		Layer(const std::string& name = "Unnamed Layer")
			: m_debugName(name) {}
		virtual ~Layer() {}

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(double deltaTime) {}
		virtual void OnRender(double deltaTime) {}
		virtual void OnEvent(Event& event) {}

		inline std::string GetDebugName() const { return m_debugName; }
	private:
		std::string m_debugName;
	};
}