--
-- client premake script
--

project_dynamic("grsecurelogin", "c++", "exe")

package.files =
{
    matchfiles(rootdir.."client/src/*.cpp"),
    matchfiles(rootdir.."client/include/*.h"),
}

-- Windows library includes.
if (windows) then
	include(rootdir.."dependencies/include")
	librarypath(rootdir.."dependencies")
end

-- Libraries to link to.
if (linux or target == "cb-gcc" or target == "gnu") then
	library("z")
	library("bz2")
else
	library("libz")
	library("libbz2")
end
if (windows) then library("ws2_32") end
