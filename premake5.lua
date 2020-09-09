workspace "Minecraft Clone"
	architecture "x86"
	configurations 
	{ 
		"Debug", 
		"Release" 
	}

project "Minecraft Clone"
	location "Source"
	kind "ConsoleApp"
	language "C++"
	targetdir "Source/bin/%{cfg.buildcfg}"

	files 
	{ 
		"Source/**.h", 
		"Source/**.cpp" 
	}
   
	includedirs
	{
		"Source/Core/Dependencies/GLEW/include",
		"Source/Core/Dependencies/GLFW/include",
		"Source/Core/Dependencies/glm",
		"Source/Core/Dependencies/imgui"
	}

	links
	{
		"glew32s.lib",
		"glfw3.lib",
		"opengl32.lib",
		"kernel32.lib",
		"user32.lib",
		"gdi32.lib",
		"winspool.lib",
		"comdlg32.lib",
		"advapi32.lib",
		"shell32.lib",
		"ole32.lib",
		"oleaut32.lib",
		"uuid.lib",
		"odbc32.lib",
		"odbccp32.lib"
	}
   
	libdirs
	{
		"Source/Core/Dependencies/GLEW/lib",
		"Source/Core/Dependencies/GLFW/lib"
	}
  
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"MC_CLONE_WINDOWS"
		}

	filter "configurations:Debug"
		defines 
		{ 
			"DEBUG",
			"_CRT_SECURE_NO_WARNINGS"
		}
		symbols "On"

	filter "configurations:Release"
		defines 
		{ 
			"NDEBUG",
			"_CRT_SECURE_NO_WARNINGS"
		}
		optimize "On"
