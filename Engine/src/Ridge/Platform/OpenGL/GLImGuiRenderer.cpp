// dear imgui: Renderer for OpenGL3 / OpenGL ES2 / OpenGL ES3 (modern OpenGL with shaders / programmatic pipeline)
// This needs to be used along with a Platform Binding (e.g. GLFW, SDL, Win32, custom..)
// (Note: We are using GL3W as a helper library to access OpenGL functions since there is no standard header to access modern OpenGL functions easily. Alternatives are GLEW, Glad, etc..)

// Implemented features:
//  [X] Renderer: User texture binding. Use 'GLuint' OpenGL texture identifier as void*/ImTextureID. Read the FAQ about ImTextureID in imgui.cpp.

// You can copy and use unmodified imgui_impl_* files in your project. See main.cpp for an example of using this.
// If you are new to dear imgui, read examples/README.txt and read the documentation at the top of imgui.cpp.
// https://github.com/ocornut/imgui

// CHANGELOG
// (minor and older changes stripped away, please see git history for details)
//  2018-11-30: Misc: Setting up io.BackendRendererName so it can be displayed in the About Window.
//  2018-11-13: OpenGL: Support for GL 4.5's glClipControl(GL_UPPER_LEFT).
//  2018-08-29: OpenGL: Added support for more OpenGL loaders: glew and glad, with comments indicative that any loader can be used.
//  2018-08-09: OpenGL: Default to OpenGL ES 3 on iOS and Android. GLSL version default to "#version 300 ES".
//  2018-07-30: OpenGL: Support for GLSL 300 ES and 410 core. Fixes for Emscripten compilation.
//  2018-07-10: OpenGL: Support for more GLSL versions (based on the GLSL version string). Added error output when shaders fail to compile/link.
//  2018-06-08: Misc: Extracted imgui_impl_opengl3.cpp/.h away from the old combined GLFW/SDL+OpenGL3 examples.
//  2018-06-08: OpenGL: Use draw_data->DisplayPos and draw_data->DisplaySize to setup projection matrix and clipping rectangle.
//  2018-05-25: OpenGL: Removed unnecessary backup/restore of GL_ELEMENT_ARRAY_BUFFER_BINDING since this is part of the VAO state.
//  2018-05-14: OpenGL: Making the call to glBindSampler() optional so 3.2 context won't fail if the function is a NULL pointer.
//  2018-03-06: OpenGL: Added const char* glsl_version parameter to ImGui_ImplOpenGL3_Init() so user can override the GLSL version e.g. "#version 150".
//  2018-02-23: OpenGL: Create the VAO in the render function so the setup can more easily be used with multiple shared GL context.
//  2018-02-16: Misc: Obsoleted the io.RenderDrawListsFn callback and exposed ImGui_ImplSdlGL3_RenderDrawData() in the .h file so you can call it yourself.
//  2018-01-07: OpenGL: Changed GLSL shader version from 330 to 150.
//  2017-09-01: OpenGL: Save and restore current bound sampler. Save and restore current polygon mode.
//  2017-05-01: OpenGL: Fixed save and restore of current blend func state.
//  2017-05-01: OpenGL: Fixed save and restore of current GL_ACTIVE_TEXTURE.
//  2016-09-05: OpenGL: Fixed save and restore of current scissor rectangle.
//  2016-07-29: OpenGL: Explicitly setting GL_UNPACK_ROW_LENGTH to reduce issues because SDL changes it. (#752)

//----------------------------------------
// OpenGL    GLSL      GLSL
// version   version   string
//----------------------------------------
//  2.0       110       "#version 110"
//  2.1       120
//  3.0       130
//  3.1       140
//  3.2       150       "#version 150"
//  3.3       330
//  4.0       400
//  4.1       410       "#version 410 core"
//  4.2       420
//  4.3       430
//  ES 2.0    100       "#version 100"
//  ES 3.0    300       "#version 300 es"
//----------------------------------------
#include "RidgePCH.h"

#include "GLImGuiRenderer.h"
#include "imgui.h"

namespace Ridge { namespace Graphics {
	GLImGuiRenderer::GLImGuiRenderer()
	{

	}

	GLImGuiRenderer::~GLImGuiRenderer()
	{

	}

	bool GLImGuiRenderer::Init()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.BackendRendererName = "imgui_impl_opengl3";

		// Store GLSL version string so we can refer to it later in case we recreate shaders. Note: GLSL version is NOT the same as GL version. Leave this to NULL if unsure.
		const char* glsl_version = "#version 130";

		IM_ASSERT((int)strlen(glsl_version) + 2 < IM_ARRAYSIZE(m_GlslVersionString));
		strcpy(m_GlslVersionString, glsl_version);
		strcat(m_GlslVersionString, "\n");

		return true;
	}

	void GLImGuiRenderer::Shutdown()
	{
		DestroyDeviceObjects();
	}

	void GLImGuiRenderer::NewFrame()
	{
		if (!m_FontTexture)
			CreateDeviceObjects();
	}

	void GLImGuiRenderer::RenderDrawData(ImDrawData* drawData)
	{
		// Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
		ImGuiIO& io = ImGui::GetIO();
		int fb_width = (int)(drawData->DisplaySize.x * io.DisplayFramebufferScale.x);
		int fb_height = (int)(drawData->DisplaySize.y * io.DisplayFramebufferScale.y);
		if (fb_width <= 0 || fb_height <= 0)
			return;
		drawData->ScaleClipRects(io.DisplayFramebufferScale);

		// Backup GL state
		GLenum last_active_texture; glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&last_active_texture);
		glActiveTexture(GL_TEXTURE0);
		GLint last_program; glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
		GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
#ifdef GL_SAMPLER_BINDING
		GLint last_sampler; glGetIntegerv(GL_SAMPLER_BINDING, &last_sampler);
#endif
		GLint last_array_buffer; glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
		GLint last_vertex_array; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
#ifdef GL_POLYGON_MODE
		GLint last_polygon_mode[2]; glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
#endif
		GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
		GLint last_scissor_box[4]; glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
		GLenum last_blend_src_rgb; glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&last_blend_src_rgb);
		GLenum last_blend_dst_rgb; glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&last_blend_dst_rgb);
		GLenum last_blend_src_alpha; glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&last_blend_src_alpha);
		GLenum last_blend_dst_alpha; glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&last_blend_dst_alpha);
		GLenum last_blend_equation_rgb; glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*)&last_blend_equation_rgb);
		GLenum last_blend_equation_alpha; glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*)&last_blend_equation_alpha);
		GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
		GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
		GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
		GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);
		bool clip_origin_lower_left = true;
#ifdef GL_CLIP_ORIGIN
		GLenum last_clip_origin = 0; glGetIntegerv(GL_CLIP_ORIGIN, (GLint*)&last_clip_origin); // Support for GL 4.5's glClipControl(GL_UPPER_LEFT)
		if (last_clip_origin == GL_UPPER_LEFT)
			clip_origin_lower_left = false;
#endif

		// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, polygon fill
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_SCISSOR_TEST);
#ifdef GL_POLYGON_MODE
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif

		// Setup viewport, orthographic projection matrix
		// Our visible imgui space lies from drawData->DisplayPos (top left) to drawData->DisplayPos+data_data->DisplaySize (bottom right). DisplayMin is typically (0,0) for single viewport apps.
		glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
		float L = drawData->DisplayPos.x;
		float R = drawData->DisplayPos.x + drawData->DisplaySize.x;
		float T = drawData->DisplayPos.y;
		float B = drawData->DisplayPos.y + drawData->DisplaySize.y;
		const float ortho_projection[4][4] =
		{
			{ 2.0f / (R - L),   0.0f,         0.0f,   0.0f },
			{ 0.0f,         2.0f / (T - B),   0.0f,   0.0f },
			{ 0.0f,         0.0f,        -1.0f,   0.0f },
			{ (R + L) / (L - R),  (T + B) / (B - T),  0.0f,   1.0f },
		};
		glUseProgram(m_ShaderHandle);
		glUniform1i(m_AttribLocationTex, 0);
		glUniformMatrix4fv(m_AttribLocationProjMtx, 1, GL_FALSE, &ortho_projection[0][0]);
#ifdef GL_SAMPLER_BINDING
		glBindSampler(0, 0); // We use combined texture/sampler state. Applications using GL 3.3 may set that otherwise.
#endif
	// Recreate the VAO every time
	// (This is to easily allow multiple GL contexts. VAO are not shared among GL contexts, and we don't track creation/deletion of windows so we don't have an obvious key to use to cache them.)
		GLuint vao_handle = 0;
		glGenVertexArrays(1, &vao_handle);
		glBindVertexArray(vao_handle);
		glBindBuffer(GL_ARRAY_BUFFER, m_VboHandle);
		glEnableVertexAttribArray(m_AttribLocationPosition);
		glEnableVertexAttribArray(m_AttribLocationUV);
		glEnableVertexAttribArray(m_AttribLocationColor);
		glVertexAttribPointer(m_AttribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, pos));
		glVertexAttribPointer(m_AttribLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, uv));
		glVertexAttribPointer(m_AttribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, col));

		// Draw
		ImVec2 pos = drawData->DisplayPos;
		for (int n = 0; n < drawData->CmdListsCount; n++)
		{
			const ImDrawList* cmd_list = drawData->CmdLists[n];
			const ImDrawIdx* idx_buffer_offset = 0;

			glBindBuffer(GL_ARRAY_BUFFER, m_VboHandle);
			glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementsHandle);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);

			for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
			{
				const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
				if (pcmd->UserCallback)
				{
					// User callback (registered via ImDrawList::AddCallback)
					pcmd->UserCallback(cmd_list, pcmd);
				}
				else
				{
					ImVec4 clip_rect = ImVec4(pcmd->ClipRect.x - pos.x, pcmd->ClipRect.y - pos.y, pcmd->ClipRect.z - pos.x, pcmd->ClipRect.w - pos.y);
					if (clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f)
					{
						// Apply scissor/clipping rectangle
						if (clip_origin_lower_left)
							glScissor((int)clip_rect.x, (int)(fb_height - clip_rect.w), (int)(clip_rect.z - clip_rect.x), (int)(clip_rect.w - clip_rect.y));
						else
							glScissor((int)clip_rect.x, (int)clip_rect.y, (int)clip_rect.z, (int)clip_rect.w); // Support for GL 4.5's glClipControl(GL_UPPER_LEFT)

						// Bind texture, Draw
						glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
						glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer_offset);
					}
				}
				idx_buffer_offset += pcmd->ElemCount;
			}
		}
		glDeleteVertexArrays(1, &vao_handle);

		// Restore modified GL state
		glUseProgram(last_program);
		glBindTexture(GL_TEXTURE_2D, last_texture);
#ifdef GL_SAMPLER_BINDING
		glBindSampler(0, last_sampler);
#endif
		glActiveTexture(last_active_texture);
		glBindVertexArray(last_vertex_array);
		glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
		glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
		glBlendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha, last_blend_dst_alpha);
		if (last_enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
		if (last_enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
		if (last_enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
		if (last_enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
#ifdef GL_POLYGON_MODE
		glPolygonMode(GL_FRONT_AND_BACK, (GLenum)last_polygon_mode[0]);
#endif
		glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
		glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
	}

	bool GLImGuiRenderer::CreateFontsTexture()
	{
		// Build texture atlas
		ImGuiIO& io = ImGui::GetIO();
		unsigned char* pixels;
		int width, height;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.

		// Upload texture to graphics system
		GLint last_texture;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
		glGenTextures(1, &m_FontTexture);
		glBindTexture(GL_TEXTURE_2D, m_FontTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		// Store our identifier
		io.Fonts->TexID = (ImTextureID)(intptr_t)m_FontTexture;

		// Restore state
		glBindTexture(GL_TEXTURE_2D, last_texture);

		return true;
	}

	void GLImGuiRenderer::DestroyFontsTexture()
	{
		if (m_FontTexture)
		{
			ImGuiIO& io = ImGui::GetIO();
			glDeleteTextures(1, &m_FontTexture);
			io.Fonts->TexID = 0;
			m_FontTexture = 0;
		}
	}

	bool GLImGuiRenderer::CreateDeviceObjects()
	{
		// Backup GL state
		GLint last_texture, last_array_buffer, last_vertex_array;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);

		// Parse GLSL version string
		int glsl_version = 130;
		sscanf(m_GlslVersionString, "#version %d", &glsl_version);

		const GLchar* vertex_shader_glsl_120 =
			"uniform mat4 ProjMtx;\n"
			"attribute vec2 Position;\n"
			"attribute vec2 UV;\n"
			"attribute vec4 Color;\n"
			"varying vec2 Frag_UV;\n"
			"varying vec4 Frag_Color;\n"
			"void main()\n"
			"{\n"
			"    Frag_UV = UV;\n"
			"    Frag_Color = Color;\n"
			"    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
			"}\n";

		const GLchar* vertex_shader_glsl_130 =
			"uniform mat4 ProjMtx;\n"
			"in vec2 Position;\n"
			"in vec2 UV;\n"
			"in vec4 Color;\n"
			"out vec2 Frag_UV;\n"
			"out vec4 Frag_Color;\n"
			"void main()\n"
			"{\n"
			"    Frag_UV = UV;\n"
			"    Frag_Color = Color;\n"
			"    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
			"}\n";

		const GLchar* vertex_shader_glsl_300_es =
			"precision mediump float;\n"
			"layout (location = 0) in vec2 Position;\n"
			"layout (location = 1) in vec2 UV;\n"
			"layout (location = 2) in vec4 Color;\n"
			"uniform mat4 ProjMtx;\n"
			"out vec2 Frag_UV;\n"
			"out vec4 Frag_Color;\n"
			"void main()\n"
			"{\n"
			"    Frag_UV = UV;\n"
			"    Frag_Color = Color;\n"
			"    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
			"}\n";

		const GLchar* vertex_shader_glsl_410_core =
			"layout (location = 0) in vec2 Position;\n"
			"layout (location = 1) in vec2 UV;\n"
			"layout (location = 2) in vec4 Color;\n"
			"uniform mat4 ProjMtx;\n"
			"out vec2 Frag_UV;\n"
			"out vec4 Frag_Color;\n"
			"void main()\n"
			"{\n"
			"    Frag_UV = UV;\n"
			"    Frag_Color = Color;\n"
			"    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
			"}\n";

		const GLchar* fragment_shader_glsl_120 =
			"#ifdef GL_ES\n"
			"    precision mediump float;\n"
			"#endif\n"
			"uniform sampler2D Texture;\n"
			"varying vec2 Frag_UV;\n"
			"varying vec4 Frag_Color;\n"
			"void main()\n"
			"{\n"
			"    gl_FragColor = Frag_Color * texture2D(Texture, Frag_UV.st);\n"
			"}\n";

		const GLchar* fragment_shader_glsl_130 =
			"uniform sampler2D Texture;\n"
			"in vec2 Frag_UV;\n"
			"in vec4 Frag_Color;\n"
			"out vec4 Out_Color;\n"
			"void main()\n"
			"{\n"
			"    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
			"}\n";

		const GLchar* fragment_shader_glsl_300_es =
			"precision mediump float;\n"
			"uniform sampler2D Texture;\n"
			"in vec2 Frag_UV;\n"
			"in vec4 Frag_Color;\n"
			"layout (location = 0) out vec4 Out_Color;\n"
			"void main()\n"
			"{\n"
			"    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
			"}\n";

		const GLchar* fragment_shader_glsl_410_core =
			"in vec2 Frag_UV;\n"
			"in vec4 Frag_Color;\n"
			"uniform sampler2D Texture;\n"
			"layout (location = 0) out vec4 Out_Color;\n"
			"void main()\n"
			"{\n"
			"    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
			"}\n";

		// Select shaders matching our GLSL versions
		const GLchar* vertex_shader = NULL;
		const GLchar* fragment_shader = NULL;
		if (glsl_version < 130)
		{
			vertex_shader = vertex_shader_glsl_120;
			fragment_shader = fragment_shader_glsl_120;
		}
		else if (glsl_version == 410)
		{
			vertex_shader = vertex_shader_glsl_410_core;
			fragment_shader = fragment_shader_glsl_410_core;
		}
		else if (glsl_version == 300)
		{
			vertex_shader = vertex_shader_glsl_300_es;
			fragment_shader = fragment_shader_glsl_300_es;
		}
		else
		{
			vertex_shader = vertex_shader_glsl_130;
			fragment_shader = fragment_shader_glsl_130;
		}

		// Create shaders
		const GLchar* vertex_shader_with_version[2] = { m_GlslVersionString, vertex_shader };
		m_VertHandle = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(m_VertHandle, 2, vertex_shader_with_version, NULL);
		glCompileShader(m_VertHandle);
		CheckShader(m_VertHandle, "vertex shader");

		const GLchar* fragment_shader_with_version[2] = { m_GlslVersionString, fragment_shader };
		m_FragHandle = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(m_FragHandle, 2, fragment_shader_with_version, NULL);
		glCompileShader(m_FragHandle);
		CheckShader(m_FragHandle, "fragment shader");

		m_ShaderHandle = glCreateProgram();
		glAttachShader(m_ShaderHandle, m_VertHandle);
		glAttachShader(m_ShaderHandle, m_FragHandle);
		glLinkProgram(m_ShaderHandle);
		CheckProgram(m_ShaderHandle, "shader program");

		m_AttribLocationTex = glGetUniformLocation(m_ShaderHandle, "Texture");
		m_AttribLocationProjMtx = glGetUniformLocation(m_ShaderHandle, "ProjMtx");
		m_AttribLocationPosition = glGetAttribLocation(m_ShaderHandle, "Position");
		m_AttribLocationUV = glGetAttribLocation(m_ShaderHandle, "UV");
		m_AttribLocationColor = glGetAttribLocation(m_ShaderHandle, "Color");

		// Create buffers
		glGenBuffers(1, &m_VboHandle);
		glGenBuffers(1, &m_ElementsHandle);

		CreateFontsTexture();

		// Restore modified GL state
		glBindTexture(GL_TEXTURE_2D, last_texture);
		glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
		glBindVertexArray(last_vertex_array);

		return true;
	}

	void GLImGuiRenderer::DestroyDeviceObjects()
	{
		if (m_VboHandle) glDeleteBuffers(1, &m_VboHandle);
		if (m_ElementsHandle) glDeleteBuffers(1, &m_ElementsHandle);
		m_VboHandle = m_ElementsHandle = 0;

		if (m_ShaderHandle && m_VertHandle) glDetachShader(m_ShaderHandle, m_VertHandle);
		if (m_VertHandle) glDeleteShader(m_VertHandle);
		m_VertHandle = 0;

		if (m_ShaderHandle && m_FragHandle) glDetachShader(m_ShaderHandle, m_FragHandle);
		if (m_FragHandle) glDeleteShader(m_FragHandle);
		m_FragHandle = 0;

		if (m_ShaderHandle) glDeleteProgram(m_ShaderHandle);
		m_ShaderHandle = 0;

		DestroyFontsTexture();
	}

	bool GLImGuiRenderer::CheckShader(GLuint handle, const char* desc)
	{
		GLint status = 0, log_length = 0;
		glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &log_length);
		if ((GLboolean)status == GL_FALSE)
			RIDGE_CORE_ERROR("ERROR: GLImGuiRenderer::CreateDeviceObjects: failed to compile {}!", desc);
		if (log_length > 0)
		{
			ImVector<char> buf;
			buf.resize((int)(log_length + 1));
			glGetShaderInfoLog(handle, log_length, NULL, (GLchar*)buf.begin());
			RIDGE_CORE_ERROR("{}", buf.begin());
		}
		return (GLboolean)status == GL_TRUE;
	}

	bool GLImGuiRenderer::CheckProgram(GLuint handle, const char* desc)
	{
		GLint status = 0, log_length = 0;
		glGetProgramiv(handle, GL_LINK_STATUS, &status);
		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &log_length);
		if ((GLboolean)status == GL_FALSE)
			RIDGE_CORE_ERROR("ERROR: GLImGuiRenderer::CreateDeviceObjects: failed to link {}! (with GLSL '{}')", desc, m_GlslVersionString);
		if (log_length > 0)
		{
			ImVector<char> buf;
			buf.resize((int)(log_length + 1));
			glGetProgramInfoLog(handle, log_length, NULL, (GLchar*)buf.begin());
			RIDGE_CORE_ERROR("{}", buf.begin());
		}
		return (GLboolean)status == GL_TRUE;
	}

} }