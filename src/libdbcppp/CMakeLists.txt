
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
    PUBLIC ${header_interface}
    PRIVATE ${src} DBCX3AdaptStructs.inl
)

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME})

# install
install(
    TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(
    FILES
		$<TARGET_PROPERTY:${PROJECT_NAME},INTERFACE_SOURCES>
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/dbcppp)
	
