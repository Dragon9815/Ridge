-- function os.winSdkVersion()
    -- local reg_arch = iif( os.is64bit(), "\\Wow6432Node\\", "\\" )
    -- local sdk_version = os.getWindowsRegistry( "HKLM:SOFTWARE" .. reg_arch .."Microsoft\\Microsoft SDKs\\Windows\\v10.0\\ProductVersion" )
    -- if sdk_version ~= nil then return sdk_version end
-- end

IncludeDir = {}
IncludeDir["GLFW"] = "./Engine/vendor/glfw/include"
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "Ridge"
	architecture "x64"
	configurations { "Debug", "Release", "Dist" }
	
	startproject "Sandbox"
	
	targetdir ("./bin/" .. outputdir .. "/")
	objdir 	  ("./obj/" .. outputdir .. "/%{prj.name}")
	
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
		"%{prj.name}/vendor/",
		"%{IncludeDir.GLFW}"
	}	
	
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		defines { "RIDGE_PLATFORM_WINDOWS" }
		
	
	filter "configurations:Debug"	
		defines { "RIDGE_DEBUG" }		
		symbols "On"
		
	filter "configurations:Release"	
		defines { "RIDGE_RELEASE" }	
		optimize "On"
		
	filter "configurations:Dist"
		defines { "HZ_DIST" }
		optimize "On"
	
	filter { "system:windows", "configurations:Release" }
		buildoptions "/MT"
	
	-- filter "platforms:*64" 
		-- architecture "x64"
		
include "Engine/vendor/glfw"
	
local Engine = project("Engine")
	kind "SharedLib"
	language "C++"
	targetname "Ridge"
	basedir "./Engine"
	
	pchheader "RidgePCH.h"
	pchsource "./Engine/src/RidgePCH.cpp"
	
	filter {} -- clear filter
	
	includedirs 
	{
		
	}
	
	libdirs
	{
	
	}
	
	links
	{
		
	}
	
	filter "system:windows"
		defines { "RIDGE_BUILD_DLL" }
	
project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	basedir "Sandbox"
	
	filter {} -- clear filter
	
	includedirs
	{
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
	
	--print("{COPY} " .. Engine.targetdir .. "/" .. Engine.targetname .. ".dll")
	
	--postbuildcommands
	--{
	--	"{COPY} %{cfg.buildtarget.directory}../Engine/Ridge.dll %{cfg.buildtarget.directory}"
	--}
	
	
	
	--buildcommands
	--{
	--	Engine.targetdir .. "/" .. Engine.targetname .. "." .. Engine.targetextension
	--}