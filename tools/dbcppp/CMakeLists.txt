cmake_minimum_required(VERSION 3.14)

add_executable(dbcppp main.cpp)
add_dependencies(dbcppp ${PROJECT_NAME})
target_link_libraries(dbcppp ${PROJECT_NAME} ${Boost_LIBRARIES} cxxopts)

install(
    TARGETS dbcppp
    DESTINATION ${CMAKE_INSTALL_BINDIR})
    
