cmake_minimum_required (VERSION 3.13)
project(Lava CXX)

# lava uses C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# lava has zh_CN charset supported, and uses UTF-8 without BOM
# add option /utf-8 here to force MSVC use UTF-8 to parse source files
if (MSVC)
	string(APPEND CMAKE_C_FLAGS " /utf-8")
	string(APPEND CMAKE_CXX_FLAGS " /utf-8")
endif ()

# lava.config: library configurations
add_library(lava-config INTERFACE)
target_sources(lava-config INTERFACE lava/config/language.h)
target_include_directories(lava-config INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# lava.format: the text formatting library
add_library(lava-format INTERFACE)
target_sources(lava-format INTERFACE
	lava/format/format.h
	lava/format/basic.h
	lava/format/integers.h
	lava/format/text.h
	lava/format/meta.h)
target_include_directories(lava-format INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# lava.assert: the assertion library
add_library(lava-assert INTERFACE)
target_sources(lava-assert INTERFACE
	lava/assert.h
	lava/assert/assert-zh-cn.h
	lava/assert/assert-en.h)
target_include_directories(lava-assert INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(lava-assert INTERFACE lava-config lava-format)

# test cases for each library
add_subdirectory(test test)
