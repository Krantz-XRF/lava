add_executable(test_format format.cpp)
target_link_libraries(test_format lava-format)

add_executable(test_assert assert.cpp)
target_link_libraries(test_assert lava-assert)

add_executable(test_finally finally.cpp)
target_link_libraries(test_finally lava-finally)

add_executable(test_resource resource.cpp)
target_link_libraries(test_resource lava-resource lava-format)

add_executable(test_bitflags bitflags.cpp)
target_link_libraries(test_bitflags lava-bitflags lava-assert)

add_executable(test_trace trace.cpp)
target_link_libraries(test_trace lava-trace lava-format)

add_executable(test_curry curry.cpp)
target_link_libraries(test_curry lava-curry lava-trace)

add_custom_target(tests)
add_dependencies(tests
	test_format test_assert test_finally test_resource
	test_bitflags test_trace test_curry)
