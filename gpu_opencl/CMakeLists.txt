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


# Opencl add
find_package(OpenCL REQUIRED)
file(GLOB CPP_SOURCES src/*.cpp)

#find_package(OpenGL REQUIRED)
file(GLOB HEADERS include/*.hpp)
file(GLOB HEADERS include/*.h)

INCLUDE_DIRECTORIES(${OpenCL_INCLUDE_DIRS})

LINK_DIRECTORIES(${OpenCL_LIBRARY})

message(STATUS "OpenCL found: ${OPENCL_FOUND}")
message(STATUS "OpenCL includes: ${OPENCL_INCLUDE_DIRS}")
message(STATUS "OpenCL CXX includes: ${OPENCL_HAS_CPP_BINDINGS}")
message(STATUS "OpenCL libraries: ${OPENCL_LIBRARIES}")

add_executable(gol main.cpp ${HEADERS})

add_executable(opencl_gol_simple src/opencl_simple.cpp ${HEADERS})
add_executable(opencl_gol_mod1 src/opencl_mod1.cpp ${HEADERS})
add_executable(opencl_gol_mod2 src/opencl_mod2.cpp ${HEADERS})

#target_link_libraries(game_of_life_opencl OpenCL::OpenCL)

add_subdirectory(src)
##add_subdirectory(extern)
add_subdirectory(include)
##add_subdirectory(tests)

###############
### GTEST #####

enable_testing()

#target_link_libraries(${PROJECT_NAME} game_of_life_lib)
