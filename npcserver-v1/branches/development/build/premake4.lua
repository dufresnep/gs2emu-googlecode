solution "npcserver"
	configurations { "Debug", "Release" }
	platforms { "native", "x32", "x64" }
	
	project "npcserver"
		kind "ConsoleApp"
		-- kind "WindowedApp"
		language "C++"
		location "projects"
		targetdir "../bin"
		targetname "npcserver"
		flags { "Symbols", "Unicode" }
		files { "../npcserver/include/**.h", "../npcserver/src/**.cpp" }
		includedirs { "../npcserver/include" }
		
		-- Dependencies.
		files { "../dependencies/zlib/**" }
		files { "../dependencies/bzip2/**" }
		includedirs { "../dependencies/zlib" }
		includedirs { "../dependencies/bzip2" }
		
		-- Windows specific.
		configuration "windows"
			if _ACTION ~= nil and string.startswith(_ACTION, "vs") then
				files { "../npcserver/platform/win32msvc/*.h" }
				includedirs { "../npcserver/platform/win32msvc" }
			else
				files { "../npcserver/platform/win32gcc/*.h" }
				includedirs { "../npcserver/platform/win32gcc" }
			end
		
		-- Libraries.
		configuration "windows"
			links { "ws2_32" }
		
		-- Windows defines.
		configuration "windows"
			defines { "WIN32", "_WIN32" }
		configuration { "windows", "x64" }
			defines { "WIN64", "_WIN64" }
		
		-- Debug options.
		configuration "Debug"
			defines { "DEBUG" }
			targetsuffix "_d"
			flags { "NoEditAndContinue" }
		
		-- Release options.
		configuration "Release"
			defines { "NDEBUG" }
			flags { "StaticRuntime", "OptimizeSpeed" }
