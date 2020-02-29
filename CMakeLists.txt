
cmake_minimum_required (VERSION 3.8)

project("dbcppp")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(Boost_USE_STATIC_LIBS "MyOption" ON)

if (!${Boost_USE_STATIC_LIBS})
	add_definitions(-DBOOST_LOG_DYN_LINK)
endif()

set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 

find_package(Boost MODULE COMPONENTS log REQUIRED)

if (Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	link_directories(${Boost_LIBRARY_DIRS})
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	add_definitions("/bigobj")
endif()

add_subdirectory(src/)

