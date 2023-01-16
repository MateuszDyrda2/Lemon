cmake_minimum_required(VERSION 3.22)

project(Lua LANGUAGES C)

#set(Lua_SRC
#    lapi.c
#    lauxlib.c
#    lbaselib.c
#    lcode.c
#    lcorolib.c
#    lctype.c
#    ldblib.c
#    ldebug.c
#    ldo.c
#    ldump.c
#    lfunc.c
#    lgc.c
#    linit.c
#    liolib.c
#    llex.c
#    lmathlib.c
#    lmem.c
#    loadlib.c
#    lobject.c
#    lopcodes.c
#    loslib.c
#    lparser.c
#    lstate.c
#    lstring.c
#    lstrlib.c
#    ltable.c
#    ltablib.c
#    ltests.c
#    ltm.c
##    lua.c
#    lundump.c
#    lutf8lib.c
#    lvm.c
#    lzio.c
#)

set(Lua_SRC onelua.c)

add_definitions(-DMAKE_LIB)

add_library(lua ${Lua_SRC})
add_library(lua::lua ALIAS lua)
target_include_directories(lua 
    PUBLIC 
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/lua>"
)
install(DIRECTORY . DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lua)
