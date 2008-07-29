--
-- client premake script
--

project_dynamic("gserver", "c++", "exe")

package.files =
{
    matchfiles(rootdir.."server/src/*.cpp"),
    matchfiles(rootdir.."server/include/*.h"),
}

if (windows)
then
	include(pkgdir.."include")
	include(rootdir.."dependencies/include")
end

if (linux or target == "cb-gcc" or target == "gnu")
then
	library("z")
	library("bz2")
else
	library("libz")
	library("libbz2")
end
if (windows) then library("ws2_32") end
