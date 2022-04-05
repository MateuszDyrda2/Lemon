cmake_minimum_required(VERSION 3.9)
project(concurrentqueue VERSION 1.0.0)

add_library(concurrentqueue INTERFACE)
target_include_directories(concurrentqueue INTERFACE .)