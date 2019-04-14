
add_executable(${PROJECT_NAME}_Test)
set_property(TARGET ${PROJECT_NAME}_Test PROPERTY CXX_STANDARD 17)
target_link_libraries(${PROJECT_NAME}_Test ${PROJECT_NAME} ${Boost_LIBRARIES})

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/src
	${CMAKE_BINARY_DIR}/src/Vector/DBC
)

file(GLOB header
    "*.h"
)
file(GLOB src
    "*.cpp"
)

target_sources(${PROJECT_NAME}_Test
    PUBLIC ${header}
    PRIVATE ${src}
)

add_dependencies(${PROJECT_NAME}_Test ${PROJECT_NAME})