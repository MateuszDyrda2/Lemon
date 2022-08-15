-- premake5.lua
workspace "Lemon"
    configurations { "Debug", "Release" }
    architecture "x86_64" 

include "Lemon"
include "Sandbox"
include "tests"
-- include "Lemonade"