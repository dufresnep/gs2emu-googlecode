--
-- SotSmed premake script
-- Heavily leeched from the CEGUI premake script
--

dofile("helpers.lua")
dofile("config.lua")

--
-- project setup
--
project.name = "gserver"
project.bindir = rootdir.."bin"
project.libdir = rootdir.."lib"
project.configs =
{
    "Debug",
    "Release"
}

--
-- Package table
--
local pkg_table =
{
    "gserver",
}


-- finalise
createpackages(pkg_table)


-- the end
