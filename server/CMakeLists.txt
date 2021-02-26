cmake_minimum_required(VERSION 3.5)

add_executable(server)

set_target_properties(server PROPERTIES
  AUTOMOC ON

  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)

set(STORAGE_SRC storage/database.cpp)
set(SERVER_SRC tcpserver/tcpserver.cpp)

set(SRC main.cpp ${SERVER_SRC} ${STORAGE_SRC})


find_package(Qt5 COMPONENTS Core Network Sql REQUIRED)

target_include_directories(server
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/protocol
)
message(${CMAKE_SOURCE_DIR})
target_sources(server PRIVATE ${SRC})
target_link_libraries(server PRIVATE Qt5::Core Qt5::Network Qt5::Sql)
