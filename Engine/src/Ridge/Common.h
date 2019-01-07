#pragma once

#ifdef RIDGE_PLATFORM_WINDOWS 
	#ifdef RIDGE_BUILD_DLL
		#define RIDGE_API __declspec(dllexport)
	#else
		#define RIDGE_API __declspec(dllimport)
	#endif
#else
	#error Ridge Engine oly supports Windows!
#endif

#define BIT(n)		(1 << (n))