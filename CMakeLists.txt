cmake_minimum_required(VERSION 3.20)

project(strict_enum)

#Require C++23 without compiler extensions
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(strict_enum INTERFACE)
target_include_directories(strict_enum INTERFACE ${strict_enum_SOURCE_DIR}/include)
