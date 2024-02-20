cmake_minimum_required(VERSION 3.15)

project(sansic_test)

#use this to find already installed and compiled sansic
#find_package(sansic)

add_subdirectory(lib)

add_executable(sansic_test test.cpp)

#we compile the library every time for these tests
target_link_libraries(sansic_test PRIVATE sansic)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -Weffc++")
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -s")

#use this to link with already installed and compiled sansic
#target_link_libraries(sansic_test PRIVATE sansic::sansic)
