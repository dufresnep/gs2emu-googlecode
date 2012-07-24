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
		--gamemonkey must finish with just /* not /** because we want to manually choose platform
		files { "../dependencies/gamemonkey/*" }
		files { "../dependencies/gamemonkey/binds/*" }
		files { "../dependencies/zlib/**" }
		files { "../dependencies/bzip2/**" }
		includedirs { "../dependencies/gamemonkey" }
		includedirs { "../dependencies/gamemonkey/binds" }
		includedirs { "../dependencies/zlib" }
		includedirs { "../dependencies/bzip2" }

		-- The following allows gmconfig.h to include platform specific gmconfig_p.h file
		if _ACTION ~= nil and string.startswith(_ACTION, "vs") then
			includedirs { "../dependencies/gamemonkey/platform/win32msvc" }
		else
			-- For now, try to use the Windows gcc platform spectific file for Linux, BSD, etc.
			-- Yes, it's wrong, but GM does not have those defined, we'll define our own soon
			includedirs { "../dependencies/gamemonkey/platform/win32gcc" }
		end
		
		-- Windows specific.
		configuration "windows"
		
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
