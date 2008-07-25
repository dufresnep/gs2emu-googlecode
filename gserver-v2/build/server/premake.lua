--
-- client premake script
--

project_dynamic("gserver", "c++", "exe")

package.files =
{
    matchfiles(rootdir.."server/src/*.cpp"),
    matchfiles(rootdir.."server/include/*.h"),
}

include(pkgdir.."include")
include(rootdir.."dependencies/include")

library("libz")
library("libbz2")
if (windows) then library("ws2_32") end
