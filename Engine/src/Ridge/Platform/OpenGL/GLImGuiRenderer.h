#pragma once

#include "Ridge/Debug/ImGuiRenderer.h"

#include <glad/glad.h>

namespace Ridge { namespace Graphics {
	class GLImGuiRenderer : public ImGuiRenderer 
	{
	public:
		GLImGuiRenderer();
		~GLImGuiRenderer();

		bool Init() override;
		void Shutdown() override;
		void NewFrame() override;
		void RenderDrawData(ImDrawData* drawData) override;

	private:
		bool CreateFontsTexture();
		void DestroyFontsTexture();
		bool CreateDeviceObjects();
		void DestroyDeviceObjects();

		bool CheckShader(GLuint handle, const char* desc);
		bool CheckProgram(GLuint handle, const char* desc);
	private:
		char         m_GlslVersionString[32] = "";
		GLuint       m_FontTexture = 0;
		GLuint       m_ShaderHandle = 0, m_VertHandle = 0, m_FragHandle = 0;
		int          m_AttribLocationTex = 0, m_AttribLocationProjMtx = 0;
		int          m_AttribLocationPosition = 0, m_AttribLocationUV = 0, m_AttribLocationColor = 0;
		unsigned int m_VboHandle = 0, m_ElementsHandle = 0;
	};

	/*bool ImGui_ImplOpenGL3_Init(const char* glsl_version = nullptr);
	void ImGui_ImplOpenGL3_Shutdown();
	void ImGui_ImplOpenGL3_NewFrame();
	void ImGui_ImplOpenGL3_RenderDrawData(ImDrawData* draw_data);

	bool ImGui_ImplOpenGL3_CreateFontsTexture();
	void ImGui_ImplOpenGL3_DestroyFontsTexture();
	bool ImGui_ImplOpenGL3_CreateDeviceObjects();
	void ImGui_ImplOpenGL3_DestroyDeviceObjects();*/
}
}