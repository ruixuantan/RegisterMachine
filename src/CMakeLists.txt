set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCES   
  RegisterMachine.cpp
  executor.hpp
  executor.cpp
  ioreader.hpp
  ioreader.cpp
  operators.hpp
  operators.cpp
  parser.hpp
  parser.cpp
  register.hpp
  register.cpp
)

add_executable(${BINARY}_run ${SOURCES})

add_library(${BINARY}_lib STATIC ${SOURCES})