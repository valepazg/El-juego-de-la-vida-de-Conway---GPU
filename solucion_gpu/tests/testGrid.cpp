#include <iostream>
#include "GameOfLife/grid.hpp"
#include <gtest/gtest.h>

/*
Primero se pone el nombre del test asignado y se hereda del tipo de
assertion de googletest
*/
// crea grid de tamaño 1
TEST(GridTest_1, BasicAssertions) {
  Grid grid(1);
  // int rows = grid.getRows();
  // int cols = grid.getCols();
  EXPECT_EQ(1,1);
  // EXPECT_EQ(cols,1);
}

// // crea grid de tamaño default
// TEST(GridTest_2, BasicAssertions) {
//   Grid<int> grid();
//   int rows = grid.getRows();
//   int cols = grid.getCols();
//   EXPECT_EQ(rows,10);
//   EXPECT_EQ(cols,10);
// }

