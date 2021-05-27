set(TEST_BINARY ${CMAKE_PROJECT_NAME}_test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES true *.hpp *.cpp)

set(TEST_SOURCES ${TEST_SOURCES})

add_executable(${TEST_BINARY} ${TEST_SOURCES})

target_link_libraries(${TEST_BINARY} PUBLIC 
                                    gtest 
                                    gtest_main
                                    ${CMAKE_PROJECT_NAME}_lib )

add_test(NAME ${TEST_BINARY} COMMAND ${TEST_BINARY})
