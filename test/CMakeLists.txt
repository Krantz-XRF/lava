add_executable(test_format format.cpp)
target_link_libraries(test_format lava-format)

add_executable(test_assert assert.cpp)
target_link_libraries(test_assert lava-assert)

add_executable(test_resource resource.cpp)
target_link_libraries(test_resource lava-resource lava-format)
