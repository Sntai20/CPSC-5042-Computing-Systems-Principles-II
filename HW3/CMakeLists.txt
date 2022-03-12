# CMakeList.txt : CMake project for Server-Project, include source and define
# project specific logic here.
cmake_minimum_required (VERSION 3.16)

project ("HW3")

# Add source to this project's executable.
add_executable (threadmutex "hw3.cpp")

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)