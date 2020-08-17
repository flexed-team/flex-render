-- TODO: rewrite defines and links

workspace "flex-render"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

-- example : "Debug-windows-x86_64"
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"	



-------------------------- PROJECTS -----------------------------


-- Flex Render ------------------------------- 
project "flex-render"
	location "./"
	kind "SharedLib"

	language "C++"
	
	-- example : "bin/Debug-windows-x86_64/flex-render"
	targetdir ("lib/" .. outputdir .. "/")
	objdir ("lib/" .. outputdir .. "/%{prj.name}-int")

	files
	{
		"%{prj.location}/src/**.h",
      	"%{prj.location}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.location}/vendor/flex-math/src",
	}
   
   links {
	  "flex-math"
   }
   
	-- Windows options
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		
		defines
		{
			"FR_PLATFORM_WINDOWS",
			"FM_PLATFORM_WINDOWS",
			"FR_BUILD_DLL",
		}


	-- General build modes options for platforms
	filter "configurations:Debug"
		defines "FR_DEBUG"
		symbols "On" -- acts like -g gcc flag
		
	filter "configurations:Release"
		defines "FR_RELEASE"
		optimize "On"
		
	filter "configurations:Dist"
		defines "FR_DIST"
		optimize "On"

-- Flex Math ------------------------------- 
project "flex-math"
	location "vendor/flex-math"
	kind "SharedLib"

	language "C++"
	
	-- example : "bin/Debug-windows-x86_64/flex-math"
	targetdir ("lib/" .. outputdir .. "/")
	objdir ("lib/" .. outputdir .. "/%{prj.name}-int")
	files
	{
		"%{prj.location}/src/**.h",
      	"%{prj.location}/src/**.cpp",
	}


	-- Windows options
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		
		defines
		{
			"FM_PLATFORM_WINDOWS",
			"FM_BUILD_DLL"
		}

		-- postbuildcommands
		-- {	-- will copy dll from engine build dir into sandbox build dir
		-- 	("{COPY} %{cfg.buildtarget.relpath} %{wks.location}/bin/" .. outputdir .. "/Sandbox/*")
		-- }


	-- General build modes options for platforms
	filter "configurations:Debug"
		defines "FM_DEBUG"
		symbols "On" -- acts like -g gcc flag

		
	filter "configurations:Release"
		defines "FM_RELEASE"
		optimize "On"
		
	filter "configurations:Dist"
		defines "FM_DIST"
		optimize "On"

