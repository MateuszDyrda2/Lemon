cmake_minimum_required(VERSION 3.20)

project(Sanbox)

add_executable(sandbox sandbox.cpp)

target_link_libraries(sandbox PUBLIC river)