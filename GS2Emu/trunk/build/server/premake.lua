--
-- client premake script
--

project_dynamic("gserver", "c++", "winexe")

package.files =
{
    matchfiles(rootdir.."gserver/src/*.cpp"),
    matchfiles(rootdir.."gserver/include/*.h"),
    matchfiles(rootdir.."gserver/include/mysql/*"),
}

include(pkgdir.."include")
include(rootdir.."dependencies/include")

library("zdll", "")
