cmake_minimum_required (VERSION 2.8.11)
project (thunder)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_subdirectory (src)

add_subdirectory (examples)

add_subdirectory (tests)


add_subdirectory(libs/googletest)


