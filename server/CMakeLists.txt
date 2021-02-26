cmake_minimum_required(VERSION 3.5)

add_executable(server)

set_target_properties(server PROPERTIES
  AUTOMOC ON

  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON

  INCLUDE_CURRENT_DIR ON
)

set(SERVER_SRC tcpserver/tcpserver.cpp)

set(SRC main.cpp ${SERVER_SRC} )


find_package(Qt5 COMPONENTS Core Network REQUIRED)

target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/protocol)
target_sources(server PRIVATE ${SRC})
target_link_libraries(server PRIVATE Qt5::Core Qt5::Network)
