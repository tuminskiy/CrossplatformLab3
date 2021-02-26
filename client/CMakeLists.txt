cmake_minimum_required(VERSION 3.5)

if (WIN32)
  set(EXEC_FLAG WIN32)
endif()

add_executable(client ${EXEC_FLAG})

set_target_properties(client PROPERTIES
  AUTOMOC ON
  AUTOUIC ON

  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)

set(GUI_SRC
  gui/mainwindow/mainwindow.cpp
  gui/mainwindow/mainwindow.ui

  gui/loginwindow/loginwindow.cpp
  gui/loginwindow/loginwindow.ui

  gui/registrationwindow/registrationwindow.cpp
  gui/registrationwindow/registrationwindow.ui
)

set(SRC main.cpp ${GUI_SRC})

find_package(Qt5 COMPONENTS Widgets Network REQUIRED)

target_include_directories(client
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/protocol
)
target_sources(client PRIVATE ${SRC})
target_link_libraries(client PRIVATE Qt5::Widgets Qt5::Network)
