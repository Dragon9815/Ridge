#pragma once

#include "Ridge/Common.h"
#include "Ridge/Core/Window.h"
#include "Ridge/Debug/ImGuiRenderer.h"
#include "Renderer.h"
#include "Shader.h"

namespace Ridge {
	enum class RIDGE_API RenderAPI {
		NONE, OPENGL, DIRECT3D
	};

	class RIDGE_API Context {
	public:
		virtual bool Init() = 0;
		virtual void SwapBuffersImpl() = 0;

		virtual Renderer* CreateRendererImpl() = 0;
		virtual ImGuiRenderer* CreateImGuiRendererImpl() = 0;
		virtual ShaderProgram* CreateShaderProgramImpl() = 0;

		static bool Create(WindowProps windowProps, void* deviceHandle);

		static RenderAPI GetRenderAPI() { return s_renderAPI; }
		static void SetRenderAPI(RenderAPI api) { s_renderAPI = api; }

		static void SwapBuffers() { s_context->SwapBuffersImpl(); }

		static Renderer* CreateRenderer() { return s_context->CreateRendererImpl(); }
		static ImGuiRenderer* CreateImGuiRenderer() { return s_context->CreateImGuiRendererImpl(); }
		static ShaderProgram* CreateShaderProgram() { return s_context->CreateShaderProgramImpl(); } 
	protected:
		static Context* s_context;
		static RenderAPI s_renderAPI;
	};
}