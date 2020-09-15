
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/src
)

file(GLOB header
    "*.h"
)
file(GLOB src
    "*.cpp"
)

add_executable(dbcppp ${header} ${src})
set_property(TARGET dbcppp PROPERTY CXX_STANDARD 17)
add_dependencies(dbcppp ${PROJECT_NAME})
message(STATUS "Boost libraries")
message(STATUS ${Boost_LIBRARIES})
target_link_libraries(dbcppp ${PROJECT_NAME} ${Boost_LIBRARIES})

install(
    TARGETS dbcppp
    DESTINATION ${CMAKE_INSTALL_BINDIR})
    
