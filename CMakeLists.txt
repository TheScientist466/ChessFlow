# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("ChessFlow")

set(BUILD_TEST true)

# Include sub-projects.
add_subdirectory("extlibs")
add_subdirectory ("tests")
add_subdirectory ("ChessFlow")


