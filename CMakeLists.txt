
cmake_minimum_required (VERSION 3.8)

project("dbcppp")

SET(WIN_PLATFORM win64)
SET(MY_OWN_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/output CACHE PATH "Prefix prepended to install directories")
SET(CMAKE_INSTALL_PREFIX "${MY_OWN_INSTALL_PREFIX}" CACHE INTERNAL "Prefix prepended to install directories" FORCE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories("third_party")

if (!${Boost_USE_STATIC_LIBS})
	add_definitions(-DBOOST_LOG_DYN_LINK)
endif()

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF) 

#boost
include_directories(D:/boost_1_69_0)
link_directories(D:/boost_1_69_0/stage/lib)


if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	add_definitions("/bigobj")
endif()

add_subdirectory(src/)


