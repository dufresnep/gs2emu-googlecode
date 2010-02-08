solution "graalserverlistdump"
	configurations { "Debug", "Release" }
	platforms { "native", "x32", "x64" }
	
	project "graalserverlistdump"
		kind "ConsoleApp"
		-- kind "WindowedApp"
		language "C++"
		location "projects"
		targetdir "../bin"
		targetname "graalserverlistdump"
		flags { "Symbols", "Unicode" }
		files { "../source/**" }
		includedirs { "../source" }
		
		-- Dependencies.
		files { "../dependencies/zlib/**" }
		files { "../dependencies/bzip2/**" }
		includedirs { "../dependencies/zlib" }
		includedirs { "../dependencies/bzip2" }
		
		-- Windows defines.
		configuration "windows"
			defines { "WIN32", "_WIN32" }
		configuration { "windows", "x64" }
			defines { "WIN64", "_WIN64" }
		
		-- Debug options.
		configuration "Debug"
			defines { "DEBUG" }
			targetsuffix "_d"
		
		-- Release options.
		configuration "Release"
			defines { "NDEBUG" }
			flags { "StaticRuntime", "OptimizeSpeed" }
