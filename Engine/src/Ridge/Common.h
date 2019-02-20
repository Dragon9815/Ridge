#pragma once

#define RIDGE_ENABLED	1
#define RIDGE_DISABLED	0

#if defined(_MSC_VER)
#	pragma warning(disable:4251)
#	if !defined(_CRT_SECURE_NO_WARNINGS)
#		define _CRT_SECURE_NO_WARNINGS
#	endif
#endif

#ifdef RIDGE_PLATFORM_WINDOWS 
#	ifdef RIDGE_BUILD_DLL
#		define RIDGE_API __declspec(dllexport)
#	else
#		define RIDGE_API __declspec(dllimport)
#	endif
#else
#	error Ridge Engine oly supports Windows!
#endif

#if defined(RIDGE_DEBUG) && !defined(RIDGE_ENABLE_ASSERTS)
#define RIDGE_ENABLE_ASSERTS	RIDGE_ENABLED
#endif

#if RIDGE_ENABLE_ASSERTS == RIDGE_ENABLED
#	define RIDGE_ASSERT(x, ...) { if(!(x)) { RIDGE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
#	define RIDGE_CORE_ASSERT(x, ...) { if(!(x)) { RIDGE_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#	define RIDGE_ASSERT(x, ...) 
#	define RIDGE_CORE_ASSERT(x, ...) 
#endif

#define BIT(n)		(1 << (n))

#define BIND_EVENT_FN(func) std::bind(&##func, this, std::placeholders::_1)

#ifdef RIDGE_ENABLE_OPENGL
#define RIDGE_API_OPENGL_FACTORY(clz, ...) case RenderAPI::OPENGL: return new GL##clz(__VA_ARGS__);
#else
#define RIDGE_API_OPENGL_FACTORY(clz, ...)
#endif

// TODO: Re-enable DirectX
#undef RIDGE_ENABLE_DIRECT3D
#ifdef RIDGE_ENABLE_DIRECT3D
#define RIDGE_API_DIRECT3D_FACTORY(clz, ...) case RenderAPI::DIRECT3D: return new DX##clz(__VA_ARGS__);
#else
#define RIDGE_API_DIRECT3D_FACTORY(clz, ...)
#endif

#define RIDGE_API_OBJECT_FACTORY(clz, ...)\
	switch(Context::GetRenderAPI()) {\
	RIDGE_API_OPENGL_FACTORY(clz, __VA_ARGS__)\
	RIDGE_API_DIRECT3D_FACTORY(clz, __VA_ARGS__)\
	}\
	return nullptr;