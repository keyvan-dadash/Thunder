set(TEST_BINARY ${CMAKE_PROJECT_NAME}_smartlock_benchmark)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES true *.hpp *.cpp)

set(TEST_SOURCES ${TEST_SOURCES})

set(SOURCES
        cachelinespinlock
        spinlock
        smartlock
        e_mutex
        )

find_package(Threads REQUIRED)

SET(CMAKE_CXX_FLAGS "-std=c++20 -g")

foreach(src ${SOURCES})
  add_executable(${src} "${src}.cpp")
  target_link_libraries(${src} PUBLIC 
      ${CMAKE_PROJECT_NAME}_lib 
      Threads::Threads )
endforeach()

