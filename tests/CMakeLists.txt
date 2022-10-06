cmake_minimum_required(VERSION 3.22)

# ---------- Unit test project -----------
project(Unit)
# ----------------------------------------

# ----------------------------------------
set(CMAKE_CXX_STANDARD 20)
# ----------------------------------------

# ----------------------------------------
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# ----------------------------------------

enable_testing()
add_subdirectory(googletest)

# ----------------------------------------
include(GoogleTest)
set(TEST_LIST
    assets/assets_test.cpp
    core/hash_string_test.cpp
    core/logger_test.cpp
    events/events_test.cpp
    threading/scheduler_test.cpp
)
add_executable(lemon_tests ${TEST_LIST})
target_link_libraries(lemon_tests
  PUBLIC
    GTest::gtest_main
    lemon::lemon
)
gtest_discover_tests(lemon_tests)
# ----------------------------------------

