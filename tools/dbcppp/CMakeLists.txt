cmake_minimum_required(VERSION 3.14)

project(dbcparser LANGUAGES CXX)

add_executable(${PROJECT_NAME} main.cpp)
add_dependencies(${PROJECT_NAME} dbcppp)
target_link_libraries(${PROJECT_NAME} dbcppp ${Boost_LIBRARIES} cxxopts)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
    
