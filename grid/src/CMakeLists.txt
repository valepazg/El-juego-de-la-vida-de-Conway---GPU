set(HPP_LIST "${PROJECT_SOURCE_DIR}/include/GameOfLife/grid.hpp"
  "${PROJECT_SOURCE_DIR}/include/GameOfLife/cpu_serial.hpp"
  "${PROJECT_SOURCE_DIR}/include/GameOfLife/util.hpp"
  "${PROJECT_SOURCE_DIR}/include/GameOfLife/err_code.h"
  "${PROJECT_SOURCE_DIR}/include/GameOfLife/gpu_cuda.h"
) 

set(CPP_LIST "grid.cpp" "cpu_serial.cpp" "gpu_cuda.cu")

add_library(game_of_life_lib ${CPP_LIST} ${HEADER_LIST})
target_include_directories(game_of_life_lib PUBLIC ../include)

find_library(CUDART_LIBRARY cudart ${CMAKE_CUDA_IMPLICIT_LINK_DIRECTORIES})
enable_language(CUDA)
find_package(CUDAToolkit)


target_link_libraries(game_of_life_lib PRIVATE CUDA::cudart)
