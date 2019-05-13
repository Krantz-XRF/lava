add_executable(test_format format.cpp)
target_link_libraries(test_format lava-format)

add_executable(test_assert assert.cpp)
target_link_libraries(test_assert lava-assert)
