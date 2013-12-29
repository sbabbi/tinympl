
cmake_minimum_required(VERSION 2.8.7)

project(tinympl)

set (tinympl_VERSION_MAJOR 0)
set (tinympl_VERSION_MINOR 2)

set(TINYMPL_HEADERS
	tinympl/algorithm.hpp
	tinympl/algorithm_variadic.hpp
	tinympl/bind.hpp
	tinympl/functional.hpp
	tinympl/fused_map.hpp
	tinympl/fused_value_map.hpp
	tinympl/lambda.hpp
	tinympl/map.hpp
	tinympl/ratio.hpp
	tinympl/sequence.hpp
	tinympl/value_map.hpp
	tinympl/variadic.hpp
	tinympl/vector.hpp)

add_library(tinympl STATIC ${TINYMPL_HEADERS})
set_target_properties(tinympl PROPERTIES LINKER_LANGUAGE CXX)

add_definitions(-std=c++11)
add_definitions(-Wall -pedantic)

add_executable(tinympl_test examples/main.cpp)
set_target_properties(tinympl_test PROPERTIES INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR})

enable_testing()
add_test( tinympl_runtime tinympl_test )

install(FILES ${TINYMPL_HEADERS} DESTINATION include/tinympl)
