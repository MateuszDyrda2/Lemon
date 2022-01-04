cmake_minimum_required(VERSION 3.20)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/)

project(RiverEditor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(RiverEditor_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/ )
set(RiverEditor_SRC
	${RiverEditor_SRC_DIR}main.cpp
	${RiverEditor_SRC_DIR}application.cpp
	${RiverEditor_SRC_DIR}editor_window.cpp
	${RiverEditor_SRC_DIR}editor_system.cpp
)

add_executable(RiverEditor ${RiverEditor_SRC})
target_include_directories(RiverEditor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(RiverEditor PUBLIC river::river imgui)