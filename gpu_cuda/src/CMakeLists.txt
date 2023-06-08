set(HPP_LIST "${PROJECT_SOURCE_DIR}/include/CudaGameOfLife/grid.hpp")
set(CPP_LIST "gpu_simple.cu"  "gpu_mode1.cu"   "gpu_mode2.cu")

add_library(CudaGameOfLife ${CPP_LIST} ${HPP_LIST})
target_include_directories(CudaGameOfLife PUBLIC ../include)

