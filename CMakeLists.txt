
cmake_minimum_required (VERSION 3.12)

project("libdbcppp")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories("third_party")

if (!${Boost_USE_STATIC_LIBS})
	add_definitions(-DBOOST_LOG_DYN_LINK)
endif()

find_package(Boost COMPONENTS program_options REQUIRED)

if (Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	link_directories(${Boost_LIBRARY_DIRS})
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	add_definitions("/bigobj")
endif()

add_subdirectory(src/)
