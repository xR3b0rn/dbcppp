
include(GNUInstallDirs)
include(TestBigEndian)

add_library(${PROJECT_NAME} SHARED "")
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${LIBXML2_LIBRARIES} "libxmlmm")

add_compile_definitions(DBCPPP_EXPORT)

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/src
)

target_include_directories(
	  ${PROJECT_NAME}
	  PUBLIC
	  $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}/>
)

test_big_endian(is_big_endian)
if (is_big_endian)
    set(BYTE_ORDER Big)
else ()
    set(BYTE_ORDER Little)
endif ()
configure_file(EndianConfig.h.in ${CMAKE_CURRENT_SOURCE_DIR}/EndianConfig.h @ONLY)

file(GLOB header
    "*.h"
)
file(GLOB header_interface
    "../../include/dbcppp/*.h"
)
file(GLOB src
    "*.cpp"
)

target_sources(${PROJECT_NAME}
    PRIVATE ${header} 
    PRIVATE ${header_interface}
    PRIVATE ${src} DBCX3AdaptStructs.inl
)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${header_interface}")

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME})

# install
install(
    TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/dbcppp)
install(
    FILES
    $<TARGET_PROPERTY:${PROJECT_NAME},INTERFACE_SOURCES>
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/dbcppp)
	
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${Upstream_VERSION}
  COMPATIBILITY AnyNewerVersion
)

export(EXPORT ${PROJECT_NAME}Targets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Targets.cmake"
)
configure_file(cmake/${PROJECT_NAME}Config.cmake
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.cmake"
  @ONLY
)

set(ConfigPackageLocation ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
install(EXPORT ${PROJECT_NAME}Targets
  FILE
    ${PROJECT_NAME}Targets.cmake
  DESTINATION
    ${ConfigPackageLocation}
)
install(
  FILES
    cmake/${PROJECT_NAME}Config.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION
    ${ConfigPackageLocation}
  COMPONENT
    Devel
)