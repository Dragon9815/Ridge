#include "RidgePCH.h"
#include "GLUtil.h"

#include <glad/glad.h>

namespace Ridge {  namespace Graphics {
	inline void GLClearError()
	{
		while (glGetError() != GL_NO_ERROR);
	}

	inline bool GLCheckError(const char* expr, const char* file, int line)
	{
		bool wasError = false;

		while (GLenum error = glGetError()) {
			wasError = true;
			RIDGE_CORE_ERROR("GL ERROR {0:x} - in {1}:{2}: {3}", error, file, line, expr);
		}

		return wasError;
	}
} }