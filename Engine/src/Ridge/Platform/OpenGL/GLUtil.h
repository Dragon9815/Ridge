#pragma once

namespace Ridge {  namespace Graphics {
	extern inline void GLClearError();
	extern inline bool GLCheckError(const char* expr, const char* file, int line);
} }

#ifdef RIDGE_DEBUG
#define GLCall(x)	GLClearError(); ##x; if(GLCheckError(#x, __FILE__, __LINE__)) __debugbreak();
#else
#define GLCall(x)	x
#endif