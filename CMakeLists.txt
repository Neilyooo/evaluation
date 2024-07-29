cmake_minimum_required(VERSION 2.8.2)

# Project Name
project(evaluation)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
# Specify the C++ standard
ADD_COMPILE_OPTIONS(-std=c++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -g -pthread")

add_definitions(-DROOT_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/\")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fexceptions" )
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread -std=c++0x -std=c++17 -fexceptions")

message("Current CPU archtecture: ${CMAKE_SYSTEM_PROCESSOR}")

# Find required packages
find_package(yaml-cpp REQUIRED)
find_package(Python3 COMPONENTS Development REQUIRED)
find_package(PythonLibs REQUIRED)
find_package(Eigen3 REQUIRED)
find_path(MATPLOTLIB_CPP_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/lib/include/matplotlibcpp.h")

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/lib/include
    ${YAML_CPP_INCLUDE_DIR}
    ${MATPLOTLIB_CPP_INCLUDE_DIRS}
    ${Python3_INCLUDE_DIRS}
    ${EIGEN3_INCLUDE_DIRS}
)
# Link directories
# link_directories(${CMAKE_SOURCE_DIR}/lib)
# Source files
set(SOURCES
    src/main.cpp
    src/configer.cpp
    src/packer.cpp
    src/plotter.cpp
)

# Add executable and specify all the source files needed
# add_library(evaluation_lib STATIC ${SOURCES})
add_executable(evaluation src/main.cpp)

# Link libraries
target_link_libraries(evaluation evaluation_lib yaml-cpp Python3::Python ${PYTHON_LIBRARIES})

# Installation directives
# set(INSTALL_DIR "${CMAKE_SOURCE_DIR}/lib")
# install(TARGETS evaluation_lib DESTINATION ${INSTALL_DIR})
# install(DIRECTORY ${MATPLOTLIB_CPP_INCLUDE_DIRS} DESTINATION ${INSTALL_DIR}/include/matplotlibcpp)
# install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${INSTALL_DIR}/include)
# install(FILES ${YAML_CPP_LIBRARIES} DESTINATION ${INSTALL_DIR}/lib)
# install(FILES ${Python3_LIBRARIES} DESTINATION ${INSTALL_DIR}/lib)
# install(FILES ${EIGEN3_LIBRARIES} DESTINATION ${INSTALL_DIR}/lib)