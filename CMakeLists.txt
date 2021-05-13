
cmake_minimum_required(VERSION 3.12)

project("libdbcppp" VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	add_definitions("/bigobj")
endif()

include_directories("include")
include_directories("third-party/libxml2/include")
include_directories("third-party/libxmlmm/libxmlmm")
include_directories("third-party/boost")

set(LIBXML2_WITH_ICONV OFF)
set(LIBXML2_WITH_LZMA OFF)
set(LIBXML2_WITH_PYTHON OFF)
set(LIBXML2_WITH_ZLIB OFF)
set(LIBXML2_WITH_TESTS OFF)

add_subdirectory(third-party/libxml2)

file(GLOB libxmlmm_header
    "third-party/libxmlmm/libxmlmm/*.h"
)
file(GLOB libxmlmm_src
    "third-party/libxmlmm/libxmlmm/*.cpp"
)

add_library(libxmlmm SHARED "")
target_link_libraries(libxmlmm LibXml2)

target_sources("libxmlmm"
    PRIVATE ${libxmlmm_header}
    PRIVATE ${libxmlmm_src}
)

install(
    TARGETS "libxmlmm"
    DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(
    DIRECTORY "libxmlmm"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libxmlmm
    FILES_MATCHING PATTERN "*.h")
    
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(examples)

set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
