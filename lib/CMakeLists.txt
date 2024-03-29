cmake_minimum_required(VERSION 3.15)

project(sansic)

add_library(sansic STATIC sansic.cpp)

set_target_properties(sansic PROPERTIES PUBLIC_HEADER "sansic.hpp")

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -Weffc++")
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -s")

#https://iamsorush.com/posts/cpp-cmake-config/
install(TARGETS sansic
    EXPORT sansicTargets
    FILE_SET HEADERS
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include)

install(EXPORT sansicTargets
    FILE sansicTargets.cmake
    NAMESPACE sansic::
    DESTINATION lib/cmake/sansic)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "sansicConfigVersion.cmake"
    VERSION "1.0"
    COMPATIBILITY AnyNewerVersion)

install(FILES "sansicConfig.cmake" "${CMAKE_CURRENT_BINARY_DIR}/sansicConfigVersion.cmake"
    DESTINATION lib/cmake/sansic)
