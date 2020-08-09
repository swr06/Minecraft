-- Premake file 

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
   targetdir "bin/%{cfg.buildcfg}"

   files 
   { 
      "Source/**.h", 
      "Source/**.cpp" 
   }
   
   includedirs
   {
      "Source/Core/Dependencies/GLEW/include",
      "Source/Core/Dependencies/GLFW/include",
      "Source/Core/Dependencies/glm"
   }

   links
   {
      "glew32s.lib",
      "glfw3.lib",
      "opengl32.lib"
   }

   filter "system::windows"
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

   filter { "system::windows", "configurations:Release" }
      buildoptions "/MT"