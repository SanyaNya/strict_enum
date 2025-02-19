cmake_minimum_required(VERSION 3.20)

project(strict_enum VERSION 1.5.1)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#Require C++23 without compiler extensions
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(Fetch)

fetch_project(hpp https://github.com/SanyaNya/hpp v1.5.0)

add_library(strict_enum INTERFACE)
target_link_libraries(strict_enum INTERFACE hpp)
target_include_directories(strict_enum INTERFACE ${strict_enum_SOURCE_DIR}/include)
