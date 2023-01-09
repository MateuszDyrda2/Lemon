cmake_minimum_required(VERSION 3.22)

# ---------- Top level CMake file -----------
project(LemonEngine)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Build sandbox 
option(LEMON_BUILD_SANDBOX "Build sandbox" ON)
# Build tests
option(LEMON_BUILD_TESTS "Build lemon tests" ON)
# Build docs
option(LEMON_BUILD_DOCS "Build docs" OFF)
# -------------------------------------------

if(LEMON_BUILD_DOCS)
	find_package(Doxygen)
	if(DOXYGEN_FOUND)
		set(DOXYGEN_IN "${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxygen.in")
		set(DOXYGEN_OUT "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")

		configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
		message("Doxygen build started")

		add_custom_target(lemon_doxygen ALL
			COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Generating documentation"
			VERBATIM )

	else()
		message("Doxygen missing")
	endif()
endif()

# ------------------ Python --------------------
find_package(Python COMPONENTS Interpreter)
# ----------------------------------------------
set(ENGINE_TYPES)
set(GAME_TYPES)
# Must be in the top level CMakeLists file
enable_testing()
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()
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


