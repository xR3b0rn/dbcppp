
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
configure_file (
    "${CMAKE_CURRENT_SOURCE_DIR}/Config.h.in"
    "${CMAKE_CURRENT_SOURCE_DIR}/Config.h"
)

add_executable(${PROJECT_NAME}_Test ${header} ${src})
set_property(TARGET ${PROJECT_NAME}_Test PROPERTY CXX_STANDARD 17)
add_dependencies(${PROJECT_NAME}_Test ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME}_Test ${PROJECT_NAME} ${Boost_LIBRARIES} ${llvm_libs})

add_custom_target(RunTests COMMAND $<TARGET_FILE:${PROJECT_NAME}_Test> "" DEPENDS ${PROJECT_NAME}_Test)
