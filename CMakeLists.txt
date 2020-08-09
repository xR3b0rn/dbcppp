
cmake_minimum_required(VERSION 3.12)

project("libdbcppp" VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


if (!${Boost_USE_STATIC_LIBS})
	add_definitions(-DBOOST_LOG_DYN_LINK)
endif()

find_package(Boost 1.72.0 REQUIRED)
find_package(LibXml2 REQUIRED)
#find_package(LLVM REQUIRED CONFIG)

#message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
#message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
#add_definitions(${LLVM_DEFINITIONS})
# Find the libraries that correspond to the LLVM components
# that we wish to use
#llvm_map_components_to_libnames(llvm_libs support core irreader mcjit jitlink native nativecodegen orcjit)

if (Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	link_directories(${Boost_LIBRARY_DIRS})
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	add_definitions("/bigobj")
endif()

include_directories(${LIBXML2_INCLUDE_DIR})
include_directories(${LLVM_INCLUDE_DIRS})
include_directories("third_party")
include_directories("include")
include_directories("third_party/libxmlmm/include")

add_subdirectory(src)

set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


add_library("libxmlmm" STATIC "")

target_link_libraries("libxmlmm" ${LIBXML2_LIBRARIES})

file(GLOB header
    "third_party/libxmlmm/include/*.h"
)
file(GLOB src
    "third_party/libxmlmm/src/*.cpp"
)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	add_compile_options("-fPIC")
endif()

target_sources("libxmlmm"
    PRIVATE ${header}
    PUBLIC ${header_interface}
    PRIVATE ${src}
)

# install
install(
    TARGETS "libxmlmm"
    DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(
    FILES
		"third_party/libxmlmm/include/*.h"
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libxmlmm)
	

