project(lemon_tests)

add_subdirectory(googletest)
enable_testing()

set(LTESTS )
macro(LEMON_TEST LNAME LFILES LEMON_LIBS)
	add_executable(${LNAME} ${LFILES})
	target_link_libraries(${LNAME} PUBLIC gtest_main ${LEMON_LIBS})
	set(LTESTS ${LTESTS} ${LNAME})
endmacro(LEMON_TEST)

LEMON_TEST(scheduler_test threading/scheduler_test.cpp lemon::threading)
LEMON_TEST(event_test events/event_test.cpp lemon::events)

include(GoogleTest)
gtest_discover_tests(${LTESTS})