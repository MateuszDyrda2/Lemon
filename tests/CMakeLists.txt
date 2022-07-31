project(LemonTests)

add_subdirectory(libs/googletest)

set(TestList "")
enable_testing()

macro(AddTest _Name _File _Libs) 
	add_executable(${_Name} ${_File})
	target_link_libraries(${_Name} PUBLIC gtest_main ${_Libs})
	set(TestList ${TestList} ${_Name})
endmacro()

AddTest(LoggerTest core/logger_test.cpp lemon::core)
AddTest(HashStringTest core/hash_string_test.cpp lemon::core)
AddTest(EventsTest events/events_test.cpp lemon::events)
AddTest(SchedulerTest threading/scheduler_test.cpp lemon::threading)
AddTest(AssetsTest assets/assets_test.cpp lemon::assets)

include(GoogleTest)
gtest_discover_tests(${TestList})