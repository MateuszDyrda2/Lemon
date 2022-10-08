cmake_minimum_required(VERSION 3.22)

# ---------- Top level CMake file -----------
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Build sandbox 
option(LEMON_BUILD_SANDBOX "Build sandbox" ON)
# Build tests
option(LEMON_BUILD_TESTS "Build lemon tests" ON)
# -------------------------------------------

# Must be in the top level CMakeLists file
enable_testing()
# --------- Add Game engine project ---------
add_subdirectory(Lemon)
# -------------------------------------------

if(LEMON_BUILD_SANDBOX)
	message("Building sandbox")
	add_subdirectory(Sandbox)
endif()
if(LEMON_BUILD_TESTS)
	message("Building tests")
	add_subdirectory(tests)
endif()
# -------------------------------------------

