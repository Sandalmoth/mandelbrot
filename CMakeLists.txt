cmake_minimum_required(VERSION 3.0.0)

set(name mandel)

add_executable(mandel main.cpp man-thrd.cpp man-sycl.cpp)

include_directories("~/Libraries/triSYCL/include") # Edit to match your fs.

find_package (SDL2)
if (SDL2_FOUND)
  include_directories(${SDL2_INCLUDE_DIRS})
  target_link_libraries (mandel ${SDL2_LIBRARIES} -lSDL2)
endif (SDL2_FOUND)

# Compilation flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++17 -fopenmp")

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og -ggdb") # debug compilation
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()
