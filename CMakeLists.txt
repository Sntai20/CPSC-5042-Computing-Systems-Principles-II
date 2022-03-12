# CMakeList.txt : CMake project for Server-Project, include source and define
# project specific logic here.
cmake_minimum_required (VERSION 3.16)

project ("CPSC-5042-Computing-Systems-Principles-II")

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source to this project's executable.
add_subdirectory ("week6")
add_subdirectory ("HW3")
add_subdirectory ("HW4")