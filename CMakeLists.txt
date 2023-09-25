cmake_minimum_required(VERSION 3.5)
project(tiemr_base_on_cpp)

include_directories(include)



add_library( timer src/timer/timer.cc )
target_link_libraries(timer pthread)

add_library( timer_manager src/timer_manager/timer.cc src/timer_manager/timer_manager.cc )
target_link_libraries(timer_manager pthread)

add_executable(test_timer test/test_timer.cc)
target_link_libraries(test_timer timer)


add_executable(test_timer_manager test/test_timer_manager.cc)
target_link_libraries(test_timer_manager timer_manager)