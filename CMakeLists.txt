cmake_minimum_required (VERSION 3.13)
project(Lava CXX)

# lava uses C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# lava has zh_CN charset supported, and uses UTF-8 without BOM
# add option /utf-8 here to force MSVC use UTF-8 to parse source files
if (MSVC)
	string(APPEND CMAKE_C_FLAGS " /source-charset:utf-8")
	string(APPEND CMAKE_CXX_FLAGS " /source-charset:utf-8")
endif ()

# lava.config: library configurations
add_library(lava-config INTERFACE)
target_sources(lava-config INTERFACE lava/config/language.h)
target_include_directories(lava-config INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# lava.format: the text formatting library
add_library(lava-format INTERFACE)
target_sources(lava-format INTERFACE
	lava/format.h
	lava/format/basic.h
	lava/format/integers.h
	lava/format/text.h
	lava/format/meta.h
	lava/format/ansi.h)
target_include_directories(lava-format INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# lava.assert: the assertion library
add_library(lava-assert INTERFACE)
target_sources(lava-assert INTERFACE lava/assert.h)
target_include_directories(lava-assert INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(lava-assert INTERFACE lava-config lava-format)

# lava.resource: the resource management library
add_library(lava-resource INTERFACE)
target_sources(lava-resource INTERFACE
	lava/resource.h
	lava/resource/utility.h
	lava/resource/single.h
	lava/resource/some.h
	lava/resource/many.h)
target_include_directories(lava-resource INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(lava-resource INTERFACE lava-config lava-assert lava-format)
# Note: lava.resource currently depends on correction P0522R0 of C++17
# For compatibility reasons, clang does not support this by default
# So here we must add a compile time option to force this
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
	target_compile_options(lava-resource INTERFACE "-frelaxed-template-template-args")
endif ()

# lava.bitflags: the bit-flags support for enum
add_library(lava-bitflags INTERFACE)
target_sources(lava-bitflags INTERFACE lava/bitflags.h)
target_include_directories(lava-bitflags INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# lava.trace: the debug trace library
add_library(lava-trace INTERFACE)
target_sources(lava-trace INTERFACE lava/trace.h)
target_include_directories(lava-bitflags INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# lava.curry: currying C++ functions
add_library(lava-curry INTERFACE)
target_sources(lava-curry INTERFACE lava/curry.h)
target_include_directories(lava-curry INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# test cases for each library
add_subdirectory(test test EXCLUDE_FROM_ALL)
