cmake_minimum_required(VERSION 3.5)

add_library(protocol STATIC)

set_target_properties(protocol PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON

  INCLUDE_CURRENT_DIR ON

  LINKER_LANGUAGE CXX
)

set(SRC protocol.cpp)

target_sources(protocol PRIVATE ${SRC})
