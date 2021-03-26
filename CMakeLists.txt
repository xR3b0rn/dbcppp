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
include_directories("third_party/libxml2/include")
include_directories("third_party/libxmlmm/include")
include_directories("third_party/cxxopts/include")
include_directories("third_party/Catch2/include")
include_directories("${CMAKE_BINARY_DIR}/third_party/libxml2")

set(LIBXML2_WITH_ICONV OFF)
set(LIBXML2_WITH_LZMA OFF)
set(LIBXML2_WITH_PYTHON OFF)
set(LIBXML2_WITH_ZLIB OFF)
set(LIBXML2_WITH_TESTS OFF)

add_subdirectory(third_party)
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(examples)

set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


add_library(libxmlmm SHARED "")
add_dependencies(libxmlmm LibXml2)
target_link_libraries(libxmlmm LibXml2)

file(GLOB header
    "third_party/libxmlmm/include/*.h"
)
file(GLOB src
    "third_party/libxmlmm/src/*.cpp"
)

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
    DIRECTORY "third_party/libxmlmm/include"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libxmlmm
    FILES_MATCHING PATTERN "*.h")