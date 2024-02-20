cmake_minimum_required(VERSION 3.15)

project(sansic)

add_library(sansic STATIC sansic.cpp)

#https://iamsorush.com/posts/cpp-cmake-config/
install(TARGETS sansic
    EXPORT sansicTargets
    FILE_SET HEADERS
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include)

install(EXPORT sansicTargets
    FILE sansicTargets.cmake
    NAMESPACE sansic::
    DESTINATION lib/cmake/sansic)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "sanicConfigVersion.cmake"
    VERSION "1.0"
    COMPATIBILITY AnyNewerVersion)

install(FILES "sansicConfig.cmake" "${CMAKE_CURRENT_BINARY_DIR}/sansicConfigVersion.cmake"
    DESTINATION lib/cmake/sansic)
