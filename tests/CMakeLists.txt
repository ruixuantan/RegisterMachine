set(BINARY ${CMAKE_PROJECT_NAME}_tests)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.hpp *.cpp *.h)

set(SOURCES ${TEST_SOURCES})

add_executable(${BINARY} ${TEST_SOURCES})

target_link_libraries(${BINARY} gtest gtest_main)

add_test(NAME ${BINARY} COMMAND ${BINARY})