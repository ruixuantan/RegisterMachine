cmake_minimum_required(VERSION 3.22)

project(RegisterMachine)
add_executable(RegisterMachine 
  src/RegisterMachine.cpp 
  src/ioreader.cpp
  src/parser.cpp
  src/operators.cpp
  src/register.cpp
  src/executor.cpp
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-std=gnu++17")
set(CMAKE_CXX_STANDARD_REQUIRED True)
