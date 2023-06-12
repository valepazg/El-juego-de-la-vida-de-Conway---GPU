set(HPP_LIST "${PROJECT_SOURCE_DIR}/include/GameOfLife/grid.hpp"
  "${PROJECT_SOURCE_DIR}/include/GameOfLife/cpu_serial.hpp"
  "${PROJECT_SOURCE_DIR}/include/GameOfLife/util.hpp"
  "${PROJECT_SOURCE_DIR}/include/GameOfLife/err_code.h"
  "${PROJECT_SOURCE_DIR}/include/GameOfLife/gpu_cuda.h"
) 

set(CPP_LIST  "cpu_serial.cpp"  "grid.cpp")

add_library(game_of_life_lib ${CPP_LIST} ${HEADER_LIST})
target_include_directories(game_of_life_lib PUBLIC ../include)



