#include "RidgePCH.h"
#include "GLShader.h"

#include "GLUtil.h"

struct ShaderSource {
	std::string vertexSource;
	std::string fragmentSource;
};

namespace Ridge { namespace Graphics {

	GLShaderProgram::GLShaderProgram()
	{
		GLCall(m_handle = glCreateProgram());
	}

	GLShaderProgram::~GLShaderProgram()
	{
		glDeleteProgram(m_handle);
	}

	ShaderSource ParseSource(const std::string& sourceCode) 
	{
		enum class ShaderType {
			None = -1, 
			Vertex, Fragment, 
			_length
		};

		ShaderType type = ShaderType::None;
		std::stringstream resultStream[static_cast<int>(ShaderType::_length)];

		std::stringstream sourceStream;
		sourceStream << sourceCode;

		std::string line;
		while (std::getline(sourceStream, line)) {
			if (line.find("#shader") != std::string::npos) {
				if (line.find("vertex") != std::string::npos) {
					type = ShaderType::Vertex;
				}
				else if (line.find("fragment") != std::string::npos) {
					type = ShaderType::Fragment;
				}
				else {
					RIDGE_CORE_ERROR("Invalid shader type: {}", line);
				}
			}
			else {
				resultStream[static_cast<int>(type)] << line << std::endl;
			}
		}

		return { 
			resultStream[static_cast<int>(ShaderType::Vertex)].str(), 
			resultStream[static_cast<int>(ShaderType::Fragment)].str() 
		};
	}

	bool GLShaderProgram::LoadFromString(const std::string& sourceCode)
	{
		bool wasSuccessful = true;

		ShaderSource source = ParseSource(sourceCode);

		GLuint vertexShaderHandle = CreateAndCompileShader(source.vertexSource, GL_VERTEX_SHADER);
		GLuint fragmentShaderHandle = CreateAndCompileShader(source.fragmentSource, GL_FRAGMENT_SHADER);

		GLCall(glAttachShader(m_handle, vertexShaderHandle));
		GLCall(glAttachShader(m_handle, fragmentShaderHandle));

		GLCall(glLinkProgram(m_handle));

		GLint isLinked = 0;
		GLCall(glGetProgramiv(m_handle, GL_LINK_STATUS, &isLinked));
		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			GLCall(glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &maxLength));

			std::vector<GLchar> errorLog(maxLength);
			GLCall(glGetProgramInfoLog(m_handle, maxLength, &maxLength, &errorLog[0]));

			RIDGE_CORE_ERROR("Failed to link program: {}", &errorLog[0]);
			wasSuccessful = false;
		}


		GLCall(glDetachShader(m_handle, vertexShaderHandle));
		GLCall(glDetachShader(m_handle, fragmentShaderHandle));

		GLCall(glDeleteShader(vertexShaderHandle));
		GLCall(glDeleteShader(fragmentShaderHandle));

		return wasSuccessful;
	}

	void GLShaderProgram::SetUniform(const std::string& name, const Matrix4f& value)
	{
		GLint location = glGetUniformLocation(m_handle, name.c_str());
		glUniformMatrix4fv(location, 1, GL_TRUE, value.Transpose().GetPointer());
	}

	void GLShaderProgram::Bind()
	{
		GLCall(glUseProgram(m_handle));
	}

	void GLShaderProgram::Unbind()
	{
		GLCall(glUseProgram(0));
	}

	GLuint GLShaderProgram::CreateAndCompileShader(const std::string& source, GLenum type)
	{
		GLCall(GLuint handle = glCreateShader(type));

		const GLchar* chSource = reinterpret_cast<const GLchar*>(source.c_str());

		GLCall(glShaderSource(handle, 1, &chSource, nullptr));
		GLCall(glCompileShader(handle));

		GLint isCompiled = 0;
		GLCall(glGetShaderiv(handle, GL_COMPILE_STATUS, &isCompiled));
		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			GLCall(glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &maxLength));

			std::vector<GLchar> errorLog(maxLength);
			GLCall(glGetShaderInfoLog(handle, maxLength, &maxLength, &errorLog[0]));

			RIDGE_CORE_ERROR("Failed to compile shader: {}", &errorLog[0]);

			GLCall(glDeleteShader(handle));
			return NULL;
		}

		return handle;
	}

} }