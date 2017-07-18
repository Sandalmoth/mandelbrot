cmake_minimum_required(VERSION 3.1.3)

set(name mandel)

add_executable(mandel main.cpp man-thrd.cpp man-sycl.cpp man-opencl.cpp)

find_package (SDL2)
if (SDL2_FOUND)
  include_directories(${SDL2_INCLUDE_DIRS})
  target_link_libraries (mandel ${SDL2_LIBRARIES} -lSDL2)
endif (SDL2_FOUND)

# find_package (OpenCL)
# if (OpenCL_FOUND)
#   include_directories(${OpenCL_INCLUDE_DIRS})
#   target_link_libraries (mandel ${OpenCL_LIBRARIES} -lOpenCL)
# endif (OpenCL_FOUND)
# For whatever reason CMAKE doesn't find opencl, thus manual paths instead.
# Update below or try above code.
target_link_libraries (mandel /usr/lib64/libOpenCL.so.1)



# Compilation flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++17")

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og -ggdb") # debug compilation
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()
