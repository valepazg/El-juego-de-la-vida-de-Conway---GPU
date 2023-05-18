cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(COMWAY_GAME_OF_LIFE game_of_life)

##################################
############ PROJECT #############
##################################

project(${COMWAY_GAME_OF_LIFE})
add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(src)
add_subdirectory(extern)
#add_subdirectory(include)
add_subdirectory(tests)

###############
### GTEST #####

enable_testing()

target_link_libraries(${PROJECT_NAME} game_of_life_lib)
