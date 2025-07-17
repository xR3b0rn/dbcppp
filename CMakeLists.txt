cmake_minimum_required(VERSION 3.12)

project("dbcppp" VERSION 3.2.6)

# CONFIGURATION

set(CMAKE_CXX_STANDARD 17)
option(build_kcd "Enable support for KCD parsing" ON)
option(build_tools "Build dbcppp utility application" ON)

option(build_tests "Build tests" ON)
option(build_examples "Build examples" ON)


# DEPENDENCIES & Requirements

find_package(Boost)

if(NOT Boost_FOUND)
    message(WARNING "Boost not found. Using libdbcppp boost (third-party/boost)")
    include_directories("third-party/boost")
endif()

# kcd is an xml based can database
message("kcd enabled: ${build_kcd}")

if(build_kcd)
    add_compile_definitions(ENABLE_KCD)

    # LibXml2

    find_package(LibXml2)

    if (NOT LibXml2_FOUND)
        message(WARNING "LibXml2 was not found. Using libdbcppp LibXml2 (third-party/libxml2)")
        set(LIBXML2_WITH_ICONV OFF)
        set(LIBXML2_WITH_LZMA OFF)
        set(LIBXML2_WITH_PYTHON OFF)
        set(LIBXML2_WITH_ZLIB OFF)
        set(LIBXML2_WITH_TESTS OFF)
        add_subdirectory(third-party/libxml2)
    endif()

    # libxmlmm (no find package because it is certainly not installed

    add_subdirectory("third-party/libxmlmm")
    target_include_directories(libxmlmm PUBLIC third-party/libxmlmm/libxmlmm ${LIBXML2_INCLUDE_DIR})
endif()


# CREATE LIBRARY

file(GLOB include "include/dbcppp/*.h")
file(GLOB headers "src/*.h")
file(GLOB sources "src/*.cpp")

add_library(${PROJECT_NAME} SHARED ${include} ${headers} ${sources})


# CONFIGURE LIBRARY

if (build_kcd)
    target_link_libraries(${PROJECT_NAME} PUBLIC libxmlmm)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    SOVERSION ${PROJECT_VERSION}
    PUBLIC_HEADER "${include}"
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include/>
)

# INSTALL LIBRARY

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}-targets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include/dbcppp
)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
set(install_cmake_dir "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

configure_package_config_file(
    ${CMAKE_SOURCE_DIR}/cmake/Config.cmake.in
    ${CMAKE_BINARY_DIR}/cmake/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION ${install_cmake_dir}
)

write_basic_package_version_file(
    ${CMAKE_BINARY_DIR}/cmake/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${VERSION}
    COMPATIBILITY AnyNewerVersion
)

# Export targets for build tree
export(TARGETS ${PROJECT_NAME}
    FILE cmake/${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
)

# Export targets for install tree
install(EXPORT ${PROJECT_NAME}-targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${install_cmake_dir}
)

install(FILES
    ${CMAKE_BINARY_DIR}/cmake/${PROJECT_NAME}Config.cmake
    ${CMAKE_BINARY_DIR}/cmake/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${install_cmake_dir}
)

# ADDITIONAL: Tools, Tests & Examples

if (build_tools)
    add_subdirectory(third-party/cxxopts)
    add_subdirectory(tools/dbcppp)
endif()

if (build_tests)
  add_subdirectory(tests)
endif()

if (build_examples)
  add_subdirectory(examples)
endif()

# PACKAGE (useful for debugging install, use make package)

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY NO)
set(CPACK_VERBATIM_VARIABLES YES)
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_IGNORE_FILES \\.git/ build/ ".*~$")
include(CPack)
