cmake_minimum_required(VERSION 3.22)
project(project)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W1 /WX")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/FindWDK/cmake")
find_package(WDK REQUIRED)

add_subdirectory("client")
add_subdirectory("driver")
