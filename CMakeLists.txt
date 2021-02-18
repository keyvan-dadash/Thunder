cmake_minimum_required (VERSION 2.8.11)
project (thunder C CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_subdirectory (src)

add_subdirectory (examples)

set_target_properties(${CMAKE_PROJECT_NAME}_lib PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(libs/googletest)

target_include_directories(
                            ${CMAKE_PROJECT_NAME}_lib PUBLIC
                            src)

add_subdirectory (tests)


