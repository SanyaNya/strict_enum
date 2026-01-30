cmake_minimum_required(VERSION 3.20)

project(strict_enum VERSION 1.10.0)

#Require C++23 without compiler extensions
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(
  DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.42.1/CPM.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

CPMAddPackage("gh:SanyaNya/hpp@1.8.1")

add_library(strict_enum INTERFACE)
target_link_libraries(strict_enum INTERFACE hpp)
target_include_directories(strict_enum INTERFACE ${strict_enum_SOURCE_DIR}/include)
