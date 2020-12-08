project(bitbuffer CXX)

cmake_minimum_required(VERSION 3.0.2)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

Option(DEV_ENVIRONMENT "Development environment (extended debugging)" OFF)

#####################################################################
# decide which components need to be built: by default, build everything,
# unless some components were explicitely disabled from the parent project
#####################################################################
Option(BITBUFFER_ARCH_NATIVE "Use -march=native compiler flag" ON)

#####################################################################
# build library
#####################################################################
add_library(bitbuffer src/bit_buffer.cpp src/bit_buffer.hpp)
target_include_directories(bitbuffer
    PUBLIC src
)

#####################################################################
# internal compiler flags
#####################################################################
if(DEV_ENVIRONMENT AND CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_definitions(bitbuffer PUBLIC _GLIBCXX_DEBUG) # PUBLIC to maintain ABI compatibility
endif()

