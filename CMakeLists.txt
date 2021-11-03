cmake_minimum_required (VERSION 3.16.3)
project (crypto-test)
set(NAME "crypto-test")

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR})

set(CMAKE_C_FLAGS "-pthread  -march=native -O3 -maes")
# set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++17")

add_subdirectory(depends)
add_subdirectory(src)
