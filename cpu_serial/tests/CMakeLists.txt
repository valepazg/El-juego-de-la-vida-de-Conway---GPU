add_executable(
  ComwayGameOfLifeTests
  testGrid.cpp
)


target_link_libraries(
  ComwayGameOfLifeTests
  gtest_main
  game_of_life_lib
)


include(GoogleTest)
gtest_discover_tests(ComwayGameOfLifeTests)


configure_file(matrix_ok.txt matrix_ok.txt COPYONLY)
configure_file(matrix_not_ok.txt matrix_not_ok.txt COPYONLY)
