cmake_minimum_required(VERSION 3.18.4)
set(CMAKE_CXX_STANDARD 17)
set(CUDA_COMWAY_GAME_OF_LIFE CudaGameOfLife)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(${CUDA_COMWAY_GAME_OF_LIFE} LANGUAGES CUDA CXX)

find_package(CUDA REQUIRED)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    SET(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    SET_PROPERTY(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
        "MinSizeRel" "RelWithDebInfo")
endif()

file(GLOB CUDA_SOURCES src/*.cu)
file(GLOB CPP_SOURCES src/*.cpp)

file(GLOB HEADERS include/GudaGameOfLife/*.hpp)

add_executable(grid src/grid.cpp ${CPP_SOURCES} ${HEADERS})
add_executable(cuda_gol_simple src/gpu_simple.cu ${CPP_SOURCES} ${HEADERS})
add_executable(cuda_gol_mode1 src/gpu_mode1.cu ${CPP_SOURCES} ${HEADERS})
add_executable(cuda_gol__mode2 src/gpu_mode2.cu ${CPP_SOURCES} ${HEADERS})


add_subdirectory(src)
add_subdirectory(extern)
add_subdirectory(tests)


enable_testing()

target_link_libraries(${PROJECT_NAME} CudaGameOfLife_lib)



