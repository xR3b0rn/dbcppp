
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

add_executable(${PROJECT_NAME}_ExampleBasicUsage ${header} ${src})
set_target_properties(${PROJECT_NAME}_ExampleBasicUsage PROPERTIES LINKER_LANGUAGE CXX)
set_property(TARGET ${PROJECT_NAME}_ExampleBasicUsage PROPERTY CXX_STANDARD 17)
add_dependencies(${PROJECT_NAME}_ExampleBasicUsage ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME}_ExampleBasicUsage ${PROJECT_NAME} ${Boost_LIBRARIES})
