set(BINARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.hpp *.cpp)

set(SOURCES ${SOURCES})

include_directories(thunder)

SET(CMAKE_CXX_FLAGS "-std=c++20 -g")

add_library(${BINARY}_lib STATIC ${SOURCES})
