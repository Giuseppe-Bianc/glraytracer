-- Check if the target CPU is AMD (this is a simplified example, you might need to replace it with a proper CPU detection method)
local isAMDCPU = os.host() == "windows" and os.outputof("detect_amd_cpu_tool") == "AMD"

workspace "glraytracer"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "glraytracer"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files { 
      "src/**.h", 
      "src/**.cpp", 
      "src/**.c",
      "src/**.frag",
      "src/**.vert",
      "src/**.png"
   }
   vpaths {
        ["Headers"] = { "**.h", "**.hpp" },
        ["Sources/*"] = {"**.c", "**.cpp"},
        ["Resources"] = {"**.frag", "**.vert","**.png"};
        ["Docs"] = "**.md"
    }

   includedirs {
      "external/spdlog/include",
      "external/glfw/include",
      "external/glm",
      "external/stb",
      "external/glad/include"
   }
   libdirs { "external/glfw/lib-vc2022" }
   links {
        "opengl32",
        "glfw3"
        -- Aggiungi qui eventuali altre librerie OpenGL o di sistema necessarie
    }

   if isAMDCPU then
      includedirs { "path_to_aocl_include_folder" }
      libdirs { "path_to_aocl_lib_folder" }
      links { "aocl_library_name" }
   end   
   targetdir ("bin/" .. outputdir .. "/%{prj.name}")
   objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

   filter "system:windows"
      systemversion "latest"

   filter "configurations:Debug"
      defines { "WL_DEBUG", "SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG" }
      runtime "Debug"
      inlining "Auto"
      symbols "On"

   filter "configurations:Release"
      defines { "WL_RELEASE", "SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE" }
      runtime "Release"
      optimize "On"
      inlining "Auto"
      symbols "On"
      flags { "LinkTimeOptimization", "MultiProcessorCompile"} 

   filter "configurations:Dist"
      defines { "WL_DIST", "SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE" }
      runtime "Release"
      optimize "On"
      inlining "Auto"
      symbols "Off"
      omitframepointer "On"
      flags { "LinkTimeOptimization", "MultiProcessorCompile", "NoBufferSecurityCheck", "NoIncrementalLink" }