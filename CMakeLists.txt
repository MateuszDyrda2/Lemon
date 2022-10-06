cmake_minimum_required(VERSION 3.22)

option(LEMON_BUILD_SANDBOX "Build sandbox" ON)
option(LEMON_BUILD_TESTS "Build lemon tests" ON)

enable_testing()
add_subdirectory(Lemon)

if(LEMON_BUILD_SANDBOX)
	message("Building sandbox")
	add_subdirectory(Sandbox)
endif()
if(LEMON_BUILD_TESTS)
	message("Building tests")
	add_subdirectory(tests)
endif()
