cmake_minimum_required(VERSION 3.15)

project("libdbcppp" VERSION 0.1.0)

option(build_tests "Build tests" OFF)
option(build_examples "Build examples" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	add_definitions("/bigobj")
endif()

find_package(LibXml2 REQUIRED)

include_directories(
    include
    ${LIBXML2_INCLUDE_DIR}
    third-party/libxmlmm/libxmlmm)
    third-party/boost)
    third-party/cxxopts/include
)


file(GLOB libxmlmm_header
    "third-party/libxmlmm/libxmlmm/*.h"
)
file(GLOB libxmlmm_src
    "third-party/libxmlmm/libxmlmm/*.cpp"
)

add_library(libxmlmm SHARED "")
target_link_libraries(libxmlmm ${LIBXML2_LIBRARIES})

target_sources("libxmlmm"
    PRIVATE ${libxmlmm_header}
    PRIVATE ${libxmlmm_src}
)

# install
install(
    TARGETS "libxmlmm"
    DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(
    DIRECTORY "libxmlmm"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libxmlmm
    FILES_MATCHING PATTERN "*.h")
    
add_subdirectory(src)
if (build_tests)
  add_subdirectory(tests)
endif()
if (build_examples)
  add_subdirectory(examples)
endif()

set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
