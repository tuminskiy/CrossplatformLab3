cmake_minimum_required(VERSION 3.5)

project(dev_os_homework_3)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug mode")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/debug/)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/debug/)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
else ()
    message("Release mode")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/release/)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/release/)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif()

message("build options: ${CMAKE_CXX_FLAGS}")

add_subdirectory(${CMAKE_SOURCE_DIR}/protocol)
add_subdirectory(${CMAKE_SOURCE_DIR}/server)
add_subdirectory(${CMAKE_SOURCE_DIR}/client)
