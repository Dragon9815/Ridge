#pragma once

#include "Ridge/Graphics/Shader.h"

#include <glad/glad.h>

namespace Ridge {  namespace Graphics {
	class GLShaderProgram : public ShaderProgram {
	public:
		GLShaderProgram();
		virtual ~GLShaderProgram();

		virtual bool LoadFromString(const std::string& sourceCode) override;

		virtual void SetUniform(const std::string& name, const Matrix4f& value);

		virtual void Bind() override;
		virtual void Unbind() override;

	private:
		GLuint CreateAndCompileShader(const std::string& source, GLenum type);

	private:
		GLuint m_handle;
	};
} }