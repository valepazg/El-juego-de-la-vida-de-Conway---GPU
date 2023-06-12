cmake_minimum_required(VERSION 3.16.1)
set(CMAKE_CXX_STANDARD 17)
set(COMWAY_GAME_OF_LIFE game_of_life)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    SET(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    SET_PROPERTY(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
        "MinSizeRel" "RelWithDebInfo")
endif()


##################################
############ PROJECT #############
##################################

if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
  set(CMAKE_CUDA_ARCHITECTURES 75)
endif()

project(${COMWAY_GAME_OF_LIFE} LANGUAGES CXX)

file(GLOB HEADERS include/GameOfLife/*.hpp)


add_executable(${PROJECT_NAME} main.cpp)


#target_link_libraries(game_of_life_opencl OpenCL::OpenCL)


add_subdirectory(src)
add_subdirectory(extern)
#add_subdirectory(include)
add_subdirectory(tests)

file(GLOB common_src "common/*.cpp")
###############
### GTEST #####

enable_testing()

target_link_libraries(${PROJECT_NAME} game_of_life_lib)
