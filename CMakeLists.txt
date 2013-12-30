
cmake_minimum_required(VERSION 2.6)

project(tinympl)
enable_testing()

set(TINYMPL_MAJOR_VERSION 0)
set(TINYMPL_MINOR_VERSION 2)
set(TINYMPL_VERSION ${TINYMPL_MAJOR_VERSION}.${TINYMPL_MINOR_VERSION})

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

configure_file(Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile @ONLY)

install(DIRECTORY tinympl DESTINATION include)

add_definitions(-std=c++11)
add_definitions(-Wall -pedantic)

add_executable(tinympl_test examples/main.cpp)
set_target_properties(tinympl_test PROPERTIES INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR})

add_test( tinympl_runtime tinympl_test )
