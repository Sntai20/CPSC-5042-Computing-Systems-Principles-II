# CMakeList.txt : CMake project for Server-Project, include source and define
# project specific logic here.
cmake_minimum_required (VERSION 3.16)

# Set the project name and CPP language.
project ("HW4" CXX)

# CPP Only
enable_language(CXX)

# Add source to this project's executable.
add_executable (HW4 "Condition_Variable.cpp")

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add pthread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries("HW4" PRIVATE Threads::Threads)