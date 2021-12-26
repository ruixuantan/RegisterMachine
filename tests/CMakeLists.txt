set(BINARY ${CMAKE_PROJECT_NAME}_tests)

set(TEST_SOURCES 
  main.cpp
  operators-test.cpp
  parser-test.cpp
)

add_executable(${BINARY} ${TEST_SOURCES})

target_link_libraries(${BINARY} gtest gtest_main ${CMAKE_PROJECT_NAME}_lib)

add_test(NAME ${BINARY} COMMAND ${BINARY})