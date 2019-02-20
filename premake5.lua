IncludeDir = {}
IncludeDir["ImGui"] = "Engine/vendor/ImGui"
IncludeDir["GLAD"] = "Engine/vendor/GLAD/include"
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "Ridge"
	architecture "x64"
	configurations { "Debug", "Release", "Dist" }
	
	startproject "Sandbox"
	
	targetdir ("./bin/" .. outputdir .. "/")
	objdir 	  ("./obj/" .. outputdir .. "/%{prj.name}")
	
	filter {} -- clear filter

	newoption {
		trigger 	= "with-opengl",
		description = "Enable OpenGL graphics API for rendering"
	}

	newoption {
		trigger 	= "with-d3d",
		description = "Enable Direct3D graphics API for rendering"
	}
	
	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		characterset "MBCS"
		defines { "RIDGE_PLATFORM_WINDOWS" }

	filter "system:linux"
		cppdialect "C++17"
		defines { "RIDGE_PLATFORM_LINUX" }
	
	filter "configurations:Debug"	
		defines { "RIDGE_DEBUG" }		
		symbols "On"
		runtime "Debug"
		staticruntime "Off"
		
	filter "configurations:Release"	
		defines { "RIDGE_RELEASE" }	
		optimize "On"
		runtime "Release"
		staticruntime "Off"

	filter "configurations:Dist"
		defines { "HZ_DIST" }
		optimize "On"
		runtime "Release"
		staticruntime "Off"

group "Dependencies"
	include "Engine/vendor/GLAD"
	include "Engine/vendor/imgui"
group ""
	
local Engine = project("Engine")
	kind "SharedLib"
	language "C++"
	targetname "Ridge"
	basedir "./Engine"
	
	pchheader "RidgePCH.h"
	pchsource "./Engine/src/RidgePCH.cpp"
	
	filter {} -- clear filter

	local sourceDir = "%{prj.name}/src/";
	local platformDir = sourceDir .. "/Ridge/Platform/";
	files 
	{
		sourceDir .. "**.h",
		sourceDir .. "**.c",
		sourceDir .. "**.hpp",
		sourceDir .. "**.cpp",
	}

	excludes 
	{
		platformDir .. "**/*.*"
	}
	
	includedirs 
	{
		sourceDir,
		"%{prj.name}/vendor/",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}"
	}	
	
	includedirs 
	{
		
	}
	
	libdirs
	{
	
	}
	
	links
	{
		"GLAD",
		"ImGui",
		"opengl32.lib",
		"d3d11.lib"
	}

	filter "options:with-opengl"
		defines { "RIDGE_ENABLE_OPENGL" }
		files 
		{
			platformDir .. "OpenGL/**.h",
			platformDir .. "OpenGL/**.c",
			platformDir .. "OpenGL/**.hpp",
			platformDir .. "OpenGL/**.cpp",
		}
		
	filter "options:with-d3d"
		defines { "RIDGE_ENABLE_DIRECT3D"}
		files
		{
			platformDir .. "DirectX/**.h",
			platformDir .. "DirectX/**.c",
			platformDir .. "DirectX/**.hpp",
			platformDir .. "DirectX/**.cpp",
		}
	
	filter "system:windows"
		defines { "RIDGE_BUILD_DLL" }
		files 
		{
			platformDir .. "Windows/**.h",
			platformDir .. "Windows/**.c",
			platformDir .. "Windows/**.hpp",
			platformDir .. "Windows/**.cpp",
		}
		
	filter {} -- clear filter	
	
project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	basedir "./Sandbox"
	
	filter {} -- clear filter

	local sourceDir = "%{prj.name}/src/";
	files 
	{
		sourceDir .. "**.h",
		sourceDir .. "**.c",
		sourceDir .. "**.hpp",
		sourceDir .. "**.cpp",
	}
	
	includedirs
	{
		sourceDir,
		Engine.basedir .. "/src",
		Engine.basedir .. "/vendor",
	}
	
	libdirs
	{
		
	}
	
	links
	{
		"Engine"
	}