
include(GNUInstallDirs)

add_library(${PROJECT_NAME} SHARED "")
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

add_compile_definitions(DBCPPP_EXPORT)

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/src
)

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
    PRIVATE ${src}
)

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME})

# install
install(
    TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(
    FILES
		${CMAKE_CURRENT_BINARY_DIR}/dbcppp_export.h
		$<TARGET_PROPERTY:${PROJECT_NAME},INTERFACE_SOURCES>
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/dbcppp)
	
