cmake_minimum_required(VERSION 3.5)
project(tiemr_base_on_cpp)

include_directories(include)



add_library( timer src/timer.cc )
target_link_libraries(timer pthread)


add_executable(test_timer test/test_timer.cc)
target_link_libraries(test_timer timer)

