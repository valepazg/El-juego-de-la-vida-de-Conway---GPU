set(CPP_LIST "opencl_simple.cpp" "opencl_mod1.cpp" "opencl_mod2.cpp")
set(HPP_LIST
  "${PROJECT_SOURCE_DIR}/include/util.hpp"
  "${PROJECT_SOURCE_DIR}/include/err_code.h"
) 

add_library(game_of_life_lib ${CPP_LIST} ${HPP_LIST})
target_include_directories(game_of_life_lib PUBLIC ../include)

